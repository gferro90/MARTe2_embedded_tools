/*
 * UdpCore.cpp
 *
 *  Created on: Jan 19, 2018
 *      Author: gferro
 */
#include "UdpCore.h"
#include <stdio.h>
#include <string.h>

extern struct netif *echo_netif;

void recv_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p,
		ip_addr_t *addr, u16_t port) {
	/* do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "p is NULL!!");
		udp_recv(pcb, NULL, NULL);
		return;
	}

	//get the size to be processingPacket
	uint32 sizeToRead = *(uint32*) arg;
	uint32 recvBuffPtr = *(uint32*) ((char8*) arg + 4);
	volatile int32 *processingPacket =
			(volatile int32 *) (*(uint32*) ((char8*) arg + 8));

	//copy to the output buffer if
	//enough data has been processingPacket
	if (p->len >= sizeToRead) {
		//xil_printf("received %d %d bytes\n\r", p->len, sizeToRead);
		Atomic::TestAndSet(processingPacket);
		//is one for sure!
		memcpy((void*) recvBuffPtr, p->payload, sizeToRead);
		Atomic::Decrement(processingPacket);
	}

	//TODO and the rest of the packet???
	/* free the processingPacket pbuf */
	pbuf_free(p);
}

void UdpCore::IpToArray(const char* in, uint8* out) {
	uint32 n = 0u;
	uint32 i = 0u;
	out[0] = 0;
	while (in[i] != 0u) {
		if (in[i] == '.') {
			i++;
			n++;
			if (n > 3) {
				break;
			} else {
				out[n] = 0;
				continue;
			}
		}
		out[n] *= 10;
		out[n] += (in[i] - '0');
		i++;
	}

}

void UdpCore::MacToArray(const char* in, uint8* out) {
	uint32 n = 0u;
	uint32 i = 0u;
	out[0] = 0;
	while (in[i] != 0u) {
		if (in[i] == ':') {
			i++;
			n++;
			if (n > 5) {
				break;
			} else {
				out[n] = 0;
				continue;
			}
		}
		out[n] <<= 4;
		char c = in[i];
		if (c >= '0' && c <= '9') {
			out[n] += (c - '0');
		} else if (c >= 'A' && c <= 'F') {
			out[n] += ((c - 'A') + 10);
		} else {
			//skip bad char
			out[n] >>= 4;
		}
		i++;
	}

}

UdpCore::UdpCore() {
	pcb = NULL;
	p = NULL;
	processingPacket = 1;
	args[0] = 0;
	args[1] = 0;
	args[2] = 0;
	isBlocking = 0u;
}

UdpCore::~UdpCore() {

	if (pcb) {
		udp_disconnect(pcb);
		udp_remove(pcb);
		netif_set_down(echo_netif);
		netif_remove(echo_netif);
	}
	pcb = NULL;
}

void UdpCore::Remove() {
	if (pcb) {
		udp_disconnect(pcb);
		udp_remove(pcb);
		netif_remove(echo_netif);
	}
	pcb = NULL;
}

int32 UdpCore::TransferUdpData(const void* send_buf, uint32 wsize) {
	//TODO This can be done better...
	xemacif_input(echo_netif);

	p = pbuf_alloc(PBUF_TRANSPORT, wsize, PBUF_RAM);
	if (!p)
		return ERR_MEM;

	memcpy(p->payload, send_buf, wsize);
	//REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "send %d %d",wsize, *(uint32*)(p->payload));

	udp_send(pcb, p);
	pbuf_free(p);

	return 0;
}

int32 UdpCore::ReceiveUdpData(void* recv_buf, uint32 rsize) {
//udp_recv_fn void *

//typedef void (*udp_recv_fn)(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t config.port);
	if (!processingPacket) {
		Atomic::TestAndSet(&processingPacket);
	}
	args[0] = rsize;
	args[1] = (uint32) recv_buf;
	args[2] = (uint32)(&processingPacket);
	udp_recv(pcb, &recv_callback, (void*) args);

//this is used just to set the function
	//blocking mode

	if (isBlocking) {
		while (processingPacket) {
			xemacif_input(echo_netif);
		}
	} else {
		xemacif_input(echo_netif);
	}
	//udp_recv(pcb, NULL, NULL);
	return 0;
}

bool UdpCore::Configure(const struct UdpConfigs &config) {
	struct ip_addr myIpAddrStruct, netmaskStruct, gwStruct;
	this->isBlocking = config.isBlocking;

	uint8 myIpArr[4];
	uint8 myNetmaskArr[4];
	uint8 myGatewayArr[4];

	IpToArray(config.myIpAddr, myIpArr);
	IpToArray(config.myNetmask, myNetmaskArr);
	IpToArray(config.myGateway, myGatewayArr);
	REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "myIp %d %d %d %d",
			myIpArr[0], myIpArr[1], myIpArr[2], myIpArr[3]);
	REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "myNm %d %d %d %d",
			myNetmaskArr[0], myNetmaskArr[1], myNetmaskArr[2], myNetmaskArr[3]);
	REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "myGw %d %d %d %d",
			myGatewayArr[0], myGatewayArr[1], myGatewayArr[2], myGatewayArr[3]);

	uint8 myMacArr[6];
	MacToArray(config.macAddr, myMacArr);

	REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "myMAC1 %d %d %d",
			myMacArr[0], myMacArr[1], myMacArr[2]);
	REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "myMAC2 %d %d %d",
			myMacArr[3], myMacArr[4], myMacArr[5]);

	/* initliaze IP addresses to be used */
	IP4_ADDR(&myIpAddrStruct, myIpArr[0], myIpArr[1], myIpArr[2], myIpArr[3]);
	IP4_ADDR(&netmaskStruct, myNetmaskArr[0], myNetmaskArr[1], myNetmaskArr[2],
			myNetmaskArr[3]);
	IP4_ADDR(&gwStruct, myGatewayArr[0], myGatewayArr[1], myGatewayArr[2],
			myGatewayArr[3]);

	lwip_init();
	/* Add network interface to the netif_list, and set it as default */
	xemac_add(echo_netif, &myIpAddrStruct, &netmaskStruct, &gwStruct, myMacArr,
	PLATFORM_EMAC_BASEADDR);

	netif_set_default(echo_netif);
	platform_enable_interrupts();
	netif_set_up(echo_netif);

	struct ip_addr ipAddrStruct;
	err_t err;

	/* create new UDP PCB structure*/
	pcb = udp_new();
	if (!pcb) {
		REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
				"Error allocating udp struc");
		return -1;
	}
	REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "hPort %d",
			config.remotePort);

	err = udp_bind(pcb, IP_ADDR_ANY, config.myPort);
	if (err != 0) {
		REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Error bind udp");
		return err;
	}

	uint8 remoteIpArr[4];
	IpToArray(config.remoteIpAddr, remoteIpArr);

	REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "hIp %d %d %d %d",
			remoteIpArr[0], remoteIpArr[1], remoteIpArr[2], remoteIpArr[3]);

	/* connect to host */
	IP4_ADDR(&ipAddrStruct, remoteIpArr[0], remoteIpArr[1], remoteIpArr[2],
			remoteIpArr[3]); /* host address */

	err = udp_connect(pcb, &ipAddrStruct, config.remotePort);

	if (err != ERR_OK) {
		REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Error connect udp");
		return err;
	}

	return 0;

}
CLASS_REGISTER(UdpCore, "1.0")

