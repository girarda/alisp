#include "CuTest.h"
#include "environment.h"
#include "error.h"

void test_create_env_from_nil_creates_pair(CuTest* tc) {
    Atom atom = create_env(NIL);
    CuAssertTrue(tc, AtomType_Pair == atom.type);
}

void test_create_env_has_first_elem_as_parent(CuTest* tc) {
    Atom atom = create_env(NIL);
    CuAssertTrue(tc, AtomType_Nil == car(atom).type);
}

void test_create_env_second_element_is_nil(CuTest* tc) {
    Atom atom = create_env(NIL);
    CuAssertTrue(tc, AtomType_Nil == car(atom).type);
}

void test_retrieve_env_finds_value_associated_to_symbol(CuTest* tc) {
    int value = 42;
    char *symbol = "AssociatedSymbol";
    Atom value_atom = make_int(value);
    Atom symbol_atom = make_sym(symbol);
    Atom env = create_env(NIL);
    add_binding_env(env, symbol_atom, value_atom);

    Atom get_atom;
    retrieve_env(env, symbol_atom, &get_atom);

    CuAssertTrue(tc, value == get_atom.value.integer);
}

void test_retrieve_env_returns_ERROR_OK_if_the_symbol_is_binded_in_the_env(CuTest* tc) {
    int value = 42;
    char *symbol = "AssociatedSym";
    Atom value_atom = make_int(value);
    Atom symbol_atom = make_sym(symbol);
    Atom env = create_env(NIL);
    add_binding_env(env, symbol_atom, value_atom);

    Atom get_atom;
    int error_code = retrieve_env(env, symbol_atom, &get_atom);

    CuAssertTrue(tc, ERROR_OK == error_code);
}

void test_retrieve_env_returns_ERROR_UNBOUND_if_the_symbol_is_not_binded_in_the_env(CuTest* tc) {
    char *symbol = "UnassociatedSymbol";
    Atom symbol_atom = make_sym(symbol);
    Atom env = create_env(NIL);

    Atom get_atom;
    int error_code = retrieve_env(env, symbol_atom, &get_atom);

    CuAssertTrue(tc, ERROR_UNBOUND == error_code);
    CuAssertTrue(tc, AtomType_Error == get_atom.type);
}

void test_retrieve_env_can_find_value_binded_in_parent_env(CuTest* tc) {
    int value = 42;
    Atom value_atom = make_int(value);
    char *symbol = "NewAssociatedSymbol";
    Atom symbol_atom = make_sym(symbol);
    Atom parent = create_env(NIL);
    Atom env = create_env(parent);
    add_binding_env(parent, symbol_atom, value_atom);

    Atom get_atom;
    int error_code = retrieve_env(env, symbol_atom, &get_atom);

    CuAssertTrue(tc, ERROR_OK == error_code);
}

void test_add_binding_env_returns_ERROR_OK(CuTest* tc) {
    int value = 42;
    Atom value_atom = make_int(value);
    char *symbol = "AssociatedSymbol";
    Atom symbol_atom = make_sym(symbol);
    Atom env = create_env(NIL);
    int error_code = add_binding_env(env, symbol_atom, value_atom);

    CuAssertTrue(tc, ERROR_OK == error_code);
}

void test_add_binding_adds_symbol_to_environment(CuTest* tc) {
    int value = 42;
    Atom value_atom = make_int(value);
    char *symbol = "UnassociatedSymbol";
    Atom symbol_atom = make_sym(symbol);
    Atom env = create_env(NIL);
    Atom retrieved_atom;
    int error_code = retrieve_env(env, symbol_atom, &retrieved_atom);
    CuAssertTrue(tc, ERROR_UNBOUND == error_code);

    add_binding_env(env, symbol_atom, value_atom);

    error_code = retrieve_env(env, symbol_atom, &retrieved_atom);

    CuAssertTrue(tc, ERROR_OK == error_code);
}

void test_add_binding_changes_value_if_symbol_already_in_env(CuTest* tc) {
    int old_value = 24;
    int new_value = 42;
    Atom value_atom = make_int(old_value);
    char *symbol = "sym";
    Atom symbol_atom = make_sym(symbol);
    Atom env = create_env(NIL);
    Atom retrieved_atom;
    add_binding_env(env, symbol_atom, value_atom);
    retrieve_env(env, symbol_atom, &retrieved_atom);
    CuAssertTrue(tc, old_value == retrieved_atom.value.integer);

    value_atom.value.integer = new_value;
    add_binding_env(env, symbol_atom, value_atom);
    retrieve_env(env, symbol_atom, &retrieved_atom);

    CuAssertTrue(tc, new_value == retrieved_atom.value.integer);
}

CuSuite* EnvironmentGetSuite(void)
{
    CuSuite* suite = CuSuiteNew();

    /* create_env */
    SUITE_ADD_TEST(suite, test_create_env_from_nil_creates_pair);
    SUITE_ADD_TEST(suite, test_create_env_has_first_elem_as_parent);
    SUITE_ADD_TEST(suite, test_create_env_second_element_is_nil);

    /* retrieve env */
    SUITE_ADD_TEST(suite, test_retrieve_env_finds_value_associated_to_symbol);
    SUITE_ADD_TEST(suite, test_retrieve_env_returns_ERROR_OK_if_the_symbol_is_binded_in_the_env);
    SUITE_ADD_TEST(suite, test_retrieve_env_returns_ERROR_UNBOUND_if_the_symbol_is_not_binded_in_the_env);
    SUITE_ADD_TEST(suite, test_retrieve_env_can_find_value_binded_in_parent_env);

    /* add_binding_env */
    SUITE_ADD_TEST(suite, test_add_binding_env_returns_ERROR_OK);
    SUITE_ADD_TEST(suite, test_add_binding_adds_symbol_to_environment);
    SUITE_ADD_TEST(suite, test_add_binding_changes_value_if_symbol_already_in_env);

    return suite;
}
