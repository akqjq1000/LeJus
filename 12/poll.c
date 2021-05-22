#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <time.h>

#define MAX_FDS 10

int main() {
	int nRetval = 0;
	char strBuffer[BUFSIZ];
	int nBufferLen = 0;
	struct pollfd fds[MAX_FDS];

	time_t nPrevTime = 0;
	time_t nCurTime = 0;

	for (int i = 0; i < MAX_FDS; i++) {
		fds[i].fd = -1;
		fds[i].events = 0x00;
		fds[i].revents = 0x00;
	}

	fds[0].fd = 0;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	time(&nPrevTime);
	do {
		nRetval = poll(fds, 1, 5000);
		if (nRetval < 0) {
			perror("Poll Exception!!\n");
			return -1;
		}
		time(&nCurTime);
		if (nCurTime >= nPrevTime + 5) {
			printf("======>%s", ctime(&nCurTime));
			nPrevTime = nCurTime;
		}
		if (nRetval == 0) continue;

		if (fds[0].revents & POLLIN) {
			bzero(strBuffer, BUFSIZ);
			nBufferLen = 0;

			nBufferLen = read(fds[0].fd, strBuffer, BUFSIZ);
			printf("Data: %s", strBuffer);

			if (strncasecmp(strBuffer, "EXIT", 4) == 0) break;
		}
	} while(1);

	printf("Exiting\n");

	return 0;
} 
