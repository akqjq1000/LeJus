#include "network_common.h"
#include <sys/poll.h>

int main(int argc, char* argv[]) {
	int sockfd;
	char strBuffer[BUFSIZ];
	int nBufferLen = 0;

	struct pollfd rfds[2];
	int nRetval;
	int nTimeout = 1000;

	struct sockaddr_in stSAddr;
	struct sockaddr_in stCAddr;
	int nSAddr;
	int nCAddr;

	if (argc != 3) {
		printf("usage: %s <ip> <port>", argv[0]);
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	bzero(&stSAddr, sizeof(stSAddr));
	stSAddr.sin_family = AF_INET;
	stSAddr.sin_addr.s_addr = inet_addr(argv[1]);
	stSAddr.sin_port = htons(atoi(argv[2]));

	rfds[0].fd = 0;
	rfds[0].events = POLLIN;
	rfds[0].revents = 0;
	rfds[1].fd = sockfd;
	rfds[1].events = POLLIN;
	rfds[1].revents = 0;
	
	do {
		nRetval = poll(rfds, 2, nTimeout);
		if (nRetval == 0) continue;
		if (nRetval <0 ) break;

		if (rfds[0].revents & POLLIN) {
			bzero(strBuffer, BUFSIZ);
			nBufferLen = read(0, strBuffer, BUFSIZ);
			sendto(sockfd, strBuffer, nBufferLen, 0, (struct sockaddr*)&stSAddr, sizeof(stSAddr));
		}
		if (rfds[1].revents & POLLIN) {
			bzero(strBuffer, BUFSIZ);
			nBufferLen = recvfrom(sockfd, strBuffer, BUFSIZ, 0, (struct sockaddr*)&stCAddr, &nCAddr);
			if (nBufferLen > 0 ) {
				printf("%s\n", strBuffer);
			}
		}

		if (nBufferLen <= 0) break;
	} while (1);

	/*
	bzero(strBuffer, BUFSIZ);
	nBufferLen = read(0, strBuffer, BUFSIZ);

	nBufferLen = sendto(sockfd, strBuffer, nBufferLen, 0, (struct sockaddr*)&stSAddr, sizeof(stSAddr));
	if (nBufferLen > 0) printf("TX: %s\n", strBuffer);

	bzero(strBuffer, BUFSIZ);
	nBufferLen = recvfrom(sockfd, strBuffer, BUFSIZ, 0, (struct sockaddr*)&stSAddr, &nSAddr);

	if (nBufferLen > 0) {
		printf("Server Information\n");
		printf("Addr: %s\n", inet_ntoa(stSAddr.sin_addr));
		printf("Port: %d\n", ntohs(stSAddr.sin_port));
		printf("RX: %s\n", strBuffer);
	}*/

	close(sockfd);
	return 0;
}
