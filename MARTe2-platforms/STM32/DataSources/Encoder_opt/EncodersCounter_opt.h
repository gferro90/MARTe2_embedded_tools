/*
 * EncodersCounter_opt.h
 *
 *  Created on: 31/ago/2016
 *      Author: pc
 */

#ifndef ENCODERSCOUNTER_OPT_H_
#define ENCODERSCOUNTER_OPT_H_
#include "TIMBase.h"
#include "stm32f4xx_hal.h"

using namespace MARTe;
//TODO The ...GetState() function returns the initialisation handle and we can change parameters and re-initialise.
//TODO Interrupt and DMA cases (but check before if it works in polling mode)
class EncodersCounter_opt: public DataSourceI {
public:

    CLASS_REGISTER_DECLARATION()

    EncodersCounter_opt();

    virtual ~EncodersCounter_opt();

    virtual bool Initialise(StructuredDataI &data);

    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    bool Synchronise();

    bool AllocateMemory();

    uint32 GetNumberOfMemoryBuffers();

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
    void Write(int32 * values);

    void Read(int32 * values);

private:
    TIM_HandleTypeDef ** encoderHandlePtr;
    uint8 numberOfElements;
    uint8 resetEncoder;

};

#endif /* ENCODERSCOUNTER_OPT_H_ */
