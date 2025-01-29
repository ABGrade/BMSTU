#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sorts_measures.h"
#include "functions.h"
#include "io.h"
#include "const_struct.h"


void bubble_sort_full(sub_t subs[], key_t keys[], size_t size)
{
    bubble_sort_items(subs, size);
    bubble_sort_keys(keys, size);
    for (size_t i = 0; i < size; i++)
    {
        subs[i].id = i;
        keys[i].id = i;
    }
}

void bubble_sort_items(sub_t subs[], size_t size)
{
    if (size == 0)
    {
        printf("Пустая таблица\n");
        return;
    }
    for (size_t i = 0; i < size - 1; ++i)
    {
        for (size_t j = 0; j < size - 1 - i; ++j)
        {
            long long number1 = (subs[j].kind_sub == FRIEND) ? subs[j].status.friend.user.number : subs[j].status.college.user.number;
            long long number2 = (subs[j + 1].kind_sub == FRIEND) ? subs[j + 1].status.friend.user.number : subs[j + 1].status.college.user.number;
            
            if (number1 > number2)
            {
                sub_t temp = subs[j];
                subs[j] = subs[j + 1];
                subs[j + 1] = temp;
            }
        }
    }
}

void quick_sort_full(sub_t subs[], key_t keys[], size_t size)
{
    qsort(subs, size, sizeof(sub_t), compare_sub);
    qsort(keys, size, sizeof(key_t), compare_key);
}

void quick_sort_items(sub_t subs[], size_t size)
{
    qsort(subs, size, sizeof(sub_t), compare_sub);
}

void bubble_sort_keys(key_t keys[], size_t size) 
{
    if (size == 0)
    {
        printf("Пустая таблица\n");
        return;
    }
    for (size_t i = 0; i < size - 1; i++) 
    {
        for (size_t j = 0; j < size - i - 1; j++) 
        {
            if (keys[j].number > keys[j + 1].number) 
            {
                key_t temp = keys[j];
                keys[j] = keys[j + 1];
                keys[j + 1] = temp;
            }
        }
    }
}

void quick_sort_keys(key_t keys[], size_t size)
{
    qsort(keys, size, sizeof(key_t), compare_key);
}

double measure_sort_time_items(void (*sort_func)(sub_t[], size_t), sub_t items[], size_t size) 
{
    sub_t copy[MAX_SUB];
    double measure_time = 0;
    for (size_t k = 0; k < MEASUERE_REPEATS; k++)
    {
        for (size_t i = 0; i < size; i++)
        {
            copy[i] = items[i];
        }
        clock_t start = clock();
        sort_func(copy, size);
        clock_t end = clock();
        measure_time += end - start;
    }
    return measure_time / CLOCKS_PER_SEC / MEASUERE_REPEATS;
}

double measure_sort_time_keys(void (*sort_func)(key_t[], size_t), key_t keys[], size_t size) 
{
    double measure_time = 0;
    key_t copy[MAX_SUB];
    for (size_t k = 0; k < MEASUERE_REPEATS; k++)
    {
        for (size_t i = 0; i < size; i++)
        {
            copy[i] = keys[i];
        }
        clock_t start = clock();
        sort_func(copy, size);
        clock_t end = clock();
        measure_time += end - start;
    }
    return measure_time / CLOCKS_PER_SEC / MEASUERE_REPEATS;
}

void compare_sorts(sub_t subs[], key_t keys[], size_t size)
{
    if (size == 0)
    {
        printf("Пустая таблица\n");
        return;
    }

    double qsi_time = measure_sort_time_items(quick_sort_items, subs, size);
    double qsk_time = measure_sort_time_keys(quick_sort_keys, keys, size);
    double bsi_time = measure_sort_time_items(bubble_sort_items, subs, size);
    double bsk_time = measure_sort_time_keys(bubble_sort_keys, keys, size);

    printf("Быстрая по таблице: %lf\nБыстрая по ключам: %lf\nПузырьковая по таблице: %lf\nПузырьковая по ключам: %lf\n",
            qsi_time, qsk_time, bsi_time, bsk_time);

    printf("Разница в скорости сортировок (быстрая по таблице к ,быстрой по ключам): %.2lf %%\n", qsi_time/qsk_time * 100);
    printf("Разница в скорости сортировок (быстрая по таблице к пузырьковой по таблице): %.2lf %%\n", qsi_time/bsi_time * 100);
    printf("Разница в скорости сортировок (быстрая по ключам к пузырьковой по ключам): %.2lf %%\n", qsk_time/bsk_time * 100);
    printf("Разница в скорости сортировок (пузырьковая по таблице к пузырьковой по ключам): %.2lf %%\n", bsi_time/bsk_time * 100);
}