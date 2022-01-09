/*
 * SendConfiguration.cpp
 *
 *  Created on: 13 giu 2018
 *      Author: pc
 */

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

#define _DEV_PATH "/dev/ttyACM0"
#define _CFG_PATH "/home/pc/MARTe2Project/GIT/MARTe2_embedded_tools/MARTe2-platforms/STM32/Platforms/F4/STM32F407-VGT6/Demos/TimerDataSource_demo/Configuration/Test1_up.cfg"
#define _PACKET_SIZE 64
#define _BAUD_RATE B115200
#define _INIT_SEQ "1234"
#define _FINAL_SEQ "4321"


inline int GetBaudRate(int speedVal) {

    int speeds[] = { B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, 0 };
    int speedValues[] = { 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 0 };

    int i = 0;
    while (speedValues[i] != 0) {
        if (speedValues[i] == speedVal) {
            return speeds[i];
        }
        i++;
    }
    return 0;

}



int main(int argc,
         const char **argv) {

    printf("Usage:\n\rsudo ./SendConfiguration.ex [DEV PATH] [CONFIG FILE PATH] [BAUD RATE] [PACKET SIZE] [INIT SEQ] [FINAL SEQ]\n\r"
            "Default:\n\rsudo ./SendConfiguration.ex /dev/ttyACM0 TimerDataSource_demo/Configuration/Test1_up.cfg 115200 64 1234 4321");


    struct timespec sleepTime;
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 10000000;
    struct timespec remTime;

    const char *devPath=_DEV_PATH;
    const char *cfgPath=_CFG_PATH;
    unsigned int packetSize = _PACKET_SIZE;
    const char* initSeq=_INIT_SEQ;
    const char* finalSeq=_FINAL_SEQ;
    int baudRate=_BAUD_RATE;

    if(argc>1){
        devPath=argv[1];
    }
    if(argc>2){
        cfgPath=argv[2];
    }
    if(argc>3){
        baudRate=GetBaudRate(atoi(argv[3]));
    }

    if(argc>4){
        packetSize=atoi(argv[4]);
    }
    if(argc>5){
        initSeq=argv[5];
    }

    if(argc>6){
        finalSeq=argv[6];
    }


    int devFile = -1;
    unsigned int j = 0u;
    const unsigned int nTries = 100u;
    //Wait 1 second before exit
    while (devFile < 0) {
        devFile = open(devPath, O_RDWR | O_NOCTTY | O_SYNC);
        if (j++ > nTries) {
            printf("USBDrv::Timeout expired waiting the open of the usb file: %s\n", devPath);
            return -1;
        }
        nanosleep(&sleepTime, &remTime);
    }

    int flags = 0;
    if ((flags = fcntl(devFile, F_GETFL, 0)) == -1) {
        flags = 0;
    }

    fcntl(devFile, F_SETFL, flags | O_NONBLOCK);

    int speed = baudRate;

    int parity = 0;

    struct termios ttyDEV;
    memset(&ttyDEV, 0, sizeof(ttyDEV));
    if (tcgetattr(devFile, &ttyDEV) != 0) {
        return -1;
    }

    cfsetospeed(&ttyDEV, speed);
    cfsetispeed(&ttyDEV, speed);

    // 8-bit chars
    ttyDEV.c_cflag = (ttyDEV.c_cflag & ~CSIZE) | CS8;
    // disable break processing
    ttyDEV.c_iflag &= ~IGNBRK;
    // no signaling chars, no echo,
    ttyDEV.c_lflag = 0;
    // no canonical processing, no remapping, no delays
    ttyDEV.c_oflag = 0;
    // read  block
    ttyDEV.c_cc[VMIN] = 1;
    // 0.5 seconds read timeout
    ttyDEV.c_cc[VTIME] = 5;
    // shut off xon/xoff ctrl
    ttyDEV.c_iflag &= ~(IXON | IXOFF | IXANY);
    // ignore modem controls, enable reading
    ttyDEV.c_cflag |= (CLOCAL | CREAD);
    // shut off parity
    ttyDEV.c_cflag &= ~(PARENB | PARODD);
    ttyDEV.c_cflag |= parity;
    ttyDEV.c_cflag &= ~CSTOPB;
    ttyDEV.c_cflag &= ~CRTSCTS;

    if (tcsetattr(devFile, TCSANOW, &ttyDEV) != 0) {
        return -1;
    }

    tcflush(devFile, TCIOFLUSH);

    int cfgFile = -1;
    cfgFile = open(cfgPath, O_RDWR);
    if (cfgFile < 0) {
        return -1;
    }

    //write the cfg on the device
    char buff[packetSize+1];
    unsigned int sizeToW = packetSize;

    //write initial sequence
    unsigned int written = 0;

    memset(buff, 0, packetSize + 1);
    memcpy(buff, initSeq, strlen(initSeq));

    do {
        sizeToW -= written;
        written += write(devFile, buff + written, sizeToW);
    }
    while (written < sizeToW);
    sleepTime.tv_sec = 1;
    sleepTime.tv_nsec = 0;
    nanosleep(&sleepTime, &remTime);

    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 100000000;

    bool end = false;
    //for (unsigned int index = 0; index < totalSize; index += packetSize) {
    while (!end) {
        sizeToW = packetSize;
        memset(buff, 0, packetSize);

        unsigned int max = packetSize;
        if ((max = read(cfgFile, buff, max)) <= 0) {
            break;
        }

        //end of file
        if (max < packetSize) {
            end = true;
        }
        written = 0;
        unsigned int iteration = 0;

        do {
            sizeToW -= written;
            written += write(devFile, buff + written, sizeToW);
            iteration++;
        }
        while (written < sizeToW);
        buff[packetSize] = 0;
        printf("\n%d bytes written on %d, %d iteration:\n [%s]", written, sizeToW, iteration, buff);

        nanosleep(&sleepTime, &remTime);
    }

    //write final sequence
    memset(buff, 0, packetSize + 1);
    memcpy(buff, finalSeq, strlen(finalSeq));
    sizeToW = packetSize;
    written = 0;
    do {
        sizeToW -= written;
        written += write(devFile, buff + written, sizeToW);
    }
    while (written < sizeToW);

    return 1;
}

