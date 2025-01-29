#ifndef __MEASURE_H__
#define __MEASURE_H__

#include "functions.h"

#define MEASURES 100
#define MEASURE_AUTO_SIZE 500

void do_measure(CSCMatrix *matrix1, CSCMatrix *matrix2);
void do_all_measures(CSCMatrix *matrix1, CSCMatrix *matrix2);

double measure_cscAddMethod(CSCMatrix matrix1, CSCMatrix matrix2);
double measure_StdAddMethod(int **matrix1, int **matrix2, int rows, int cols);

#endif // __MEASURE_H__