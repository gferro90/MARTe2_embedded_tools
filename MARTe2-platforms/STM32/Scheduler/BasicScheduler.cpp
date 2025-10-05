/**
 * @file BasicScheduler.cpp
 * @brief Source file for class BasicScheduler
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
 * the class BasicScheduler (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicScheduler.h"
#include "BrokerI.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAM.h"
#include "GAMSchedulerI.h"
#include "RealTimeApplication.h"
#include "RealTimeThread.h"
#include "ReferenceContainerFilterReferences.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//extern int8 SM_changeState;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
BasicScheduler::BasicScheduler() {
    scheduledStates = NULL;
}

bool BasicScheduler::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);

    return ret;
}

bool BasicScheduler::ConfigureScheduler(Reference realTimeAppIn) {

    realTimeApp = realTimeAppIn;
    ReferenceT<RealTimeApplication> rtApp = realTimeApp;
    bool ret = rtApp.IsValid();

    ReferenceT<ReferenceContainer> statesContainer;
    if (ret) {
        statesContainer = rtApp->Find("States");
        ret = statesContainer.IsValid();
    }

    if (ret) {
        numberOfStates = statesContainer->Size();
        states = new ScheduledState[numberOfStates];
        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
            states[i].numberOfThreads = 0u;
            states[i].threads = NULL_PTR(ScheduledThread *);
        }
        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
            ReferenceT<RealTimeState> stateElement = statesContainer->Get(i);
            ret = stateElement.IsValid();
            if (ret) {
                ReferenceT<ReferenceContainer> threadContainer = stateElement->Find("Threads");
                ret = threadContainer.IsValid();
                if (ret) {
                    uint32 numberOfThreads = threadContainer->Size();

                    states[i].numberOfThreads = numberOfThreads;
                    states[i].name = stateElement->GetName();

                    states[i].threads = new ScheduledThread[numberOfThreads];

                    for (uint32 j = 0u; (j < numberOfThreads) && (ret); j++) {
                        ReferenceT<RealTimeThread> threadElement = threadContainer->Get(j);
                        ret = threadElement.IsValid();
                        if (ret) {

                            ReferenceContainer gams;
                            ret = threadElement->GetGAMs(gams);
                            uint32 numberOfGams = threadElement->GetNumberOfGAMs();
                            uint32 numberOfExecutables = numberOfGams;

                            for (uint32 k = 0u; (k < numberOfGams) && (ret); k++) {
                                ReferenceT<GAM> gam = gams.Get(k);
                                ret = gam.IsValid();
                                if (ret) {
                                    ReferenceContainer inputBrokers;
                                    ret = gam->GetInputBrokers(inputBrokers);
                                    if (ret) {
                                        numberOfExecutables += inputBrokers.Size();

                                    }
                                    if (ret) {
                                        ReferenceContainer outputBrokers;
                                        ret = gam->GetOutputBrokers(outputBrokers);
                                        if (ret) {
                                            numberOfExecutables += outputBrokers.Size();
                                        }
                                    }
                                }
                            }
                            if (ret) {
                                states[i].threads[j].executables = new ExecutableI*[numberOfExecutables];

                                states[i].threads[j].numberOfExecutables = numberOfExecutables;
                                states[i].threads[j].name = threadElement->GetName();
                                states[i].threads[j].cpu = threadElement->GetCPU();
                                states[i].threads[j].stackSize = threadElement->GetStackSize();
                            }
                            uint32 c = 0u;
                            for (uint32 k = 0u; (k < numberOfGams) && (ret); k++) {
                                //add input brokers
                                StreamString gamFullName;
                                ReferenceT<GAM> gam = gams.Get(k);
                                ret = gam->GetQualifiedName(gamFullName);
                                if (ret) {
                                    ret = InsertInputBrokers(gam, gamFullName.Buffer(), i, j, c);
                                }
                                //add gam
                                if (ret) {
                                    ret = InsertGAM(gam, gamFullName.Buffer(), i, j, c);
                                    if (ret) {
                                        c++;
                                    }
                                }

                                //add output brokers
                                if (ret) {
                                    ret = InsertOutputBrokers(gam, gamFullName.Buffer(), i, j, c);
                                }
                            }

                        }
                    }
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "Invalid RealTimeState in states container");
            }
        }
    }

    if (ret) {
        scheduledStates = GetSchedulableStates();
    }
    return ret;
}


bool BasicScheduler::InsertInputBrokers(ReferenceT<GAM> gam,
                                       const char8 * const gamFullName,
                                       const uint32 stateIdx,
                                       const uint32 threadIdx,
                                       uint32 &executableIdx) const{

    //add input brokers
    bool ret = (states != NULL);
    ReferenceContainer inputBrokers;
    if (ret) {
        ret = gam->GetInputBrokers(inputBrokers);
    }
    uint32 numberOfInputBrokers = 0u;
    if (ret) {
        numberOfInputBrokers = inputBrokers.Size();
    }
    for (uint32 n = 0u; (n < numberOfInputBrokers) && (ret); n++) {
        ReferenceT<ExecutableI> input = inputBrokers.Get(n);
        ret = input.IsValid();
        if (ret) {
            //lint -e{613} states != NULL checked before entering here.
            ret = (states[stateIdx].threads != NULL_PTR(ScheduledThread *));
        }
        if (ret) {
            //lint -e{613} states != NULL checked before entering here.
            states[stateIdx].threads[threadIdx].executables[executableIdx] = input.operator->();
        }
        executableIdx++;
    }

    return ret;
}

bool BasicScheduler::InsertGAM(ReferenceT<GAM> gam,
                              const char8 * const gamFullName,
                              const uint32 stateIdx,
                              const uint32 threadIdx,
                              const uint32 executableIdx) const {

    bool ret = (states != NULL);
    if (ret) {
        //lint -e{613} states != NULL checked before entering here.
        ret = (states[stateIdx].threads != NULL);
    }
    if (ret) {
        //lint -e{613} states != NULL checked before entering here.
        states[stateIdx].threads[threadIdx].executables[executableIdx] = gam.operator->();
    }
    return ret;
}

bool BasicScheduler::InsertOutputBrokers(ReferenceT<GAM> gam,
                                        const char8 * const gamFullName,
                                        const uint32 stateIdx,
                                        const uint32 threadIdx,
                                        uint32 &executableIdx) const {
    bool ret = (states != NULL);
    if (ret) {
        //lint -e{613} states != NULL checked before entering here.
        ret = (states[stateIdx].threads != NULL);
    }
    ReferenceContainer outputBrokers;
    if (ret) {
        ret = gam->GetOutputBrokers(outputBrokers);
    }
    uint32 numberOfOutputBrokers = 0u;
    if (ret) {
        numberOfOutputBrokers = outputBrokers.Size();
    }
    for (uint32 n = 0u; (n < numberOfOutputBrokers) && (ret); n++) {
        ReferenceT<ExecutableI> output = outputBrokers.Get(n);
        ret = output.IsValid();
        if (ret) {
            //lint -e{613} states != NULL checked before entering here.
            states[stateIdx].threads[threadIdx].executables[executableIdx] = output.operator->();
        }
        executableIdx++;
    }
    return ret;
}


ErrorManagement::ErrorType BasicScheduler::StartNextStateExecution() {
    ErrorManagement::ErrorType err;
    //while (!SM_changeState) {
    while (1) {
        //run the first thread
        Cycle(0);
    }
    return err;
}

ErrorManagement::ErrorType BasicScheduler::StopCurrentStateExecution() {
    ErrorManagement::ErrorType err;
    return err;
}

void BasicScheduler::Cycle(uint32 threadId) {
    ReferenceT<RealTimeApplication> rtApp=realTimeApp;
    ExecuteSingleCycle(scheduledStates[rtApp->GetIndex()]->threads[threadId].executables,
                       scheduledStates[rtApp->GetIndex()]->threads[threadId].numberOfExecutables);
}


bool BasicScheduler::ExecuteSingleCycle(ExecutableI * const * const executables,
                                       const uint32 numberOfExecutables) const {
    // warning: possible segmentation faults if the previous operations
    // lack or fail and the pointers are invalid.

    bool ret = true;
    for (uint32 i = 0u; (i < numberOfExecutables) && (ret); i++) {
        // save the time before
        // execute the gam
        ret = executables[i]->Execute();
        if (!ret) {
            BrokerI *broker = dynamic_cast<BrokerI *>(executables[i]);
            if (broker != NULL_PTR(BrokerI *)) {
                StreamString ownerFunctionName = broker->GetOwnerFunctionName();
                StreamString ownerDataSourceName = broker->GetOwnerDataSourceName();
                StreamString brokerName = "unnamed";
                if (broker->GetName() != NULL_PTR(const char8 * const)) {
                    brokerName = broker->GetName();
                }
                REPORT_ERROR (ErrorManagement::Warning, "BrokerI %s failed, owner function: %s, owner DataSource: %s", brokerName.Buffer(), ownerFunctionName.Buffer(), ownerDataSourceName.Buffer());
            }
            else {
                Object *obj = dynamic_cast<Object *>(executables[i]);
                if (obj != NULL_PTR(Object *)) {
                    REPORT_ERROR (ErrorManagement::Warning, "ExecutableI %s failed", obj->GetName());
                }
            }
        }
    }

    return ret;
}

void BasicScheduler::CustomPrepareNextState(){
}
CLASS_REGISTER(BasicScheduler, "1.0")
