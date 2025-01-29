#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "tree_functions.h"
#include "tree_complex_functions.h"
#include "io.h"
#include "measure.h"

void action(int option, tree_t *tree)
{
    switch (option)
    {
        case READ_TREE:
            read_tree(tree, FILEIN);
            travel(tree, 1, 1);
            break;
        
        case ADD_NODE:
            add_node(tree, FILEIN);
            travel(tree, 1, 1);
            break;
        
        case DELETE_NODE:
            delete_node(tree);
            travel(tree, 1, 1);
            break;
        
        case FIND:
            printf("Введите значение узла: ");
            find(tree->root, read_int(), 0);
            break;
        
        case SHOW:
            export_tree_to_dot(tree->root, FILEOUT);
            break;
        
        case BYPASS:
            travel(tree, 1, 1);
            levels(tree);
            break;

        case SORT:
            sort(tree);
            break;
        
        case TESTING:
            measure();
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
    while ((c = getchar()) != '\n' && c != EOF);
}

int my_max(int a, int b)
{
    return (a > b) ? a : b;
}

void entry_message(void)
{
    printf("Программа реализует бинарное дерево\n");
}

void menu(void)
{
    printf("\n\t Выберите опцию:\n\n");
    printf("%d. Прочитать дерево\n", READ_TREE);
    printf("%d. Добавить в дерево\n", ADD_NODE);
    printf("%d. Удалить из дерева\n", DELETE_NODE);
    printf("%d. Найти в дереве\n", FIND);
    printf("%d. Сохранить дерево в dot\n", SHOW);
    printf("%d. Произвести обход\n", BYPASS);
    printf("%d. Вывести в отсортированном виде\n", SORT);
    printf("%d. Тестирование\n", TESTING);
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

void write_into_file_multiple(int *values, int size, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) 
    {
        printf("Ошибка открытия файла\n");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        fprintf(file, "%d\n", values[i]);
    }
    fclose(file);
}