#include <stdio.h>

#include "types.h"

#include "io.h"
#include "arr_func.h"
#include "list_func.h"
#include "modeling.h"
#include "measure.h"

extern double t1b, t1e;
extern double t2b, t2e;
extern double t3b, t3e;

void action(int option, arr_queue_t *arr_queue, list_queue_t *list_queue)
{
    switch (option)
    {
        case CREATE_QUEUE_ARR:
            printf("Создаем очередь...\n");
            arr_pushs(arr_queue);
            break;

        case CREATE_QUEUE_LIST:
            printf("Создаем очередь...\n");
            list_pushs(list_queue);
            break;

        case DEL_ELEMS_ARR:
            printf("Удаляем... \n");
            arr_pops(arr_queue);
            break;

        case DEL_ELEMS_LIST:
            printf("Удаляем... \n");
            list_pops(list_queue);
            break;

        case SHOW_STATE_ARR:
            show_arr_queue(*arr_queue);
            break;

        case SHOW_STATE_LIST:
            show_list_queue(*list_queue);
            break;

        case MODELING_ARRAY:
            simulate_service_system_array(1);
            break;

        case MODELING_LIST:
            simulate_service_system_list(1);
            break;

        case UPDATE_TIME:
            change_time();
            break;

        case SPEEDTEST:
            measure();
            break;

        case EXIT:
            printf("Выходим...\n");
            break;
        
        default:
            printf("Неизвестный выбор, повторите попытку\n");
            break;
    }
}

int get_option(void)
{
    int option;
    printf("Опция: ");
    while (scanf("%d", &option) != 1)
    {
        printf("Повторите попытку ввода: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    return option;
}

unsigned get_unsigned(void)
{
    int num = 0;
    while (!scanf("%d", &num) || num < 1)
    {
        printf("Неверный ввод\n");
    }
    clear_input_buffer();
    return (unsigned)num;
}

void change_time(void) 
{
    printf("Введите новые значения для времени (t1b t1e t2b t2e t3b t3e): ");
    while (scanf("%lf %lf %lf %lf %lf %lf", &t1b, &t1e, &t2b, &t2e, &t3b, &t3e) != 6) 
    {
        printf("Неправильный ввод.\n");
    } 
    printf("Обновленные значения: t1b=%lf, t1e=%lf, t2b=%lf, t2e=%lf, t3b=%lf, t3e=%lf\n", 
            t1b, t1e, t2b, t2e, t3b, t3e);
}

void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void show_arr_queue(arr_queue_t queue)
{
    if (queue.length == 0)
    {
        printf("Очередь еще пуста.\n");
        return;
    }
    
    for (unsigned i = 0; i < queue.length; i++)
    {
        unsigned p = (queue.out + i + MAX_QUEUE_LEN) % MAX_QUEUE_LEN;
        printf("ind#%u %p\n", p, &queue.array[p]);
    }
    printf("Занятая память: %zu байт\n", sizeof(arr_queue_t));
}

void show_list_queue(list_queue_t queue)
{
    if (queue.length == 0)
    {
        printf("Очередь еще пуста.\n");
        return;
    }

    queue_item_t *p = queue.head;
    for (unsigned i = 0; i < queue.length; i++)
    {
        printf("#%u %p\n", i, p);
        p = p->prev;
    }

    deleted_node_t *d = queue.deleted;
    if (d)
    {
        printf("Удаленные адреса:\n");
        while (d)
        {
            printf("%p\n", d);
            d = d->next;
        }
    }
    printf("Занятая память: %zu байт\n", sizeof(queue_item_t) * queue.length);
}

void entry_message(void)
{
    printf("Программа реализует систему массового обслуживания заявок с представлением типа данных - очередь\n");
}

void menu(void)
{
    printf("\n\t Выберите опцию:\n\n");
    printf("%d. Добавление в очередь (массивом)\n", CREATE_QUEUE_ARR);
    printf("%d. Добавление в очередь (списком)\n", CREATE_QUEUE_LIST);
    printf("%d. Удаление из очереди (массивом)\n", DEL_ELEMS_ARR);
    printf("%d. Удаление из очереди (списком)\n", DEL_ELEMS_LIST);
    printf("%d. Показать состояние очереди (массивом)\n", SHOW_STATE_ARR);
    printf("%d. Показать состояние очереди (списком)\n", SHOW_STATE_LIST);
    printf("%d. Моделирование (массивом)\n", MODELING_ARRAY);
    printf("%d. Моделирование (списком)\n", MODELING_LIST);
    printf("%d. Настроить время для моделирования\n", UPDATE_TIME);
    printf("%d. Тест скорости\n", SPEEDTEST);
    printf("%d. Выход\n", EXIT);
}