#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/rc4.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
	int nFd_In = -1;
	int nFd_Out = -1;

	unsigned char p_cKey[16] = {1,2,3,4,5,6,7,8,9,0};

	int nDataLen;
	unsigned char p_cInputData[BUFSIZ] = {0};
	unsigned char p_cOutputData[BUFSIZ] = {0};

	RC4_KEY stRc4Key;

	if (argc != 3) return -1;

	printf("KEY=");
	for (int i = 0; i < 16; i++) {
		printf("%02X", p_cKey[i]);
	}
	printf("\n");

	nFd_In = open(argv[1], O_RDONLY);
	nFd_Out = open(argv[2], O_WRONLY | O_CREAT, 0644);

	printf("set key.\n");
	RC4_set_key(&stRc4Key, 16, p_cKey);

	printf("Start to encrypt.\n");
	while (1) {
		nDataLen = read(nFd_In, p_cInputData, BUFSIZ);
		if (nDataLen <= 0) break;

		RC4(&stRc4Key, nDataLen, p_cInputData, p_cOutputData);

		if (write(nFd_Out, p_cOutputData, nDataLen) <= 0) break;
	}

	close(nFd_In);
	close(nFd_Out);

	printf("En/De-cryption is done.\n");
	return 0;
}
