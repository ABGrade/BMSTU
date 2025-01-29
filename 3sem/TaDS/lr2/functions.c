#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sorts_measures.h"
#include "functions.h"
#include "io.h"
#include "const_struct.h"

void del_from_table(sub_t subs[], key_t keys[], size_t *size)
{
    unsigned int id;
    if (*size == 0)
    {
        printf("Пустая таблица\n");
        return;
    }
    
    printf("Введите id элемента на удаление (неотрицательное число): ");
    if (scanf("%u", &id) != 1)
    {
        printf("Неправильный тип id\n");
        return;
    }
    
    char is_found = 0;
    for (size_t i = 0; i < *size; i++)
    {
        if (subs[i].id == id)
        {
            is_found = 1;
            subs[i] = subs[*size - 1];
            keys[i] = keys[*size - 1];
            subs[i].id = id;
            keys[i].id = id;
            (*size)--;
        }
    }
    if (!is_found)
    {
        printf("Неудается найти абонента с данным id\n");
    }
}

void add_to_table(sub_t subs[], key_t keys[], size_t *size)
{
    sub_t temp_sub;
    key_t temp_key;
    if (read_sub_comms(stdin, *size, &temp_sub) == NULL)
    {
        printf("Невозможно добавть абонента\n");
        return;
    }
    if (*size + 1 > MAX_SUB)
    {
        printf("Таблица достигла максимального кол-ва абонентов\n");
        return;
    }
    temp_key.id = temp_sub.id;
    if (temp_sub.kind_sub == FRIEND)
    {
        temp_key.number = temp_sub.status.friend.user.number;
    }
    else
    {
        temp_key.number = temp_sub.status.college.user.number;
    }
    subs[*size] = temp_sub;
    keys[*size] = temp_key;
    (*size)++;
}



void clear_input_buffer(void) 
{
    int c;
    // Читаем символы до конца строки или до конца файла
    while ((c = getchar()) != '\n' && c != EOF);
}

int compare_sub(const void *a, const void *b)
{
    // Приводим указатели к типу sub_t
    const sub_t *sub_a = (const sub_t *)a;
    const sub_t *sub_b = (const sub_t *)b;

    if (sub_a->kind_sub == FRIEND)
    {
        if (sub_a->status.friend.user.number < sub_b->status.friend.user.number) 
        {
            return -1; // sub_a меньше sub_b
        }
        else if (sub_a->status.friend.user.number > sub_b->status.friend.user.number) 
        {
            return 1; // sub_a больше sub_b
        }
        return 0; // sub_a равен sub_b
    }
    else
    {
        if (sub_a->status.college.user.number < sub_b->status.college.user.number) 
        {
            return -1; // sub_a меньше sub_b
        }
        else if (sub_a->status.college.user.number > sub_b->status.college.user.number) 
        {
            return 1; // sub_a больше sub_b
        }
        return 0; // sub_a равен sub_b
    }
}

int compare_key(const void *a, const void *b)
{
    const key_t *key_a = (const key_t *)a;
    const key_t *key_b = (const key_t *)b;

    if (key_a->number < key_b->number)
    {
        return -1;
    }
    else if (key_a->number > key_b->number)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void choose(int rc, sub_t subs[], key_t keys[], size_t *size)
{
    switch (rc)
    {
        case 1:
            show_table(subs, *size);
            break;
        
        case 2:
            show_key_table(keys, *size);
            break;

        case 3:
            show_table_by_key(subs, keys, *size);
            break;

        case 4:
            add_to_table(subs, keys, size);
            break;

        case 5:
            del_from_table(subs, keys, size);
            break;

        case 6:
            print_upcoming_birthdays(subs, *size);
            break;

        case 7:
            bubble_sort_keys(keys, *size);
            break;

        case 8:
            bubble_sort_full(subs, keys, *size);
            break;

        case 9:
            compare_sorts(subs, keys, *size);
            break;

        default:
            printf("Unknown option\n");
            break;
    }
}

int is_leap_year(int year) 
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Определение количества дней в месяце
int days_in_month(int month, int year) 
{
    int days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year)) 
    {
        return 29;
    }
    return days_in_months[month - 1];
}

// Сравнение двух дат
int compare_dates(date_t d1, date_t d2) 
{
    if (d1.month == d2.month && d1.day == d2.day) 
        return 1;
    return 0;
}

// Увеличение даты на 1 день
void increment_date(date_t *date) 
{
    date->day++;
    if (date->day > days_in_month(date->month, date->year)) 
    {
        date->day = 1;
        date->month++;
        if (date->month > 12) 
        {
            date->month = 1;
            date->year++;
        }
    }
}

// Вывод всех дней рождений в течение следующей недели
void print_upcoming_birthdays(sub_t subs[], size_t size) 
{
    date_t current_date_base;
    printf("Ведите дату (в формате: год месяц день): ");
    if(read_date(stdin, &current_date_base, 0)) return;
    for (size_t i = 0; i < size; i++)
    {
        date_t current_date = current_date_base;
        if (subs[i].kind_sub == COLLEGE)
        {
            continue;
        }
        date_t temp_date = subs[i].status.friend.date_birth;
        for (size_t j = 0; j <= 7; j++)
        {
            if (compare_dates(temp_date, current_date)) 
            {
                printf("День рождения у %s %s будет %d.%d\n", 
                        subs[i].status.friend.user.surname, subs[i].status.friend.user.name,
                        temp_date.month, temp_date.day);
                break;
            }
            increment_date(&current_date);
        }
    }
}