
#ifndef COMMON_H
#define COMMON_H

#include <zephyr.h>
#include <drivers/gpio.h>
#include <arm_math.h>

#define DISPLAY_LED 1

#define TARGET_POSITION_TIMEOUT 25
#define TARGET_POSITION_PRIORITY 6
#define TARGET_POSITION_STACKSIZE 2048

#define RUN_PID_TIMEOUT 5
#define RUN_PID_PRIORITY 5
#define RUN_PID_STACKSIZE 2048

struct gpio_peripheral {
	struct gpio_dt_spec spec;
	const char *gpio_pin_name;
};

extern float32_t targetPosition;
extern struct k_mutex targetPositionMutex;

#endif /* COMMON_H */