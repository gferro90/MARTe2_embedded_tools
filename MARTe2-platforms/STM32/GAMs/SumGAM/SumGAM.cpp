/**
 * @file SumGAM.cpp
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
#include "AdvancedErrorManagement.h"
#include "SumGAM.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
SumGAM::SumGAM() :
        GAM() {
}

bool SumGAM::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    bool ret = GAM::SetConfiguredDatabase(*(dynamic_cast<ConfigurationDatabase*>(&data)));
    if (ret) {
        ret = (GetNumberOfInputSignals() == 2u);
    }
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetNumberOfInputSignals() != 2");
    }
    TypeDescriptor desc;
    if (ret) {
        desc = GetSignalType(InputSignals, 0);
        ret = (desc == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
    }
    if (ret) {
        desc = GetSignalType(InputSignals, 1);
        ret = (desc == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
    }
    if (ret) {
        ret = (GetNumberOfOutputSignals() == 1u);
    }
    if (ret) {
        desc = GetSignalType(OutputSignals, 0);
        ret = (desc == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
    }

    return ret;
}

bool SumGAM::Execute() {
    uint32 *signal1 = static_cast<uint32 *>(GetInputSignalMemory(0));
    uint32 *signal2 = static_cast<uint32 *>(GetInputSignalMemory(1));
    uint32 *sum = static_cast<uint32 *>(GetOutputSignalMemory(0));
    *sum = *signal1 + *signal2;
    return true;
}

bool SumGAM::Setup(){
	return true;
}

CLASS_REGISTER(SumGAM, "1.0")
