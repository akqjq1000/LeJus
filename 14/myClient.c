#include "network_common.h"
#include <sys/poll.h>
#include <openssl/aes.h>
#include <string.h>
#include <openssl/aes.h>

int main(int argc, char* argv[]) {
	struct pollfd rfds[2];

	int nBufferLen = 0;
	char strBuffer[BUFSIZ];

	rfds[0].fd = 0;
	rfds[0].events = POLLIN;
	rfds[0].revents = 0;

	unsigned char key[16] = {1,2,3,4,5,6,7,8,9,0};
	unsigned char iv[16] = {1,2,3,4,5,6,7,8,9,0};
	unsigned char iv_temp[AES_BLOCK_SIZE];
	unsigned char ciphertext[BUFSIZ];
	unsigned char deciphertext[BUFSIZ];
	int num = 0;
	AES_KEY ekey;

	AES_set_encrypt_key(key, 128, &ekey);

	if (argc != 3) {
		printf("usage: %s <ip> <port>", argv[0]);
	}

	rfds[1].fd = connect_to_tcp_server(argv[1], atoi(argv[2]));
	rfds[1].events = POLLIN;
	rfds[1].revents = 0;

	int nRetval;
	int nTimeout = 1000;
	while (1) {
		num = 0;
		bzero(ciphertext, BUFSIZ);
		bzero(deciphertext, BUFSIZ);
		nRetval = poll(rfds, 2, nTimeout);

		if (nRetval < 0) break;
		if (nRetval == 0) continue;

		if (rfds[0].revents & POLLIN) {
			bzero(strBuffer, BUFSIZ);
			nBufferLen = read(0, strBuffer, BUFSIZ);
			printf("%s\n", strBuffer);
			if (nBufferLen > 0) {
				memcpy(iv_temp, iv, AES_BLOCK_SIZE);
				AES_cfb128_encrypt(strBuffer, ciphertext, sizeof(strBuffer), &ekey, iv_temp, &num, AES_ENCRYPT);
				send(rfds[1].fd, ciphertext, sizeof(ciphertext), 0);
			}
		}

		if (rfds[1].revents & POLLIN) {
			bzero(strBuffer, BUFSIZ);
			nBufferLen = read(rfds[1].fd, strBuffer, BUFSIZ);

			if (nBufferLen > 0) {
				memcpy(iv_temp, iv, AES_BLOCK_SIZE);
				AES_cfb128_encrypt(strBuffer, deciphertext, sizeof(strBuffer), &ekey, iv_temp, &num, AES_DECRYPT);
				printf("%s\n", deciphertext);
			}
		}

		if (nBufferLen <= 0) break;
	}

	close(rfds[1].fd);
	return 0;
}
