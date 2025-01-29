#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "tree_functions.h"
#include "tree_complex_functions.h"
#include "io.h"
#include "hash_functions.h"
#include "hash_types.h"
#include "common.h"
#include "measure.h"

void action(int option, tree_t *tree, hash_chain *chain, hash_open *open)
{
    switch (option)
    {
    case READ_TREE:
        read_all(tree, chain, open, FILEIN);
        travel(tree, 1, 1);
        break;

    case ADD_NODE:
        add_all(tree, chain, open, FILEIN);
        travel(tree, 1, 1);
        break;

    case DELETE_NODE:
        delete_all(tree, chain, open);
        travel(tree, 1, 1);
        break;

    case FIND:
        find_all(tree, chain, open);
        break;

    case SHOW:
        export_tree_to_dot(tree->root, FILEOUT);
        break;

    case BYPASS:
        travel(tree, 1, 1);
        levels(tree);
        show_open_hash_table(*open);
        show_chain_hash_table(*chain);
        break;

    case SORT:
        sort(tree);
        break;

    case TESTING:
        measure();
        break;

    case BALANCE:
        tree = balance_tree(tree);
        travel(tree, 1, 0);
        break;

    case CHANGE_TABLE_SIZE:
        change_hash_consts(open, chain);
        break;

    case EXIT:
        printf("Выходим...\n");
        break;

    default:
        printf("Неизвестный выбор, повторите попытку\n");
        break;
    }
}

int get_option(void)
{
    printf("Опция: ");
    return read_int();
}

int read_int(void)
{
    int value;
    while (scanf("%d", &value) != 1)
    {
        printf("Повторите попытку ввода: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    return value;
}

unsigned int read_uint(void)
{
    long int value;
    while (scanf("%ld", &value) != 1 || value < 0)
    {
        printf("Повторите попытку ввода: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    return value;
}

char *read_line_dynamic(FILE *file)
{
    size_t max_len = BASE_STR_SIZE;
    size_t len = 0;
    char *buffer = (char *)malloc(max_len * sizeof(char));
    if (!buffer)
    {
        return NULL;
    }

    int ch;
    while ((ch = fgetc(file)) != '\n' && ch != EOF)
    {
        // Если достигли предела буфера, увеличиваем его
        if (len + 1 >= max_len)
        {
            max_len *= 2;
            char *new_buffer = (char *)realloc(buffer, max_len * sizeof(char));
            if (!new_buffer)
            {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
        buffer[len++] = ch;
    }

    // Проверка на конец файла без символов
    if (len == 0 && ch == EOF)
    {
        free(buffer);
        return NULL;
    }

    // Завершаем строку нулевым символом
    buffer[len] = '\0';
    return buffer;
}

void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int my_max(int a, int b)
{
    return (a > b) ? a : b;
}

void entry_message(void)
{
    printf("Программа реализует сбалансированное дерево и хэш-таблицы с открытой и цепочной адресацией.\n");
}

void menu(void)
{
    printf("\n\t Выберите опцию:\n\n");
    printf("%d. Прочитать дерево\n", READ_TREE);
    printf("%d. Добавить\n", ADD_NODE);
    printf("%d. Удалить\n", DELETE_NODE);
    printf("%d. Найти\n", FIND);
    printf("%d. Сохранить дерево в dot\n", SHOW);
    printf("%d. Произвести обход\n", BYPASS);
    printf("%d. Вывести в отсортированном виде\n", SORT);
    printf("%d. Тестирование\n", TESTING);
    printf("%d. Сбалансировать\n", BALANCE);
    printf("%d. Изменить размер хеш-таблицы\n", CHANGE_TABLE_SIZE);
    printf("%d. Выход\n", EXIT);
}

void write_into_file(int value, char *filename)
{
    FILE *file = fopen(filename, "a+");
    if (file == NULL)
    {
        printf("Ошибка открытия файла\n");
        return;
    }
    fprintf(file, "%d\n", value);
    fclose(file);
}