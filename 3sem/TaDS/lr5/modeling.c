#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "types.h"
#include "arr_func.h"
#include "list_func.h"
#include "errors.h"

#define TIME_STEP 1e-3
#define INVERSE_TIME_STEP (1 / TIME_STEP)
#define MAX_DIST_FOR_SECOND 4

double t1b = 0, t1e = 5,
       t2b = 0, t2e = 4,
       t3b = 0, t3e = 4;

double generate_double(double min, double max) 
{
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

void generate_request(request_t *request, request_type_t type) 
{
    request->type = type;
    if (type == FIRST) 
    {
        request->coming = generate_double(t1b, t1e);
        request->processing = generate_double(t2b, t2e);
    } 
    else 
    {
        request->coming = generate_double(t3b, t3e);
        request->processing = generate_double(t3b, t3e);
    }
}


void simulate_service_system_array(char verbose) 
{
    arr_queue_t queue = { .in = 0, .out = 0, .length = 0 };

    double time = 0.0;
    double next_first_arrival = generate_double(t1b, t1e);
    double processing_end_time = -1;
    int total_first_requests = 0;
    int total_second_requests = 0;
    double idle_time = 0;

    // Для сбора статистики
    double total_first_intervals = 0.0;
    double total_queue_time = 0.0;
    int num_first_type_in = 0;
    int num_first_type_out = 0;

    double cumulative_queue_length = 0;
    int last_reported_count = 0;

    // Ожидаемое значение среднего интервала между заявками
    double expected_first_interval = (t1b + t1e) / 2;
    double expected_second_interval = (t2b + t2e) / 2;
    double expected_third_interval = (t3b + t3e) / 2;

    request_t new_request;
    generate_request(&new_request, SECOND);
    new_request.coming = time;
    if (arr_push(&queue, new_request) == QUEUE_OVERFLOW_ERROR)
    {
        printf("Очередь переполнена.\n");
        return;
    }

    while (num_first_type_out < 1000) 
    {
        // Приход заявки первого типа
        if (time >= next_first_arrival) 
        {
            request_t new_request;
            generate_request(&new_request, FIRST);
            new_request.coming = time;
            if (arr_push(&queue, new_request) == QUEUE_OVERFLOW_ERROR)
            {
                printf("\n\t Очередь переполнена. Обработка прервана\n");
                return;
            }
            total_first_requests++;
            num_first_type_in++;
            total_first_intervals += next_first_arrival - time;
            next_first_arrival = time + generate_double(t1b, t1e);
        }

        // Обслуживание текущей заявки
        if (processing_end_time < 0 && queue.length > 0) 
        {
            request_t *current_request = arr_pop(&queue);

            double wait_time = time - current_request->coming; // Время ожидания заявки в очереди
            total_queue_time += wait_time; // Суммируем время ожидания

            processing_end_time = time + current_request->processing;

            if (current_request->type == FIRST) 
            {
                num_first_type_out++;
            }
            else
            {
                request_t new_request;
                generate_request(&new_request, SECOND);
                new_request.coming = time;
                if (queue.length < MAX_DIST_FOR_SECOND)
                {
                    if (arr_push(&queue, new_request) == QUEUE_OVERFLOW_ERROR)
                    {
                        printf("\n\t Очередь переполнена. Обработка прервана\n");
                        return;
                    }
                }
                else
                {
                    if (arr_insert_at(&queue, new_request, 4) == QUEUE_OVERFLOW_ERROR)
                    {
                        printf("\n\t Очередь переполнена. Обработка прервана\n");
                        return;
                    }
                }
                total_second_requests++;
            }
        }

        // Завершение обслуживания
        if (processing_end_time >= 0 && time >= processing_end_time) 
        {
            processing_end_time = -1;
        }

        // Простои
        if (processing_end_time < 0 && queue.length == 0) 
        {
            idle_time += TIME_STEP;
        }

        // Накопление длины очереди
        cumulative_queue_length += queue.length;

        time += TIME_STEP;

        // Информирование каждые 100 запросов
        if (num_first_type_out % 100 == 0 && num_first_type_out > 0 && num_first_type_out != last_reported_count && verbose) 
        {
            last_reported_count = num_first_type_out;
            double average_queue_length = cumulative_queue_length / (time / TIME_STEP);
            double average_queue_time = total_queue_time / num_first_type_out;
            printf("\nСТАТИСТИКА ПОСЛЕ ОБСЛУЖИВАНИЯ %d ЗАЯВОК ПЕРВОГО ТИПА\n", num_first_type_out);
            printf("Текущая длина очереди: %u\n", queue.length);
            printf("Средняя длина очереди: %.2f\n", average_queue_length);
            printf("Количество вошедших заявок: %d\n", num_first_type_in);
            printf("Количество вышедших заявок: %d\n", num_first_type_out);
            printf("Среднее время пребывания в очереди: %.3f\n", average_queue_time);
        }
    }

    // Ожидаемое и фактическое время моделирования
    double expected_model_time = expected_first_interval > expected_second_interval && expected_first_interval > expected_third_interval ? 
                                expected_first_interval * num_first_type_out : 
                                expected_second_interval * num_first_type_out + expected_third_interval * (num_first_type_out / MAX_DIST_FOR_SECOND);
    double actual_model_time = time;

    // Процентное расхождение
    double time_discrepancy = fabs(actual_model_time - expected_model_time) / expected_model_time * 100;

    // Итоговая статистика
    if (verbose)
    {
        printf("\n\t ИТОГОВАЯ СТАТИСТИКА\n\n");
        printf("Значения временных промежутков: t1b=%.1lf, t1e=%.1lf, t2b=%.1lf, t2e=%.1lf, t3b=%.1lf, t3e=%.1lf\n", 
                t1b, t1e, t2b, t2e, t3b, t3e);
        printf("Общее время моделирования: %.3f\n", time);
        printf("Ожидаемое время моделирования: %.3f\n", expected_model_time);
        printf("Время простоя ОА: %lf\n", idle_time);
        printf("Количество вошедших заявок первого типа: %d\n", num_first_type_in);
        printf("Количество вышедших заявок первого типа: %d\n", num_first_type_out);
        printf("Количество обращений заявок второго типа: %d\n", total_second_requests);

        // Проверка на допустимость расхождения
        printf("\n\t ПРОЦЕНТ РАССОГЛАСОВАНИЯ\n");
        printf("Расхождение между расчетным и фактическим временем моделирования: %.2f%%\n", time_discrepancy);
    }
}

void simulate_service_system_list(char verbose) 
{
    list_queue_t queue = { .head = NULL, .tail = NULL, .deleted = NULL, .length = 0 };

    double time = 0.0;
    double next_first_arrival = generate_double(t1b, t1e);
    double processing_end_time = -1;
    int total_first_requests = 0;
    int total_second_requests = 0;
    double idle_time = 0;

    double total_first_intervals = 0.0;
    double total_queue_time = 0.0;
    int num_first_type_in = 0;
    int num_first_type_out = 0;

    double cumulative_queue_length = 0;
    int last_reported_count = 0;

    double expected_first_interval = (t1b + t1e) / 2;
    double expected_second_interval = (t2b + t2e) / 2;
    double expected_third_interval = (t3b + t3e) / 2;

    request_t new_request;
    generate_request(&new_request, SECOND);
    new_request.coming = time;
    list_push(&queue, new_request);

    while (num_first_type_out < 1000) 
    {
        // Приход заявки первого типа
        if (time >= next_first_arrival) 
        {
            request_t new_request;
            generate_request(&new_request, FIRST);
            new_request.coming = time;
            list_push(&queue, new_request);
            total_first_requests++;
            num_first_type_in++;
            total_first_intervals += next_first_arrival - time;
            next_first_arrival = time + generate_double(t1b, t1e);
        }

        // Обслуживание текущей заявки
        if (processing_end_time < 0 && queue.length > 0) 
        {
            request_t *current_request = &list_pop(&queue)->data;

            double wait_time = time - current_request->coming; // Время ожидания заявки в очереди
            total_queue_time += wait_time; // Суммируем время ожидания

            processing_end_time = time + current_request->processing;

            if (current_request->type == FIRST) 
            {
                num_first_type_out++;
            } 
            else 
            {
                request_t new_request;
                generate_request(&new_request, SECOND);
                new_request.coming = time;
                if (queue.length < MAX_DIST_FOR_SECOND) 
                {
                    list_push(&queue, new_request);
                } 
                else 
                {
                    list_insert_at(&queue, new_request, 4); // Добавляем в определённую позицию
                }
                total_second_requests++;
            }
        }

        // Завершение обслуживания
        if (processing_end_time >= 0 && time >= processing_end_time) 
        {
            processing_end_time = -1;
        }

        // Простои
        if (processing_end_time < 0 && queue.length == 0) 
        {
            idle_time += TIME_STEP;
        }

        // Накопление длины очереди
        cumulative_queue_length += queue.length;

        time += TIME_STEP;

        // Информирование каждые 100 запросов
        if (num_first_type_out % 100 == 0 && num_first_type_out > 0 && num_first_type_out != last_reported_count && verbose) 
        {
            last_reported_count = num_first_type_out;
            double average_queue_length = cumulative_queue_length / (time / TIME_STEP);
            double average_queue_time = total_queue_time / num_first_type_out;
            printf("\nСТАТИСТИКА ПОСЛЕ ОБСЛУЖИВАНИЯ %d ЗАЯВОК ПЕРВОГО ТИПА\n", num_first_type_out);
            printf("Текущая длина очереди: %u\n", queue.length);
            printf("Средняя длина очереди: %.2f\n", average_queue_length);
            printf("Количество вошедших заявок: %d\n", num_first_type_in);
            printf("Количество вышедших заявок: %d\n", num_first_type_out);
            printf("Среднее время пребывания в очереди: %.3f\n", average_queue_time);
        }
    }

    // Ожидаемое и фактическое время моделирования
    double expected_model_time = expected_first_interval > expected_second_interval && expected_first_interval > expected_third_interval ? 
                                expected_first_interval * num_first_type_out : 
                                expected_second_interval * num_first_type_out + expected_third_interval * (num_first_type_out / MAX_DIST_FOR_SECOND);    double actual_model_time = time;

    // Процентное расхождение
    double time_discrepancy = fabs(actual_model_time - expected_model_time) / expected_model_time * 100;

    // Итоговая статистика
    if (verbose)
    {
        printf("\n\t ИТОГОВАЯ СТАТИСТИКА\n\n");
        printf("Общее время моделирования: %.3f\n", time);
        printf("Ожидаемое время моделирования: %.3f\n", expected_model_time);
        printf("Время простоя ОА: %lf\n", idle_time);
        printf("Количество вошедших заявок первого типа: %d\n", num_first_type_in);
        printf("Количество вышедших заявок первого типа: %d\n", num_first_type_out);
        printf("Количество обращений заявок второго типа: %d\n", total_second_requests);

        // Проверка на допустимость расхождения
        printf("\n\t ПРОЦЕНТ РАССОГЛАСОВАНИЯ\n");
        printf("Расхождение между расчетным и фактическим временем моделирования: %.2f%%\n", time_discrepancy);
    }
}
