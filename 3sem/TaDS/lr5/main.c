#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// #include <windows.h>

#include "types.h"
#include "io.h"

int main(void)
{    
    int option;
    arr_queue_t arr_queue = {.length = 0, .in = 0, .out = 0};
    list_queue_t list_queue = {.head = NULL, .tail = NULL, .deleted = NULL, .length = 0};

    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8); 
    srand(time(NULL));

    entry_message();
    do
    {
        menu();
        option = get_option();
        action(option, &arr_queue, &list_queue);
    } while (option);
    return 0;
}