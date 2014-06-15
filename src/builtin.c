#include "builtin.h"
#include "error.h"
#include "expression.h"

int builtin_car(Atom args, Atom *result) {
    if (is_nil(args) || !is_nil(cdr(args))) {
        *result = make_error("builtin_car: Invalid number of arguments. Expected 1 argument.");
        return ERROR_ARGS;
    }

    if (is_nil(car(args))) {
        *result = NIL;
    } else if (car(args).type != AtomType_Pair) {
        *result = make_error("builtin_car: Invalid type. Expected pair.");
        return ERROR_TYPE;
    } else {
        *result = car(car(args));
    }

    return ERROR_OK;
}

int builtin_cdr(Atom args, Atom *result) {
    if (is_nil(args) || !is_nil(cdr(args))) {
        *result = make_error("builtin_cdr: Invalid number of arguments. Expected 1 argument.");
        return ERROR_ARGS;
    }

    if (is_nil(car(args))) {
        *result = NIL;
    } else if (car(args).type != AtomType_Pair) {
        *result = make_error("builtin_cdr: Invalid type. Expected pair.");
        return ERROR_TYPE;
    } else {
        *result = cdr(car(args));
    }

    return ERROR_OK;
}

int builtin_cons(Atom args, Atom *result) {
    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("builtin_cons: Invalid number of arguments. Expected 2 arguments.");
        return ERROR_ARGS;
    }

    *result = cons(car(args), car(cdr(args)));

    return ERROR_OK;
}

int builtin_add(Atom args, Atom *result) {
    Atom a, b;

    int sum = 0;
    if (is_nil(args)) {
        *result = make_error("builtin_add: Invalid number of arguments. Expected at least one argument.");
        return ERROR_ARGS;
    }

    b = args;

    while (b.type != AtomType_Nil) {
        a = car(b);
        if (a.type != AtomType_Integer) {
            *result = make_error("builtin_add: Invalid argument type. Expected integers.");
            return ERROR_TYPE;
        }

        sum += a.value.integer;
        b = cdr(b);
    }

    *result = make_int(sum);

    return ERROR_OK;
}

int builtin_substract(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args)) {
        *result = make_error("builtin_add: Invalid number of arguments. Expected at least one argument.");
        return ERROR_ARGS;
    }

    if (car(args).type != AtomType_Integer) {
        *result = make_error("builtin_add: Invalid argument type. Expected integers.");
        return ERROR_TYPE;
    }

    /* If only one argument, negate it) */
    if (!is_nil(args) && is_nil(cdr(args))) {
        a = car(args);
         *result = make_int(0 - a.value.integer);
        return ERROR_OK;
    }

    int difference = car(args).value.integer;
    b = cdr(args);

    while (b.type != AtomType_Nil) {
        a = car(b);
        if (a.type != AtomType_Integer) {
            *result = make_error("builtin_add: Invalid argument type. Expected integers.");
            return ERROR_TYPE;
        }

        difference -= a.value.integer;
        b = cdr(b);
    }

    *result = make_int(difference);

    return ERROR_OK;
}

int builtin_multiply(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("builtin_multiply: Invalid number of arguments. Expected 2 arguments.");
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        *result = make_error("builtin_multiply: Invalid type. Expected integer.");
        return ERROR_TYPE;
    }

    *result = make_int(a.value.integer * b.value.integer);

    return ERROR_OK;
}

int builtin_divide(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("builtin_divide: Invalid number of arguments. Expected 2 arguments.");
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        *result = make_error("builtin_divide: Invalid type. Expected integer.");
        return ERROR_TYPE;
    }

    if (b.value.integer == 0) {
        *result = make_error("builtin_divide. Invalid arguments. Cannot divide by zero");
        return ERROR_ARGS;
    }

    *result = make_int(a.value.integer / b.value.integer);

    return ERROR_OK;
}

int builtin_numeq(Atom args, Atom *result) {
    Atom a, b;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("builtin_numeq: Invalid number of arguments. Expected 2 arguments.");
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        *result = make_error("builtin_numeq: Invalid type. Expected integer.");
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
        *result = make_error("builtin_less: Invalid number of arguments. Expected 2 arguments.");
        return ERROR_ARGS;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer) {
        *result = make_error("builtin_less: Invalid type. Expected integer.");
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
        *result = make_error("builtin_apply: Invalid number of arguments. Expected 2 arguments.");
        return ERROR_ARGS;
    }

    function = car(args);
    args = car(cdr(args));

    if (!is_valid_expr(args)) {
        *result = make_error("builtin_apply: Syntax error.");
        return ERROR_SYNTAX;
    }

    return apply(function, args, result);
}

int builtin_eq(Atom args, Atom* result) {
    Atom a, b;
    int eq;

    if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
        *result = make_error("builtin_apply: Invalid number of arguments. Expected 2 arguments.");
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
        case AtomType_String:
            eq = (a.value.string == b.value.string);
            break;
        case AtomType_Symbol:
            eq = (a.value.symbol == b.value.symbol);
            break;
        }
    } else {
        eq = 0;
    }
    *result = eq ? make_sym("T") : NIL;
    return ERROR_OK;
}
