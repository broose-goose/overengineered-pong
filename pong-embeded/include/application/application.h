
#ifndef APPLICATION_H
#define APPLICATION_H

#include <zephyr.h>

typedef struct application {
    struct k_thread network_thread_;
    struct k_thread client_thread_;
};

void ApplicationStartup();

#endif /* APPLICATION_H */