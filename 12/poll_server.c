#include "network_common.h"
#include <sys/poll.h>

#define MAX_CLIENTS 10

int main(int argc, char* argv[]) {
	struct sockaddr_in stCAddr;
	socklen_t nCAddr;

	struct pollfd rfds[MAX_CLIENTS + 2];

	int nTimeout = 0;
	int nRetval;

	char strBuffer[BUFSIZ];
	int nBufferLen = 0;

	bzero(strBuffer, BUFSIZ);
	for (int i = 0; i < MAX_CLIENTS + 2; i++) {
		rfds[i].fd = -1;
		rfds[i].events = 0;
		rfds[i].revents = 0;
	}

	if (argc != 3) {
		printf("usage: %s <port> <queue>\n", argv[0]);
		return -1;
	}

	printf("Port : %s\n", argv[1]);
	printf("Queue : %s\n", argv[2]);
	if (atoi(argv[2]) > MAX_CLIENTS) return -1;

	rfds[0].fd = 0;
	rfds[0].events = POLLIN;
	rfds[0].revents = 0;

	rfds[1].fd = start_tcp_server(atoi(argv[1]), atoi(argv[2]));
	if (rfds[1].fd < 0) {
		printf("Strting Server is failed\n");
		return -1;
	}
	printf("%d\n", rfds[1].fd);
	rfds[1].events = POLLIN;
	rfds[1].revents = 0;

	nTimeout = 1000;

	do {
		nRetval = poll(rfds, MAX_CLIENTS + 2, nTimeout);

		if(nRetval > 0) {
			for (int i = 0; i < MAX_CLIENTS + 2; i++) {
				if (rfds[i].fd < 0) continue;

				if (rfds[i].revents & POLLIN) {
					if (i == 0) {
						bzero(strBuffer, BUFSIZ);
						nBufferLen = read(0, strBuffer, BUFSIZ);
						printf("Input:  %s", strBuffer);
						for (int j = 2; j < MAX_CLIENTS + 2; j++) {
							if (rfds[j].fd > 0) {
								if (send(rfds[j].fd, strBuffer, nBufferLen, 0) > 0)
									printf("send MSG: %s", strBuffer);
							}
						}
						if (nBufferLen > 0) {
							if (strncasecmp(strBuffer, "exit", 4) == 0) break;
						}
						nBufferLen = 0;
					} else if (i == 1) {
						printf("Connecting...");
						int fd = accept(rfds[i].fd, (struct sockaddr * )&stCAddr, &nCAddr);
						if (fd > 0) {
							for (int j = 2; j < MAX_CLIENTS + 2; j++)  {
								if (rfds[j].fd < 0) {
									rfds[j].fd = fd;
									rfds[j].events = POLLIN;
									rfds[j].revents = 0;
									break;
								}
							}
							printf("Success!!");
						}
						printf("\n");
					} else {
						bzero(strBuffer, BUFSIZ);
						nBufferLen = read(rfds[i].fd, strBuffer, BUFSIZ);
						if (nBufferLen <= 0) {
							printf("A Client(%d) is disconnected\n", rfds[i].fd);
							close(rfds[i].fd);
							rfds[i].fd = -1;
							rfds[i].events = 0;
							rfds[i].revents = 0;
						}
						printf("A Client(%d) MSG: %s\n", rfds[i].fd, strBuffer);
						bzero(strBuffer, BUFSIZ);
						nBufferLen = 0;
					}
				}
			}

			if (nBufferLen > 0) {
				if (strncasecmp(strBuffer, "exit", 4) == 0) break;
			}
		} else if (nRetval == 0) {
			time_t nTime = 0;

			time(&nTime);
			nBufferLen = sprintf(strBuffer, "%s", ctime(&nTime));
			for (int i = 2; i < MAX_CLIENTS + 2; i++) {
				if (rfds[i].fd < 0) continue;
				send (rfds[i].fd, strBuffer, nBufferLen, 0);
			}
		} else {
			break;
		}
	} while(1);

	for (int i = 0; i < MAX_CLIENTS + 2; i++) {
		if (rfds[i].fd > 0 ) close(rfds[i].fd);
	}

	printf("Bye~\n");

	return 0;
}
