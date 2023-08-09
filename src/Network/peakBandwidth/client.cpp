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
#include <iostream>
#define PORT 8080
#define PINGSIZE 1024

using namespace std;

int main(int argc, char const *argv[]) {
    int SendSize = 1;
    cout << "Please input the transfer size, the unit is KB: " << endl;
    cin >> SendSize;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *ping = new char[SendSize * PINGSIZE];
    for (int i = 0; i < SendSize * PINGSIZE; i++) {
        ping[i] = 'c';
    };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

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

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;

    int iterations = 100;
    int received = 0;
    double sum = 0;
    for (int i = 0; i < iterations; i++) {
        TICK();
        auto n = send(sock, ping, SendSize * PINGSIZE, 0);
        TOCK();
        received += n;
        uint64_t start = (((uint64_t) cycles_high0 << 32) | cycles_low0);
        uint64_t end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
        sum += (double) (end - start);
    }
    cout << received << endl;
    cout << sum << endl;
    printf("Peak Bandwidth is %f byte per cycle\n",
           (double) ( received / sum));
    return 0;
}
