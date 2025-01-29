#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "io.h"
#include "tree_functions.h"
#include "tree_complex_functions.h"

// Функция для рекурсивного вывода узлов дерева в формате DOT
void generate_dot(vertex_t *root, FILE *file) 
{
    if (root == NULL)
        return;

    // Для каждого узла выводим его значение
    if (root->left) 
    {
        fprintf(file, "    %d -> %d;\n", root->value, root->left->value);
        generate_dot(root->left, file);
    }
    if (root->right) 
    {
        fprintf(file, "    %d -> %d;\n", root->value, root->right->value);
        generate_dot(root->right, file);
    }
}

// Функция для построения файла DOT для дерева
void export_tree_to_dot(vertex_t *root, char *filename) 
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) 
    {
        perror("Ошибка открытия файла");
        return;
    }

    // Начало графа
    fprintf(file, "digraph BinaryTree {\n");
    fprintf(file, "    node [fontname=\"Arial\", shape=circle];\n");

    // Если дерево пустое
    if (root == NULL) 
        fprintf(file, "    EmptyTree;\n");
    else 
        generate_dot(root, file); // Рекурсивно добавляем узлы

    // Конец графа
    fprintf(file, "}\n");
    fclose(file);

    printf("Дерево экспортировано в файл %s.\n", filename);
}

void find(vertex_t *root, int value, int steps)
{
    if (root == NULL)
    {
        printf("Узел не найден за %d шаг(а)\n", steps);
        return;
    }

    steps++;

    if (root->value == value)
    {
        printf("Узел найден за %d шаг(ов)\n", steps);
        return;
    }
    else if (value < root->value)
    {
        find(root->left, value, steps);
    }
    else
    {
        find(root->right, value, steps);
    }
}


void read_tree(tree_t *tree, char *filename) 
{
    int value;
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        printf("Файл не может быть открыт\n");
        return;
    }

    while (fscanf(file, "%d", &value) == 1) 
    {
        tree_insert(tree, value);
    }

    fclose(file);
    printf("Успешно прочитано.\n");
}

int bypass(vertex_t *root, int *size, char verbose)
{
    if (root == NULL)
        return 0;

    (*size)++;

    int left = bypass(root->left, size, verbose);
    int right = bypass(root->right, size, verbose);

    if (verbose)
        printf("%d ", root->value);
    
    return 1 + my_max(left, right);
}

void travel(tree_t *tree, char verbose_base, char verbose_all)
{
    if (!tree)
        return;
    
    tree->size = 0;
    tree->height = bypass(tree->root, &tree->size, verbose_all);
    printf("\n");
    if (verbose_base)
        printf("Размер: %d, высота: %d\n", tree->size, tree->height);
}

void count_nodes_per_level(vertex_t *root, int *level_count, int level)
{
    if (root == NULL)
        return;

    level_count[level]++; // Увеличиваем количество узлов на текущем уровне

    count_nodes_per_level(root->left, level_count, level + 1);
    count_nodes_per_level(root->right, level_count, level + 1);
}

void levels(tree_t *tree)
{
    if (!tree)
        return;

    int *level_count = (int *)calloc(tree->height, sizeof(int));

    count_nodes_per_level(tree->root, level_count, 0);

    printf("Количество узлов на каждом уровне:\n");
    for (int i = 0; i < tree->height; i++) 
    {
        printf("Уровень %d: %d узел(ов)\n", i, level_count[i]);
    }

    free(level_count);
}

void print_sorted(vertex_t *root)
{
    if (root == NULL)
        return;
    
    print_sorted(root->left);

    printf("%d ", root->value);

    print_sorted(root->right);
    
}

void sort(tree_t *tree)
{
    if (!tree || tree->height == 0)
    {
        printf("Дерево пусто\n");
        return;
    }
    printf("Элементы дерева в отсортированном виде:\n");
    print_sorted(tree->root);
}