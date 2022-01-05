/*
 * EfficientReader.h
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#ifndef EfficientReader_H_
#define EfficientReader_H_

#include "MemoryMapInputBroker.h"

using namespace MARTe;

class EfficientDriver;


class EfficientReader: public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

    EfficientReader();
    virtual ~EfficientReader();
    virtual bool Execute();
    virtual bool Init(const SignalDirection direction,
              DataSourceI &dataSourceIn,
              const char8 * const functionName,
              void * const gamMemoryAddress);
private:
    EfficientDriver *dataSourceCpuFpga;
};

#endif /* EfficientReader_H_ */
