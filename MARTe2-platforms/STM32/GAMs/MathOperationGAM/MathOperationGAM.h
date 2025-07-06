/**
 * @file MathOperationGAM.h
 * @brief Header file for class BufferGAM
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

 * @details This header file contains the declaration of the class BufferGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATHOPERATION_H_
#define MATHOPERATION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
#define REGISTERED_OPERATIONS 5

/**
 * @brief A GAM to perform math operations
 *
 * @details The developer can register math operations.
 * Current math operations registered are:
 * - + : sum operation. The output signal is the sum of the input signals. Multiple input signals but only one output is allowed.
 * - & : bitwise and. The output signal is the bitwise [and] of the inputs. Multiple input signals but only one output is allowed. Signals must be integer type.
 * - | : bitwise or. The output signal is the bitwise [or] of the inputs. Multiple input signals but only one output is allowed. Signals must be integer type.
 * - ^ : bitwise xor. The output signal is the bitwise [xor] of the inputs. Multiple input signals but only one output is allowed. Signals must be integer type.
 * - ~ : bitwise inversion. The output signal is the bitwise [inv] of the input. Only one input and only one output is allowed. Signals must be integer type.
 */
class MathOperationGAM: public GAM {
public:CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    MathOperationGAM();

    /**
     * @brief Destructor
     */
    virtual ~MathOperationGAM();

    /**
     * @brief Configures the GAM
     * @details The user must define:
     *  - Operation : one of the registered operations
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Executes the operation specified by the user
     */
    virtual bool Execute();

    /**
     * @brief Performs the checks (number of signals, types, etc.)
     */
    virtual bool Setup();

    /**
     * @brief Finds the specified operation among the registered ones
     */
    uint32 GetOperationIndex(const char8 *operationIn);

    /**
     * @brief Performs the setup for operations multiple inputs, single outputs.
     */
    bool Setup_MISO();

    /**
     * @brief Performs the setup for operations multiple inputs, single outputs, integer type.
     */
    bool Setup_MISO_INT();

    /**
     * @brief Performs the setup for operations single inputs, single outputs, integer type
     */
    bool Setup_SISO_INT();

    /**
     * @brief Executes the sum operation
     */
    bool ExecuteSum();

    /**
     * @brief Executes the bitwise and operation
     */
    bool ExecuteBitwiseAnd();

    /**
     * @brief Executes the bitwise or operation
     */
    bool ExecuteBitwiseOr();

    /**
     * @brief Executes the bitwise xor operation
     */
    bool ExecuteBitwiseXor();

    /**
     * @brief Executes the bitwise invert operation
     */
    bool ExecuteBitwiseInv();

    /**
     * @brief Template sum function
     */
    template<typename T>
    void Sum(T &sum);

    /**
     * @brief Template bitwise and function
     */
    template<typename T>
    void BitwiseAnd(T &sum);

    /**
     * @brief Template bitwise or function
     */
    template<typename T>
    void BitwiseOr(T &sum);

    /**
     * @brief Template bitwise xor function
     */
    template<typename T>
    void BitwiseXor(T &sum);

    /**
     * @brief Template bitwise invert function
     */
    template<typename T>
    void BitwiseInv(T &sum);

private:

    /**
     * Operation index in the registered operations list
     */
    uint32 opIndex;

    /**
     * Input type id
     */
    uint32 inputTypeIdx;

    /**
     * Output type id
     */
    uint32 outputTypeIdx;

    /**
     * Setup functions list
     */
    bool (MathOperationGAM::*OperationSetup[REGISTERED_OPERATIONS])();

    /**
     * Execute functions list
     */
    bool (MathOperationGAM::*OperationExecute[REGISTERED_OPERATIONS])(void);

    /**
     * Operations list
     */
    const char8 *operation[REGISTERED_OPERATIONS];

    uint32 lastSize;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T>
void MathOperationGAM::Sum(T &sum) {
    MemoryOperationsHelper::Set(&sum, 0, sizeof(T));
    uint32 ptr_1 = (uint32)(inputSignalsMemoryIndexer[0]);
    for (uint32 i = 1u; i < numberOfInputSignals; i++) {
        uint32 ptr = (uint32) inputSignalsMemoryIndexer[i];
        uint32 size = (ptr - ptr_1);
        ptr_1 = ptr;
        T operand;
        MemoryOperationsHelper::Set(&operand, 0, sizeof(T));
        MemoryOperationsHelper::Copy(&operand, inputSignalsMemoryIndexer[i - 1], size);
        sum += operand;
    }
    {
        T operand;
        MemoryOperationsHelper::Set(&operand, 0, sizeof(T));
        MemoryOperationsHelper::Copy(&operand, inputSignalsMemoryIndexer[numberOfInputSignals - 1], lastSize);
        sum += operand;
    }
}

template<typename T>
void MathOperationGAM::BitwiseAnd(T &result) {
    MemoryOperationsHelper::Set(&result, 0xFF, sizeof(T));
    uint32 ptr_1 = (uint32)(inputSignalsMemoryIndexer[0]);
    for (uint32 i = 1u; i < numberOfInputSignals; i++) {
        uint32 ptr = (uint32) inputSignalsMemoryIndexer[i];
        uint32 size = (ptr - ptr_1);
        ptr_1 = ptr;
        T operand;
        MemoryOperationsHelper::Set(&operand, 0, sizeof(T));
        MemoryOperationsHelper::Copy(&operand, inputSignalsMemoryIndexer[i - 1], size);
        result &= operand;
    }
    {
        T operand;
        MemoryOperationsHelper::Set(&operand, 0, sizeof(T));
        MemoryOperationsHelper::Copy(&operand, inputSignalsMemoryIndexer[numberOfInputSignals - 1], lastSize);
        result &= operand;
    }
}

template<typename T>
void MathOperationGAM::BitwiseOr(T &result) {
    MemoryOperationsHelper::Set(&result, 0, sizeof(T));
    uint32 ptr_1 = (uint32)(inputSignalsMemoryIndexer[0]);
    for (uint32 i = 1u; i < numberOfInputSignals; i++) {
        uint32 ptr = (uint32) inputSignalsMemoryIndexer[i];
        uint32 size = (ptr - ptr_1);
        ptr_1 = ptr;
        T operand;
        MemoryOperationsHelper::Set(&operand, 0, sizeof(T));
        MemoryOperationsHelper::Copy(&operand, inputSignalsMemoryIndexer[i - 1], size);
        result |= operand;
    }
    {
        T operand;
        MemoryOperationsHelper::Set(&operand, 0, sizeof(T));
        MemoryOperationsHelper::Copy(&operand, inputSignalsMemoryIndexer[numberOfInputSignals - 1], lastSize);
        result |= operand;
    }
}

template<typename T>
void MathOperationGAM::BitwiseXor(T &result) {
    MemoryOperationsHelper::Set(&result, 0, sizeof(T));
    uint32 ptr_1 = (uint32)(inputSignalsMemoryIndexer[0]);
    for (uint32 i = 1u; i < numberOfInputSignals; i++) {
        uint32 ptr = (uint32) inputSignalsMemoryIndexer[i];
        uint32 size = (ptr - ptr_1);
        ptr_1 = ptr;
        T operand;
        MemoryOperationsHelper::Set(&operand, 0, sizeof(T));
        MemoryOperationsHelper::Copy(&operand, inputSignalsMemoryIndexer[i - 1], size);
        if (i == 1u) {
            result = operand;
        } else {
            result ^= operand;
        }
    }
    {
        T operand;
        MemoryOperationsHelper::Set(&operand, 0, sizeof(T));
        MemoryOperationsHelper::Copy(&operand, inputSignalsMemoryIndexer[numberOfInputSignals - 1], lastSize);
        if (numberOfInputSignals == 1u) {
            result = operand;
        } else {
            result ^= operand;
        }
    }
}

template<typename T>
void MathOperationGAM::BitwiseInv(T &result) {
    {
        T operand;
        MemoryOperationsHelper::Set(&operand, 0, sizeof(T));
        MemoryOperationsHelper::Copy(&operand, inputSignalsMemoryIndexer[0], lastSize);
        result = ~operand;
    }
}

#endif /* MATHOPERATION_H_ */

