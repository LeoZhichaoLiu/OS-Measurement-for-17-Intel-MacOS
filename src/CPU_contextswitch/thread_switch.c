#include <sys/syscall.h>
#include <pthread.h>
#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <sys/fcntl.h>
#include <strings.h>
#include <limits.h>
#include <math.h>

static double TRIAL_NUM = 2000;
static int fd[2];

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

static inline void* pthread_func(void* emp){
    uint64_t s = rdtsc();
    int n = write(fd[1], (void*) &s, sizeof(uint64_t));
    if (n < 0){
        perror("writing to pipe fails");
    }
    pthread_exit(NULL);
}

double thread_context_switch(double pipe_overhead){

    pthread_t id;
    uint64_t end;
    pipe(fd);

    uint64_t dummy;

    pthread_create(&id, NULL, pthread_func, NULL);
    uint64_t start = rdtsc();
    read(fd[0], (void *)&start, sizeof (uint64_t));
    pthread_join(id, NULL);
    end = rdtsc();

    return end-start-pipe_overhead;
}


double calc_pipe_overhead(){

    fd[0] = open("../processor/input.txt", O_RDWR);
    fd[1] = open("../processor/output.txt", O_RDWR);
    pipe(fd);
    uint64_t start, end;
    uint64_t total_cycle = 0;
    for (int i = 0; i < TRIAL_NUM; i++){

        uint64_t num = LONG_MAX;
        uint64_t out;
        start = rdtsc();
        write(fd[1], (void*) &num, sizeof(uint64_t));
        read ( fd[0], (void*) &out, 1024 );
        end = rdtsc();

        total_cycle += end - start;
        //printf("transfer LONG_MAX: %llu\n", out);
    }

    close(fd[0]);
    close(fd[1]);

    return total_cycle/TRIAL_NUM;


}


int main (){
    double overhead = calc_pipe_overhead();
    //printf("overhead %f\n", overhead);

//    FILE * thread_file = fopen("../processor/result/context_switch_thread.txt", "w");
    double thread_total = 0;
    for (int i = 0; i < TRIAL_NUM; i++){
        double thread = thread_context_switch( overhead);
        printf("%d\n", (int) floor(thread));
        thread_total += thread;
    }
    printf("thread context switch time: %f", thread_total/TRIAL_NUM);



    return 0;
}