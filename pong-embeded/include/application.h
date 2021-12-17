
#ifndef APPLICATION_H
#define APPLICATION_H

#include <zephyr.h>

#include <net/net_mgmt.h>

typedef enum {
    APP_WAITING_NET = 0,
    APP_WAITING_DHCP = 1,
    APP_RUNNING = 2,
} ApplicationState;

typedef enum {
    EVENT_ETHERNET_UP = 0,
    EVENT_ETHERNET_DOWN = 1,
    EVENT_DHCP_UP = 2,
    EVENT_DHCP_DOWN = 3,
    EVENT_RUNNING_ERROR = 4,
} ApplicationEvent;

struct ApplicationEventMessage {
    void *fifo_reserved; /* 1st word reserved for use by fifo */
    ApplicationEvent application_event_;
};

typedef struct Application {
    ApplicationState state_;
    struct k_mutex state_change_lock_;
    struct k_fifo bus_;
    struct net_mgmt_event_callback iface_callback_;
    struct net_mgmt_event_callback dhcp_callback_;
};

extern struct Application MainApplication;

void ApplicationStartup();

void ApplicationRun();

void ApplicationSendEvent(ApplicationEvent e);

void applicationIfaceCallback(
    struct net_mgmt_event_callback *cb,
	uint32_t mgmt_event,
	struct net_if *iface
);
void applicationDhcpCallback(
    struct net_mgmt_event_callback *cb,
	uint32_t mgmt_event,
	struct net_if *iface
);
void applicationProcessEvents();
void applicationHandleEvent(ApplicationEvent e);

void applicationStopDhcpState();
void applicationStartDhcpState();
void applicationResetDhcpState();

void applicationStopRunningState();
void applicationStartRunningState();
void applicationResetRunningState();


#endif /* APPLICATION_H */