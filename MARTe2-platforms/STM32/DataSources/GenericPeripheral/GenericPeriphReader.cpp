/**
 * @file GenericPeriphReader.cpp
 * @brief Source file for class GenericPeriphReader
 * @date 20 giu 2018
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
 * the class GenericPeriphReader (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GenericPeriphReader.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GenericPeriphReader::GenericPeriphReader() :
        MemoryMapBroker() {
    // Auto-generated constructor stub for GenericPeriphReader
    // TODO Verify if manual additions are needed
    periphDriver = NULL;
}

GenericPeriphReader::~GenericPeriphReader() {
    // Auto-generated destructor stub for GenericPeriphReader
    // TODO Verify if manual additions are needed
}

bool GenericPeriphReader::Execute() {

    uint32 n;
    /*lint -e{613} null pointer checked before.*/
    uint32 i = dataSource->GetCurrentStateBuffer();
    bool ret = true;
    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
        for (n = 0u; (n < numberOfCopies) && (ret); n++) {
            uint32 size = copyTable[n].copySize;
            ret = periphDriver->DriverRead((char8 *) (copyTable[n].gamPointer), size);
        }
    }
    return ret;
}

bool GenericPeriphReader::Init(const SignalDirection direction,
                               DataSourceI &dataSourceIn,
                               const char8 * const functionName,
                               void * const gamMemoryAddress) {
    bool ret = MemoryMapBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    if (ret) {
        periphDriver = dynamic_cast<GenericPeripheral *>(dataSource);
        ret = (periphDriver != NULL);
    }
    return ret;
}
CLASS_REGISTER(GenericPeriphReader, "1.0")

