#ifndef TYPE_H
#define TYPE_H

#include <stdlib.h>

struct Atom {
    enum {
        AtomType_Nil,
        AtomType_Pair,
        AtomType_Symbol,
        AtomType_Integer
    } type;

    union {
        struct Pair *pair;
        const char *symbol;
        long integer;
    } value;
};

struct Pair {
    struct Atom atom[2];
};

typedef struct Atom Atom;

#define car(p) ((p).value.pair->atom[0])
#define cdr(p) ((p).value.pair->atom[1])
#define is_nil(atom) ((atom).type == AtomType_Nil)

static const Atom NIL = { AtomType_Nil };
static Atom sym_table = { AtomType_Nil };

Atom make_int(long x);
Atom make_sym(const char *s);

Atom cons(Atom car_val, Atom cdr_val);

void print_expr(Atom atom);

// private methods
Atom look_for_symbol(const char *s);
void add_symbol_to_table(Atom atom);


#endif
