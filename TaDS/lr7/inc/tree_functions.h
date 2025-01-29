#ifndef __TREE_FUNCTIONS_H__
#define __TREE_FUNCTIONS_H__

#include "types.h"

tree_t *balance_tree(tree_t *tree);

tree_t *init_tree();
void free_tree(tree_t *tree);

vertex_t *node_delete(vertex_t *root, int value);

void tree_insert(tree_t *tree, int value);
void tree_balance_insert(tree_t *tree, int value);

void tree_delete(tree_t *tree, int value);
void tree_balance_delete(tree_t *tree, int value);

int calculate_size(vertex_t *node);
int calculate_height(vertex_t *node);
void update_balance(vertex_t *node);

#endif // __TREE_FUNCTIONS_H__