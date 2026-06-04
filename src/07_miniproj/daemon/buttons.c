#include "buttons.h"
#include "fanctl_sysfs.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#define GPIO_EXPORT "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"

#define POWER_LED_NR "362"
#define POWER_LED_PATH "/sys/class/gpio/gpio362"
#define POWER_LED_BASELINE 0

#define FREQ_MIN 1
#define FREQ_MAX 20

struct btn_def {
    const char* nr;
    const char* path;
};

static const struct btn_def btn_defs[N_BUTTONS] = {
    {"0", "/sys/class/gpio/gpio0"},
    {"2", "/sys/class/gpio/gpio2"},
    {"3", "/sys/class/gpio/gpio3"},
};

static int btn_fds[N_BUTTONS]   = {-1, -1, -1};
static int btn_state[N_BUTTONS] = {0, 0, 0};
static int led_fd               = -1;

static int export_dir(const char* nr, const char* path, const char* dir)
{
    int f = open(GPIO_UNEXPORT, O_WRONLY);
    if (f >= 0) {
        write(f, nr, strlen(nr));
        close(f);
    }

    f = open(GPIO_EXPORT, O_WRONLY);
    if (f < 0) return -1;
    write(f, nr, strlen(nr));
    close(f);

    char buf[128];
    snprintf(buf, sizeof(buf), "%s/direction", path);
    f = open(buf, O_WRONLY);
    if (f < 0) return -1;
    write(f, dir, strlen(dir));
    close(f);
    return 0;
}

static int open_button(const char* nr, const char* path)
{
    if (export_dir(nr, path, "in") != 0) return -1;

    char buf[128];
    snprintf(buf, sizeof(buf), "%s/edge", path);
    int f = open(buf, O_WRONLY);
    if (f < 0) return -1;
    write(f, "both", 4);
    close(f);

    snprintf(buf, sizeof(buf), "%s/value", path);
    return open(buf, O_RDONLY | O_NONBLOCK);
}

static int open_output(const char* nr, const char* path)
{
    if (export_dir(nr, path, "out") != 0) return -1;
    char buf[128];
    snprintf(buf, sizeof(buf), "%s/value", path);
    return open(buf, O_RDWR);
}

static int is_pressed(int fd)
{
    char val = '1';
    pread(fd, &val, 1, 0);
    return val == '0';
}

static void led_write(int fd, int state)
{
    pwrite(fd, state ? "1" : "0", 1, 0);
}

static void update_power_led(void)
{
    int any = btn_state[0] || btn_state[1] || btn_state[2];
    led_write(led_fd, POWER_LED_BASELINE ^ any);
}

int buttons_init(void)
{
    for (int i = 0; i < N_BUTTONS; i++) {
        btn_fds[i] = open_button(btn_defs[i].nr, btn_defs[i].path);
        if (btn_fds[i] < 0) return -1;
    }

    led_fd = open_output(POWER_LED_NR, POWER_LED_PATH);
    if (led_fd < 0) return -1;
    led_write(led_fd, POWER_LED_BASELINE);

    char dummy;
    for (int i = 0; i < N_BUTTONS; i++) pread(btn_fds[i], &dummy, 1, 0);
    return 0;
}

void buttons_close(void)
{
    if (led_fd >= 0) {
        led_write(led_fd, 0);
        close(led_fd);
        led_fd = -1;
    }
    for (int i = 0; i < N_BUTTONS; i++) {
        if (btn_fds[i] >= 0) {
            close(btn_fds[i]);
            btn_fds[i] = -1;
        }
    }
}

int buttons_fd(int idx)
{
    if (idx < 0 || idx >= N_BUTTONS) return -1;
    return btn_fds[idx];
}

int buttons_owns_fd(int fd)
{
    for (int i = 0; i < N_BUTTONS; i++)
        if (btn_fds[i] == fd) return i;
    return -1;
}

void buttons_handle_event(int idx)
{
    int pressed = is_pressed(btn_fds[idx]);
    int was     = btn_state[idx];
    btn_state[idx] = pressed;

    update_power_led();

    if (!pressed || was) return;

    int f;
    if (fanctl_read_freq(&f) != 0) return;

    switch (idx) {
        case 0:
            if (f < FREQ_MAX) f++;
            fanctl_write_freq(f);
            fanctl_write_mode("manual");
            syslog(LOG_INFO, "K1: freq=%d (manual)", f);
            break;
        case 1:
            if (f > FREQ_MIN) f--;
            fanctl_write_freq(f);
            fanctl_write_mode("manual");
            syslog(LOG_INFO, "K2: freq=%d (manual)", f);
            break;
        case 2: {
            char mode[32];
            if (fanctl_read_mode(mode, sizeof(mode)) != 0) return;
            fanctl_write_mode(strcmp(mode, "auto") == 0 ? "manual" : "auto");
            syslog(LOG_INFO, "K3: mode toggled");
            break;
        }
    }
}
