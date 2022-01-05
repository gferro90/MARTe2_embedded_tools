/*
 * USBCommunicationReader.cpp
 *
 *  Created on: 23/ago/2016
 *      Author: pc
 */

#include "USBCommunicationReader.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"
#include "USBCommunication.h"
#include "AdvancedErrorManagement.h"

USBCommunicationReader::USBCommunicationReader() :
        MemoryMapInputBroker() {
    // TODO Auto-generated constructor stub
    readNonBlocking = 0u;
}

USBCommunicationReader::~USBCommunicationReader() {
    // TODO Auto-generated destructor stub
}

bool USBCommunicationReader::Execute() {
    uint32 n;
    bool ret = true;

    uint32 readSize=0;
    for (n = 0u; (n < numberOfCopies) && (ret); n++) {

        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
            uint32 sizeToRead = copyTable[n].copySize;
            USBRead((char* const ) (copyTable[n].gamPointer), (uint32_t*) &sizeToRead, readNonBlocking);

            // if(sizeToRead>0){
            //   	REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "received %d %d %d",((uint32*)copyTable[n].gamPointer)[0], ((uint32*)copyTable[n].gamPointer)[1], copyTable[n].copySize)
            // }
            readSize+=sizeToRead;
            USBSync();
        }


    }

    //it should be -1 if the syncronised read-write is disabled
    if (dataSourceUSB->GetReadFlag() >= 0) {

        dataSourceUSB->SetReadFlag(int8(readSize > 0));
    }
    return ret;
}

bool USBCommunicationReader::Init(const SignalDirection direction,
                                  DataSourceI &dataSourceIn,
                                  const char8 * const functionName,
                                  void * const gamMemoryAddress) {
    bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    if (ret) {
        dataSourceUSB = dynamic_cast<USBCommunication *>(dataSource);
        readNonBlocking = ((dataSourceUSB)->IsReadBlocking() + 1) % 2;
    }
    return ret;
}

CLASS_REGISTER(USBCommunicationReader, "1.0")
