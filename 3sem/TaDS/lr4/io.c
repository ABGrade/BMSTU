#include <stdio.h>

#include "functions.h"
#include "io.h"

void decode_error_code(int code)
{
    switch (code)
    {
        case READ_OPTION_ERROR:
            printf("Неудалось прочитать опцию.\n");
            break;
        
        default:
            printf("Выполнено успешно!\n");
            break;
    }
}

char get_option(void)
{
    char option;
    printf("Опция: ");
    while (scanf(" %c", &option) != 1)
    {
        printf("Повторите попытку ввода: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    return option - '0';
}

void display_stack(stack_t stack)
{
    printf("Текущее состояние стека:\n");
    if (stack.type == ARRAY)
    {
        printf("Стек реализован в виде массива.\n");
        for (int *p = stack.stack_u.array_stack.sp; p >= stack.stack_u.array_stack.values; p--)
        {
            printf("Элемент: %d, адрес: %p\n", *p, p);
        }
    }
    else
    {
        printf("Стек реализован списком.\n");
        stack_list_t *p = stack.stack_u.list_stack;
        while (p)
        {
            printf("Элемент: %d, адрес: %p\n", p->val, p);
            p = p->next;
        }

        deleted_node_t *d = stack.deleted;
        while (d)
        {
            printf("Удаленный адрес: %p\n", d->address);
            d = d->next;
        }
    }
}

void entry_message(void)
{
    printf("Программа симулирует работу со стеком в двух вариациях реаилзации стека: массивом и списком.\n");
    menu_stack();
}

void menu(void)
{
    printf("Выберите опцию:\n");
    printf("%d. Сменить стек\n", SWITCH_STACK);
    printf("%d. Добавить \n", ADD_ELEMENT);
    printf("%d. Удалить \n", DEL_ELEMENT);
    printf("%d. Отобразить состояние\n", DISPLAY);
    printf("%d. Сортировать\n", SORT);
    printf("%d. Тест\n", MEASURE);
    printf("%d. Выход\n", EXIT);
}

void menu_stack(void)
{
    printf("Выберите реализацию стека:\n");
    printf("1. Массивом\n");
    printf("2. Списком\n");
}

void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

