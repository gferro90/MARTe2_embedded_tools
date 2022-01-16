/*
 * PWMWriter_opt.h
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#ifndef PWM_PWMWRITER_OPT_H_
#define PWM_PWMWRITER_OPT_H_

#include "MemoryMapInputBroker.h"
#include QUOTE(_HAL_H)

using namespace MARTe;

class PWM_opt;

class PWMWriter_opt: public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

    PWMWriter_opt();
    virtual ~PWMWriter_opt();
    virtual bool Execute();

    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 * const functionName,
                      void * const gamMemoryAddress);

private:
    PWM_opt *pwmDS;
};

#endif /* PWMWRITER_OPT_H_ */
