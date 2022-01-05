/**
 * @file .cpp
 * @brief Source file for class
 * @date Jun 16, 2017
 * @author pc
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class  (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GenericStreamReader.h"
#include "GenericStreamDriver.h"
#include "AdvancedErrorManagement.h"

GenericStreamReader::GenericStreamReader() :
        MemoryMapInputBroker() {
    // TODO Auto-generated constructor stub
    bufferAllocated = 0u;
    driver = NULL;
}

GenericStreamReader::~GenericStreamReader() {
    // TODO Auto-generated destructor stub
}

bool GenericStreamReader::Execute() {
    bool ret = true;

    if (bufferAllocated != 0) {
        driver->SyncBuffer();
        ret = MemoryMapInputBroker::Execute();
        //REPORT_ERROR(ErrorManagement::Warning, "reader ok");
    }
    else {
        uint32 n;

        uint32 readSize = 0;
        for (n = 0u; (n < numberOfCopies) && (ret); n++) {

            if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
                uint32 sizeToRead = copyTable[n].copySize;
                driver->Read((char* const ) (copyTable[n].gamPointer), sizeToRead);
            }

        }

        driver->TerminateRead();
    }
    return ret;
}

bool GenericStreamReader::Init(const SignalDirection direction,
                               DataSourceI &dataSourceIn,
                               const char8 * const functionName,
                               void * const gamMemoryAddress) {
    bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    if (ret) {
        driver = dynamic_cast<GenericStreamDriver *>(dataSource);
        bufferAllocated = driver->IsBufferAllocated();
    }
    return ret;
}

CLASS_REGISTER(GenericStreamReader, "1.0")
