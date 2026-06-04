#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 1337
#define BUF_SIZE 256

static void usage(const char* prog)
{
    fprintf(stderr,
            "Usage: %s <command> [args...]\n"
            "Commands:\n"
            "  STATUS                    -> mode=... freq=... temp=...\n"
            "  MODE AUTO | MODE MANUAL   -> set operating mode\n"
            "  FREQ <n>                  -> set blink frequency (1..20 Hz)\n"
            "  QUIT                      -> close server connection\n", prog);
}

static int connect_server(const char* host, int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port   = htons(port),
    };
    if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
        fprintf(stderr, "invalid host: %s\n", host);
        close(fd);
        return -1;
    }

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(fd);
        return -1;
    }
    return fd;
}

static int send_command(int fd, int argc, char** argv)
{
    char line[BUF_SIZE];
    size_t off = 0;

    for (int i = 0; i < argc; i++) {
        size_t len = strlen(argv[i]);
        if (off + len + 2 >= sizeof(line)) {
            fprintf(stderr, "command too long\n");
            return -1;
        }
        if (i > 0) line[off++] = ' ';
        memcpy(line + off, argv[i], len);
        off += len;
    }
    line[off++] = '\n';

    if (write(fd, line, off) != (ssize_t)off) {
        perror("write");
        return -1;
    }
    return 0;
}

static int read_response(int fd, char* out, size_t out_sz)
{
    size_t off = 0;
    while (off + 1 < out_sz) {
        ssize_t n = read(fd, out + off, out_sz - 1 - off);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("read");
            return -1;
        }
        if (n == 0) break;
        off += n;
        if (memchr(out, '\n', off) != NULL) break;
    }
    out[off] = '\0';
    return (int)off;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        usage(argv[0]);
        return 2;
    }

    int fd = connect_server(DEFAULT_HOST, DEFAULT_PORT);
    if (fd < 0) return 1;

    if (send_command(fd, argc - 1, argv + 1) != 0) {
        close(fd);
        return 1;
    }

    char resp[BUF_SIZE];
    int n = read_response(fd, resp, sizeof(resp));
    close(fd);

    if (n <= 0) {
        fprintf(stderr, "no response from server\n");
        return 1;
    }

    fputs(resp, stdout);
    if (resp[n - 1] != '\n') fputc('\n', stdout);

    return (strncmp(resp, "ERR", 3) == 0) ? 1 : 0;
}
