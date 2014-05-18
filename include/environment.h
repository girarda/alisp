#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "atom.h"

Atom create_env(Atom parent);

int retrieve_env(Atom env, Atom symbol, Atom *result);
int add_binding_env(Atom env, Atom symbol, Atom value);

#endif
