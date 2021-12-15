
#ifndef APPLICATION_H
#define APPLICATION_H

#include <zephyr.h>

typedef enum {
    WAITING_NET = 0,
    WAITING_DHCP = 1,
    WAITING_SOCKET = 2,
    RUNNING = 3,
} ApplicationState;

typedef struct Application{
    ApplicationState state_;
    struct k_mutex state_change_lock_;
};

extern struct Application MainApplication;

void ApplicationStartup();


#endif /* APPLICATION_H */