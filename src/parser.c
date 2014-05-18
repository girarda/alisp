#include <string.h>
#include "parser.h"
#include "error.h"

int read_expr(const char *input, const char **end, Atom *result) {
    const char *token;
    Error err = lex(input, &token, end);

    if (err) {
        return err;
    }

    if (token[0] == '(') {
        return read_list(*end, end, result);
    } else if (token[0] == ')') {
        return ERROR_SYNTAX;
    } else {
        return parse_simple(token, *end, result);
    }
}

int read_list(const char* start, const char **end, Atom *result) {
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

int parse_simple(const char *start, const char *end, Atom *result) {
    char *buffer, *p;

    /* Is it an integer? */
    long val = strtol(start, &p, 10);
    if (p == end) {
        result->type = AtomType_Integer;
        result->value.integer = val;
        return ERROR_OK;
    }

    /* NIL or symbol */
    buffer = malloc(end - start + 1);
    p = buffer;
    while (start != end) {
        *p++ = *start, ++start;
    }
    *p = '\0';
    if (strcmp(buffer, "nil") == 0) {
        *result = NIL;
    } else {
        *result = make_sym(buffer);
    }

    free(buffer);

    return ERROR_OK;
}

int lex(const char *str, const char **start, const char **end) {
    const char *word_stop = " \t\n";
    const char *delimiter = "() \t\n";
    const char *prefix = "()";

    str += strspn(str, word_stop);

    if (str[0] == '\0') {
        *start = *end = NULL;
        return ERROR_SYNTAX;
    }

    *start = str;

    if (strchr(prefix, str[0]) != NULL) {
        *end = str + 1;
    } else {
        *end = str + strcspn(str, delimiter);
    }

    return ERROR_OK;
}

