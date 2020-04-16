#ifndef __FIB_H
#define __FIB_H

#define bigN_base 100000000
#define bigN_num 32

typedef struct bigN_t {
    long long part[bigN_num];
} bigN;

bigN fib_sequence(long long k);
#endif
