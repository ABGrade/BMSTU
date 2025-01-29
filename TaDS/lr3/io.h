#ifndef __IO_H__
#define __IO_H__

#include "functions.h"

CSCMatrix *init_csc_matr(int rows, int cols, int nnz);

void printCSCMatrix(CSCMatrix matrix);
void printCSCMatrix_stdView(CSCMatrix matrix);
void printStdMatrix(int** matrix, int rows, int cols);
void show_menu(void);

#endif // __IO_H__cls
