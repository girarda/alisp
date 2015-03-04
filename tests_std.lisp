;; Test list
(list 1 2 3)
;=>(1 2 3)

;; Test len list
(len (list 1 2 3))
;=>3

;; Test first list
(first (list 1 2 3))
;=>1

;; Test first list
(last (list 1 2 3))
;=>3

;; Test init list
(init (list 1 2 3))
;=>(1 2)

;; Test tail list
(tail (list 1 2 3))
;=>(2 3)

;; Test reverse list
(reverse (list 1 2 3))
;=>(3 2 1)

;; Test fold left list
(foldl + 2 (list 1 2 3))
;=>8

;; Test fold left list
(foldr + 2 (list 1 2 3))
;=>8

;; Test map
(map + '(1 2 3) '(4 5 6))
;=>(5 7 9)

;; Test abs
(abs -2)
;=>2