/*
 * EncodersCounterDataSource.cpp
 *
 *  Created on: 31/ago/2016
 *      Author: pc
 */

#include "EncodersCounterDataSource.h"
#include "AdvancedErrorManagement.h"
#include "TIMBaseReader.h"
#include "TIMBaseWriter.h"
#include "HandleDatabase.h"
//TODO
//the standard USBCommunicationReader is not sync and it is blocking
//add the sync and the non-blocking one and return them depending on configuration parameters



EncodersCounterDataSource::EncodersCounterDataSource() :
        TIMBase() {
    //Initialisation already done by the tool !!!
    encoderHandlePtr = NULL;
    resetEncoder = 0u;
}

EncodersCounterDataSource::~EncodersCounterDataSource() {

    HAL_TIM_Encoder_Stop(encoderHandlePtr, TIM_CHANNEL_ALL);
}

bool EncodersCounterDataSource::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);

    if (ret) {
        if (!data.Read("ResetEncoder", resetEncoder)) {
            resetEncoder = 0u;
        }

        StreamString timId;
        ret = data.Read("TimIdentifier", timId);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::Warning, "No TIM id specified: assuming 0");
        }
        if (ret) {
            encoderHandlePtr = (TIM_HandleTypeDef*) GetHandle(timId.Buffer());
            ret = (encoderHandlePtr != NULL);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The handle is not valid");
            }
        }

    }
    return ret;
}

bool EncodersCounterDataSource::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);

    if (ret) {
        uint32 numberOfSignals = GetNumberOfSignals();

        ret = (numberOfSignals == 1u);

        if (ret) {
            ret = (GetSignalType(0u) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The encoder signal type has to be uint32");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The encoder data source allows one signal only");
        }
        if (ret) {
            HAL_TIM_Encoder_Start(encoderHandlePtr, TIM_CHANNEL_ALL);

        }
    }

    return ret;
}

TIM_HandleTypeDef * EncodersCounterDataSource::GetHandle() {
    //todo in the main map into an array the handles of the defined uarts
    //define this function
    return encoderHandlePtr;
}

bool EncodersCounterDataSource::PrepareNextState(const char8 * const currentStateName,
                                                 const char8 * const nextStateName) {
    bool ret = TIMBase::PrepareNextState(currentStateName, nextStateName);
    if (ret && resetEncoder) {
        __HAL_TIM_SET_COUNTER(encoderHandlePtr, 0);
    }
    return ret;
}

CLASS_REGISTER(EncodersCounterDataSource, "1.0")
