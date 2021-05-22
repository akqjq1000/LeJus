#include "network_common.h"
#include <sys/poll.h>

int main(int argc, char* argv[]) {
	char strBuffer[BUFSIZ];
	int nBufferLen = 0;
	int nRetval;

	struct pollfd rfds[2];
	int nTimeout = 1000;

	if (argc != 3) return -1;


	rfds[0].fd = 0;
	rfds[0].events = POLLIN;
	rfds[0].revents = 0;

	rfds[1].fd = connect_to_tcp_server(argv[1], atoi(argv[2]));
	if (rfds[1].fd < 0) return -1;
	printf("%d\n", rfds[1].fd);
	printf("%d\n", getpid());
	rfds[1].events = POLLIN;
	rfds[1].revents = 0;

	do {
		nRetval = poll(rfds, 2, nTimeout);

		if (nRetval > 0) {
			for (int i = 0; i < 2; i++) {
				if (rfds[i].fd < 0) continue;
				if (rfds[i].revents & POLLIN) {
					if (i == 0) {
						bzero(strBuffer, BUFSIZ);
						nBufferLen = read(0, strBuffer, BUFSIZ);
						//printf("input: %s", strBuffer);
						if (write(rfds[1].fd, strBuffer, nBufferLen) > 0) {
							printf("send MSG: %s", strBuffer);
						}
					} else if (i == 1) {
						bzero(strBuffer, BUFSIZ);
						nBufferLen = recv(rfds[i].fd, strBuffer, BUFSIZ, 0);
						printf("Recv: %s", strBuffer);
					}
				}
			}
			if (nBufferLen > 0) if (strncasecmp(strBuffer, "exit", 4) == 0) break;
		}
	} while(1);

	for (int i = 0; i < 2; i++) {
		if (rfds[i].fd > 0)close(rfds[i].fd);
	}

	printf("DisConnection...\n");

	return 0;
}
