/*
 * USBSignalDisplay.h
 *
 *  Created on: 16/ago/2016
 *      Author: Giuseppe Ferrò
 */

#ifndef USB_USBSignalDisplay_H_
#define USB_USBSignalDisplay_H_


#include "DataSourceI.h"


using namespace MARTe;

struct SignalInfo {
	StreamString signalName;
	void *signalValue;
	AnyType asAnyType;
};

class USBSignalDisplay: public DataSourceI {
public:

	CLASS_REGISTER_DECLARATION()

	USBSignalDisplay();
	virtual ~USBSignalDisplay();

	virtual bool Initialise(StructuredDataI &data);

    bool Synchronise();

    bool AllocateMemory();

    MARTe::uint32 GetNumberOfMemoryBuffers();

    bool GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress);

    virtual const MARTe::char8 *GetBrokerName(MARTe::StructuredDataI &data, const MARTe::SignalDirection direction);

    virtual bool GetInputBrokers(MARTe::ReferenceContainer &inputBrokers, const MARTe::char8* const functionName, void * const gamMemPtr);

    virtual bool GetOutputBrokers(MARTe::ReferenceContainer &outputBrokers, const MARTe::char8* const functionName, void * const gamMemPtr);

    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
            const MARTe::char8 * const nextStateName);


    SignalInfo *usbSignalInfos;

    bool running;

    uint32 refreshTimeUS;

    uint8 blocking;
    uint8 activate;
};

#endif /* USB_USBSignalDisplay_H_ */
