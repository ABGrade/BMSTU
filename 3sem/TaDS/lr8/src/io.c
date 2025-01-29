#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "io.h"
#include "graph_functions.h"

void clear_input_buffer(void);
unsigned int read_uint(void);
int read_int(void);
int get_start_vertex(void);

void action(int option, graph_t* graph)
{
    switch (option)
    {
        case DOT:
            save_graph_to_dot(graph, FILEDOT);
            break;

        case CONNECTIONS:
            print_graph(graph);
            break;

        case UNREACHABLE:
            find_unreachable(graph, get_start_vertex());
            break;

        case EXIT:
            printf("Выходим...\n");
            break;
    
        default:
            printf("Такой опции нет\n");
            break;
    }
}


void menu(void)
{
    printf("\n\t Выберите опцию:\n\n");
    printf("%d. Сохранить в dot\n", DOT);
    printf("%d. Матрица связанности\n", CONNECTIONS);
    printf("%d. Недостижимые вершины\n", UNREACHABLE);
    printf("%d. Выход\n", EXIT);
}

int get_option(void)
{
    printf("Опция: ");
    return read_int();
}

int get_start_vertex(void)
{
    printf("Введите начальную вершину: ");
    return read_uint();
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

void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void entry_message()
{
    printf("Программа реализует граф с поиском недостижимых вершин.\n");
}