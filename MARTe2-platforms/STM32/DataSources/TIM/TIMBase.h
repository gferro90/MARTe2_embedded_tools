/*
 * TIMBase.h
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#ifndef TIM_TIMBASEDATASOURCE_H_
#define TIM_TIMBASEDATASOURCE_H_

#include "DataSourceI.h"
#include QUOTE(_HAL_H)


using namespace MARTe;

class TIMBase: public DataSourceI {
public:

	CLASS_REGISTER_DECLARATION()

TIMBase();
	virtual ~TIMBase();

	virtual bool Initialise(StructuredDataI &data);

	bool Synchronise();

	bool AllocateMemory();

	virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);


	bool GetSignalMemoryBuffer(const uint32 signalIdx,
			const uint32 bufferIdx, void *&signalAddress);

	virtual const char8 *GetBrokerName(StructuredDataI &data,
			const SignalDirection direction);

	virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
			const char8* const functionName, void * const gamMemPtr);

	virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
			const char8* const functionName, void * const gamMemPtr);


	virtual bool PrepareNextState(const char8 * const currentStateName,
			const char8 * const nextStateName);


	virtual TIM_HandleTypeDef * GetHandle();

private:
	TIM_HandleTypeDef *timHandlePtr;
	bool isInterruptMode;
};

#endif /* MARTE2_PLATFORMS_STM32_F4_PLATFORMS_STM32F407_VGT6_DATASOURCES_TIM_TIMBASEDATASOURCE_H_ */
