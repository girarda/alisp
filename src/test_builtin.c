#include "CuTest.h"
#include "atom.h"
#include "builtin.h"
#include "error.h"
#include "util.h"

void test_builtin_car_returns_ERROR_OK_if_args_is_a_cons_of_cons_NIL(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);
    Atom pair = cons(cons(first, second), NIL);

    Atom atom_car;
    int result = builtin_car(pair, &atom_car);

    CuAssertTrue(tc, ERROR_OK == result);
}

void test_builtin_car_returns_ERROR_ARGS_if_args_is_not_a_cons_of_cons(CuTest* tc) {
    Atom atom = cons(make_int(24), make_int(42));

    Atom atom_car;
    int result = builtin_car(atom, &atom_car);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_car.type);
}

void test_builtin_car_returns_the_first_element_of_a_pair(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);
    Atom pair = cons(cons(first, second), NIL);

    Atom atom_car;
    builtin_car(pair, &atom_car);

    CuAssertTrue(tc, atom_car.value.integer == 24);
    CuAssertTrue(tc, atom_car.type == AtomType_Integer);
}

void test_builtin_car_returns_NIL_if_first_element_of_pair_is_nil(CuTest* tc) {
    Atom first = NIL;
    Atom second = make_int(42);
    Atom pair = cons(cons(first, second), NIL);

    Atom atom_car;
    int result = builtin_car(pair, &atom_car);

    CuAssertTrue(tc, AtomType_Nil == atom_car.type);
}

void test_builtin_car_returns_ERROR_TYPE_if_first_element_is_not_a_pair(CuTest* tc) {
    Atom first = NIL;
    Atom pair = cons(first, NIL);

    Atom atom_car;
    builtin_car(pair, &atom_car);

    CuAssertTrue(tc, atom_car.type == AtomType_Nil);
}

void test_builtin_cdr_returns_ERROR_OK_if_args_is_a_cons_of_cons_NIL(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);
    Atom pair = cons(cons(first, second), NIL);

    Atom atom_cdr;
    int result = builtin_cdr(pair, &atom_cdr);

    CuAssertTrue(tc, ERROR_OK == result);
}

void test_builtin_cdr_returns_ERROR_ARGS_if_args_is_not_a_cons_of_cons(CuTest* tc) {
    Atom atom = cons(make_int(24), make_int(42));

    Atom atom_cdr;
    int result = builtin_cdr(atom, &atom_cdr);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_cdr.type);
}

void test_builtin_cdr_returns_the_first_element_of_a_pair(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);
    Atom pair = cons(cons(first, second), NIL);

    Atom atom_cdr;
    builtin_cdr(pair, &atom_cdr);

    CuAssertTrue(tc, atom_cdr.value.integer == 42);
    CuAssertTrue(tc, atom_cdr.type == AtomType_Integer);
}

void test_builtin_cdr_returns_NIL_if_first_element_of_pair_is_nil(CuTest* tc) {
    Atom first = make_int(42);
    Atom second = NIL;
    Atom pair = cons(cons(first, second), NIL);

    Atom atom_cdr;
    builtin_cdr(pair, &atom_cdr);

    CuAssertTrue(tc, atom_cdr.type == AtomType_Nil);
}

void test_builtin_cons_returns_ERROR_ARGS_if_args_is_NIL(CuTest* tc) {
    Atom atom_cons;
    int result = builtin_cons(NIL, &atom_cons);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, atom_cons.type == AtomType_Error);
}

void test_builtin_cons_returns_ERROR_OK(CuTest* tc) {
    Atom first = cons(make_int(24), NIL);
    Atom second = cons(make_int(42), NIL);

    Atom args = cons(first, second);

    Atom atom_cons;
    int result = builtin_cons(args, &atom_cons);

    CuAssertTrue(tc, ERROR_OK == result);
}

void test_builtin_cons_returns_cons_of_first_and_second_atoms(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_cons;
    builtin_cons(args, &atom_cons);

    CuAssertTrue(tc, 24 == car(atom_cons).value.integer);
    CuAssertTrue(tc, 42 == cdr(atom_cons).value.integer);
}

void test_builtin_add_returns_ERROR_ARGS_if_only_one_arg(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, NIL);

    Atom atom_add;
    int result = builtin_add(args, &atom_add);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, atom_add.type == AtomType_Error);
}

void test_builtin_add_returns_ERROR_ARGS_if_arg_is_NIL(CuTest* tc) {
    Atom atom_add;
    int result = builtin_add(NIL, &atom_add);
    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, atom_add.type == AtomType_Error);
}

void test_builtin_add_returns_sum_of_all_integers(CuTest* tc) {
    Atom first = make_int(1);
    Atom second = make_int(40);
    Atom third = make_int(1);

    Atom args = cons(first, cons(second, cons(third, NIL)));
    Atom atom_add;
    int result = builtin_add(args, &atom_add);

    CuAssertTrue(tc, 42 == atom_add.value.integer);
}

void test_builtin_add_returns_ERROR_OK_if_two_args(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_cons;
    int result = builtin_add(args, &atom_cons);

    CuAssertTrue(tc, ERROR_OK == result);
}

void test_builtin_add_returns_ERROR_TYPE_if_first_arg_is_not_integer(CuTest* tc) {
    Atom second = make_int(42);
    Atom args = cons(NIL, cons(second, NIL));

    Atom atom_add;
    int result = builtin_add(args, &atom_add);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, atom_add.type == AtomType_Error);
}

void test_builtin_add_returns_ERROR_TYPE_if_second_arg_is_not_integer(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, cons(NIL, NIL));

    Atom atom_add;
    int result = builtin_add(args, &atom_add);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, atom_add.type == AtomType_Error);
}

void test_builtin_add_returns_sum_of_two_integers(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_cons;
    builtin_add(args, &atom_cons);

    CuAssertTrue(tc, 66 == atom_cons.value.integer);
}

void test_builtin_substract_negates_the_argument_if_only_one_arg(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, NIL);

    Atom atom_sub;
    int result = builtin_substract(args, &atom_sub);

    CuAssertTrue(tc, ERROR_OK == result);
    CuAssertTrue(tc, -24 == atom_sub.value.integer);
}

void test_builtin_substract_returns_ERROR_ARGS_if_arg_is_NIL(CuTest* tc) {
    Atom atom_sub;
    int result = builtin_substract(NIL, &atom_sub);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_sub.type);
}

void test_builtin_substract_substract_the_arguments_from_the_first_arg(CuTest* tc) {
    Atom first = make_int(42);
    Atom second = make_int(1);
    Atom third = make_int(1);

    Atom args = cons(first, cons(second, cons(third, NIL)));

    Atom atom_sub;
    int result = builtin_substract(args, &atom_sub);

    CuAssertTrue(tc, ERROR_OK == result);
    CuAssertTrue(tc, 40 == atom_sub.value.integer);
}

void test_builtin_substract_returns_ERROR_OK_if_two_args(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_cons;
    int result = builtin_substract(args, &atom_cons);

    CuAssertTrue(tc, ERROR_OK == result);
}

void test_builtin_substract_returns_ERROR_TYPE_if_first_arg_is_not_integer(CuTest* tc) {
    Atom second = make_int(42);
    Atom args = cons(NIL, cons(second, NIL));

    Atom atom_sub;
    int result = builtin_substract(args, &atom_sub);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, AtomType_Error == atom_sub.type);
}

void test_builtin_substract_returns_ERROR_TYPE_if_second_arg_is_not_integer(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, cons(NIL, NIL));

    Atom atom_sub;
    int result = builtin_substract(args, &atom_sub);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, AtomType_Error == atom_sub.type);
}

void test_builtin_substract_returns_difference_of_two_integers(CuTest* tc) {
    Atom first = make_int(42);
    Atom second = make_int(24);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_cons;
    builtin_substract(args, &atom_cons);

    CuAssertTrue(tc, 18 == atom_cons.value.integer);
}

void test_builtin_multiply_returns_ERROR_ARGS_if_only_one_arg(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, NIL);

    Atom atom_mul;
    int result = builtin_multiply(args, &atom_mul);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_mul.type);
}

void test_builtin_multiply_returns_ERROR_ARGS_if_arg_is_NIL(CuTest* tc) {
    Atom atom_mul;
    int result = builtin_multiply(NIL, &atom_mul);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_mul.type);
}

void test_builtin_multiply_returns_ERROR_ARGS_if_more_than_two_args(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);
    Atom third = make_int(9001);

    Atom args = cons(first, cons(second,third));

    Atom atom_mul;
    int result = builtin_multiply(args, &atom_mul);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_mul.type);
}

void test_builtin_multiply_returns_ERROR_OK_if_two_args(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_cons;
    int result = builtin_multiply(args, &atom_cons);

    CuAssertTrue(tc, ERROR_OK == result);
}

void test_builtin_multiply_returns_ERROR_TYPE_if_first_arg_is_not_integer(CuTest* tc) {
    Atom second = make_int(42);
    Atom args = cons(NIL, cons(second, NIL));

    Atom atom_mul;
    int result = builtin_multiply(args, &atom_mul);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, AtomType_Error == atom_mul.type);
}

void test_builtin_multiply_returns_ERROR_TYPE_if_second_arg_is_not_integer(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, cons(NIL, NIL));

    Atom atom_mul;
    int result = builtin_multiply(args, &atom_mul);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, AtomType_Error == atom_mul.type);
}

void test_builtin_multiply_returns_multiplication_of_two_integers(CuTest* tc) {
    Atom first = make_int(3);
    Atom second = make_int(2);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_mul;
    builtin_multiply(args, &atom_mul);

    CuAssertTrue(tc, 6 == atom_mul.value.integer);
}

void test_builtin_divide_returns_ERROR_ARGS_if_only_one_arg(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, NIL);

    Atom atom_div;
    int result = builtin_multiply(args, &atom_div);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_div.type);
}

void test_builtin_divide_returns_ERROR_ARGS_if_arg_is_NIL(CuTest* tc) {
    Atom atom_div;

    int result = builtin_divide(NIL, &atom_div);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_div.type);

}

void test_builtin_divide_returns_ERROR_ARGS_if_more_than_two_args(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);
    Atom third = make_int(9001);

    Atom args = cons(first, cons(second,third));

    Atom atom_div;
    int result = builtin_divide(args, &atom_div);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_div.type);
}

void test_builtin_divide_returns_ERROR_OK_if_two_args(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_cons;
    int result = builtin_divide(args, &atom_cons);

    CuAssertTrue(tc, ERROR_OK == result);
}

void test_builtin_divide_returns_ERROR_TYPE_if_first_arg_is_not_integer(CuTest* tc) {
    Atom second = make_int(42);
    Atom args = cons(NIL, cons(second, NIL));

    Atom atom_div;
    int result = builtin_divide(args, &atom_div);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, AtomType_Error == atom_div.type);
}

void test_builtin_divide_returns_ERROR_TYPE_if_second_arg_is_not_integer(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, cons(NIL, NIL));

    Atom atom_div;
    int result = builtin_divide(args, &atom_div);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, AtomType_Error == atom_div.type);
}

void test_builtin_divide_returns_division_of_two_integers(CuTest* tc) {
    Atom first = make_int(4);
    Atom second = make_int(2);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_div;
    builtin_divide(args, &atom_div);

    CuAssertTrue(tc, 2 == atom_div.value.integer);
}

void test_builtin_divide_returns_ERROR_ARGS_if_second_arg_is_zero(CuTest* tc) {
    Atom first = make_int(4);
    Atom second = make_int(0);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_div;
    int result = builtin_divide(args, &atom_div);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_div.type);
}

void test_builtin_numeq_returns_ERROR_ARGS_if_only_one_arg(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, NIL);

    Atom atom_numeq;
    int result = builtin_numeq(args, &atom_numeq);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_numeq.type);
}

void test_builtin_numeq_returns_ERROR_ARGS_if_arg_is_NIL(CuTest* tc) {
    Atom atom_numeq;

    int result = builtin_numeq(NIL, &atom_numeq);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_numeq.type);
}

void test_builtin_numeq_returns_ERROR_ARGS_if_more_than_two_args(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);
    Atom third = make_int(9001);

    Atom args = cons(first, cons(second,third));

    Atom atom_numeq;
    int result = builtin_numeq(args, &atom_numeq);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_numeq.type);
}

void test_builtin_numeq_returns_ERROR_OK_if_two_args(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_numeq;
    int result = builtin_numeq(args, &atom_numeq);

    CuAssertTrue(tc, ERROR_OK == result);
}

void test_builtin_numeq_returns_ERROR_TYPE_if_first_arg_is_not_integer(CuTest* tc) {
    Atom second = make_int(42);
    Atom args = cons(NIL, cons(second, NIL));

    Atom atom_numeq;
    int result = builtin_numeq(args, &atom_numeq);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, AtomType_Error == atom_numeq.type);
}

void test_builtin_numeq_returns_ERROR_TYPE_if_second_arg_is_not_integer(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, cons(NIL, NIL));

    Atom atom_numeq;
    int result = builtin_numeq(args, &atom_numeq);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, AtomType_Error == atom_numeq.type);
}

void test_builtin_numeq_returns_true_if_two_integers_are_equals(CuTest* tc) {
    Atom first = make_int(42);
    Atom second = make_int(42);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_numeq;
    builtin_numeq(args, &atom_numeq);

    CuAssertTrue(tc, is_same_string("T", atom_numeq.value.symbol));
}

void test_builtin_numeq_returns_NIL_if_two_integers_are_not_equals(CuTest* tc) {
    Atom first = make_int(42);
    Atom second = make_int(22);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_numeq;
    builtin_numeq(args, &atom_numeq);

    CuAssertTrue(tc, is_nil(atom_numeq));
}

void test_builtin_less_returns_ERROR_ARGS_if_only_one_arg(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, NIL);

    Atom atom_less;
    int result = builtin_less(args, &atom_less);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_less.type);
}

void test_builtin_less_returns_ERROR_ARGS_if_arg_is_NIL(CuTest* tc) {
    Atom atom_less;

    int result = builtin_less(NIL, &atom_less);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_less.type);
}

void test_builtin_less_returns_ERROR_ARGS_if_more_than_two_args(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);
    Atom third = make_int(9001);

    Atom args = cons(first, cons(second,third));

    Atom atom_less;
    int result = builtin_less(args, &atom_less);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, AtomType_Error == atom_less.type);
}

void test_builtin_less_returns_ERROR_OK_if_two_args(CuTest* tc) {
    Atom first = make_int(24);
    Atom second = make_int(42);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_cons;
    int result = builtin_less(args, &atom_cons);

    CuAssertTrue(tc, ERROR_OK == result);
}

void test_builtin_less_returns_ERROR_TYPE_if_first_arg_is_not_integer(CuTest* tc) {
    Atom second = make_int(42);
    Atom args = cons(NIL, cons(second, NIL));

    Atom atom_less;
    int result = builtin_less(args, &atom_less);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, AtomType_Error == atom_less.type);
}

void test_builtin_less_returns_ERROR_TYPE_if_second_arg_is_not_integer(CuTest* tc) {
    Atom first = make_int(24);
    Atom args = cons(first, cons(NIL, NIL));

    Atom atom_less;
    int result = builtin_less(args, &atom_less);

    CuAssertTrue(tc, ERROR_TYPE == result);
    CuAssertTrue(tc, AtomType_Error == atom_less.type);
}

void test_builtin_less_returns_true_if_first_arg_is_smaller_than_second(CuTest* tc) {
    Atom first = make_int(2);
    Atom second = make_int(42);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_less;
    builtin_less(args, &atom_less);

    CuAssertTrue(tc, is_same_string("T", atom_less.value.symbol));
}

void test_builtin_less_returns_NIL_if_first_arg_is_not_smaller_than_second(CuTest* tc) {
    Atom first = make_int(42);
    Atom second = make_int(42);

    Atom args = cons(first, cons(second, NIL));

    Atom atom_cons;
    builtin_less(args, &atom_cons);

    CuAssertTrue(tc, is_nil(atom_cons));
}

void test_builtin_eq_returns_ERROR_ARGS_if_no_arguments_are_provided(CuTest* tc) {
    Atom atom_result;

    int result = builtin_eq(NIL, &atom_result);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, atom_result.type == AtomType_Error);
}

void test_builtin_eq_returns_ERROR_ARGS_if_only_one_argument_is_provided(CuTest* tc) {
    Atom first = make_int(42);
    Atom atom_result;

    int result = builtin_eq(cons(first, NIL), &atom_result);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, atom_result.type == AtomType_Error);
}

void test_builtin_eq_returns_ERROR_ARGS_if_more_than_two_arguements_are_provided(CuTest* tc) {
    Atom first = make_int(42);
    Atom second = make_int(42);
    Atom third = make_int(9001);
    Atom atom_result;

    int result = builtin_eq(cons(first, cons(second, third)), &atom_result);

    CuAssertTrue(tc, ERROR_ARGS == result);
    CuAssertTrue(tc, atom_result.type == AtomType_Error);
}

CuSuite* BuiltinGetSuite(void) {
    CuSuite* suite = CuSuiteNew();

    /* builtin_car */
    SUITE_ADD_TEST(suite, test_builtin_car_returns_ERROR_OK_if_args_is_a_cons_of_cons_NIL);
    SUITE_ADD_TEST(suite, test_builtin_car_returns_ERROR_ARGS_if_args_is_not_a_cons_of_cons);
    SUITE_ADD_TEST(suite, test_builtin_car_returns_the_first_element_of_a_pair);
    SUITE_ADD_TEST(suite, test_builtin_car_returns_NIL_if_first_element_of_pair_is_nil);
    SUITE_ADD_TEST(suite, test_builtin_car_returns_NIL_if_first_element_of_pair_is_nil);

    /* builin_cdr */
    SUITE_ADD_TEST(suite, test_builtin_cdr_returns_ERROR_OK_if_args_is_a_cons_of_cons_NIL);
    SUITE_ADD_TEST(suite, test_builtin_cdr_returns_ERROR_ARGS_if_args_is_not_a_cons_of_cons);
    SUITE_ADD_TEST(suite, test_builtin_cdr_returns_the_first_element_of_a_pair);
    SUITE_ADD_TEST(suite, test_builtin_cdr_returns_NIL_if_first_element_of_pair_is_nil);

    /* builtin_cons */
    SUITE_ADD_TEST(suite, test_builtin_cons_returns_ERROR_ARGS_if_args_is_NIL);
    SUITE_ADD_TEST(suite, test_builtin_cons_returns_ERROR_OK);
    SUITE_ADD_TEST(suite, test_builtin_cons_returns_cons_of_first_and_second_atoms);

    /* builtin_add */
    //SUITE_ADD_TEST(suite, test_builtin_add_returns_ERROR_ARGS_if_only_one_arg);
    SUITE_ADD_TEST(suite, test_builtin_add_returns_ERROR_ARGS_if_arg_is_NIL);
    SUITE_ADD_TEST(suite, test_builtin_add_returns_sum_of_all_integers);
    SUITE_ADD_TEST(suite, test_builtin_add_returns_ERROR_OK_if_two_args);
    SUITE_ADD_TEST(suite, test_builtin_add_returns_ERROR_TYPE_if_first_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_add_returns_ERROR_TYPE_if_second_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_add_returns_sum_of_two_integers);

    /* builtin_substract */
    SUITE_ADD_TEST(suite, test_builtin_substract_negates_the_argument_if_only_one_arg);
    SUITE_ADD_TEST(suite, test_builtin_substract_returns_ERROR_ARGS_if_arg_is_NIL);
    SUITE_ADD_TEST(suite, test_builtin_substract_substract_the_arguments_from_the_first_arg);
    SUITE_ADD_TEST(suite, test_builtin_substract_returns_ERROR_OK_if_two_args);
    SUITE_ADD_TEST(suite, test_builtin_substract_returns_ERROR_TYPE_if_first_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_substract_returns_ERROR_TYPE_if_second_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_substract_returns_difference_of_two_integers);

    /* builtin_multiply */
    SUITE_ADD_TEST(suite, test_builtin_multiply_returns_ERROR_ARGS_if_only_one_arg);
    SUITE_ADD_TEST(suite, test_builtin_multiply_returns_ERROR_ARGS_if_arg_is_NIL);
    SUITE_ADD_TEST(suite, test_builtin_multiply_returns_ERROR_ARGS_if_more_than_two_args);
    SUITE_ADD_TEST(suite, test_builtin_multiply_returns_ERROR_OK_if_two_args);
    SUITE_ADD_TEST(suite, test_builtin_multiply_returns_ERROR_TYPE_if_first_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_multiply_returns_ERROR_TYPE_if_second_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_multiply_returns_multiplication_of_two_integers);

    /* builtin_divide */
    SUITE_ADD_TEST(suite, test_builtin_divide_returns_ERROR_ARGS_if_only_one_arg);
    SUITE_ADD_TEST(suite, test_builtin_divide_returns_ERROR_ARGS_if_arg_is_NIL);
    SUITE_ADD_TEST(suite, test_builtin_divide_returns_ERROR_ARGS_if_more_than_two_args);
    SUITE_ADD_TEST(suite, test_builtin_divide_returns_ERROR_OK_if_two_args);
    SUITE_ADD_TEST(suite, test_builtin_divide_returns_ERROR_TYPE_if_first_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_divide_returns_ERROR_TYPE_if_second_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_divide_returns_division_of_two_integers);
    SUITE_ADD_TEST(suite, test_builtin_divide_returns_ERROR_ARGS_if_second_arg_is_zero);

    /* builtin_numeq */
    SUITE_ADD_TEST(suite, test_builtin_numeq_returns_ERROR_ARGS_if_only_one_arg);
    SUITE_ADD_TEST(suite, test_builtin_numeq_returns_ERROR_ARGS_if_arg_is_NIL);
    SUITE_ADD_TEST(suite, test_builtin_numeq_returns_ERROR_ARGS_if_more_than_two_args);
    SUITE_ADD_TEST(suite, test_builtin_numeq_returns_ERROR_OK_if_two_args);
    SUITE_ADD_TEST(suite, test_builtin_numeq_returns_ERROR_TYPE_if_first_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_numeq_returns_ERROR_TYPE_if_second_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_numeq_returns_true_if_two_integers_are_equals);
    SUITE_ADD_TEST(suite, test_builtin_numeq_returns_NIL_if_two_integers_are_not_equals);

    /* builtin_less */
    SUITE_ADD_TEST(suite, test_builtin_less_returns_ERROR_ARGS_if_only_one_arg);
    SUITE_ADD_TEST(suite, test_builtin_less_returns_ERROR_ARGS_if_arg_is_NIL);
    SUITE_ADD_TEST(suite, test_builtin_less_returns_ERROR_ARGS_if_more_than_two_args);
    SUITE_ADD_TEST(suite, test_builtin_less_returns_ERROR_OK_if_two_args);
    SUITE_ADD_TEST(suite, test_builtin_less_returns_ERROR_TYPE_if_first_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_less_returns_ERROR_TYPE_if_second_arg_is_not_integer);
    SUITE_ADD_TEST(suite, test_builtin_less_returns_true_if_first_arg_is_smaller_than_second);
    SUITE_ADD_TEST(suite, test_builtin_less_returns_NIL_if_first_arg_is_not_smaller_than_second);

    /* builtin_eq */
    SUITE_ADD_TEST(suite, test_builtin_eq_returns_ERROR_ARGS_if_no_arguments_are_provided);
    SUITE_ADD_TEST(suite, test_builtin_eq_returns_ERROR_ARGS_if_only_one_argument_is_provided);
    SUITE_ADD_TEST(suite, test_builtin_eq_returns_ERROR_ARGS_if_more_than_two_arguements_are_provided);
}
