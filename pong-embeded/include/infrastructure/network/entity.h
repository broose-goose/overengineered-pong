
#ifndef NETWORK_ENTITY_H
#define NETWORK_ENTITY_H

#include <zephyr.h>
#include <net/net_mgmt.h>

typedef enum {
    NETWORK_STATE_WAITING_NET = 0,
    NETWORK_STATE_WAITING_DHCP = 1,
    NETWORK_STATE_UP = 2,
} networkState;

typedef enum {
    NETWORK_EVENT_ETHERNET_UP = 0,
    NETWORK_EVENT_ETHERNET_DOWN = 1,
    NETWORK_EVENT_DHCP_UP = 2,
    NETWORK_EVENT_DHCP_DOWN = 3,
} networkEvent;

typedef struct networkEventMessage {
    void *fifo_reserved; /* 1st word reserved for use by fifo */
    networkEvent network_event_;
};

typedef struct network {
    struct k_fifo bus_;
    networkState state_;
    struct net_mgmt_event_callback iface_callback_;
    struct net_mgmt_event_callback dhcp_callback_;
};

void networkSendEvent(networkEvent);

void networkStartup();
void networkRun();

#endif /* NETWORK_ENTITY_H */