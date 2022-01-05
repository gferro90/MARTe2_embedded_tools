/*
 * UDPCommunicationWriter.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: gferro
 */


#include "UDPCommunicationWriter.h"
#include "UDPCommunication.h"
#include "AdvancedErrorManagement.h"


UDPCommunicationWriter::UDPCommunicationWriter() :
        MemoryMapOutputBroker() {
    // TODO Auto-generated constructor stub
    bufferAllocated = 0u;
    dataSourceUDP=NULL;
}

UDPCommunicationWriter::~UDPCommunicationWriter() {
    // TODO Auto-generated destructor stub
}

bool UDPCommunicationWriter::Execute() {
    bool ret = true;
    if (bufferAllocated == 1) {
        dataSourceUDP->SyncBuffer();
        ret = MemoryMapOutputBroker::Execute();
        //REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "writer ok");
    }
    else {
        uint32 n;

            for (n = 0u; (n < numberOfCopies) && (ret); n++) {
                if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
                    uint32 sizeToWrite = copyTable[n].copySize;
                    //TODO usb write op
        			//REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,"Executing %d", copyTable[n].copySize);
                    dataSourceUDP->TransferUdpData(copyTable[n].gamPointer, sizeToWrite);
                }
            }
    }
    return ret;
}

bool UDPCommunicationWriter::Init(const SignalDirection direction,
                                  DataSourceI &dataSourceIn,
                                  const char8 * const functionName,
                                  void * const gamMemoryAddress) {
    bool ret = MemoryMapOutputBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    if (ret) {
        dataSourceUDP = dynamic_cast<UDPCommunication *>(dataSource);
        bufferAllocated=dataSourceUDP->IsBufferAllocated();
    }
    return ret;
}

CLASS_REGISTER(UDPCommunicationWriter, "1.0")
