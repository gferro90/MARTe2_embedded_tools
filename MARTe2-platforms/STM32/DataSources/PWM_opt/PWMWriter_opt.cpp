/*
 * PWMWriter_opt.cpp
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#include "PWMWriter_opt.h"
#include "AdvancedErrorManagement.h"
#include "PWM_opt.h"

static const uint32 channelNames[] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4 };

PWMWriter_opt::PWMWriter_opt() :
        MemoryMapInputBroker() {
    // TODO Auto-generated constructor stub
    pwmDS = NULL;
}

PWMWriter_opt::~PWMWriter_opt() {
    // TODO Auto-generated destructor stub
}

bool PWMWriter_opt::Execute() {
    uint32 n;
    bool ret = true;
    for (n = 0u; (n < numberOfCopies) && (ret); n++) {
        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
            pwmDS->Write((uint32*)copyTable[n].gamPointer);
        }
    }
    return ret;
}

bool PWMWriter_opt::Init(const SignalDirection direction,
                         DataSourceI &dataSourceIn,
                         const char8 * const functionName,
                         void * const gamMemoryAddress) {
    bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    pwmDS = dynamic_cast<PWM_opt*>(dataSource);
    if (pwmDS == NULL) {
        ret=false;
        REPORT_ERROR(ErrorManagement::FatalError, "Dynamic cast from DataSourceI to TIMBaseDataSource failed");
    }
    return ret;
}

CLASS_REGISTER(PWMWriter_opt, "1.0")
