#ifndef __TYPES_H__
#define __TYPES_H__

// Структура узла дерева
typedef struct node 
{
    int value;               // Значение узла
    int bal;                 // Баланс вершины
    int height;              // Высота вершины
    struct node *left;       // Левый потомок
    struct node *right;      // Правый потомок
} vertex_t;

// Структура дерева
typedef struct
{
    vertex_t *root;          // Корень дерева
    int size;                // Количество узлов
    int height;              // Высота дерева
} tree_t;

#endif // __TYPES_H__