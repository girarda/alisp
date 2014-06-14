#include <string.h>
#include "parser.h"
#include "error.h"

int read_expr(const char *input, const char **end, Atom *result) {
    const char *token;
    Error err;

    err = lex(input, &token, end);
    if (err) {
        return err;
    }

    if (token[0] == '(') {
        return read_list(*end, end, result);
    } else if (token[0] == ')') {
        return ERROR_SYNTAX;
    } else if (token[0] == '\'') {
        *result = cons(make_sym("QUOTE"), cons(NIL, NIL));
        return read_expr(*end, end, &car(cdr(*result)));
    } else {
        return parse_simple(token, *end, result);
    }
}

int read_list(const char* start, const char **end, Atom *result) {
    Atom p;

    *end = start;
    p = *result = NIL;

    for (;;) {
        const char *token;
        Atom item;
        Error err;

        err = lex(*end, &token, end);
        if (err)
            return err;

        if (token[0] == ')')
            return ERROR_OK;

        if (token[0] == '.' && *end - token == 1) {
            /* Improper list */
            if (is_nil(p))
                return ERROR_SYNTAX;

            err = read_expr(*end, end, &item);
            if (err)
                return err;

            cdr(p) = item;

            /* Read the closing ')' */
            err = lex(*end, &token, end);
            if (!err && token[0] != ')')
                err = ERROR_SYNTAX;

            return err;
        }

        err = read_expr(token, end, &item);
        if (err)
            return err;

        if (is_nil(p)) {
            /* First item */
            *result = cons(item, NIL);
            p = *result;
        } else {
            cdr(p) = cons(item, NIL);
            p = cdr(p);
        }
    }
}

int parse_simple(const char *start, const char *end, Atom *result)
{
    char *buf, *p;

    /* Is it an integer? */
    long val = strtol(start, &p, 10);
    if (p == end) {
        result->type = AtomType_Integer;
        result->value.integer = val;
        return ERROR_OK;
    }

    /* NIL or symbol */
    buf = malloc(end - start + 1);
    p = buf;
    while (start != end)
        *p++ = toupper(*start), ++start;
    *p = '\0';

    if (strcmp(buf, "NIL") == 0)
        *result = NIL;
    else
        *result = make_sym(buf);

    free(buf);

    return ERROR_OK;
}

int lex(const char *str, const char **start, const char **end) {
    const char *ws = " \t\n";
    const char *delim = "() \t\n";
    const char *prefix = "()\'";

    str += strspn(str, ws);

    if (str[0] == '\0') {
        *start = *end = NULL;
        return ERROR_SYNTAX;
    }

    *start = str;

    if (strchr(prefix, str[0]) != NULL)
        *end = str + 1;
    else
        *end = str + strcspn(str, delim);

    return ERROR_OK;
}
