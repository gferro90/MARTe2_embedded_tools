/*
 * PWMWriter.cpp
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#include "PWMWriter.h"
#include "AdvancedErrorManagement.h"
#include "PWM.h"

static const uint32 channelNames[] = { TIM_CHANNEL_1, TIM_CHANNEL_2,
		TIM_CHANNEL_3, TIM_CHANNEL_4 };

PWMWriter::PWMWriter() :
		MemoryMapInputBroker() {
	// TODO Auto-generated constructor stub
	pwmHandle = NULL;
	channel =0u;
}

PWMWriter::~PWMWriter() {
	// TODO Auto-generated destructor stub
}

bool PWMWriter::Execute() {
	uint32 n;
	bool ret = true;
	for (n = 0u; (n < numberOfCopies) && (ret); n++) {
		if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
			uint32 threshold = 0u;
			MemoryOperationsHelper::Copy(&threshold, copyTable[n].gamPointer,
					copyTable[n].copySize);
			__HAL_TIM_SET_COMPARE(pwmHandle, channelNames[channel],
					threshold);
		}
	}
	return ret;
}

bool PWMWriter::Init(const SignalDirection direction, DataSourceI &dataSourceIn,
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

CLASS_REGISTER(PWMWriter, "1.0")
