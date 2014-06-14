#include <string.h>
#include "parser.h"
#include "error.h"
#include "util.h"

int read_expr(const char *input, const char **end, Atom *result) {
    const char *token;
    Error err;

    err = lex(input, &token, end);
    if (err) {
        *result = make_error(""); /* no expression to read */
        return err;
    }

    if (token[0] == '(') { /* TODO: Test this case */
        return read_list(*end, end, result);
    } else if (token[0] == ')') {
        *result = make_error("Error syntax: read_expr");
        return ERROR_SYNTAX;
    } else if (token[0] == '\'') { /* TODO: Test this case */
        *result = cons(make_sym("QUOTE"), cons(NIL, NIL));
        return read_expr(*end, end, &car(cdr(*result)));
    } else {
        return parse_simple(token, *end, result);
    }
}

int read_list(const char* start, const char **end, Atom *result) {
    /* Test this method */
    Atom atom;

    *end = start;
    atom = *result = NIL;

    for (;;) {
        const char *token;
        Atom item;
        Error err;

        err = lex(*end, &token, end);
        if (err) {
            return err;
        }

        if (token[0] == ')') {
            return ERROR_OK;
        }

        if (token[0] == '.' && *end - token == 1) {
            /* Improper list */
            if (is_nil(atom)) {
                *result = make_error("Error syntax: read_list");
                return ERROR_SYNTAX;
            }

            err = read_expr(*end, end, &item);
            if (err) {
                return err;
            }

            cdr(atom) = item;

            /* Read the closing ')' */
            err = lex(*end, &token, end);
            if (!err && token[0] != ')') {
                *result = make_error("Error syntax: read_list");
                err = ERROR_SYNTAX;
            }

            return err;
        }

        err = read_expr(token, end, &item);
        if (err) {
            return err;
        }

        if (is_nil(atom)) {
            /* First item */
            *result = cons(item, NIL);
            atom = *result;
        } else {
            cdr(atom) = cons(item, NIL);
            atom = cdr(atom);
        }
    }
}

int parse_simple(const char *start, const char *end, Atom *result)
{
    Error is_integer = parse_integer(start, end, result);
    if (ERROR_OK == is_integer) {
        return ERROR_OK;
    }

    return parse_symbol_or_nil(start, end, result);
}

int parse_integer(const char *start, const char *end, Atom *result) {
    char *ptr;
    long val = strtol(start, &ptr, 10);

    if (ptr == end) {
        *result = make_int(val);
        return ERROR_OK;
    }
    *result = make_error("Error syntax: parse_integer");
    return ERROR_SYNTAX;
}

int parse_symbol_or_nil(const char *start, const char *end, Atom *result) {
    char *buffer;
    char *ptr;

    buffer = malloc(end - start + 1);
    ptr = buffer;
    while (start != end) {
        *ptr++ = toupper(*start);
        ++start;
    }

    if (is_same_string(buffer, "NIL"))
        *result = NIL;
    else
        *result = make_sym(buffer);

    free(buffer);

    return ERROR_OK;
}

int lex(const char *str, const char **start, const char **end) {
    const char *word_stop = " \t\n";
    const char *delimiter = "() \t\n";
    const char *prefix = "()\'";

    str += strspn(str, word_stop);

    if (str[0] == '\0') {
        *start = *end = NULL;
        return ERROR_SYNTAX;
    }

    *start = str;

    if (starts_with_prefix(str, prefix))
        /* Skip prefix */
        *end = str + 1;
    else
        *end = str + strcspn(str, delimiter);

    return ERROR_OK;
}

int starts_with_prefix(const char *str, const char* prefix) {
    return strchr(prefix, str[0]) != NULL;
}
