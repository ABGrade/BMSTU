#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "functions.h"
#include "errors.h"
#include "io.h"

int read_bigint(char str[], bigint_t *big_num)
{
    char zero = 0;
    size_t i = 0;

    // Пропускаем начальные пробелы
    while (isspace(str[i]))
    {
        i++;
    }

    // Проверяем знак
    if (str[i] == '+')
    {
        i++;
    }
    else if (str[i] == '-')
    {
        big_num->sign = -1;
        i++;
    }


    // Считываем цифры
    while (isdigit(str[i]))
    {
        if (str[i] == '0' && big_num->size == 0)
        {
            zero = 1;
            i++;
            continue;
        }

        big_num->size++;
        if (big_num->size > 40)
        {
            return OVERFLOW_ERROR;
        }

        big_num->mantissa[big_num->size - 1] = to_digit(str[i]);
        i++;
    }

    if (big_num->size == 0 && zero)
    {
        big_num->mantissa[big_num->size++] = 0;
    }
    

    // Пропускаем конечные пробелы
    while (isspace(str[i]))
    {
        i++;
    }

    if (str[i] != '\0' || big_num->size == 0)
    {
        return READ_FAIL;
    }
    return 0;
}

int read_exponential(char str[], bigint_t *big_num)
{
    char zero = 0;
    char e_count = 0;
    size_t i = 0;

    // Пропускаем начальные пробелы
    while (isspace(str[i]))
    {
        i++;
    }

    // Проверяем знак
    if (str[i] == '+')
    {
        i++;
    }
    else if (str[i] == '-')
    {
        big_num->sign = -1;
        i++;
    }

    // Проверяем цифры до точки или e/E
    while (isdigit(str[i]))
    {
        if (str[i] == '0' && big_num->size == 0)
        {
            zero = 1;
            i++;
            continue;
        }

        big_num->size++;
        if (big_num->size > MAX_NUMS)
        {
            return OVERFLOW_ERROR;
        }

        big_num->mantissa[big_num->size - 1] = to_digit(str[i]);
        i++;
    }

    // Проверяем точку и цифры после точки
    if (str[i] == '.')
    {
        i++;
        while (isdigit(str[i]))
        {
            big_num->size++;
            if (big_num->size > MAX_NUMS)
            {
                return OVERFLOW_ERROR;
            }
            big_num->mantissa[big_num->size - 1] = to_digit(str[i]);
            big_num->order--;
            i++;
        }
    }

    if (big_num->size == 0 && zero)
    {
        big_num->mantissa[big_num->size++] = 0;
    }
    
    if (big_num->size == 0)
    {
        return INCORRECT_STRING;
    }
    
    char order[MAX_ORDER_SIZE + 1] = "\0";
    char order_sign = 1;
    size_t order_size = 0;
    zero = 0;

    // Проверяем e/E и цифры после него
    if (str[i] == 'e' || str[i] == 'E')
    {
        i++;
        e_count++;

        // Проверяем знак
        if (str[i] == '+')
        {
            i++;
        }
        else if (str[i] == '-')
        {
            order_sign = -1;
            i++;
        }

        while (isdigit(str[i]))
        {
            if (str[i] == '0' && order_size == 0)
            {
                zero = 1;
                i++;
                continue;
            }
            order_size++;
            if (order_size > MAX_ORDER_SIZE)
            {
                return ORDER_OVERFLOW_ERROR;
            }
            order[order_size - 1] = str[i];
            i++;
        }
        if (order_size)
        {
            big_num->order += order_sign * atoi(order);
        }
    }

    if (e_count && (!order_size) && !zero)
    {
        return AFTER_E_ERROR;
    }
    
    // Пропускаем конечные пробелы
    while (isspace(str[i]))
    {
        i++;
    }

    // Если остались непрочитанные символы или неверное количество точек/е/E/цифр
    if (str[i] != '\0' || big_num->size == 0)
    {
        return READ_FAIL;
    }
    del_leading_zeros(big_num);

    return 0;
}

int print_bigint(bigint_t big_num) 
{
    big_round(&big_num);

    int adjusted_order = big_num.size + big_num.order;
    if (adjusted_order > 99999 || adjusted_order < -99999)
    {
        return ORDER_RES_OVERFLOW_ERROR;
    }

    // Определим знак
    if (big_num.sign == -1) 
    {
        printf("-");
    }

    // Выводим число в формате 0.m1
    printf("0.");

    int last_zeros = 0;
    for (int i = big_num.size - 1; i >= 0 && big_num.mantissa[i] == 0; i--)
    {
        last_zeros++;
    }
    
    for (size_t i = 0; i < big_num.size - last_zeros; i++) 
    {
        printf("%d", big_num.mantissa[i]);
    } 

    printf("E%d\n", adjusted_order);
    return 0;
}