#include <stdio.h>
#include <time.h>

#include "types.h"
#include "errors.h"

#include "arr_func.h"
#include "io.h"

void arr_pushs(arr_queue_t *queue)
{
    request_t request = {.type = FIRST, .coming = 0, .processing = 0};
    printf("Введите кол-во добавляемых элементов: ");
    unsigned addition_size = get_unsigned();
    for (unsigned i = 0; i < addition_size; i++)
    {
        arr_push(queue, request);
    }
}

void arr_pops(arr_queue_t *queue)
{
    printf("Введите кол-во удаляемых элементов: ");
    unsigned del_size = get_unsigned();
    for (unsigned i = 0; i < del_size; i++)
    {
        arr_pop(queue);
    }
}

int arr_push(arr_queue_t *queue, request_t request)
{
    if (queue->length == MAX_QUEUE_LEN)
        return QUEUE_OVERFLOW_ERROR;

    queue->array[queue->in] = request;
    queue->in = (queue->in + 1 + MAX_QUEUE_LEN) % MAX_QUEUE_LEN;
    queue->length++;
    return 0;
}

request_t *arr_pop(arr_queue_t *queue)
{
    if (!queue->length)
        return NULL;

    request_t *d = &queue->array[queue->out];
    queue->length--;
    queue->out = (queue->out + 1) % MAX_QUEUE_LEN;
    return d;
}

int arr_insert_at(arr_queue_t *queue, request_t request, unsigned position) 
{
    if (queue->length == MAX_QUEUE_LEN)
        return QUEUE_OVERFLOW_ERROR;

    if (position > queue->length)
        position = queue->length; // Если позиция вне очереди, вставляем в конец

    unsigned real_position = (queue->out + position) % MAX_QUEUE_LEN;

    // Сдвиг элементов вправо
    for (unsigned i = queue->length; i > position; --i) 
    {
        unsigned from = (queue->out + i - 1) % MAX_QUEUE_LEN;
        unsigned to = (queue->out + i) % MAX_QUEUE_LEN;
        queue->array[to] = queue->array[from];
    }

    // Вставка элемента
    queue->array[real_position] = request;

    // Обновление индекса `in` и длины очереди
    queue->in = (queue->in + 1) % MAX_QUEUE_LEN;
    queue->length++;

    return 0;
}
