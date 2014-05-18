#include <stdio.h>
#include <editline/readline.h>
#include <editline/history.h>

#include "atom.h"

int main(int argc, char** argv) {
    Atom atom = cons(make_sym("foo"), make_int(2));
    print_expr(atom);
    return 0;
}

