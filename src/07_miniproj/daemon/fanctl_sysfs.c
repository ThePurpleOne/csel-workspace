#include "fanctl_sysfs.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define FANCTL_MODE "/sys/class/fanctl/fanctl/mode"
#define FANCTL_FREQ "/sys/class/fanctl/fanctl/frequency"
#define FANCTL_TEMP "/sys/class/fanctl/fanctl/temperature"

static int read_line(const char* path, char* buf, size_t buf_sz)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0) return -1;
    ssize_t n = read(fd, buf, buf_sz - 1);
    close(fd);
    if (n < 0) return -1;
    buf[n] = '\0';
    while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == ' ')) buf[--n] = '\0';
    return 0;
}

static int write_str(const char* path, const char* s)
{
    int fd = open(path, O_WRONLY);
    if (fd < 0) return -1;
    size_t len = strlen(s);
    ssize_t n  = write(fd, s, len);
    close(fd);
    return (n == (ssize_t)len) ? 0 : -1;
}

int fanctl_read_mode(char* buf, size_t buf_sz)
{
    return read_line(FANCTL_MODE, buf, buf_sz);
}

int fanctl_read_freq(int* freq)
{
    char buf[16];
    if (read_line(FANCTL_FREQ, buf, sizeof(buf)) != 0) return -1;
    *freq = atoi(buf);
    return 0;
}

int fanctl_read_temp_mC(int* temp)
{
    char buf[16];
    if (read_line(FANCTL_TEMP, buf, sizeof(buf)) != 0) return -1;
    *temp = atoi(buf);
    return 0;
}

int fanctl_write_mode(const char* mode) { return write_str(FANCTL_MODE, mode); }

int fanctl_write_freq(int freq)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", freq);
    return write_str(FANCTL_FREQ, buf);
}
