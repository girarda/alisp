#include <stdio.h>
#include <editline/readline.h>
#include <editline/history.h>

#include "atom.h"
#include "error.h"
#include "parser.h"
#include "environment.h"
#include "builtin.h"
#include "expression.h"

int main(int argc, char **argv)
{
    Atom env;
    char *input;

    env = create_env(NIL);

    /* Set up initial environment */
    add_binding_env(env, make_sym("CAR"), make_builtin(builtin_car));
    add_binding_env(env, make_sym("CDR"), make_builtin(builtin_cdr));
    add_binding_env(env, make_sym("CONS"), make_builtin(builtin_cons));

    add_binding_env(env, make_sym("+"), make_builtin(builtin_add));
    add_binding_env(env, make_sym("-"), make_builtin(builtin_substract));
    add_binding_env(env, make_sym("*"), make_builtin(builtin_multiply));
    add_binding_env(env, make_sym("/"), make_builtin(builtin_divide));

    add_binding_env(env, make_sym("T"), make_sym("T"));

    add_binding_env(env, make_sym("="), make_builtin(builtin_numeq));
    add_binding_env(env, make_sym("<"), make_builtin(builtin_less));

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
