#ifndef __IO_H__
#define __IO_H__

#include "types.h"

#define DOT 1
#define CONNECTIONS 2
#define UNREACHABLE 3  
#define EXIT 0

#define FILEIN "graph.txt"
#define FILEDOT "graph.dot"

void action(int option, graph_t* graph);

void menu(void);
void entry_message();
int get_option(void);

#endif // __IO_H__