#ifndef __TYPES_H__
#define __TYPES_H__

#define MAX_QUEUE_LEN 1000

typedef enum order_type
{
    FIRST = 1,
    SECOND
} request_type_t;

typedef struct
{
    request_type_t type;
    double coming;
    double processing;
} request_t;

typedef struct deleted_node 
{
    void *address;               
    struct deleted_node *next;   
} deleted_node_t;

typedef struct
{
    request_t array[MAX_QUEUE_LEN];
    unsigned in;
    unsigned out;
    unsigned length;
} arr_queue_t;

typedef struct queue_item
{
    request_t data;
    struct queue_item *prev;
}queue_item_t;

typedef struct list_queue
{
    queue_item_t *head;
    queue_item_t *tail;
    deleted_node_t *deleted;
    unsigned length;
} list_queue_t;

#endif // __TYPES_H__