/*
 * TIMBaseWriter.cpp
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#include "TIMBaseWriter.h"
#include "AdvancedErrorManagement.h"
#include "TIMBase.h"

TIMBaseWriter::TIMBaseWriter() :
		MemoryMapInputBroker() {
	// TODO Auto-generated constructor stub
	timHandle = NULL;
}

TIMBaseWriter::~TIMBaseWriter() {
	// TODO Auto-generated destructor stub
}

bool TIMBaseWriter::Execute() {
	uint32 n;
	bool ret = true;
	for (n = 0u; (n < numberOfCopies) && (ret); n++) {
		if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
			uint32 counter = 0u;
			MemoryOperationsHelper::Copy(&counter, copyTable[n].gamPointer,
					copyTable[n].copySize);
			__HAL_TIM_SET_COUNTER(timHandle, counter);
		}
	}
	return ret;
}

bool TIMBaseWriter::Init(const SignalDirection direction,
		DataSourceI &dataSourceIn, const char8 * const functionName,
		void * const gamMemoryAddress) {
	bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName,
			gamMemoryAddress);
	TIMBase *timDS = dynamic_cast<TIMBase*>(dataSource);
	if (timDS != NULL) {
		timHandle = timDS->GetHandle();
	} else {
		REPORT_ERROR(ErrorManagement::FatalError,
				"Dynamic cast from DataSourceI to TIMBase failed");
	}
	return ret;
}

CLASS_REGISTER(TIMBaseWriter, "1.0")
