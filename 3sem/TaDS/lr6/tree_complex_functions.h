#ifndef __TREE_COMPLEX_FUNCTIONS_H__
#define __TREE_COMPLEX_FUNCTIONS_H__

#include "types.h"

void read_tree(tree_t *tree, char *filename);
void export_tree_to_dot(vertex_t *root, char *filename);

void travel(tree_t *tree, char verbose_base, char verbose_all);
void levels(tree_t *tree);
void find(vertex_t *root, int value, int steps);
void sort(tree_t *tree);

#endif // __TREE_COMPLEX_FUNCTIONS_H__