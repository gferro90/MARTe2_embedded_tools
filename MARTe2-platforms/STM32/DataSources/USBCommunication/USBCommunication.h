/*
 * USBCommunication.h
 *
 *  Created on: 23/ago/2016
 *      Author: pc
 */
 
#ifndef USBCOMMUNICATION_H_
#define USBCOMMUNICATION_H_

#include "GenericStreamDriver.h"

using namespace MARTe;

class USBCommunication: public GenericStreamDriver {
public:

    CLASS_REGISTER_DECLARATION()

    USBCommunication();
    virtual ~USBCommunication();

    virtual bool Initialise(StructuredDataI &data);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);

    uint8 IsReadBlocking() const;

    uint8 IsWriteBlocking() const;

    int8 GetReadFlag() const;

    void SetReadFlag(const int8 flag);

    virtual void Read(void * bufferToFill, uint32 &sizeToRead);

    virtual void Write(void * bufferToFlush, uint32 &sizeToWrite);

    virtual void TerminateRead();
    virtual void TerminateWrite();

private:


    uint8 readBlocking;
    uint8 writeBlocking;
    int8 readFlag;
    uint32 readBytes;

};

#endif /* USBCOMMUNICATION_H_ */
