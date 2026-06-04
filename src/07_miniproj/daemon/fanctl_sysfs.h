#ifndef FANCTL_SYSFS_H
#define FANCTL_SYSFS_H

#include <stddef.h>

int fanctl_read_mode(char* buf, size_t buf_sz);
int fanctl_read_freq(int* freq);
int fanctl_read_temp_mC(int* temp);

int fanctl_write_mode(const char* mode);
int fanctl_write_freq(int freq);

#endif
