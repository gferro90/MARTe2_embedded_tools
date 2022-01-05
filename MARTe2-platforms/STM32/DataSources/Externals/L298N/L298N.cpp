/**
 * @file L298N.cpp
 * @brief Source file for class L298N
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
 * the class L298N (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "L298N.h"
#include "L298NWriter.h"
#include "AdvancedErrorManagement.h"
#include "HandleDatabase.h"

static const uint32 channelNames[] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4 };

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

L298N::L298N() {
    // Auto-generated constructor stub for L298N
    // TODO Verify if manual additions are needed
    dirPort = NULL;
    pwms = NULL;
    dirPin = NULL;
    numberOfElements = 0u;
    channel = NULL;
    maxPwm=NULL;
}

L298N::~L298N() {
    // Auto-generated destructor stub for L298N
    // TODO Verify if manual additions are needed
    for (uint32 i = 0u; i < numberOfElements; i++) {
        HAL_TIM_PWM_Stop(pwms[i], channelNames[channel[i]]);
    }

    if (dirPort != NULL) {
        delete[] dirPort;
    }

    if (dirPin != NULL) {
        delete[] dirPin;
    }

    if (pwms != NULL) {
        delete[] pwms;
    }
    if (channel != NULL) {
        delete[] channel;
    }
    if (maxPwm != NULL) {
        delete[] maxPwm;
    }
}

bool L298N::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {

        //read pwms
        AnyType pwmAT = data.GetType("TimIdentifiers");
        ret=!pwmAT.IsVoid();
        if (ret) {
            numberOfElements = pwmAT.GetNumberOfElements(0u);
            Vector < uint8 > pwmNums(numberOfElements);
            ret = data.Read("TimIdentifiers", pwmNums);
            if (ret) {
                pwms = new TIM_HandleTypeDef *[numberOfElements];
                char8 timIdArr[] = { 'T', 'I', 'M', 0, 0, 0 };

                for (uint8 i = 0u; i < numberOfElements && ret; i++) {
                    uint8 index = 3u;
                    for (uint8 k = 0u; k < 3; k++) {
                        timIdArr[index + k] = 0;
                    }

                    while (pwmNums[i] > 0u) {
                        if (index > 4u) {
                            ret = false;
                            REPORT_ERROR(ErrorManagement::InitialisationError, "The timer number can not exceed 2 digits");
                            break;
                        }
                        for (uint8 k = index; k > 3u; k--) {
                            timIdArr[k] = timIdArr[k - 1];
                        }
                        timIdArr[3] = (pwmNums[i] % 10) + '0';
                        pwmNums[i] /= 10;
                        index++;
                    }
                    if (ret) {
                        pwms[i] = (TIM_HandleTypeDef *) GetHandle(timIdArr);
                        ret = (pwms[i] != NULL);
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Timer %s not found", timIdArr);
                        }
                    }
                }
            }
        }

        // read gpio ports
        if (ret) {
            Vector < uint8 > dirPortVec(numberOfElements);
            ret = data.Read("DirPorts", dirPortVec);
            if (ret) {
                dirPort = new GPIO_TypeDef*[numberOfElements];
                char8 dirPortIdArr[] = { 'G', 'P', 'I', 'O', 0, 0 };

                for (uint8 i = 0u; i < numberOfElements && ret; i++) {
                    ret = (dirPortVec[i] <= 5u);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "The GPIO number must be < 5");
                        break;
                    }
                    for (uint8 k = 0u; k < 2; k++) {
                        dirPortIdArr[4 + k] = 0;
                    }
                    dirPortIdArr[4] = 'A' + dirPortVec[i];
                    dirPort[i] = (GPIO_TypeDef *) GetHandle(dirPortIdArr);
                    ret = (dirPort[i] != NULL);
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "GPIO %s not found", dirPortIdArr);
                    }
                }
            }
        }

        //read pwms channel
        if (ret) {
            channel = new uint8[numberOfElements];
            Vector < uint8 > channelsVec(channel, numberOfElements);
            ret = data.Read("Channels", channelsVec);
        }

        // read dir pins
        if (ret) {
            dirPin = new uint8[numberOfElements];
            Vector < uint8 > dirPinVec(dirPin, numberOfElements);
            ret = data.Read("DirPins", dirPinVec);
        }

        if (ret) {
            maxPwm = new uint32[numberOfElements];
            Vector < uint32 > maxPwmVec(maxPwm, numberOfElements);
            ret = data.Read("MaxPwm", maxPwmVec);
        }


    }

    return ret;
}

bool L298N::Synchronise() {
    return true;
}

bool L298N::AllocateMemory() {
    return true;
}

uint32 L298N::GetNumberOfMemoryBuffers() {

    return 1u;
}

bool L298N::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);

    if (ret) {
        uint32 numberOfSignals = GetNumberOfSignals();

        for (uint32 i = 0u; i < numberOfSignals && ret; i++) {

            ret = (GetSignalType(i) == TypeDescriptor::GetTypeDescriptorFromTypeName("int32"));
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The signals type has to be int32");
            }

            if (ret) {
                uint32 numberOfElements_rec = 0u;
                ret = GetSignalNumberOfElements(i, numberOfElements_rec);
                if (ret) {
                    ret = numberOfElements_rec == numberOfElements;
                }
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "All signals must have the number of elements equal to the number of pwms");
                }
            }
        }
    }

    if (ret) {
        for (uint8 i = 0u; i < numberOfElements; i++) {
            if (IS_TIM_CCX_INSTANCE(pwms[i]->Instance, channelNames[channel[i]])) {

                __HAL_TIM_SET_COMPARE(pwms[i], channelNames[channel[i]], 0);
                HAL_TIM_PWM_Start(pwms[i], channelNames[channel[i]]);

            }
            else {
                ret = false;
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "The Channel %d is not available for the respective tim", channel[i]);
            }
        }
    }

    return ret;
}

bool L298N::GetSignalMemoryBuffer(const uint32 signalIdx,
                                  const uint32 bufferIdx,
                                  void *&signalAddress) {
    signalAddress = NULL;
    return true;
}

const char8 *L298N::GetBrokerName(StructuredDataI &data,
                                  const SignalDirection direction) {

    if (direction == OutputSignals) {
        return "L298NWriter";
    }
    return "";
}

bool L298N::GetInputBrokers(ReferenceContainer &inputBrokers,
                            const char8* const functionName,
                            void * const gamMemPtr) {

    return false;
}

bool L298N::GetOutputBrokers(ReferenceContainer &outputBrokers,
                             const char8* const functionName,
                             void * const gamMemPtr) {

    ReferenceT < L298NWriter > broker("L298NWriter");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool L298N::PrepareNextState(const char8 * const currentStateName,
                             const char8 * const nextStateName) {
    return true;
}

void L298N::Write(int32 *vals) {
    for (uint32 i = 0u; i < numberOfElements; i++) {
        if (vals[i] < 0) {
            __HAL_TIM_SET_COMPARE(pwms[i], channelNames[channel[i]], maxPwm[i]+vals[i]);
             dirPort[i]->BSRR |= (1 << dirPin[i]);//set negative direction
         }
         else {
             __HAL_TIM_SET_COMPARE(pwms[i], channelNames[channel[i]], vals[i]);
             dirPort[i]->BSRR |= (1 << (16+dirPin[i]));//set positive direction
         }
    }
}

CLASS_REGISTER(L298N, "1.0")
