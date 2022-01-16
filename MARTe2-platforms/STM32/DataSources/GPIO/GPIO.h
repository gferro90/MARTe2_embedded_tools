/*
 * GPIO.h
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#ifndef GPIO_GPIODATASOURCE_H_
#define GPIO_GPIODATASOURCE_H_

#include "DataSourceI.h"
#include QUOTE(_HAL_H)


using namespace MARTe;


class GPIO : public DataSourceI {
public:

	CLASS_REGISTER_DECLARATION()

	GPIO();

	virtual ~GPIO();

	virtual bool Initialise(StructuredDataI &data);

	bool Synchronise();

	bool AllocateMemory();

	uint32 GetNumberOfMemoryBuffers();

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


	GPIO_TypeDef * GetHwHandle();


private:
	GPIO_TypeDef *gpioHandlePtr ;

};

#endif /* GPIODATASOURCE_H_ */
