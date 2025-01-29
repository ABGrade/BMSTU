#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "io.h"

stack_t *stack_init(int type)
{
    stack_t *stack = malloc(sizeof(stack_t));
    if (!stack)
    {
        printf("Ошибка выделения памяти.\n");
        return NULL;
    }
    stack->deleted = NULL;
    if (type == ARRAY)
    {
        stack->type = ARRAY;
        stack->stack_u.array_stack.ep = stack->stack_u.array_stack.values + STACK_SIZE - 1;
        stack->stack_u.array_stack.sp = stack->stack_u.array_stack.values - 1;
        return stack;
    }
    else if (type == LIST)
    {
        stack->type = LIST;
        stack->stack_u.list_stack = NULL;
        return stack;
    }
    else
    {
        free(stack);
        printf("Неизвестный тип.\n");
    }
    printf("Ошибка Инициализации.\n");
    return NULL;    
}

void switch_stack(stack_t **stack1, stack_t **stack2)
{
    stack_t *temp = *stack1;
    *stack1 = *stack2;
    *stack2 = temp;
    if (*stack1 == NULL)
    {
        do
        {
            menu_stack();
            int type = get_option();
            *stack1 = stack_init(type);
        } while (!(*stack1));
    }
}

void get_int(int *num)
{
    printf("Введите число: ");
    while (scanf("%d", num) != 1)
    {
        printf("Повторите ввод: ");
        clear_input_buffer();
    }
}

int *get_values_to_add(int *count) 
{
    printf("Введите количество элементов для добавления\n");
    get_int(count);

    int *values = malloc((*count) * sizeof(int));
    if (!values) 
    {
        printf("Ошибка выделения памяти.\n");
        *count = 0;
        return NULL;
    }

    printf("Введите значения:\n");
    for (int i = 0; i < *count; i++) 
    {
        get_int(&values[i]);
    }

    return values;
}

int get_number_to_delete() 
{
    int count;
    printf("Введите количество элементов для удаления\n");
    get_int(&count);
    return count;
}

void push_arr(stack_t *stack, int value)
{
    if (stack->stack_u.array_stack.sp >= stack->stack_u.array_stack.ep)
        return;
        
    stack->stack_u.array_stack.sp++;
    *(stack->stack_u.array_stack.sp) = value;
}

void add_to_sarray(stack_t *stack, int *values, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (stack->stack_u.array_stack.ep <= stack->stack_u.array_stack.sp)
        {
            printf("Ожидается переполнение стека! Ввод будет прекращен.\n");
            return;
        }
        push_arr(stack, values[i]);
    }
}

int *pop_arr(stack_t *stack)
{
    if (stack->stack_u.array_stack.sp < stack->stack_u.array_stack.values)
        return NULL;

    int *val = stack->stack_u.array_stack.sp;
    stack->stack_u.array_stack.sp--;
    return val;
}

void delete_from_sarray(stack_t *stack, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (stack->stack_u.array_stack.sp < stack->stack_u.array_stack.values)
        {
            printf("Стек пуст! Удаление будет прекращено.\n");
            return;
        }
        printf("Удален элемент по адресу: %p с значением: %d\n", stack->stack_u.array_stack.sp, *stack->stack_u.array_stack.sp);
        pop_arr(stack);
    }
}

void push_list(stack_t *stack, int value, stack_list_t *node)
{
    if (!node)
    {
        node = malloc(sizeof(stack_list_t));
        if (!node)
        {
            printf("Ошибка выделения памяти.\n");
            return;
        }
        node->val = value;
    }
    node->next = stack->stack_u.list_stack;
    stack->stack_u.list_stack = node;
}

void add_to_slist(stack_t *stack, int *values, int count)
{
    for (int i = 0; i < count; i++)
    {
        push_list(stack, values[i], NULL);
    }
}

stack_list_t *pop_list(stack_t *stack)
{
    if (stack->stack_u.list_stack == NULL)
        return NULL;

    stack_list_t *deleted = stack->stack_u.list_stack;
    stack->stack_u.list_stack = stack->stack_u.list_stack->next;
    return deleted;
}

void delete_from_slist(stack_t *stack, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (stack->stack_u.list_stack == NULL)
        {
            printf("Стек пуст! Удаление будет прекращено.\n");
            return;
        }
        stack_list_t *temp = stack->stack_u.list_stack;
        printf("Удален элемент по адресу: %p с значением: %d\n", temp, temp->val);
        
        deleted_node_t *deleted_node = malloc(sizeof(deleted_node_t));
        if (!deleted_node) 
        {
            printf("Ошибка выделения памяти для хранения адреса удаленного элемента.\n");
            return;
        }

        deleted_node->address = (void *)temp;
        deleted_node->next = stack->deleted;
        stack->deleted = deleted_node;

        pop_list(stack);
        free(temp);
    }
}

void sort_list(stack_t *stack1, stack_t *stack2, stack_t *sorted) 
{
    // Переместить все элементы из stack2 в stack1
    while (stack2->stack_u.list_stack != NULL)
    {
        stack_list_t *val = pop_list(stack2);
        push_list(stack1, 0, val);
    }

    // Действует пока остались элементы в обоих стеках
    while (stack1->stack_u.list_stack != NULL) 
    {
        stack_list_t *min_node = pop_list(stack1);

        // Найти минимальный элемент в stack1, переместить остальные в stack2
        while (stack1->stack_u.list_stack != NULL) 
        {
            stack_list_t *current_node = pop_list(stack1);
            if (current_node->val < min_node->val) 
            {
                push_list(stack2, 0, min_node);
                min_node = current_node;
            } 
            else 
            {
                push_list(stack2, 0, current_node);
            }
        }

        // Поместить минимальный элемент в sorted
        if (min_node != NULL) 
            push_list(sorted, 0, min_node);

        // Найти минимальный элемент в stack2, переместить остальные в stack1
        min_node = pop_list(stack2);

        while (stack2->stack_u.list_stack != NULL) 
        {
            stack_list_t *current_node = pop_list(stack2);
            if (current_node->val < min_node->val) 
            {
                push_list(stack1, 0, min_node);
                min_node = current_node;
            } 
            else 
            {
                push_list(stack1, 0, current_node);
            }
        }

        // Поместить минимальный элемент в sorted
        if (min_node != NULL) 
            push_list(sorted, 0, min_node);
    }
}

void sort_arr(stack_t *stack1, stack_t *stack2, stack_t *sorted) 
{
    // Переместить все элементы из stack2 в stack1
    while (stack2->stack_u.array_stack.sp >= stack2->stack_u.array_stack.values) 
    {
        int *val = pop_arr(stack2);
        push_arr(stack1, *val);
    }

    while (stack1->stack_u.array_stack.sp >= stack1->stack_u.array_stack.values) 
    {
        int *min_found = pop_arr(stack1);

        // Найти минимальный элемент в stack1, переместить остальные в stack2
        while (stack1->stack_u.array_stack.sp >= stack1->stack_u.array_stack.values) 
        {
            int *val = pop_arr(stack1);
            if (val && *val < *min_found) 
            {
                push_arr(stack2, *min_found);
                min_found = val;
            } 
            else 
            {
                push_arr(stack2, *val);
            }
        }

        // Поместить минимальный элемент в sorted
        push_arr(sorted, *min_found);

        // Найти минимальный элемент в stack2, переместить остальные в stack1
        min_found = pop_arr(stack2);

        while (stack2->stack_u.array_stack.sp >= stack2->stack_u.array_stack.values) 
        {
            int *val = pop_arr(stack2);
            if (val && *val < *min_found) 
            {
                push_arr(stack1, *min_found);
                min_found = val;
            } 
            else 
            {
                push_arr(stack1, *val);
            }
        }

        // Поместить минимальный элемент в sorted
        if (min_found)
            push_arr(sorted, *min_found);
    }
}


// void from_arr_to_list(stack_t *from, stack_t *to)
// {
//     while (from->stack_u.array_stack.sp >= from->stack_u.array_stack.bp)
//     {
//         int val = pop_arr(from);
//         push_list(to, val);
//     }
// }

// void from_list_to_list(stack_t from, stack_t *to)
// {
//     while (from.stack_u.list_stack != NULL)
//     {
//         int val = pop_list(&from);
//         push_arr(to, val);
//     }
// }



// void from_to(stack_t *from, stack_t *to)
// {
//     if (from->type == ARRAY && to->type == LIST)
//     {
//         from_arr_to_list(from, to);
//     }
//     else if (from->type == LIST && to->type == ARRAY)
//     {
//         from_list_to_arr(from, to);
//     }
//     else if (from->type == ARRAY && to->type == ARRAY)
//     {
//         from_arr_to_arr(from, to);
//     }
//     else if (from->type == LIST && to->type == LIST)
//     {
//         from_list_to_list(from, to);
//     }
// }

// void sort_list(stack_t *stack) 
// {
//     if (stack->type != LIST) 
//     {
//         printf("Ошибка: Сортировка поддерживается только для стека, реализованного списком.\n");
//         return;
//     }

//     stack_t sorted;
//     sorted.type = LIST;
//     sorted.stack_u.list_stack = NULL;

//     while (stack->stack_u.list_stack != NULL) 
//     {
//         // Извлекаем верхний элемент из stack
//         int top_val = stack->stack_u.list_stack->val;
//         stack_list_t *temp_node = stack->stack_u.list_stack;
//         stack->stack_u.list_stack = stack->stack_u.list_stack->next;
//         free(temp_node);

//         // Перемещаем элементы из sorted обратно в stack, пока находим меньший элемент
//         while (sorted.stack_u.list_stack != NULL && sorted.stack_u.list_stack->val > top_val) 
//         {
//             stack_list_t *move_node = sorted.stack_u.list_stack;
//             sorted.stack_u.list_stack = sorted.stack_u.list_stack->next;
//             move_node->next = stack->stack_u.list_stack;
//             stack->stack_u.list_stack = move_node;
//         }

//         // Вставляем top_val в правильное место в sorted
//         stack_list_t *new_node = malloc(sizeof(stack_list_t));
//         new_node->val = top_val;
//         new_node->next = sorted.stack_u.list_stack;
//         sorted.stack_u.list_stack = new_node;
//     }

//     // Переносим все элементы обратно из sorted в stack, чтобы стек оказался отсортированным
//     while (sorted.stack_u.list_stack != NULL) 
//     {
//         stack_list_t *move_back_node = sorted.stack_u.list_stack;
//         sorted.stack_u.list_stack = sorted.stack_u.list_stack->next;
//         move_back_node->next = stack->stack_u.list_stack;
//         stack->stack_u.list_stack = move_back_node;
//     }

//     printf("Сортировка завершена.\n");
// }

