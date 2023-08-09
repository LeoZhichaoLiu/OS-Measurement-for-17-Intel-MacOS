#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/resource.h>
#include <unistd.h>
#include "../helpers/macro.h"

uint64_t measureProcedure() {
    unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;
    TICK();
    TOCK();
    uint64_t start = (((uint64_t) cycles_high0 << 32) | cycles_low0);
    uint64_t end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
    return end - start;
}

uint64_t measureSyscall() {
    unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;
    TICK();
    pid_t pid = getpid();
    TOCK();
    uint64_t start = (((uint64_t) cycles_high0 << 32) | cycles_low0);
    uint64_t end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
    return end - start;
}

int main(void)
{
    nice(-20);
    printf("syscall:%llu\n,procedure:%llu\n", measureSyscall(), measureProcedure());
    return 0;
}
