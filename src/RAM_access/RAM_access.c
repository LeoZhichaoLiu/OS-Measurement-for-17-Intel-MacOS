#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <mach/mach_time.h>
#include <string.h>
#include <math.h>


// L1 size: 32kb
// L2 size: 256kb
// L3 size: 10MB

// Based on this, we will initialize different int64 array,
// with stride set to eliminate the effect of cache line and prefetching

// Basic calculation
const int  mb = 1024*1024;
const int kb = 1024;
const int REF_COUNT = 100000;


// already divided int size
//static int strideList[11] = {1, 2, 8, 16,  128, 512, kb, 4*kb, 64*kb, 128*kb, mb};

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}


void getAverageMemReference(int strideSize, int arraysize, FILE *file);
double clockcycleToNanosec(double cycles);

int main(int argc, const char * argv[]){

    FILE *file;
    file = fopen("./out.txt", "w");

    static int strideList[10];

    for (int i = 0; i <10; i++){
        strideList[i] = pow(2, i);
    }
    static int sizeList[20];
    for (int i = 0; i < 20; i ++){
        sizeList[i] = pow(2,i)*256;
    }


    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            getAverageMemReference(strideList[i], sizeList[j], file);
        }
    }

    fclose(file);

}

void getAverageMemReference(int strideSize, int arraysize, FILE *file) {

        //Step 1: malloc a new array
        int* data;
        data = (int *)malloc(arraysize*sizeof(int));


        int index;
        // Initializing array
        for (int j = 0; j < arraysize; j++){
            index = j + strideSize;

            if (index >= arraysize) {
                index %= arraysize;
            }

            data[j] = index;
        }

        uint64_t start, end;

        int nextIdx = 0;
        start = rdtsc();
        for (int j = 0; j < REF_COUNT; j++){
            nextIdx = data[nextIdx];
        }
        end = rdtsc();
        double cycles_total = 1.0*(end - start)/REF_COUNT;
        double time_total = clockcycleToNanosec(cycles_total);

        free(data);
        fprintf(file, "%i %i %f %f\n", strideSize, arraysize, cycles_total, time_total);
        printf("%i %i %f %f\n", strideSize, arraysize, cycles_total, time_total);

}

double clockcycleToNanosec(double cycles) {
    return cycles*10/24;
}