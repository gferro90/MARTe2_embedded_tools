/*
 * CpuFpgaCommunicationReader.h
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#ifndef CPUFPGACOMMUNICATIONREADER_H_
#define CPUFPGACOMMUNICATIONREADER_H_

#include "MemoryMapInputBroker.h"

using namespace MARTe;

class CpuFpgaCommunication;


class CpuFpgaCommunicationReader: public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

    CpuFpgaCommunicationReader();
    virtual ~CpuFpgaCommunicationReader();
    virtual bool Execute();
    virtual bool Init(const SignalDirection direction,
              DataSourceI &dataSourceIn,
              const char8 * const functionName,
              void * const gamMemoryAddress);
private:
    CpuFpgaCommunication *dataSourceCpuFpga;
    uint8 bufferAllocated;
};

#endif /* CPUFPGACOMMUNICATIONREADER_H_ */
