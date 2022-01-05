/**
 * @file UdpStream.cpp
 * @brief Source file for class UdpStream
 * @date Jun 16, 2017
 * @author pc
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing,
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class UdpStream (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UdpStream.h"
#include "AdvancedErrorManagement.h"
#include "StreamString.h"

extern struct netif *echo_netif;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

UdpStream::UdpStream() :
		udpCore() {

}

UdpStream::~UdpStream() {
	// Auto-generated destructor stub for UdpStream
	// TODO Verify if manual additions are needed
}

bool UdpStream::Initialise(StructuredDataI &data) {

	bool ret = ReferenceContainer::Initialise(data);
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
		uint32 isBlocking;
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

bool UdpStream::Read(char8 * const output, uint32 & size, uint32 timeout) {
	return (udpCore.ReceiveUdpData(output, size) == 0);
}
bool UdpStream::Write(const char8 * const input, uint32 & size,
		uint32 timeout) {
	return (udpCore.TransferUdpData(input, size) == 0);
}
CLASS_REGISTER(UdpStream, "1.0")

