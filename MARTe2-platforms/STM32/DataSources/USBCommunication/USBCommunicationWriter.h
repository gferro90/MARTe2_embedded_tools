/*
 * USBCommunicationWriter.h
 *
 *  Created on: 23/ago/2016
 *      Author: pc
 */


#ifndef USBCOMMUNICATIONWRITER_H_
#define USBCOMMUNICATIONWRITER_H_

#include "MemoryMapOutputBroker.h"

using namespace MARTe;

class USBCommunication;

class USBCommunicationWriter: public MemoryMapOutputBroker {
public:
    CLASS_REGISTER_DECLARATION()

    USBCommunicationWriter();
    virtual ~USBCommunicationWriter();
    virtual bool Execute();
    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 * const functionName,
                      void * const gamMemoryAddress);

private:
    uint8 writeNonBlocking;
    USBCommunication *dataSourceUSB;
    uint8 bufferAllocated;
};

#endif /* USBCOMMUNICATIONWRITER_H_ */
