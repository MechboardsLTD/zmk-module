
#include <zephyr/drivers/hwinfo.h>
#include <stdio.h>
#include <string.h>
#include <zmk/ble.h>

#define MAX_NAME_LEN 16
#define NAME_BUF_LEN (MAX_NAME_LEN + 1)

static int set_ble_name_from_hwinfo(void)
{
    uint8_t id[16];
    ssize_t hwlen = hwinfo_get_device_id(id, sizeof(id));

    if (hwlen <= 0) {
        return -EIO;
    }

    char suffix[7];
    snprintf(suffix, sizeof(suffix), "%02X%02X%02X", id[0], id[1], id[2]);

    char name[NAME_BUF_LEN];
    snprintf(name, sizeof(name), "Merzigo-%s", suffix);
    name[NAME_BUF_LEN - 1] = '\0';

    zmk_ble_set_device_name(name);
    return 0;
}

static int device_name_init(void)
{
    return set_ble_name_from_hwinfo();
}

SYS_INIT(device_name_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
