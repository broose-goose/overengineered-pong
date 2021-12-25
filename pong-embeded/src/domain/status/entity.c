
#include "domain/status/entity.h"

#include <zephyr.h>

K_TIMER_DEFINE(network_timer, runBlinkyStatus, NULL);
struct statusLed network_led = {
    .timer_ = &network_timer,
    .spec_ = GPIO_DT_SPEC_GET_OR(NETWORK_LED_NODE, gpios, {0}),
	.gpio_pin_name_ = DT_PROP_OR(NETWORK_LED_NODE, label, ""),
};

K_TIMER_DEFINE(client_timer, runBlinkyStatus, NULL);
struct statusLed client_led = {
    .timer_ = &client_timer,
    .spec_ = GPIO_DT_SPEC_GET_OR(CLIENT_LED_NODE, gpios, {0}),
	.gpio_pin_name_ = DT_PROP_OR(CLIENT_LED_NODE, label, ""),
};

void runBlinkyStatus(struct k_timer *timer_id) {
    struct gpio_dt_spec *blinky_spec;
    if (timer_id == network_led.timer_) {
        blinky_spec = &network_led.spec_;
    } else if (timer_id == client_led.timer_){
        blinky_spec = &client_led.spec_;
    } else {
		return;
	}
    const int32_t out = !gpio_pin_get(blinky_spec->port, blinky_spec->pin);
	gpio_pin_set(blinky_spec->port, blinky_spec->pin, out);
}

bool enableStatusPins() {

    if (!device_is_ready(network_led.spec_.port)) {
		printk("Error: %s device is not ready\n", network_led.spec_.port->name);
		return false;
	}
    
    int32_t ret = gpio_pin_configure_dt(&network_led.spec_, GPIO_OUTPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure pin %d (NAME '%s')\n",
			ret, network_led.spec_.pin, network_led.gpio_pin_name_);
		return false;
	}

    if (!device_is_ready(client_led.spec_.port)) {
		printk("Error: %s device is not ready\n", client_led.spec_.port->name);
		return false;
	}
    
    ret = gpio_pin_configure_dt(&client_led.spec_, GPIO_OUTPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure pin %d (NAME '%s')\n",
			ret, client_led.spec_.pin, client_led.gpio_pin_name_);
		return false;
	}

    return true;
}