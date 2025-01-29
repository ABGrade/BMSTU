#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "io.h"
#include "tree_functions.h"


vertex_t *rotate_right(vertex_t *y) 
{
    vertex_t *x = y->left;
    vertex_t *T = x->right;

    // Выполняем поворот
    x->right = y;
    y->left = T;

    // Обновляем баланс и высоту узлов
    update_balance(y);
    update_balance(x);

    return x;
}

// Функция для левого поворота
vertex_t *rotate_left(vertex_t *x) 
{
    vertex_t *y = x->right;
    vertex_t *T = y->left;

    // Выполняем поворот
    y->left = x;
    x->right = T;

    // Обновляем баланс и высоту узлов
    update_balance(x);
    update_balance(y);

    return y;
}

vertex_t *balance_node(vertex_t *node)
{
    update_balance(node);

    // Выполняем балансировку текущего узла, если требуется
    if (node->bal > 1) 
    {
        if (node->right && node->right->bal < 0)
            node->right = rotate_right(node->right);
        node = rotate_left(node);
    }
    else if (node->bal < -1) 
    {
        if (node->left && node->left->bal > 0)
            node->left = rotate_left(node->left);
        node = rotate_right(node);
    }

    update_balance(node);
    return node;
}

// Функция рекурсивной балансировки всего дерева
vertex_t *balance_recursive(vertex_t *node) 
{
    if (!node)
        return NULL;

    // Рекурсивно балансируем поддеревья
    node->right = balance_recursive(node->right);
    node = balance_node(node);
    update_balance(node);
    
    node->left = balance_recursive(node->left);
    node = balance_node(node);
    update_balance(node);

    return node;
}

tree_t *balance_tree(tree_t *tree) 
{
    if (tree && tree->root)
        tree->root = balance_recursive(tree->root);
    return tree;
}

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
    node->bal = 0;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void update_balance(vertex_t *node)
{
    if (node)
    {
        int left_height = node->left ? calculate_height(node->left) : 0;
        int right_height = node->right ? calculate_height(node->right) : 0;
        node->height = 1 + (left_height > right_height ? left_height : right_height);
        node->bal = right_height - left_height;
    }
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

    node->height = calculate_height(node);
    update_balance(node);
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

void tree_balance_insert(tree_t *tree, int value)
{
    if (!tree)
        return; 

    tree->root = node_insert(tree->root, value);
    tree->size = calculate_size(tree->root);
    tree->height = calculate_height(tree->root);
    tree = balance_tree(tree);
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

    root->height = calculate_height(root);
    update_balance(root);
    return root;
}


void tree_delete(tree_t *tree, int value)
{
    tree->root = node_delete(tree->root, value);  
    tree->size = calculate_size(tree->root);
    tree->height = calculate_height(tree->root);
}

void tree_balance_delete(tree_t *tree, int value)
{
    tree->root = node_delete(tree->root, value);  
    tree->size = calculate_size(tree->root);
    tree->height = calculate_height(tree->root);
    tree = balance_tree(tree);
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