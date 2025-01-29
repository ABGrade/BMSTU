// Смоделировать операцию деления целого числа длиной до
// 40 десятичных цифр на действительное число в форме
// ±m.n Е K, где суммарная длина мантиссы (m+n) - до 40
// значащих цифр, а величина порядка K - до 5 цифр.
// Результат выдать в форме ±0.m1 Е ±K1, где m1 - до 40
// значащих цифр, а K1 - до 5 цифр.

#include <stdio.h>

#include "functions.h"
#include "calculations.h"
#include "errors.h"
#include "io.h"

int main(void)
{
    char buffer[BUFFER_SIZE]; // Буфер для хранения ввода пользователя
    bigint_t big_num1 = bigint_initialize();

    printf("                |---|----|----|----|----|----|----|----|\n"); // Вывод разделителя
    printf("Enter bigint 1: ");

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        show_error(FGETS_STRING_ERROR);
        return FGETS_STRING_ERROR; // Возврат ошибки ввода строки
    }
    
    int rc = read_bigint(buffer, &big_num1); // Чтение большого целого числа из буфера
    if (rc)
    {
        show_error(rc);
        return rc; // Возврат кода ошибки при чтении
    }

    bigint_t big_num2 = bigint_initialize(); // Инициализация второго большого целого числа
    printf("Enter bigflt 2: ");

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        show_error(FGETS_STRING_ERROR);
        return FGETS_STRING_ERROR; // Возврат ошибки ввода строки
    }

    rc = read_exponential(buffer, &big_num2); // Чтение экспоненциальной формы числа
    if (rc)
    {
        show_error(rc);
        return rc; // Возврат кода ошибки при чтении
    }

    if (big_num1.mantissa[0] == 0)
    {
        printf("0\n"); // Вывод нуля, если первый элемент мантиссы равен нулю
        return 0;
    }
    if (big_num2.mantissa[0] == 0)
    {
        show_error(ZERO_ERROR);
        return ZERO_ERROR; // Возврат ошибки нулевого делителя
    }
    
    bigint_t result = division(big_num1, big_num2); // Выполнение деления
    rc = print_bigint(result); // Вторичный вывод результата
    if (rc)
    {
        show_error(rc);
        return rc;
    }
    printf("  |---|----|----|----|----|----|----|----|\n"); // Вывод разделителя
    return 0;
}