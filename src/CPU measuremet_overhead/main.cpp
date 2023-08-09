#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/resource.h>
#include <unistd.h>
#include "../helpers/macro.h"

static inline uint64_t getTimeDiffSingleOp() {
    unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;
    TICK();
    TOCK();
    uint64_t start = (((uint64_t) cycles_high0 << 32) | cycles_low0);
    uint64_t end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
    return end - start;
}

static inline uint64_t getTimeDiffForLoop(int numLoop) {
    unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;
    int counter = 0;

    TICK();
    for (int i = 0; i < numLoop; i++) {
      counter += 1;
    }
    TOCK();

    uint64_t start = (((uint64_t) cycles_high0 << 32) | cycles_low0);
    uint64_t end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
    return end - start;
}

int main(void)
{
    nice(-20);
    int numTests = 100;
    double sum = 0.0;
    for (int i = 0; i < numTests; i++) {
        sum += (double)getTimeDiffSingleOp();
    }
    printf("Cycles of reading time: %d cycles\n", (int)(sum / numTests));

    int numLoops = 1000;
    sum = 0.0;
    for (int i = 0; i < numTests; i++) {
        sum += (double)getTimeDiffForLoop(numLoops);
    }
    printf("Cycles of using a loop to measure %d iterations of an operation: %d cycles\n", numLoops, (int)(sum / numTests));
    return 0;
}
