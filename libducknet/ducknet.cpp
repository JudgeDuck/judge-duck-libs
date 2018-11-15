#include <ducknet.h>

#include "ducknet_impl.h"

static int (*idle)();

int ducknet_init(const DucknetConfig *conf) {
	int r;
	if ((r = ducknet_utils_init(&(conf->utils))) < 0) {
		return r;
	}
	if ((r = ducknet_phy_init(&(conf->phy))) < 0) {
		return r;
	}
	if ((r = ducknet_ether_init(&(conf->ether))) < 0) {
		return r;
	}
	if ((r = ducknet_arp_init(&(conf->arp))) < 0) {
		return r;
	}
	if ((r = ducknet_ipv4_init(&(conf->ipv4))) < 0) {
		return r;
	}
	if ((r = ducknet_icmp_init(&(conf->icmp))) < 0) {
		return r;
	}
	if ((r = ducknet_udp_init(&(conf->udp))) < 0) {
		return r;
	}
	idle = conf->idle;
	return 0;
}

int ducknet_flush() {
	return ducknet_phy_flush();
}

int ducknet_mainloop() {
	int r = 0;
	static char pkt[MAX_MTU + 64];
	while (1) {
		if ((r = ducknet_idle()) < 0) break;
		if (idle && (r = idle()) < 0) break;
		int tmp = ducknet_phy_recv(pkt);
		if (tmp < 0) continue;
		ducknet_packet_handle(pkt, tmp);
	}
	return r;
}

int ducknet_idle() {
	int r;
	if ((r = ducknet_utils_idle()) < 0) {
		return r;
	}
	if ((r = ducknet_phy_idle()) < 0) {
		return r;
	}
	if ((r = ducknet_ether_idle()) < 0) {
		return r;
	}
	if ((r = ducknet_arp_idle()) < 0) {
		return r;
	}
	if ((r = ducknet_ipv4_idle()) < 0) {
		return r;
	}
	if ((r = ducknet_icmp_idle()) < 0) {
		return r;
	}
	if ((r = ducknet_udp_idle()) < 0) {
		return r;
	}
	return 0;
}

int ducknet_packet_handle(void *pkt, int len) {
	return ducknet_phy_packet_handle(pkt, len);
}
