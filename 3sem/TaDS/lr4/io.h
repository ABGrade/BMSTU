#ifndef __IO_H__
#define __IO_H__

#define READ_OPTION_ERROR 1
#define ALLOCATE_ERROR 2    

#include "functions.h"

void decode_error_code(int code);

char get_option(void);
void display_stack(stack_t stack);
void clear_input_buffer(void);

void entry_message(void);
void menu(void);
void menu_stack(void);

#endif // __IO_H__