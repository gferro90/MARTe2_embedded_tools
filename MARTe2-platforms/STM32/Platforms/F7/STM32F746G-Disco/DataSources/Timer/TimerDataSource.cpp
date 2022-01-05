/**
 * @file TimerDataSource.cpp
 * @brief Source file for class TimerDataSource
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
 * the class TimerDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "HighResolutionTimer.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "TimerDataSource.h"
#include "Threads.h"
#include "FastPollingMutexSem.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static uint32 dataSourceTime;
static bool triggered;
static MARTe::EventSem synchSem;
static FastPollingMutexSem synchMux;
static bool running;
static uint32 sleepTimeUS;

static void TimerThreadFunction(TimerDataSource &timerDataSource) {
    float32 calibTodo = 0.9;
    float32 sleepTime = calibTodo * sleepTimeUS * 1.0e-6;
    while (running) {
        Sleep::Sec(sleepTime);
        dataSourceTime += (sleepTimeUS);
        synchMux.FastTryLock();
        triggered = true;
        synchSem.Post();
        synchMux.FastUnLock();
    }
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TimerDataSource::TimerDataSource() :
        DataSourceI() {
    dataSourceTime = 0u;
    synchSem.Create();
    synchSem.Reset();
    synchMux.Create();
    triggered = false;
    running = true;
    frequency = 0;
}

TimerDataSource::~TimerDataSource() {
    running = false;
}

uint32 last = HighResolutionTimer::Counter32();
bool TimerDataSource::Synchronise() {
    uint32 nowTicks = HighResolutionTimer::Counter32();
    uint32 ellapsedTicks = lastSynchTicks - nowTicks;
    if(ellapsedTicks < maxSleepTicks) {
        float32 sleepSec = ellapsedTicks * HighResolutionTimer::Period();
        Sleep::Sec(sleepSec);
    }
    lastSynchTicks = HighResolutionTimer::Counter32();
    //dataSourceTime = HighResolutionTimer::Counter32() * HighResolutionTimer::Frequency();

/*    synchMux.FastTryLock();
    if (!triggered) {
        synchMux.FastUnLock();
        synchSem.Wait();
        triggered = false;
        synchSem.Reset();
    }
    else {
        triggered = false;
        synchMux.FastUnLock();
    }
    triggered = false;*/
    return true;
}

bool TimerDataSource::AllocateMemory() {
    return true;
}

uint32 TimerDataSource::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool TimerDataSource::GetSignalMemoryBuffer(const uint32 signalIdx,
                                            const uint32 bufferIdx,
                                            void *&signalAddress) {
    bool ret = (signalIdx == 0u);
    if (ret) {
        signalAddress = reinterpret_cast<void *>(&dataSourceTime);
    }
    return ret;
}

const char8 *TimerDataSource::GetBrokerName(StructuredDataI &data,
                                            const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapSynchronisedInputBroker";
    }
    return "";
}

bool TimerDataSource::GetInputBrokers(ReferenceContainer &inputBrokers,
                                      const char8* const functionName,
                                      void * const gamMemPtr) {
    ReferenceT < MemoryMapSynchronisedInputBroker > broker("MemoryMapSynchronisedInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool TimerDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                       const char8* const functionName,
                                       void * const gamMemPtr) {
    return false;
}

bool TimerDataSource::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);
    if (ret) {
        ret = (GetNumberOfSignals() == 1);
    }
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Only one signal can be specified");
    }
    if (ret) {
        ret = (GetSignalType(0) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
    }
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "The time signal type has to be uint32");
    }
    if (ret) {
        ret = GetFunctionSignalReadFrequency(InputSignals, 0, 0, frequency);
    }
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Could not read the time signal frequency");
    }
    if (ret) {
        ret = (frequency > 0);
    }
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "The frequency has to be > 0");
    }
    if (ret) {
        float32 period = 1.0/frequency;
        uint32 periodSleepTicks = HighResolutionTimer::Frequency() * period;
        maxSleepTicks = periodSleepTicks * 0.4;
        sleepTimeUS = 1e6 / frequency;
    }
    if (ret) {
        running = true;
        //Threads::BeginThread((ThreadFunctionType) TimerThreadFunction, this, configMINIMAL_STACK_SIZE);
    }
    return ret;
}

bool TimerDataSource::PrepareNextState(const MARTe::char8 * const currentStateName,
                                       const MARTe::char8 * const nextStateName) {
    return true;
}

CLASS_REGISTER(TimerDataSource, "1.0")
