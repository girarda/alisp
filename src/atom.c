#include "atom.h"
#include "expression.h"
#include "error.h"
#include <string.h>
#include <stdio.h>

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

int make_closure(Atom env, Atom args, Atom body, Atom *result) {
    Atom atom;

    if (!is_valid_expr(args) || !is_valid_expr(body)) {
        return ERROR_SYNTAX;
    }

    /* Check argument names are all symbols */
    atom = args;
    while (!is_nil(atom)) {
        if (car(atom).type != AtomType_Symbol) {
            return ERROR_TYPE;
        }
        atom = cdr(atom);
    }

    *result = cons(env, cons(args, body));
    result->type = AtomType_Closure;

    return ERROR_OK;
}

Atom cons(Atom car_val, Atom cdr_val) {
    Atom atom;

    atom.type = AtomType_Pair;
    atom.value.pair = malloc(sizeof(struct Pair));

    car(atom) = car_val;
    cdr(atom) = cdr_val;

    return atom;
}

Atom copy_list(Atom list) {
    Atom atom, p;

    if (is_nil(list)) {
        return NIL;
    }

    atom = cons(car(list), NIL);
    p = atom;
    list = cdr(list);

    while (!is_nil(list)) {
        cdr(p) = cons(car(list), NIL);
        p = cdr(p);
        list = cdr(list);
    }

    return atom;
}


void print_expr(Atom atom) {
    switch(atom.type) {
    case AtomType_Builtin:
        printf("#<BUILTIN:%p>", atom.value.builtin);
    case AtomType_Integer:
        printf("%ld", atom.value.integer);
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