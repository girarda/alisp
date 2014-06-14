#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "atom.h"

int eval_expr(Atom expr, Atom env, Atom *result);

int is_valid_expr(Atom expr);

int eval_do_return(Atom *stack, Atom *expr, Atom *env, Atom *result);
int eval_do_apply(Atom *stack, Atom *expr, Atom *env, Atom *result);
int eval_do_bind(Atom *stack, Atom *expr, Atom *env);
int eval_do_exec(Atom *stack, Atom *expr, Atom *env);

Atom make_frame(Atom parent, Atom env, Atom tail);

Atom list_get(Atom list, int k);
void list_set(Atom list, int k, Atom value);
void list_reverse(Atom *list);

int apply(Atom fn, Atom args, Atom *result);

#endif
