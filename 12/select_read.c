#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <time.h>
#include <sys/select.h>
#include <sys/types.h>

int main() {

	fd_set rfds;
	struct timeval tv;
	int retval;

	time_t nTime;
	char strBuffer[BUFSIZ];
	int nBufferLen = 0;

	do {
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);

		tv.tv_sec = 5;
		tv.tv_usec = 0;

		retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);

		if (retval) {
			if (FD_ISSET(0, &rfds)) {
				memset(strBuffer, 0, BUFSIZ);
				nBufferLen = read(0, strBuffer, BUFSIZ);
				if (nBufferLen > 0) {
					if (strncasecmp(strBuffer, "exit", 4) == 0) break;

					printf("[%02d] : %s", nBufferLen, strBuffer);
				}
			}
		} else {
			time(&nTime);
			printf("Current: %s", ctime(&nTime));
		}
	} while(1);

	return 0;
}
