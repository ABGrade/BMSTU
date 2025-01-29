#ifndef __IO_H__
#define __IO_H__

#include "functions.h"

int read_bigint(char str[], bigint_t *big_num);
int read_exponential(char str[], bigint_t *big_num);

int print_bigint(bigint_t big_num);

#endif // __IO_H__