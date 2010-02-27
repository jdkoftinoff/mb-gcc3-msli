;; Scheduling description for the Xilinx PowerPC 405/440 APU Floating Point Unit.
;; Copyright (C) 2003 Free Software Foundation, Inc.
;;
;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 2, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING.  If not, write to the
;; Free Software Foundation, 59 Temple Place - Suite 330, Boston,
;; MA 02111-1307, USA.


;;----------------------------------------------------
;; Xilinx APU FPU Pipeline Description
;;
;;  - attr 'type' and 'fp_type' should definitely
;;    be cleaned up at some point in the future.
;;    ddiv,sdiv,dmul,smul etc are quite confusing.
;;    Should use consistent fp* attrs. 'fp_type'
;;    should also go away, leaving us only with 'fp'
;;
;;----------------------------------------------------
(define_automaton "Xfpu")
(define_cpu_unit "Xfpu_issue,Xfpu_addsub,Xfpu_mul,Xfpu_div" "Xfpu")

;; Latencies for PowerPC@200 MHz
(define_insn_reservation "fp-default" 2
  (and (and 
        (eq_attr "type" "fp")
        (eq_attr "fp_type" "fp_default"))
       (eq_attr "cpu" "ppc405"))
  "Xfpu_issue*3")

(define_insn_reservation "fp-compare" 22
  (and (eq_attr "type" "fpcompare")                     ;; Inconsistent naming
       (eq_attr "cpu" "ppc405"))
  "Xfpu_issue*3,Xfpu_addsub,nothing*18")

(define_insn_reservation "fp-addsub-s" 19
  (and (and
        (eq_attr "type" "fp")
        (eq_attr "fp_type" "fp_addsub_s"))
       (eq_attr "cpu" "ppc405"))
  "Xfpu_issue*3,Xfpu_addsub,nothing*15")

;; (define_insn_reservation "fp-addsub-d" 18
;;   (and (and
;;         (eq_attr "type" "fp")
;;         (eq_attr "fp_type" "fp_addsub_d"))
;;        (eq_attr "cpu" "ppc405"))
;;   "Xfpu_issue*3,Xfpu_addsub,nothing*16")

(define_insn_reservation "fp-mul-s" 14
  (and (and
        (eq_attr "type" "fp")
        (eq_attr "fp_type" "fp_mul_s"))
       (eq_attr "cpu" "ppc405"))
  "Xfpu_issue*3,Xfpu_mul,nothing*10")

;; (define_insn_reservation "fp-mul-d" 16
;;   (and (and
;;         (eq_attr "type" "fp")
;;         (eq_attr "fp_type" "fp_mul_d"))
;;        (eq_attr "cpu" "ppc405"))
;;   "Xfpu_issue*3,Xfpu_mul,nothing*14")

(define_insn_reservation "fp-div-s" 35
   (and (eq_attr "type" "sdiv")                          ;; Inconsistent attr naming
        (eq_attr "cpu" "ppc405"))
   "Xfpu_issue*3,Xfpu_div,nothing*34")

;; (define_insn_reservation "fp-div-d" 57
;;   (and (eq_attr "type" "ddiv")
;;        (eq_attr "cpu" "ppc405"))                        ;; Inconsistent attr naming
;;   "Xfpu_issue*3,Xfpu_div,nothing*55")

(define_insn_reservation "fp-maddsub-s" 26
  (and (and
        (eq_attr "type" "fp")
        (eq_attr "fp_type" "fp_maddsub_s"))
       (eq_attr "cpu" "ppc405"))
  "Xfpu_issue*3,Xfpu_mul,nothing*7,Xfpu_addsub,nothing*14")

;; (define_insn_reservation "fp-maddsub-d" 33
;;   (and (and
;;         (eq_attr "type" "dmul")                         ;; Inconsistent attr naming
;;         (eq_attr "fp_type" "fp_maddsub_d"))
;;        (eq_attr "cpu" "ppc405"))
;;   "Xfpu_issue*3,Xfpu_mul,nothing*14,Xfpu_addsub,nothing*16")

(define_insn_reservation "fp-load" 9                      ;; FIXME. Is double/single precision the same ?
  (and (eq_attr "type" "fpload, fpload_ux, fpload_u")
       (eq_attr "cpu" "ppc405"))
  "Xfpu_issue*9")

(define_insn_reservation "fp-store" 9                      ;; FIXME. What is the latency?
  (and (eq_attr "type" "fpstore, fpstore_ux, fpstore_u")
       (eq_attr "cpu" "ppc405"))
  "Xfpu_issue*9")


;; (define_insn_reservation "fp-sqrt-s" 28
;;   (and (eq_attr "type" "ssqrt")
;;        (eq_attr "cpu" "ppc405"))
;;   "Xfpu_issue*3,Xfpu_sqrt,nothing*25")


;; (define_insn_reservation "fp-sqrt-d" 57
;;   (and (eq_attr "type" "dsqrt")
;;        (eq_attr "cpu" "ppc405"))
;;   "Xfpu_issue*3,Xfpu_sqrt,nothing*54")


(automata_option "v")
(automata_option "time")
(automata_option "progress")
