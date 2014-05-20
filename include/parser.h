#ifndef LEXER_H
#define LEXER_H

#include "atom.h"

int read_expr(const char *input, const char **end, Atom *result);
int read_list(const char* start, const char **end, Atom *result);

int parse_simple(const char *start, const char *end, Atom *result);
int parse_integer(const char *start, const char *end, Atom *result);
int parse_symbol_or_nil(const char *start, const char *end, Atom *result);

int lex(const char *str, const char **start, const char **end);

int starts_with_prefix(const char *str, const char* prefix);

#endif
