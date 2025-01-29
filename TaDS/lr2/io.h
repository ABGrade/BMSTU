#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>

#include "const_struct.h"

int read_subs_from_file(const char* filename, sub_t subs[], size_t *subs_count, key_t keys[]);
sub_t *read_sub(FILE *file, size_t sub_count, sub_t *temp_sub);
sub_t *read_sub_comms(FILE *file, size_t sub_count, sub_t *temp_sub);
int read_date(FILE* file, date_t *date, size_t sub_count);

void write_into_file(const char* filename, sub_t subs[], size_t subs_count);

void show_entry_message(void);
void show_menu(void);
void show_table(sub_t subs[], size_t size);
void show_key_table(key_t keys[], size_t size);
void show_table_by_key(sub_t subs[], key_t keys[], size_t size);

void print_friend(sub_t sub);
void print_colleague(sub_t sub);

unsigned find_id(sub_t subs[], size_t size, unsigned to_find_id);
int is_number(char *string_in);

#endif