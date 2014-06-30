#include "expression.h"
#include "environment.h"
#include "error.h"
#include "gc.h"

int eval_expr(Atom expr, Atom env, Atom *result) {
    static int count = 0;
    Atom stack = NIL;

    do {
        if (++count == 100000) {
                    gc_mark(expr);
                    gc_mark(env);
                    gc_mark(stack);
                    gc();
                    count = 0;
                }

        if (expr.type == AtomType_Symbol) {
            retrieve_env(env, expr, result);
        } else if (expr.type != AtomType_Pair) {
            *result = expr;
        } else if (!is_valid_expr(expr)) {
            *result = make_error("Syntax error: eval_expr");
            return ERROR_SYNTAX;
        } else {
            Atom op = car(expr);
            Atom args = cdr(expr);

            if (op.type == AtomType_Symbol) {
                /* Handle special forms */

                if (strcmp(op.value.symbol, "QUOTE") == 0) { /* I'm not sure this one still works */
                    ASSERT_NUM_ARGS("eval_expr", args, 1, result)

                    *result = car(args);
                } else if (strcmp(op.value.symbol, "DEFINE") == 0) {
                    Atom sym;

                    ASSERT_NUM_ARGS("eval_expr", args, 2, result)

                    sym = car(args);
                    if (sym.type == AtomType_Pair) {
                        make_closure(env, cdr(sym), cdr(args), result);
                        sym = car(sym);
                        if (sym.type != AtomType_Symbol) {
                            *result = make_error("Type error: eval_expr");
                            return ERROR_TYPE;
                        }
                        (void) add_binding_env(env, sym, *result);
                        *result = sym;
                    } else if (sym.type == AtomType_Symbol) {
                        if (!is_nil(cdr(cdr(args)))) {
                            *result = make_error("Args error: eval_expr");
                            return ERROR_ARGS;
                        }
                        stack = make_frame(stack, env, NIL);
                        list_set(stack, 2, op);
                        list_set(stack, 4, sym);
                        expr = car(cdr(args));
                        continue;
                    } else {
                        *result = make_error("Type error: eval_expr");
                        return ERROR_TYPE;
                    }
                } else if (strcmp(op.value.symbol, "LAMBDA") == 0) {
                    if (length_cons(args) < 2) {
                        *result = make_error("Args error: eval_expr");
                        return ERROR_ARGS;
                    }

                    make_closure(env, car(args), cdr(args), result);
                } else if (strcmp(op.value.symbol, "IF") == 0) {
                    ASSERT_NUM_ARGS("eval_expr", args, 3, result)

                    stack = make_frame(stack, env, cdr(args));
                    list_set(stack, 2, op);
                    expr = car(args);
                    continue;
                } else if (strcmp(op.value.symbol, "DEFMACRO") == 0) {
                    Atom name, macro;

                    ASSERT_NUM_ARGS("eval_expr", args, 2, result)

                    if (car(args).type != AtomType_Pair) {
                        *result = make_error("Syntax error: eval_expr");
                        return ERROR_SYNTAX;
                    }

                    name = car(car(args));
                    if (name.type != AtomType_Symbol) {
                        *result = make_error("Type error: eval_expr");
                        return ERROR_TYPE;
                    }

                    make_closure(env, cdr(car(args)), cdr(args), &macro);
                    if (!is_error(*result)) {
                        macro.type = AtomType_Macro;
                        *result = name;
                        (void) add_binding_env(env, name, macro);
                    }
                } else if (strcmp(op.value.symbol, "APPLY") == 0) {
                    ASSERT_NUM_ARGS("eval_expr", args, 2, result)

                    stack = make_frame(stack, env, cdr(args));
                    list_set(stack, 2, op);
                    expr = car(args);
                    continue;
                } else {
                    goto push;
                }
            } else if (op.type == AtomType_Builtin) {
                (*op.value.builtin)(args, result);
            } else {
            push:
                /* Handle function application */
                stack = make_frame(stack, env, args);
                expr = op;
                continue;
            }
        }

        if (is_nil(stack)) {
            break;
        }

        if (!is_error(*result)) {
            eval_do_return(&stack, &expr, &env, result);
        }
    } while (!is_error(*result));

    if (is_error(*result)) {
        return ERROR_SYNTAX; // may not be the right type, I am trying to remove the whole enum
    }
    return ERROR_OK;
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

int eval_do_return(Atom *stack, Atom *expr, Atom *env, Atom *result) {
    Atom op, args, body;

    *env = list_get(*stack, 1);
    op = list_get(*stack, 2);
    body = list_get(*stack, 5);

    if (!is_nil(body)) {
        /* Still running a procedure; ignore the result */
        return eval_do_apply(stack, expr, env, result);
    }

    if (is_nil(op)) {
        /* Finished evaluating operator */
        op = *result;
        list_set(*stack, 2, op);

        if (op.type == AtomType_Macro) {
            /* Don't evaluate macro arguments */
            args = list_get(*stack, 3);
            *stack = make_frame(*stack, *env, NIL);
            op.type = AtomType_Closure;
            list_set(*stack, 2, op);
            list_set(*stack, 4, args);
            return eval_do_bind(stack, expr, env);
        }
    } else if (op.type == AtomType_Symbol) {
        /* Finished working on special form */
        if (strcmp(op.value.symbol, "DEFINE") == 0) {
            Atom sym = list_get(*stack, 4);
            (void) add_binding_env(*env, sym, *result);
            *stack = car(*stack);
            *expr = cons(make_sym("QUOTE"), cons(sym, NIL));
            return ERROR_OK;
        } else if (strcmp(op.value.symbol, "IF") == 0) {
            args = list_get(*stack, 3);
            *expr = is_nil(*result) ? car(cdr(args)) : car(args);
            *stack = car(*stack);
            return ERROR_OK;
        } else {
            goto store_arg;
        }
    } else if (op.type == AtomType_Macro) {
        /* Finished evaluating macro */
        *expr = *result;
        *stack = car(*stack);
        return ERROR_OK;
    } else {
    store_arg:
        /* Store evaluated argument */
        args = list_get(*stack, 4);
        list_set(*stack, 4, cons(*result, args));
    }

    args = list_get(*stack, 3);
    if (is_nil(args)) {
        /* No more arguments left to evaluate */
        return eval_do_apply(stack, expr, env, result);
    }

    /* Evaluate next argument */
    *expr = car(args);
    list_set(*stack, 3, cdr(args));
    return ERROR_OK;
}
int eval_do_apply(Atom *stack, Atom *expr, Atom *env, Atom *result){
    Atom op, args;

    op = list_get(*stack, 2);
    args = list_get(*stack, 4);

    if (!is_nil(args)) {
        list_reverse(&args);
        list_set(*stack, 4, args);
    }

    if (op.type == AtomType_Symbol) {
        if (strcmp(op.value.symbol, "APPLY") == 0) {
            /* Replace the current frame */
            *stack = car(*stack);
            *stack = make_frame(*stack, *env, NIL);
            op = car(args);
            args = car(cdr(args));
            if (!is_valid_expr(args)) {
                *result = make_error("Syntax error: eval_do_apply");
                return ERROR_SYNTAX;
            }

            list_set(*stack, 2, op);
            list_set(*stack, 4, args);
        }
    }

    if (op.type == AtomType_Builtin) {
        *stack = car(*stack);
        *expr = cons(op, args);
        return ERROR_OK;
    } else if (op.type != AtomType_Closure) {
        *result = make_error("Type error: eval_do_apply");
        return ERROR_TYPE;
    }

    return eval_do_bind(stack, expr, env);
}

int eval_do_bind(Atom *stack, Atom *expr, Atom *env) {
    Atom op, args, arg_names, body;

    body = list_get(*stack, 5);
    if (!is_nil(body)) {
        return eval_do_exec(stack, expr, env);
    }

    op = list_get(*stack, 2);
    args = list_get(*stack, 4);

    *env = create_env(car(op));
    arg_names = car(cdr(op));
    body = cdr(cdr(op));
    list_set(*stack, 1, *env);
    list_set(*stack, 5, body);

    /* Bind the arguments */
    while (!is_nil(arg_names)) {
        if (arg_names.type == AtomType_Symbol) {
            add_binding_env(*env, arg_names, args);
            args = NIL;
            break;
        }

        if (is_nil(args)) {
            return ERROR_ARGS;
        }
        add_binding_env(*env, car(arg_names), car(args));
        arg_names = cdr(arg_names);
        args = cdr(args);
    }
    if (!is_nil(args)) {
        return ERROR_ARGS;
    }

    list_set(*stack, 4, NIL);

    return eval_do_exec(stack, expr, env);
}

int eval_do_exec(Atom *stack, Atom *expr, Atom *env) {
    Atom body;

    *env = list_get(*stack, 1);
    body = list_get(*stack, 5);
    *expr = car(body);
    body = cdr(body);
    if (is_nil(body)) {
        /* Finished function; pop the stack */
        *stack = car(*stack);
    } else {
        list_set(*stack, 5, body);
    }

    return ERROR_OK;
}

Atom make_frame(Atom parent, Atom env, Atom tail){
    return cons(parent,
        cons(env,
        cons(NIL, /* op */
        cons(tail,
        cons(NIL, /* args */
        cons(NIL, /* body */
        NIL))))));
}

Atom list_get(Atom list, int k)
{
    while (k--) {
        list = cdr(list);
    }
    return car(list);
}

void list_set(Atom list, int k, Atom value)
{
    while (k--) {
        list = cdr(list);
    }
    car(list) = value;
}

void list_reverse(Atom *list)
{
    Atom tail = NIL;
    while (!is_nil(*list)) {
        Atom p = cdr(*list);
        cdr(*list) = tail;
        tail = *list;
        *list = p;
    }
    *list = tail;
}

int apply(Atom fn, Atom args, Atom *result)
{
    Atom env, arg_names, body;

    if (fn.type == AtomType_Builtin)
        return (*fn.value.builtin)(args, result);
    else if (fn.type != AtomType_Closure)
        return ERROR_TYPE;

    env = create_env(car(fn));
    arg_names = car(cdr(fn));
    body = cdr(cdr(fn));

    /* Bind the arguments */
    while (!is_nil(arg_names)) {
        if (is_nil(args))
            return ERROR_ARGS;
        add_binding_env(env, car(arg_names), car(args));
        arg_names = cdr(arg_names);
        args = cdr(args);
    }
    if (!is_nil(args))
        return ERROR_ARGS;

    /* Evaluate the body */
    while (!is_nil(body)) {
        Error err = eval_expr(car(body), env, result);
        if (err)
            return err;
        body = cdr(body);
    }

    return ERROR_OK;
}
