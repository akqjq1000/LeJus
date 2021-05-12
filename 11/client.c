#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int fd=-1;
	struct sockaddr_in sock_client;

	if (argc != 3) {
		return -1;
	}

	memset(&sock_client, 0, sizeof(sock_client));

	sock_client.sin_family = AF_INET;
	sock_client.sin_addr.s_addr = (in_addr_t)inet_addr(argv[1]);
	sock_client.sin_port = htons(atoi(argv[2]));

	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (connect(fd, (struct sockaddr*)&sock_client, sizeof(sock_client)) == 0) {
		printf("Connection Success.\n");
	} else {
		printf("Connection failure.\n");
	}

	printf("Press Enter key to finish.\n");
	getchar();

	close(fd);
	return 0;
}
