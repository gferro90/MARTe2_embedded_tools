/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "RobotHomeGAM.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RobotHomeGAM::RobotHomeGAM() :
        GAM() {
    numberOfMotors = 0u;
    directionMask = 0u;
    deadBand = 20;
    step = 2000;

    endSwitches = NULL;
    measures = NULL;
    references = NULL;
}

RobotHomeGAM::~RobotHomeGAM() {

}

bool RobotHomeGAM::Initialise(StructuredDataI &data) {
    bool ret = GAM::Initialise(data);
    if (ret) {
        if (!data.Read("DirectionMask", directionMask)) {
            directionMask = 0u;
            REPORT_ERROR(ErrorManagement::Information, "DirectionMask not defined. Set to %d", directionMask);
        }
        if (!data.Read("DeadBand", deadBand)) {
            deadBand = 20;
            REPORT_ERROR(ErrorManagement::Information, "DeadBand not defined. Set to %d", deadBand);
        }
        if (!data.Read("Step", step)) {
            step = 2000;
            REPORT_ERROR(ErrorManagement::Information, "Step not defined. Set to %d", step);
        }
    }

    return ret;
}

bool RobotHomeGAM::Setup() {
    numberOfMotors = (numberOfInputSignals - 1u);
    bool ret = (numberOfMotors == numberOfOutputSignals);
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "NumberOfOutputs=%d must be (NumberOfInputs - 1)=%d", numberOfOutputSignals, numberOfMotors);
    }
    if (ret) {
        ret = (numberOfMotors <= 16u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Cannot handle more than 16 motors");
        }
    }
    if (ret) {
        TypeDescriptor td = GetSignalType(InputSignals, 0u);
        ret = (td == UnsignedInteger32Bit);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Input signal 0 (end switches) must be uint32");
        }
    }
    if (ret) {
        for (uint32 i = 1u; i < numberOfInputSignals; i++) {
            TypeDescriptor td = GetSignalType(InputSignals, i);
            ret = (td == SignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Input signal %d (measure) must be int32", i);
            }
        }
    }
    if (ret) {
        endSwitches = (uint32*) (inputSignalsMemoryIndexer[0]);
        measures = (int32*) (inputSignalsMemoryIndexer[1]);
    }
    if (ret) {
        for (uint32 i = 0u; i < numberOfOutputSignals; i++) {
            TypeDescriptor td = GetSignalType(OutputSignals, i);
            ret = (td == SignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Output signal %d (reference) must be int32", i);
            }
        }
    }
    if (ret) {
        references = (int32*) (outputSignalsMemoryIndexer[0]);
    }
    return ret;
}

bool RobotHomeGAM::Execute() {

    for (uint8 i = 0u; i < numberOfMotors; i++) {
        bool isSwitch1 = (((*endSwitches) & (1 << (2 * i))) > 0);
        bool isSwitch2 = (((*endSwitches) & (1 << ((2 * i) + 1))) > 0);

        bool isSwitch = (isSwitch1 || isSwitch2);

        int32 direction = (((directionMask >> i) & (0x1u)) == 0u) ? (1) : (-1);
        if (isSwitch) {
            references[i] = (measures[i]);
        } else {
            if (((references[i] - measures[i]) < deadBand) && ((references[i] - measures[i]) > -deadBand)) {
                references[i] = (measures[i] + (direction * step));
            }
        }
    }

    return true;
}

CLASS_REGISTER(RobotHomeGAM, "1.0")

} /* namespace MARTe */

