/*
 * USBCommunicationWriter.cpp
 *
 *  Created on: 23/ago/2016
 *      Author: pc
 */

#include "USBCommunicationWriter.h"
#include "USBCommunication.h"
#include "usbd_cdc_custom.h"
#include "usb_device.h"
#include "AdvancedErrorManagement.h"


USBCommunicationWriter::USBCommunicationWriter() :
        MemoryMapOutputBroker() {
    // TODO Auto-generated constructor stub
    writeNonBlocking = 0u;
    bufferAllocated = 0u;
    dataSourceUSB=NULL;
}

USBCommunicationWriter::~USBCommunicationWriter() {
    // TODO Auto-generated destructor stub
}

bool USBCommunicationWriter::Execute() {
    bool ret = true;
    if (bufferAllocated == 1) {
        dataSourceUSB->SyncBuffer();
        ret = MemoryMapOutputBroker::Execute();
        REPORT_ERROR(ErrorManagement::Warning, "writer ok");
    }
    else {
        uint32 n;

        if (dataSourceUSB->GetReadFlag() != 0) {
            for (n = 0u; (n < numberOfCopies) && (ret); n++) {
                if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
                    uint32 sizeToWrite = copyTable[n].copySize;
                    USBWrite((const char* const ) copyTable[n].gamPointer, (uint32_t*) &sizeToWrite, writeNonBlocking);
                    USBSync();
                }
            }
        }
    }
    return ret;
}

bool USBCommunicationWriter::Init(const SignalDirection direction,
                                  DataSourceI &dataSourceIn,
                                  const char8 * const functionName,
                                  void * const gamMemoryAddress) {
    bool ret = MemoryMapOutputBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    if (ret) {
        dataSourceUSB = dynamic_cast<USBCommunication *>(dataSource);
        writeNonBlocking = ((dataSourceUSB)->IsWriteBlocking() + 1) % 2;
        bufferAllocated=dataSourceUSB->IsBufferAllocated();

    }
    return ret;
}

CLASS_REGISTER(USBCommunicationWriter, "1.0")

