#ifndef BUILTIN_H
#define BUILTIN_H

#include "atom.h"

int builtin_car(Atom args, Atom *result);
int builtin_cdr(Atom args, Atom *result);
int builtin_cons(Atom args, Atom *result);

int builtin_add(Atom args, Atom *result);
int builtin_substract(Atom args, Atom *result);
int builtin_multiply(Atom args, Atom *result);
int builtin_divide(Atom args, Atom *result);

int builtin_numeq(Atom args, Atom *result);
int builtin_less(Atom args, Atom *result);

int builtin_apply(Atom args, Atom* result);
int builtin_eq(Atom args, Atom* result);

#endif
