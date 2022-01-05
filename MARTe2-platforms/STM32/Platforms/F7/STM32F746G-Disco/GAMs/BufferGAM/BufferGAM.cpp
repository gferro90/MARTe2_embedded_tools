/**
 * @file BufferGAM.cpp
 * @brief Source file for class BufferGAM
 * @date 6 Aug 2016
 * @author andre
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
 * the class BufferGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BufferGAM.h"

#include "AdvancedErrorManagement.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
BufferGAM::BufferGAM() :
        GAM() {
    totalSignalsByteSize = 0u;
}

bool BufferGAM::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    bool ret = GAM::SetConfiguredDatabase(data);
    if (ret) {
        ret = (GetNumberOfInputSignals() == GetNumberOfOutputSignals());
    }
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetNumberOfInputSignals() != GetNumberOfOutputSignals()");
    }
    uint32 n;
    uint32 totalInputSignalsByteSize = 0u;
    for (n = 0u; (n < GetNumberOfInputSignals()) && (ret); n++) {
        uint32 byteSize;
        ret = GetSignalByteSize(InputSignals, n, byteSize);
        if (ret) {
            totalInputSignalsByteSize += byteSize;
        }
    }
    uint32 totalOutputSignalsByteSize = 0u;
    for (n = 0u; (n < GetNumberOfOutputSignals()) && (ret); n++) {
        uint32 byteSize;
        ret = GetSignalByteSize(OutputSignals, n, byteSize);
        if (ret) {
            totalOutputSignalsByteSize += byteSize;
        }
    }
    if (ret) {
        ret = (totalInputSignalsByteSize == totalOutputSignalsByteSize);
    }
    //Missing check for type compatibility of each signal...
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "InputSignalsByteSize != OutputSignalsByteSize");
    }
    if (ret) {
        totalSignalsByteSize = totalInputSignalsByteSize;
    }

    return ret;
}

bool BufferGAM::Execute() {
    return MemoryOperationsHelper::Copy(GetOutputSignalsMemory(), GetInputSignalsMemory(), totalSignalsByteSize);
}
CLASS_REGISTER(BufferGAM, "1.0")
