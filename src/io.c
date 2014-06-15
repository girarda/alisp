#include "io.h"
#include "parser.h"
#include "error.h"
#include <stdio.h>

char *slurp(const char *path) {
    FILE *file;
        char *buf;
        long len;

        file = fopen(path, "r");
        if (!file)
            return NULL;
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        fseek(file, 0, SEEK_SET);

        buf = malloc(len);
        if (!buf)
            return NULL;

        fread(buf, 1, len, file);
        fclose(file);

        return buf;
}

Atom load_file(Atom env, const char *path)
{
    char *text;

    printf("Reading %s...\n", path);
    text = slurp(path);
    Atom expr;
    if (text) {
        const char *p = text;
        while (read_expr(p, &p, &expr) == ERROR_OK) {
            Atom result;
            Error err = eval_expr(expr, env, &result);
            if (err) {
                printf("Error in expression:\n\t");
                print_expr(expr);
                putchar('\n');
            } else {
                print_expr(result);
                putchar('\n');
            }
        }
        free(text);
    } else {
        expr = make_error("IO Error: Could not load file.");
    }
    return expr;
}
