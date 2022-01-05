/**
 * @file L298NWriter.cpp
 * @brief Source file for class L298NWriter
 * @date 22/dic/2016
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
 * the class L298NWriter (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "L298NWriter.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

L298NWriter::L298NWriter() {
    // Auto-generated constructor stub for L298NWriter
    // TODO Verify if manual additions are needed
    dsHandle=NULL;
}

L298NWriter::~L298NWriter() {
    // Auto-generated destructor stub for L298NWriter
    // TODO Verify if manual additions are needed
}

bool L298NWriter::Execute() {
    uint32 n;
    //REPORT_ERROR(ErrorManagement::Warning, "Reading Enc");
    bool ret = true;
    for (n = 0u; (n < numberOfCopies) && (ret); n++) {
        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
                dsHandle->Write((int32*)copyTable[n].gamPointer);
        }
    }
    return ret;
}

bool L298NWriter::Init(const SignalDirection direction,
                          DataSourceI &dataSourceIn,
                          const char8 * const functionName,
                          void * const gamMemoryAddress) {
    bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    if (ret) {
        dsHandle = dynamic_cast<L298N*>(dataSource);
        ret = (dsHandle != NULL);
    }
    return ret;
}

CLASS_REGISTER(L298NWriter, "1.0")
