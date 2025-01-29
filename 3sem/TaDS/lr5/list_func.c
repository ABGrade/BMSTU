#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "errors.h"

#include "list_func.h"
#include "io.h"

void list_pushs(list_queue_t *queue)
{
    request_t request = {.type = FIRST, .coming = 0, .processing = 0};

    printf("Введите кол-во добавляемых элементов: ");
    unsigned addition_size = get_unsigned();
    for (unsigned i = 0; i < addition_size; i++)
    {
        list_push(queue, request);
    }
}

void list_pops(list_queue_t *queue)
{
    printf("Введите кол-во удаляемых элементов: ");
    unsigned del_size = get_unsigned();
    for (unsigned i = 0; i < del_size; i++)
    {
        queue_item_t *removed_item = list_pop(queue);
        if (removed_item)
        {
            deleted_node_t *new_deleted_node = malloc(sizeof(deleted_node_t));
            if (!new_deleted_node)
            {
                printf("Недостаточно памяти.\n");
                return;
            }

            new_deleted_node->address = removed_item;
            new_deleted_node->next = queue->deleted;
            queue->deleted = new_deleted_node;
            free(removed_item);
        }
    }
}

int list_push(list_queue_t *queue, request_t request)
{
    queue_item_t *new = malloc(sizeof(queue_item_t));
    if (!new)
        return ALLOCATE_ERROR;

    new->data = request;
    new->prev = NULL;

    (!queue->tail) ? (queue->tail = new) : ((*queue->tail).prev = new);
    queue->tail = new;
    queue->head = (!queue->head) ? new : queue->head;
    queue->length++;
    return 0;
}

queue_item_t *list_pop(list_queue_t *queue)
{
    if (!queue->length)
        return NULL;

    queue_item_t *tmp = queue->head;
    queue->head = tmp->prev;
    if (!queue->head)
        queue->tail = NULL;
    queue->length--;
    return tmp;
}

void clear_list(list_queue_t *queue)
{
    queue_item_t *current = queue->head;
    while (current)
    {
        queue_item_t *next = current->prev;
        free(current);
        current = next;
    }
    queue->head = NULL;
    queue->tail = NULL;

    // deleted_node_t *deleted_current = queue->deleted;
    // while (deleted_current)
    // {
    //     deleted_node_t *next = deleted_current->next;
    //     free(deleted_current);
    //     deleted_current = next;
    // }
    // queue->deleted = NULL;
    // queue->length = 0;
}

void list_insert_at(list_queue_t *queue, request_t new_request, unsigned position)
{
    // Создаем новый элемент
    queue_item_t *new_item = (queue_item_t *)malloc(sizeof(queue_item_t));
    if (!new_item) 
    {
        printf("Ошибка выделения памяти.\n");
        return;
    }

    new_item->data = new_request;
    new_item->prev = NULL;

    // Если очередь пуста, вставляем элемент как первый
    if (queue->head == NULL) 
    {
        queue->head = queue->tail = new_item;
        queue->length++;
        return;
    }

    // Вставка элемента на указанную позицию
    queue_item_t *current = queue->head;
    unsigned index = 0;

    // Находим элемент, предшествующий нужной позиции
    while (current != NULL && index < position - 1) 
    {
        current = current->prev;
        index++;
    }

    // Вставляем новый элемент после текущего
    new_item->prev = current->prev;
    current->prev = new_item;

    // Если вставка происходит перед последним элементом, обновляем хвост
    if (new_item->prev == NULL) 
    {
        queue->tail = new_item;
    }

    queue->length++;
}
