#ifndef __ARR_FUNC_H__
#define __ARR_FUNC_H__

#include "types.h"

void arr_pushs(arr_queue_t *queue);
void arr_pops(arr_queue_t *queue);

int arr_push(arr_queue_t *queue, request_t request);
request_t *arr_pop(arr_queue_t *queue);

int arr_insert_at(arr_queue_t *queue, request_t request, unsigned position);


#endif //__ARR_FUNC_H__