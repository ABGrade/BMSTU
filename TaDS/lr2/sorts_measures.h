#ifndef __SORTS_MEASURES_H__
#define __SORTS_MEASURES_H__

#include <stdio.h>

#include "const_struct.h"

void bubble_sort_full(sub_t subs[], key_t keys[], size_t size);
void bubble_sort_items(sub_t subs[], size_t size);
void bubble_sort_keys(key_t keys[], size_t size);

void quick_sort_full(sub_t subs[], key_t keys[], size_t size);
void quick_sort_items(sub_t subs[], size_t size);
void quick_sort_keys(key_t keys[], size_t size);

void compare_sorts(sub_t subs[], key_t keys[], size_t size);
double measure_sort_time_items(void (*sort_func)(sub_t[], size_t), sub_t items[], size_t size);
double measure_sort_time_keys(void (*sort_func)(key_t[], size_t), key_t keys[], size_t size);

#endif  // __SORTS_MEASURES_H__