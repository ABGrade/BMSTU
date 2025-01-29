#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "io.h"
#include "functions.h"


void shuffle(int *array, int size) 
{
    if (size > 1) 
    {
        for (int i = 0; i < size - 1; i++) 
        {
            int j = i + rand() % (size - i);
            int temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}

int compare_ints(const void *a, const void *b) 
{
    return (*(int*)a - *(int*)b);
}


// Функция для создания случайной разреженной матрицы в формате CSC
int createRandomCSC(CSCMatrix **given_matrix, int rows, int cols, double percentage) 
{
    // Рассчитываем количество ненулевых элементов
    int nnz = (int)((rows * cols) * (percentage / 100.0));
    if (!nnz)
    {
        printf("Запрещено создавать матрицы с нулевым количеством ненулевых элементов.\n");
        return 1;
    }
        
    // Инициализируем матрицу
    CSCMatrix *matrix = init_csc_matr(rows, cols, nnz);
    if (!matrix)
    {
        printf("Ошибка инициализации матрицы.\n");
        return 1;
    } 

    int current_nnz = 0;  // Текущий индекс для записи ненулевых элементов
    int remaining_nnz = nnz;

    for (int col = 0; col < cols; col++) 
    {
        int min_elem = 0;
        // Рассчитываем минимальное количество элементов, которое нужно оставить для остальных столбцов
        if (remaining_nnz > rows * (cols - col - 1))
        {
            min_elem = ceil(((double)remaining_nnz) / (cols - col));
        }
        
        // Задаем случайное количество ненулевых элементов в столбце, минимум 1, максимум - остаток
        int num_nonzeros = rand() % (rows - min_elem + 1);
        if (num_nonzeros < min_elem)
        {
            num_nonzeros = min_elem;
        }

        // Если текущий столбец последний, отдаем ему все оставшиеся элементы
        if (col == cols - 1 || num_nonzeros > remaining_nnz) 
        {
            num_nonzeros = remaining_nnz;
        }
        
        matrix->nonZ_sum[col + 1] = matrix->nonZ_sum[col] + num_nonzeros;  // Устанавливаем кумулянт ненулевых элементов
        if (num_nonzeros == 0 || remaining_nnz <= 0)
        {
            continue;
        }
        remaining_nnz -= num_nonzeros;

        // Создаем массив всех возможных строк
        int *all_rows = malloc(rows * sizeof(int));
        if (!all_rows) 
            return 1;
        
        // Заполняем массив номерами строк
        for (int i = 0; i < rows; i++) 
        {
            all_rows[i] = i;
        }
        
        // Перемешиваем массив строк
        shuffle(all_rows, rows);
        all_rows = realloc(all_rows, num_nonzeros * sizeof(int));
        qsort(all_rows, num_nonzeros, sizeof(int), compare_ints);

        // Выбираем первые num_nonzeros элементов после перемешивания
        for (int i = 0; i < num_nonzeros; i++) 
        {
            int rand_row = all_rows[i];  // Берем уникальный индекс строки
            matrix->row_indices[current_nnz] = rand_row;
            matrix->values[current_nnz] = rand() % 10 + 1;  // Генерируем значения от 1 до 10
            current_nnz++;
        }
        free(all_rows);
    }
    

    // Если матрица не пуста, очищаем её перед переинициализированием
    if (*given_matrix)
    {
        free_csc_matrix(*given_matrix);
    }
    *given_matrix = matrix;

    return 0;
}

int createMatrixByCoordinates(CSCMatrix **given_matrix, int rows, int cols) 
{
    int nnz;
    printf("Введите количество ненулевых элементов: ");
    while (scanf("%d", &nnz) != 1 || nnz <= 0 || nnz > rows * cols)
    {
        printf("Неверный ввод. Повторите попытку: \n");
        clear_input_buffer();
    }
    clear_input_buffer();

    // Инициализируем CSC-матрицу с заданными размерами и количеством ненулевых элементов
    CSCMatrix *matrix = init_csc_matr(rows, cols, nnz);
    if (!matrix) 
    {
        printf("Ошибка инициализации матрицы.\n");
        return 1;
    }


    // Создаем матрицу для отслеживания заполненных позиций (инициализируем нулями)
    int **filled_positions = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; ++i) 
    {
        filled_positions[i] = (int *)calloc(cols, sizeof(int));
    }

    // Временный массив для подсчета ненулевых элементов по столбцам
    int* col_counts = (int*)calloc(cols, sizeof(int));
    if (!col_counts)
    {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    // Ввод данных по координатам
    for (int k = 0; k < nnz; k++) 
    {
        int row, col, value;
        printf("Введите координаты (строка, столбец) и значение для элемента #%d: ", k + 1);
        while (scanf("%d %d %d", &row, &col, &value) != 3 || row < 0 || row >= rows 
                || col < 0 || col >= cols || value == 0 || filled_positions[row][col])
        {
            if (filled_positions[row][col])
            {
                printf("Данная позиция уже занята!\n");
            }
            printf("Неверный ввод. Повторите попытку: \n");
            clear_input_buffer();
        }
        clear_input_buffer();

        // Заполняем значения и индексы строк
        matrix->values[k] = value;
        matrix->row_indices[k] = row;
        col_counts[col]++;  // Увеличиваем счетчик для данного столбца
        filled_positions[row][col] = 1;
    }

    // Формируем массив кумулянтов nonZ_sum
    for (int j = 1; j <= cols; j++) 
    {
        matrix->nonZ_sum[j] = matrix->nonZ_sum[j - 1] + col_counts[j - 1];
    }
    free(col_counts);  // Освобождаем временный массив

    // Если матрица не пуста, очищаем её перед переинициализированием
    if (*given_matrix)
    {
        free_csc_matrix(*given_matrix);
    }
    *given_matrix = matrix;
    return 0;
}

// Функция для сложения двух разреженных матриц в формате CSC
CSCMatrix *addCSCMatrices(CSCMatrix A, CSCMatrix B) 
{
    if (A.rows != B.rows || A.cols != B.cols) 
    {
        printf("Ошибка: размерности матриц не совпадают!\n");
        return NULL;
    }
        // Максимальная оценка размера результирующих массивов
    int maxNonZeros = A.nonZ_sum[A.cols] + B.nonZ_sum[B.cols];

    CSCMatrix *result = init_csc_matr(A.rows, A.cols, maxNonZeros);

    int nz_counter = 0; // счетчик ненулевых элементов

    // Проход по каждому столбцу
    for (int col = 0; col < A.cols; col++) 
    {
        result->nonZ_sum[col] = nz_counter;

        int a_idx = A.nonZ_sum[col];       // начало столбца в A
        int b_idx = B.nonZ_sum[col];       // начало столбца в B
        int a_end = A.nonZ_sum[col + 1];   // конец столбца в A
        int b_end = B.nonZ_sum[col + 1];   // конец столбца в B

        // Объединение ненулевых элементов столбца из A и B
        while (a_idx < a_end || b_idx < b_end) 
        {
            if (a_idx < a_end && (b_idx >= b_end || A.row_indices[a_idx] < B.row_indices[b_idx])) 
            {
                // Элемент только в A
                result->values[nz_counter] = A.values[a_idx];
                result->row_indices[nz_counter] = A.row_indices[a_idx];
                a_idx++;
            } 
            else if (b_idx < b_end && (a_idx >= a_end || B.row_indices[b_idx] < A.row_indices[a_idx])) 
            {
                // Элемент только в B
                result->values[nz_counter] = B.values[b_idx];
                result->row_indices[nz_counter] = B.row_indices[b_idx];
                b_idx++;
            } 
            else 
            {
                // Элемент присутствует в обеих матрицах
                result->values[nz_counter] = A.values[a_idx] + B.values[b_idx];
                result->row_indices[nz_counter] = A.row_indices[a_idx];
                a_idx++;
                b_idx++;
            }
            nz_counter++;
        }
    }

    result->nonZ_sum[A.cols] = nz_counter; // последний элемент указывает на конец

    // Обрезаем массивы до фактического размера
    result->values = (int*)realloc(result->values, nz_counter * sizeof(int));
    result->row_indices = (int*)realloc(result->row_indices, nz_counter * sizeof(int));

    return result;
}


int** create_std_matrix_from_csc(CSCMatrix* cscMatrix) 
{
    if (!cscMatrix) return NULL;

    // Создаем двумерный массив для стандартной матрицы (заполняем его нулями)
    int** denseMatrix = (int**)malloc(cscMatrix->rows * sizeof(int*));
    for (int i = 0; i < cscMatrix->rows; ++i) 
    {
        denseMatrix[i] = (int*)calloc(cscMatrix->cols, sizeof(int));
    }

    // Проходим по каждому столбцу CSC матрицы
    for (int col = 0; col < cscMatrix->cols; ++col) 
    {
        // Начало и конец ненулевых элементов для текущего столбца
        int start = cscMatrix->nonZ_sum[col];
        int end = cscMatrix->nonZ_sum[col + 1];

        // Проходим по ненулевым элементам текущего столбца
        for (int idx = start; idx < end; ++idx) 
        {
            int row = cscMatrix->row_indices[idx];  // строка ненулевого элемента
            int value = cscMatrix->values[idx];     // значение ненулевого элемента

            // Заполняем стандартную матрицу
            denseMatrix[row][col] = value;
        }
    }

    return denseMatrix;
}

int **addStdMatrix(int **matrix1, int **matrix2, int rows, int cols)
{
    int** res_matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; ++i) 
    {
        res_matrix[i] = (int*)calloc(cols, sizeof(int));
    }

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            res_matrix[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return res_matrix;
}

void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void free_csc_matrix(CSCMatrix *matrix) 
{
    if (!matrix) return;

    // Освобождаем массивы, если они были выделены
    if (matrix->values) {
        free(matrix->values);
    }

    if (matrix->row_indices) {
        free(matrix->row_indices);
    }

    if (matrix->nonZ_sum) {
        free(matrix->nonZ_sum);
    }

    // Освобождаем саму структуру матрицы
    free(matrix);
}
