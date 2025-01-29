#ifndef __HASH_TYPES_H__
#define __HASH_TYPES_H__

extern unsigned int TABLE_SIZE_CHAIN;
extern unsigned int TABLE_SIZE_OPEN;
extern unsigned int HASH_PERFOMANCE_LIMIT_OPEN;
extern unsigned int HASH_PERFOMANCE_LIMIT_CHAIN;

// Закрытая адресация (цепочки)
typedef struct hash_node 
{
    int key;
    struct hash_node *next;
} node_t;

typedef struct 
{
    node_t** table;
    unsigned int size;
} hash_chain;

typedef enum
{
    FREE,
    OCCUPIED,
    DELETED
} node_status_t;

// Открытая адресация
typedef struct 
{
    int** table;
    node_status_t *status;
    unsigned int size;
} hash_open;

void init_chain(hash_chain* ht, unsigned int size);
void init_open(hash_open* ht, unsigned int size);

#endif // __HASH_TYPES_H__