(define (NOT x)
  (if x 
    () 
    t))

(define (AND x y) 
  (if x 
    (if y 
      t 
      ())
    ()))

(define (OR x y) 
  (if x 
    t
    (if y 
      t 
      ())))

(define (LIST . items)
  (foldr cons nil items))

(define (LEN list) 
  (if = list nil
    nil
    (+ 1 (len cdr(list)))))

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