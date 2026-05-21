#define _GNU_SOURCE
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

static void sig_handler(int sig)
{
    // write() is async-signal-safe, printf() is not
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "signal %d received, ignoring\n", sig);
    write(STDOUT_FILENO, buf, n);
}

static void setup_signals(void)
{
    struct sigaction sa = {
        .sa_handler = sig_handler,
        .sa_flags   = 0,
    };
    sigemptyset(&sa.sa_mask);

    sigaction(SIGHUP,  &sa, NULL);
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

static void set_cpu(int core)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(core, &set);
    sched_setaffinity(0, sizeof(set), &set);
}

int main(void)
{
    int sv[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

    pid_t pid = fork();

    if (pid > 0) {
        // parent: uses sv[0], core 0
        close(sv[1]);
        set_cpu(0);
        setup_signals();

        char buf[256];
        while (1) {
            ssize_t n = read(sv[0], buf, sizeof(buf) - 1);
            if (n < 0) {
                if (errno == EINTR) continue;  // signal interrupted, retry
                break;
            }
            if (n == 0) break;  // child closed the socket
            buf[n] = '\0';
            printf("[parent] received: %s\n", buf);
            if (strcmp(buf, "exit") == 0) break;
        }

        close(sv[0]);
        wait(NULL);

    } else {
        // child: uses sv[1], core 1
        close(sv[0]);
        set_cpu(1);
        setup_signals();

        char buf[256];
        while (1) {
            ssize_t n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
            if (n < 0) {
                if (errno == EINTR) continue;  // signal interrupted, retry
                break;
            }
            if (n == 0) break;  // EOF
            buf[n] = '\0';
            buf[strcspn(buf, "\n")] = '\0';  // strip trailing newline
            write(sv[1], buf, strlen(buf));
            if (strcmp(buf, "exit") == 0) break;
        }

        close(sv[1]);
        exit(0);
    }

    return 0;
}
