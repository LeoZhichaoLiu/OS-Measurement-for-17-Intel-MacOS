#include <pthread.h>
#include "stdio.h"
#include <unistd.h>
#include <stdlib.h>

static const double TRIAL_NUM = 1000;

void* empty_func(void* emp){
    pthread_exit(NULL);
}

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

double thread_creation(){
    pthread_t id;
    uint64_t start, end;
    uint64_t total_cycle = 0;
    for (int i = 0; i < TRIAL_NUM; i++){
        start = rdtsc();
        pthread_create(&id, NULL, empty_func, NULL);
        //pthread_join(id, NULL);
        end = rdtsc();
        total_cycle += (end - start);
        // fprintf(output, "%llu\n", end-start);
        printf("%llu\n", end-start);
    }

    return total_cycle/TRIAL_NUM;
}



int main (){
    //FILE * thread_file= fopen("../processor/create_thread1.txt", "w");
    double thread = thread_creation();

    printf("thread creation time: %f", thread);
    return 0;
}