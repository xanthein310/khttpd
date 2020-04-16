#include "fib.h"
#include <linux/preempt.h>
#include <linux/string.h>

void bigN_copy(bigN *x, bigN *y)
{
    int i;
    for (i = 0; i < bigN_num; i++)
        x->part[i] = y->part[i];
}

static void bigN_add(bigN x, bigN y, bigN *result)
{
    int i;
    long long carry = 0;

    preempt_disable();
    memset(result, 0, sizeof(bigN));
    preempt_enable();

    for (i = 0; i < bigN_num; i++) {
        long long tmp = carry + x.part[i] + y.part[i];
        result->part[i] = tmp % bigN_base;
        carry = tmp / bigN_base;
    }
}

static void bigN_sub(bigN x, bigN y, bigN *result)
{
    int i;

    preempt_disable();
    memset(result, 0, sizeof(bigN));
    preempt_enable();

    for (i = 0; i < bigN_num; i++) {
        result->part[i] += x.part[i] - y.part[i];
        if (result->part[i] < 0) {
            result->part[i] += bigN_base;
            result->part[i + 1]--;
        }
    }
}

static void bigN_mul(bigN x, bigN y, bigN *result)
{
    int i, j;

    preempt_disable();
    memset(result, 0, sizeof(bigN));
    preempt_enable();

    for (i = 0; i < bigN_num; i++) {
        long long carry = 0;
        for (j = 0; i + j < bigN_num; j++) {
            long long tmp = x.part[i] * y.part[j] + carry + result->part[i + j];
            result->part[i + j] = tmp % bigN_base;
            carry = tmp / bigN_base;
        }
    }
}

bigN fib_sequence(long long k)
{
    bigN fn, fn1, f2n, f2n1;
    bigN two;
    bigN temp[2];
    int count = 0;
    long long temp_k = k;

    memset(&fn, 0, sizeof(bigN));
    memset(&fn1, 0, sizeof(bigN));
    memset(&f2n, 0, sizeof(bigN));
    memset(&f2n1, 0, sizeof(bigN));
    memset(&two, 0, sizeof(bigN));
    memset(temp, 0, 2 * sizeof(bigN));

    two.part[0] = 2;

    fn1.part[0] = 1;

    if (k == 0)
        return fn;

    while ((temp_k & 0x8000000000000000) == 0) {
        count++;
        temp_k = temp_k << 1;
    }

    k = k << count;
    for (count = 64 - count; count > 0; count--) {
        bigN_mul(fn, fn, &temp[0]);
        bigN_mul(fn1, fn1, &temp[1]);
        bigN_add(temp[0], temp[1], &f2n1);
        bigN_mul(fn1, two, &temp[0]);
        bigN_sub(temp[0], fn, &temp[1]);
        bigN_mul(fn, temp[1], &f2n);
        if (k & 0x8000000000000000) {
            bigN_copy(&fn, &f2n1);
            bigN_add(f2n, f2n1, &fn1);
        } else {
            bigN_copy(&fn, &f2n);
            bigN_copy(&fn1, &f2n1);
        }
        k = k << 1;
    }

    return fn;
}
