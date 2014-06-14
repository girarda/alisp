#include "builtin.h"
#include "error.h"
#include "expression.h"

int builtin_car(Atom args, Atom *result) {
    if (is_nil(args) || !is_nil(cdr(args))) {
        *result = make_error("Error args: builtin_car");
        return ERROR_ARGS;
    }

    if (is_nil(car(args))) {
        *result = NIL;
    } else if (car(args).type != AtomType_Pair) {
        *result = make_error("Error type: builtin_car");
        return ERROR_TYPE;
    } else {
        *result = car(car(args));
    }

    return ERROR_OK;
}

int builtin_cdr(Atom args, Atom *result) {
    if (is_nil(args) || !is_nil(cdr(args))) {
        *result = make_error("Error args: builtin_cdr");
        return ERROR_ARGS;
    }

    if (is_nil(car(args))) {
        *result = NIL;
    } else if (car(args).type != AtomType_Pair) {
        *result = make_error("Error type: builtin_cdr");
        return ERROR_TYPE;
    } else {
        *result = cdr(car(args));
    }

    return ERROR_OK;
}

int builtin_cons(Atom args, Atom *result) {
    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("Error args: builtin_cons");
        return ERROR_ARGS;
    }

    *result = cons(car(args), car(cdr(args)));

    return ERROR_OK;
}

int builtin_add(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("Error args: builtin_add");
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        *result = make_error("Error type: builtin_add");
        return ERROR_TYPE;
    }

    *result = make_int(a.value.integer + b.value.integer);

    return ERROR_OK;
}

int builtin_substract(Atom args, Atom *result) {
    Atom a, b;

    /* If only one argument, negate it) */
    if (!is_nil(args) && is_nil(cdr(args))) {
        a = car(args);
         *result = make_int(0 - a.value.integer);
        return ERROR_OK;
    }

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("Error args: builtin_substract");
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        *result = make_error("Error type: builtin_substract");
        return ERROR_TYPE;
    }

    *result = make_int(a.value.integer - b.value.integer);

    return ERROR_OK;
}

int builtin_multiply(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("Error args: builtin_multiply");
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        *result = make_error("Error type: builtin_multiply");
        return ERROR_TYPE;
    }

    *result = make_int(a.value.integer * b.value.integer);

    return ERROR_OK;
}

int builtin_divide(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("Error args: builtin_divide");
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        *result = make_error("Error type: builtin_divide");
        return ERROR_TYPE;
    }

    if (b.value.integer == 0) {
        *result = make_error("Error args: builtin_divide. Cannot divide by zero");
        return ERROR_ARGS;
    }

    *result = make_int(a.value.integer / b.value.integer);

    return ERROR_OK;
}

int builtin_numeq(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("Error args: builtin_numeq");
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        *result = make_error("Error type: builtin_numeq");
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
        *result = make_error("Error args: builtin_less");
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        *result = make_error("Error type: builtin_less");
        return ERROR_TYPE;
    }

    if (a.value.integer < b.value.integer) {
        *result = make_sym("T");
    } else {
        *result = NIL;
    }
    return ERROR_OK;
}

int builtin_apply(Atom args, Atom* result) {
    Atom function;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("Error args: builtin_apply");
        return ERROR_ARGS;
    }

    function = car(args);
    args = car(cdr(args));

    if (!is_valid_expr(args)) {
        *result = make_error("Error syntax: builtin_apply");
        return ERROR_SYNTAX;
    }

    return apply(function, args, result);
}

int builtin_eq(Atom args, Atom* result) {
    Atom a, b;
    int eq;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("Error args: builtin_apply");
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type == b.type) {
        switch (a.type) {
        case AtomType_Builtin:
            eq = (a.value.builtin == b.value.builtin);
            break;
        case AtomType_Closure:
        case AtomType_Macro:
        case AtomType_Pair:
            eq = (a.value.pair == b.value.pair);
            break;
        case AtomType_Error:
            eq = (a.value.error == b.value.error);
            break;
        case AtomType_Integer:
            eq = (a.value.integer == b.value.integer);
            break;
        case AtomType_Nil:
        case AtomType_OK:
            eq = 1;
            break;
        case AtomType_Symbol:
            eq = (a.value.error == b.value.error);
            break;
        }
    } else {
        eq = 0;
    }
    *result = eq ? make_sym("T") : NIL;
    return ERROR_OK;
}

int builtin_is_pair(Atom args, Atom* result) {
    if (is_nil(args) || !is_nil(cdr(args))) {
        *result = make_error("Error args: builtin_is_pair");
        return ERROR_ARGS;
    }

    *result = (car(args).type == AtomType_Pair ? make_sym("T") : NIL);
    return ERROR_OK;
}
