
#ifndef STATUS_ENTITY_H
#define STATUS_ENTITY_H

#include <drivers/gpio.h>
#include <devicetree/io-channels.h>

#define NETWORK_LED_NODE DT_ALIAS(led0)
#define CLIENT_LED_NODE DT_ALIAS(led1)
#define FRONTEND_LED_NODE DT_ALIAS(led2)

#if !DT_NODE_HAS_STATUS(NETWORK_LED_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif
#if !DT_NODE_HAS_STATUS(CLIENT_LED_NODE, okay)
#error "Unsupported board: led1 devicetree alias is not defined"
#endif

struct statusLed {
    struct k_timer *timer_;
	struct gpio_dt_spec spec_;
	const char *gpio_pin_name_;
};

extern struct statusLed network_led;
extern struct statusLed client_led;

bool enableStatusPins();
void runBlinkyStatus(struct k_timer *timer_id);

#endif /* STATUS_ENTITY_H */