#ifndef __GRAPH_FUNCTIONS_H__
#define __GRAPH_FUNCTIONS_H__

#include "types.h"

void read_graph_from_file(graph_t* graph, char* filename);
void save_graph_to_dot(graph_t *graph, char *filename);

void find_unreachable(graph_t *graph, int start);
void print_graph(graph_t* graph);

void free_graph(graph_t* graph);

#endif // __GRAPH_FUNCTIONS_H__