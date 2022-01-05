/**
 * @file TimerDataSource.h
 * @brief Header file for class TimerDataSource
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

 * @details This header file contains the declaration of the class TimerDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TIMERDATASOURCE_H_
#define TIMERDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EventSem.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Allows to synchronse the MARTe cycle time at a certain frequency
 *
 * @details The user can choose to use an internal thread that synchronises using an event semaphore and so releasing the cpu.
 * Since by default the FreeRTOS tick rate is about 1KHz, frequencies above 1KHz are not supported in this mode. Alternatively,
 * it is possible synchronise polling on the high resolution counter that not releases the cpu. In this case the frequency depends
 * by the configuration of the timer employed by HighResolutionCounter. By default the timer TIM6 is used, and its configuration allows
 * a resolution of 1us, allowing frequencies up to 1MHz.
 *
 * @details Follows an example of configuration
 * <pre>
 * +Timer = {
 *     Class = TimerDataSource
 *     UseThread = 1 //Use or not the internal thread ad the event semaphore to sync. Default 0 (don't use an internal thread)
 * }
 * </pre>
 */
class TimerDataSource: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

	/**
	 * @brief Constructor.
	 * @details Initialises the class variables.
	 */
    TimerDataSource ();

    /**
     * @brief Destructor
     */
    virtual ~TimerDataSource();

    /**
     * @brief Waits that the specified period has passed.
     * @see DataSourceI::Synchronise.
     * @details if (UseThread == 1) then it waits on an event semaphore. In this case if the MARTe cycle is too slow, the next synchronisation
     * will occur after another period of time because that is the instant in which the thread will post the semaphore. Otherwise, if (UseThread == 0),
     * it waits until (now-last)<period. This means that if the MARTe cycle is too slow, it returns immediately and the next synchronisation point
     * will be after the time period.
     */
    bool Synchronise();

    /**
     * @see DataSourceI::AllocateMemory
     * @details It trivially returns true withoud doing anything
     * @return true
     */
    bool AllocateMemory();

    /**
     * @see DataSourceI::GetNumberOfMemoryBuffers
     * @details It trivially returns 1.
     * @return 1
     */
    MARTe::uint32 GetNumberOfMemoryBuffers();

    /**
     * @see DataSourceI::GetSignalMemoryBuffer
     * @details returns the pointer to the internal member that holds the elapsed time in us.
     */
    bool GetSignalMemoryBuffer(const MARTe::uint32 signalIdx,
                               const MARTe::uint32 bufferIdx,
                               void *&signalAddress);

    /**
     * @see DataSourceI::GetBrokerName
     * @details returns MemoryMapSynchronisedInputBroker if (direction == InputSignals), otherwise it return NULL.
     */
    virtual const MARTe::char8 *GetBrokerName(MARTe::StructuredDataI &data,
                                              const MARTe::SignalDirection direction);

    /**
     * @see DataSourceI::GetInputBrokers
     * @details Returns the initialised MemoryMapSynchronisedInputBroker.
     */
    virtual bool GetInputBrokers(MARTe::ReferenceContainer &inputBrokers,
                                 const MARTe::char8* const functionName,
                                 void * const gamMemPtr);
    /**
     * @see DataSourceI::GetOutputBrokers
     * @details Returns false
     */
    virtual bool GetOutputBrokers(MARTe::ReferenceContainer &outputBrokers,
                                  const MARTe::char8* const functionName,
                                  void * const gamMemPtr);

    /**
     * @see DataSourceI::SetConfiguredDatabase
     * @details Checks that only one signal is specified, that the frequency is specified and greater than 0,
     * that the type of the signal is uint32.
     */
    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

    /**
     * @see DataSourceI::PrepareNextState
     * @details It trivially returns true.
     * @return true.
     */
    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
                                  const MARTe::char8 * const nextStateName);



public:

    /**
     * Holds the current elapsed time
     */
    uint32 dataSourceTime;

    /**
     * The event semaphore used to synchronise
     * if (UseThread == 1) in configuration
     */
    EventSem synchSem;

    /**
     * A flag used to check if the execution has started
     */
    bool running;

    /**
     * The time period in us
     */
    uint32 sleepTimeUS;

    /**
     * The flag that specifies if the synchronisation
     * is implemented using a thread and a semaphore
     */
    uint8 useThread;

    /**
     * Stores the counter if the synchronisation is in polling
     * mode using the high resolution counter.
     */
    uint64 oldCounter;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TIMERDATASOURCE_H_ */
