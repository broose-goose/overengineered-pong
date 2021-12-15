
#include "common.h"

#include <math.h>

#include <drivers/adc.h>
#include <devicetree/io-channels.h>
#include <drivers/pwm.h>

#define LED1_NODE DT_ALIAS(led1)
#define DIR_MOTOR_NODE DT_ALIAS(dir_motor)
#define STP_MOTOR_NODE DT_ALIAS(stp_motor)


#if !DT_NODE_HAS_STATUS(LED1_NODE, okay)
#error "Unsupported board: led1 devicetree alias is not defined"
#endif
#if !DT_NODE_HAS_STATUS(DIR_MOTOR_NODE, okay)
#error "Unsupported board: dir motor devicetree alias is not defined"
#endif
#if !DT_NODE_HAS_STATUS(STP_MOTOR_NODE, okay)
#error "Unsupported board: stp motor devicetree alias is not defined"
#endif
#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "Unsupported board: io_channels devicetree alias is not defined"
#endif


const float32_t kp = 0.1;
const int32_t minEpsilon = 3;

static const struct gpio_peripheral led1 = {
	.spec = GPIO_DT_SPEC_GET_OR(LED1_NODE, gpios, {0}),
	.gpio_pin_name = DT_PROP_OR(LED1_NODE, label, ""),
};

static const struct gpio_peripheral dirMotor = {
	.spec = GPIO_DT_SPEC_GET_OR(DIR_MOTOR_NODE, gpios, {0}),
	.gpio_pin_name = DT_PROP_OR(DIR_MOTOR_NODE, label, ""),
};


#define PWM_CTLR	DT_PWMS_CTLR(STP_MOTOR_NODE)
#define PWM_CHANNEL	DT_PWMS_CHANNEL(STP_MOTOR_NODE)
#define PWM_FLAGS	DT_PWMS_FLAGS(STP_MOTOR_NODE)


#define ADC_NODE DT_PHANDLE(DT_PATH(zephyr_user), io_channels)
#define ADC_RESOLUTION		10
#define ADC_GAIN		ADC_GAIN_1
#define ADC_REFERENCE		ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME	ADC_ACQ_TIME_DEFAULT

uint32_t sample_buffer[1];

struct adc_channel_cfg channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	/* channel ID will be overwritten below */
	.channel_id = DT_IO_CHANNELS_INPUT_BY_IDX(DT_PATH(zephyr_user), 0),
	.differential = 0,
};

struct adc_sequence sequence = {
	/* individual channels will be added below */
	.channels    = 1,
	.buffer      = sample_buffer,
	/* buffer size in bytes, not number of samples */
	.buffer_size = sizeof(sample_buffer),
	.resolution  = ADC_RESOLUTION,
};


bool setupRunPid(
    const struct gpio_dt_spec *led_spec,
    const struct gpio_dt_spec *dir_motor_spec,
    const struct device *stp_motor_dev,
    const struct device *wpr_track_dev
) {
    
    /* setup LED */
	if (!device_is_ready(led_spec->port)) {
		printk("Error: %s device is not ready\n", led_spec->port->name);
		return false;
	}
    int32_t ret = gpio_pin_configure_dt(led_spec, GPIO_OUTPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure pin %d (NAME '%s')\n",
			ret, led_spec->pin, led1.gpio_pin_name);
		return false;
	}

    /* setup direction pin */
	if (!device_is_ready(dir_motor_spec->port)) {
		printk("Error: %s device is not ready\n", dir_motor_spec->port->name);
		return false;
	}
    ret = gpio_pin_configure_dt(dir_motor_spec, GPIO_OUTPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure pin %d (NAME '%s')\n",
			ret, dir_motor_spec->pin, dirMotor.gpio_pin_name);
		return false;
	}


    /* setup step pin */
    if (!device_is_ready(stp_motor_dev)) {
		printk("Error: PWM device %s is not ready\n", stp_motor_dev->name);
		return;
	}


    /* setup adc */
    if (!device_is_ready(wpr_track_dev)) {
		printk("ADC device not found\n");
		return;
	}
    adc_channel_setup(wpr_track_dev, &channel_cfg);


    return true;
}

uint8_t minPwm = 160;

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

uint32_t interpolatePwm(int32_t pwmVal, int32_t period) {
    return (uint32_t) period * ((pwmVal + minPwm) / 255.0f);
}

void MotorForward(
    int32_t pwmVal,
    const struct gpio_dt_spec *dirMotorSpec,
    const struct device *stpMotorDev
) {
    pwm_pin_set_nsec(stpMotorDev, PWM_CHANNEL, 40000, interpolatePwm(pwmVal, 40000), PWM_FLAGS);
    gpio_pin_set(dirMotorSpec->port, dirMotorSpec->pin, 0);
}

void MotorBackward(
    int32_t pwmVal,
    const struct gpio_dt_spec *dirMotorSpec,
    const struct device *stpMotorDev
) {
    
    pwm_pin_set_nsec(stpMotorDev, PWM_CHANNEL, 40000 , interpolatePwm(pwmVal, 40000), PWM_FLAGS);
    gpio_pin_set(dirMotorSpec->port, dirMotorSpec->pin, 1);
}

void MotorStop(
    const struct gpio_dt_spec *dirMotorSpec,
    const struct device *stpMotorDev
) {
    pwm_pin_set_nsec(stpMotorDev, PWM_CHANNEL, 45000 , 0, PWM_FLAGS);
    gpio_pin_set(dirMotorSpec->port, dirMotorSpec->pin, 1);
}

#define OVERSAMPLE 3

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to)
{
  if (from != to) {
    if (from > to) {
      value = (value < (uint32_t)(1 << (from - to))) ? 0 : ((value + 1) >> (from - to)) - 1;
    } else {
      if (value != 0) {
        value = ((value + 1) << (to - from)) - 1;
      }
    }
  }
  return value;
}

int AdcRead(
    const struct device *wpr_track_dev,
    const struct adc_sequence *seq,
    float32_t *currentPosition
) {
    uint32_t outputPosition = 0;
    int e;
    for (int i = 0; i < OVERSAMPLE; i++) {
        e = adc_read(wpr_track_dev, seq);
        if (e != 0) {
            return e;
        }
        outputPosition += sample_buffer[0];
    }
    outputPosition = mapResolution(outputPosition, 12, 10);
    (*currentPosition) = outputPosition;
    (*currentPosition) /= OVERSAMPLE;
    return 0;
}


void RunPid(void *p1, void *p2, void *p3) {
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    const struct gpio_dt_spec *led_spec = &led1.spec;
    const struct gpio_dt_spec *dir_motor_spec = &dirMotor.spec;
    const struct device *stp_motor_dev = DEVICE_DT_GET(PWM_CTLR);
    const struct device *wpr_track_dev = DEVICE_DT_GET(ADC_NODE);
    const bool ret = setupRunPid(led_spec, dir_motor_spec, stp_motor_dev, wpr_track_dev);

    if (ret == false) {
        return; 
    }

    float32_t currentPosition, err;
    int32_t pwmOut;
    bool lastStepMoved = false;
    uint8_t count = 0;
    int e;
    printk("Starting run pid\n");
    while(1) {
        k_msleep(RUN_PID_TIMEOUT);

        e = AdcRead(wpr_track_dev, &sequence, &currentPosition);
		if (e != 0) {
			printk("ADC reading failed with error %d.\n", e);
			return;
		}
        
        k_mutex_lock(&targetPositionMutex, K_FOREVER);
        err = targetPosition - currentPosition;
        k_mutex_unlock(&targetPositionMutex);

        pwmOut = (int32_t) (kp * err);
        const int32_t pwmOutAbs = fabs(pwmOut);
        if (pwmOutAbs < minEpsilon) {
            MotorStop(dir_motor_spec, stp_motor_dev);
            if (lastStepMoved) {
                // send current position to notifier queue
            }
            lastStepMoved = false;
        } else {
            if (pwmOut > 0) {
                MotorForward(pwmOutAbs, dir_motor_spec, stp_motor_dev);
            } else {
                MotorBackward(pwmOutAbs, dir_motor_spec, stp_motor_dev);
            }
            // send current position to notifier queue
            lastStepMoved = true;
        }

        // to prove i'm trying to do something, blink the light
        
        if (DISPLAY_LED != 0) {
            count = ++count % 4;
            if (count == 0) {
                const int32_t out = !gpio_pin_get(led_spec->port, led_spec->pin);
		        gpio_pin_set(led_spec->port, led_spec->pin, out);
            }
        }
    }
}