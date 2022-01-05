/**
 * @file SIM900GPRS.cpp
 * @brief Source file for class SIM900GPRS
 * @date Jul 30, 2017
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
 * the class SIM900GPRS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SIM900GPRS.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

void RemoteDriverClient::SendAndReceive(const char* question,
                                        char* response,
                                        int32 respSize,
                                        uint32 usecTimeout,
                                        uint32 secTimeout,
                                        uint32 maxWaitLoops,
                                        bool print) {

    /*
     SleepMsec(10);
     write(gsmDeviceFd, question, strlen(question));
     SleepMsec(10);
     read(gsmDeviceFd, response, respSize);

     */

    FD_ZERO(&set); /* clear the set */
    FD_SET(gsmDeviceFd, &set); /* add our file descriptor to the set */
    struct timeval timeout;
    timeout.tv_sec = secTimeout;
    timeout.tv_usec = usecTimeout;
    int rv = 1;
    memset(response, 0, respSize);

    int32 readBytes = 0;
    if (print) {
        printf("\n%s:\n", question);
    }
    SleepMsec(10);
    write(gsmDeviceFd, question, strlen(question));
    uint32 nWaits = 0u;

    while ((rv > 0) && (nWaits < maxWaitLoops)) {
        rv = select(gsmDeviceFd + 1, &set, NULL, NULL, &timeout);
        if (print) {
            if (rv > 0) {
                readBytes = read(gsmDeviceFd, response, respSize);
                printf("\n%s %d\n", response, readBytes);
            }
        }
        nWaits++;
    }
}

void RemoteDriverClient::WriteToGsm(const void* data,
                                    uint32 size,
                                    bool terminate) {

    write(gsmDeviceFd, data, size);
    if (terminate) {
        int32 readBytes=0;
        char response[64];
        memset(response, 0, 64);

        FD_ZERO(&set); /* clear the set */
        FD_SET(gsmDeviceFd, &set); /* add our file descriptor to the set */
        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        int rv = 1;

        uint8 sequenceEnd = 0x1a;
        write(gsmDeviceFd, &sequenceEnd, 1);

        //while (rv > 0) {
            rv = select(gsmDeviceFd + 1, &set, NULL, NULL, &timeout);
            if (rv > 0) {
                readBytes = read(gsmDeviceFd, response, 64);
                //printf("\n%s %d\n", response, readBytes);
            }
        //}
    }
}

void RemoteDriverClient::ReadFromGsm(void* data,
                                     uint32 size) {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

SIM900_GPRS::SIM900_GPRS() {
    // Auto-generated constructor stub for SIM900_GPRS
    // TODO Verify if manual additions are needed
}

SIM900_GPRS::~SIM900_GPRS() {
    // Auto-generated destructor stub for SIM900_GPRS
    // TODO Verify if manual additions are needed
}

bool SIM900_GPRS::Initialise(StructuredDataI &data) {

    FString gsmDevice;
    if (!cdb.ReadFString(gsmDevice, "GsmDevice", "/dev/ttyAMA0")) {
        AssertErrorCondition(Warning, "RemoteDriverClient::ObjectLoadSetup: %s GsmDevice not specified. Using default: %s", Name(), gsmDevice.Buffer());
    }

    //set up the serial communication with GSM board
    gsmDeviceFd = open(gsmDevice.Buffer(), O_RDWR | O_NOCTTY); //| O_NDELAY
    if (gsmDeviceFd < 0) {
        printf("\nfailed opening gsm device\n");
        AssertErrorCondition(InitialisationError, "RemoteDriverClient::Init: Failed to open the serial port %s", gsmDevice.Buffer());
        ret = false;
    }

    if (ret) {
        if (!cdb.ReadInt32(baudRate, "BaudRate", 115200)) {
            AssertErrorCondition(Warning, "RemoteDriverClient::ObjectLoadSetup: %s BaudRate not specified. Using default: %d", Name(), baudRate);
        }

        struct termios options;
        tcgetattr(gsmDeviceFd, &options);
        options.c_cflag = GetBaudRate(baudRate) | CS8 | CLOCAL | CREAD;
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(gsmDeviceFd, TCIFLUSH);
        tcsetattr(gsmDeviceFd, TCSANOW, &options);
        FD_ZERO (&set); /* clear the set */
        FD_SET(gsmDeviceFd, &set); /* add our file descriptor to the set */
    }


    if (useGsmBoard) {
           const int32 respSize = 64;
           char response[respSize];
           //set up the connection with the GSM board
           SendAndReceive("AT\r", response, respSize);
           SendAndReceive("AT+CPIN?\r", response, respSize);
           SendAndReceive("AT+CREG?\r", response, respSize);
           SendAndReceive("AT+CGATT?\r", response, respSize);
           SendAndReceive("AT+CIPSHUT\r", response, respSize);
           SendAndReceive("AT+CIPMUX=0\r", response, respSize);
           SendAndReceive("AT+CSTT=\"WAP.TIM.IT\",\"WAPTIM\",\"WAPTIM\"\r", response, respSize);
           SendAndReceive("AT+CIICR\r", response, respSize);
           SendAndReceive("AT+CIFSR\r", response, respSize);
           SendAndReceive("AT+CIPSTART=\"TCP\",\"160.80.97.45\",\"4444\"\r", response, respSize);
           SendAndReceive("AT+CIPSEND\r", response, respSize);

           write(gsmDeviceFd, &packetSize, sizeof(uint32));
           uint8 sequenceEnd = 0x1a;
           write(gsmDeviceFd, &sequenceEnd, 1);

   #if 0
           /** First test if everything is okay **/

           => AT

           <= AT /** This should come back. SIM900 default is to echo back commands you enter **/

           <= OK /** This string should tell you all is well**/

           =>AT+CPIN? /**This is to check if SIM is unlocked. This sample assumes unlocked SIMs**/

           <= +CPIN: READY /** If your response contains this, then it means SIM is unlocked and ready**/

           =>AT+CREG? /**This checks if SIM is registered or not**/

           <=+CREG: 0,1 /**This string in the response indicates SIM is registered**/

           =>AT+CGATT? /**Check if GPRS is attached or not**/

           <=+CGATT: 1 /**A response containing this string indicates GPRS is attached**/

           =>AT+CIPSHUT /**Reset the IP session if any**/

           <=SHUT OK /**This string in the response represents all IP sessions shutdown.**/

           =>AT+CIPSTATUS /**Check if the IP stack is initialized**/

           <=STATE: IP INITIAL /**This string in the response indicates IP stack is initialized**/

           =>AT+CIPMUX=0 /**To keep things simple, I’m setting up a single connection mode**/

           <=OK /**This string indicates single connection mode set successfully at SIM 900**/

           =>AT+CSTT= “APN”, “UNAME”, “PWD” /**Start the task, based on the SIM card you are using, you need to know the APN, username and password for your service provider**/

           <= OK /**This response indicates task started successfully**/

           => AT+CIICR /**Now bring up the wireless. Please note, the response to this might take some time**/

           <=OK /**This text in response string indicates wireless is up**/

           =>AT+CIFSR /**Get the local IP address. Some people say that this step is not required, but if I do not issue this, it was not working for my case. So I made this mandatory, no harm.**/

           <= xxx.xxx.xxx.xxx /**If previous command is successful, you should see an IP address in the response**/

           =>AT+CIPSTART= “TCP” , “www.vishnusharma.com”, “80” /**Start the connection, TCP, domain name, port**/

           <= CONNECT OK /**This string in the response indicates TCP connection established**/

           =>AT+CIPSEND /**Request initiation of data sending (the request)**/

           <= > /**The response should be the string “>” to indicate, type your data to send**/

           => xxxxxx /**Just type anything for now**/

           =>
   #026 /**Now type the sequence #026. This tells the terminal.exe to send the hex code 0x1a (which is Ctrl+Z) to indicate end of data sending**/

           <= xxxxxxxxxx /**You should get some response back from the server…it would generally be a complain that the request string was not valid…but that is a different subject…you have established the connection**/

           /**To close the connection**/

           =>AT+CIPSHUT /**Request shutting down of the current connections**/

           <=SHUT OK /**Indicates shutdown successful**/
   #endif

       }
    if (useGsmBoard) {
//#if 0
        SendAndReceive("AT+CIPSEND\r", NULL, 0, 0, 1, 1, false);
        SleepMsec(10);
        WriteToGsm((void*) buffer, sizeToWrite, true);
        //write(gsmDeviceFd, "AT+CIPSEND\r", strlen("AT+CIPSEND\r"));

        //send null char if the other part cannot see the end of transmission
        if ((k == (numberOfPackets - 1)) && (sizeToWrite == packetSize)) {
            SendAndReceive("AT+CIPSEND\r", NULL, 0, 0, 1, 1, false);
            SleepMsec(10);
            //SendAndReceive("AT+CIPSEND\r", response, respSize);
            WriteToGsm((void*) "", 1, true);
            SleepMsec(10);
        }
       // printf("\nwrite %d p=%d\n", sizeToWrite, k);
//#endif
    }



}

CLASS_REGISTER(SIM900_GPRS, "1.0")

