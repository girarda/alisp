#include "expression.h"
#include "environment.h"
#include "error.h"

int eval_expr(Atom expr, Atom env, Atom *result) {
    Atom op, args, p;
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
        } else if (strcmp(op.value.symbol, "IF") == 0) {
            Atom cond, val;

            /*TODO: the argument check is terrible
            Possible solutions: car and cdr return NIL if argument is not pair
            or create helper function to count list lenght
            */
            if (is_nil(args) || is_nil(cdr(args)) || is_nil(cdr(cdr(args))) || !is_nil(cdr(cdr(cdr(args))))) {
                return ERROR_ARGS;
            }

            err = eval_expr(car(args), env, &cond);
            if (err) {
                return err;
            }

            if (!is_nil(cond)) {
                val = car(cdr(args));
            } else {
                val = car(cdr(cdr(args)));
            }
            return eval_expr(val, env, result);
        }else if (strcmp(op.value.symbol, "LAMBDA") == 0) {
            if (is_nil(args) || is_nil(cdr(args))) {
                return ERROR_ARGS;
            }
            return make_closure(env, car(args), cdr(args), result);
        } else if (strcmp(op.value.symbol, "DEFINE") == 0) {
            Atom sym, val;

            if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
                return ERROR_ARGS;
            }

            sym = car(args);
            if (sym.type == AtomType_Pair) {
                err = make_closure(env, cdr(sym), cdr(args), &val);
                sym = car(sym);
                if (sym.type != AtomType_Symbol) {
                    return ERROR_TYPE;
                }
            } else if (sym.type == AtomType_Symbol) {
                if (!is_nil(cdr(cdr(args)))) {
                    return ERROR_ARGS;
                }
                err = eval_expr(car(cdr(args)), env, &val);
            } else {
                return ERROR_TYPE;
            }

            if (err) {
                return err;
            }

            *result = sym;
            return add_binding_env(env, sym, val);
        }
    }

    err = eval_expr(op, env, &op);
    if (err) {
        return err;
    }

    /* Evaluate arguments */
    args = copy_list(args);
    p = args;
    while (!is_nil(p)) {
        err = eval_expr(car(p), env, &car(p));
        if (err) {
            return err;
        }
        p = cdr(p);
    }
    return apply(op, args, result);
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

int apply(Atom fn, Atom args, Atom *result)
{
    Atom env, arg_names, body;

    if (fn.type == AtomType_Builtin) {
        return (*fn.value.builtin)(args, result);
    } else if (fn.type != AtomType_Closure) {
        return ERROR_TYPE;
    }

    env = create_env(car(fn));
    arg_names = car(cdr(fn));
    body = cdr(cdr(fn));

    /* Bind the arguments */
    while (!is_nil(arg_names)) {
        if (is_nil(args)) {
            return ERROR_ARGS;
        }
        add_binding_env(env, car(arg_names), car(args));
        arg_names = cdr(arg_names);
        args = cdr(args);
    }
    if (!is_nil(args)) {
        return ERROR_ARGS;
    }

    /* Evaluate the body */
    while (!is_nil(body)) {
        Error err = eval_expr(car(body), env, result);
        if (err)
            return err;
        body = cdr(body);
    }

    return ERROR_OK;
}

