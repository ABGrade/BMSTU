#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>

#include "types.h"
#include "tree_functions.h"
#include "tree_complex_functions.h"
#include "hash_functions.h"
#include "io.h"
#include "hash_types.h"
#include "measure.h"

void refresh_hash_sizes()
{
    TABLE_SIZE_CHAIN = 23;
    TABLE_SIZE_OPEN = 23;
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
        array[i] = rand();
    }
    return array;
}

char *deanon_letter(int type)
{
    switch (type)
    {
        case SKEWED:
            return "вырожденное дерево";
        
        case BALANCED:
            return "сбалансированное дерево";

        case BINARY:
            return "бинарное дерево";
        
        case CHAIN:
            return "цепочная хэш-таблица";
        
        case OPEN:
            return "открытая хэш-таблица";

        default:
            return "hz";
    }
}

// Функция для создания вырожденного дерева
tree_t *create_skewed_tree(int size) 
{
    tree_t *tree = init_tree();
    if (!tree) return NULL;
    for (int i = 1; i <= size; i++) 
    {
        tree_insert(tree, i);
    }
    return tree;
}

// Функция для создания сбалансированного дерева
tree_t *create_balanced_tree(int size, int *values) 
{
    tree_t *tree = init_tree();
    if (!tree) return NULL;
    for (int i = 0; i < size; i++)
        tree_insert(tree, values[i]);
    tree = balance_tree(tree);
    return tree;
}

// Функция для создания случайного бинарного дерева
tree_t *create_random_tree(int size, int *values)
{
    tree_t *tree = init_tree();
    if (!tree) return NULL;
    for (int i = 0; i < size; i++)
    {
        tree_insert(tree, values[i]);
    }
    return tree;
}

// Функция для измерения времени выполнения операции с использованием clock_gettime
double measure_time(struct timespec start, struct timespec end) 
{
    return (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
}

// Функция для измерения времени добавления
void measure_add(int size, char type, int *values) 
{
    double total_time = 0;
    size_t memory_size = 0;

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        struct timespec start, end;
        tree_t *tree = NULL;
        hash_chain chain_hash;
        hash_open open_hash;
    
        switch (type)
        {
            case SKEWED: // Вырожденное
                tree = init_tree(tree);
                clock_gettime(CLOCK_MONOTONIC, &start);

                for (int j = 0; j < size; j++)
                    tree_insert(tree, j);

                clock_gettime(CLOCK_MONOTONIC, &end);
                total_time += measure_time(start, end);
                break;
            case BALANCED: // Сбалансированное
                tree = init_tree(tree);
                clock_gettime(CLOCK_MONOTONIC, &start);

                for (int j = 0; j < size; j++)
                    tree_balance_insert(tree, values[j]);

                clock_gettime(CLOCK_MONOTONIC, &end);
                total_time += measure_time(start, end);
                break;
             case BINARY: // Бинарное
                tree = init_tree(tree);
                clock_gettime(CLOCK_MONOTONIC, &start);

                for (int j = 0; j < size; j++)
                    tree_insert(tree, values[j]);

                clock_gettime(CLOCK_MONOTONIC, &end);
                total_time += measure_time(start, end);
                break;
            case CHAIN: // Цепочки
                init_chain(&chain_hash, TABLE_SIZE_CHAIN);
                clock_gettime(CLOCK_MONOTONIC, &start);

                for (int j = 0; j < size; j++)
                {
                    int count = insert_chain(&chain_hash, values[j], 0);
                    if (count > HASH_PERFOMANCE_LIMIT_CHAIN)
                    {
                        change_table_size(1);
                        resize_chain(&chain_hash, TABLE_SIZE_CHAIN);
                    }
                }

                clock_gettime(CLOCK_MONOTONIC, &end);
                total_time += measure_time(start, end);
                if (i == 0)
                {
                    memory_size = sizeof(node_t*) * chain_hash.size;
                    for (unsigned int k = 0; k < chain_hash.size; k++)
                    {
                        if (chain_hash.table[k] != NULL)
                        {
                            node_t* current = chain_hash.table[k];
                            while (current != NULL)
                            {
                                memory_size += sizeof(node_t);
                                current = current->next;
                            }
                         }
                    }
                }
                clear_chain(&chain_hash);
                break;
             case OPEN: // Открытая
                init_open(&open_hash, TABLE_SIZE_OPEN);
                clock_gettime(CLOCK_MONOTONIC, &start);

                for (int j = 0; j < size; j++)
                {
                    int count = insert_open(&open_hash, values[j], 0);
                    if (count > HASH_PERFOMANCE_LIMIT_OPEN)
                    {
                        change_table_size(0);
                        resize_open(&open_hash, TABLE_SIZE_OPEN);
                    }
                }
                    
                clock_gettime(CLOCK_MONOTONIC, &end);
                total_time += measure_time(start, end);
                if (i == 0)
                {
                    memory_size = sizeof(int*) * open_hash.size;
                     for (unsigned int k = 0; k < open_hash.size; k++)
                     {
                        if (open_hash.table[k] != NULL)
                         {
                            memory_size += sizeof(int);
                        }
                    }
                }
                clear_open(&open_hash);
                break;
            default:
                printf("Неверный тип структуры данных\n");
                return;
            }
            if (tree && i == 0)
            {
                travel(tree, 1, 0);
                memory_size = calculate_size(tree->root) * sizeof(vertex_t);
            }
            free_tree(tree);            
        }
    printf("Среднее время добавления для %s: %.2f нс\n", deanon_letter(type), total_time / NUM_ITERATIONS);
    printf("Занимаемая память: %zu байт\n", memory_size);
}

// Функция для измерения времени удаления
void measure_delete(int size, char type, int *values) 
{
    double total_time = 0;

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        int del_element = values[rand() % size];
        struct timespec start, end;
        tree_t *tree = NULL;
        hash_chain chain_hash;
        hash_open open_hash;

        switch (type)
        {
            case SKEWED:
                tree = create_skewed_tree(size);
                del_element = rand() % size + 1;
                clock_gettime(CLOCK_MONOTONIC, &start);
                node_delete(tree->root, del_element);
                clock_gettime(CLOCK_MONOTONIC, &end);
                total_time += measure_time(start, end);
                free_tree(tree);
                break;
            case BALANCED:
                tree = create_balanced_tree(size, values);
                clock_gettime(CLOCK_MONOTONIC, &start);
                node_delete(tree->root, del_element);
                clock_gettime(CLOCK_MONOTONIC, &end);
                total_time += measure_time(start, end);
                free_tree(tree);
                break;
            case BINARY:
                tree = create_random_tree(size, values);
                clock_gettime(CLOCK_MONOTONIC, &start);
                node_delete(tree->root, del_element);
                clock_gettime(CLOCK_MONOTONIC, &end);
                total_time += measure_time(start, end);
                free_tree(tree);
                break;
            case CHAIN:
                init_chain(&chain_hash, TABLE_SIZE_CHAIN);
                for (int j = 0; j < size; j++)
                    insert_chain(&chain_hash, values[j], 0);
                clock_gettime(CLOCK_MONOTONIC, &start);
                delete_chain(&chain_hash, del_element);
                clock_gettime(CLOCK_MONOTONIC, &end);
                total_time += measure_time(start, end);
                clear_chain(&chain_hash);
                break;
            case OPEN:
                init_open(&open_hash, TABLE_SIZE_OPEN);
                for (int j = 0; j < size; j++)
                        insert_open(&open_hash, values[j], 0);
                clock_gettime(CLOCK_MONOTONIC, &start);
                delete_open(&open_hash, del_element, 0);
                clock_gettime(CLOCK_MONOTONIC, &end);
                total_time += measure_time(start, end);
                clear_open(&open_hash);
                break;
            default:
                printf("Неверный тип структуры данных\n");
                return;
        }
    }
    printf("Среднее время удаления для %s: %.2f нс\n", deanon_letter(type), total_time / NUM_ITERATIONS);
}

// Функция для измерения времени поиска
void measure_search(int size, char type, int *values) 
{
    double total_time = 0;

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        struct timespec start, end;
        tree_t *tree = NULL;
        hash_chain chain_hash;
        hash_open open_hash;

        switch (type)
        {
        case SKEWED:
            tree = create_skewed_tree(size);
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int i = 0; i < size; i++)
            {
                find(tree->root, values[i], 0, 0);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time += measure_time(start, end);
            free_tree(tree);
            break;
        case BALANCED:
            tree = create_balanced_tree(size, values);
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int i = 0; i < size; i++)
            {
                find(tree->root, values[i], 0, 0);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);;
            total_time += measure_time(start, end);
            free_tree(tree);
            break;
         case BINARY:
            tree = create_random_tree(size, values);
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int i = 0; i < size; i++)
            {   
                find(tree->root, values[i], 0, 0);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time += measure_time(start, end);
            free_tree(tree);
            break;
        case CHAIN:
            init_chain(&chain_hash, TABLE_SIZE_CHAIN);
            for (int j = 0; j < size; j++)
                insert_chain(&chain_hash, values[j], 0);
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int i = 0; i < size; i++)
            {
                search_chain(&chain_hash, values[i]);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time += measure_time(start, end);
            clear_chain(&chain_hash);
            break;
        case OPEN:
            init_open(&open_hash, TABLE_SIZE_OPEN);
            for (int j = 0; j < size; j++)
                    insert_open(&open_hash, values[j], 0);
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int i = 0; i < size; i++)
            {
                search_open(&open_hash, values[i]);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time += measure_time(start, end);
            clear_open(&open_hash);
           break;
        default:
            printf("Неверный тип структуры данных\n");
             return;
        }
    }
    printf("Среднее время поиска всех элементов для %s: %.2f нс\n", deanon_letter(type), total_time / NUM_ITERATIONS);
}


void measure() 
{
    int sizes[] = {500, 750, 1000, 1250, 1500};
    int types[] = {SKEWED, BALANCED, BINARY, CHAIN, OPEN};
    
    printf("\n--- Измерение времени для различных структур данных ---\n");

    for (int i = 0; i < ARRAY_SIZE(sizes); i++) 
    {
        int size = sizes[i];
        printf("\nРазмер: %d\n", size);
    
        int *values = generate_array(size);
        for (int j = 0; j < ARRAY_SIZE(types); j++)
        {
            int type = types[j];
            printf("\nПроверка %s\n", deanon_letter(type));
            measure_add(size, type, values);
            measure_delete(size, type, values);
            measure_search(size, type, values);
            if (type == OPEN)
            {
                show_hash_status();
                refresh_hash_sizes();
            }
        }
        free(values);
    }
}
