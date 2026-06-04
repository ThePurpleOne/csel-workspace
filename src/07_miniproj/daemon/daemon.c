#include "buttons.h"
#include "comm.h"
#include "oled_view.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <syslog.h>
#include <unistd.h>

#define TCP_PORT 1337
#define OLED_REFRESH_MS 500
#define EPOLL_MAX_EVENTS 16

int main(void)
{
    openlog("fanctl-daemon", LOG_NDELAY | LOG_PID | LOG_PERROR, LOG_DAEMON);
    syslog(LOG_INFO, "starting");

    if (oled_view_init() != 0) {
        syslog(LOG_ERR, "OLED init failed");
        return 1;
    }
    if (buttons_init() != 0) {
        syslog(LOG_ERR, "buttons init failed");
        return 1;
    }
    if (comm_init(TCP_PORT) != 0) {
        syslog(LOG_ERR, "TCP listen failed on port %d", TCP_PORT);
        return 1;
    }
    syslog(LOG_INFO, "listening on TCP port %d", TCP_PORT);

    int refresh_fd        = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    struct itimerspec its = {
        .it_interval = {.tv_sec = 0, .tv_nsec = OLED_REFRESH_MS * 1000000L},
        .it_value    = {.tv_sec = 0, .tv_nsec = OLED_REFRESH_MS * 1000000L},
    };
    timerfd_settime(refresh_fd, 0, &its, NULL);

    int epfd              = epoll_create1(0);
    struct epoll_event ev = {0};

    for (int i = 0; i < N_BUTTONS; i++) {
        ev.events  = EPOLLPRI;
        ev.data.fd = buttons_fd(i);
        epoll_ctl(epfd, EPOLL_CTL_ADD, buttons_fd(i), &ev);
    }

    ev.events  = EPOLLIN;
    ev.data.fd = comm_listen_fd();
    epoll_ctl(epfd, EPOLL_CTL_ADD, comm_listen_fd(), &ev);

    ev.events  = EPOLLIN;
    ev.data.fd = refresh_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, refresh_fd, &ev);

    struct epoll_event events[EPOLL_MAX_EVENTS];
    for (;;) {
        int n = epoll_wait(epfd, events, EPOLL_MAX_EVENTS, -1);
        if (n < 0) break;

        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;

            int btn_idx = buttons_owns_fd(fd);
            if (btn_idx >= 0) {
                buttons_handle_event(btn_idx);
            } else if (fd == comm_listen_fd()) {
                comm_accept_client(epfd);
            } else if (fd == refresh_fd) {
                uint64_t exp;
                read(refresh_fd, &exp, sizeof(exp));
                oled_view_refresh();
            } else if (comm_owns_client(fd)) {
                comm_handle_client(fd, epfd);
            }
        }
    }

    syslog(LOG_INFO, "shutting down");
    comm_close(epfd);
    buttons_close();
    close(refresh_fd);
    close(epfd);
    closelog();
    return 0;
}
