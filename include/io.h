#ifndef IO_H
#define IO_H
#include "atom.h"

char *slurp(const char *path);

void load_file(Atom env, const char *path);

#endif
