#include "CuTest.h"
#include "gc.h"
#include "error.h"
#include "util.h"

void test_gc_mark_does_marks_an_atom_if_it_has_children(CuTest* tc) {
    Atom a = make_int(42);
    Atom b = make_int(24);
    Atom list = cons(a,b);
    struct Allocation *alloc = get_allocation(list);
    CuAssertTrue(tc, 0 == alloc->mark);

    gc_mark(list);

    CuAssertTrue(tc, -1 == alloc->mark);
}

void test_gc_mark_does_marks_an_atoms_children_if_they_have_children(CuTest* tc) {
    Atom a = cons(NIL, NIL);
    Atom b = cons(NIL, NIL);
    Atom list = cons(a,b);
    struct Allocation *a_alloc = get_allocation(a);
    struct Allocation *b_alloc = get_allocation(b);
    struct Allocation *list_alloc = get_allocation(list);

    gc_mark(list);

    CuAssertTrue(tc, -1 == a_alloc->mark);
    CuAssertTrue(tc, -1 == b_alloc->mark);
    CuAssertTrue(tc, -1 == list_alloc->mark);
}

void test_gc_mark_does_not_mark_an_atom_that_was_not_a_child(CuTest* tc) {
    Atom not_a_child = cons(NIL, NIL);
    Atom list = cons(NIL,NIL);
    struct Allocation *not_a_child_alloc = get_allocation(not_a_child);
    struct Allocation *list_alloc = get_allocation(list);

    gc_mark(list);

    CuAssertTrue(tc, 0 == not_a_child_alloc->mark);
    CuAssertTrue(tc, -1 == list_alloc->mark);
}

void test_gc_clear_marks_clears_the_marks(CuTest* tc) {
    Atom a = cons(NIL, NIL);
    Atom b = cons(NIL, NIL);
    Atom list = cons(a,b);
    struct Allocation *a_alloc = get_allocation(a);
    struct Allocation *b_alloc = get_allocation(b);
    struct Allocation *list_alloc = get_allocation(list);
    gc_mark(list);

    gc_clear_marks();

    CuAssertTrue(tc, 0 == a_alloc->mark);
    CuAssertTrue(tc, 0 == b_alloc->mark);
    CuAssertTrue(tc, 0 == list_alloc->mark);
}

void test_get_allocation_returns_the_allocation_for_the_right_atom(CuTest* tc) {
    Atom not_a_child = cons(NIL, NIL);
    Atom list = cons(NIL,NIL);
    struct Allocation *not_a_child_alloc = get_allocation(not_a_child);
    struct Allocation *list_alloc = get_allocation(list);

    CuAssertTrue(tc, 0 == not_a_child_alloc->mark);
    CuAssertTrue(tc, 0 == list_alloc->mark);

    gc_mark(list);

    CuAssertTrue(tc, 0 == not_a_child_alloc->mark);
    CuAssertTrue(tc, -1 == list_alloc->mark);
}

CuSuite* GcGetSuite(void)
{
    CuSuite* suite = CuSuiteNew();

    /* gc_mark */
    SUITE_ADD_TEST(suite, test_gc_mark_does_marks_an_atom_if_it_has_children);
    SUITE_ADD_TEST(suite, test_gc_mark_does_marks_an_atoms_children_if_they_have_children);
    SUITE_ADD_TEST(suite, test_gc_mark_does_not_mark_an_atom_that_was_not_a_child);

    /* gc_clear_marks */
    //SUITE_ADD_TEST(suite, test_gc_clear_marks_clears_the_marks);

    /* get_allocation */
    SUITE_ADD_TEST(suite, test_get_allocation_returns_the_allocation_for_the_right_atom);

    return suite;
}


