/*
 * UARTCommunicationReader.cpp
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#include "UARTCommunicationReader.h"
#include "AdvancedErrorManagement.h"
#include "UARTCommunication.h"

UARTCommunicationReader::UARTCommunicationReader():MemoryMapInputBroker() {
	// TODO Auto-generated constructor stub
	uartHandle=NULL;
}

UARTCommunicationReader::~UARTCommunicationReader() {
	// TODO Auto-generated destructor stub
}


bool UARTCommunicationReader::Execute() {
    uint32 n;
    bool ret = true;
    for (n = 0u; (n < numberOfCopies) && (ret); n++) {
        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
        	uint32 sizeToRead=copyTable[n].copySize;
            HAL_UART_Receive(uartHandle, (uint8_t*)copyTable[n].gamPointer, sizeToRead, HAL_MAX_DELAY);
        //char buff[32];

        //HAL_UART_Receive(uartHandle, (uint8_t*)buff, 32, HAL_MAX_DELAY);
        //HAL_UART_Transmit(uartHandle, (uint8_t*)"ciao\r\n", 7, HAL_MAX_DELAY);


        //REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,"Here %d",*(int*)(copyTable[n].gamPointer));
        }
    }
    return ret;
}

bool UARTCommunicationReader::Init(const SignalDirection direction,
	                           DataSourceI &dataSourceIn,
	                           const char8 * const functionName,
	                           void * const gamMemoryAddress){
	bool ret=MemoryMapInputBroker::Init(direction,dataSourceIn, functionName, gamMemoryAddress);
	UARTCommunication *uartDS=dynamic_cast<UARTCommunication*>(dataSource);
	if(uartDS!=NULL){
		uartHandle=uartDS->GetHandle();
	}
	else{
		REPORT_ERROR(ErrorManagement::FatalError, "Dynamic cast from DataSourceI to UARTCommunication failed");
	}
	return ret;
}


CLASS_REGISTER(UARTCommunicationReader, "1.0")
