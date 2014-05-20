# __alisp__
__alisp__ is an implementation of Lisp in C. The implementation contains a mark-and-sweep garbage collector and function continuation.

## Compile
* `make gen-cmake` to produce the necessary cmake
* `make check` to compile and run the unit tests

## To do
* Add missing unit tests for expression.c
* Add missing unit tests for gc.c
* Add missing unit tests for parser.c
* Assert type in add_symbol_to_table + test
* Assert atom is pair in car/cdr + test
* Add useful error messages through macros
* Minus with one argument should negate the number
* Use arithmetic operators with variatic arguments
* Add strings
* Add double
* Be able to manipulate double and long
* Add booleans?
* Find better gc algorithm
* Add standard library
* Document standard library
* Be able to execute from file
* Be able to import files
* User defined types
* quasiquotation
* List literal ? (use [] to give literal notation for lists of evaluated values lists)
* OS interaction
* Pool allocation?
* Lexical scoping?
* Static typing ?
* Use hashtable for environment?
* Exit function for stopping the prompt and exit cleanly

