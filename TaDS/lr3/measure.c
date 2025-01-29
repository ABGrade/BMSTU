#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "io.h"
#include "functions.h"
#include "measure.h"


double measure_cscAddMethod(CSCMatrix matrix1, CSCMatrix matrix2)
{
    double measure_time = 0;
    for (size_t i = 0; i < MEASURES; i++)
    {
        clock_t start = clock();
        addCSCMatrices(matrix1, matrix2);
        clock_t end = clock();
        measure_time += end - start;
    }
    return measure_time / CLOCKS_PER_SEC / MEASURES;
}

double measure_StdAddMethod(int **matrix1, int **matrix2, int rows, int cols)
{
    double measure_time = 0;
    for (size_t i = 0; i < MEASURES; i++)
    {
        clock_t start = clock();
        addStdMatrix(matrix1, matrix2, rows, cols);
        clock_t end = clock();
        measure_time += end - start;
    }
    return measure_time / CLOCKS_PER_SEC / MEASURES;
}

void do_measure(CSCMatrix *matrix1, CSCMatrix *matrix2)
{
    int **std_matrixA = create_std_matrix_from_csc(matrix1);
    int **std_matrixB = create_std_matrix_from_csc(matrix2);

    double csc_time = measure_cscAddMethod(*matrix1, *matrix2);
    double std_time = measure_StdAddMethod(std_matrixA, std_matrixB, matrix1->rows, matrix1->cols);

    printf("Сложение стандартным способ выполнено за: %lf\n", std_time);
    printf("Сложение csc способ выполнено за: %lf\n", csc_time);
    printf("Отношение std к csc: %.2lf%%\n", std_time / csc_time * 100);
}

void do_all_measures(CSCMatrix *matrix1, CSCMatrix *matrix2)
{
    double persentages[] = { 0.01, 0.05, 0.1, 1.0, 5.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0};
    for (size_t i = 0; i < 14; i++)
    {
        createRandomCSC(&matrix1, MEASURE_AUTO_SIZE, MEASURE_AUTO_SIZE, persentages[i]);
        createRandomCSC(&matrix2, MEASURE_AUTO_SIZE, MEASURE_AUTO_SIZE, persentages[i]);

        int **std_matrixA = create_std_matrix_from_csc(matrix1);
        int **std_matrixB = create_std_matrix_from_csc(matrix2);

        double csc_time = measure_cscAddMethod(*matrix1, *matrix2);
        double std_time = measure_StdAddMethod(std_matrixA, std_matrixB, matrix1->rows, matrix1->cols);

        printf("Тест на %.3lf%% заполнении с %d количеством элементов.\n", persentages[i], (int) ((persentages[i] / 100) * MEASURE_AUTO_SIZE *  MEASURE_AUTO_SIZE));
        printf("Сложение стандартным способ выполнено за: %lf\n", std_time);
        printf("Сложение csc способ выполнено за: %lf\n", csc_time);
        printf("Отношение std к csc: %.2lf%%\n", std_time / csc_time * 100);
    }
}