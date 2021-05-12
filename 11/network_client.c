#include "network_common.h"

int main(int argc, char* argv[]) {
	int nCFd = -1;
	char strBuffer[BUFSIZ];
	int nBufferLen = 0;

	if (argc != 3) {
		return -1;
	}

	nCFd = connect_to_tcp_server(argv[1], atoi(argv[2]));
	if (nCFd < 0) return -1;

	while ((nBufferLen = recv(nCFd, strBuffer, BUFSIZ, 0)) > 0) {
		printf("Recv : %s", strBuffer);
	}

	printf("DisConnection\n");
	
	close(nCFd);

	return 0;
}
