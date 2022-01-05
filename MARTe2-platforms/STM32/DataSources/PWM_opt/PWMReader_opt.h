/*
 * PWMReader_opt.h
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#ifndef PWM_PWMREADER_OPT_H_
#define PWM_PWMREADER_OPT_H_

#include "MemoryMapInputBroker.h"
#include "stm32f4xx_hal.h"

using namespace MARTe;

class PWM_opt;

class PWMReader_opt: public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

    PWMReader_opt();
    virtual ~PWMReader_opt();
    virtual bool Execute();

    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 * const functionName,
                      void * const gamMemoryAddress);

private:
    PWM_opt *pwmDS;
};

#endif /* PWM_PWMREADER_OPT_H_ */
