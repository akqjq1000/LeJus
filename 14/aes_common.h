#include <stdio.h>
#include <openssl/aes.h>
#include <string.h>
#include <unistd.h>

char* encrypt_aes(char* data, int option) {
	char *retData = malloc(sizeof(data));
	unsigned char key[16] = {1,2,3,4,5,6,7,8,9,0};
	unsigned char iv[AES_BLOCK_SIZE] = {1,2,3,4,5,6,7,8,9,0};
	unsigned char iv_temp[AES_BLOCK_SIZE];
	unsigned char ciphertext[sizeof(data)];
	unsigned char deciphertext[sizeof(data)];
	int num = 0;
	AES_KEY ekey;

	AES_set_encrypt_key(key, 128, &ekey);
	memcpy(iv_temp, iv, AES_BLOCK_SIZE);
	AES_cfb128_encrypt((unsigned char*)data, retData, sizeof(data), &ekey, iv_temp, &num, (option == 1) ? AES_ENCRYPT : AES_DECRYPT);

	return retData;
}
