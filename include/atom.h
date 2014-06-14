#ifndef TYPE_H
#define TYPE_H

#include <stdlib.h>

struct Atom;

typedef int (*Builtin)(struct Atom args, struct Atom *result);

struct Atom {
    enum {
        AtomType_Builtin,
        AtomType_Closure,
        AtomType_Error,
        AtomType_Integer,
        AtomType_Macro,
        AtomType_Nil,
        AtomType_OK,
        AtomType_Pair,
        AtomType_Symbol
    } type;

    union {
        Builtin builtin;
        long integer;
        struct Pair *pair;
        const char *symbol;
        const char *error;
    } value;
};

struct Pair {
    struct Atom atom[2];
};

struct Allocation {
    struct Pair pair;
    int mark: 1;
    struct Allocation *next;
};

struct Allocation *global_allocations;

typedef struct Atom Atom;

/* TODO: Assert p is pair ? */
#define car(p) ((p).value.pair->atom[0])
#define cdr(p) ((p).value.pair->atom[1])
#define is_nil(atom) ((atom).type == AtomType_Nil)

static const Atom NIL = { AtomType_Nil };
static const Atom OK = { AtomType_OK };

static Atom sym_table = { AtomType_Nil };

Atom make_builtin(Builtin function);
Atom make_int(long x);
Atom make_sym(const char *s);
Atom make_error(const char *s);

int make_closure(Atom env, Atom args, Atom body, Atom *result);

Atom cons(Atom car_val, Atom cdr_val);

int is_closure(Atom atom);
int is_macro(Atom atom);
int is_pair(Atom atom);
int is_symbol(Atom atom);
int is_error(Atom atom);

int has_children(Atom atom);

void print_expr(Atom atom);

// private methods
Atom look_for_symbol(const char *s);
Atom create_and_add_symbol_to_table(const char *s);
void add_symbol_to_table(Atom atom);



#endif
