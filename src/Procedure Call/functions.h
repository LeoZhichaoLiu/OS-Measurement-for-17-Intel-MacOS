#include <stdint.h>

#ifndef _FUNCTION_H
#define _FUNCTION_H

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

void function0(){}

void function1(int a){}

void function2(int a, int b){}

void function3(int a, int b, int c){}

void function4(int a, int b, int c, int d){}

void function5(int a, int b, int c, int d, int e){}

void function6(int a, int b, int c, int d, int e, int f){}

void function7(int a, int b, int c, int d, int e, int f, int g){}

#endif