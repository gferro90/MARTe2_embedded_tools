/*
 * PWMWriter.h
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#ifndef PWM_PWMWRITER_H_
#define PWM_PWMWRITER_H_
#include "MemoryMapInputBroker.h"
#include "stm32f4xx_hal.h"

using namespace MARTe;
class PWMWriter : public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

PWMWriter();
	virtual ~PWMWriter();
	virtual bool Execute();

	virtual bool Init(const SignalDirection direction,
	                           DataSourceI &dataSourceIn,
	                           const char8 * const functionName,
	                           void * const gamMemoryAddress);

private:
	TIM_HandleTypeDef *pwmHandle;
	uint32 channel;
};

#endif /* PWMWRITER_H_ */
