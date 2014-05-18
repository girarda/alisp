#ifndef GC_H
#define GC_H

#include "atom.h"

void gc();
void gc_mark(Atom root);

#endif
