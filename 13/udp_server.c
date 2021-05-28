#include "network_common.h"
#include <sys/poll.h>

int main(int argc, char* argv[]) {
	int sockfd;
	char strBuffer[BUFSIZ];
	int nBufferLen = 0;

	struct sockaddr_in stSAddr;
	struct sockaddr_in stCAddr;
	int nCAddr;

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

	do {
		nCAddr = sizeof(stCAddr);
		memset(strBuffer, 0, BUFSIZ);
		nBufferLen = recvfrom(sockfd, strBuffer, BUFSIZ, 0, (struct sockaddr *)&stCAddr, &nCAddr);
		if (nBufferLen > 0) {
			printf("Client Information\n");
			printf("Addr: %s\n", inet_ntoa(stCAddr.sin_addr));
			printf("Port: %d\n", ntohs(stCAddr.sin_port));
			printf("RX: %s\n", strBuffer);
		} else if (nBufferLen <= 0) break;

		nBufferLen = sendto(sockfd, strBuffer, nBufferLen, 0, (struct sockaddr*) &stCAddr, nCAddr);
	} while (1);

	close(sockfd);
	return 0;
}
