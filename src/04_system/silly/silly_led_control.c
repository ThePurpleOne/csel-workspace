/**
 * Copyright 2018 University of Applied Sciences Western Switzerland / Fribourg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Project: HEIA-FR / HES-SO MSE - MA-CSEL1 Laboratory
 *
 * Abstract: System programming -  file system
 *
 * Purpose: NanoPi silly status led control system
 *
 * Autĥor:  Daniel Gachet
 * Date:    07.11.2018
 */
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>
#include <stdio.h>

/*
 * status led - gpioa.10 --> gpio10
 * power led  - gpiol.10 --> gpio362
 */
#define GPIO_EXPORT "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"
#define GPIO_LED "/sys/class/gpio/gpio10"
#define LED "10"

/*
 * push buttons
 * k1 - gpioa.0 --> gpio0  - increase frequency
 * k2 - gpioa.2 --> gpio2  - reset frequency
 * k3 - gpioa.3 --> gpio3  - decrease frequency
 */
#define GPIO_K1 "/sys/class/gpio/gpio0"
#define K1      "0"
#define GPIO_K2 "/sys/class/gpio/gpio2"
#define K2      "2"
#define GPIO_K3 "/sys/class/gpio/gpio3"
#define K3      "3"

#define FREQ_INIT 2   // Hz
#define FREQ_MIN  1   // Hz
#define FREQ_MAX  20  // Hz
#define FREQ_STEP 1   // Hz

static int open_led()
{
    // unexport pin out of sysfs (reinitialization)
    int f = open(GPIO_UNEXPORT, O_WRONLY);

    write(f, LED, strlen(LED));
    close(f);

    // export pin to sysfs
    f = open(GPIO_EXPORT, O_WRONLY);
    write(f, LED, strlen(LED));
    close(f);

    // config pin
    f = open(GPIO_LED "/direction", O_WRONLY);
    write(f, "out", 3);
    close(f);

    // open gpio value attribute
    f = open(GPIO_LED "/value", O_RDWR);
    return f;
}

static int open_button(const char* nr, const char* path)
{
    // unexport pin out of sysfs (reinitialization)
    int f = open(GPIO_UNEXPORT, O_WRONLY);
    write(f, nr, strlen(nr));
    close(f);

    // export pin to sysfs
    f = open(GPIO_EXPORT, O_WRONLY);
    write(f, nr, strlen(nr));
    close(f);

    // config pin as input
    char buf[64];
    snprintf(buf, sizeof(buf), "%s/direction", path);
    f = open(buf, O_WRONLY);
    write(f, "in", 2);
    close(f);

    // enable edge detection for both press and release
    snprintf(buf, sizeof(buf), "%s/edge", path);
    f = open(buf, O_WRONLY);
    write(f, "both", 4);
    close(f);

    // open gpio value attribute
    snprintf(buf, sizeof(buf), "%s/value", path);
    return open(buf, O_RDONLY | O_NONBLOCK);
}

// buttons are active-low: '0' = pressed, '1' = released
static int button_pressed(int fd)
{
    char val = '1';
    pread(fd, &val, 1, 0);
    return val == '0';
}

static void set_blink_timer(int tfd, int freq)
{
    long half_ns = 500000000L / freq;
    struct itimerspec ts = {
        .it_interval = {0, half_ns},
        .it_value    = {0, half_ns},
    };
    timerfd_settime(tfd, 0, &ts, NULL);
}

// returns 1 if frequency changed
static int apply_freq_change(int btn, int* freq)
{
    int prev = *freq;
    switch (btn) {
        case 0: if (*freq < FREQ_MAX) *freq += FREQ_STEP; break;
        case 1: *freq = FREQ_INIT; break;
        case 2: if (*freq > FREQ_MIN) *freq -= FREQ_STEP; break;
    }
    return *freq != prev;
}

int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    openlog("silly_led", LOG_PID | LOG_CONS, LOG_USER);

    int led_fd  = open_led();
    int k_fd[3] = {
        open_button(K1, GPIO_K1),
        open_button(K2, GPIO_K2),
        open_button(K3, GPIO_K3),
    };
    int blink_tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    int epfd      = epoll_create1(0);

    int freq      = FREQ_INIT;
    int led_state = 0;

    pwrite(led_fd, "0", 1, 0);
    set_blink_timer(blink_tfd, freq);
    syslog(LOG_INFO, "started, freq=%d Hz", freq);

    // consume initial POLLPRI state on button fds before entering epoll loop
    char dummy;
    for (int i = 0; i < 3; i++) pread(k_fd[i], &dummy, 1, 0);

    struct epoll_event ev = {0};

    ev.events = EPOLLIN; ev.data.fd = blink_tfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, blink_tfd, &ev);

    for (int i = 0; i < 3; i++) {
        ev.events = EPOLLPRI; ev.data.fd = k_fd[i];
        epoll_ctl(epfd, EPOLL_CTL_ADD, k_fd[i], &ev);
    }

    struct epoll_event events[8];
    while (1) {
        int n = epoll_wait(epfd, events, 8, -1);
        if (n < 0) {
            if (errno == EINTR) continue;
            break;
        }

        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;

            if (fd == blink_tfd) {
                uint64_t exp;
                read(blink_tfd, &exp, sizeof(exp));
                led_state ^= 1;
                pwrite(led_fd, led_state ? "1" : "0", 1, 0);

            } else {
                int btn = (fd == k_fd[0]) ? 0 : (fd == k_fd[1]) ? 1 : 2;
                if (button_pressed(fd)) {
                    if (apply_freq_change(btn, &freq)) {
                        set_blink_timer(blink_tfd, freq);
                        syslog(LOG_INFO, "k%d pressed: freq=%d Hz", btn + 1, freq);
                    }
                }
            }
        }
    }

    closelog();
    return 0;
}
