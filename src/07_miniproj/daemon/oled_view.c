#include "oled_view.h"
#include "fanctl_sysfs.h"
#include "ssd1306.h"

#include <stdio.h>
#include <string.h>

static char last_mode[8] = "";
static int last_freq     = -1;
static int last_temp     = -1;

int oled_view_init(void)
{
    if (ssd1306_init() != 0) return -1;
    ssd1306_set_position(0, 0);
    ssd1306_puts("CSEL Mini-Proj  ");
    ssd1306_set_position(0, 1);
    ssd1306_puts("----------------");
    return 0;
}

void oled_view_refresh(void)
{
    char mode[8];
    int freq, temp;

    if (fanctl_read_mode(mode, sizeof(mode)) != 0) return;
    if (fanctl_read_freq(&freq) != 0) return;
    if (fanctl_read_temp_mC(&temp) != 0) return;

    if (strcmp(mode, last_mode) == 0 && freq == last_freq && temp == last_temp)
        return;

    strncpy(last_mode, mode, sizeof(last_mode) - 1);
    last_mode[sizeof(last_mode) - 1] = '\0';
    last_freq = freq;
    last_temp = temp;

    char buf[32];

    ssd1306_set_position(0, 2);
    snprintf(buf, sizeof(buf), "Mode: %-8s", mode);
    ssd1306_puts(buf);

    ssd1306_set_position(0, 3);
    snprintf(buf, sizeof(buf), "Temp: %3d'C   ", temp / 1000);
    ssd1306_puts(buf);

    ssd1306_set_position(0, 4);
    snprintf(buf, sizeof(buf), "Freq: %2d Hz   ", freq);
    ssd1306_puts(buf);
}
