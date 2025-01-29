#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "measure.h"
#include "types.h"
#include "io.h"
#include "tree_functions.h"
#include "tree_complex_functions.h"

void find_clear(vertex_t *root, int value);
void sort_clear(vertex_t *root);

void do_insert_measure(tree_t *tree, int values[], int size);
void do_find_measure(vertex_t *root, int values[], int size);
void do_sort_measure(vertex_t *root, int size);
void do_delete_measure(int values[], int size, char is_reversed);
void do_write_measure(int values[], int size);

void measure()
{
    int SIZES[DIF_SIZES] = {1000, 2500, 5000, 7500, 10000};
    int values_wight[DIF_SIZES][10000] = {0};
    int values_height[DIF_SIZES][10000] = {0};
    tree_t trees_wight[DIF_SIZES] = {0};
    tree_t trees_height[DIF_SIZES] = {0};

    printf("Инициализируем...\n");
    for (int i = 0; i < DIF_SIZES; i++)
    {
        for (int j = 0; j < SIZES[i]; j++)
        {
            values_wight[i][j] = rand() % 1000000;
        }
    }

    for (int i = 0; i < DIF_SIZES; i++)
    {
        for (int j = 0; j < SIZES[i]; j++)
        {
            values_height[i][j] = j;
        }
    }

    for (size_t i = 0; i < DIF_SIZES; i++)
    {
        tree_t *tree_w = init_tree();
        tree_t *tree_h = init_tree();
        if (!tree_w || !tree_h)
        {
            printf("Ошибка выделения памяти!\n");
            return;
        }
        trees_wight[i] = *tree_w;
        trees_height[i] = *tree_h;
    }

    // printf("\n\t Тест вставки\n");
    // printf("\nШирокое дерево\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     do_insert_measure(&trees_wight[i], values_wight[i], SIZES[i]);
    //     travel(&trees_wight[i], 1, 0);
    // }
    // printf("\nДлинное дерево\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     do_insert_measure(&trees_height[i], values_height[i], SIZES[i]);
    //     travel(&trees_height[i], 1, 0);
    // }

    // printf("\n\t Тест поиска\n");
    // printf("\nШирокое дерево\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     do_find_measure(trees_wight[i].root, values_wight[i], SIZES[i]);
    // }
    // printf("\nДлинное дерево\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     do_find_measure(trees_height[i].root, values_height[i], SIZES[i]);
    // }

    // printf("\n\t Тест сортировки\n");
    // printf("\nШирокое дерево\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     do_sort_measure(trees_wight[i].root, SIZES[i]);
    // }
    // printf("\nДлинное дерево\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     do_sort_measure(trees_height[i].root, SIZES[i]);
    // }

    // printf("\n\t Тест удаления\n");
    // printf("\nШирокое дерево\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     do_delete_measure(values_wight[i], SIZES[i], 0);
    // }
    // printf("\nДлинное дерево\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     do_delete_measure(values_height[i], SIZES[i], 0);
    // }
    // printf("\nДлинное дерево в обратном порядке\n");
    // for (size_t i = 0; i < DIF_SIZES; i++)
    // {
    //     do_delete_measure(values_height[i], SIZES[i], 1);
    // }

    printf("\n\t Тест скорости записи в файл\n");
    for (size_t i = 0; i < DIF_SIZES; i++)
    {
        do_write_measure(values_wight[i], SIZES[i]);
    }

}

void do_insert_measure(tree_t *tree, int values[], int size)
{
    double measure_time = 0.0;
    for (size_t i = 0; i < MEASURE_REPEATS; i++)
    {
        clock_t start = clock();
        for (size_t j = 0; j < size; j++)
        {
            tree_insert(tree, values[j]);
        }
        clock_t end = clock();
        measure_time += (double)end - start;
    }
    measure_time /= MEASURE_REPEATS;
    printf("Добавление %d элементов прошло за %lf с\n", size, measure_time / CLOCKS_PER_SEC);
}

void do_find_measure(vertex_t *root, int values[], int size)
{
    double total_time = 0.0;
    for (size_t i = 0; i < size; i++)
    {
        clock_t start = clock();
        for (size_t j = 0; j < MEASURE_REPEATS; j++)
        {
            find_clear(root, values[i]);
        }
        clock_t end = clock();
        double measure_time = (double)end - start;
        measure_time /= MEASURE_REPEATS;
        total_time += measure_time;
    }
    printf("Среднее время поиска одного элемента (всего %d элементов) составило %lf с\n", size, total_time / size);
}

void do_sort_measure(vertex_t *root, int size)
{
    double measure_time = 0.0;
    clock_t start = clock();
    for (size_t i = 0; i < MEASURE_REPEATS; i++)
    {
        sort_clear(root);
    }
    clock_t end = clock();
    measure_time = (double)end - start;
    measure_time /= MEASURE_REPEATS;
    printf("Соритровка %d элементов прошла за %lf с\n", size, measure_time / CLOCKS_PER_SEC);
}

void do_delete_measure(int values[], int size, char is_reversed)
{
    double measure_time = 0.0;
    for (size_t i = 0; i < MEASURE_REPEATS && is_reversed == 0; i++)
    {
        tree_t *copy = init_tree();
        if (!copy)
        {
            printf("Ошибка выделения памяти!\n");
            return;
        }
        for (size_t j = 0; j < size; j++)
        {
            tree_insert(copy, values[j]);
        }

        clock_t start = clock();
        for (size_t j = 0; j < size; j++)
        {
            node_delete(copy->root, values[i]);
        }
        clock_t end = clock();
        measure_time += (double)end - start;
    }
    for (size_t i = 0; i < MEASURE_REPEATS && is_reversed; i++)
    {
        tree_t *copy = init_tree();
        if (!copy)
        {
            printf("Ошибка выделения памяти!\n");
            return;
        }
        for (size_t j = 0; j < size; j++)
        {
            tree_insert(copy, values[j]);
        }

        clock_t start = clock();
        for (int j = size - 1; j >= 0; j--)
        {
            node_delete(copy->root, values[j]);
        }
        clock_t end = clock();
        measure_time += (double)end - start;
    }
    measure_time /= MEASURE_REPEATS;
    printf("Удаление %d элементов прошло за %lf с\n", size, measure_time / CLOCKS_PER_SEC);
}

void do_write_measure(int values[], int size)
{
    double measure_time = 0.0;
    for (size_t i = 0; i < MEASURE_REPEATS; i++)
    {
        clock_t start = clock();
        for (size_t j = 0; j < size; j++)
        {
            write_into_file(values[j], "test.txt");
        }
        clock_t end = clock();
        measure_time += (double)end - start;

        FILE *file = fopen("test.txt", "w");
        fclose(file);
    }
    measure_time /= MEASURE_REPEATS;
    printf("Добавление %d элементов в файл прошло за %lf с\n", size, measure_time / CLOCKS_PER_SEC);
}

void find_clear(vertex_t *root, int value)
{
    if (root == NULL)
    {
        return;
    }
    if (root->value == value)
    {
        return;
    }
    else if (value < root->value)
    {
        find_clear(root->left, value);
    }
    else
    {
        find_clear(root->right, value);
    }
}

void sort_clear(vertex_t *root)
{
    if (root == NULL)
        return;
    sort_clear(root->left);
    // printf("%d ", root->value);
    sort_clear(root->right);
}