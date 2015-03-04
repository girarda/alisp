;; Test addition
(+ 1 2)
;=>3

;; Test addition multiple args
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

;; Test negation multiple args
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

