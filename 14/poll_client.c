#include "network_common.h"
#include <sys/poll.h>
#include <openssl/aes.h>
#include <string.h>
#include "aes_common.h"

int main(int argc, char* argv[]) {
	struct pollfd rfds[2];

	int nBufferLen = 0;
	char strBuffer[BUFSIZ];

	rfds[0].fd = 0;
	rfds[0].events = POLLIN;
	rfds[0].revents = 0;

	if (argc != 3) {
		printf("usage: %s <ip> <port>", argv[0]);
	}

	rfds[1].fd = connect_to_tcp_server(argv[1], atoi(argv[2]));
	rfds[1].events = POLLIN;
	rfds[1].revents = 0;

	int nRetval;
	int nTimeout = 1000;
	while (1) {
		nRetval = poll(rfds, 2, nTimeout);

		if (nRetval < 0) break;
		if (nRetval == 0) continue;

		if (rfds[0].revents & POLLIN) {
			bzero(strBuffer, BUFSIZ);
			nBufferLen = read(0, strBuffer, BUFSIZ);
			if (nBufferLen > 0) {
				send(rfds[1].fd, encrypt_aes(strBuffer, 1), sizeof(encrypt_aes(strBuffer, 1)) + 1, 0);
			}
		}

		if (rfds[1].revents & POLLIN) {
			bzero(strBuffer, BUFSIZ);
			nBufferLen = read(rfds[1].fd, strBuffer, BUFSIZ);
			if (nBufferLen > 0) {
				printf("%s\n", encrypt_aes(strBuffer, 2));
			}
		}

		if (nBufferLen <= 0) break;
	}

	close(rfds[1].fd);
	return 0;
}
