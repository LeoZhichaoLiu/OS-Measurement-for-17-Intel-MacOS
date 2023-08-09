#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>

static const int TRIAL_NUM = 5;
const int BLOCKSIZE = 4096;

static const char* mbtemp = "../filesystem/files/%iMB.txt";
static const char* gbtemp = "../filesystem/files/%iG.txt";

static const int MB = 1024*1024;
static const long GB = 1024*1024*1024;
static int MBnum = 11;

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

//double clockcycleToNanosec(double cycles) {
//    return cycles;
//}

//void calculate_acess_time(uint64_t filesize, int outfile, char* filename){
//    void* buf = malloc(BLOCKSIZE);
//    uint64_t time_total = 0;
//    uint64_t block_num =filesize/BLOCKSIZE;
//    int fd = open(filename, O_RDONLY| O_SYNC);
//
//    lseek(fd, filesize - 1, SEEK_SET);
//
//    // warm up stage
//    for (int i = 0; i < block_num; i++) {
//        lseek(fd, -2 * BLOCKSIZE, SEEK_CUR);
//        read(fd, buf, BLOCKSIZE);
//    }
//
//    // seek the last block first
//    for (int j = 0; j < TRIAL_NUM; j++){
//        // set the cursor to the end of the file
//        lseek(fd, filesize - 1, SEEK_SET);
//        for (int i = 0; i < block_num; i++) {
//            // move the cursor back to the start of the last block
//            lseek(fd, -2 * BLOCKSIZE, SEEK_CUR);
//            uint64_t start, end;
//            start = rdtsc();
//            int bytes = read(fd, buf, BLOCKSIZE);
//            end = rdtsc();
//            if (bytes <= 0){
//                break;
//            }
//            time_total += (end-start);
//        }
//        time_total/= block_num;
//
//    }
//
//    char buffer[100];
//    sprintf(buffer, "%f %f\n", filesize, time_total/(TRIAL_NUM));
//    printf("%d %d\n", filesize, time_total/(TRIAL_NUM));
//    int err = write(outfile, buf, 100);
//    if (err == 0) {
//        printf("error in write to result file");
//    }
//    free(buf);
//}

// read


void calculate_acess_time(uint64_t filesize, int outfile, char* filename){
    char* buf = malloc(BLOCKSIZE);
    float_t time_total = 0;
    uint64_t block_num =filesize/BLOCKSIZE;
    int fd = open(filename, O_RDONLY| O_SYNC);

    lseek(fd, 0, SEEK_SET);

    // warm up stage
    for (int i = 0; i < block_num; i++) {
        read(fd, buf, BLOCKSIZE);
        lseek(fd, BLOCKSIZE, SEEK_CUR);
    }

    // seek the last block first
    for (int j = 0; j < TRIAL_NUM; j++){
        // set the cursor to the end of the file
        lseek(fd, 0, SEEK_SET);
        uint64_t this_round = 0;
        for (int i = 0; i < block_num; i++) {
            // move the cursor back to the start of the last block
            uint64_t start, end;
            start = rdtsc();
            int bytes = read(fd, buf, BLOCKSIZE);
            end = rdtsc();
            if (bytes <= 0){
                break;
            }
            this_round += (end-start);
            lseek(fd, BLOCKSIZE, SEEK_CUR);
        }
        time_total += this_round/block_num;

    }

    double average_time_per_block = time_total/TRIAL_NUM;

    char buffer[100];
    sprintf(buffer, "%llu %f\n", filesize, average_time_per_block);
    printf("%llu %f\n", filesize, average_time_per_block);
    int err = write(outfile, buf, 100);
    if (err == 0) {
        printf("error in write to result file");
    }
    free(buf);
}


void calculate_big_acess_time(uint64_t filesize, int outfile, char* filename, uint64_t offset){
    char* buf = malloc(BLOCKSIZE);
    uint64_t time_total = 0;
    int block_num = filesize/BLOCKSIZE;
    int fd = open(filename, O_RDONLY| O_SYNC);

    lseek(fd, 0, SEEK_SET);

    // warm up stage
    for (int i = 0; i < block_num; i++) {
        lseek(fd, i*BLOCKSIZE*offset, SEEK_CUR);
        read(fd, buf, BLOCKSIZE);
    }

    // seek the last block first
    for (int j = 0; j < TRIAL_NUM; j++){
        // set the cursor to the end of the file
        lseek(fd, 0, SEEK_SET);

        for (int i = 0; i < block_num; i++) {
            // move the cursor back to the start of the last block
            lseek(fd, i * BLOCKSIZE*offset, SEEK_CUR);
            uint64_t start, end;
            start = rdtsc();
            int bytes = read(fd, buf, BLOCKSIZE);
            end = rdtsc();
            if (bytes <= 0){
                break;
            }
            time_total += (end-start);
        }

        time_total /= block_num;

    }

    char buffer[100];
    sprintf(buffer, "%llu %llu\n", filesize, time_total/(TRIAL_NUM));
    printf("bigfile %llu %llu\n", filesize, time_total/(TRIAL_NUM));
    int err = write(outfile, buf, 100);
    if (err == 0) {
        printf("error in write to result file");
    }
    free(buf);
}


void FBC_size(){

    uint64_t MBSizes[MBnum];

    for (int i = 0; i < MBnum; i++){
        MBSizes[i] = pow(2, i)*MB;
    }


    char* outputfile = "../filesystem/result/FBC_output.txt";
    int outfile = open(outputfile, O_RDWR| O_SYNC);
   // ssize_t outputSize = 0;

    for (int i = 0; i < MBnum; i++) {
        uint64_t filesize = MBSizes[i];
        char filename[100];
        int num = pow(2, i);
        sprintf(filename, mbtemp, num);
        calculate_acess_time(filesize, outfile, filename);
    }

    for (int i = 1; i < 5; i++) {
        int num = pow(2, i);
        uint64_t filesize = MBSizes[i]*1024;
        char filename[100];
        sprintf(filename, gbtemp, num);
        calculate_acess_time(filesize, outfile, filename);
    }
    close(outfile);

}

int main() {
    printf("%lld\n", LONG_LONG_MAX);
    FBC_size();
    return 0;
}
