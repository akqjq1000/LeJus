#include <stdio.h>
#include <unistd.h>


// little endian (int = 32bit)
int main() {
	int nValue = 0x12345678;

	printf("Value : 0x%08X\n", nValue);

	for (int i = 0; i < 4; i++) {
		char * pC = (char*) &nValue + i;
		printf("%p : 0x%02X\n", pC, *pC);
	}

	printf("\n");
	return 0;
}
