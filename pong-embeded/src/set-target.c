
#include "set-target.h"

#include "common.h"


#define LED0_NODE DT_ALIAS(led0)
#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

const float32_t minPosition = 0.0f;
const float32_t maxPosition = 1024.0f;
const float32_t rangePosition = maxPosition - minPosition;
const float32_t halfPosition = rangePosition * 0.5f + minPosition;
const float32_t halfPositionOffset = halfPosition - minPosition;

static const struct gpio_peripheral led0 = {
	.spec = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, {0}),
	.gpio_pin_name = DT_PROP_OR(LED0_NODE, label, ""),
};

void SetTargetPositionLoop(void *p1, void *p2, void *p3) {
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    const struct gpio_dt_spec *spec = &led0.spec;
    
	if (!device_is_ready(spec->port)) {
		printk("Error: %s device is not ready\n", spec->port->name);
		return;
	}

    const int32_t ret = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure pin %d (LED '%s')\n",
			ret, spec->pin, led0.gpio_pin_name);
		return;
	}

    float32_t lastTimestamp = k_uptime_get();
    uint8_t count = 0;
    while(1) {
        // this might end up with int percision because of k_uptime_get...
        // not sure
        const float32_t seconds = k_uptime_get() / 1000.0f;

        k_mutex_lock(&targetPositionMutex, K_FOREVER);
        targetPosition = arm_sin_f32(seconds * PI) * halfPositionOffset + halfPosition;
        k_mutex_unlock(&targetPositionMutex);

        // to prove i'm trying to do something, blink the light
        if (DISPLAY_LED != 0) {
            const int32_t out = !gpio_pin_get(spec->port, spec->pin);
		    gpio_pin_set(spec->port, spec->pin, out);
        }

        k_msleep(TARGET_POSITION_TIMEOUT);
    }
}