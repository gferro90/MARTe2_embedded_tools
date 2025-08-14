/**
 * @file UdpReceiverDataSource.h
 * @brief Header file for class UdpReceiverDataSource
 * @date 28/set/2016
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

 * @details This header file contains the declaration of the class ADCDMA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_UdpReceiverDataSource_H_
#define DATASOURCES_UdpReceiverDataSource_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MemoryDataSourceI.h"
#include "ReferenceT.h"
#include "UdpStream.h"
#include QUOTE(_HAL_H)

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief This data source receives data through the ethernet interface using UDP protocol
 * @details The used must specify the path to UDP stream interface (@see UDPStream) which is initalised with the board local IP and port.
 *
 * @details Follows an example of configuration
 * <pre>
 * +UdpReceiver = {
 *     Class = UdpDataSource::UdpReceiverDataSource
 *     Interface = UdpInterface
 * }
 * </pre>
 */
class UdpReceiverDataSource: public MemoryDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    UdpReceiverDataSource();

    /**
     * @brief Destructor
     */
    virtual ~UdpReceiverDataSource();

    /**
     * @brief Initialises the data source
     * @details The user must define the path to the UDP interface (@see UdpStream) and the remote IpAddress and Port
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Returns MemoryMapSynchronisedOutputBroker
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @brief Returns true
     */
    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

    /**
     * @brief Returns true
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief Receives the data source memory buffer over UDP
     */
    bool Synchronise();

private:

    /**
     * Pointer to the UDP interface
     */
    ReferenceT<UdpStream> udpInterface;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UdpReceiverDataSource_H_ */

