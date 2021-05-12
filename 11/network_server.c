#include <sys/time.h>
#include <time.h>
#include "network_common.h"

int sending_time_info(int nCFd);
int sending_user_data(int nCFd);

int main(int argc, char* argv[]) {
	int nSFd;
	int nCFd;
	int nPid;

	struct sockaddr_in cAddr;
	socklen_t nCAddr;

	if (argc != 2) {
		printf("usage : [IP Address] [Port]");
		return -1;
	}

	printf("Port : %s\n", argv[1]);

	nSFd = start_tcp_server(atoi(argv[1]), 1);
	if (nSFd > 0) {
		printf("TCP Server is started.\n");
	} else {
		printf("Starting Server is failed.\n");
		return -1;
	}

	nCFd = accept(nSFd, (struct sockaddr*) &cAddr, &nCAddr);

	if (nCFd > 0) {
		printf("Client Info: \n");
		printf("\t- IP Address : %s\n", inet_ntoa(cAddr.sin_addr));
		printf("\t- Port : %d\n", ntohs(cAddr.sin_port));

		nPid = fork();
		if (nPid == 0) sending_user_data(nCFd);
		else sending_time_info(nCFd);
		
		wait(NULL);

		close(nCFd);
	} else {
		printf("Connection Failure.\n");
	}

	close(nSFd);

	return 0;
}

int sending_time_info(int nCFd) {
	char strBuffer[BUFSIZ];
	int nBufferLen = 0;

	for (int i = 0; i < 10; i++) {
		struct timeval stTimeval;
		struct tm stTm;

		gettimeofday(&stTimeval, 0);
		localtime_r(&stTimeval.tv_sec, &stTm);
		nBufferLen = sprintf(strBuffer, "%04d/%02d/%02d %02d:%02d:%02d.%03d\n", stTm.tm_year + 1900, stTm.tm_mon + 1, stTm.tm_mday + 1, stTm.tm_hour, stTm.tm_min, stTm.tm_sec, stTimeval.tv_usec / 1000);
		send(nCFd, strBuffer, nBufferLen, 0);
		sleep(1);
	}
}

int sending_user_data(int nCFd) {
	char strBuffer[BUFSIZ];
	int nBufferLen = 0;

	for (int i = 0; ; i++) {
		memset(strBuffer, 0, BUFSIZ);

		nBufferLen = read(0, strBuffer, BUFSIZ);
		if (nBufferLen <= 0) break;
		else send(nCFd, strBuffer, nBufferLen,0);
	}

	return 0;
}
