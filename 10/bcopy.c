#include <stdio.h>
#include <string.h>
#include <strings.h>

#define TEXT_SIZE 20

int main() {
	char p_strText1[TEXT_SIZE] = {0, };
	char p_strText2[TEXT_SIZE] = {0, };

	for (int i = 0; i < TEXT_SIZE; i++) {
		p_strText1[i] = 'a' + i;
	}

	printf("Text1: %s\n", p_strText1);
	printf("Text2: %s\n", p_strText2);

	printf("Calling bcopy().\n");
	bcopy(p_strText1, p_strText2, TEXT_SIZE);

	printf("Text1: %s\n", p_strText1);
	printf("Text2: %s\n", p_strText2);

	return 0;
}
