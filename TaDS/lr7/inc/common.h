#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include "types.h"
#include "hash_functions.h"

void read_all(tree_t *tree, hash_chain *chain, hash_open *open, char *filename);
void add_all(tree_t *tree, hash_chain *chain, hash_open *open, char *filename);
void delete_all(tree_t *tree, hash_chain *chain, hash_open *open);
void find_all(tree_t *tree, hash_chain *chain, hash_open *open);

#endif // __COMMON_H__