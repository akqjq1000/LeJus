#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {

	int nSFd;
	int nCFd;
	uint16_t nPort;
	struct sockaddr_in cAddr;
	struct sockaddr_in sAddr;
	socklen_t nCAddr;

	char strBuffer[BUFSIZ];
	int nBufferLen = 0;

	if (argc != 3) return -1;

	nPort = atoi(argv[2]);
	printf("IP Address: %s\n", argv[1]);
	printf("Port: %d\n", nPort);

	nSFd = socket(AF_INET, SOCK_STREAM, 0);
	printf("Server FD: %d\n", nSFd);

	bzero(&sAddr, sizeof(struct sockaddr_in));
	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = inet_addr(argv[1]);
	sAddr.sin_port = htons(nPort);

	if (bind(nSFd, (struct sockaddr*) &sAddr, sizeof(sAddr))< 0) {
		printf("Binding Failed...\n");
		return -1;
	}

	listen(nSFd, 1);
	printf("listenning is OK, now.\n");

	nCFd = accept(nSFd, (struct sockaddr*)&cAddr, &nCAddr);
	if (nCFd > 0) {
		printf("Client Info: \n");
		printf("\t- IP Address : %s \n", inet_ntoa(cAddr.sin_addr));
		printf("\t- Port : %d\n", ntohs(cAddr.sin_port));

		nBufferLen = sprintf(strBuffer, "Hello There!!\n");
		send(nCFd, strBuffer, nBufferLen, 0);

		close(nCFd);
	}

	close(nSFd);

	return 0;
}
