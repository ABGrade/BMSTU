#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// #include <windows.h>

#include "types.h"
#include "tree_functions.h"
#include "errors.h"
#include "io.h"

int main(void)
{  
    int option;
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8); 
    srand(time(NULL));  

    tree_t *tree = init_tree();
    if (!tree)
    {
        printf("Ошибка при инициализации дерева\n");
        return ALLOC_ERROR;
    }

    entry_message();
    do
    {
        menu();
        option = get_option();
        action(option, tree);
    } while (option);

    free_tree(tree);
    return 0;
}