#include "network_common.h"
#include <sys/poll.h>

int main(int argc, char* argv[]) {
	int sockfd;
	char strBuffer[BUFSIZ];
	int nBufferLen = 0;

	struct sockaddr_in stSAddr;
	int nSAddr;

	if (argc != 3) {
		printf("usage: %s <ip> <port>", argv[0]);
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	bzero(&stSAddr, sizeof(stSAddr));
	stSAddr.sin_family = AF_INET;
	stSAddr.sin_addr.s_addr = inet_addr(argv[1]);
	stSAddr.sin_port = htons(atoi(argv[2]));
	nSAddr = sizeof(stSAddr);

	bzero(strBuffer, BUFSIZ);
	nBufferLen = read(0, strBuffer, BUFSIZ);

	nBufferLen = sendto(sockfd, strBuffer, nBufferLen, 0, (struct sockaddr*)&stSAddr, nSAddr);
	if (nBufferLen > 0) printf("TX: %s\n", strBuffer);

	bzero(strBuffer, BUFSIZ);
	nBufferLen = recvfrom(sockfd, strBuffer, BUFSIZ, 0, (struct sockaddr*)&stSAddr, &nSAddr);

	if (nBufferLen > 0) {
		printf("Server Information\n");
		printf("Addr: %s\n", inet_ntoa(stSAddr.sin_addr));
		printf("Port: %d\n", ntohs(stSAddr.sin_port));
		printf("RX: %s\n", strBuffer);
	}

	close(sockfd);
	return 0;
}
