/**
 * @file EncodersReader_opt.cpp
 * @brief Source file for class EncodersReader_opt
 * @date 18/gen/2017
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
 * the class EncodersReader_opt (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "EncodersReader_opt.h"
#include "AdvancedErrorManagement.h"
#include "EncodersCounter_opt.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



EncodersReader_opt::EncodersReader_opt() :
        MemoryMapInputBroker() {
    // TODO Auto-generated constructor stub
    encoderDS = NULL;
}

EncodersReader_opt::~EncodersReader_opt() {
    // TODO Auto-generated destructor stub
}

bool EncodersReader_opt::Execute() {
    uint32 n;
    bool ret = true;
    for (n = 0u; (n < numberOfCopies) && (ret); n++) {
        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
            encoderDS->Read((int32*) copyTable[n].gamPointer);
        }
    }
    return ret;
}

bool EncodersReader_opt::Init(const SignalDirection direction,
                         DataSourceI &dataSourceIn,
                         const char8 * const functionName,
                         void * const gamMemoryAddress) {
    bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    encoderDS = dynamic_cast<EncodersCounter_opt*>(dataSource);
    if (encoderDS == NULL) {
        ret = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Dynamic cast from DataSourceI to EncodersCounter_opt failed");
    }
    return ret;
}

CLASS_REGISTER(EncodersReader_opt, "1.0")
