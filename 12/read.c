#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <time.h>

int main() {
	time_t nTime;
	char strBuffer[BUFSIZ];
	int nBufferLen = 0;

	for (int i = 0; ; i++) {
		memset(strBuffer, 0, BUFSIZ);
		nBufferLen = read(0, strBuffer, BUFSIZ);
		if (nBufferLen > 0) {
			if (strncasecmp(strBuffer, "exit", 4) == 0) break;

			printf("[%02d] : %s", nBufferLen, strBuffer);
		}
		sleep(5);
		time(&nTime);

		printf("Current: %s", ctime(&nTime));
	}

	return 0;
}
