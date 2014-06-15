# __alisp__
__alisp__ is an implementation of Lisp in C. The implementation contains a mark-and-sweep garbage collector and function continuation.

## Compile
Execute the following commands in order to compile and run the unit tests.
    $ make check gen-cmake
    $ make check

## To do
* Add missing unit tests for expression.c
* Add integration tests for gc.c
* Add missing unit tests for parser.c
* Add missing unit tests for expression.c
* Add missing unit tests for builtin_apply
* Add missing unit tests for apply
* Add missing unit tests for builtin_eq
* Add missing unit tests for make_string
* Add missing unit tests for is_string

* Assert type in add_symbol_to_table + test
* Assert atom is pair in car/cdr + test
* Add useful error messages through macros
* Minus with one argument should negate the number
* Use arithmetic operators with variatic arguments
* Add strings manipulation
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
* Add language features documentation

Further Reading
---------------
Here are some references that helped me implement __alisp__ 
1. http://groups.csail.mit.edu/mac/classes/6.001/abelson-sussman-lectures/
2. http://www.buildyourownlisp.com/contents
3. http://www.lwh.jp/lisp/
4. http://jozefg.bitbucket.org/posts/2014-05-05-i-used-c-correctly.html
5. http://journal.stuffwithstuff.com/2013/12/08/babys-first-garbage-collector/
6. https://github.com/rui314/minilisp
