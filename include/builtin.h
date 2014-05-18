#ifndef BUILTIN_H
#define BUILTIN_H

#include "atom.h"

int builtin_car(Atom args, Atom *result);
int builtin_cdr(Atom args, Atom *result);
int builtin_cons(Atom args, Atom *result);

#endif
