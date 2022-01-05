/*
 * UDPCommunicationWriter.h
 *
 *  Created on: Jan 17, 2018
 *      Author: gferro
 */

#ifndef UDPCOMMUNICATIONWRITER_H_
#define UDPCOMMUNICATIONWRITER_H_
#include "MemoryMapOutputBroker.h"

using namespace MARTe;

class UDPCommunication;

class UDPCommunicationWriter : public MemoryMapOutputBroker {
public:
    CLASS_REGISTER_DECLARATION()

    UDPCommunicationWriter();
    virtual ~UDPCommunicationWriter();
    virtual bool Execute();
    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 * const functionName,
                      void * const gamMemoryAddress);

private:
    UDPCommunication *dataSourceUDP;
    uint8 bufferAllocated;
};

#endif /* UDPCOMMUNICATIONWRITER_H_ */

