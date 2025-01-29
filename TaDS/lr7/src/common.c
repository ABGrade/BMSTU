#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "types.h"
#include "hash_functions.h"
#include "tree_functions.h"
#include "tree_complex_functions.h"
#include "io.h"
#include "hash_types.h"

void read_all(tree_t *tree, hash_chain *chain, hash_open *open, char *filename) 
{
    int value;
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        printf("Файл не может быть открыт\n");
        return;
    }

    while (fscanf(file, "%d", &value) == 1) 
    {
        tree_balance_insert(tree, value);
        int step_chain = insert_chain(chain, value, 1);
        int step_open = insert_open(open, value, 1);

        if (step_chain > HASH_PERFOMANCE_LIMIT_CHAIN)
        {
            change_table_size(1);
            resize_chain(chain, TABLE_SIZE_CHAIN);
        }

        if (step_open > HASH_PERFOMANCE_LIMIT_OPEN)
        {
            change_table_size(0);
            resize_open(open, TABLE_SIZE_OPEN);
        }
    }

    fclose(file);
    printf("Успешно прочитано.\n");
}

void add_all(tree_t *tree, hash_chain *chain, hash_open *open, char *filename)
{
    int value;
    printf("Введите значение узла: ");
    value = read_int();
    tree_balance_insert(tree, value);
    int step_chain = insert_chain(chain, value, 1);
    int step_open = insert_open(open, value, 1);

    if (step_chain > HASH_PERFOMANCE_LIMIT_CHAIN)
    {
        change_table_size(1);
        resize_chain(chain, TABLE_SIZE_CHAIN);
    }

    if (step_open > HASH_PERFOMANCE_LIMIT_OPEN)
    {
        change_table_size(0);
        resize_open(open, TABLE_SIZE_OPEN);
    }
    printf("Поиск в цепочке: %d шаг(ов)\n", step_chain);
    printf("Поиск в открытом списке: %d шаг(ов)\n", step_open);

    write_into_file(value, filename);
}

void delete_all(tree_t *tree, hash_chain *chain, hash_open *open)
{
    printf("Введите значение узла: ");
    int value = read_int();
    tree_balance_delete(tree, value);
    int step_chain = delete_chain(chain, value);
    int step_open = delete_open(open, value, 1);

    if (step_chain > HASH_PERFOMANCE_LIMIT_CHAIN)
    {
        change_table_size(1);
        resize_chain(chain, TABLE_SIZE_CHAIN);
    }

    if (step_open > HASH_PERFOMANCE_LIMIT_OPEN)
    {
        change_table_size(0);
        resize_open(open, TABLE_SIZE_OPEN);
    }
    printf("Поиск в цепочке: %d шаг(ов)\n", step_chain);
    printf("Поиск в открытом списке: %d шаг(ов)\n", step_open);
}

void find_all(tree_t *tree, hash_chain *chain, hash_open *open)
{
    printf("Введите значение узла: ");
    int val = read_int();
    find(tree->root, val, 0, 1);
    int step_chain = search_chain(chain, val);
    int step_open = search_open(open, val);

    if (step_chain > HASH_PERFOMANCE_LIMIT_CHAIN)
    {
        change_table_size(1);
        resize_chain(chain, TABLE_SIZE_CHAIN);
    }

    if (step_open > HASH_PERFOMANCE_LIMIT_OPEN)
    {
        change_table_size(0);
        resize_open(open, TABLE_SIZE_OPEN);
    }
    
    printf("Поиск в цепочке: %d шаг(ов)\n", step_chain);
    printf("Поиск в открытом списке: %d шаг(ов)\n", step_open);
}