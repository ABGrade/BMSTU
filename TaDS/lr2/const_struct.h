#ifndef __CONST_STRUCT_H__
#define __CONST_STRUCT_H__

#define MAX_STRING_LENGTH 256
#define MAX_SUB 640

#define MEASUERE_REPEATS 100

typedef struct
{
    char street[MAX_STRING_LENGTH];
    char house[MAX_STRING_LENGTH];
}adress_t;

typedef struct
{
    char surname[MAX_STRING_LENGTH];
    char name[MAX_STRING_LENGTH];
    long long int number;
    adress_t adress;
}user_t;

typedef struct
{
    int year;
    int month;
    int day;
}date_t;

typedef struct
{
    user_t user;
    date_t date_birth;
}friend_t;

typedef struct
{
    user_t user;
    char position[MAX_STRING_LENGTH];
    char organization[MAX_STRING_LENGTH];
}college_t;

typedef enum
{
    FRIEND,
    COLLEGE
}king_sub_e;

typedef union
{
    friend_t friend;
    college_t college;
}status_t;

typedef struct
{
    unsigned int id;
    king_sub_e kind_sub;
    status_t status;
}sub_t;

typedef struct
{
    unsigned int id;
    long long int number;
}key_t;


#endif // __CONST_STRUCT_H__