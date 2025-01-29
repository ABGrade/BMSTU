#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "functions.h"
#include "io.h"
#include "measure.h"

void choice(char option, stack_t **stack1, stack_t **stack2);

int main(void)
{
    srand(time(NULL));
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); 
    
    char option;
    stack_t *stack1 = NULL;
    stack_t *stack2 = NULL;
    
    entry_message(); 
    do
    {
        option = get_option();
        stack1 = stack_init(option);
    } while (!stack1);
    
    do
    {
        menu();
        option = get_option();
        choice(option, &stack1, &stack2);
    } while (option);

    free(stack1);
    free(stack2);
    return 0;
}

void choice(char option, stack_t **stack1, stack_t **stack2)
{
    switch (option)
    {
        case SWITCH_STACK:
            switch_stack(stack1, stack2);
            break;

        case ADD_ELEMENT:
            int size; 
            int *values = get_values_to_add(&size);
            if ((*stack1)->type == ARRAY)
            {
                add_to_sarray(*stack1, values, size);
            }
            else if ((*stack1)->type == LIST)
            {
                add_to_slist(*stack1, values, size);
            }
            free(values);
            break;

        case DEL_ELEMENT:
            if ((*stack1)->type == ARRAY)
            {
                delete_from_sarray(*stack1, get_number_to_delete());
            }
            else if ((*stack1)->type == LIST)
            {
                delete_from_slist(*stack1, get_number_to_delete());
            }
            break;
        
        case DISPLAY:
            if ((*stack1) == NULL)
            {
                printf("Стек пуст!\n");
                break;
            }
            display_stack(**stack1);
            break;

        case SORT:
            if ((*stack1) == NULL || (*stack2) == NULL)
            {
                printf("Один из стеков остается пуст.\n");
                break;
            }
            if ((*stack1)->type != (*stack2)->type)
            {
                printf("Неодинаковые типы не могут использоваться для сортировки.\n");
                break;
            }
            stack_t *sorted = NULL;
            if ((*stack1)->type == LIST)
            {
                sorted = stack_init(LIST);
                if (sorted == NULL)
                    break;
                sort_list(*stack1, *stack2, sorted);
            }
            else
            {
                sorted = stack_init(ARRAY);
                if (sorted == NULL)
                    break;
                sort_arr(*stack1, *stack2, sorted);
            }
            display_stack(*sorted);
            break;
        
        case MEASURE:
            measure();
            break;

        case EXIT:
            printf("Выход...\n");
            break;

        default:
            printf("Неизвестная опция.\n");
            break;
    }
}