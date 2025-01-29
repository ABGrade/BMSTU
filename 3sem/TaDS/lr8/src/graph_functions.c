#include <stdio.h>
#include <stdlib.h>

#include "graph_functions.h"
#include "types.h"

void init_graph(graph_t *graph, int vertices)
{
    graph->vertices = vertices;
    graph->data = malloc(vertices * sizeof(int *));
    if (graph->data == NULL)
    {
        printf("Ошибка выделения памяти\n");
        return;
    }

    for (int i = 0; i < vertices; i++)
    {
        graph->data[i] = calloc(vertices, sizeof(int));
        if (graph->data[i] == NULL)
        {
            printf("Ошибка выделения памяти\n");
            return;
        }
    }
}

void free_graph(graph_t *graph)
{
    for (int i = 0; i < graph->vertices; i++)
    {
        free(graph->data[i]);
    }
    free(graph->data);
}

void read_graph_from_file(graph_t *graph, char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Ошибка открытия файла\n");
        return;
    }

    int vertices;
    if (fscanf(file, "%d", &vertices) != 1)
    {
        printf("Ошибка чтения файла\n");
        fclose(file);
        return;
    }

    if (vertices <= 0)
    {
        printf("Некорректные данные в файле\n");
        fclose(file);
        return;
    }

    init_graph(graph, vertices);

    int from, to, weight;
    while (fscanf(file, "%d %d %d", &from, &to, &weight) == 3)
    {
        graph->data[from][to] = weight;
    }

    fclose(file);
}

void print_graph(graph_t *graph)
{
    for (int i = 0; i < graph->vertices; i++)
    {
        for (int j = 0; j < graph->vertices; j++)
        {
            printf("%d ", graph->data[i][j]);
        }
        printf("\n");
    }
}

void save_graph_to_dot(graph_t *graph, char *filename)
{
    FILE *file = fopen("graph.dot", "w");
    if (!file)
    {
        printf("Невозможно открыть файл\n");
        return;
    }

    fprintf(file, "digraph G {\n");
    for (int i = 0; i < graph->vertices; i++)
    {
        for (int j = 0; j < graph->vertices; j++)
        {
            if (graph->data[i][j] != 0)
            {
                fprintf(file, "  %d -> %d [label=\"%d\"];\n", i, j, graph->data[i][j]);
            }
        }
    }
    fprintf(file, "}\n");
    fclose(file);
}

void dfs(const graph_t *graph, int vertex, char *visited)
{
    visited[vertex] = 1;

    for (int i = 0; i < graph->vertices; i++)
    {
        if (graph->data[vertex][i] != 0 && !visited[i])
            dfs(graph, i, visited);
    }
}

void find_unreachable(graph_t *graph, int start)
{
    int count = 0;
    char *unreachable = (char *)calloc(graph->vertices, sizeof(char));
    char *visited = (char *)calloc(graph->vertices, sizeof(char));

    dfs(graph, start, visited);

    for (int i = 0; i < graph->vertices; i++)
        unreachable[i] = !visited[i];

    printf("Недостижимые вершины из данной: ");
    for (int i = 0; i < graph->vertices; i++)
    {
        if (unreachable[i])
        {
            printf("%d ", i);
            count++;
        }
    }

    if (count == 0)
    {
        printf("отвутствуют");
    }
    printf("\n");

    free(visited);
    free(unreachable);
}