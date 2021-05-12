#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
	char strBuffer[BUFSIZ];
	int nBufferLen = 0;

	for (int i = 0; ; i++) {
		memset(strBuffer, 0, BUFSIZ);

		nBufferLen = read(0, strBuffer, BUFSIZ);

		if (nBufferLen <= 0) break;
		else printf("Recv: %s", strBuffer);

		sleep(1);
		printf("Step: %d\n", i);
	}
	return 0;
}
