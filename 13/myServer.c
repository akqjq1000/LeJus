#include "network_common.h"
#include <sys/poll.h>
#define MAX_CLIENTS 10

int main(int argc, char* argv[]) {
	int sockfd;
	char strBuffer[BUFSIZ];
	char sendMessage[BUFSIZ];
	int nBufferLen = 0;

	struct sockaddr_in stSAddr;
	struct sockaddr_in stCAddr[MAX_CLIENTS];
	struct sockaddr_in stTemp;
	int thisPort = -1;
	int nCAddr;
	char clientID[10][BUFSIZ];

	if (argc != 2) {
		printf("usage: %s <port>", argv[0]);
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	memset(&stSAddr, 0, sizeof(stSAddr));
	stSAddr.sin_family = AF_INET;
	stSAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stSAddr.sin_port = htons(atoi(argv[1]));

	if (bind(sockfd, (struct sockaddr *)&stSAddr, sizeof(stSAddr)) < 0) {
		printf("Binding Failed...\n");
		return -1;
	}

	for (int i = 0; i < MAX_CLIENTS; i++) {
		stCAddr[i].sin_family = 0;
		stCAddr[i].sin_addr.s_addr = 0;
		stCAddr[i].sin_port = 0;
		bzero(clientID[i], BUFSIZ);
	}

	do {
		thisPort = -1;
		nCAddr = sizeof(stCAddr);
		memset(strBuffer, 0, BUFSIZ);
		nBufferLen = recvfrom(sockfd, strBuffer, BUFSIZ, 0, (struct sockaddr *)&stTemp, &nCAddr);

		if (nBufferLen > 0) nBufferLen = sprintf(sendMessage, "[%d]: %s", stTemp.sin_port, strBuffer);
		else break;

		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (stTemp.sin_port == stCAddr[i].sin_port) {
				thisPort = i;
				break;
			}
		}

		if (thisPort == -1) {
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (stCAddr[i].sin_port == 0) {
					stCAddr[i] = stTemp;
					thisPort = stCAddr[i].sin_port;
					break;
				}
			}
		}

		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (stCAddr[i].sin_port == 0) break;
			sendto(sockfd, sendMessage, nBufferLen, 0, (struct sockaddr*)&stCAddr[i], nCAddr);
		}
	} while (1);

	close(sockfd);
	return 0;
}
