#include <pthread.h>
#include "stdio.h"
#include <unistd.h>
#include <stdlib.h>

static const double TRIAL_NUM = 1000;

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}


uint64_t process_creation(int counter){
    uint64_t start, end;
    uint64_t total_cycle = 0;

    start = rdtsc();
    int id = fork();
    if (id <= 0){
        printf("In child process %d\n", counter);
        exit(1);
    }else{
        wait(&id);
        end = rdtsc();
        total_cycle = (end - start);
        //printf("In parent process, child ID: %d\n", id);
        //fprintf(output, "%llu\n", end-start);
    }


    return total_cycle;
}


int main (){

    FILE * proc_file= fopen("../processor/create_proc.txt", "w");
    double total  = 0;
    for (int i = 0; i < TRIAL_NUM; i++) {
        uint64_t proc = process_creation(i);
        printf("counter %d\n", i);
        fprintf(proc_file, "counter %d\n", i);
        //fprintf(proc_file, "%llu\n", proc);
        total += proc;
    }


    printf("process creation time: %f", total/TRIAL_NUM);
    fclose(proc_file);
    return 0;
}