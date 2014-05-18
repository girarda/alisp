#include "gc.h"
#include "stddef.h"

void gc() {
    struct Allocation *a, **p;

    gc_mark(sym_table);

    /* Free unmarked allocations */
    p = &global_allocations;
    while (*p != NULL) {
        a = *p;
        if (!a->mark) {
            *p = a->next;
            free(a);
        } else {
            p = &a->next;
        }
    }

    /* Clear marks */
    a = global_allocations;
    while (a != NULL) {
        a->mark = 0;
        a = a->next;
    }
}

void gc_mark(Atom root) {
    struct Allocation *a;

    if (!(root.type == AtomType_Pair
        || root.type == AtomType_Closure
        || root.type == AtomType_Macro))
        return;

    a = (struct Allocation *)
        ((char *) root.value.pair
            - offsetof(struct Allocation, pair));

    if (a->mark)
        return;

    a->mark = 1;

    gc_mark(car(root));
    gc_mark(cdr(root));
}
