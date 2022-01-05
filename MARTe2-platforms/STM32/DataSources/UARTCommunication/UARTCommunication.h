/*
 * UARTCommunication.h
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#include "GenericStreamDriver.h"
#include QUOTE(_HAL_H)

#ifndef UARTCOMMUNICATION_H_
#define UARTCOMMUNICATION_H_

using namespace MARTe;

class UARTCommunication: public GenericStreamDriver {
public:

    CLASS_REGISTER_DECLARATION()

    UARTCommunication();
    virtual ~UARTCommunication();

    virtual bool Initialise(StructuredDataI &data);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);


    UART_HandleTypeDef * GetHandle();

    virtual void Read(void * bufferToFill, uint32 &sizeToRead);

    virtual void Write(void * bufferToFlush, uint32 &sizeToWrite);


private:
    UART_HandleTypeDef * uartHandle;

};

#endif /* UARTCOMMUNICATION_H_ */
