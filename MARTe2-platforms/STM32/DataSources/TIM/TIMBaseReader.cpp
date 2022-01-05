/*
 * TIMBaseReader.cpp
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#include "TIMBaseReader.h"
#include "AdvancedErrorManagement.h"
#include "TIMBase.h"

TIMBaseReader::TIMBaseReader():MemoryMapInputBroker() {
	// TODO Auto-generated constructor stub
	timHandle=NULL;
}

TIMBaseReader::~TIMBaseReader() {
	// TODO Auto-generated destructor stub
}


bool TIMBaseReader::Execute() {
    uint32 n;
    //REPORT_ERROR(ErrorManagement::Warning, "Reading Enc");
    bool ret = true;
    for (n = 0u; (n < numberOfCopies) && (ret); n++) {
        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
        	uint32 counter=__HAL_TIM_GET_COUNTER(timHandle);
        	MemoryOperationsHelper::Copy(copyTable[n].gamPointer, &counter ,copyTable[n].copySize);
        }
    }
    return ret;
}

bool TIMBaseReader::Init(const SignalDirection direction,
	                           DataSourceI &dataSourceIn,
	                           const char8 * const functionName,
	                           void * const gamMemoryAddress){
	bool ret=MemoryMapInputBroker::Init(direction,dataSourceIn, functionName, gamMemoryAddress);
	TIMBase *timDS=dynamic_cast<TIMBase*>(dataSource);
	if(timDS!=NULL){
		timHandle=timDS->GetHandle();
	}
	else{
		REPORT_ERROR(ErrorManagement::FatalError, "Dynamic cast from DataSourceI to TIMBase failed");
	}
	return ret;
}


CLASS_REGISTER(TIMBaseReader, "1.0")
