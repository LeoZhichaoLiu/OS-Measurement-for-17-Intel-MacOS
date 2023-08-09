#include "procedure_call.h"
#include "functions.h"


void calculate() {
    uint64_t start_time;
    uint64_t end_time;
    uint64_t count = 0;

    //rdtsc();
    for (int i = 0; i < 1000000; i++) {
        start_time = rdtsc();
        function0();
        end_time = rdtsc();
        count += (end_time - start_time);
    }
    std::cout << ((double)count / (double)1000000) << std::endl;
    count = 0;

    rdtsc();
    for (int i = 0; i < 1000000; i++) {
        start_time = rdtsc();
        function1(1);
        end_time = rdtsc();
        count += (end_time - start_time);
    }
    std::cout << ((double)count / (double)1000000) << std::endl;
    count = 0;

    rdtsc();
    for (int i = 0; i < 1000000; i++) {
        start_time = rdtsc();
        function2(1,1);
        end_time = rdtsc();
        count += (end_time - start_time);
    }
    std::cout << ((double)count / (double)1000000) << std::endl;
    count = 0;

    rdtsc();
    for (int i = 0; i < 1000000; i++) {
        start_time = rdtsc();
        function3(1,1,1);
        end_time = rdtsc();
        count += (end_time - start_time);
    }
    std::cout << ((double)count / (double)1000000) << std::endl;
    count = 0;

    rdtsc();
    for (int i = 0; i < 1000000; i++) {
        start_time = rdtsc();
        function4(1,1,1,1);
        end_time = rdtsc();
        count += (end_time - start_time);
    }
    std::cout << ((double)count / (double)1000000) << std::endl;
    count = 0;

    rdtsc();
    for (int i = 0; i < 1000000; i++) {
        start_time = rdtsc();
        function5(1,1,1,1,1);
        end_time = rdtsc();
        count += (end_time - start_time);
    }
    std::cout << ((double)count / (double)1000000) << std::endl;
    count = 0;

    rdtsc();
    for (int i = 0; i < 1000000; i++) {
        start_time = rdtsc();
        function6(1,1,1,1,1,1);
        end_time = rdtsc();
        count += (end_time - start_time);
    }
    std::cout << ((double)count / (double)1000000) << std::endl;
    count = 0;

    rdtsc();
    for (int i = 0; i < 1000000; i++) {
        start_time = rdtsc();
        function7(1,1,1,1,1,1,1);
        end_time = rdtsc();
        count += (end_time - start_time);
    }
    std::cout << ((double)count / (double)1000000) << std::endl;

    return;
}