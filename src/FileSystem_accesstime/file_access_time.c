#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <unistd.h>

static const char* mbtemp = "../filesystem/files/%iMB.txt";
static const char* gbtemp = "../filesystem/files/%iG.txt";


static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

double clockcycleToNanosec(double cycles) {
    return cycles;
}

static const double BlockSize = 4096;
static const int MB = 1024*1024;
static const long GB = 1024*1024*1024;

double random_access_time(char* filename, uint64_t filesize){
    int fd = open(filename, O_RDWR|O_SYNC);
    void*buf = malloc(BlockSize);
    if (fcntl(fd, F_NOCACHE, 1) == -1){
        printf("Fail to disable cache");
    }

    uint64_t blocknum = filesize/BlockSize;
    uint64_t total_cycle = 0;
    for (int i = 0; i < blocknum; i++) {
        int idx = rand() % blocknum;
        uint64_t start, end;
        start = rdtsc();
        lseek(fd, idx*BlockSize, SEEK_SET);
        int bytes = read(fd, buf, BlockSize);
        end = rdtsc();
        if (bytes <= 0) {
            break;
        }
        total_cycle += end-start;
    }

    close(fd);
    return clockcycleToNanosec(total_cycle/blocknum);
}


double sequential_access_time(char* filename, uint64_t filesize){
    int fd = open(filename, O_RDWR|O_SYNC);
    void* buf = malloc(BlockSize);
    if (fcntl(fd, F_NOCACHE, 1) == -1){
        printf("Fail to disable cache");
    }
    uint64_t blocknum = filesize/BlockSize;
    uint64_t total_cycle = 0;
    for (int i = 0; i < blocknum; i++) {
        uint64_t start, end;
        start = rdtsc();
        int bytes = read(fd, buf, BlockSize);
        if (bytes <= 0) {
            break;
        }
        end = rdtsc();
        total_cycle += end-start;
    }

    close(fd);
    return clockcycleToNanosec(total_cycle/blocknum);

}

void calculate_file_access_time() {

    int MBnum = 10;
    int GBnum = 5;
    uint64_t MBSizes[MBnum];
    uint64_t GBSizes[GBnum];

    for (int i = 0; i < MBnum; i++){
        MBSizes[i] = pow(2, i)*MB;
    }

    for (int i = 0; i < GBnum; i++){
        GBSizes[i] = pow(2, i)*GB;
    }

    for (int i = 0; i < MBnum; i++) {
        char filename[200];
        int num = pow(2,i);
        sprintf(filename, mbtemp, num);
        double rand_time = random_access_time(filename, MBSizes[i]);
        double seq_time = sequential_access_time(filename, MBSizes[i]);
        printf("%d %f %f\n", num, seq_time, rand_time);
    }


//    for (int i = 0; i < MBnum; i++) {
//        char filename[200];
//        int num = pow(2,i);
//        sprintf(filename, mbtemp, num);
//        double seq_time = sequential_access_time(filename, MBSizes[i]);
//
//        //printf("The average time for sequential reading is %f\n", seq_time);
//
//        //double rand_time = random_access_time(filename, MBSizes[i]);
//        printf("%d %f\n", num, seq_time);
//    }


//    for (int i = 0; i < GBnum; i++) {
//        char filename[200];
//        int num = pow(2,i);
//        sprintf(filename, gbtemp, num);
//        double seq_time = sequential_access_time(filename, GBSizes[i]);
//        printf("The average time for random reading is %f\n", seq_time);
//
//        double rand_time = random_access_time(filename, GBSizes[i]);
//        printf("The average time for random reading is %f\n", rand_time);
//    }

}


int main() {
    calculate_file_access_time();
    return 0;
}
