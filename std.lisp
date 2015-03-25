(define (NOT x)
  (if x 
    () 
    t))

(defmacro (and a b) (list 'if a b nil))

(define (OR x y) 
  (if x 
    t
    (if y 
      t 
      ())))

(define (LIST . items)
  (foldr cons nil items))

(define (LEN list) 
  (if (not list)
    0
    (+ 1 (len (cdr list)))))

(define (FIRST list)
  (car list))

(define (_LAST list)
  (if (cdr list)
    (_last (cdr list))
    (car list)))

(define (LAST list)
  (_LAST list))

(define (INIT list)
  (if (cdr list)
    (cons (car list) (init(cdr list)))
    NIL))

(define (TAIL list)
  (cdr list))

(define (REVERSE list)
  (foldl (lambda (a x) (cons x a)) nil list))

(define (FOLDL proc init list)
  (if list
      (foldl proc
             (proc init (car list))
             (cdr list))
      init))

(define (FOLDR proc init list)
  (if list
      (proc (car list)
            (foldr proc init (cdr list)))
      init))

(define (UNARY-MAP proc list)
  (foldr (lambda (x rest) (cons (proc x) rest))
         nil
         list))

(define (MAP proc . arg-lists)
  (if (car arg-lists)
      (cons (apply proc (unary-map car arg-lists))
            (apply map (cons proc
                             (unary-map cdr arg-lists))))
      nil))

(define (ABS x) 
  (if (< x 0) 
    (- x) 
    x))

(define (count n) 
  (if (= n 0) 
    t 
    (count (- n 1))))

(define (append a b) (foldr cons b a))

(define (caar x) (car (car x)))

(define (cadr x) (car (cdr x)))

(defmacro (quasiquote x)
  (if (pair? x)
      (if (eq (car x) 'unquote)
          (cadr x)
          (if (and (pair? (car x)) (eq (caar x) 'unquote-splicing))
              (list 'append
                    (cadr (car x))
                    (list 'quasiquote (cdr x)))
              (list 'cons
                    (list 'quasiquote (car x))
                    (list 'quasiquote (cdr x)))))
      (list 'quote x)))

(defmacro (let defs . body)
  `((lambda ,(map car defs) ,@body)
    ,@(map cadr defs)))
