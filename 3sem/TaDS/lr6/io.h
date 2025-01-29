#ifndef __IO_H__
#define __IO_H__

#define READ_TREE 1
#define ADD_NODE 2
#define DELETE_NODE 3
#define FIND 4
#define SHOW 5
#define BYPASS 6
#define SORT 7
#define TESTING 8
#define EXIT 0

#define BASE_STR_SIZE 16

#define FILEIN "tree.txt"
#define FILEOUT "tree.dot"

void action(int option, tree_t *tree);
int get_option(void);

int read_int(void);
char *read_line_dynamic(FILE *file);

void clear_input_buffer(void);
int my_max(int a, int b);

void entry_message(void);
void menu(void);

void write_into_file(int value, char *filename);
void write_into_file_multiple(int *values, int size, char *filename);

#endif // __IO_H__