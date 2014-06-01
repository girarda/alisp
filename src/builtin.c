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

int builtin_add(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        return ERROR_TYPE;
    }

    *result = make_int(a.value.integer + b.value.integer);

    return ERROR_OK;
}

int builtin_substract(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        return ERROR_TYPE;
    }

    *result = make_int(a.value.integer - b.value.integer);

    return ERROR_OK;
}

int builtin_multiply(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        return ERROR_TYPE;
    }

    *result = make_int(a.value.integer * b.value.integer);

    return ERROR_OK;
}

int builtin_divide(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        return ERROR_TYPE;
    }

    if (b.value.integer == 0) {
        return ERROR_ARGS;
    }

    *result = make_int(a.value.integer / b.value.integer);

    return ERROR_OK;
}

int builtin_numeq(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        return ERROR_TYPE;
    }

    if (a.value.integer == b.value.integer) {
        *result = make_sym("T");
    } else {
        *result = NIL;
    }
    return ERROR_OK;
}

int builtin_less(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        return ERROR_TYPE;
    }

    if (a.value.integer < b.value.integer) {
        *result = make_sym("T");
    } else {
        *result = NIL;
    }
    return ERROR_OK;
}
