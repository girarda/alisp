#include <stdio.h>
#include <editline/readline.h>
#include <editline/history.h>

#include "atom.h"
#include "error.h"
#include "parser.h"

int main(int argc, char** argv) {
    char *input;

//    input = "(test)";
//    const char *p = input;

//    Error err;
//    Atom expr;

//    err = read_expr(p, &p, &expr);

//    switch (err) {
//    case ERROR_OK:
//        print_expr((expr));
//        putchar('\n');
//        break;
//    case ERROR_SYNTAX:
//        puts("Syntax error");
//        break;
//    }

    while ((input = readline("> ")) != NULL) {
        add_history(input);
        const char *p = input;
        Error err;
        Atom expr;

        err = read_expr(p, &p, &expr);

        switch (err) {
        case ERROR_OK:
            print_expr((expr));
            putchar('\n');
            break;
        case ERROR_SYNTAX:
            puts("Syntax error");
            break;
        }

        free(input);
    }

    return 0;
}

