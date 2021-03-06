#include "gc.h"
#include "stddef.h"

/* TODO: Make integration test for whole gc */

void gc() {
    gc_mark(sym_table);
    gc_free_unmarked();
    gc_clear_marks();
}

void gc_mark(Atom root) {
    if (has_children(root)) {
        struct Allocation *alloc = get_allocation(root);

        if (!alloc->mark) {
            alloc->mark = 1;
            gc_mark(car(root));
            gc_mark(cdr(root));
        }
    }
}

void gc_free_unmarked() {
    struct Allocation *alloc;
    struct Allocation **ptr_alloc = &global_allocations;

    while (*ptr_alloc != NULL) {
        alloc = *ptr_alloc;
        if (!alloc->mark) {
            *ptr_alloc = alloc->next;
            free_symbol_if_atom_symbol(alloc->pair.atom[0]);
            free_symbol_if_atom_symbol(alloc->pair.atom[1]);
            free(alloc);
        } else {
            ptr_alloc = &alloc->next;
        }
    }
}

void gc_clear_marks() {
    struct Allocation *alloc = global_allocations;
    while (alloc != NULL) {
        alloc->mark = 0;
        alloc = alloc->next;
    }
}

struct Allocation * get_allocation(Atom root) {
    return (struct Allocation *)
            ((char *) root.value.pair
                - offsetof(struct Allocation, pair));
}

void free_symbol_if_atom_symbol(Atom atom) {
    if(is_symbol(atom)) {
        free(atom.value.symbol);
    } else if (is_error(atom)) {
        free(atom.value.error);
    } else if (is_string(atom)) {
        free(atom.value.string);
    }
}
