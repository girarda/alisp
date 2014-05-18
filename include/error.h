#ifndef ERROR_H
#define ERROR_H

typedef enum {
    ERROR_OK = 0,
    ERROR_ARGS, // List expression is shorter or longer than expected
    ERROR_SYNTAX,
    ERROR_TYPE, // Object was of different type than expected
    ERROR_UNBOUND // Attempted to evaluate symbol for which no binding exists
} Error;

#endif
