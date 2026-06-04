#include "comm.h"
#include "fanctl_sysfs.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define TCP_BACKLOG 8
#define MAX_CLIENTS 8
#define CLIENT_BUF_SZ 256

#define FREQ_MIN 1
#define FREQ_MAX 20

struct client {
    int fd;
    char buf[CLIENT_BUF_SZ];
    size_t len;
};

static int listen_fd = -1;
static struct client clients[MAX_CLIENTS];

static struct client* client_alloc(int fd)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].fd < 0) {
            clients[i].fd  = fd;
            clients[i].len = 0;
            return &clients[i];
        }
    }
    return NULL;
}

static struct client* client_lookup(int fd)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
        if (clients[i].fd == fd) return &clients[i];
    return NULL;
}

static void client_free(struct client* c, int epfd)
{
    epoll_ctl(epfd, EPOLL_CTL_DEL, c->fd, NULL);
    close(c->fd);
    c->fd  = -1;
    c->len = 0;
}

static void send_line(int fd, const char* s)
{
    size_t len = strlen(s);
    send(fd, s, len, MSG_NOSIGNAL);
    if (len == 0 || s[len - 1] != '\n') send(fd, "\n", 1, MSG_NOSIGNAL);
}

static void handle_command(int fd, char* line)
{
    while (*line == ' ' || *line == '\t') line++;
    size_t len = strlen(line);
    while (len > 0 && (line[len - 1] == '\r' || line[len - 1] == ' ')) {
        line[--len] = '\0';
    }
    if (len == 0) return;

    if (strncasecmp(line, "STATUS", 6) == 0
        && (line[6] == '\0' || line[6] == ' ')) {
        char mode[32];
        int freq, temp;
        if (fanctl_read_mode(mode, sizeof(mode)) != 0
            || fanctl_read_freq(&freq) != 0
            || fanctl_read_temp_mC(&temp) != 0) {
            send_line(fd, "ERR sysfs read failed");
            return;
        }
        char resp[128];
        snprintf(resp, sizeof(resp), "mode=%s freq=%d temp=%d", mode, freq, temp);
        send_line(fd, resp);
        return;
    }

    if (strncasecmp(line, "MODE ", 5) == 0) {
        const char* arg = line + 5;
        if (strcasecmp(arg, "AUTO") == 0) {
            send_line(fd, fanctl_write_mode("auto") == 0 ? "OK"
                                                        : "ERR cannot set mode");
        } else if (strcasecmp(arg, "MANUAL") == 0) {
            send_line(fd, fanctl_write_mode("manual") == 0
                              ? "OK"
                              : "ERR cannot set mode");
        } else {
            send_line(fd, "ERR invalid mode");
        }
        return;
    }

    if (strncasecmp(line, "FREQ ", 5) == 0) {
        char* endp;
        long v = strtol(line + 5, &endp, 10);
        if (*endp != '\0' || v < FREQ_MIN || v > FREQ_MAX) {
            send_line(fd, "ERR freq out of range");
            return;
        }
        send_line(fd, fanctl_write_freq((int)v) == 0 ? "OK"
                                                    : "ERR cannot set freq");
        return;
    }

    if (strncasecmp(line, "QUIT", 4) == 0) {
        send_line(fd, "BYE");
        shutdown(fd, SHUT_RDWR);
        return;
    }

    send_line(fd, "ERR unknown command");
}

int comm_init(int port)
{
    for (int i = 0; i < MAX_CLIENTS; i++) clients[i].fd = -1;

    int s = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (s < 0) return -1;

    int one = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    struct sockaddr_in addr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port        = htons(port),
    };
    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(s);
        return -1;
    }
    if (listen(s, TCP_BACKLOG) < 0) {
        close(s);
        return -1;
    }
    listen_fd = s;
    return 0;
}

void comm_close(int epfd)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].fd >= 0) {
            epoll_ctl(epfd, EPOLL_CTL_DEL, clients[i].fd, NULL);
            close(clients[i].fd);
            clients[i].fd = -1;
        }
    }
    if (listen_fd >= 0) {
        close(listen_fd);
        listen_fd = -1;
    }
}

int comm_listen_fd(void) { return listen_fd; }

int comm_accept_client(int epfd)
{
    int cfd = accept4(listen_fd, NULL, NULL, SOCK_NONBLOCK);
    if (cfd < 0) return -1;
    struct client* c = client_alloc(cfd);
    if (c == NULL) {
        close(cfd);
        return -1;
    }
    struct epoll_event ev = {.events = EPOLLIN, .data.fd = cfd};
    epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
    return cfd;
}

int comm_owns_client(int fd) { return client_lookup(fd) != NULL; }

void comm_handle_client(int fd, int epfd)
{
    struct client* c = client_lookup(fd);
    if (c == NULL) return;

    for (;;) {
        ssize_t n = read(c->fd, c->buf + c->len, sizeof(c->buf) - c->len);
        if (n < 0) {
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) return;
            client_free(c, epfd);
            return;
        }
        if (n == 0) {
            client_free(c, epfd);
            return;
        }
        c->len += n;

        for (;;) {
            char* nl = memchr(c->buf, '\n', c->len);
            if (nl == NULL) {
                if (c->len == sizeof(c->buf)) {
                    send_line(c->fd, "ERR line too long");
                    c->len = 0;
                }
                break;
            }
            *nl       = '\0';
            size_t ll = nl - c->buf;
            handle_command(c->fd, c->buf);
            size_t rest = c->len - (ll + 1);
            if (rest > 0) memmove(c->buf, c->buf + ll + 1, rest);
            c->len = rest;
        }
    }
}
