#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "io.h"
#include "functions.h"
#include "const_struct.h"

int read_date(FILE* file, date_t *date, size_t sub_count)
{
    char buffer[MAX_STRING_LENGTH];  // Буфер для хранения строки даты

    // Считываем строку с датой
    if (fgets(buffer, sizeof(buffer), file) == NULL)
    {
        printf("Ошибка в чтении абонента #%zu\n", sub_count);
        return -1;  // Ошибка чтения
    }

    // Используем sscanf для разбора даты из строки
    if (sscanf(buffer, "%d %d %d", &date->year, &date->month, &date->day) != 3 ||
        date->month > 12 || date->month < 1 || date->day > 31 || date->day < 1 || date->year < 0)
    {
        printf("Неправилльный формат даты у #%zu\n", sub_count);
        return -2;  // Ошибка формата даты
    }

    if (date->day > days_in_month(date->month, date->year)) 
    {
        printf("Некорректное кол-во дней у #%zu (месяц #%d имеет %d дней)\n", 
                sub_count, date->month, days_in_month(date->month, date->year));
        return -3;
    }
    return 0; 
}

sub_t *read_sub(FILE *file, size_t sub_count, sub_t *temp_sub)
{
    user_t temp_user;
    char buffer[MAX_STRING_LENGTH];

    // Чтение фамилии
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';  // Удаляем символ новой строки
    strncpy(temp_user.surname, buffer, MAX_STRING_LENGTH);

    // Чтение имени
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(temp_user.name, buffer, MAX_STRING_LENGTH);


    // Чтение номера телефона
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';
    if (!is_number(buffer) || strlen(buffer) > 16) return NULL;
    temp_user.number = atoll(buffer);  // Конвертация строки в long long

    // Проверка корректности номера
    if (temp_user.number <= 0) 
    {
        printf("Error: Invalid phone number for user %s %s\n", temp_user.surname, temp_user.name);
        return NULL;
    }

    // Чтение улицы
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(temp_user.adress.street, buffer, MAX_STRING_LENGTH);

    // Чтение номера дома
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(temp_user.adress.house, buffer, MAX_STRING_LENGTH);

    // Чтение типа записи (FRIEND или COLLEGE)
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';  // Удаляем символ новой строки

    if (strcmp(buffer, "FRIEND") == 0 || strcmp(buffer, "0") == 0) 
    {
        temp_sub->status.friend.user = temp_user;
        if(read_date(file, &temp_sub->status.friend.date_birth, sub_count)) return NULL;
        temp_sub->kind_sub = FRIEND;
    } else if (strcmp(buffer, "COLLEGE") == 0 || strcmp(buffer, "1") == 0) 
    {
        temp_sub->status.college.user = temp_user;

        // Чтение позиции
        if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(temp_sub->status.college.position, buffer, MAX_STRING_LENGTH);

        // Чтение организации
        if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(temp_sub->status.college.organization, buffer, MAX_STRING_LENGTH);
        temp_sub->kind_sub = COLLEGE;
    } 
    else 
    {
        printf("Error: Unknown sub type: %s\n", buffer);
        return NULL;
    }
    temp_sub->id = sub_count;
    return temp_sub;
}

sub_t *read_sub_comms(FILE *file, size_t sub_count, sub_t *temp_sub)
{
    user_t temp_user;
    char buffer[MAX_STRING_LENGTH];

    // Чтение фамилии
    printf("Ведите фамилию (255 символов): ");
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';  // Удаляем символ новой строки
    strncpy(temp_user.surname, buffer, MAX_STRING_LENGTH);

    // Чтение имени
    printf("Ведите имя (255 символов): ");
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(temp_user.name, buffer, MAX_STRING_LENGTH);


    // Чтение номера телефона
    printf("Введите номер (максимальная длина 16 цифр): ");
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';
    if (!is_number(buffer) || strlen(buffer) > 16) return NULL;
    temp_user.number = atoll(buffer);  // Конвертация строки в long long

    // Проверка корректности номера
    if (temp_user.number <= 0) 
    {
        printf("Error: Invalid phone number for user %s %s\n", temp_user.surname, temp_user.name);
        return NULL;
    }

    // Чтение улицы
    printf("Ведите улицу (255 символов): ");
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(temp_user.adress.street, buffer, MAX_STRING_LENGTH);

    // Чтение номера дома
    printf("Ведите номер дома (255 символов): ");
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(temp_user.adress.house, buffer, MAX_STRING_LENGTH);

    // Чтение типа записи (FRIEND или COLLEGE)
    printf("Ведите статус абонента (FRIEND - 0 или COLLEGE - 1): ");
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = '\0';  // Удаляем символ новой строки

    if (strcmp(buffer, "FRIEND") == 0 || strcmp(buffer, "0") == 0) 
    {
        temp_sub->status.friend.user = temp_user;
        printf("Ведите дату (в формате год месяц день): ");
        if(read_date(file, &temp_sub->status.friend.date_birth, sub_count)) return NULL;
        temp_sub->kind_sub = FRIEND;
    } else if (strcmp(buffer, "COLLEGE") == 0 || strcmp(buffer, "1") == 0) 
    {
        temp_sub->status.college.user = temp_user;

        // Чтение позиции
        printf("Ведите должность (255 символов): ");
        if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(temp_sub->status.college.position, buffer, MAX_STRING_LENGTH);

        // Чтение организации
        printf("Ведите организацию (255 символов): ");
        if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(temp_sub->status.college.organization, buffer, MAX_STRING_LENGTH);
        temp_sub->kind_sub = COLLEGE;
    } 
    else 
    {
        printf("Error: Unknown sub type: %s\n", buffer);
        return NULL;
    }
    temp_sub->id = sub_count;
    return temp_sub;
}

// Функция для считывания данных из файла
int read_subs_from_file(const char* filename, sub_t subs[], size_t *subs_count, key_t keys[])
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        perror("Error opening file");
        return -1;
    }

    size_t sub_count = 0;
    while (sub_count < MAX_SUB) 
    {
        sub_t temp_sub;
        if (read_sub(file, sub_count, &temp_sub) == NULL)
        {
           break; 
        }
        subs[sub_count] = temp_sub;
        keys[sub_count].id = temp_sub.id;
        if (temp_sub.kind_sub == FRIEND)
        {
            keys[sub_count].number = temp_sub.status.friend.user.number;
        }
        else
        {
            keys[sub_count].number = temp_sub.status.college.user.number;
        }
        sub_count++;
    }
    *subs_count = sub_count;
    fclose(file);
    return 0;
}

void write_into_file(const char* filename, sub_t subs[], size_t subs_count)
{
    FILE* file = fopen(filename, "w");
    for (size_t i = 0; i < subs_count; i++)
    {
        if (subs[i].kind_sub == FRIEND)
        {
            fprintf(file, "%s\n%s\n%lld\n%s\n%s\nFRIEND\n%d\n%d\n%d\n",
                    subs[i].status.friend.user.surname, subs[i].status.friend.user.name, subs[i].status.friend.user.number, 
                    subs[i].status.friend.user.adress.street, subs[i].status.friend.user.adress.house,
                    subs[i].status.friend.date_birth.day, subs[i].status.friend.date_birth.month, subs[i].status.friend.date_birth.year);
        }
        else if (subs[i].kind_sub == COLLEGE)
        {
            fprintf(file, "%s\n%s\n%lld\n%s\n%s\nCOLLEGE\n%s\n%s\n", 
                    subs[i].status.college.user.surname, subs[i].status.college.user.name, subs[i].status.college.user.number, 
                    subs[i].status.college.user.adress.street, subs[i].status.college.user.adress.house,
                    subs[i].status.college.position, subs[i].status.college.organization);
        }
        else
        {
            printf("Unknown kind\n");
        }
    }   
}

void show_entry_message(void)
{
    printf("Программа предоставляет возможности работы с таблицей и проведения сравнений сортировок\n"
           "Сортировка проходит по полю number\n");
}

void show_menu(void)
{
    printf("1 - Показать таблицу\n"
        "2 - Показать таблицу ключей\n"
        "3 - Показать таблицу в соответствии с таблицей ключей\n"
        "4 - Добавить элемент в конец\n"
        "5 - Удалить элемент по id\n"
        "6 - Вывод будущих дней рождений\n"
        "7 - Сортировать таблицу по ключам\n"
        "8 - Сортировать таблицу (и ключи)\n"
        "9 - Провести сравнение сортировок\n"
        "0 - Выход\n");
    printf("Действие #: ");
}

void show_table(sub_t subs[], size_t size)
{
    printf("id|Surname|Name|Number|Street|House|Status|Date of birth|Position|Organization\n");
    for (int i = 0; i < size; i++) 
    {
        if (subs[i].kind_sub == FRIEND)
        {
            print_friend(subs[i]);
        }
        else if (subs[i].kind_sub == COLLEGE)
        {
            print_colleague(subs[i]);
        }
        else
        {
            printf("Unknown kind\n");
        }   
    }
}

void show_key_table(key_t keys[], size_t size)
{
    printf("ind|id|number\n");
    for (size_t i = 0; i < size; i++)
    {
        printf("%zu|%u|%lld\n", i, keys[i].id, keys[i].number);
    }
}

void show_table_by_key(sub_t subs[], key_t keys[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (subs[keys[i].id].kind_sub == FRIEND)
        {
            print_friend(subs[keys[i].id]);
        }
        else
        {
            print_colleague(subs[keys[i].id]);
        }
    }
}

void print_friend(sub_t sub)
{
    printf("%u|%s|%s|%lld|%s|%s|Friend|%d.%d.%d|---|---|\n", 
            sub.id, sub.status.friend.user.surname, sub.status.friend.user.name, sub.status.friend.user.number, 
            sub.status.friend.user.adress.street, sub.status.friend.user.adress.house,
            sub.status.friend.date_birth.year, sub.status.friend.date_birth.month, sub.status.friend.date_birth.day);
}

void print_colleague(sub_t sub)
{
    printf("%u|%s|%s|%lld|%s|%s|Colleague|---|%s|%s|\n", 
            sub.id, sub.status.friend.user.surname, sub.status.friend.user.name, sub.status.friend.user.number,
            sub.status.college.user.adress.street, sub.status.college.user.adress.house,
            sub.status.college.position, sub.status.college.organization);
}

int is_number(char *string_in)
{
    unsigned i = 0;
    
    // Проходим по строке
    while (string_in[i] != '\0')  // Пока не достигли конца строки
    {
        if (!isdigit(string_in[i]) && !isspace(string_in[i]))  // Если не цифра и не пробел
        {
            printf("%s Not num\n", string_in);
            return 0;  // Возвращаем ложь
        }
        i++;  // Переходим к следующему символу
    }
    
    return 1;  // Если все символы - цифры, возвращаем истину
}