#include <stdio.h>

#include "errors.h"

int show_error(int rc)
{
    switch (rc)
    {
        case READ_FAIL:
            printf("READ_FAIL\n");
            break;
        
        case INCORRECT_STRING:
            printf("INCORRECT_STRING\n");
            break;

        case FGETS_STRING_ERROR:
            printf("FGETS_STRING_ERROR\n");
            break;

        case AFTER_E_ERROR:
            printf("AFTER_E_ERROR\n");
            break;

        case OVERFLOW_ERROR:
            printf("OVERFLOW_ERROR\n");
            break;
        
        case ORDER_OVERFLOW_ERROR:
            printf("ORDER_OVERFLOW_ERROR\n");
            break;

        case ORDER_RES_OVERFLOW_ERROR:
            printf("ORDER_RES_OVERFLOW_ERROR\n");
            break;

        case ZERO_ERROR:
            printf("ZERO_ERROR\n");
            break;
                
        default:
            break;
    }
    return rc;
}