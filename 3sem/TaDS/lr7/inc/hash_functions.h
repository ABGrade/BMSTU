#ifndef __HASH_FUNCTIONS_H__
#define __HASH_FUNCTIONS_H__

#include "hash_types.h"

#define NOT_FOUND -1
#define NEED_RESIZE -2

int insert_chain(hash_chain *ht, int key, char verbose);
int search_chain(hash_chain* ht, int key);
int delete_chain(hash_chain* ht, int key);
void resize_chain(hash_chain* ht, unsigned int new_size);

int insert_open(hash_open *ht, int key, char verbose);
int search_open(hash_open* ht, int key);
int delete_open(hash_open* ht, int key, char verbose);
void resize_open(hash_open* ht, unsigned int new_size);

void show_open_hash_table(hash_open open);
void show_chain_hash_table(hash_chain chain);

void clear_chain(hash_chain* ht);
void clear_open(hash_open* ht);

void change_hash_consts(hash_open *open, hash_chain *chain);
void change_table_size(int is_chain_change);
void show_hash_status();

unsigned int next_prime(unsigned int start);

#endif // __HASH_FUNCTIONS_H__