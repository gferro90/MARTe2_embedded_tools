/*
 * USBCommunication.cpp
 *
 *  Created on: 23/ago/2016
 *      Author: pc
 */

#include "USBCommunication.h"

#include "AdvancedErrorManagement.h"
#include "USBCommunicationReader.h"
#include "USBCommunicationWriter.h"
#include "usbd_cdc_if.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
uint32 sentPacketNumber = 0u;
uint32 receivedPacketNumber = 0u;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//TODO
//the standard USBCommunicationReader is not sync and it is blocking
//add the sync and the non-blocking one and return them depending on configuration parameters
USBCommunication::USBCommunication() :
        DataSourceI() {
    //Initialisation already done by the tool !!!
    if (!USBInitialized()) {
        USBOpen();
    }
    readBlocking = 1u;
    writeBlocking = 1u;
    readFlag = -1;
}

USBCommunication::~USBCommunication() {

}

bool USBCommunication::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        if (!data.Read("ReadBlocking", readBlocking)) {
            REPORT_ERROR(ErrorManagement::Warning, "ReadBlocking not set: using default 1 (true)");
            readBlocking = 1u;
        }
        if (!data.Read("WriteBlocking", writeBlocking)) {
            REPORT_ERROR(ErrorManagement::Warning, "WriteBlocking not set: using default 1 (true)");
            writeBlocking = 1u;
        }

        int8 isSyncWrite = 0;
        if (!data.Read("SynchronisedWrite", isSyncWrite)) {
            REPORT_ERROR(ErrorManagement::Warning, "SynchronisedWrite not set: using default 0 (disabled)");
            readFlag = -1;
        }
        else {
            readFlag = (isSyncWrite == 0) ? (-1) : (0);
        }
    }

    if (!USBInitialized()) {
        USBOpen();
    }
    return true;
}

bool USBCommunication::Synchronise() {
    return true;
}

bool USBCommunication::AllocateMemory() {
    return true;
}

uint32 USBCommunication::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool USBCommunication::GetSignalMemoryBuffer(const uint32 signalIdx,
                                             const uint32 bufferIdx,
                                             void *&signalAddress) {
    signalAddress = NULL;
    return true;
}

const char8 *USBCommunication::GetBrokerName(StructuredDataI &data,
                                             const SignalDirection direction) {
    if (direction == InputSignals) {
        return "USBCommunicationReader";
    }
    if (direction == OutputSignals) {
        return "USBCommunicationWriter";
    }
    return "";
}

bool USBCommunication::GetInputBrokers(ReferenceContainer &inputBrokers,
                                       const char8* const functionName,
                                       void * const gamMemPtr) {
    ReferenceT<USBCommunicationReader> broker("USBCommunicationReader");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool USBCommunication::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                        const char8* const functionName,
                                        void * const gamMemPtr) {
    ReferenceT<USBCommunicationWriter> broker("USBCommunicationWriter");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool USBCommunication::PrepareNextState(const MARTe::char8 * const currentStateName,
                                        const MARTe::char8 * const nextStateName) {
    return true;
}

uint8 USBCommunication::IsReadBlocking() const {
    return readBlocking;
}

uint8 USBCommunication::IsWriteBlocking() const {
    return writeBlocking;
}

int8 USBCommunication::GetReadFlag() const {
    return readFlag;
}

void USBCommunication::SetReadFlag(const int8 flag) {
    readFlag = flag;
}

CLASS_REGISTER(USBCommunication, "1.0")
