
#include "domain/status/service.h"


void StatusSetup() {
    bool has_setup = false;
    while (!has_setup) {
        has_setup = enableStatusPins();
    }
    StatusClientDown();
    StatusNetworkDown();
};



void StatusNetworkUp() {
    k_timer_stop(network_led.timer_);
    gpio_pin_set(network_led.spec_.port, network_led.spec_.pin, (int)true);
}
void StatusNetworkDown() {
    k_timer_stop(network_led.timer_);
    gpio_pin_set(network_led.spec_.port, network_led.spec_.pin, (int)false);
}
void StatusNetworkConnecting() {
    k_timer_start(network_led.timer_, K_NO_WAIT, K_MSEC(350));
}



void StatusClientUp() {
    k_timer_stop(client_led.timer_);
    gpio_pin_set(client_led.spec_.port, client_led.spec_.pin, (int)true);
}
void StatusClientDown() {
    k_timer_stop(client_led.timer_);
    gpio_pin_set(client_led.spec_.port, client_led.spec_.pin, (int)false);
}
void StatusClientConnecting() {
    k_timer_start(client_led.timer_, K_NO_WAIT, K_MSEC(350));
}
