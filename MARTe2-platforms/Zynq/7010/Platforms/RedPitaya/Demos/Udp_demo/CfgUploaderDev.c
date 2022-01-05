/*
 * CfgUploaderDev.cpp
 *
 *  Created on: Jan 19, 2018
 *      Author: gferro
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#define PORT 4444
#define ADDR "192.168.1.10"
#define CONFIGURATION_FILE_DIR "/home/pi/UDPclient/UdpCommunication.cfg"

int main() {

	struct sockaddr_in other, me;
	int udpFile = socket(AF_INET, SOCK_DGRAM, 0);

	if(udpFile <0) {
		printf("error socket\n");
		return 0;
	}


	other.sin_family = AF_INET;
	other.sin_port = htons(PORT);
	other.sin_addr.s_addr = inet_addr(ADDR);
        me.sin_family=AF_INET;
	me.sin_port=htons(4444);
	me.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(udpFile,(struct sockaddr*)&me, sizeof(me))==-1){
		
		printf("error bind\n");
	}
	//TODO open the udp socket

	int stmCfgFile = open(CONFIGURATION_FILE_DIR, O_RDWR);
	if (stmCfgFile < 0) {
		printf("\nError in cfg open\n");

		return 0;
	}

	const unsigned int packetSize = 64;
	const unsigned int buffSize = 65;

	char buff[buffSize];
	unsigned int sizeToW = packetSize;

	//write initial sequence
	unsigned int written = 0;

	memset(buff, 0, buffSize);
	const char * initSeq = "1234";
	memcpy(buff, initSeq, strlen(initSeq));

	do {
		sizeToW -= written;
		written += sendto(udpFile, buff + written, sizeToW, 0,
				(struct sockaddr*) &other, sizeof(other));
	} while (written < sizeToW);
	printf("\n7\n");

	int end = 0;

	while (!end) {
		sizeToW = packetSize;
		memset(buff, 0, buffSize);

		unsigned int max = packetSize;
		if ((max = read(stmCfgFile, buff, max)) <= 0) {
			break;
		}

		//end of file
		if (max < packetSize) {
			end =1 ;
		}
		written = 0;
		unsigned int iteration = 0;

		do {
			sizeToW -= written;
			written += sendto(udpFile, buff + written, sizeToW, 0,
					(struct sockaddr*) &other, sizeof(other));
			iteration++;
		} while (written < sizeToW);
		buff[packetSize] = 0;
		printf("\n%d bytes written on %d, %d iteration:\n [%s]", written,
				sizeToW, iteration, buff);

		usleep(100000);
	}

	//write final sequence
	memset(buff, 0, buffSize);
	const char * finalSeq = "4321";
	memcpy(buff, finalSeq, strlen(finalSeq));
	sizeToW = packetSize;
	written = 0;
	do {
		sizeToW -= written;
		written += sendto(udpFile, buff + written, sizeToW, 0,
				(struct sockaddr*) &other, sizeof(other));
	} while (written < sizeToW);
	int cnt=0;
	int slen=sizeof(other);	
	sleep(5);
	printf("start app\n");

        while(1){
                int n=0;

		cnt++;
		


		sendto(udpFile, &cnt, 4, 0, (struct sockaddr *)&other, sizeof(other));        
		int x;
		n=recvfrom(udpFile, &x, 4, 0, (struct sockaddr*)&other, &slen);
		if(n==-1){
			printf("%s", strerror(errno));
		}
		printf("received %d %d\n", n, x);
		usleep(900000);
	}
	return 0;
}
