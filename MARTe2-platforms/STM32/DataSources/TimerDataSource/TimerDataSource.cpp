/**
 * @file TimerDataSource.cpp
 * @brief Source file for class TimerDataSource
 * @date 12/06/2018
 * @author Giuseppe Ferro'
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
 * the class TimerDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "TimerDataSource.h"
#include "Threads.h"
#include "FastPollingMutexSem.h"
#include "HighResolutionTimer.h"
#include "HandleDatabase.h"
#include QUOTE(_HAL_H)

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static void TimerThreadFunction(TimerDataSource &timerDataSource) {
    float32 calibTodo = 1.0;
    float32 sleepTime = calibTodo * (timerDataSource.sleepCounter * HighResolutionTimer::Period());
    while (timerDataSource.running) {
        Sleep::Sec(sleepTime);
        timerDataSource.dataSourceTime += (uint64) (sleepTime * 1e6);
        //timerDataSource.triggered = true;
        timerDataSource.synchSem.Post();
    }
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TimerDataSource::TimerDataSource() :
        DataSourceI() {
    dataSourceTime = 0ull;
    synchSem.Create();
    synchSem.Reset();
    running = false;
    useThread = 0u;
    targetCounter = 0ull;
    phase = 0xFFFFFFFFu;
}

TimerDataSource::~TimerDataSource() {
    running = false;
}

bool TimerDataSource::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        if (!data.Read("UseThread", useThread)) {
            useThread = 0u;
        }
        if (!data.Read("Phase", phase)) {
            phase = 0xFFFFFFFFu;
        }
    }
    return ret;
}

bool TimerDataSource::Synchronise() {
    if (useThread > 0u) {
        //wait on the next stop triggered by the thread
        //each sleep time
        synchSem.ResetWait(TTInfiniteWait);
    } else {
        //wait until the time elapsed is greater than the sleep time
        //this would not release the cpu
        if (!running) {
            targetCounter = (HighResolutionTimer::Counter() + sleepCounter);
            if (phase < 0xFFFFFFFFu) {
                uint64 targetUs = targetCounter * HighResolutionTimer::Period() * 1e6;
                uint64 remaining = (1000000 - (targetUs % 1000000));
                targetCounter += (remaining + phase);
            }
            running = true;
        }
        while (1) {
            uint64 counter = HighResolutionTimer::Counter();
            if (counter >= targetCounter) {
                targetCounter += sleepCounter;
                dataSourceTime = (uint64) ((counter * HighResolutionTimer::Period()) * 1e6);
                break;
            }
        }
    }
    return true;
}

bool TimerDataSource::AllocateMemory() {
    return true;
}

uint32 TimerDataSource::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool TimerDataSource::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    bool ret = (signalIdx == 0u);
    if (ret) {
        signalAddress = reinterpret_cast<void*>(&dataSourceTime);
    }
    return ret;
}

const char8* TimerDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapSynchronisedInputBroker";
    }
    return NULL;
}

bool TimerDataSource::GetInputBrokers(ReferenceContainer &inputBrokers, const char8 *const functionName, void *const gamMemPtr) {
    ReferenceT < MemoryMapSynchronisedInputBroker > broker("MemoryMapSynchronisedInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "TimerDataSource::GetInputBrokers 0");
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "TimerDataSource::GetInputBrokers 1");
        ret = inputBrokers.Insert(broker);
    }

    return ret;
}

bool TimerDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8 *const functionName, void *const gamMemPtr) {
    return false;
}

bool TimerDataSource::SetConfiguredDatabase(StructuredDataI &data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);
    if (ret) {
        //only the time in us is provided
        ret = (GetNumberOfSignals() == 1);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Only one signal can be specified");
        }
    }

    if (ret) {
        //the type must be uint32 or uint64
        ret = (GetSignalType(0) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
        if (!ret) {
            ret = (GetSignalType(0) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint64"));
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The time signal type has to be uint32 or uint64");
            }
        }
    }

//this datasource is synchronising, so the frequency must be specified
    float32 frequency = 0.;
    if (ret) {
        ret = GetFunctionSignalReadFrequency(InputSignals, 0, 0, frequency);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Could not read the time signal frequency");
        }
    }

//the frequency must be >0
    if (ret) {
        ret = (frequency > 0.);
        if (ret) {
            sleepCounter = (uint32) ((1.F / frequency) * HighResolutionTimer::Frequency());
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The frequency has to be > 0");
        }
    }

    if (ret) {
        if (useThread > 0u) {
            running = true;
            Threads::BeginThread((ThreadFunctionType) TimerThreadFunction, this, 128);
        }
        if (phase < 0xFFFFFFFF) {
            uint32 periodUs = (uint32) ((1.F / frequency) * 1e6);
            ret = (phase < periodUs);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The phase %dus must be < than period %dus", phase, periodUs);
            }
        }
    }
    return ret;
}

bool TimerDataSource::PrepareNextState(const MARTe::char8 *const currentStateName, const MARTe::char8 *const nextStateName) {
    return true;
}

CLASS_REGISTER(TimerDataSource, "1.0")
