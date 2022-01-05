/*
 * UARTCommunicationWriter.cpp
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#include "UARTCommunicationWriter.h"
#include "UARTCommunication.h"
#include "AdvancedErrorManagement.h"

UARTCommunicationWriter::UARTCommunicationWriter():MemoryMapOutputBroker() {
	// TODO Auto-generated constructor stub
	uartHandle=NULL;
}

UARTCommunicationWriter::~UARTCommunicationWriter() {
	// TODO Auto-generated destructor stub
}


bool UARTCommunicationWriter::Execute() {
    uint32 n;
    bool ret = true;
    for (n = 0u; (n < numberOfCopies) && (ret); n++) {
        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
        	uint32 sizeToWrite=copyTable[n].copySize;
			HAL_UART_Transmit(uartHandle, (uint8_t*) copyTable[n].gamPointer, sizeToWrite, HAL_MAX_DELAY);
        }
    }
    return ret;
}

bool UARTCommunicationWriter::Init(const SignalDirection direction,
	                           DataSourceI &dataSourceIn,
	                           const char8 * const functionName,
	                           void * const gamMemoryAddress){
	bool ret=MemoryMapOutputBroker::Init(direction,dataSourceIn, functionName, gamMemoryAddress);
	UARTCommunication *uartDS=dynamic_cast<UARTCommunication*>(dataSource);
	if(uartDS!=NULL){
		uartHandle=uartDS->GetHandle();
	}
	else{
		REPORT_ERROR(ErrorManagement::FatalError, "Dynamic cast from DataSourceI to UARTCommunication failed");
	}
	return ret;
}

CLASS_REGISTER(UARTCommunicationWriter, "1.0")


