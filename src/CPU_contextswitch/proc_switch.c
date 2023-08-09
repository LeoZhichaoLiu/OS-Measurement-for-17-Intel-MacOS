#include <sys/syscall.h>
#include <pthread.h>
#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <sys/fcntl.h>
#include <strings.h>
#include <limits.h>
#include <math.h>


static double TRIAL_NUM = 1000;
static int fd[2];

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

uint64_t process_context_switch(int pipe_overhead){

    fd[0] = open("../processor/input.txt", O_RDWR);
    fd[1] = open("../processor/output.txt", O_RDWR);
    pipe(fd);
    pid_t child_pid;


    uint64_t start, end = 0;

    uint64_t dummy;
    int status = 0;
    start = rdtsc();
    if ((child_pid = fork())==-1){
        perror("fork");
        exit(1);
    }

    if(child_pid == 0)
    {
        /* Child process closes up output side of pipe */
        start = rdtsc();
        write(fd[1], (void*) &start, sizeof(uint64_t));
        exit(0);
    }
    else
    {
        /* Parent process closes up input side of pipe */

        int id = wait(&status);
        //write(fd[1], (void*) &start, sizeof(uint64_t));
        read (fd[0], (void*)&start, sizeof(uint64_t));
        end = rdtsc();

        if (id == 0){
            perror("child exit sucessfully");
        }
        //printf("%f\n", end-start-pipe_overhead);
    }

    close(fd[0]);
    close(fd[1]);
    return floor((end-start-pipe_overhead)/2.0);
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

    // specify a
    int fd = open("../processor/result/context_switch_proc.txt", O_RDWR|O_SYNC);
    uint64_t proc_total =0;
    uint64_t counter = 0;
    for (int i = 0; i < TRIAL_NUM; i++){
        uint64_t proc = process_context_switch( floor(overhead));
        dprintf(fd, "%llu\n", proc);
        proc_total += proc;
    }
    printf("process context switch time: %f", proc_total/TRIAL_NUM);



    return 0;
}