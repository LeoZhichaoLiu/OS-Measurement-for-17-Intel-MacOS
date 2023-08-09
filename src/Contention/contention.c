#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <unistd.h>

static const double BlockSize = 4096;

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

double clockcycleToNanosec(double cycles) {
    return cycles / 3100;
}

double random_access_time (char* filename, int filesize) {

    // Read file and setup buffer
    int fd = open(filename, O_RDWR|O_SYNC);
    void*buf = malloc(BlockSize);

    // Use fcntl for disable caching
    if (fcntl(fd, F_NOCACHE, 1) == -1){
        printf("Unable to disable cache\n");
    }

    // Caculate block numbers, and setup measurement variables
    int blocknum = (filesize*1024*1024)/BlockSize;
    long long start, end;
    long long cycle_count = 0;

    // Continue to read each block's data until terminataion
    for (int i = 0; i < blocknum; i++) {

        // Select random block index
        int idx = rand() % blocknum;
        start = rdtsc();

        // Go read that block
        lseek(fd, idx*BlockSize, SEEK_SET);
        int bytes = read(fd, buf, BlockSize);
        end = rdtsc();

        // Stop when finishing reading
        if (bytes <= 0) {
            break;
        }

        cycle_count += end - start;
    }

    close(fd);

    // Convert unit from cycle to ns
    double cycle_per_block = cycle_count / blocknum;
    return clockcycleToNanosec(cycle_per_block);
}


double sequential_access_time(char* filename,int filesize){
    
    // Read file and setup buffer
    int fd = open(filename, O_RDWR|O_SYNC);

    printf("%s\n", filename);
    void* buf = malloc(BlockSize);

    // Use fcntl for disable caching
    if (fcntl(fd, F_NOCACHE, 1) == -1){
        printf("Unable to disable cache\n");
    }

    // Caculate block numbers, and setup measurement variables
    int blocknum = (filesize*1024*1024)/BlockSize;
    long long cycle_count = 0;
    long long start = 0, end = 0;

    // Continue to read each block's data until terminataion
    for (int i = 0; i < blocknum; i++) {
        start = rdtsc();
        int bytes = read(fd, buf, BlockSize);
        end = rdtsc();

        // Stop when finishing reading
        if (bytes <= 0) {
            break;
        }

        cycle_count += end - start;
    }

    close(fd);

    // Convert unit from cycle to ns
    double cycle_per_block = cycle_count / blocknum;
    return clockcycleToNanosec(cycle_per_block);
}

int main(int argc, char* argv[]) {

    int process_num = atoll(argv[1]);

    printf("%d\n", process_num);

    int pid_table[process_num];

	for (int i = 0; i < process_num; i++) {
        int pid = fork();
        pid_table[i] = pid;
        if (pid_table[i] == 0) {
            char filename[18];
            sprintf(filename, "../content_file_%d", i);

            //double seq_time = sequential_access_time(filename, 32);
            //printf("The average time for sequential reading is %f\n", seq_time);

            //exit(0);

            double rand_time = random_access_time(filename, 32);
            printf("The average time for random reading is %f\n", rand_time);
            exit(0);
        } 
    }
}