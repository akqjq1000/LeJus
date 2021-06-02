#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/aes.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
	int nFd_In = -1;
	int nFd_Out = -1;
	int nRead, nWritten;
	unsigned char p_cInputData[AES_BLOCK_SIZE];
	unsigned char p_cOutputData[AES_BLOCK_SIZE];
	int bEncryption = false;
	int nOpt = -1;

	unsigned char p_ckey[16] = {1,2,3,4,5,6,7,8,9,0};
	unsigned char p_cIvec[16] = {1,2,3,4,5,6,7,8,9,0};

	AES_KEY stKey;

	if (argc != 4) {
		printf("usage: %s <-e/-d> <in-file> <out-file>\n", argv[0]);
		return -1;
	}

	while ((nOpt=getopt(argc, argv, "ed")) > 0) {
		switch(nOpt) {
			case 'e':
				bEncryption = true;
				break;
			case 'd':
				bEncryption = false;
				break;
			default:
				printf("Usage: %s <-e/-d> <in-file> <out-file>\n", argv[0]);
				return -1;
		}
	}

	printf("CKEY=");
	for (int i = 0; i < 16; i++) {
		printf("%02X", p_ckey[i]);
	}
	printf("\n");

	printf("IVEC=");
	for (int i = 0; i < 16; i++) {
		printf("%02X", p_cIvec[i]);
	}
	printf("\n");

	printf("Input File: %s\n", argv[optind]);
	printf("OutputFile: %s\n", argv[optind + 1]);

	nFd_In = open(argv[optind], O_RDONLY);
	nFd_Out = open(argv[optind + 1], O_WRONLY | O_CREAT, 0644);

	AES_set_encrypt_key(p_ckey, 128, &stKey);

	int nBlockIndex = 0;
	while (1) {
		nRead = read(nFd_In, p_cInputData, AES_BLOCK_SIZE);
		if (nRead <= 0) break;
		if (bEncryption)	AES_cfb128_encrypt(p_cInputData, p_cOutputData, nRead, &stKey, p_cIvec, &nBlockIndex, AES_ENCRYPT);
		else AES_cfb128_encrypt(p_cInputData, p_cOutputData, nRead, &stKey, p_cIvec, &nBlockIndex, AES_DECRYPT);

		nWritten = write(nFd_Out, p_cOutputData, nRead);
	}

	close(nFd_In);
	close(nFd_Out);

	return 0;
}

