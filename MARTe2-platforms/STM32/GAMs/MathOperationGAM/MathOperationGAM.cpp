/**
 * @file MathOperationGAM.cpp
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
#include "MathOperationGAM.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

enum {
    U8_ID = 0, I8_ID, U16_ID, I16_ID, U32_ID, I32_ID, U64_ID, I64_ID, F32_ID, F64_ID
};

static uint32 GetTypeWeight(const char8 *type) {
    const char8 *types[] = { "uint8", "int8", "uint16", "int16", "uint32", "int32", "uint64", "int64", "float32", "float64", NULL };
    uint32 i = 0u;
    while (types[i] != NULL) {
        if (StringHelper::Compare(types[i], type) == 0) {
            return i;
        }
        i++;
    }
    return i;
}

static void MathOperationGAMCast(uint32 outputType, void *input, void *output) {
    if ((outputType >= U8_ID) && (outputType <= I8_ID)) {
        MemoryOperationsHelper::Copy(output, input, sizeof(uint8));
    } else if ((outputType >= U16_ID) && (outputType <= I16_ID)) {
        MemoryOperationsHelper::Copy(output, input, sizeof(uint16));
    } else if (((outputType >= U32_ID) && (outputType <= I32_ID)) || (outputType == F32_ID)) {
        MemoryOperationsHelper::Copy(output, input, sizeof(uint32));
    } else if (((outputType >= U64_ID) && (outputType <= I64_ID)) || (outputType == F64_ID)) {
        MemoryOperationsHelper::Copy(output, input, sizeof(uint64));
    }
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
MathOperationGAM::MathOperationGAM() :
        GAM() {
    opIndex = 0u;
    inputTypeIdx = 0u;
    outputTypeIdx = 0u;
    lastSize = 0u;
    operation[0] = "+";
    OperationSetup[0] = &MathOperationGAM::Setup_MISO;
    OperationExecute[0] = &MathOperationGAM::ExecuteSum;
    operation[1] = "&";
    OperationSetup[1] = &MathOperationGAM::Setup_MISO_INT;
    OperationExecute[1] = &MathOperationGAM::ExecuteBitwiseAnd;
    operation[2] = "|";
    OperationSetup[2] = &MathOperationGAM::Setup_MISO_INT;
    OperationExecute[2] = &MathOperationGAM::ExecuteBitwiseOr;
    operation[3] = "^";
    OperationSetup[3] = &MathOperationGAM::Setup_MISO_INT;
    OperationExecute[3] = &MathOperationGAM::ExecuteBitwiseXor;
    operation[4] = "~";
    OperationSetup[4] = &MathOperationGAM::Setup_SISO_INT;
    OperationExecute[4] = &MathOperationGAM::ExecuteBitwiseInv;
}

MathOperationGAM::~MathOperationGAM() {

}

uint32 MathOperationGAM::GetOperationIndex(const char8 *operationIn) {
    for (uint32 i = 0u; i < REGISTERED_OPERATIONS; i++) {
        if (StringHelper::Compare(operationIn, operation[i]) == 0) {
            return i;
        }
    }
    return 0xFFFFFFFFu;
}

bool MathOperationGAM::Initialise(StructuredDataI &data) {
    bool ret = GAM::Initialise(data);
    if (ret) {
        StreamString operationIn;
        ret = data.Read("Operation", operationIn);
        if (ret) {
            opIndex = GetOperationIndex(operationIn.Buffer());
            if (opIndex == 0xFFFFFFFFu) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Operation %s not recognized", operationIn.Buffer());
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Operation must be defined");
        }
    }
    return ret;
}

bool MathOperationGAM::Setup_MISO() {
    bool ret = (numberOfOutputSignals == 1u) && (numberOfInputSignals > 0u);
    if (ret) {
        inputTypeIdx = 0u;
        for (uint32 i = 0u; i < numberOfInputSignals; i++) {
            TypeDescriptor td = GetSignalType(InputSignals, i);
            uint32 tweight = GetTypeWeight(TypeDescriptor::GetTypeNameFromTypeDescriptor(td));
            if (tweight > inputTypeIdx) {
                inputTypeIdx = tweight;
            }
        }
        TypeDescriptor outTd = GetSignalType(OutputSignals, 0u);
        outputTypeIdx = GetTypeWeight(TypeDescriptor::GetTypeNameFromTypeDescriptor(outTd));
    } else {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Operation %s requires #inputs>0 && #outputs==1", operation[opIndex]);
    }
    return ret;
}

bool MathOperationGAM::Setup_MISO_INT() {
    bool ret = (numberOfOutputSignals == 1u) && (numberOfInputSignals > 0u);
    if (ret) {
        inputTypeIdx = 0u;
        for (uint32 i = 0u; (i < numberOfInputSignals) && ret; i++) {
            TypeDescriptor td = GetSignalType(InputSignals, i);
            ret = ((td != Float32Bit) && (td != Float64Bit));
            if (ret) {
                uint32 tweight = GetTypeWeight(TypeDescriptor::GetTypeNameFromTypeDescriptor(td));
                if (tweight > outputTypeIdx) {
                    outputTypeIdx = tweight;
                }
            } else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Operation %s requires all inputs to be integer type", operation[opIndex]);
            }
        }
        TypeDescriptor outTd = GetSignalType(OutputSignals, 0u);
        outputTypeIdx = GetTypeWeight(TypeDescriptor::GetTypeNameFromTypeDescriptor(outTd));
    } else {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Operation %s requires #inputs>0 && #outputs==1", operation[opIndex]);
    }
    return ret;
}

bool MathOperationGAM::Setup_SISO_INT() {
    bool ret = (numberOfOutputSignals == 1u) && (numberOfInputSignals == 1u);
    if (ret) {
        inputTypeIdx = 0u;
        TypeDescriptor td = GetSignalType(InputSignals, 0);
        ret = ((td != Float32Bit) && (td != Float64Bit));
        if (ret) {
            uint32 tweight = GetTypeWeight(TypeDescriptor::GetTypeNameFromTypeDescriptor(td));
            if (tweight > outputTypeIdx) {
                outputTypeIdx = tweight;
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Operation %s requires all inputs to be integer type", operation[opIndex]);
        }
        TypeDescriptor outTd = GetSignalType(OutputSignals, 0u);
        outputTypeIdx = GetTypeWeight(TypeDescriptor::GetTypeNameFromTypeDescriptor(outTd));
    } else {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Operation %s requires #inputs>0 && #outputs==1", operation[opIndex]);
    }
    return ret;
}

bool MathOperationGAM::ExecuteSum() {

    if (inputTypeIdx < F32_ID) {
        uint64 sum = 0ull;
        Sum(sum);
        MathOperationGAMCast(outputTypeIdx, &sum, outputSignalsMemory);
    } else {
        float64 sum = 0.;
        Sum(sum);
        MathOperationGAMCast(outputTypeIdx, &sum, outputSignalsMemory);
    }

    return true;
}

bool MathOperationGAM::ExecuteBitwiseAnd() {
    uint64 result = 0ull;
    BitwiseAnd(result);
    MathOperationGAMCast(outputTypeIdx, &result, outputSignalsMemory);
    return true;
}

bool MathOperationGAM::ExecuteBitwiseOr() {
    uint64 result = 0ull;
    BitwiseOr(result);
    MathOperationGAMCast(outputTypeIdx, &result, outputSignalsMemory);
    return true;
}

bool MathOperationGAM::ExecuteBitwiseXor() {
    uint64 result = 0ull;
    BitwiseXor(result);
    MathOperationGAMCast(outputTypeIdx, &result, outputSignalsMemory);
    return true;
}

bool MathOperationGAM::ExecuteBitwiseInv() {
    uint64 result = 0ull;
    BitwiseInv(result);
    MathOperationGAMCast(outputTypeIdx, &result, outputSignalsMemory);
    return true;
}

bool MathOperationGAM::Setup() {
    bool ret = (this->*OperationSetup[this->opIndex])();
    if (ret) {
        if (numberOfInputSignals > 0u) {
            GetSignalByteSize(InputSignals, numberOfInputSignals - 1u, lastSize);
        }
    }
    return ret;
}

bool MathOperationGAM::Execute() {
    bool ret = (this->*OperationExecute[this->opIndex])();
    return ret;
}

CLASS_REGISTER(MathOperationGAM, "1.0")
