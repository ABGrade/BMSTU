#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_functions.h"
#include "hash_types.h"
#include "io.h"

unsigned int TABLE_SIZE_CHAIN = 23;
unsigned int TABLE_SIZE_OPEN = 23;
unsigned int HASH_PERFOMANCE_LIMIT_OPEN = 5;
unsigned int HASH_PERFOMANCE_LIMIT_CHAIN = 5;

// Хэш-функция
unsigned int hash(int key, unsigned int table_size)
{
    return (unsigned int)(key % table_size);
}

// Инициализация хеш-таблицы с цепочками
void init_chain(hash_chain *ht, unsigned int size)
{
    ht->size = size;
    ht->table = (node_t **)calloc(size, sizeof(node_t *));
    if (ht->table == NULL)
    {
        printf("Ошибка выделения памяти\n");
        return;
    }
}

// Добавление в хеш-таблицу с цепочками
int insert_chain(hash_chain *ht, int key, char verbose)
{
    int count = 1;
    unsigned int index = hash(key, TABLE_SIZE_CHAIN);

    int is_exist = search_chain(ht, key);
    if (is_exist > 0)
    {
        if (verbose)
        {
            printf("Элемент %d уже существует\n", key);
        }
        return -1;
    }
    

    node_t *new = (node_t *)malloc(sizeof(node_t));
    if (!new)
    {
        printf("Ошибка выделения памяти\n");
        return 0;
    }

    new->key = key;
    new->next = NULL;

    if (ht->table[index] == NULL)
    {
        ht->table[index] = new;
    }
    else
    {
        node_t *current = ht->table[index];
        count++;
        while (current->next != NULL)
        {
            current = current->next;
            count++;
        }
        current->next = new;
    }
    return count;
}

// Поиск в хеш-таблице с цепочками
int search_chain(hash_chain *ht, int key)
{
    int count = 1;
    unsigned int index = hash(key, TABLE_SIZE_CHAIN);
    node_t *current = ht->table[index];

    while (current != NULL)
    {
        if (current->key == key)
            return count; // Элемент найден, возвращаем количество шагов

        current = current->next;
        count++;

        // Проверяем превышение лимита производительности
        if (count > HASH_PERFOMANCE_LIMIT_CHAIN)
            return NEED_RESIZE; // Сигнал для рехэширования
    }

    return NOT_FOUND; // Элемент не найден
}


// Удаление из хеш-таблицы с цепочками
int delete_chain(hash_chain *ht, int key)
{
    int count = 1;
    unsigned int index = hash(key, TABLE_SIZE_CHAIN);
    node_t *current = ht->table[index];
    node_t *prev = NULL;
    while (current != NULL && current->key != key)
    {
        prev = current;
        current = current->next;
        count++;
    }
    if (current == NULL)
        return NOT_FOUND; // Не найдено

    if (prev == NULL)
        ht->table[index] = current->next;
    else
        prev->next = current->next;
    free(current);
    return count;
}

void resize_chain(hash_chain *ht, unsigned int new_size)
{
    int need_resize = 1;
    while (need_resize)
    {
        int need_resize = 0;
        hash_chain new_ht;
        init_chain(&new_ht, new_size);

        for (unsigned int i = 0; i < ht->size; i++)
        {
            if (ht->table[i] != NULL)
            {
                node_t *current = ht->table[i];
                while (current)
                {
                    int count = insert_chain(&new_ht, current->key, 1);
                    if (count > HASH_PERFOMANCE_LIMIT_CHAIN)
                    {
                        need_resize = 1;
                    }
                    current = current->next;

                    // Если необходимо увеличить таблицу, можно завершить текущий цикл
                    if (need_resize)
                        break;
                }
            }
            // Если требуется увеличить таблицу, можно выйти из внешнего цикла
            if (need_resize)
                break;
        }

        // Если требуется увеличивать размер, повторить операцию
        if (need_resize)
        {
            new_size = next_prime(new_size);
            TABLE_SIZE_CHAIN = new_size;
            continue;
        }

        free(ht->table);
        *ht = new_ht;
        break;
    }
}

// Инициализация хеш-таблицы с открытой адресацией
void init_open(hash_open *ht, unsigned int size)
{
    ht->size = size;
    ht->table = (int **)calloc(size, sizeof(int *));
    ht->status = (node_status_t *)calloc(size, sizeof(node_status_t));

    if (ht->table == NULL || ht->status == NULL)
    {
        printf("Ошибка выделения памяти\n");
        return;
    }

    for (unsigned int i = 0; i < size; i++)
    {
        ht->status[i] = FREE;
    }
}

// Добавление в хеш-таблицу с открытой адресацией
int insert_open(hash_open *ht, int key, char verbose)
{
    if (search_open(ht, key) > 0)
    {
        if (verbose)
        {
            printf("Элемент %d уже существует.\n", key);
        }
        return -1;
    }
    
    int count = 1;
    int idx = hash(key, TABLE_SIZE_OPEN);
    int start_idx = idx;
    while (ht->status[idx] == OCCUPIED)
    {
        idx = (idx + 1) % TABLE_SIZE_OPEN;
        if (idx == start_idx)
        {
            change_table_size(0);
            resize_open(ht, TABLE_SIZE_OPEN);
            count = insert_open(ht, key, 1);
            return count;
        }
        count++;
    }

    if (ht->status[idx] == FREE || ht->status[idx] == DELETED)
    {
        ht->table[idx] = malloc(sizeof(int));
        if (ht->table[idx] == NULL)
        {
            printf("Ошибка выделения памяти");
            return count;
        }
        *(ht->table[idx]) = key;
        ht->status[idx] = OCCUPIED;
    }
    return count;
}

// Поиск в хеш-таблице с открытой адресацией
int search_open(hash_open *ht, int key)
{
    int count = 1;
    int idx = hash(key, TABLE_SIZE_OPEN);
    int start_idx = idx;
    while (ht->status[idx] != FREE)
    {
        if (ht->status[idx] == OCCUPIED && *(ht->table[idx]) == key)
            return count;
        idx = (idx + 1) % TABLE_SIZE_OPEN;
        count++;
        if (idx == start_idx)
            break;

        if (count > HASH_PERFOMANCE_LIMIT_OPEN)
            return NEED_RESIZE;
    }
    return NOT_FOUND; // Не найдено
}

// Удаление из хеш-таблицы с открытой адресацией
int delete_open(hash_open *ht, int key, char verbose)
{
    int count = 1;
    unsigned int idx = hash(key, TABLE_SIZE_OPEN);
    unsigned int start_idx = idx;
    while (ht->status[idx] != FREE)
    {
        if (ht->status[idx] == OCCUPIED && *(ht->table[idx]) == key)
        {
            free(ht->table[idx]);
            ht->table[idx] = NULL;
            ht->status[idx] = DELETED;
            if (verbose)
            {
                printf("Ячейка со значением %d удалена.\n", key);
            }
            return count;
        }
        idx = (idx + 1) % TABLE_SIZE_OPEN;
        if (idx == start_idx)
            break;
        count++;
    }
    if (verbose)
    {
        printf("Значение %d не найдено.\n", key);
    }
    return -1;
}

void resize_open(hash_open *ht, unsigned int new_size)
{
    int need_resize = 1;
    while (need_resize)
    {
        need_resize = 0;
        hash_open new_ht;
        init_open(&new_ht, new_size);

        for (unsigned int i = 0; i < ht->size; i++)
        {
            if (ht->table[i] != NULL)
            {
                int count = insert_open(&new_ht, *(ht->table[i]), 1);
                if (count > HASH_PERFOMANCE_LIMIT_OPEN)
                {
                    need_resize = 1;
                }
                // Если необходимо увеличить таблицу, можно завершить текущий цикл
                if (need_resize)
                    break;
            }
        }

        // Если требуется увеличивать размер, повторить операцию
        if (need_resize)
        {
            new_size = next_prime(new_size);
            TABLE_SIZE_OPEN = new_size;
            free(new_ht.table);
            continue;
        }

        clear_open(ht);
        *ht = new_ht;
        break;
    }
}


void show_open_hash_table(hash_open open)
{
    show_hash_status();
    printf("\n | Итоговый хэш-ключ | Значение | Начальный хэш-ключ |\n");
    for (size_t i = 0; i < TABLE_SIZE_OPEN; i++)
    {
        if (open.table[i] != NULL)
        {
            printf(" |%19zu|%10d|%20u|\n", i, *open.table[i], hash(*open.table[i], TABLE_SIZE_OPEN));
        }
    }
}

void show_chain_hash_table(hash_chain chain)
{
    show_hash_status();
    printf("\n | Хэш-ключ | Значения в данной ячейке \n");
    for (size_t i = 0; i < TABLE_SIZE_CHAIN; i++)
    {
        if (chain.table[i] != NULL)
        {
            printf(" |%10zu|", i);
            node_t *current = chain.table[i];
            while (current != NULL)
            {
                printf(" %7d", current->key);
                current = current->next;
            }
            printf("\n");
        }
    }
}

int is_simple(unsigned int n)
{
    if (n == 1)
        return 0;
    if (n == 2 || n == 3)
        return 1;
    if (n % 2 == 0 || n % 3 == 0)
        return 0;
    for (unsigned int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }
    return 1;
}

unsigned int next_prime(unsigned int start)
{
    unsigned int candidate = start + 1;
    while (!is_simple(candidate) && candidate > start)
    {
        candidate++;
    }
    if (candidate < start)
    {
        printf("Невозможно найти простое число больше %u\n", start);
        return start;
    }
    return candidate;
}

void show_hash_status()
{
    printf("\n\t Текущее состояние\n");
    printf("%u - ограничение цепочной хеш-таблицы\n", TABLE_SIZE_CHAIN);
    printf("%u - ограничение открытой хеш-таблицы\n", TABLE_SIZE_OPEN);
    printf("%u - лимит производительности открытой хеш-таблицы\n", HASH_PERFOMANCE_LIMIT_OPEN);
    printf("%u - лимит производительности цепочной хеш-таблицы\n", HASH_PERFOMANCE_LIMIT_CHAIN);
}

void change_hash_perfomance()
{
    int answer;
    printf("\nСмена производительности\n");
    printf("Меняем для открытой? (1/0): ");
    if (scanf("%d", &answer) == 1 && answer == 1)
    {
        printf("Введите предел производительности таблицы: ");
        HASH_PERFOMANCE_LIMIT_OPEN = read_uint();
    }
    printf("Меняем для цепочной? (1/0): ");
    if (scanf("%d", &answer) == 1 && answer == 1)
    {
        printf("Введите предел производительности таблицы: ");
        HASH_PERFOMANCE_LIMIT_CHAIN = read_uint();
    }
}

void change_table_size_verbose()
{
    int answer;
    printf("\nСмена размерности\n");
    printf("Меняем для открытой? (1/0): ");
    if (scanf("%d", &answer) == 1 && answer == 1)
    {
        TABLE_SIZE_OPEN = next_prime(TABLE_SIZE_OPEN); 
    }
    printf("Меняем для цепочной? (1/0): ");
    if (scanf("%d", &answer) == 1 && answer == 1)
    {
        TABLE_SIZE_CHAIN = next_prime(TABLE_SIZE_CHAIN);
    }
}

void change_table_size(int is_chain_change)
{
    if (is_chain_change)
    {
        TABLE_SIZE_CHAIN = next_prime(TABLE_SIZE_CHAIN);
    }
    else
    {
        TABLE_SIZE_OPEN = next_prime(TABLE_SIZE_OPEN);
    }
}

void change_hash_consts(hash_open *open, hash_chain *chain)
{
    unsigned int hash_open_prev_table_size = TABLE_SIZE_OPEN;
    unsigned int hash_open_prev_perfomance = HASH_PERFOMANCE_LIMIT_OPEN;
    unsigned int hash_chain_prev_table_size = TABLE_SIZE_CHAIN;
    unsigned int hash_chain_prev_perfomance = HASH_PERFOMANCE_LIMIT_CHAIN;
    change_table_size_verbose();
    change_hash_perfomance();
    if (hash_open_prev_table_size != TABLE_SIZE_OPEN || hash_open_prev_perfomance != HASH_PERFOMANCE_LIMIT_OPEN)
    {
        resize_open(open, TABLE_SIZE_OPEN);
    }
    if (hash_chain_prev_table_size != TABLE_SIZE_CHAIN || hash_chain_prev_perfomance != HASH_PERFOMANCE_LIMIT_CHAIN)
    {
        resize_chain(chain, TABLE_SIZE_CHAIN);
    }
    show_hash_status();
}


void clear_chain(hash_chain *ht)
{
    if (ht == NULL || ht->table == NULL)
        return;

    for (unsigned int i = 0; i < ht->size; i++)
    {
        node_t *current = ht->table[i];
        while (current != NULL)
        {
            node_t *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht->table);
    ht->table = NULL;
    ht->size = 0;
}

void clear_open(hash_open *ht)
{
    if (ht == NULL || ht->table == NULL)
        return;

    for (unsigned int i = 0; i < ht->size; i++)
    {
        if (ht->status[i] == OCCUPIED)
        {
            free(ht->table[i]);
        }
    }
    free(ht->table);
    free(ht->status);
}