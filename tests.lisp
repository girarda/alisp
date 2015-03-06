;; Test cons
(cons 1 2)
;=>(1 . 2)

;; Test car
(define foo (quote (a b c)))
(car foo)
;=>a

;; Test cdr
(define foo (quote (a b c)))
(cdr foo)
;=>(b c)

;; Test addition
(+ 1 2)
;=>3

;; Test variadic addition
(+ 1 2 3)
;=>6

;; Test Negation
(- 2)
;=>-2

;; Test double negation
(- -2)
;=>2

;; Test negation
(- 5 2)
;=>3

;; Test variadic negation
(- 5 2 7)
;=>-4

;; Test multiplication
(* 3 4)
;=>12

;; Test division
(* 3 4)
;=>12

;; Test equals different
(= 2 1)
;=>NIL

;; Test equals same
(= 1 1)
;=>T

;; Test eq different
(eq 2 1)
;=>NIL

;; Test eq same
(= 1 1)
;=>T

;; Test if true
(if (= 2 2) 4 5)
;=>4

;; Test if true
(if (= 2 1) 4 5)
;=>5

;; Test < true
(< 1 2)
;=>T

;; Test < false
(< 4 2)
;=>NIL

;; Test define
(define foo 42)
foo
;=>42

;; Test quote
(quote foo)
;=>FOO

;; Test quote syntactic sugar
'foo
;=>FOO

;; Test define quote
(define foo (quote bar))
foo
;=>bar

;; Test quote quote
''()
;=>(QUOTE NIL)

;; Test define lambda
(define square (lambda (x) (* x x)))
(square 2)
;=>4

;; Test define no lambda
(define (square x) (* x x))
(square 2)
;=>4

;; Test quote
'((lambda (x) (- x 2)) 7)
;=>((lambda (x) (- x 2)) 7)

;; Test closure
(define make-adder (lambda (x) (lambda (y) (+ x y))))
(define add-two (make-adder 2))
(add-two 5)
;=>7

;; Test variadic lambda
((lambda (a . b) a) 1 2 3)
;=>1

;; Test variadic closure
(define (sum-list xs) (if xs (+ (car xs) (sum-list (cdr xs))) 0))
(define (add . xs) (sum-list xs))
(add 1 2 3)
;=>6

;; Test defmacro
(defmacro (ignore x) (cons 'quote (cons x nil)))
(ignore foo)
;=>foo

;; Test tail recursion
(define (count n a) (if (= n 0) a(count (- n 1) (+ a 1))))
(count 100000 0)
;=>100000 

;; Test pair false
(pair? 2)
;=>nil

;; Test pair true
(pair? '(1 2 3))
;=>t