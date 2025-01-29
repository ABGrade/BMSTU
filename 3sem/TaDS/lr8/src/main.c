#include <stdio.h>

#include "io.h"
#include "graph_functions.h"
#include "types.h"

int main()
{
    int option;
    graph_t graph;
    read_graph_from_file(&graph, FILEIN);

    entry_message();
    do
    {
        menu();
        option = get_option();
        action(option, &graph);
    } while (option);
    free_graph(&graph); 
    return 0;
}