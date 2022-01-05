/**
 * @file master.c
 * @brief Source file for class master
 * @date Jan 11, 2018
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
 * the class master (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(){

    int uartFile = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_SYNC);



    int flags;
    if ((flags = fcntl(uartFile, F_GETFL, 0)) == -1) {
        flags = 0;
    }
    fcntl(uartFile, F_SETFL, flags | O_NONBLOCK);

    int speed = B115200;

    int parity = 0;

    struct termios ttyUSB;
    memset(&ttyUSB, 0, sizeof ttyUSB);
    if (tcgetattr(uartFile, &ttyUSB) != 0) {
        printf("error !\n");
        return -1;
    }

    cfsetospeed(&ttyUSB, speed);
    cfsetispeed(&ttyUSB, speed);

    ttyUSB.c_cflag = (ttyUSB.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    ttyUSB.c_iflag &= ~IGNBRK;         // disable break processing
    ttyUSB.c_lflag = 0;                // no signaling chars, no echo,
                                       // no canonical processing
    ttyUSB.c_oflag = 0;                // no remapping, no delays
    ttyUSB.c_cc[VMIN] = 1;            // read  block
    ttyUSB.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
    ttyUSB.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    ttyUSB.c_cflag |= (CLOCAL | CREAD); // ignore modem controls,
                                        // enable reading
    ttyUSB.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    ttyUSB.c_cflag |= parity;
    ttyUSB.c_cflag &= ~CSTOPB;
    ttyUSB.c_cflag &= ~CRTSCTS;

    if (tcsetattr(uartFile, TCSANOW, &ttyUSB) != 0) {
        return -1;
    }

    tcflush(uartFile, TCIOFLUSH);

    int counter=0;
    while(1){
        int k=write(uartFile, &counter, sizeof(int));
        counter++;
        int received=0;
        int n=read(uartFile, &received, sizeof(int));
        printf("received %d %d, %d %d\n",uartFile, k, n, received);
    }


    return 0;
}


