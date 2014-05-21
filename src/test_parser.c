#include "CuTest.h"
#include "parser.h"
#include "error.h"
#include "util.h"

void test_parse_integer_returns_ERROR_OK_if_it_parsed_a_number(CuTest* tc) {
    Atom result;
    char *str = "23";

    int error_code = parse_integer(str, str+2, &result);

    CuAssertTrue(tc, ERROR_OK == error_code);
}

void test_parse_integer_changes_the_result_atom_to_right_integer_type(CuTest* tc) {
    Atom result;
    char *str = "23";

    parse_integer(str, str+2, &result);

    CuAssertTrue(tc, AtomType_Integer == result.type);
}

void test_parse_integer_changes_the_result_atom_to_right_value(CuTest* tc) {
    Atom result;
    char *str = "23";

    parse_integer(str, str+2, &result);

    CuAssertTrue(tc, 23 == result.value.integer);
}

void test_parse_integer_returns_ERROR_SYNTAX_if_it_did_not_parse_a_number(CuTest* tc) {
    Atom result;
    char *str = "a";

    int error_code = parse_integer(str, str+2, &result);

    CuAssertTrue(tc, ERROR_SYNTAX == error_code);
}

void test_parse_integer_returns_ERROR_SYNTAX_if_end_does_not_match_end_of_character(CuTest* tc) {
    Atom result;
    char *str = "23";

    int error_code = parse_integer(str, str+1, &result);

    CuAssertTrue(tc, ERROR_SYNTAX == error_code);
}

void test_parse_symbol_or_nil_returns_RESULT_OK(CuTest* tc) {
    Atom result;
    char *str = "nil";

    int error_code = parse_symbol_or_nil(str, str+3, &result);

    CuAssertTrue(tc, ERROR_OK == error_code);
}

void test_parse_symbol_or_nil_sets_result_atom_to_nil_if_it_read_exactly_nil(CuTest* tc) {
    Atom result;
    char *str = "nil";

    parse_symbol_or_nil(str, str+3, &result);

    CuAssertTrue(tc, AtomType_Nil == result.type);
}

void test_parse_symbol_or_nil_does_not_set_result_atom_to_nil_if_it_does_not_read_exactly_nil(CuTest* tc) {
    Atom result;
    char *str = "nile";

    parse_symbol_or_nil(str, str+4, &result);

    CuAssertTrue(tc, AtomType_Nil != result.type);
}

void test_parse_symbol_or_nil_sets_result_atom_to_symbol_if_it_does_not_read_exactly_nil(CuTest* tc) {
    Atom result;
    char *str = "nile";

    parse_symbol_or_nil(str, str+4, &result);

    CuAssertTrue(tc, AtomType_Symbol == result.type);
}

void test_parse_symbol_or_nil_sets_result_atom_to_symbol_with_right_value_if_it_does_not_read_exactly_nil(CuTest* tc) {
    Atom result;
    char *str = "NILE";

    parse_symbol_or_nil(str, str+4, &result);

    CuAssertTrue(tc, is_same_string(str, result.value.symbol));
}

void test_parse_symbol_or_nil_converts_symbol_to_maj(CuTest* tc) {
    Atom result;
    char *str = "nile";

    parse_symbol_or_nil(str, str+4, &result);

    CuAssertTrue(tc, is_same_string("NILE", result.value.symbol));
}

void parse_simple_returns_ERROR_OK(CuTest* tc) {
    Atom result;
    char *str = "str";

    int error_code = parse_simple(str, str + 3, &result);

    CuAssertTrue(tc, ERROR_OK == error_code);
}

void test_parse_simple_changes_the_result_atom_to_right_integer_atom_if_read_integer(CuTest* tc) {
    Atom result;
    char *str = "23";

    parse_simple(str, str + 2, &result);

    CuAssertTrue(tc, AtomType_Integer == result.type);
    CuAssertTrue(tc, 23 == result.value.integer);
}

void test_parse_simple_changes_the_result_atom_to_nil_if_read_nil(CuTest* tc) {
    Atom result;
    char *str = "nil";

    parse_simple(str, str + 3, &result);

    CuAssertTrue(tc, AtomType_Nil == result.type);
}

void test_parse_simple_changes_the_result_atom_to_right_symbol_atom(CuTest* tc) {
    Atom result;
    char *str = "nile";

    parse_simple(str, str + 4, &result);

    CuAssertTrue(tc, AtomType_Symbol == result.type);
    CuAssertTrue(tc, is_same_string("NILE", result.value.symbol));
}

void test_lex_returns_ERROR_SYNTAX_if_string_begins_by_NULL_terminator(CuTest* tc) {
    char *str = "\0";

    const char *start;
    const char *end;

    int error_code= lex(str, &start, &end);

    CuAssertTrue(tc, ERROR_SYNTAX == error_code);
}

void test_lex_sets_start_and_end_to_NULL_if_invalid_string(CuTest* tc) {
    char *str = "\0";

    const char *start;
    const char *end;

   lex(str, &start, &end);

   CuAssertTrue(tc, NULL == start);
   CuAssertTrue(tc, NULL == end);
}

void test_lex_finds_beginning_and_end_of_first_word(CuTest* tc) {
    char *str = "hello world";

    const char *start;
    const char *end;

    lex(str, &start, &end);

    CuAssertTrue(tc, str == start);
    CuAssertTrue(tc, str + 5 == end);
}

void test_lex_skips_prefixes(CuTest* tc) {
    char *str = "(hello world";

    const char *start;
    const char *end;

    lex(str, &start, &end);

    CuAssertTrue(tc, str == start);
    CuAssertTrue(tc, str + 1 == end);
}

void test_starts_with_prefix_returns_true_if_first_char_is_in_prefix_string(CuTest* tc) {
    char *str = "(hello world";
    const char *prefix = "()\'";

    CuAssertTrue(tc, starts_with_prefix(str, prefix));
}

void test_starts_with_prefix_returns_false_if_first_char_is_not_in_prefix_string(CuTest* tc) {
    char *str = "hello world";
    const char *prefix = "()\'";

    CuAssertTrue(tc, !starts_with_prefix(str, prefix));
}

void test_read_expr_returns_ERROR_SYNTAX_if_string_starts_with_NULL_terminator(CuTest* tc) {
    Atom result;
    char *str = "\0";

    const char *end;

    int error_code= read_expr(str, &end, &result);

    CuAssertTrue(tc, ERROR_SYNTAX == error_code);
}

void test_read_expr_returns_ERROR_SYNTAX_if_string_starts_with_closing_parenthesis(CuTest* tc) {
    Atom result;
    char *str = ")";

    const char *end;

    int error_code= read_expr(str, &end, &result);

    CuAssertTrue(tc, ERROR_SYNTAX == error_code);
}

void test_read_expr_returns_ERROR_OK_if_valid_expression(CuTest* tc) {
    Atom result;
    char *str = "42";

    const char *end;

    int error_code= read_expr(str, &end, &result);

    CuAssertTrue(tc, ERROR_OK == error_code);
}

void test_read_expr_sets_atom_correctly_if_simple_expression(CuTest* tc) {
    Atom result;
    char *str = "42";

    const char *end;

    read_expr(str, &end, &result);

    CuAssertTrue(tc, AtomType_Integer == result.type);
    CuAssertTrue(tc, 42 == result.value.integer);}

CuSuite* ParserGetSuite(void)
{
    CuSuite* suite = CuSuiteNew();

    /* rest_expr */
    SUITE_ADD_TEST(suite, test_read_expr_returns_ERROR_SYNTAX_if_string_starts_with_NULL_terminator);
    SUITE_ADD_TEST(suite, test_read_expr_returns_ERROR_SYNTAX_if_string_starts_with_closing_parenthesis);
    SUITE_ADD_TEST(suite, test_read_expr_returns_ERROR_OK_if_valid_expression);
    SUITE_ADD_TEST(suite, test_read_expr_sets_atom_correctly_if_simple_expression);

    /* parse_integer */
    SUITE_ADD_TEST(suite, test_parse_integer_returns_ERROR_OK_if_it_parsed_a_number);
    SUITE_ADD_TEST(suite, test_parse_integer_returns_ERROR_SYNTAX_if_it_did_not_parse_a_number);
    SUITE_ADD_TEST(suite, test_parse_integer_returns_ERROR_SYNTAX_if_end_does_not_match_end_of_character);
    SUITE_ADD_TEST(suite, test_parse_integer_changes_the_result_atom_to_right_integer_type);
    SUITE_ADD_TEST(suite, test_parse_integer_changes_the_result_atom_to_right_value);

    /* parse_symbol_or_nil */
    SUITE_ADD_TEST(suite, test_parse_symbol_or_nil_returns_RESULT_OK);
    SUITE_ADD_TEST(suite, test_parse_symbol_or_nil_sets_result_atom_to_nil_if_it_read_exactly_nil);
    SUITE_ADD_TEST(suite, test_parse_symbol_or_nil_does_not_set_result_atom_to_nil_if_it_does_not_read_exactly_nil);
    SUITE_ADD_TEST(suite, test_parse_symbol_or_nil_sets_result_atom_to_symbol_if_it_does_not_read_exactly_nil);
    SUITE_ADD_TEST(suite, test_parse_symbol_or_nil_sets_result_atom_to_symbol_with_right_value_if_it_does_not_read_exactly_nil);
    SUITE_ADD_TEST(suite, test_parse_symbol_or_nil_converts_symbol_to_maj);

    /* parse_simple */
    SUITE_ADD_TEST(suite, parse_simple_returns_ERROR_OK);
    SUITE_ADD_TEST(suite, test_parse_simple_changes_the_result_atom_to_right_integer_atom_if_read_integer);
    SUITE_ADD_TEST(suite, test_parse_simple_changes_the_result_atom_to_nil_if_read_nil);
    SUITE_ADD_TEST(suite, test_parse_simple_changes_the_result_atom_to_right_symbol_atom);

    /* lex */
    SUITE_ADD_TEST(suite, test_lex_returns_ERROR_SYNTAX_if_string_begins_by_NULL_terminator);
    SUITE_ADD_TEST(suite, test_lex_sets_start_and_end_to_NULL_if_invalid_string);
    SUITE_ADD_TEST(suite, test_lex_finds_beginning_and_end_of_first_word);
    SUITE_ADD_TEST(suite, test_lex_skips_prefixes);

    /* starts_with_prefix */
    SUITE_ADD_TEST(suite, test_starts_with_prefix_returns_true_if_first_char_is_in_prefix_string);
    SUITE_ADD_TEST(suite, test_starts_with_prefix_returns_false_if_first_char_is_not_in_prefix_string);


    return suite;
}

