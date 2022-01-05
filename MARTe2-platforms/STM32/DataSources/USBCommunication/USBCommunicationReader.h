/*
 * USBCommunicationReader.h
 *
 *  Created on: 23/ago/2016
 *      Author: pc
 */


#ifndef USBCOMMUNICATIONREADER_H_
#define USBCOMMUNICATIONREADER_H_

#include "MemoryMapInputBroker.h"

using namespace MARTe;

class USBCommunication;

class USBCommunicationReader: public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

    USBCommunicationReader();
    virtual ~USBCommunicationReader();
    virtual bool Execute();
    virtual bool Init(const SignalDirection direction,
              DataSourceI &dataSourceIn,
              const char8 * const functionName,
              void * const gamMemoryAddress);
private:
    uint8 readNonBlocking;
    USBCommunication *dataSourceUSB;
    uint8 bufferAllocated;
};

#endif /* USBCOMMUNICATIONREADER_H_ */
