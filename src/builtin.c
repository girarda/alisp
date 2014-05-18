#include "builtin.h"
#include "error.h"

int builtin_car(Atom args, Atom *result) {
    if (is_nil(args) || !is_nil(cdr(args))) {
        return ERROR_ARGS;
    }

    if (is_nil(car(args))) {
        *result = NIL;
    } else if (car(args).type != AtomType_Pair) {
        return ERROR_TYPE;
    } else {
        *result = car(car(args));
    }

    return ERROR_OK;
}

int builtin_cdr(Atom args, Atom *result) {
    if (is_nil(args) || !is_nil(cdr(args))) {
        return ERROR_ARGS;
    }

    if (is_nil(car(args))) {
        *result = NIL;
    } else if (car(args).type != AtomType_Pair) {
        return ERROR_TYPE;
    } else {
        *result = cdr(car(args));
    }

    return ERROR_OK;
}

int builtin_cons(Atom args, Atom *result) {
    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        return ERROR_ARGS;
    }

    *result = cons(car(args), car(cdr(args)));

    return ERROR_OK;
}
