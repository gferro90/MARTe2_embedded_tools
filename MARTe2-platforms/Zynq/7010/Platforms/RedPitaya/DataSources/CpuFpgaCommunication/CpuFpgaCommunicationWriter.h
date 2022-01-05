/*
 * CpuFpgaCommunicationWriter.h
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#ifndef CPUFPGACOMMUNICATIONWRITER_H_
#define CPUFPGACOMMUNICATIONWRITER_H_



#include "MemoryMapOutputBroker.h"

using namespace MARTe;

class CpuFpgaCommunication;

class CpuFpgaCommunicationWriter : public MemoryMapOutputBroker {
public:
    CLASS_REGISTER_DECLARATION()

    CpuFpgaCommunicationWriter();
    virtual ~CpuFpgaCommunicationWriter();
    virtual bool Execute();
    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 * const functionName,
                      void * const gamMemoryAddress);

private:
    CpuFpgaCommunication *dataSourceCpuFpga;
    uint8 bufferAllocated;

};

#endif /* CPUFPGACOMMUNICATIONWRITER_H_ */

