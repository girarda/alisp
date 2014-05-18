#include "atom.h"
#include <string.h>
#include <stdio.h>

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
        atom.type = AtomType_Symbol;
        atom.value.symbol = strdup(s);
        add_symbol_to_table(atom);
    }

    return atom;
}

Atom look_for_symbol(const char *s) {
    Atom atom, existing_symbol;

    existing_symbol = sym_table;
    while(!is_nil(existing_symbol)) {
        atom = car(existing_symbol);
        if (strcmp(atom.value.symbol, s) == 0) {
            return atom;
        }
        existing_symbol = cdr(existing_symbol);
    }
    return existing_symbol;
}

void add_symbol_to_table(Atom atom) {
    //TODO ASSERT atom.type == AtomType_Symbol
    sym_table = cons(atom, sym_table);
}

Atom cons(Atom car_val, Atom cdr_val) {
    Atom atom;

    atom.type = AtomType_Pair;
    atom.value.pair = malloc(sizeof(struct Pair));

    car(atom) = car_val;
    cdr(atom) = cdr_val;

    return atom;
}

void print_expr(Atom atom) {
    switch(atom.type) {
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
    case AtomType_Integer:
        printf("%ld", atom.value.integer);
        break;
    }
}
