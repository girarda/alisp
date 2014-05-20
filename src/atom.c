#include "atom.h"
#include "expression.h"
#include "error.h"
#include "util.h"
#include <string.h>
#include <stdio.h>

struct Allocation *global_allocations = NULL;

Atom make_builtin(Builtin function) {
    Atom atom;

    atom.type = AtomType_Builtin;
    atom.value.builtin = function;

    return atom;
}

Atom make_int(long x) {
    Atom atom;

    atom.type = AtomType_Integer;
    atom.value.integer = x;

    return atom;
}

Atom make_sym(const char *s) {
    Atom atom;

    atom = look_for_symbol(s);

    if (is_nil(atom)) {
        return create_and_add_symbol_to_table(s);
    }

    return atom;
}

Atom look_for_symbol(const char *s) {
    Atom atom, existing_symbol;

    existing_symbol = sym_table;
    while(!is_nil(existing_symbol)) {
        atom = car(existing_symbol);
        if (is_same_string(atom.value.symbol, s)) {
            return atom;
        }
        existing_symbol = cdr(existing_symbol);
    }
    return existing_symbol;
}

Atom create_and_add_symbol_to_table(const char *s) {
    Atom atom;
    atom.type = AtomType_Symbol;
    atom.value.symbol = strdup(s);
    add_symbol_to_table(atom);
    return atom;
}

void add_symbol_to_table(Atom atom) {
    //TODO ASSERT atom.type == AtomType_Symbol
    sym_table = cons(atom, sym_table);
}

int make_closure(Atom env, Atom args, Atom body, Atom *result) {
    Atom atom;

    if (!is_valid_expr(body)) {
        return ERROR_SYNTAX;
    }

    /* Check argument names are all symbols */
    atom = args;
    while (!is_nil(atom)) {
        if (atom.type == AtomType_Symbol) {
            break;
        } else if (!is_pair(atom) || !is_symbol(car(atom))) {
            return ERROR_TYPE;
        }
        atom = cdr(atom);
    }

    *result = cons(env, cons(args, body));
    result->type = AtomType_Closure;

    return ERROR_OK;
}

Atom cons(Atom car_val, Atom cdr_val) {
    struct Allocation *alloc;
    Atom atom;

    alloc = malloc(sizeof(struct Allocation));
    alloc->mark = 0;
    alloc->next = global_allocations;

    atom.type = AtomType_Pair;
    atom.value.pair = &alloc->pair;

    car(atom) = car_val;
    cdr(atom) = cdr_val;

    return atom;
}

int is_closure(Atom atom) {
    return atom.type == AtomType_Closure;
}

int is_macro(Atom atom) {
    return atom.type == AtomType_Macro;
}

int is_pair(Atom atom) {
    return atom.type == AtomType_Pair;
}

int is_symbol(Atom atom) {
    return atom.type == AtomType_Symbol;
}

int has_children(Atom atom) {
    return is_closure(atom) || is_macro(atom) || is_pair(atom);
}

void print_expr(Atom atom) {
    switch(atom.type) {
    case AtomType_Builtin:
        printf("#<BUILTIN:%p>", atom.value.builtin);
        break;
    case AtomType_Closure:
        printf("#<CLOSURE:%p>", atom.value.builtin);
        break;
    case AtomType_Integer:
        printf("%ld", atom.value.integer);
        break;
    case AtomType_Macro:
        printf("#<MACRO:%p>", atom.value.builtin);
        break;
    case AtomType_Nil:
        printf("NIL");
        break;
    case AtomType_Pair:
        putchar('(');
        print_expr(car(atom));
        atom = cdr(atom);
        while(!is_nil(atom)) {
            if (atom.type == AtomType_Pair) {
                putchar(' ');
                print_expr(car(atom));
                atom = cdr(atom);
            } else {
                printf(" . ");
                print_expr(atom);
                break;
            }
        }
        putchar(')');
        break;
    case AtomType_Symbol:
        printf("%s", atom.value.symbol);
        break;
    }
}
