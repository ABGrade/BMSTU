#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "measure.h"
#include "functions.h"
#include "io.h"

int *generate_array(int size);

void do_push_measure(int *arr, int size, int type, void (*push)(stack_t *, int *, int));
void do_pop_measure(int *arr, int size, int type, void (*push)(stack_t *, int *, int), void (*pop)(stack_t *, int));
void do_sort_measure(int *arr, int size, int type, void (*push)(stack_t *, int *, int), void (*sort)(stack_t *, stack_t *, stack_t *));


void delete_from_slist_without_comms(stack_t *stack, int count);
void delete_from_sarray_without_comms(stack_t *stack, int count);


void measure(void)
{
    int SIZES[DIF_SIZE] = {25000, 50000, 75000, 100000};
    int *arrs[DIF_SIZE] = {generate_array(SIZES[0]), generate_array(SIZES[1]), generate_array(SIZES[2]), generate_array(SIZES[3])};

    printf("\n\t Тест добавления в массив\n");
    for (size_t i = 0; i < DIF_SIZE; i++)
    {
        do_push_measure(arrs[i], SIZES[i], ARRAY, add_to_sarray);
    }

    printf("\n\t Тест удаления из массива\n");
    for (size_t i = 0; i < DIF_SIZE; i++)
    {
        do_pop_measure(arrs[i], SIZES[i], ARRAY, add_to_sarray, delete_from_sarray_without_comms);
    }

    printf("\n\t Тест добавления в список\n");
    for (size_t i = 0; i < DIF_SIZE; i++)
    {
        do_push_measure(arrs[i], SIZES[i], LIST, add_to_slist);
    }

    printf("\n\t Тест удаления из списка\n");
    for (size_t i = 0; i < DIF_SIZE; i++)
    {
        do_pop_measure(arrs[i], SIZES[i], LIST, add_to_slist, delete_from_slist_without_comms);
    }

    // printf("\n\t Тест сортировки массива\n");
    // for (size_t i = 0; i < DIF_SIZE; i++)
    // {
    //     do_sort_measure(arrs[i], SIZES[i], ARRAY, add_to_sarray, sort_arr);
    // }

    // printf("\n\t Тест сортировки списка\n");
    // for (size_t i = 0; i < DIF_SIZE; i++)
    // {
    //     do_sort_measure(arrs[i], SIZES[i], LIST, add_to_slist, sort_list);
    // }
}

void do_push_measure(int *arr, int size, int type, void (*push)(stack_t *, int *, int))
{
    double measure_time = 0;
    for (size_t i = 0; i < MEASURES; i++)
    {
        stack_t *stack = stack_init(type); 
        clock_t start = clock();
        push(stack, arr, size);
        clock_t end = clock();
        measure_time += end - start;
        free(stack);
    }
    printf("Для стека из %d элементов время: %lfс\n", size, measure_time / MEASURES / CLOCKS_PER_SEC);
}

void do_pop_measure(int *arr, int size, int type, void (*push)(stack_t *, int *, int), void (*pop)(stack_t *, int))
{
    double measure_time = 0;
    for (size_t i = 0; i < MEASURES; i++)
    {
        stack_t *stack = stack_init(type); 
        push(stack, arr, size);
        clock_t start = clock();
        pop(stack, size);
        clock_t end = clock();
        measure_time += end - start;
        free(stack);
    }
    printf("Для стека из %d элементов время: %lfс\n", size, measure_time / MEASURES / CLOCKS_PER_SEC);
}

void do_sort_measure(int *arr, int size, int type, void (*push)(stack_t *, int *, int), void (*sort)(stack_t *, stack_t *, stack_t *))
{
    double measure_time = 0;
    for (size_t i = 0; i < MEASURES_SORT; i++)
    {
        stack_t *stack1 = stack_init(type); 
        stack_t *stack2 = stack_init(type); 
        stack_t *sorted = stack_init(type);
        push(stack1, arr, size);

        clock_t start = clock();
        sort(stack1, stack2, sorted);
        clock_t end = clock();

        measure_time += end - start;

        free(stack1);
        free(stack2);
        free(sorted);
    }
    printf("Для стека из %d элементов время: %lfс\n", size, measure_time / MEASURES_SORT / CLOCKS_PER_SEC);
}

int *generate_array(int size)
{
    int *array = malloc(sizeof(int) * size);
    if (!array)
    {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % 100;
    }
    return array;
}

void delete_from_slist_without_comms(stack_t *stack, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (stack->stack_u.list_stack == NULL)
        {
            return;
        }
        pop_list(stack);
    }
}

void delete_from_sarray_without_comms(stack_t *stack, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (stack->stack_u.array_stack.sp < stack->stack_u.array_stack.values)
        {
            return;
        }
        pop_arr(stack);
    }
}