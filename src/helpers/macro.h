#define TICK()\
{\
    asm volatile (\
        "cpuid\n\t"\
        "rdtsc\n\t"\
        "mov %%edx, %0\n\t"\
        "mov %%eax, %1\n\t"\
        : "=r" (cycles_high0), "=r" (cycles_low0)::"%rax", "%rbx", "%rcx", "%rdx");\
}

#define TOCK()\
{\
    asm volatile (\
        "rdtscp\n\t"\
        "mov %%edx, %0\n\t"\
        "mov %%eax, %1\n\t"\
        "cpuid\n\t"\
        : "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx");\
}