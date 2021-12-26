
#include "infrastructure/network/entity.h"

#include "infrastructure/network/net.h"

#include "infrastructure/bus/network.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(network_entity, LOG_LEVEL_WRN);

#include <net/net_if.h>

static struct network network_instance;

void networkSendEvent(networkEvent e) {

    struct networkEventMessage tx_event = { .network_event_ = e };
    
    size_t size = sizeof(struct networkEventMessage);
	char *mem_ptr = k_malloc(size);
	__ASSERT_NO_MSG(mem_ptr != 0);

	memcpy(mem_ptr, &tx_event, size);

    k_fifo_put(&network_instance.bus_, mem_ptr);
}

static void networkStopDhcpState() {
    LOG_INF("Stoping DHCP");
    struct net_if *iface;
    iface = net_if_get_default();
    net_dhcpv4_stop(iface);
}
static void networkStartDhcpState() {
    LOG_INF("Starting DHCP");
    struct net_if *iface;
    iface = net_if_get_default();
    net_dhcpv4_start(iface);
}
static void networkResetDhcpState() {
    LOG_INF("Reseting DHCP");
    networkStopDhcpState();
    k_sleep(K_MSEC(10));
    networkStartDhcpState();
}

static void networkHandleEvent(networkEvent e) {
    LOG_INF("recieved event: %d", e);
    if (e == NETWORK_EVENT_ETHERNET_DOWN) {
        if (network_instance.state_ == NETWORK_STATE_WAITING_NET) {
            return;
        }
        HandleNetworkIsDown();
        networkStopDhcpState();
        network_instance.state_ = NETWORK_STATE_WAITING_NET;
    } else if (e == NETWORK_EVENT_ETHERNET_UP) {
        if (network_instance.state_ != NETWORK_STATE_WAITING_NET) {
            return;
        }
        HandleNetworkConnecting();
        networkStartDhcpState();
        network_instance.state_ = NETWORK_STATE_WAITING_DHCP;
    } else if (e == NETWORK_EVENT_DHCP_DOWN) {
        if (network_instance.state_ == NETWORK_STATE_WAITING_NET) {
            return;
        } else if (network_instance.state_ == NETWORK_STATE_UP) {
            HandleNetworkConnecting();
        }
        networkResetDhcpState();
        network_instance.state_ = NETWORK_STATE_WAITING_DHCP;
    } else if (e == NETWORK_EVENT_DHCP_UP) {
        if (network_instance.state_ != NETWORK_STATE_WAITING_DHCP) {
            return;
        }
        HandleNetworkIsUp();
        network_instance.state_ = NETWORK_STATE_UP;
    }
    LOG_DBG("now has state: %d", network_instance.state_);
}


static void networkProcessEvents() {
    while (1) {
        struct networkEventMessage *rx_event = k_fifo_get(
            &network_instance.bus_, K_NO_WAIT
        );
        if (rx_event == NULL) return;
        networkHandleEvent(rx_event->network_event_);
        k_free(rx_event);
    }
}

void networkStartup() {
    
    LOG_INF("Waiting for ethernet to initialize if present");
    k_sleep(K_SECONDS(5));
    LOG_INF("Running startup sequence");
    
    /* intialize members */
    network_instance.state_ = NETWORK_STATE_WAITING_NET;
    k_fifo_init(&network_instance.bus_);

    /* setup listeners */
    net_mgmt_init_event_callback(
        &network_instance.iface_callback_, networkIfaceCallback, NET_EVENT_IF_UP | NET_EVENT_IF_DOWN
    );
    net_mgmt_init_event_callback(
        &network_instance.dhcp_callback_, networkDhcpCallback, NET_EVENT_IPV4_DHCP_BOUND | NET_EVENT_IPV4_DHCP_STOP
    );
    net_mgmt_add_event_callback(&network_instance.iface_callback_);
    net_mgmt_add_event_callback(&network_instance.dhcp_callback_);

    /* inital ethernet check */
	struct net_if *iface;
    iface = net_if_get_default();

	bool is_up = net_if_is_up(iface);
	if (is_up) {
		networkSendEvent(NETWORK_EVENT_ETHERNET_UP);
	} else {
		networkSendEvent(NETWORK_EVENT_ETHERNET_DOWN);
	}
}

void networkRun() {
    while (1) {
        networkProcessEvents();
        k_sleep(K_MSEC(100));
    }
}