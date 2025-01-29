#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#define MAX_NUMS 40
#define ORDER_MAX_SIZE 99999
#define BUFFER_SIZE 256
#define MAX_ORDER_SIZE 5

typedef struct 
{
    char sign;
    int mantissa[MAX_NUMS + 1];
    size_t size;
    int order;
} bigint_t;

bigint_t bigint_initialize(void);

int string_to_bigint(char *string, bigint_t *big_num);
char to_digit(char symb);

int insert(bigint_t *big_num, int num);
int insert_first(bigint_t *big_num, int push_amount);
void del_leading_zeros(bigint_t *big_num);
void big_round(bigint_t *big_num);

void show_bigint(bigint_t big_num);

#endif