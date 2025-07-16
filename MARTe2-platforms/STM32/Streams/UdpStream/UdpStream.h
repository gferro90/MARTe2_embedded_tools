/**
 * @file UdpStream.h
 * @brief Header file for class UdpStream
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

 * @details This header file contains the declaration of the class UdpStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UDPSTREAM_H_
#define UDPSTREAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamParent.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Implementation of UDP communication interface for STM32 boards.
 */
class UdpStream: public StreamParent {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    UdpStream();

    /**
     * @brief Destructor
     */
    virtual ~UdpStream();

    /**
     * @brief Creates the socket and binds to provided local IPs and Port
     * @details The static IP must match the one defined in STM32Cube.
     * User must configure the following parameters:
     *  - LocalIpAddress: a 4 bytes array specifying the board IP
     *  - LocalPort: the port to bind to
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Creates the socket and binds to provided local IPs and Port
     * @param[in] ip: a 4 bytes array specifying the board IP
     * @param[in] port: the port to bind to
     * @details This function is called by Initialise, to it is meant to be explicitely called when not used with Initialise.
     */
    virtual bool Open(const uint8 *ip, uint16 port);

    /**
     * @brief Connects to remote host
     * @param[in] ip: a 4 bytes array specifying the remote IP to connect to
     * @param[in] port: the remote port to connect to
     */
    virtual bool Connect(const uint8 *ip, uint16 port);

    /**
     * @see StremParent::Read
     * @details Reads from the UDP queue (1024 bytes)
     */
    virtual bool Read(char8 * const output,
                      uint32 & size,
                      uint32 timeout);

    /**
     * @see StremParent::Write
     * @details Sends the buffer to UDP.
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size,
                       uint32 timeout);

private:

    /**
     * The UDP structure
     */
    struct udp_pcb *upcb;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UDPSTREAM_H_ */

