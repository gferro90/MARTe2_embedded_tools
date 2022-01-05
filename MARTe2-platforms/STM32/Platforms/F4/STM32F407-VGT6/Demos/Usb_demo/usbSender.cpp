#include "stdio.h"
#include "fcntl.h"
#include "unistd.h"
#include <string.h>
#include <termios.h>



int main(){

    int fd=open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_SYNC);

    if(fd<0){
        printf("\nunable to open usb\n");
        return 0;
    }



    int flags;
    if ((flags = fcntl(fd , F_GETFL, 0))==-1){
        flags = 0;
    }
    fcntl(fd , F_SETFL, flags | O_NONBLOCK);

    int speed = B115200;

    int parity = 0;

    struct termios ttyUSB;
    memset(&ttyUSB, 0, sizeof ttyUSB);
    if (tcgetattr(fd, &ttyUSB) != 0) {
        return false;
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

    if (tcsetattr(fd, TCSANOW, &ttyUSB) != 0) {
        return false;
    }

    tcflush(fd, TCIOFLUSH);







// give time to the initialisation
//    sleep(20);
    int x =1;
    int y;
    int z;
    printf("\nStart!!\n");

    while(1){
        y=0;
        z=0;
        if(write(fd, &x, 4)<0){
            printf("\nFailed write!!\n");
        }
        /*printf("writed x=%d!!\n", x);
        sleep(1);
        if(read(fd, &y, 4)<0){
            printf("\nFailed read!!\n");
        }
        printf("read y=%d!!\n",y);
        sleep(1);
        if(read(fd, &z, 4)<0){
            printf("\nFailed read!!\n");
        }
        printf("read z=%d!!\n",z);*/
        sleep(1);
        x++;
    }

    return 0;
}
