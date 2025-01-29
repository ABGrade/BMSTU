#ifndef __CALCULATIONS_H__
#define __CALCULATIONS_H__

#include "functions.h"

bigint_t subtract(bigint_t minuend, bigint_t subtrahend);
bigint_t division(bigint_t dividend, bigint_t divider);
bigint_t multiply_by_short(bigint_t big_num, int num);

int compare_bigint(bigint_t big_num1, bigint_t big_num2);

int calc_digits(int num);


#endif // __CALCULATIONS_H__