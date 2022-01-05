/*
 * UDPCommunication.h
 *
 *  Created on: 23/ago/2016
 *      Author: pc
 */

#ifndef UDPCOMMUNICATION_H_
#define UDPCOMMUNICATION_H_

#include "GenericStreamDriver.h"
#include "UdpCore.h"

using namespace MARTe;

class UDPCommunication: public GenericStreamDriver {
public:

    CLASS_REGISTER_DECLARATION()

    UDPCommunication();
    virtual ~UDPCommunication();

    virtual bool Initialise(StructuredDataI &data);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);



    virtual void Read(void * bufferToFill, uint32 &sizeToRead);

    virtual void Write(void * bufferToFlush, uint32 &sizeToWrite);


    int32 TransferUdpData(void* send_buf, uint32 wsize);

    int32 ReceiveUdpData(void* recv_buf, uint32 rsize);
private:

    UdpCore udpCore;
};

#endif /* UDPCOMMUNICATION_H_ */
