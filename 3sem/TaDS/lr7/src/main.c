#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// #include <windows.h>

#include "types.h"
#include "tree_functions.h"
#include "io.h"
#include "hash_functions.h"
#include "hash_types.h"

int main(void)
{
    int option;
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
    srand(time(NULL));

    tree_t *tree = init_tree();

    hash_chain chain;
    init_chain(&chain, TABLE_SIZE_CHAIN);

    hash_open open;
    init_open(&open, TABLE_SIZE_OPEN);

    if (!tree)
    {
        printf("Ошибка при инициализации дерева\n");
        return -1;
    }

    entry_message();
    do
    {
        menu();
        option = get_option();
        action(option, tree, &chain, &open);
    } while (option);

    free_tree(tree);
    return 0;
}