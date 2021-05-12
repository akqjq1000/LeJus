#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {

	int nPid = 0;

	nPid = fork();

	if (nPid == 0) printf("child PID: %d\n", nPid);
	else printf("parent PID: %d\n", nPid);

	return 0;
}
