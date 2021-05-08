#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

void print_hostent(struct hostent* host);

int main(int argc, char* argv[]) {
	struct hostent* host;
	uint32_t addr = 0;

	if (argc != 2) return -1;
	
	addr = inet_addr(argv[1]);
	host = gethostbyaddr(&addr, 4, AF_INET);
	if (!host) {
		printf("gethostbyaddr function error\n");
		return -1;
	}

	print_hostent(host);

	printf("Done.\n");

	return 0;
}

void print_hostent(struct hostent* host) {
	printf("Official name of host: %s\n\n", host->h_name);
	if (host->h_aliases[0] != NULL) {
		printf("--Aliases--\n");
		for (int i = 0; host->h_aliases[i]; i++) {
			printf("%02dth: %s\n", i + 1, host->h_aliases[i]);
		}
		printf("\n");
	}

	printf("Address type: %s\n\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

	printf("IP Address: \n");
	for (int i = 0; host->h_addr_list[i]; i++) {
		printf("%02d: %s\n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	}

}
