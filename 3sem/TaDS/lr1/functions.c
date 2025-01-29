#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "functions.h"
#include "calculations.h"

// Инициализирует структуру bigint_t с нулевыми значениями
bigint_t bigint_initialize(void)
{
    bigint_t big_num;
    memset(big_num.mantissa, 0, sizeof(big_num.mantissa));
    big_num.size = 0;
    big_num.order = 0;
    big_num.sign = 1;
    return big_num;
}

int string_to_bigint(char *string, bigint_t *big_num)
{
    string[strcspn(string, "\n")] = '\0'; // Удаляем символ новой строки
    (*big_num).size = strlen(string);
    for (size_t i = 0; i < big_num->size; i++)
    {
        big_num->mantissa[i] = to_digit(string[i]); // Преобразование символа в цифру
    }
    if ((*big_num).size > MAX_NUMS)
    {
        return 1; // Возврат ошибки, если число слишком велико
    }
    return 0;
}

void show_bigint(bigint_t big_num)
{
    printf("Размер: %zu\n", big_num.size);
    printf("Порядок: %d\n", big_num.order);
    printf("Знак: %d\n", big_num.sign);
    for (size_t i = 0; i < big_num.size; i++)
    {
        printf("%d", big_num.mantissa[i]);
    }
    printf("\n");
    printf("-----------\n");
}

// Преобразует символ в цифру
char to_digit(char symb)
{
    return symb - '0';
}

// Удаляет ведущие нули из bigint_t
void del_leading_zeros(bigint_t *big_num)
{
    size_t i = 0;
    for (; i < big_num->size && big_num->mantissa[i] == 0; i++)
    {
        ; // Пустое тело цикла для оптимизации
    }
    if (i == 0)
    {
        return; // Если все цифры нули, ничего не делать
    }
    else if (i == big_num->size)
    {
        big_num->size = 1; // Уменьшаем размер до 1, если все нули
        return;
    }
    for (size_t j = 0, k = i; k < big_num->size; k++, j++)
    {
        big_num->mantissa[j] = big_num->mantissa[k];
    }
    big_num->size -= i;
}

int insert_first(bigint_t *big_num, int num)
{
    int push_amount = calc_digits(num);
    if (big_num->size + push_amount > MAX_NUMS + 1)
    {
        return 1; // Возврат ошибки, если число слишком велико
    }
    for (int i = big_num->size - 1; i >= 0; i--)
    {
        big_num->mantissa[i + push_amount] = big_num->mantissa[i];
    }
    for (int i = push_amount - 1; i >= 0; i--)
    {
        big_num->mantissa[i] = num % 10;
        num /= 10;
    }
    big_num->size += push_amount;
    return 0;
}

int insert(bigint_t *big_num, int num)
{
    int digits = calc_digits(num);
    if (big_num->size + digits > MAX_NUMS + 1)
    {
        return 1; // Возврат ошибки, если число слишком велико
    }
    for (int i = big_num->size + digits - 1; num > 0; i--)
    {
        big_num->mantissa[i] = num % 10;
        num /= 10;
    }
    big_num->size += digits;
    return 0;
}

void big_round(bigint_t *big_num)
{
    if (big_num->mantissa[big_num->size - 1] >= 5 && big_num->size == 41)
    {
        char offer = 1;
        big_num->size--;
        big_num->order++;
        
        for (int i = big_num->size - 1; i >= 0 && offer; i--)
        {
            big_num->mantissa[i]++;
            if (big_num->mantissa[i] == 10)
            {
                big_num->mantissa[i] = 0;
                if (i == 0)
                {
                    insert_first(big_num, 1);  // Вставляем 1 в начало числа
                }
                continue;   
            }
            offer = 0;
        }
    }
}