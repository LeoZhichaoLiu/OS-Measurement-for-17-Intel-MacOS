#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <inttypes.h>
#include <time.h>

unsigned long long memory_write(int * Array);
unsigned long long memory_read (int * Array);

static __inline__ unsigned long long int rdtsc(void)
{
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}


int main(int argc, char const *argv[]) {

    // 64MB
    int * Array = (int *)malloc(4096 * 4096 * 4);

    for (int i = 0; i < 4096 * 4096; i++) {
        Array[i] = 0;
    }

    unsigned long long write_bandwidth = memory_write(Array);
    unsigned long long read_bandwidth = memory_read(Array);

    printf("%llu\n", write_bandwidth);
    printf("%llu\n", read_bandwidth);

    free(Array);
}


unsigned long long memory_write(int * Array) {
    unsigned long long start_time = 0;
    unsigned long long end_time = 0;
    unsigned long long total_time = 0;

    for (int i = 0; i < 4096 * 4096; i += 32) {
        start_time = rdtsc();
        Array[i] = 1;
        Array[i+1] = 1;
        Array[i+2] = 1;
        Array[i+3] = 1;
        Array[i+4] = 1;
        Array[i+5] = 1;
        Array[i+6] = 1;
        Array[i+7] = 1;
        Array[i+8] = 1;
        Array[i+9] = 1;
        Array[i+10] = 1;
        Array[i+11] = 1;
        Array[i+12] = 1;
        Array[i+13] = 1;
        Array[i+14] = 1;
        Array[i+15] = 1;
        Array[i+16] = 1;
        Array[i+17] = 1;
        Array[i+18] = 1;
        Array[i+19] = 1;
        Array[i+20] = 1;
        Array[i+21] = 1;
        Array[i+22] = 1;
        Array[i+23] = 1;
        Array[i+24] = 1;
        Array[i+25] = 1;
        Array[i+26] = 1;
        Array[i+27] = 1;
        Array[i+28] = 1;
        Array[i+29] = 1;
        Array[i+30] = 1;
        Array[i+31] = 1;
        end_time = rdtsc();
        total_time += (end_time-start_time);
    }


    return (unsigned long long)(4096.0 * 4096.0 * 4.0) / ((total_time)/(2133*2));
}

unsigned long long memory_read (int * Array) {

    unsigned long long start_time = 0;
    unsigned long long end_time = 0;
    unsigned long long total_time = 0;

    unsigned long long sum = 0;
   
    for (int i = 0; i < 4096 * 4096; i += 32) {
        start_time = rdtsc();
        sum += Array[i] + Array[i+1] + Array[i+2] + Array[i+3] +Array[i+4]
        + Array[i+5] + Array[i+6] + Array[i+7] + Array[i+8] + Array[i+9]
        + Array[i+10] + Array[i+11] + Array[i+12] + Array[i+13] + Array[i+14] 
        + Array[i+15] + Array[i+16] + Array[i+17] + Array[i+18] + Array[i+19] +Array[i+20]
        + Array[i+21] + Array[i+22] + Array[i+23] + Array[i+24] + Array[i+25]
        + Array[i+26] + Array[i+27] + Array[i+28] + Array[i+29] + Array[i+30] 
        + Array[i+31];
        end_time = rdtsc();
        total_time += (end_time-start_time);
    }
    

    return 4096.0 * 4096.0 * 4.0 / ((total_time)/(2133*2));
}