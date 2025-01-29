#ifndef __IO_H__
#define __IO_H__

#define CREATE_QUEUE_ARR 1
#define CREATE_QUEUE_LIST 2
#define DEL_ELEMS_ARR 3
#define DEL_ELEMS_LIST 4
#define SHOW_STATE_ARR 5
#define SHOW_STATE_LIST 6
#define MODELING_ARRAY 7
#define MODELING_LIST 8
#define UPDATE_TIME 9
#define SPEEDTEST 10
#define EXIT 0

void action(int option, arr_queue_t *arr_queue, list_queue_t *list_queue);
void show_arr_queue(arr_queue_t queue);
void show_list_queue(list_queue_t queue);

int get_option(void);
unsigned get_unsigned(void);
void change_time(void);

void entry_message(void);
void menu(void);

void clear_input_buffer(void);

#endif // __IO_H__