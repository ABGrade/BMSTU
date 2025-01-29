#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "calculations.h"

// minuend - Уменьшаемое, subtrahend - вычитаемое. Ожидается, что minuend >= subtrahend
bigint_t subtract(bigint_t minuend, bigint_t subtrahend)
{
    int m_i = minuend.size - 1;
    int s_i = subtrahend.size - 1;
    char offer = 0;
    while (s_i >= 0)
    {
        if (offer)
        {
            minuend.mantissa[m_i] -= 1;
            offer = 0;
        }
        if (minuend.mantissa[m_i] < subtrahend.mantissa[s_i])
        {
            offer = 1;
            minuend.mantissa[m_i] += 10;
        }
        minuend.mantissa[m_i] -= subtrahend.mantissa[s_i];
        m_i--;
        s_i--;
    }
    if (offer)
    {
        minuend.mantissa[m_i] -= 1;
    }
    del_leading_zeros(&minuend);
    return minuend;
}

// dividend - делимое, divider - делитель
bigint_t division(bigint_t dividend, bigint_t divider)
{
    bigint_t res = bigint_initialize();
    res.sign = dividend.sign * divider.sign;
    res.order = divider.order * -1;

    if (compare_bigint(dividend, divider) == 0) 
    {
        res.size = 1;
        res.mantissa[0] = 1;
        return res;
    }

    size_t add_divider_size = dividend.size - divider.size;
    int rc;

    while ((rc = compare_bigint(dividend, divider)) > 0)
    {
        bigint_t temp = divider;
        if (add_divider_size > 0)
        {
            add_divider_size--;
        }
        while (temp.size + add_divider_size > dividend.size && add_divider_size > 0)
        {
            add_divider_size--;
            insert(&res, 0); 
        }

        temp.size += add_divider_size;

        int q = (dividend.size == temp.size) ?
                (dividend.mantissa[0] * 10 + dividend.mantissa[1]) /
                (divider.mantissa[0] * 10 + divider.mantissa[1]) :
                (dividend.mantissa[0] * 100 + dividend.mantissa[1] * 10 + dividend.mantissa[2]) /
                (divider.mantissa[0] * 10 + divider.mantissa[1]);

        temp = multiply_by_short(temp, q);
        if (compare_bigint(dividend, temp) < 0)
        {
            q -= 1;
            temp = divider;
            temp.size += add_divider_size;
            temp = multiply_by_short(temp, q);
        }

        dividend = subtract(dividend, temp);
        insert(&res, q);
    }

    if (rc == 0)
    {
        res.size += add_divider_size;
        res.mantissa[res.size - 1] = 1;
        return res;
    }

    while (res.size < MAX_NUMS + 1)
    {
        dividend.size++;
        dividend.mantissa[dividend.size - 1] = 0;
        res.order--;

        while (compare_bigint(dividend, divider) < 0)
        {
            dividend.size++;
            dividend.mantissa[dividend.size - 1] = 0;
            res.order--;
            if (res.size != 0)
            {
                insert(&res, 0);
                if (res.size == MAX_NUMS + 1)
                {
                    res.order++;
                    return res;
                }  
            }     
        }  

        int q = (dividend.size == divider.size) ?
                (dividend.mantissa[0] * 10 + dividend.mantissa[1]) /
                (divider.mantissa[0] * 10 + divider.mantissa[1]) :
                (dividend.mantissa[0] * 100 + dividend.mantissa[1] * 10 + dividend.mantissa[2]) /
                (divider.mantissa[0] * 10 + divider.mantissa[1]);

        bigint_t temp = multiply_by_short(divider, q);
        if (compare_bigint(dividend, temp) < 0)
        {
            q--;
            temp = multiply_by_short(divider, q);
        }        
            
        dividend = subtract(dividend, temp);
        insert(&res, q);
    }    
    return res;
}

bigint_t multiply_by_short(bigint_t big_num, int num)
{
    int carry = 0;
    for (int i = big_num.size - 1; i >= 0; i--)
    {
        big_num.mantissa[i] *= num;
        big_num.mantissa[i] += carry;
        carry = big_num.mantissa[i] / 10;
        big_num.mantissa[i] %= 10;
    }
    if (carry)
    {
        insert_first(&big_num, carry);
    }
    return big_num;
}

int calc_digits(int num)
{
    int digits = 0;
    do
    {
        num /= 10;
        digits++;
    } while (num != 0);
    return digits;
}

int compare_bigint(bigint_t big_num1, bigint_t big_num2)
{
    if (big_num1.size > big_num2.size)
    {
        return 1;
    }
    else if (big_num1.size < big_num2.size)
    {
        return -1;
    }
    else
    {
        for (size_t i = 0; i < big_num1.size; i++)
        {
            if (big_num1.mantissa[i] > big_num2.mantissa[i])
            {
                return 1;
            }
            if (big_num1.mantissa[i] < big_num2.mantissa[i])
            {
                return -1;
            }
        }
        return 0;
    }
}