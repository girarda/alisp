#ifndef TYPE_H
#define TYPE_H

#include <stdlib.h>

struct Atom;

typedef int (*Builtin)(struct Atom args, struct Atom *result);

struct Atom {
    enum {
        AtomType_Builtin,
        AtomType_Closure,
        AtomType_Integer,
        AtomType_Macro,
        AtomType_Nil,
        AtomType_Pair,
        AtomType_Symbol
    } type;

    union {
        Builtin builtin;
        long integer;
        struct Pair *pair;
        const char *symbol;
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

Atom make_builtin(Builtin function);
Atom make_int(long x);
Atom make_sym(const char *s);

int make_closure(Atom env, Atom args, Atom body, Atom *result);

Atom cons(Atom car_val, Atom cdr_val);

Atom copy_list(Atom list);


void print_expr(Atom atom);

// private methods
Atom look_for_symbol(const char *s);
void add_symbol_to_table(Atom atom);


#endif
