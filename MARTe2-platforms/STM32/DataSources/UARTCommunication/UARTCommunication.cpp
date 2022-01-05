/*
 * UARTCommunication.cpp
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#include "UARTCommunication.h"
#include "AdvancedErrorManagement.h"
#include "UARTCommunicationReader.h"
#include "UARTCommunicationWriter.h"
//TODO
//the standard USBCommunicationReader is not sync and it is blocking
//add the sync and the non-blocking one and return them depending on configuration parameters

extern "C" {

extern void * GetHandle(const char * id);

}

UARTCommunication::UARTCommunication() :
        GenericStreamDriver() {
    uartHandle = NULL;
}

UARTCommunication::~UARTCommunication() {

}

bool UARTCommunication::Initialise(StructuredDataI &data) {
    bool ret = GenericStreamDriver::Initialise(data);
    if (ret) {
        StreamString uartId;
        if (!data.Read("UartId", uartId)) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "No UART Id specified");
            ret = false;
        }
        if (ret) {
            uartHandle = (UART_HandleTypeDef*)GetHandle(uartId.Buffer());
            ret = (uartHandle != NULL);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The handle is not valid");
            }
        }

    }
    return ret;
}

const char8 *UARTCommunication::GetBrokerName(StructuredDataI &data,
                                              const SignalDirection direction) {
    if (direction == InputSignals) {
        return "UARTCommunicationReader";
    }
    if (direction == OutputSignals) {
        return "UARTCommunicationWriter";
    }
    return "";
}

bool UARTCommunication::GetInputBrokers(ReferenceContainer &inputBrokers,
                                        const char8* const functionName,
                                        void * const gamMemPtr) {
    ReferenceT<UARTCommunicationReader> broker("UARTCommunicationReader");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool UARTCommunication::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                         const char8* const functionName,
                                         void * const gamMemPtr) {
    ReferenceT<UARTCommunicationWriter> broker("UARTCommunicationWriter");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

UART_HandleTypeDef * UARTCommunication::GetHandle() {
    //todo in the main map into an array the handles of the defined uarts
    //define this function
    return uartHandle;
}

void UARTCommunication::Read(void *bufferToFill,
                             uint32 &sizeToRead) {
    HAL_UART_Receive(uartHandle, (uint8_t*) bufferToFill, sizeToRead, HAL_MAX_DELAY);
}

void UARTCommunication::Write(void *bufferToFlush,
                              uint32 &sizeToWrite) {
    HAL_UART_Transmit(uartHandle, (uint8_t*) bufferToFlush, sizeToWrite, HAL_MAX_DELAY);
}

CLASS_REGISTER(UARTCommunication, "1.0")
