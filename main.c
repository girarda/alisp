#include <stdio.h>
#include <editline/readline.h>
#include <editline/history.h>

#include "atom.h"
#include "error.h"
#include "parser.h"
#include "environment.h"
#include "builtin.h"
#include "expression.h"
#include "gc.h"
#include "io.h"

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

    add_binding_env(env, make_sym("="), make_builtin(builtin_apply));
    add_binding_env(env, make_sym("<"), make_builtin(builtin_eq));
    add_binding_env(env, make_sym("<"), make_builtin(builtin_is_pair));

    load_file(env, "std.lisp");

    while ((input = readline("> ")) != NULL) {
        if (strcmp(input, "exit") == 0 ) {
            gc_clear_marks();
            gc_free_unmarked();
            free(input);
            return 0;
        }
        if (strcmp(input, "") == 0) {
            free(input);
            continue;
        }
        add_history(input);
        const char *p = input;
        Atom expr, result;

        read_expr(p, &p, &expr);

        if (!is_error(expr)) {
            eval_expr(expr, env, &result);
            print_expr(result);
        } else {
            print_expr(expr);
            putchar('\n');
        }

        putchar('\n');

        free(input);
    }

    return 0;
}
