/*
 * PWM_opt.h
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#ifndef PWM_PWM_OPT_H_
#define PWM_PWM_OPT_H_

#include "DataSourceI.h"
#include QUOTE(_HAL_H)

using namespace MARTe;

class PWM_opt: public DataSourceI {
public:

    CLASS_REGISTER_DECLARATION()

    PWM_opt();

    virtual ~PWM_opt();

    virtual bool Initialise(StructuredDataI &data);

    bool Synchronise();

    bool AllocateMemory();

    uint32 GetNumberOfMemoryBuffers();

    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

    bool GetSignalMemoryBuffer(const uint32 signalIdx,
                               const uint32 bufferIdx,
                               void *&signalAddress);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    void Write(uint32 * values);

    void Read(uint32 * values);


private:
    TIM_HandleTypeDef **pwmHandlePtr;
    uint8 *channel;
    uint32 *zeroVal;
    uint8 numberOfElements;
};

#endif /* PWM_PWM_OPT_H_ */
