#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#define EPS 1e-10

typedef struct 
{
    int* values; // значения
    int* row_indices;  // номера строк
    int* nonZ_sum;  // Кумулянта не нулевых значений
    int rows;
    int cols;
} CSCMatrix;

int createRandomCSC(CSCMatrix **given_matrix, int rows, int cols, double percentage);
int createMatrixByCoordinates(CSCMatrix **given_matrix, int rows, int cols);
int** create_std_matrix_from_csc(CSCMatrix* cscMatrix);

CSCMatrix *addCSCMatrices(CSCMatrix A, CSCMatrix B);
int **addStdMatrix(int **matrix1, int **matrix2, int rows, int cols);

int compare_ints(const void *n1, const void *n2);
void free_csc_matrix(CSCMatrix *matrix);
void clear_input_buffer(void);

#endif // __FUNCTIONS_H__