// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/resource.h>
#include "../helpers/macro.h"
#define PORT 8080
#define NUMITER 100

using namespace std;

unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;

int main(int argc, char const *argv[])
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if (argc == 2) {
        if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }
    }
    else {
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }
    }


	double duration = 0;
    for (size_t i = 0; i < NUMITER; i++)
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		TICK();
		connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
		TOCK();
		uint64_t start = (((uint64_t) cycles_high0 << 32) | cycles_low0);
  		uint64_t end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
		duration += (double)(end - start);
		close(sock);
	}

	printf("Average setup time is %f cycle\n", duration / NUMITER);
	return 0;
}
