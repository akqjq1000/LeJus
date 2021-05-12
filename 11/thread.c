#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void* thread_function (void *arg) {
	int *value = (int*)arg;

	for (int i = 0; i < 10; i++) {
		(*value)++;
		sleep(1);
	}
}

int main() {
	int value = 10;

	pthread_t pthread;

	pthread_create(&pthread, NULL, thread_function, (void*)&value);

	for (int i = 0; i < 10; i++) {
		printf("Value : %d\n", value);
		sleep(1);
	}

	pthread_join(pthread, NULL);

	return 0;
}
