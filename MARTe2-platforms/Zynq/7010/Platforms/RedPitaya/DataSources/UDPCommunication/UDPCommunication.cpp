/*
 * UDPCommunication.cpp
 *
 *  Created on: Jan 16, 2018
 *      Author: gferro
 */

#include "UDPCommunication.h"
#include "AdvancedErrorManagement.h"
#include "UDPCommunicationReader.h"
#include "UDPCommunicationWriter.h"

#include <string.h>



int32 UDPCommunication::TransferUdpData(void* send_buf, uint32 wsize) {
	return udpCore.TransferUdpData(send_buf, wsize);
}

int32 UDPCommunication::ReceiveUdpData(void* recv_buf, uint32 rsize) {
	return udpCore.ReceiveUdpData(recv_buf, rsize);

}

UDPCommunication::UDPCommunication() :
		GenericStreamDriver(), udpCore() {

}

UDPCommunication::~UDPCommunication() {
}

bool UDPCommunication::Initialise(StructuredDataI &data) {
	bool ret = GenericStreamDriver::Initialise(data);
	if (ret) {
		StreamString ipAddress;
		if (!data.Read("IP_Address", ipAddress)) {
			REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
					"IP_Address not set: using default 127.0.0.1");
			ipAddress = "127.0.0.1";
		}
		uint32 port;
		if (!data.Read("Port", port)) {
			REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
					"port not set: using default 8080");
			port = 8080;
		}
		StreamString myIpAddress;
		if (!data.Read("My_IP_Address", myIpAddress)) {
			REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
					"My_IP_Address not set: using default 127.0.0.1");
			myIpAddress = "127.0.0.1";
		}
		StreamString myNetmask;
		if (!data.Read("My_Netmask", myNetmask)) {
			REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
					"My_Netmask not set: using default 255.255.255.0");
			myNetmask = "255.255.255.0";
		}
		StreamString myGateway;
		if (!data.Read("My_Gateway", myGateway)) {
			REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
					"My_Gateway not set: using default 127.0.0.1");
			myGateway = "127.0.0.1";
		}
		uint32 myPort;
		if (!data.Read("My_Port", myPort)) {
			REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
					"myPort not set: using default 8080");
			myPort = 8080;
		}
		StreamString macEthernetAddress;
		if (!data.Read("My_MAC_Address", macEthernetAddress)) {
			REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
					"My_MAC_Address not set: using default 0:0:0:0:0:0");
			macEthernetAddress = "0:0:0:0:0:0";
		}
		uint8 isBlocking;
		if (!data.Read("IsBlocking", isBlocking)) {
			REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
					"IsBlocking not set: using default 0 (false)");
			isBlocking = 0u;
		}

		//it is ok with const char*, don't need them after config
		struct UdpConfigs config = { macEthernetAddress.Buffer(),
				ipAddress.Buffer(), myIpAddress.Buffer(), myGateway.Buffer(),
				myNetmask.Buffer(), port, myPort, isBlocking };

		ret = (udpCore.Configure(config) == 0);
		if (!ret) {
			REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
					"Error in UDP configuration");
		}
	}

	return ret;
}

const char8 *UDPCommunication::GetBrokerName(StructuredDataI &data,
		const SignalDirection direction) {
	if (direction == InputSignals) {
		return "UDPCommunicationReader";
	}
	if (direction == OutputSignals) {
		return "UDPCommunicationWriter";
	}
	return "";
}

bool UDPCommunication::GetInputBrokers(ReferenceContainer &inputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	ReferenceT<UDPCommunicationReader> broker("UDPCommunicationReader");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = inputBrokers.Insert(broker);
	}
	return ret;
}

bool UDPCommunication::GetOutputBrokers(ReferenceContainer &outputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	ReferenceT<UDPCommunicationWriter> broker("UDPCommunicationWriter");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = outputBrokers.Insert(broker);
	}
	return ret;
}

void UDPCommunication::Read(void *bufferToFill, uint32 &sizeToRead) {
	udpCore.ReceiveUdpData(bufferToFill, sizeToRead);
}

void UDPCommunication::Write(void *bufferToFlush, uint32 &sizeToWrite) {
	udpCore.TransferUdpData(bufferToFlush, sizeToWrite);
}

CLASS_REGISTER(UDPCommunication, "1.0")
