#ifndef __LIST_FUNC_H__
#define __LIST_FUNC_H__

void list_pushs(list_queue_t *queue);
void list_pops(list_queue_t *queue);

int list_push(list_queue_t *queue, request_t request);
queue_item_t *list_pop(list_queue_t *queue);

void list_insert_at(list_queue_t *queue, request_t new_request, unsigned position);

void clear_list(list_queue_t *queue);

#endif // __LIST_FUNC_H__