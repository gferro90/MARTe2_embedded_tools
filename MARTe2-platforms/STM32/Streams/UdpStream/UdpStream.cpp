/**
 * @file UdpStream.cpp
 * @brief Source file for class UdpStream
 * @date Jun 16, 2017
 * @author pc
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class UdpStream (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "lwip/pbuf.h"
#include "lwip/udp.h"

#include "stdio.h"
#include "string.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UdpStream.h"
#include "HighResolutionTimer.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#define UDP_BUF_SIZE 256

static uint8 udpBuffer[UDP_BUF_SIZE];
static uint32 packetSize = 0u;
#if 0
static uint32 udpBufferWriteIdx = 0u;
static uint32 udpBufferReadIdx = 0u;
static uint8 countWrite = 0u;
static uint8 countRead = 0u;

static FastPollingMutexSem semudp;

static void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    if (semudp.FastLock()) {

        if ((udpBufferWriteIdx + p->len) >= UDP_BUF_SIZE) {
            uint32 first = (UDP_BUF_SIZE - udpBufferWriteIdx);
            uint32 second = (udpBufferWriteIdx + p->len) - UDP_BUF_SIZE;
            MemoryOperationsHelper::Copy(&udpBuffer[udpBufferWriteIdx], p->payload, first);
            MemoryOperationsHelper::Copy(&udpBuffer[0], p->payload + first, second);
            countWrite++;
        } else {
            MemoryOperationsHelper::Copy(&udpBuffer[udpBufferWriteIdx], p->payload, p->len);
        }
        udpBufferWriteIdx += p->len;
        udpBufferWriteIdx %= UDP_BUF_SIZE;
        pbuf_free(p);
        semudp.FastUnLock();
    }

}
#endif

extern void PrintF(const char *const message);

static void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    packetSize = (p->len > UDP_BUF_SIZE) ? (UDP_BUF_SIZE) : (p->len);
    MemoryOperationsHelper::Set(&udpBuffer[0], 0, UDP_BUF_SIZE);
    MemoryOperationsHelper::Copy(&udpBuffer[0], p->payload, packetSize);
    pbuf_free(p);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

UdpStream::UdpStream() {
    MX_LWIP_Init();
    upcb = NULL;
    remotePort = 0;
}

UdpStream::~UdpStream() {
    // Auto-generated destructor stub for UdpStream
    // TODO Verify if manual additions are needed
    if (upcb != NULL) {
        udp_remove(upcb);
    }
}

bool UdpStream::Initialise(StructuredDataI &data) {
    Vector<uint8> localIp(4);
    bool ret = data.Read("LocalIpAddress", localIp);
    if (ret) {
        uint16 port;
        ret = data.Read("LocalPort", port);
        if (ret) {
            ret = Open(localIp.GetDataPointer(), port);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed Open");
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "LocalPort not specified");
        }
    } else {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Failed to read LocalIpAddress");
    }
    return ret;
}

bool UdpStream::Open(const uint8 *ip, uint16 port) {
    bool ret = true;

    upcb = udp_new();

    ip_addr_t myIPaddr;
    IP_ADDR4(&myIPaddr, ip[0], ip[1], ip[2], ip[3]);
    ret = (udp_bind(upcb, &myIPaddr, port) == ERR_OK);
    udp_recv(upcb, udp_receive_callback, NULL);
    return ret;
}

bool UdpStream::Connect(const uint8 *ip, uint16 port) {
    bool ret = true;
#if 0
     ip_addr_t destIPaddr;
     IP_ADDR4(&destIPaddr, ip[0], ip[1], ip[2], ip[3]);
     ret = (udp_connect(upcb, &destIPaddr, port) == ERR_OK);
#endif
    IP_ADDR4(&remoteIp, ip[0], ip[1], ip[2], ip[3]);
    remotePort = port;
    return ret;
}

#if 0
bool UdpStream::Read(char8 *const output, uint32 &size, uint32 timeout) {
    uint64 tic = HighResolutionTimer::Counter();
    if (semudp.FastLock()) {
        if (countWrite > countRead) {
            countWrite = (countRead + 1u);
        }
        uint8 countOverrun = (countWrite - countRead);
        //this cannot be... but check anyway
        if ((countOverrun == 0) && (udpBufferReadIdx > udpBufferWriteIdx)) {
            udpBufferReadIdx = udpBufferWriteIdx;
            countWrite = 0u;
            countRead = 0u;
        }

        uint32 writtenBytes = (udpBufferWriteIdx + (countOverrun * UDP_BUF_SIZE) - udpBufferReadIdx);
        //overrun... reset
        if (writtenBytes >= UDP_BUF_SIZE) {
            writtenBytes = size;
            udpBufferReadIdx = (udpBufferWriteIdx > size) ? (udpBufferWriteIdx - size) : (UDP_BUF_SIZE - (size - udpBufferWriteIdx));
        }
        if (size > writtenBytes) {
            if (timeout == 0u) {
                size = writtenBytes;
            } else {
                semudp.FastUnLock();
                uint32 elapsedMillis = (uint32) (((HighResolutionTimer::Counter() - tic) * HighResolutionTimer::Period()) * 1000);
                //use HRT to remove from the timeout?
                if (timeout > elapsedMillis) {
                    return Read(output, size, (timeout - elapsedMillis));
                } else {
                    //timeout
                    return false;
                }
            }
        }

        if (size > 0u) {
            if ((udpBufferReadIdx + size) >= UDP_BUF_SIZE) {
                uint32 first = (UDP_BUF_SIZE - udpBufferReadIdx);
                uint32 second = (udpBufferReadIdx + size - UDP_BUF_SIZE);
                MemoryOperationsHelper::Copy(output, &udpBuffer[udpBufferReadIdx], first);
                MemoryOperationsHelper::Copy(output, &udpBuffer[0], second);
                countRead++;
                if (countRead >= countWrite) {
                    countRead = 0u;
                    countWrite = 0u;
                }
            } else {
                MemoryOperationsHelper::Copy(output, &udpBuffer[udpBufferReadIdx], size);
            }
            udpBufferReadIdx += size;
            udpBufferReadIdx %= UDP_BUF_SIZE;
        }
        semudp.FastUnLock();
    }
    return true;
}
#endif

bool UdpStream::Read(char8 *const output, uint32 &size, uint32 timeout) {

    uint32 rsize = (packetSize > size) ? (size) : (packetSize);
    if (rsize > 0u) {
        MemoryOperationsHelper::Set(output, 0, size);
        MemoryOperationsHelper::Copy(output, udpBuffer, rsize);
    }
    size = rsize;
    return true;
}

bool UdpStream::Write(const char8 *const input, uint32 &size, uint32 timeout) {
    struct pbuf *txBuf;
    txBuf = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);

    if (txBuf != NULL) {
        pbuf_take(txBuf, input, size);
        udp_sendto(upcb, txBuf, &remoteIp, remotePort);
        pbuf_free(txBuf);
    }
    return true;
}
CLASS_REGISTER(UdpStream, "1.0")
