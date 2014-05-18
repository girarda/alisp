#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "atom.h"

int eval_expr(Atom expr, Atom env, Atom *result);

int is_valid_expr(Atom expr);

#endif
