#include <stdio.h>
#include <stdlib.h>

#include "const_struct.h"
#include "io.h"
#include "functions.h"

int main() 
{
    sub_t subs[MAX_SUB];
    key_t keys[MAX_SUB];
    size_t sub_count = 0;

    // Вызов функции для чтения данных из файла
    if (read_subs_from_file("users.txt", subs, &sub_count, keys) == 0) 
    {
        int option;
        printf("Успешно прочитано %zu абонентов из файла.\n", sub_count);
        show_entry_message();
        show_menu();
        while (scanf("%d", &option) != 0 && option != 0)
        {
            clear_input_buffer();
            choose(option, subs, keys, &sub_count);
            show_menu();
        }
        write_into_file("users_out.txt", subs, sub_count);
    } 
    else 
    {
        printf("Невозможно прочитать файл.\n");
    }
    return 0;
}