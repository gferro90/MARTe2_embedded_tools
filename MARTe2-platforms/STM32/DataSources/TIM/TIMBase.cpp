/*
 * TIMBase.cpp
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#include "TIMBase.h"
#include "AdvancedErrorManagement.h"
#include "TIMBaseReader.h"
#include "TIMBaseWriter.h"
//TODO
//the standard USBCommunicationReader is not sync and it is blocking
//add the sync and the non-blocking one and return them depending on configuration parameters

extern "C" {

extern void * GetHandle(const char * id);

}
TIMBase::TIMBase() :
        DataSourceI() {
    //Initialisation already done by the tool !!!
    isInterruptMode = false;
    timHandlePtr = NULL;
}

TIMBase::~TIMBase() {

    if (isInterruptMode) {
        //HAL_TIM_Base_Stop_IT(timHandlePtr);
    }
    else {
        //HAL_TIM_Base_Stop(timHandlePtr);
    }
}

bool TIMBase::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        StreamString timId;
        ret = data.Read("TimIdentifier", timId);
        if (ret) {
            timHandlePtr = (TIM_HandleTypeDef *)GetHandle(timId.Buffer());
            ret=(timHandlePtr!=NULL);
        }
        else {
            REPORT_ERROR(ErrorManagement::Warning, "No TIM number specified");
        }
        if (ret) {
            uint32 interrupt = 0u;
            if (!data.Read("IsInterrupt", interrupt)) {
                interrupt = 0u;
            }
            isInterruptMode = (interrupt != 0);
            ret = (timHandlePtr != NULL);
        }
    }
    return ret;
}

bool TIMBase::Synchronise() {
    return true;
}

bool TIMBase::AllocateMemory() {
    return true;
}

uint32 TIMBase::GetNumberOfMemoryBuffers() {

    return 1u;
}

bool TIMBase::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);
    uint32 numberOfSignals = GetNumberOfSignals();

    for (uint32 i = 0u; i < numberOfSignals && ret; i++) {
        ret = (GetSignalType(i) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The TimBaseDataSource signal type has to be uint32");
        }
    }
    if (ret) {

        //	REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "timNumber is %d",
        //		timNumber);
        if (isInterruptMode) {
            ret = HAL_TIM_Base_Start_IT(timHandlePtr) == HAL_OK;

        }
        else {
            ret = HAL_TIM_Base_Start(timHandlePtr) == HAL_OK;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::InitialisationError, "The handle is not valid");
    }

    return ret;
}

bool TIMBase::GetSignalMemoryBuffer(const uint32 signalIdx,
                                    const uint32 bufferIdx,
                                    void *&signalAddress) {
    signalAddress = NULL;
    return true;
}

const char8 *TIMBase::GetBrokerName(StructuredDataI &data,
                                    const SignalDirection direction) {
    if (direction == InputSignals) {
        return "TIMBaseReader";
    }
    if (direction == OutputSignals) {
        return "TIMBaseWriter";
    }
    return "";
}

bool TIMBase::GetInputBrokers(ReferenceContainer &inputBrokers,
                              const char8* const functionName,
                              void * const gamMemPtr) {
    ReferenceT<TIMBaseReader> broker("TIMBaseReader");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool TIMBase::GetOutputBrokers(ReferenceContainer &outputBrokers,
                               const char8* const functionName,
                               void * const gamMemPtr) {
    ReferenceT<TIMBaseWriter> broker("TIMBaseWriter");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool TIMBase::PrepareNextState(const MARTe::char8 * const currentStateName,
                               const MARTe::char8 * const nextStateName) {
    return true;
}

TIM_HandleTypeDef * TIMBase::GetHandle() {
    //todo in the main map into an array the handles of the defined tims
    //define this function
    return timHandlePtr;
}

CLASS_REGISTER(TIMBase, "1.0")
