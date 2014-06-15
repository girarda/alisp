(define (NOT x)
  (if x 
    () 
    t
    ))

(define (AND x y) 
  (if x 
    (if y 
      t 
      ())
    ()
    ))

(define (OR x y) 
  (if x 
    t
    (if y 
      t 
      ()
      )
    ))

(define (LIST . items)
  (foldr cons nil items))

(define (LEN list) 
  (if = list nil
    nil
    (+ 1 (LEN cdr(list)))
    ))

(define (FIRST list)
  (car list))

(define (LAST list)
  (if (cdr list)
    (last (cdr list))
    list
  ))

(define (INIT list)
  (if (cdr list)
    (cons (car list) (init(cdr list)))
    NIL
  ))

(define (TAIL list)
  (cdr list))

(define (reverse list)
  (foldl (lambda (a x) (cons x a)) nil list))

(define (foldl proc init list)
  (if list
      (foldl proc
             (proc init (car list))
             (cdr list))
      init))

(define (foldr proc init list)
  (if list
      (proc (car list)
            (foldr proc init (cdr list)))
      init))

(define (unary-map proc list)
  (foldr (lambda (x rest) (cons (proc x) rest))
         nil
         list))

(define (map proc . arg-lists)
  (if (car arg-lists)
      (cons (apply proc (unary-map car arg-lists))
            (apply map (cons proc
                             (unary-map cdr arg-lists))))
      nil))

(define (filter proc  list)
  (if (car list)
    (cons 
      (if (proc (first (list)) (head list) NIL) (filter proc (tail list))))
    nil))

(define (abs x) (if (< x 0) (- x) x))

(define (count n) (if (= n 0) t (count (- n 1))))