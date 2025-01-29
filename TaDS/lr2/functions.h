#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include <stdio.h>

#include "const_struct.h"

void choose(int rc, sub_t subs[], key_t keys[], size_t *size);

void del_from_table(sub_t subs[], key_t keys[], size_t *size);
void add_to_table(sub_t subs[], key_t keys[], size_t *size);
void print_upcoming_birthdays(sub_t subs[], size_t size);

void clear_input_buffer(void);
int compare_sub(const void *a, const void *b);
int compare_key(const void *a, const void *b);
int compare_dates(date_t d1, date_t d2);

void increment_date(date_t *date);
int days_in_month(int month, int year);
int is_leap_year(int year);

#endif // __FUNCTIONS_H__