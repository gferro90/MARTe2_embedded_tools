/*
 * USBCommunication.h
 *
 *  Created on: 23/ago/2016
 *      Author: pc
 */

#ifndef USBCOMMUNICATION_H_
#define USBCOMMUNICATION_H_

#include "DataSourceI.h"

using namespace MARTe;

class USBCommunication: public DataSourceI {
public:

    CLASS_REGISTER_DECLARATION()

    USBCommunication();
    virtual ~USBCommunication();

    virtual bool Initialise(StructuredDataI &data);

    bool Synchronise();

    bool AllocateMemory();

    uint32 GetNumberOfMemoryBuffers();

    bool GetSignalMemoryBuffer(const uint32 signalIdx,
                               const uint32 bufferIdx,
                               void *&signalAddress);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    uint8 IsReadBlocking() const;

    uint8 IsWriteBlocking() const;

    int8 GetReadFlag() const;

    void SetReadFlag(const int8 flag);
private:


    uint8 readBlocking;
    uint8 writeBlocking;
    int8 readFlag;
};

#endif /* USBCOMMUNICATION_H_ */
