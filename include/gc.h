#ifndef GC_H
#define GC_H

#include "atom.h"

void gc();
void gc_mark(Atom root);
void gc_free_unmarked();
void gc_clear_marks();

void free_symbol_if_atom_symbol(Atom atom);

struct Allocation * get_allocation(Atom root);

#endif
