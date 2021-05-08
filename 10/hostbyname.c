#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
	struct in_addr addr;
	char* ip_addr;

	addr.s_addr = htonl(0xC0A80ADB);
	printf("Network Address: 0x%X\n", addr);

	ip_addr = inet_ntoa(addr);
	printf("IP Addrees: %s\n", ip_addr);

	return 0;
}

