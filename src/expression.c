#include "expression.h"
#include "environment.h"
#include "error.h"

int eval_expr(Atom expr, Atom env, Atom *result) {
    Atom op, args;
    Error err;

    if (expr.type == AtomType_Symbol) {
        return retrieve_env(env, expr, result);
    } else if (expr.type != AtomType_Pair) {
        *result = expr;
        return ERROR_OK;
    }

    if (!is_valid_expr(expr)) {
        return ERROR_SYNTAX;
    }

    op = car(expr);
    args = cdr(expr);

    if (op.type == AtomType_Symbol) {
        if (strcmp(op.value.symbol, "QUOTE") == 0) {
            if (is_nil(args) || !is_nil(cdr(args))) {
                return ERROR_ARGS;
            }

            *result = car(args);
            return ERROR_OK;
        } else if (strcmp(op.value.symbol, "DEFINE") == 0) {
            Atom sym, val;

            if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
                return ERROR_ARGS;
            }

            sym = car(args);
            if (sym.type != AtomType_Symbol) {
                return ERROR_TYPE;
            }

            err = eval_expr(car(cdr(args)), env, &val);
            if (err) {
                return err;
            }

            *result = sym;
            return add_binding_env(env, sym, val);
        }
    }
    return ERROR_SYNTAX;
}

int is_valid_expr(Atom expr) {
    while (!is_nil(expr)) {
        if (expr.type != AtomType_Pair) {
            return 0;
        }
        expr = cdr(expr);
    }
    return 1;
}
