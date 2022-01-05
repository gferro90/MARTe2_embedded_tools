/**
 * @file GenericPeriphSyncWriter.cpp
 * @brief Source file for class GenericPeriphSyncWriter
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
 * the class GenericPeriphSyncWriter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GenericPeriphSyncWriter.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GenericPeriphSyncWriter::GenericPeriphSyncWriter() {
    // Auto-generated constructor stub for GenericPeriphSyncWriter
    // TODO Verify if manual additions are needed
}

GenericPeriphSyncWriter::~GenericPeriphSyncWriter() {
    // Auto-generated destructor stub for GenericPeriphSyncWriter
    // TODO Verify if manual additions are needed
}


bool GenericPeriphSyncWriter::Execute() {
    uint32 n;

    bool ret = true;
    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
        for (n = 0u; (n < numberOfCopies) && (ret); n++) {
            uint32 size = copyTable[n].copySize;
            ret = periphDriver->DriverWrite((const char8 *) (copyTable[n].gamPointer), size);
        }
    }

    if(ret){
        ret=periphDriver->Synchronise();
    }
    return ret;
}

CLASS_REGISTER(GenericPeriphSyncWriter, "1.0")

