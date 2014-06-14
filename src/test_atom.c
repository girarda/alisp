#include "CuTest.h"
#include "atom.h"
#include "builtin.h"
#include "error.h"
#include "environment.h"
#include "util.h"

void test_make_builtin_creates_builtin_atom(CuTest* tc) {
    Atom atom = make_builtin(builtin_add);
    CuAssertTrue(tc, AtomType_Builtin == atom.type);
}

void test_make_builtin_creates_builtin_with_right_value(CuTest* tc) {
    Atom atom = make_builtin(builtin_add);
    CuAssertTrue(tc, builtin_add == atom.value.builtin);
}

void test_make_int_creates_integer_atom(CuTest* tc)
{
    long value = 42;
    Atom atom = make_int(value);
    CuAssertTrue(tc, AtomType_Integer == atom.type);
}

void test_make_int_creates_atom_with_right_value(CuTest* tc)
{
    long value = 42;
    Atom atom = make_int(value);
    CuAssertTrue(tc, value == atom.value.integer);
}

void test_make_sym_creates_symbol_atom(CuTest* tc)
{
    char *symbol = "Symbol";
    Atom atom = make_sym(symbol);
    CuAssertTrue(tc, AtomType_Symbol == atom.type);
}

void test_make_sym_creates_symbol_with_right_value(CuTest* tc)
{
    char *symbol = "Symbol";
    Atom atom = make_sym(symbol);

    int is_same_symbol = is_same_string(symbol, atom.value.symbol);

    CuAssertTrue(tc, is_same_symbol);
}

void test_make_sym_adds_new_symbol_to_symbol_table(CuTest* tc)
{
    char *new_symbol = "NewSymbol";
    Atom atom_in_table = look_for_symbol(new_symbol);
    CuAssertTrue(tc, is_nil(atom_in_table));

    make_sym(new_symbol);
    atom_in_table = look_for_symbol(new_symbol);

    CuAssertTrue(tc, !is_nil(atom_in_table));
}

void test_make_error_creates_error_atom(CuTest* tc)
{
    char *symbol = "Error";
    Atom atom = make_error(symbol);
    CuAssertTrue(tc, AtomType_Error == atom.type);
}

void test_make_error_creates_error_with_right_value(CuTest* tc)
{
    char *error = "Error";
    Atom atom = make_error(error);

    int is_same_error_msg = is_same_string(error, atom.value.error);

    CuAssertTrue(tc, is_same_error_msg);
}


void test_look_for_symbol_returns_nil_if_symbol_is_not_in_table(CuTest* tc)
{
    char *new_symbol = "NonExistingSymbol";
    Atom atom_in_table = look_for_symbol(new_symbol);
    CuAssertTrue(tc, is_nil(atom_in_table));
}

void test_look_for_symbol_returns_symbol_atom_if_symbol_is_in_table(CuTest* tc)
{
    char *new_symbol = "ExistingSymbol";
    make_sym(new_symbol);
    Atom atom_in_table = look_for_symbol(new_symbol);

    CuAssertTrue(tc, !is_nil(atom_in_table));
}

void test_add_symbol_to_table_adds_symbol_to_table(CuTest* tc)
{
    Atom atom;
    atom.type = AtomType_Symbol;
    char* symbol = "HandMadeSymbolAtom";
    atom.value.symbol = symbol;
    Atom atom_in_table = look_for_symbol(symbol);
    CuAssertTrue(tc, is_nil(atom_in_table));

    add_symbol_to_table(atom);
    atom_in_table = look_for_symbol(symbol);

    CuAssertTrue(tc, !is_nil(atom_in_table));
}

void test_cons_creates_atom_where_first_atom_is_car(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);

    Atom atom = cons(first, second);

    CuAssertTrue(tc, first.value.integer == car(atom).value.integer);
}

void test_cons_creates_atom_where_second_atom_is_cdr(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);

    Atom atom = cons(first, second);

    CuAssertTrue(tc, second.value.integer == cdr(atom).value.integer);
}

void test_cons_creates_pair_atom(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);

    Atom atom = cons(first, second);

    CuAssertTrue(tc, AtomType_Pair == atom.type);
}

void test_make_closure_returns_ERROR_OK_if_no_error_occurs(CuTest* tc) {
    Atom env, args, body, result;
    args = NIL;
    body = NIL;

    int error_code = make_closure(env, args, body, &result);

    CuAssertTrue(tc, ERROR_OK == error_code);
}

void test_make_closure_returns_ERROR_SYNTAX_if_body_is_not_valid_expression(CuTest* tc) {
    Atom env, args, body, result;
    args = NIL;
    body = make_int(42);

    int error_code = make_closure(env, args, body, &result);

    CuAssertTrue(tc, ERROR_SYNTAX == error_code);
    CuAssertTrue(tc, AtomType_Error == result.type);
}

void test_make_closure_returns_ERROR_TYPE_if_all_arg_names_are_not_symbols(CuTest* tc) {
    Atom env, args, body, result;
    args = cons(make_sym("make_int(42)"), make_int(42));
    body = NIL;
    int error_code = make_closure(env, args, body, &result);

    CuAssertTrue(tc, ERROR_TYPE == error_code);
    CuAssertTrue(tc, AtomType_Error == result.type);
}

void test_make_closure_creates_closure_atom(CuTest* tc) {
    Atom env, args, body, result;
    args = cons(make_sym("make_int(42)"), make_sym("hello"));
    body = NIL;

    make_closure(env, args, body, &result);

    CuAssertTrue(tc, AtomType_Closure == result.type);
}

void test_make_closure_creates_pair_where_first_atom_is_env(CuTest* tc) {
    Atom env, args, body, result;
    env = create_env(NIL);
    args = cons(make_sym("make_int(42)"), make_sym("hello"));
    body = NIL;

    make_closure(env, args, body, &result);

    CuAssertTrue(tc, car(env).type == car(result.value.pair->atom[0]).type);
    CuAssertTrue(tc, cdr(env).type == cdr(result.value.pair->atom[0]).type);
}

void test_make_closure_creates_pair_where_second_atom_is_pair_of_args_and_body(CuTest* tc) {
    Atom env, args, body, result;
    env = create_env(NIL);
    args = cons(make_sym("make_int(42)"), make_sym("hello"));
    body = NIL;

    make_closure(env, args, body, &result);

    CuAssertTrue(tc, strcmp(car(args).value.symbol, car(car(result.value.pair->atom[1])).value.symbol) == 0);
    CuAssertTrue(tc, strcmp(cdr(args).value.symbol, cdr(car(result.value.pair->atom[1])).value.symbol) == 0);
    CuAssertTrue(tc, body.type == cdr(result.value.pair->atom[1]).type);
    CuAssertTrue(tc, AtomType_Nil == cdr(result.value.pair->atom[1]).type);
}

void test_car_returns_first_element_of_pair(CuTest* tc) {
    long first_value = 24;
    long second_value = 42;
    Atom first = make_int(first_value);
    Atom second = make_int(second_value);
    Atom pair = cons(first, second);

    Atom atom = car(pair);

    CuAssertTrue(tc, first_value == atom.value.integer);
}

void test_cdr_returns_second_element_of_pair(CuTest* tc) {
    long first_value = 24;
    long second_value = 42;
    Atom first = make_int(first_value);
    Atom second = make_int(second_value);
    Atom pair = cons(first, second);

    Atom atom = cdr(pair);

    CuAssertTrue(tc, second_value == atom.value.integer);
}

void test_is_nil_returns_true_if_nil(CuTest* tc) {
    Atom atom = NIL;
    CuAssertTrue(tc, is_nil(atom));
}

void test_is_nil_returns_false_if_not_nil(CuTest* tc) {
    Atom atom = make_int(42);
    CuAssertTrue(tc, !is_pair(atom));
}

void test_is_closure_returns_true_if_atom_is_closure(CuTest* tc) {
    Atom env, args, body, result;
    env = create_env(NIL);
    args = cons(make_sym("make_int(42)"), make_sym("hello"));
    body = NIL;

    make_closure(env, args, body, &result);

    CuAssertTrue(tc, is_closure(result));
}

void test_is_closure_returns_false_if_not_closure(CuTest* tc) {
    Atom atom = make_int(42);
    CuAssertTrue(tc, !is_closure(atom));
}

void test_is_macro_returns_true_if_atom_is_macro(CuTest* tc) {
    Atom env, args, body, result;
    env = create_env(NIL);
    args = cons(make_sym("make_int(42)"), make_sym("hello"));
    body = NIL;

    make_closure(env, args, body, &result);
    result.type = AtomType_Macro;

    CuAssertTrue(tc, is_macro(result));
}

void test_is_macro_returns_false_if_atom_is_not_macro(CuTest* tc) {
    Atom env, args, body, result;
    env = create_env(NIL);
    args = cons(make_sym("make_int(42)"), make_sym("hello"));
    body = NIL;

    make_closure(env, args, body, &result);

    CuAssertTrue(tc, !is_macro(result));
}

void test_is_pair_returns_true_if_atom_is_pair(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);
    Atom pair = cons(first, second);

    CuAssertTrue(tc, is_pair(pair));
}

void test_is_pair_returns_false_if_atom_is_not_pair(CuTest* tc) {
    Atom atom = make_int(24);

    CuAssertTrue(tc, !is_pair(atom));
}

void test_is_symbol_returns_true_if_atom_is_symbol(CuTest* tc) {
    Atom symbol = make_sym("ASymbol");

    CuAssertTrue(tc, is_symbol(symbol));
}

void test_is_symbol_returns_false_if_atom_is_not_symbol(CuTest* tc) {
    Atom atom = make_int(24);

    CuAssertTrue(tc, !is_symbol(atom));
}

void test_is_error_returns_true_if_atom_is__error(CuTest* tc) {
    Atom atom = make_error("Error");

    CuAssertTrue(tc, !is_error(atom));
}

void test_is_error_returns_false_if_atom_is_not_error(CuTest* tc) {
    Atom atom = make_int(24);

    CuAssertTrue(tc, !is_error(atom));
}

void test_has_children_returns_true_if_atom_is_closure(CuTest* tc) {
    Atom env, args, body, result;
    env = create_env(NIL);
    args = cons(make_sym("make_int(42)"), make_sym("hello"));
    body = NIL;

    make_closure(env, args, body, &result);

    CuAssertTrue(tc, has_children(result));
}

void test_has_children_returns_true_if_atom_is_macro(CuTest* tc) {
    Atom env, args, body, result;
    env = create_env(NIL);
    args = cons(make_sym("make_int(42)"), make_sym("hello"));
    body = NIL;

    make_closure(env, args, body, &result);
    result.type = AtomType_Macro;

    CuAssertTrue(tc, has_children(result));
}

void test_has_children_returns_true_if_atom_is_pair(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);
    Atom pair = cons(first, second);

    CuAssertTrue(tc, has_children(pair));
}

void test_has_children_returns_false_if_atom_is_not_closure_nor_macro_nor_pair(CuTest* tc) {
    Atom atom = make_int(24);
    CuAssertTrue(tc, !has_children(atom));
}

CuSuite* AtomGetSuite(void) {
    CuSuite* suite = CuSuiteNew();

    /* make_builtin */
    SUITE_ADD_TEST(suite, test_make_builtin_creates_builtin_atom);
    SUITE_ADD_TEST(suite, test_make_builtin_creates_builtin_with_right_value);

    /* make_int */
    SUITE_ADD_TEST(suite, test_make_int_creates_integer_atom);
    SUITE_ADD_TEST(suite, test_make_int_creates_atom_with_right_value);

    /* make_sym */
    SUITE_ADD_TEST(suite, test_make_sym_creates_symbol_atom);
    SUITE_ADD_TEST(suite, test_make_sym_creates_symbol_with_right_value);
    SUITE_ADD_TEST(suite, test_make_sym_adds_new_symbol_to_symbol_table);

    /* make_error */
    SUITE_ADD_TEST(suite, test_make_error_creates_error_atom);
    SUITE_ADD_TEST(suite, test_make_error_creates_error_with_right_value);

    /* look_for_symbol */
    SUITE_ADD_TEST(suite, test_look_for_symbol_returns_nil_if_symbol_is_not_in_table);
    SUITE_ADD_TEST(suite, test_look_for_symbol_returns_symbol_atom_if_symbol_is_in_table);

    /* add_symbol_to_table */
    SUITE_ADD_TEST(suite, test_add_symbol_to_table_adds_symbol_to_table);

    /* cons */
    SUITE_ADD_TEST(suite, test_cons_creates_atom_where_first_atom_is_car);
    SUITE_ADD_TEST(suite, test_cons_creates_atom_where_second_atom_is_cdr);
    SUITE_ADD_TEST(suite, test_make_closure_creates_pair_where_second_atom_is_pair_of_args_and_body);

    /* make_closure */
    SUITE_ADD_TEST(suite, test_make_closure_returns_ERROR_OK_if_no_error_occurs);
    SUITE_ADD_TEST(suite, test_make_closure_returns_ERROR_SYNTAX_if_body_is_not_valid_expression);
    SUITE_ADD_TEST(suite, test_make_closure_returns_ERROR_TYPE_if_all_arg_names_are_not_symbols);
    SUITE_ADD_TEST(suite, test_make_closure_creates_closure_atom);
    SUITE_ADD_TEST(suite, test_make_closure_creates_pair_where_first_atom_is_env);
    SUITE_ADD_TEST(suite, test_make_closure_creates_pair_where_second_atom_is_pair_of_args_and_body);

    /* car */
    SUITE_ADD_TEST(suite, test_car_returns_first_element_of_pair);

    /* cdr */
    SUITE_ADD_TEST(suite, test_cdr_returns_second_element_of_pair);

    /* is_nil */
    SUITE_ADD_TEST(suite, test_is_nil_returns_true_if_nil);
    SUITE_ADD_TEST(suite, test_is_nil_returns_false_if_not_nil);

    /* is_closure */
    SUITE_ADD_TEST(suite, test_is_closure_returns_true_if_atom_is_closure);
    SUITE_ADD_TEST(suite, test_is_pair_returns_false_if_atom_is_not_pair);

    /* is_macro */
    SUITE_ADD_TEST(suite, test_is_macro_returns_true_if_atom_is_macro);
    SUITE_ADD_TEST(suite, test_is_macro_returns_false_if_atom_is_not_macro);

    /* is_pair */
    SUITE_ADD_TEST(suite, test_is_pair_returns_true_if_atom_is_pair);
    SUITE_ADD_TEST(suite, test_is_pair_returns_false_if_atom_is_not_pair);

    /* is_symbol */
    SUITE_ADD_TEST(suite, test_is_symbol_returns_true_if_atom_is_symbol);
    SUITE_ADD_TEST(suite, test_is_symbol_returns_false_if_atom_is_not_symbol);

    /* is_error */
    SUITE_ADD_TEST(suite, test_is_error_returns_true_if_atom_is__error);
    SUITE_ADD_TEST(suite, test_is_error_returns_false_if_atom_is_not_error);


    /* has_children */
    SUITE_ADD_TEST(suite, test_has_children_returns_true_if_atom_is_closure);
    SUITE_ADD_TEST(suite, test_has_children_returns_true_if_atom_is_macro);
    SUITE_ADD_TEST(suite, test_has_children_returns_true_if_atom_is_pair);
    SUITE_ADD_TEST(suite, test_has_children_returns_false_if_atom_is_not_closure_nor_macro_nor_pair);

    return suite;
}
