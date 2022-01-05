/*
 * PWMReader.cpp
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#include "PWMReader.h"
#include "AdvancedErrorManagement.h"
#include "PWM.h"

static const uint32 channelNames[] = { TIM_CHANNEL_1, TIM_CHANNEL_2,
		TIM_CHANNEL_3, TIM_CHANNEL_4 };

PWMReader::PWMReader() :
		MemoryMapInputBroker() {
	// TODO Auto-generated constructor stub
	pwmHandle = NULL;
	channel =0u;
}

PWMReader::~PWMReader() {
	// TODO Auto-generated destructor stub
}

bool PWMReader::Execute() {
	uint32 n;
	bool ret = true;
	for (n = 0u; (n < numberOfCopies) && (ret); n++) {
		if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
			uint32 threshold = __HAL_TIM_GET_COMPARE(pwmHandle,
					channelNames[channel]);
			MemoryOperationsHelper::Copy(copyTable[n].gamPointer, &threshold,
					copyTable[n].copySize);
		}
	}
	return ret;
}

bool PWMReader::Init(const SignalDirection direction, DataSourceI &dataSourceIn,
		const char8 * const functionName, void * const gamMemoryAddress) {
	bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName,
			gamMemoryAddress);
	PWM *pwmDS = dynamic_cast<PWM*>(dataSource);
	if (pwmDS != NULL) {
		pwmHandle = pwmDS->GetHandle();
		channel = pwmDS->GetChannel();
	} else {
		REPORT_ERROR(ErrorManagement::FatalError,
				"Dynamic cast from DataSourceI to TIMBaseDataSource failed");
	}
	return ret;
}

CLASS_REGISTER(PWMReader, "1.0")
