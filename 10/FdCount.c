#include <stdio.h>// standard I/O
#include <unistd.h>// Unix function
#include <time.h>// time
#include <fcntl.h>// low level function
#include <stdlib.h>// random()
#include <string.h>// memset()

// Max File Count
#define MAX_FD 16

int main() {
	// File Descripter Array
	int fd[MAX_FD];
	int fdCnt;// File Count
	int nRandom;
	char buffer[BUFSIZ];// file name
	time_t currentTime;
	memset(fd, 0, sizeof(fd));// memory initialize

	currentTime = time(NULL);// get current time by seconds
	srandom((unsigned)currentTime);// set random seed 

	nRandom = (random() % MAX_FD) + 1;// 1~16

	for(fdCnt = 0; fdCnt < nRandom; fdCnt++) {
		sprintf(buffer, "./test%d.txt", fdCnt);// set filename
		fd[fdCnt] = open(buffer, O_CREAT | O_RDONLY, 0644);// create file(chmod = 644)
	}

	printf("Process Number : %d\n", getpid());// print process id
	printf("The number of FDs is %d\n", fdCnt);// print file count
	getchar();// enter key wait

	// file descripter close
	for(int i=0; i < MAX_FD; i++) {
		if (fd[i] > 0) {
			close(fd[i]);
		}
	}

	// file remove
	for(int i=0; i< fdCnt; i++) {
		sprintf(buffer, "./test%d.txt", i);
		unlink(buffer);
	}
	return 0;
}
