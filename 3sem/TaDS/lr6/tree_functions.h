#ifndef __TREE_FUNCTIONS_H__
#define __TREE_FUNCTIONS_H__

tree_t *init_tree();
void free_tree(tree_t *tree);

void add_node(tree_t *tree, char *filename);
void delete_node(tree_t *tree);
vertex_t *node_delete(vertex_t *root, int value);

void tree_insert(tree_t *tree, int value);

int calculate_size(vertex_t *node);
int calculate_height(vertex_t *node);

#endif // __TREE_FUNCTIONS_H__