#include "environment.h"
#include "error.h"

Atom create_env(Atom parent) {
    return cons(parent, NIL);
}

int retrieve_env(Atom env, Atom symbol, Atom *result) {
    Atom parent = car(env);
    Atom bs = cdr(env);

    while (!is_nil(bs)) {
        Atom b = car(bs);
        if (car(b).value.symbol == symbol.value.symbol) {
            *result = cdr(b);
            return ERROR_OK;
        }
        bs = cdr(bs);
    }

    if (is_nil(parent))
        return ERROR_UNBOUND;

    return retrieve_env(parent, symbol, result);
}

int add_binding_env(Atom env, Atom symbol, Atom value) {
    Atom bs = cdr(env);
    Atom b = NIL;

    while (!is_nil(bs)) {
        b = car(bs);
        if (car(b).value.symbol == symbol.value.symbol) {
            cdr(b) = value;
            return ERROR_OK;
        }
        bs = cdr(bs);
    }

    b = cons(symbol, value);
    cdr(env) = cons(b, cdr(env));

    return ERROR_OK;
}
