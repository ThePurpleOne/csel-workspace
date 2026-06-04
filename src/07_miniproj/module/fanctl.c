#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/thermal.h>
#include <linux/timer.h>
#include <linux/workqueue.h>

#define STATUS_LED_GPIO 10

#define FREQ_MIN 1
#define FREQ_MAX 20
#define FREQ_DEFAULT 2

#define TEMP_THRESHOLD_LOW 35000
#define TEMP_THRESHOLD_MID 40000
#define TEMP_THRESHOLD_HIGH 45000

#define FREQ_LOW 2
#define FREQ_MID 5
#define FREQ_HIGH 10
#define FREQ_MAX_AUTO 20

#define THERMAL_POLL_MS 1000

enum mode { MODE_MANUAL = 0, MODE_AUTO = 1 };

static enum mode current_mode = MODE_AUTO;
static int current_freq       = FREQ_DEFAULT;
static int current_temp_mC    = 0;
static int led_state          = 0;

static struct thermal_zone_device* tz = NULL;
static struct timer_list blink_timer;
static struct delayed_work thermal_work;
static struct class* fanctl_class;
static struct device* fanctl_device;

static unsigned long freq_to_jiffies(int freq)
{
    unsigned long period = HZ / (2 * freq);
    if (period == 0) period = 1;
    return period;
}

static void blink_callback(struct timer_list* t)
{
    led_state ^= 1;
    gpio_set_value(STATUS_LED_GPIO, led_state);
    mod_timer(&blink_timer, jiffies + freq_to_jiffies(current_freq));
}

static int compute_auto_freq(int temp_mC)
{
    if (temp_mC < TEMP_THRESHOLD_LOW) return FREQ_LOW;
    if (temp_mC < TEMP_THRESHOLD_MID) return FREQ_MID;
    if (temp_mC < TEMP_THRESHOLD_HIGH) return FREQ_HIGH;
    return FREQ_MAX_AUTO;
}

static void thermal_work_handler(struct work_struct* w)
{
    int temp = 0;

    if (tz != NULL) {
        if (thermal_zone_get_temp(tz, &temp) != 0) temp = 0;
    }

    current_temp_mC = temp;
    if (current_mode == MODE_AUTO) {
        current_freq = compute_auto_freq(temp);
    }

    schedule_delayed_work(&thermal_work, msecs_to_jiffies(THERMAL_POLL_MS));
}

static ssize_t mode_show(struct device* dev,
                         struct device_attribute* attr,
                         char* buf)
{
    return sprintf(buf, "%s\n",
                   (current_mode == MODE_AUTO) ? "auto" : "manual");
}

static ssize_t mode_store(struct device* dev,
                          struct device_attribute* attr,
                          const char* buf,
                          size_t count)
{
    if (sysfs_streq(buf, "auto")) {
        current_mode = MODE_AUTO;
        current_freq = compute_auto_freq(current_temp_mC);
    } else if (sysfs_streq(buf, "manual")) {
        current_mode = MODE_MANUAL;
    } else {
        return -EINVAL;
    }
    return count;
}
static DEVICE_ATTR_RW(mode);

static ssize_t frequency_show(struct device* dev,
                              struct device_attribute* attr,
                              char* buf)
{
    return sprintf(buf, "%d\n", current_freq);
}

static ssize_t frequency_store(struct device* dev,
                               struct device_attribute* attr,
                               const char* buf,
                               size_t count)
{
    int new_freq;
    if (kstrtoint(buf, 10, &new_freq) != 0) return -EINVAL;
    if (new_freq < FREQ_MIN || new_freq > FREQ_MAX) return -EINVAL;
    current_freq = new_freq;
    return count;
}
static DEVICE_ATTR_RW(frequency);

static ssize_t temperature_show(struct device* dev,
                                struct device_attribute* attr,
                                char* buf)
{
    return sprintf(buf, "%d\n", current_temp_mC);
}
static DEVICE_ATTR_RO(temperature);

static struct thermal_zone_device* lookup_cpu_zone(void)
{
    struct thermal_zone_device* z;

    z = thermal_zone_get_zone_by_name("cpu-thermal");
    if (!IS_ERR(z)) {
        pr_info("fanctl: using thermal zone 'cpu-thermal'\n");
        return z;
    }
    pr_warn("fanctl: no cpu thermal zone found, temperature will be 0\n");
    return NULL;
}

static int __init fanctl_init(void)
{
    int status;

    fanctl_class = class_create(THIS_MODULE, "fanctl");
    if (IS_ERR(fanctl_class)) return PTR_ERR(fanctl_class);

    fanctl_device = device_create(fanctl_class, NULL, 0, NULL, "fanctl");
    if (IS_ERR(fanctl_device)) {
        status = PTR_ERR(fanctl_device);
        goto err_class;
    }

    status = device_create_file(fanctl_device, &dev_attr_mode);
    if (status != 0) goto err_device;
    status = device_create_file(fanctl_device, &dev_attr_frequency);
    if (status != 0) goto err_mode;
    status = device_create_file(fanctl_device, &dev_attr_temperature);
    if (status != 0) goto err_freq;

    status = gpio_request(STATUS_LED_GPIO, "fanctl-status-led");
    if (status != 0) goto err_temp;
    status = gpio_direction_output(STATUS_LED_GPIO, 0);
    if (status != 0) goto err_gpio;

    tz = lookup_cpu_zone();

    INIT_DELAYED_WORK(&thermal_work, thermal_work_handler);
    schedule_delayed_work(&thermal_work, 0);

    timer_setup(&blink_timer, blink_callback, 0);
    mod_timer(&blink_timer, jiffies + freq_to_jiffies(current_freq));

    pr_info("fanctl: module loaded\n");
    return 0;

err_gpio:
    gpio_free(STATUS_LED_GPIO);
err_temp:
    device_remove_file(fanctl_device, &dev_attr_temperature);
err_freq:
    device_remove_file(fanctl_device, &dev_attr_frequency);
err_mode:
    device_remove_file(fanctl_device, &dev_attr_mode);
err_device:
    device_destroy(fanctl_class, 0);
err_class:
    class_destroy(fanctl_class);
    return status;
}

static void __exit fanctl_exit(void)
{
    del_timer(&blink_timer);
    cancel_delayed_work_sync(&thermal_work);

    gpio_set_value(STATUS_LED_GPIO, 0);
    gpio_free(STATUS_LED_GPIO);

    device_remove_file(fanctl_device, &dev_attr_temperature);
    device_remove_file(fanctl_device, &dev_attr_frequency);
    device_remove_file(fanctl_device, &dev_attr_mode);
    device_destroy(fanctl_class, 0);
    class_destroy(fanctl_class);

    pr_info("fanctl: module unloaded\n");
}

module_init(fanctl_init);
module_exit(fanctl_exit);

MODULE_AUTHOR("Nicolas Albanesi");
MODULE_DESCRIPTION("CSEL mini-project fan control (thermal + GPIO LED)");
MODULE_LICENSE("GPL");
