;;; $Id$ -*- scwm -*-
;;; simple1
(begin
  (define solver (make-cl-solver))
  (define x (make-cl-variable "x" 167))
  (define y (make-cl-variable "y" 2))
  (define eq (make-cl-equality (make-cl-expression x) y))
  (cl-add-constraint solver eq)
  (for-each (lambda (x) (display x))
	    (list "x = " (cl-value x) "\n" "y = " (cl-value y) "\n"))
  (= (cl-value x) (cl-value y)))

;;; justStay1
(begin
  (define solver (make-cl-solver))
  (define x (make-cl-variable "x" 5))
  (define y (make-cl-variable "y" 10))
  (cl-add-stay solver x y)
  (for-each (lambda (x) (display x))
	    (list "x = " (cl-value x) "\n" "y = " (cl-value y) "\n")))

;;; addDelete1
(begin
  (define solver (make-cl-solver))
  (define x (make-cl-variable "x"))
  (cl-add-constraint solver (make-cl-equality x (make-cl-expression 100) cls-weak))
  (define c10 (make-cl-inequality x <= 10))
  (define c20 (make-cl-inequality x <= 20))
  (cl-add-constraint solver c10)
  (cl-add-constraint solver c20)
  (for-each (lambda (x) (display x)) (list "x == " (cl-value x) "\n"))  
  ;; want 10

  (cl-remove-constraint solver c10)
  (for-each (lambda (x) (display x)) (list "x == " (cl-value x) "\n"))  
  ;; want 20

  (cl-remove-constraint solver c20)
  (for-each (lambda (x) (display x)) (list "x == " (cl-value x) "\n"))
  ;; want 100

  (define c10again (make-cl-inequality x <= 10))
  (cl-add-constraint solver c10 c10again)
  ;; (cl-solver-debug-print solver)
  (for-each (lambda (x) (display x)) (list "x == " (cl-value x) "\n"))
  ;; want 10

  (cl-remove-constraint solver c10)
  (for-each (lambda (x) (display x)) (list "x == " (cl-value x) "\n"))
  ;; want 10

  (cl-remove-constraint solver c10again)
  (for-each (lambda (x) (display x)) (list "x == " (cl-value x) "\n"))
  ;; want 100
  )
  

;; addDelSimple
(begin 
  (define solver (make-cl-solver))
  (define x (make-cl-variable "x" 5))
  (define y (make-cl-variable "y"))
  (cl-add-stay solver x y)
  (define eq (make-cl-equality x (cl-times y 2)))
  (cl-add-constraint solver eq)
  (for-each (lambda (x) (display x)) (list "x == " (cl-value x) "\n" "y == " (cl-value y) "\n"))
  (cl-add-editvar solver x)
  (cl-begin-edit solver)
  (cl-suggest-value solver x 9)
  (cl-resolve solver)
  (for-each (lambda (x) (display x)) (list "x == " (cl-value x) "\n" "y == " (cl-value y) "\n"))
  (cl-end-edit solver)
  )

;;; Local Variables:
;;; eval: (load "scwm")
;;; eval: (scwm-mode)
;;; End:
