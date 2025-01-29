#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "io.h"
#include "tree_functions.h"

tree_t *init_tree() 
{
    tree_t *tree = malloc(sizeof(tree_t));
    if (!tree) 
        return NULL;

    tree->root = NULL;
    tree->size = 0;
    tree->height = 0;
    return tree;
}

vertex_t *node_init(int value)
{
    vertex_t *node = malloc(sizeof(vertex_t));
    if (!node)
        return NULL;

    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

vertex_t *node_insert(vertex_t *node, int value)
{
    if (!node)
        return node_init(value);

    if (value < node->value)
        node->left = node_insert(node->left, value);
    else if (value > node->value)
        node->right = node_insert(node->right, value);
    else
        return node;

    return node;
}

void tree_insert(tree_t *tree, int value) 
{
    if (!tree)
        return; 

    tree->root = node_insert(tree->root, value);
    tree->size = calculate_size(tree->root);
    tree->height = calculate_height(tree->root);
}

void add_node(tree_t *tree, char *filename)
{
    int value;
    printf("Введите значение узла: ");
    value = read_int();
    tree_insert(tree, value);

    write_into_file(value, filename);
}

vertex_t *find_min(vertex_t *root) 
{
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

vertex_t *node_delete(vertex_t *root, int value) 
{
    if (root == NULL)
        return NULL; // Узел не найден

    if (value < root->value) 
        root->left = node_delete(root->left, value); // Ищем в левом поддереве
    else if (value > root->value)
        root->right = node_delete(root->right, value); // Ищем в правом поддереве
    else  // Узел найден
    {
        if (root->left == NULL && root->right == NULL) 
        {
            // Узел — лист
            free(root);
            return NULL;
        } 
        else if (root->left == NULL) 
        {
            // Один потомок справа
            vertex_t *temp = root->right;
            free(root);
            return temp;
        } 
        else if (root->right == NULL) 
        {
            // Один потомок слева
            vertex_t *temp = root->left;
            free(root);
            return temp;
        } 
        else 
        {
            // Два потомка
            vertex_t *min_right = find_min(root->right); // Находим минимум в правом поддереве
            root->value = min_right->value;             // Копируем его значение
            root->right = node_delete(root->right, min_right->value); // Удаляем найденный узел
        }
    }
    return root;
}

void delete_node(tree_t *tree)
{
    int value;
    printf("Введите значение узла: ");
    value = read_int();
    tree->root = node_delete(tree->root, value);  
}

int calculate_size(vertex_t *node) 
{
    if (!node)
        return 0;
    return 1 + calculate_size(node->left) + calculate_size(node->right);
}

int calculate_height(vertex_t *node) 
{
    if (!node)
        return 0;

    int left_height = calculate_height(node->left);
    int right_height = calculate_height(node->right);

    return 1 + (left_height > right_height ? left_height : right_height);
}

void free_nodes(vertex_t *node)
{
    if (!node)
        return;

    free_nodes(node->left);
    free_nodes(node->right);
    free(node);
}


void free_tree(tree_t *tree)
{
    if (!tree)
        return;

    free_nodes(tree->root);
    tree->root = NULL;
    tree->size = 0;
    tree->height = 0;
}