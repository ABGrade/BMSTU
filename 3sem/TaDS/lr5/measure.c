#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "measure.h"
#include "types.h"
#include "arr_func.h"
#include "list_func.h"
#include "modeling.h"

double do_measure_arr_push(arr_queue_t *queue, unsigned addition_size);
double do_measure_arr_pop(arr_queue_t *queue, unsigned del_size);

double do_measure_list_push(list_queue_t *queue, unsigned addition_size);
double do_measure_list_pop(list_queue_t *queue, unsigned del_size);

void do_measure_array_modeling();
void do_measure_list_modeling();



void measure(void)
{
    // unsigned SIZES[] = {30000, 50000, 70000, 90000, 100000};
    // arr_queue_t aqueue[DIF_SIZES] = {0};
    // list_queue_t lqueue[DIF_SIZES] = {0};

    // printf("\n\t Тест добавления в массив\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     double avg_time = do_measure_arr_push(&aqueue[i], SIZES[i]);
    //     printf("%-7u элементов добавлены за %lf с\n", SIZES[i], avg_time);
    // }

    // printf("\n\t Тест удаления из массива\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     double avg_time = do_measure_arr_pop(&aqueue[i], SIZES[i]);
    //     printf("%-7u элементов удалены за %lf с\n", SIZES[i], avg_time);
    // }

    // printf("\n\t Тест добавления в список\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     double avg_time = do_measure_list_push(&lqueue[i], SIZES[i]);
    //     printf("%-7u элементов добавлены за %lf с\n", SIZES[i], avg_time);
    //     // clear_list(&lqueue[i]); // Очистка памяти
    // }

    // printf("\n\t Тест удаления из списка\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     double avg_time = do_measure_list_pop(&lqueue[i], SIZES[i]);
    //     printf("%-7u элементов удалены за %lf с\n", SIZES[i], avg_time);
    //     // clear_list(&lqueue[i]); // Очистка памяти
    // }

    do_measure_array_modeling();
    do_measure_list_modeling();
}

double do_measure_arr_push(arr_queue_t *queue, unsigned addition_size)
{
    double total_time = 0.0;
    request_t request = {.type = FIRST, .coming = 0, .processing = 0};
    for (int repeat = 0; repeat < REPEAT_COUNT; repeat++)
    {
        clock_t start = clock();
        for (unsigned i = 0; i < addition_size; i++)
        {
            arr_push(queue, request);
        }
        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    return total_time / REPEAT_COUNT;
}


double do_measure_arr_pop(arr_queue_t *queue, unsigned del_size)
{
    double total_time = 0.0;
    for (int repeat = 0; repeat < REPEAT_COUNT; repeat++)
    {
        clock_t start = clock();
        for (unsigned i = 0; i < del_size; i++)
        {
            arr_pop(queue);
        }
        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    return total_time / REPEAT_COUNT;
}

double do_measure_list_push(list_queue_t *queue, unsigned addition_size)
{
    double total_time = 0.0;
    request_t request = {.type = FIRST, .coming = 0, .processing = 0};
    for (int repeat = 0; repeat < REPEAT_COUNT; repeat++)
    {
        clock_t start = clock();
        for (unsigned i = 0; i < addition_size; i++)
        {
            int rc = list_push(queue, request);
            if (rc)
            {
                printf("Ошибка памяти\n");
            }
            
        }
        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    return total_time / REPEAT_COUNT;
}

double do_measure_list_pop(list_queue_t *queue, unsigned del_size)
{
    double total_time = 0.0;
    for (int repeat = 0; repeat < REPEAT_COUNT; repeat++)
    {
        clock_t start = clock();
        for (unsigned i = 0; i < del_size; i++)
        {
            list_pop(queue);
        }
        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    return total_time / REPEAT_COUNT;
}

void do_measure_array_modeling()
{
    printf("\n\t Тест моделирования очечереди массивом\n");
    double total_time = 0.0;
    clock_t start = clock();
    for (int repeat = 0; repeat < REPEAT_COUNT; repeat++)
    {
        simulate_service_system_array(0);
    }
    clock_t end = clock();
    total_time += (double)(end - start) / CLOCKS_PER_SEC;
    printf("Время моделирования: %lf с\n", total_time / REPEAT_COUNT);
}

void do_measure_list_modeling()
{
    printf("\n\t Тест моделирования очереди списком\n");
    double total_time = 0.0;
    clock_t start = clock();
    for (int repeat = 0; repeat < REPEAT_COUNT; repeat++)
    {
        simulate_service_system_list(0);
    }
    clock_t end = clock();
    total_time += (double)(end - start) / CLOCKS_PER_SEC;
    printf("Время моделирования: %lf с\n", total_time / REPEAT_COUNT);
}