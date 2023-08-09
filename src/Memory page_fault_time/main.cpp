#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/resource.h>
#include <unistd.h>
#include <condition_variable>
#include "../../helpers/macro.h"
#include <iostream>
#include <math.h>
#include <fcntl.h>
#include <sys/mman.h>

using namespace std;

unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;


#define PAGESIZE 4096

#define STRIDE 10

#define NUMTEST 100

#define NUMITER 10

#define OFFSET PAGESIZE * STRIDE

#define FILESIZE 10 * 1024 * 1024

#define FILEPATH "./temp.txt"

#define ONE(i) temp = map[i * OFFSET];

#define FIVE(i) ONE(i) ONE(i + 1) ONE(i + 2) ONE(i + 3) ONE(i + 4)

#define TWENTYFIVE(i) FIVE(i) FIVE(i + 5) FIVE(i + 10) FIVE(i + 15) FIVE(i + 20)

#define HUNDRED(i) TWENTYFIVE(i) TWENTYFIVE(i + 25) TWENTYFIVE(i + 50) TWENTYFIVE(i + 75)



int main(int argc, char *argv[]) {
    nice(-20);
    
    int fd = open(FILEPATH, O_RDWR | O_CREAT);
    if (fd < 0) {
        printf("Open file failed\n");
        return 1;
    }

    char* map =(char*) mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    char temp;
    
    double duration = 0;

    for (size_t i = 0; i < NUMITER; i++)
    {
        system("sudo purge");
        TICK()

        HUNDRED(0)

        TOCK()

        uint64_t start = (((uint64_t) cycles_high0 << 32) | cycles_low0);
        uint64_t end = (((uint64_t) cycles_high1 << 32) | cycles_low1);

        duration += (end - start) / NUMTEST;
    }
    

    printf("Page Fault per page : %f\n", duration / NUMITER);

    munmap(map, FILESIZE);
    close(fd);
    return 0;
}
