/*
 * UDPCommunicationReader.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: gferro
 */

#include "UDPCommunicationReader.h"
#include "UDPCommunication.h"
#include "AdvancedErrorManagement.h"

UDPCommunicationReader::UDPCommunicationReader() :
		MemoryMapInputBroker() {
	// TODO Auto-generated constructor stub
	bufferAllocated = 0u;
	dataSourceUDP = NULL;
}

UDPCommunicationReader::~UDPCommunicationReader() {
	// TODO Auto-generated destructor stub
}

bool UDPCommunicationReader::Execute() {
	bool ret = true;

	if (bufferAllocated == 1) {
		dataSourceUDP->SyncBuffer();
		ret = MemoryMapInputBroker::Execute();
		//REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "reader ok");
	} else {
		uint32 n;
		for (n = 0u; (n < numberOfCopies); n++) {

			if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
				uint32 sizeToRead = copyTable[n].copySize;
				//read operation
				dataSourceUDP->ReceiveUdpData(copyTable[n].gamPointer,
						sizeToRead);
				REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Received %d",
						*(uint32*) (copyTable[n].gamPointer));
			}
		}
	}
	return ret;
}

bool UDPCommunicationReader::Init(const SignalDirection direction,
		DataSourceI &dataSourceIn, const char8 * const functionName,
		void * const gamMemoryAddress) {
	bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName,
			gamMemoryAddress);
	if (ret) {
		dataSourceUDP = dynamic_cast<UDPCommunication *>(dataSource);
		bufferAllocated = dataSourceUDP->IsBufferAllocated();
	}
	return ret;
}

CLASS_REGISTER(UDPCommunicationReader, "1.0")
