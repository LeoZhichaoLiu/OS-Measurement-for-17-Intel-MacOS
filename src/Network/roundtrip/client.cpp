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
#define PINGSIZE 56

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char ping[PINGSIZE];
	for (int i = 0; i < PINGSIZE - 2; i++) {
		ping[i] = 'c';
	}
	ping[PINGSIZE - 2] = '$';
	ping[PINGSIZE - 1] = '\0';
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;

	int iterations = 100;
	double sum = 0;
	for (int i = 0; i < iterations; i++) {
  	TICK();
		write(sock , ping , sizeof(ping));
		read(sock , buffer, sizeof(buffer));
  	TOCK();
		printf("%s\n",buffer );
		bzero(buffer, sizeof(buffer));
		uint64_t start = (((uint64_t) cycles_high0 << 32) | cycles_low0);
  	uint64_t end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
		sum += (double)(end - start);
	}
	printf("Cycles of using a loop to measure average from %d iterations of an operation: %d cycles\n", iterations, (int)(sum / iterations));
	return 0;
}