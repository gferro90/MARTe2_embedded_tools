/*
 * UdpCore.h
 *
 *  Created on: Jan 19, 2018
 *      Author: gferro
 */

#ifndef UDPUTILS_H_
#define UDPUTILS_H_
#include <string.h>
#include <stdio.h>

extern "C" {
#include "xparameters.h"
#include "netif/xadapter.h"
#include "platform.h"
#include "platform_config.h"
#include "lwip/tcp.h"
#include "xil_cache.h"
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/udp.h"
#include "xil_printf.h"
void lwip_init(void);

}

#include "AdvancedErrorManagement.h"
#include "ReferenceContainer.h"
using namespace MARTe;


void recv_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p,
		ip_addr_t *addr, u16_t port);

struct UdpConfigs {
	const char* macAddr;
	const char* remoteIpAddr;
	const char* myIpAddr;
	const char* myGateway;
	const char* myNetmask;
	uint32 remotePort;
	uint32 myPort;
	uint8 isBlocking;
};

class UdpCore: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

	UdpCore();

	virtual ~UdpCore();
	bool Configure(const struct UdpConfigs &config);

	int32 Read(char8 * const output, uint32 & size, uint32 timeout);

	int32 Write(const char8 * const input, uint32 & size, uint32 timeout);

	int32 TransferUdpData(const void* send_buf, uint32 wsize);

	int32 ReceiveUdpData(void* recv_buf, uint32 rsize);

	static void IpToArray(const char* in, uint8* out);

	static void MacToArray(const char* in, uint8* out);

	void Remove();

private:

	struct udp_pcb *pcb;
	struct pbuf *p;
	volatile int32 processingPacket;
	uint32 args[3];
	uint8 isBlocking;
};

#endif /* UDPUTILS_H_ */
