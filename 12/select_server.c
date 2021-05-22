#include "network_common.h"

#define MAX_CLIENTS 10

int main(int argc, char* argv[]) {
	int nSFd;// server fd
	int nCFds[MAX_CLIENTS];// clients fds

	struct sockaddr_in stCAddr;// client address info
	socklen_t nCAddr;// client address info size

	fd_set rfds;// event handler
	struct timeval tv;// time value
	int nRetval;// return value

	char strBuffer[BUFSIZ];// message buffer
	int nBufferLen = 0;// message buffer size

	bzero(strBuffer, BUFSIZ);// message buffer initialize
	for (int i = 0; i < MAX_CLIENTS; i++) {
		nCFds[i] = -1;// clients fd initialize
	}

	if (argc != 3) {// not equal 3
		printf("Usage: %s <port> <queue>\n", argv[0]);
		return -1;
	}

	printf("Port: %s\n", argv[1]);// print port number
	printf("Queue: %s\n", argv[2]);// print queue count

	if (atoi(argv[2]) > MAX_CLIENTS) return -1;

	// starting server(port number, max queue size)
	// server status is listening!!
	nSFd = start_tcp_server(atoi(argv[1]), atoi(argv[2]));
	if (nSFd < 0) {
		printf("Starting server is failed\n");
		return -1;
	}

	do {// select start!!!
		FD_ZERO(&rfds);// fd_set initialize
		FD_SET(0, &rfds);// set the input event to register that fd_set
		FD_SET(nSFd, &rfds);// set the server socket event to register that fd_set

		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (nCFds[i] > 0) FD_SET(nCFds[i], &rfds);
			// when clients is accepted that event register
		}

		if (tv.tv_sec == 0 && tv.tv_usec == 0) {
			tv.tv_sec = 1;
			tv.tv_usec = 0;
		}


		nRetval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);

		if (nRetval < 0) break;
		else if (nRetval == 0) {
			time_t nTime;

			time(&nTime);
			nBufferLen = sprintf(strBuffer, "%s", ctime(&nTime));
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (nCFds[i] > 0) send(nCFds[i], strBuffer, nBufferLen, 0);
			}
		} else {
			if (FD_ISSET(0, &rfds)) {
				memset(strBuffer, 0, BUFSIZ);
				nBufferLen = read(0, strBuffer, BUFSIZ);
				if (nBufferLen > 0)
					if (strncasecmp(strBuffer, "exit", 4) == 0) break;
			}
			if (FD_ISSET(nSFd, &rfds)) {
				int fd = accept(nSFd, (struct sockaddr *)&stCAddr, &nCAddr);
				for (int i = 0; i < MAX_CLIENTS; i++) {
					if (nCFds[i] < 0) {
						nCFds[i] = fd;
						break;
					}
				}
			}
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (nCFds[i] > 0) {
					if (FD_ISSET(nCFds[i], &rfds)) {
						bzero(strBuffer, BUFSIZ);
						nBufferLen = read(nCFds[i], strBuffer, BUFSIZ);
						if (nBufferLen <= 0) {
							printf("Disconnecting...[%d]", nCFds[i]);
							close(nCFds[i]);
							nCFds[i] = -1;
						}
						printf("Recv: %s", strBuffer);
					}
				}
			}
		}
	} while (1);

	for (int i = 0; i < 10; i++) {
		if (nCFds[i] > 0) close(nCFds[i]);
	}
	close(nSFd);

	printf("Bye~\n");
	return 0;
}
