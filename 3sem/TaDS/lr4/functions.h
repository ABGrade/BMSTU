#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#define SWITCH_STACK 1
#define ADD_ELEMENT 2
#define DEL_ELEMENT 3
#define DISPLAY 4
#define SORT 5
#define MEASURE 6
#define EXIT 0

#define EPS 1e-8

#define STACK_SIZE 100000

typedef struct
{
    int values[STACK_SIZE];
    int *ep;
    int *sp;
}stack_array_t;

typedef struct stack_list
{
    int val;
    struct stack_list *next;
} stack_list_t;

typedef enum
{
    ARRAY = 1,
    LIST
}type_stack;

typedef struct deleted_node 
{
    void *address;               
    struct deleted_node *next;   
} deleted_node_t;

typedef struct
{
    type_stack type;
    union 
    {
        stack_array_t array_stack;
        stack_list_t *list_stack;
    }stack_u;
    deleted_node_t *deleted;
}stack_t;

stack_t *stack_init(int type);

void switch_stack(stack_t **stack1, stack_t **stack2);

int *get_values_to_add(int *count);
int get_number_to_delete();

void add_to_sarray(stack_t *stack, int *values, int count);
void push_arr(stack_t *stack, int value);
void delete_from_sarray(stack_t *stack, int count);
int *pop_arr(stack_t *stack);

void add_to_slist(stack_t *stack, int *values, int count);
void push_list(stack_t *stack, int value, stack_list_t *node);
void delete_from_slist(stack_t *stack, int count);
stack_list_t *pop_list(stack_t *stack);

// void combine(stack_t stack1, stack_t stack2, stack_t *sorted_stack);
// void from_arr_to_list(stack_t *from, stack_t *to);
// void from_list_to_list(stack_t *from, stack_t *to);

void sort_list(stack_t *stack1, stack_t *stack2, stack_t *sorted);
void sort_arr(stack_t *stack1, stack_t *stack2, stack_t *sorted);

void get_int(int *num);
#endif // __FUNCTIONS_H__