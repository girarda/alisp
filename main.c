#include <stdio.h>
#include <editline/readline.h>
#include <editline/history.h>

#include "atom.h"
#include "error.h"
#include "parser.h"
#include "environment.h"

int main(int argc, char **argv)
{
    Atom env;
    char *input;

    env = create_env(NIL);

    while ((input = readline("> ")) != NULL) {
        add_history(input);
        const char *p = input;
        Error err;
        Atom expr, result;

        err = read_expr(p, &p, &expr);

        if (!err)
            err = eval_expr(expr, env, &result);

        switch (err) {
        case ERROR_OK:
            print_expr(result);
            putchar('\n');
            break;
        case ERROR_ARGS:
            puts("Wrong number of arguments");
            break;
        case ERROR_SYNTAX:
            puts("Syntax error");
            break;
        case ERROR_TYPE:
            puts("Wrong type");
            break;
        case ERROR_UNBOUND:
            puts("Symbol not bound");
            break;
        }


        free(input);
    }

    return 0;
}
