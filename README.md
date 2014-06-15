# __alisp__
__alisp__ is an implementation of Lisp in C. The implementation contains a mark-and-sweep garbage collector and function continuation.

## Compile
Execute the following commands in order to compile and run the unit tests.
    $ make check gen-cmake
    $ make check

## Run
	$ bind/alisp

## Language features
__alisp is a Lisp interpreter. It reads on expression at a time from the standard input, evaluates it, and prints the return value of the expression.
Here is an exmaple of a valid input:

	> (+ 1 2)

The above expression prints "3".

### Literals
__alisp__ supports integer literals, `()`, `t`, `nil`, symbols, strings, and lists.

* Integer literals are positive or negative integers.
* `()`, or `nil`, are the only false values. They also represent the empty list.
* `t` is a predefined variable evaluated to itself. It is the preferred way to represent a true value. 
* Symbols are objects with unique name. They are used to represent identifiers.
* String literals are surrounded by `""`.
* List literals are cons cells. They are either regular lists whose last element's cdr is `()` or a dotted list ending with any non-`() value. Dotted lists are written as `(a . )`.

### List operators
`cons` takes two arguments and creates a new cons cell where the first argument is the car and the second the cdr.
	> cons(1 2)

`car` and `cdr` are accessors for con cells.
 
### Numeric operators
`+` returns the sum of two arguments
	> (+ 1 2)
`-` returns the difference of two arguments or negates one argument
	>(- 2 1)
	> (- 2)
`*` returns the product of two arguments
	> (* 3 4)
`/` returns the quotient of two arguments
	> (/ 4 2)
`=` returns `t` if the two arguments are the same integer and `()` if they are different
	> (= 2 1)
`<` returns `t` if the first argument is smaller than the second
	> (< 2 1)

### Conditionals
`(if cond then else)`. It evaluates *cond*, and if it is true, *then* is evaluated. Otherwise, *else* is evaluated.
	> (if (= 2 3) 4 5)

### Equivalence test operators
`eq` takes two arguments and returns `t` if the objects are the same.
	> (eq NIL 2)

### Definitions
User defined variables and functions are supported by __alisp__. Functions can defined using `lamda`.
`(lambda (args ...) expr ...)` returns a function object which can be assign to a variable using `define`.
	> (define square (lambda (x) (* x x)))

The `lambda` can also be omitted in the following way:
	> (define (square x) (* x x))

The two expressions will be evaluated to the same result.

### Continuations
__alisp__ uses continuations and tail recursions to be able to use recursion as without being limited by the stack of the interpreter. To do so, evaluated arguments and pending evaluations are kept in a stack that is manipulated directly instead of using the machine code stack.

Tail calls do not require a new stack frame to the call stack, so they can recurse as many levels as necessary without increasing the stack depth.

### Quote
__alisp__ supports quotes, or `'`, which can be used to manipulate expressions without evaluating them.
	> '((lambda (x) (- x 2)) 7) 

### Garbage collection
A mark-and-sweep algorithm is used to collect memory. Since all the data is allocated through the `cons`, it is possible to keep track of every allocation by tracking the `cons` atoms in a linked list.

Every 100'000 evaluations, the garbage collector goes through the tree of pairs, and mark them as "in used". It then traverses the linked list of allocations and free any atom that is not marked. The marks are then cleared, and the application can continue running.

## Standard library
__alis__ as a small library with some useful functions.

### Logical Functions
#### not
	> (not t)

#### or
	> (or () t)

#### and
	> (and t ())

### List functions
#### list
	> (list 1 2 3)

#### len
	> (len list 1 2 3)

#### first
	> (first (list 2 3 1))

#### last
	> (last (list 2 3 1))

#### init
	> (init (list 2 3 1))

#### tail
	> (tail (list 1 2 3))

#### reverse
	> (reverse (list 1 2 3))

#### Fold Left
	> (foldl + 2 (list 10 20 30))

#### foldr
	> (foldr + 2 (list 10 20 30))

#### map
	> (map + '(1 2 3) '(4 5 6))

### Numeric Functions
#### abs
	> (abs -2)

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
* Use arithmetic operators with variatic arguments
* Add strings manipulation
* Add double
* Be able to manipulate double and long
* Add booleans?
* Implement better gc algorithm
* Add more functions to std
* Add load from file builtin import files
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
* Quasiquotes

## Further Reading
Here are some references that helped me implement __alisp__ 
1. http://groups.csail.mit.edu/mac/classes/6.001/abelson-sussman-lectures/
2. http://www.buildyourownlisp.com/contents
3. http://www.lwh.jp/lisp/
4. http://jozefg.bitbucket.org/posts/2014-05-05-i-used-c-correctly.html
5. http://journal.stuffwithstuff.com/2013/12/08/babys-first-garbage-collector/
6. https://github.com/rui314/minilisp
