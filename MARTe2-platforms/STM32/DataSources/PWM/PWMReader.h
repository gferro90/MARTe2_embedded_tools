/*
 * PWMReader.h
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#ifndef PWM_PWMREADER_H_
#define PWM_PWMREADER_H_

#include "MemoryMapInputBroker.h"
#include "stm32f4xx_hal.h"

using namespace MARTe;

class PWMReader: public MemoryMapInputBroker {
public:
	CLASS_REGISTER_DECLARATION()

	PWMReader();
	virtual ~PWMReader();
	virtual bool Execute();

	virtual bool Init(const SignalDirection direction,
			DataSourceI &dataSourceIn, const char8 * const functionName,
			void * const gamMemoryAddress);

private:
	TIM_HandleTypeDef *pwmHandle;
	uint32 channel;
};

#endif /* PWM_PWMREADER_H_ */
