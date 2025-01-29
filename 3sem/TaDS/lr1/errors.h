#ifndef __ERRORS_H__
#define __ERRORS_H__

#define READ_FAIL 1
#define INCORRECT_STRING 2
#define FGETS_STRING_ERROR 3
#define AFTER_E_ERROR 4

#define OVERFLOW_ERROR 10
#define ORDER_OVERFLOW_ERROR 11
#define ORDER_RES_OVERFLOW_ERROR 12

#define ZERO_ERROR 20

int show_error(int rc);

#endif // __ERRORS_H__