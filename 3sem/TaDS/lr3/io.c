#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "functions.h"

// Функция для создания пустой матрицы в формате CSC
CSCMatrix *init_csc_matr(int rows, int cols, int nnz)
{
    CSCMatrix *matrix = malloc(sizeof(CSCMatrix));
    if (!matrix)
        return NULL;
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->values = calloc(nnz, sizeof(int));
    matrix->row_indices = calloc(nnz, sizeof(int));
    matrix->nonZ_sum = calloc(cols + 1, sizeof(int));
    if (!matrix->values || !matrix->row_indices || !matrix->nonZ_sum)
        return NULL;
    return matrix;
}



// Функция для отображения матрицы в формате CSC
void printCSCMatrix(CSCMatrix matrix)
{
    // Выводим значения
    printf("Значения: ");
    int nnz = matrix.nonZ_sum[matrix.cols]; // общее количество ненулевых элементов
    for (int i = 0; i < nnz; i++) 
    {
        printf("%d ", matrix.values[i]);
    }

    // Выводим номера строк
    printf("\nНомера строк: ");
    for (int i = 0; i < nnz; i++) 
    {
        printf("%d ", matrix.row_indices[i]);
    }

    // Выводим кумулянт ненулевых элементов по столбцам
    printf("\nКумулята Не-0 по столбцам: ");
    for (int i = 0; i <= matrix.cols; i++)  // здесь i <= matrix.cols, чтобы захватить последний элемент
    {
        printf("%d ", matrix.nonZ_sum[i]);
    }
    printf("\n");
}



// Функция для вывода матрицы в стандартном формате (если размер меньше 30x30)
void printCSCMatrix_stdView(CSCMatrix csc_matrix) 
{
    // Проверяем ограничения на размер матрицы
    if (csc_matrix.rows > 30 || csc_matrix.cols > 30) {
        printf("Матрица слишком большая для стандартного отображения.\n");
        return;
    }

    // Создаем нулевую матрицу для отображения
    int** standardMatrix = (int**)malloc(csc_matrix.rows * sizeof(int*));
    for (int i = 0; i < csc_matrix.rows; i++) {
        standardMatrix[i] = (int*)calloc(csc_matrix.cols, sizeof(int));
    }

    // Заполняем стандартную матрицу значениями из CSC-формата
    for (int j = 0; j < csc_matrix.cols; j++) 
    {
        for (int k = csc_matrix.nonZ_sum[j]; k < csc_matrix.nonZ_sum[j + 1]; k++) 
        {
            int row = csc_matrix.row_indices[k];  // индекс строки
            int value = csc_matrix.values[k];     // значение элемента
            standardMatrix[row][j] = value;   // добавляем значение в соответствующую позицию
        }
    }

    // Выводим стандартную матрицу
    printf("Стандартное представление матрицы:\n");
    for (int i = 0; i < csc_matrix.rows; i++) {
        for (int j = 0; j < csc_matrix.cols; j++) {
            printf("%d ", standardMatrix[i][j]);
        }
        printf("\n");
    }

    // Освобождаем память
    for (int i = 0; i < csc_matrix.rows; i++) {
        free(standardMatrix[i]);
    }
    free(standardMatrix);
}

// Функция для печати стандартной матрицы
void printStdMatrix(int** matrix, int rows, int cols) 
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void show_menu(void)
{
    printf("\nМеню:\n");
    printf("1. Сменить матрицу для работы\n");
    printf("2. Создать случайную матрицу\n");
    printf("3. Создать матрицу и заполнить по координатам\n");
    printf("4. Сложение матриц (CSC)\n");
    printf("5. Сложение матриц (стандартный метод)\n");
    printf("6. Вывести матрицу в формате CSC\n");
    printf("7. Вывести матрицу в стандартном формате (если размерность <= 30x30)\n");
    printf("8. Сравнить время сложения (CSC и стандартный метод)\n");
    printf("9. Сравнить время сложения (CSC и стандартный метод) полностью автоматически\n");
    printf("0. Выход\n");
    printf("Выберите опцию: ");
}