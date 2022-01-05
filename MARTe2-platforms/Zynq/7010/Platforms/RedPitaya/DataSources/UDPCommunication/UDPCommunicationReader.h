/*
 * UDPCommunicationReader.h
 *
 *  Created on: Jan 17, 2018
 *      Author: gferro
 */

#ifndef UDPCOMMUNICATIONREADER_H_
#define UDPCOMMUNICATIONREADER_H_

#include "MemoryMapInputBroker.h"

using namespace MARTe;

class UDPCommunication;


class UDPCommunicationReader: public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

    UDPCommunicationReader();
    virtual ~UDPCommunicationReader();
    virtual bool Execute();
    virtual bool Init(const SignalDirection direction,
              DataSourceI &dataSourceIn,
              const char8 * const functionName,
              void * const gamMemoryAddress);
private:
    UDPCommunication *dataSourceUDP;
    uint8 bufferAllocated;
};

#endif /* UDPCOMMUNICATIONREADER_H_ */

