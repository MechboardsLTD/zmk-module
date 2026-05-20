#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led.h>

#include <zmk/events/layer_state_changed.h>
#include <zmk/events/activity_state_changed.h>
#include <zmk/keymap.h>

BUILD_ASSERT(DT_NODE_EXISTS(DT_ALIAS(led_status)),
             "An alias for a status LED was not found");

#define LED_GPIO_NODE_ID DT_COMPAT_GET_ANY_STATUS_OKAY(gpio_leds)


void set_status_led(void){
    if (zmk_keymap_highest_layer_active() > 0){ 
        led_on(DEVICE_DT_GET(LED_GPIO_NODE_ID), DT_NODE_CHILD_IDX(DT_ALIAS(led_status)));
    } else {
        led_off(DEVICE_DT_GET(LED_GPIO_NODE_ID), DT_NODE_CHILD_IDX(DT_ALIAS(led_status)));
    }
}

static int layer_listener(const zmk_event_t *eh) {
    const struct zmk_layer_state_changed *ev = as_zmk_layer_state_changed(eh);
    if (ev != NULL) {
        set_status_led();
    }
    return 0;
}
ZMK_LISTENER(layer_led, layer_listener);
ZMK_SUBSCRIPTION(layer_led, zmk_layer_state_changed);

static int sleep_listener(const zmk_event_t *eh) {
    const struct zmk_activity_state_changed  *ev = as_zmk_activity_state_changed(eh);
    if (ev != NULL) {
        switch (ev->state) {
        case ZMK_ACTIVITY_SLEEP:
            led_off(DEVICE_DT_GET(LED_GPIO_NODE_ID), DT_NODE_CHILD_IDX(DT_ALIAS(led_status)));
        default:
            break;
        }
    }
    return 0;
}

ZMK_LISTENER(sleep_led, sleep_listener);
ZMK_SUBSCRIPTION(sleep_led, zmk_activity_state_changed);