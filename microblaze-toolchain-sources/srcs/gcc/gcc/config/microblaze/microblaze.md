;; microblaze.md
;; Copyright (C) 1989, 90-98, 1999 Free Software Foundation, Inc.

;; This file is part of GNU CC.

;; GNU CC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.

;; GNU CC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU CC; see the file COPYING.  If not, write to
;; the Free Software Foundation, 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.


;;----------------------------------------------------------------------------------------------------------------------------------------------------
;; 
;; Copyright (c) 2001 - 2005 Xilinx, Inc.  All rights reserved. 
;; 
;; microblaze.md
;; 
;; MicroBlaze specific file. Contains functions for generating MicroBlaze code. 
;; Certain lines of code are from Free Software Foundation
;; 
;; $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/gcc/src-3.4/gcc/config/microblaze/microblaze.md,v 1.25.2.1 2006/01/20 08:22:59 vasanth Exp $
;; 
;;----------------------------------------------------------------------------------------------------------------------------------------------------


;;----------------------------------------------------
;; Constants
;;----------------------------------------------------
(define_constants [
  (R_SP        1)       ;; Stack pointer reg
  (R_SR       15)       ;; Sub-routine return addr reg
  (R_IR       14)       ;; Interrupt return addr reg
  (R_DR       16)       ;; Debug trap return addr reg
  (R_ER       17)       ;; Exception return addr reg
  (R_TMP      18)       ;; Assembler temporary reg
  (MB_PIPE_3   0)       ;; Microblaze 3-stage pipeline 
])


;;----------------------------------------------------
;; Instruction Attributes
;;----------------------------------------------------

;; Classification of each insn.
;; branch	conditional branch
;; jump		unconditional jump
;; call		unconditional call
;; load		load instruction(s)
;; store	store instruction(s)
;; move		data movement within same register set
;; arith	integer arithmetic instruction
;; darith	double precision integer arithmetic instructions
;; imul		integer multiply
;; idiv		integer divide
;; icmp		integer compare
;; Xfadd		floating point add/subtract
;; Xfmul		floating point multiply
;; Xfmadd	floating point multiply-add
;; Xfdiv		floating point divide
;; Xfabs		floating point absolute value
;; Xfneg		floating point negation
;; Xfcmp		floating point compare
;; Xfcvt		floating point convert
;; Xfsqrt	floating point square root
;; multi	multiword sequence (or user asm statements)
;; nop		no operation
;; bshift 	Shift operations

(define_attr "type"
  "unknown,branch,jump,call,load,store,move,arith,darith,imul,idiv,icmp,multi,nop,no_delay_arith,no_delay_load,no_delay_store,no_delay_imul,no_delay_move,bshift,fadd,frsub,fmul,fdiv,fcmp,fsl"
  (const_string "unknown"))

;; Main data type used by the insn
(define_attr "mode" "unknown,none,QI,HI,SI,DI,SF,DF" (const_string "unknown"))

;; # instructions (4 bytes each)
(define_attr "length" "" (const_int 4))

;;----------------------------------------------------
;; Attribute describing the processor.  
;;----------------------------------------------------

;; Describe a user's asm statement.
(define_asm_attributes
  [(set_attr "type" "multi")])

;; whether or not generating calls to position independent functions
(define_attr "abicalls" "no,yes"
  (const (symbol_ref "microblaze_abicalls_attr")))


;;----------------------------------------------------------------
;; Microblaze DFA Pipeline description
;;----------------------------------------------------------------
                  
;;-----------------------------------------------------------------
/*
   This is description of pipeline hazards based on DFA.  The
   following constructions can be used for this:

   o define_cpu_unit string [string]) describes a cpu functional unit
     (separated by comma).

     1st operand: Names of cpu function units.
     2nd operand: Name of automaton (see comments for
     DEFINE_AUTOMATON).

     All define_reservations and define_cpu_units should have unique
     names which can not be "nothing".

   o (exclusion_set string string) means that each CPU function unit
     in the first string can not be reserved simultaneously with each
     unit whose name is in the second string and vise versa.  CPU
     units in the string are separated by commas. For example, it is
     useful for description CPU with fully pipelined floating point
     functional unit which can execute simultaneously only single
     floating point insns or only double floating point insns.

   o (presence_set string string) means that each CPU function unit in
     the first string can not be reserved unless at least one of units
     whose names are in the second string is reserved.  This is an
     asymmetric relation.  CPU units in the string are separated by
     commas.  For example, it is useful for description that slot1 is
     reserved after slot0 reservation for a VLIW processor.

   o (absence_set string string) means that each CPU function unit in
     the first string can not be reserved only if each unit whose name
     is in the second string is not reserved.  This is an asymmetric
     relation (actually exclusion set is analogous to this one but it
     is symmetric).  CPU units in the string are separated by commas.
     For example, it is useful for description that slot0 can not be
     reserved after slot1 or slot2 reservation for a VLIW processor.

   o (define_bypass number out_insn_names in_insn_names) names bypass with
     given latency (the first number) from insns given by the first
     string (see define_insn_reservation) into insns given by the
     second string.  Insn names in the strings are separated by
     commas.

   o (define_automaton string) describes names of an automaton
     generated and used for pipeline hazards recognition.  The names
     are separated by comma.  Actually it is possibly to generate the
     single automaton but unfortunately it can be very large.  If we
     use more one automata, the summary size of the automata usually
     is less than the single one.  The automaton name is used in
     define_cpu_unit.  All automata should have unique names.

   o (define_reservation string string) names reservation (the first
     string) of cpu functional units (the 2nd string).  Sometimes unit
     reservations for different insns contain common parts.  In such
     case, you describe common part and use one its name (the 1st
     parameter) in regular expression in define_insn_reservation.  All
     define_reservations, define results and define_cpu_units should
     have unique names which can not be "nothing".

   o (define_insn_reservation name default_latency condition regexpr)
     describes reservation of cpu functional units (the 3nd operand)
     for instruction which is selected by the condition (the 2nd
     parameter).  The first parameter is used for output of debugging
     information.  The reservations are described by a regular
     expression according the following syntax:

       regexp = regexp "," oneof
              | oneof

       oneof = oneof "|" allof
             | allof

       allof = allof "+" repeat
             | repeat

       repeat = element "*" number
              | element

       element = cpu_function_name
               | reservation_name
               | result_name
               | "nothing"
               | "(" regexp ")"

       1. "," is used for describing start of the next cycle in
          reservation.

       2. "|" is used for describing the reservation described by the
          first regular expression *or* the reservation described by
          the second regular expression *or* etc.

       3. "+" is used for describing the reservation described by the
          first regular expression *and* the reservation described by
          the second regular expression *and* etc.

       4. "*" is used for convenience and simply means sequence in
          which the regular expression are repeated NUMBER times with
          cycle advancing (see ",").

       5. cpu function unit name which means reservation.

       6. reservation name -- see define_reservation.

       7. string "nothing" means no units reservation.

*/
;;-----------------------------------------------------------------

;;----------------------------------------------------------------
;; Microblaze 3-stage pipeline description (for v4.00.a and earlier)
;;----------------------------------------------------------------

(define_automaton   "mbpipe_3")
(define_cpu_unit    "mb3_iu" "mbpipe_3")

(define_insn_reservation "mb3-integer" 1 
  (and (eq_attr "type" "branch,jump,call,arith,darith,icmp,nop,no_delay_arith")
       (eq (symbol_ref  "microblaze_pipe") (const_int MB_PIPE_3)))
  "mb3_iu")

(define_insn_reservation "mb3-special-move" 2
  (and (eq_attr "type" "move")
       (eq (symbol_ref  "microblaze_pipe") (const_int MB_PIPE_3)))
  "mb3_iu*2")

(define_insn_reservation "mb3-mem-load" 2
  (and (eq_attr "type" "load,no_delay_load")
       (eq (symbol_ref  "microblaze_pipe") (const_int MB_PIPE_3)))
  "mb3_iu")

(define_insn_reservation "mb3-mem-store" 1
  (and (eq_attr "type" "store,no_delay_store")
       (eq (symbol_ref  "microblaze_pipe") (const_int MB_PIPE_3)))
  "mb3_iu")

(define_insn_reservation "mb3-mul" 3
  (and (eq_attr "type" "imul,no_delay_imul")
       (eq (symbol_ref  "microblaze_pipe") (const_int MB_PIPE_3)))
  "mb3_iu,nothing*2")

(define_insn_reservation "mb3-div" 34            
  (and (eq_attr "type" "idiv")
       (eq (symbol_ref  "microblaze_pipe") (const_int MB_PIPE_3)))
    "mb3_iu,nothing*33")

(define_insn_reservation "mb3-bs" 2 
  (and (eq_attr "type" "bshift")
       (eq (symbol_ref  "microblaze_pipe") (const_int MB_PIPE_3)))
   "mb3_iu,nothing")

;; We are not producing FSL instructions anyways for this to have any practical use.
;; (define_insn_reservation "mb-fsl" 2 
;;  (and (eq_attr "type" "fsl")
;;       (eq (symbol_ref  "microblaze_pipe") (const_int MB_PIPE_3)))
;;    "mb3_iu*2")

(define_insn_reservation "mb3-fpu-add-sub-mul" 6
  (and (eq_attr "type" "fadd,frsub,fmul")
       (eq (symbol_ref  "microblaze_pipe") (const_int MB_PIPE_3)))
  "mb3_iu,nothing*5")

(define_insn_reservation "mb3-fpu-fcmp" 3
  (and (eq_attr "type" "fcmp")
       (eq (symbol_ref  "microblaze_pipe") (const_int MB_PIPE_3)))
  "mb3_iu,nothing*2")

(define_insn_reservation "mb3-fpu-div" 30
  (and (eq_attr "type" "fdiv")
       (eq (symbol_ref  "microblaze_pipe") (const_int MB_PIPE_3)))
  "mb3_iu,nothing*29")

(automata_option "v")
(automata_option "time")
(automata_option "progress")

;;----------------------------------------------------------------
;; Microblaze old pipeline description (for v4.00.a and earlier)
;; Deprecated. We now use the DFA model above.
;;----------------------------------------------------------------
                                      
;; (define_function_unit NAME MULTIPLICITY SIMULTANEITY
;;			TEST READY-DELAY ISSUE-DELAY [CONFLICT-LIST])

;; (define_function_unit "memory" 1 0 (eq_attr "type" "load") 2 0)

;; (define_function_unit "memory"  1 0 (eq_attr "type" "store") 1 0)

;; (define_function_unit "memory" 1 0 (eq_attr "type" "no_delay_load") 2 0)

;; (define_function_unit "memory"   1 0 (eq_attr "type" "no_delay_store") 2 0)

;; (define_function_unit "imuldiv"  1 0 (eq_attr "type" "imul") 2 0)

;; (define_function_unit "imuldiv"  1 0 (eq_attr "type" "idiv") 20 20)

;; (define_function_unit "bshift"  1 0 (eq_attr "type" "bshift") 2 0)


;;----------------------------------------------------------------
;; Microblaze delay slot description
;;----------------------------------------------------------------
(define_delay (eq_attr "type" "branch,call,jump")
  [(and (eq_attr "type" "!branch,call,jump,icmp,multi,no_delay_arith,no_delay_load,no_delay_store,no_delay_imul,no_delay_move,darith") 
        (ior (eq (symbol_ref "microblaze_no_dangerous_delay") (const_int 0))
              (eq_attr "type" "!fadd,frsub,fmul,fdiv,fcmp,store,load")
              ))
  (nil) (nil)])


;;----------------------------------------------------------------
;; Microblaze FPU
;;----------------------------------------------------------------

(define_insn "addsf3"
  [(set (match_operand:SF 0 "register_operand" "=d")
        (plus:SF (match_operand:SF 1 "register_operand" "d")
                 (match_operand:SF 2 "register_operand" "d")))]
  "TARGET_HARD_FLOAT"
  "fadd\t%0,%1,%2"
  [(set_attr "type"     "fadd")
  (set_attr "mode"      "SF")
  (set_attr "length"    "4")])

(define_insn "subsf3"
  [(set (match_operand:SF 0 "register_operand" "=d")
        (minus:SF (match_operand:SF 1 "register_operand" "d")
                  (match_operand:SF 2 "register_operand" "d")))]
  "TARGET_HARD_FLOAT"
  "frsub\t%0,%2,%1"
  [(set_attr "type"     "frsub")
  (set_attr "mode"      "SF")
  (set_attr "length"    "4")])

(define_insn "mulsf3"
  [(set (match_operand:SF 0 "register_operand" "=d")
        (mult:SF (match_operand:SF 1 "register_operand" "d")
                 (match_operand:SF 2 "register_operand" "d")))]
  "TARGET_HARD_FLOAT"
  "fmul\t%0,%1,%2"
  [(set_attr "type"     "fmul")
  (set_attr "mode"      "SF")
  (set_attr "length"    "4")])


(define_insn "divsf3"
  [(set (match_operand:SF 0 "register_operand" "=d")
        (div:SF (match_operand:SF 1 "register_operand" "d")
                (match_operand:SF 2 "register_operand" "d")))]
  "TARGET_HARD_FLOAT"
  "fdiv\t%0,%2,%1"
  [(set_attr "type"     "fdiv")
  (set_attr "mode"      "SF")
  (set_attr "length"    "4")])


;;----------------------------------------------------------------
;; Add
;;----------------------------------------------------------------

;; Add 2 SImode integers [ src1 = reg ; src2 = arith ; dest = reg ]

(define_expand "addsi3"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(plus:SI (match_operand:SI 1 "reg_or_0_operand" "dJ")
		 (match_operand:SI 2 "arith_operand" "d")))]
  ""
  ""
  )


;; Leave carry as is
(define_insn "addsi3_internal_reg"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(plus:SI (match_operand:SI 1 "reg_or_0_operand" "dJ")
		 (match_operand:SI 2 "register_operand" "d")))]
  "GET_CODE (operands[2]) != CONST_INT"
  "addk\t%0,%z1,%2"
  [(set_attr "type"	"arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"4")])

;; Leave carry as is
(define_insn "addsi3_internal_imm"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(plus:SI (match_operand:SI 1 "reg_or_0_operand" "dJ,dJ")
		 (match_operand:SI 2 "immediate_operand" "K,i")))]
  "(GET_CODE (operands[2]) == CONST_INT)"
  "addik\t%0,%z1,%2"
  [(set_attr "type"	"arith,no_delay_arith")
  (set_attr "mode"	"SI,SI")
  (set_attr "length"	"4,8")])

;;----------------------------------------------------------------
;; Double Precision Additions
;;----------------------------------------------------------------

;; reg_DI_dest = reg_DI_src1 + DI_src2

(define_expand "adddi3"
  [(set (match_operand:DI 0 "register_operand" "")
	(plus:DI (match_operand: DI 1 "register_operand" "")
		 (match_operand: DI 2 "arith_operand" "")))]
  ""
  ""
)

;; Adding 2 DI operands in register

(define_insn "adddi3_internal1"
  [(set (match_operand:DI 0 "register_operand" "=d")
	(plus:DI (match_operand:DI 1 "register_operand" "d")
		 (match_operand:DI 2 "register_operand" "d")))]
  ""
  { 
      return "add\t%L0,%L1,%L2\;addc\t%M0,%M1,%M2";
  }
  [(set_attr "type"	"darith")
  (set_attr "mode"	"DI")
  (set_attr "length"	"2")])

;; Adding 2 DI operands, one in reg, other imm.
(define_insn "adddi3_internal2"
  [(set (match_operand:DI 0 "register_operand" "=d")
	(plus:DI (match_operand:DI 1 "register_operand" "d")
		 (match_operand:SI 2 "immediate32_operand" "i")))]
  ""
  { 
        if (INTVAL (operands[2]) > 0)
                return "addi\t%L0,%L1,%2\;addc\t%M0,%M1,r0";
        else
                return "addi\t%L0,%L1,%2\;addc\t%M0,%M1,r0\;addi\t%M0,%M0,-1";
  }
  [(set_attr "type"	"darith")
  (set_attr "mode"	"DI")
  (set_attr "length"	"2")])


;;----------------------------------------------------------------
;; Add with carry 
;; 
;; This will never be generated by the compiler, 
;; via matching but by internal generation
;; OBSOLETE: Not used any more.
;;----------------------------------------------------------------

(define_insn "addc"
  [(set (match_operand:SI 0 "register_operand" "+d")
	(plus:SI (match_operand:SI 1 "register_operand" "+d")
		 (match_operand:SI 2 "register_operand" "+d")))
  (use (match_operand:SI 3 "arith_operand" "i"))]
  "(GET_CODE (operands[3]) == CONST_INT && (INTVAL(operands[3]) == 0xff))" 
  "addc\t%0,%1,%1"
  [(set_attr "type"	"arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])



;;----------------------------------------------------------------
;; Subtraction
;;----------------------------------------------------------------


(define_expand "subsi3"
  [(set (match_operand:SI 0 "register_operand" "=d,d,d")
	(minus:SI (match_operand:SI 1 "arith_operand" "dJ,dJ,i")
                  (match_operand:SI 2 "arith_operand" "d,i,dJ")))]
  ""
  ""
  )

(define_insn "subsi3_internal"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(minus:SI (match_operand:SI 1 "reg_or_0_operand" "dJ")
		  (match_operand:SI 2 "register_operand" "d")))]
  "(GET_CODE (operands[2]) != CONST_INT)"
  "rsubk\t%0,%2,%z1"
  [(set_attr "type"	"arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])


(define_insn "subsi3_internal2"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(minus:SI (match_operand:SI 1 "reg_or_0_operand" "dJ,dJ")
		  (match_operand:SI 2 "immediate_operand" "K,i")))]
  "(GET_CODE (operands[2]) == CONST_INT)"
  {
      return "addik\t%0,%z1,-%2";
  }
  [(set_attr "type"	"arith,no_delay_arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])

;;
;;(define_insn "subsi3_internal2"
;;  [(set (match_operand:SI 0 "register_operand" "=d,d")
;;	(minus:SI (match_operand:SI 1 "reg_or_0_operand" "dJ,dJ")
;;		  (match_operand:SI 2 "immediate_operand" "K,i")))]
;;  "(GET_CODE (operands[2]) == CONST_INT)"
;;  "addi\t%0,%z1,-%2"
;;  [(set_attr "type"	"arith,no_delay_arith")
;;   (set_attr "mode"	"SI")
;;   (set_attr "length"	"1")])

(define_insn "subsi3_internal3"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(minus:SI (match_operand:SI 1 "immediate_operand" "K,i")
		  (match_operand:SI 2 "register_operand" "dJ,dJ")))]
  "(GET_CODE (operands[1]) == CONST_INT)"
  "rsubik\t%0,%2,%z1"
  [(set_attr "type"	"arith,no_delay_arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])


;;----------------------------------------------------------------
;; Double Precision Subtraction
;;----------------------------------------------------------------
(define_expand "subdi3"
  [(set (match_operand:DI 0 "register_operand" "")
	(minus:DI (match_operand:DI 1 "register_operand" "")
		  (match_operand:DI 2 "arith_operand" "")))]
  ""
  ""
)

(define_insn "subdi3_internal"
  [(set (match_operand:DI 0 "register_operand" "=d")
	(minus:DI (match_operand:DI 1 "register_operand" "d")
		  (match_operand:DI 2 "register_operand" "d")))]
  "GET_CODE (operands[2]) != CONST_INT"
  "rsub\t%L0,%L2,%L1\;rsubc\t%M0,%M2,%M1"
  [(set_attr "type"	"darith")
  (set_attr "mode"	"DI")
  (set_attr "length"	"2")])


(define_insn "subdi3_internal_2"
  [(set (match_operand:DI 0 "register_operand" "=d,d,d")
	(minus:DI (match_operand:DI 1 "register_operand" "d,d,d")
		  (match_operand:DI 2 "small_int" "P,J,N")))
  (clobber (match_operand:SI 3 "register_operand" "=d,d,d"))]
  ""
  "@
   rsub\t%L0,%L2,%L1\;rsubc\t%M0,%M2,%M1
   add\t%L0,%L1\;add\t%M0,%M1
   rsub\t%L0,%L2,%L1\;rsubc\t%M0,%M2,%M1"
  [(set_attr "type"	"darith")
  (set_attr "mode"	"DI")
  (set_attr "length"	"2,2,2")])


;;----------------------------------------------------------------
;; Multiplication
;;----------------------------------------------------------------

(define_insn "mulsi3_imm0"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(mult:SI  (match_operand:SI 1 "register_operand" "d")
		  (match_operand:SI 2 "immediate_operand" "di")))]
  "INTVAL(operands[2]) == 0"
  "addk\t%0,r0,r0"
  [(set_attr "type"	"imul")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])

(define_insn "mulsi3_imm2"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(mult:SI  (match_operand:SI 1 "register_operand" "d,d")
		  (match_operand:SI 2 "immediate_operand" "K,i")))]
  "INTVAL(operands[2]) == 2"
  "addk\t%0,%1,%1"
  [(set_attr "type"	"imul,no_delay_imul")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1,2")])

(define_insn "mulsi3_imm4"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(mult:SI  (match_operand:SI 1 "register_operand" "d")
		  (match_operand:SI 2 "immediate_operand" "di")))]
  "(INTVAL(operands[2]) == 4 && (TARGET_SOFT_MUL))"
  "addk\t%0,%1,%1\;addk\t%0,%0,%0"
  [(set_attr "type"	"no_delay_imul")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])

(define_insn "mulsi3_imm8"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(mult:SI  (match_operand:SI 1 "register_operand" "d")
		  (match_operand:SI 2 "immediate_operand" "di")))]
  "(INTVAL(operands[2]) == 8 && (TARGET_SOFT_MUL))"
  "addk\t%0,%1,%1\;addk\t%0,%0,%0\;addk\t%0,%0,%0"
  [(set_attr "type"	"no_delay_imul")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])

(define_insn "mulsi3_imm"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(mult:SI  (match_operand:SI 1 "register_operand" "d,d")
		  (match_operand:SI 2 "immediate_operand" "K,i")))]
  "!TARGET_SOFT_MUL"
  "muli\t%0,%1,%2"
  [(set_attr "type"	"imul,no_delay_imul")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1,2")])

(define_expand "mulsi3"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(mult:SI (match_operand:SI 1 "register_operand" "d")
		 (match_operand:SI 2 "register_operand" "d")))
  (clobber (match_scratch:SI 3 "=d"))
  (clobber (match_scratch:SI 4 "=d"))]
  "!TARGET_SOFT_MUL"
  {
    emit_insn (gen_mulsi3_mult3 (operands[0], operands[1], operands[2]));
    DONE;
  }
)

(define_insn "mulsi3_mult3"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(mult:SI (match_operand:SI 1 "register_operand" "d")
		 (match_operand:SI 2 "register_operand" "d")))]
  "!TARGET_SOFT_MUL"
  "mul\t%0,%1,%2"
  [(set_attr "type"	"imul")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])


;;----------------------------------------------------------------
;; Division and remainder
;;----------------------------------------------------------------
(define_expand "divsi3"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(div:SI (match_operand:SI 1 "register_operand" "d")
                (match_operand:SI 2 "register_operand" "d")))
  ]
  "(!TARGET_SOFT_DIV) || (TARGET_BARREL_SHIFT && TARGET_SMALL_DIVIDES)"
  {
    if (TARGET_SOFT_DIV && TARGET_BARREL_SHIFT && TARGET_SMALL_DIVIDES) { 
        
        /* Table lookup software divides. Works for all (nr/dr) where (0 <= nr,dr <= 15) */

        rtx regt1 = gen_reg_rtx (SImode); 
        rtx reg18 = gen_rtx_REG (SImode, R_TMP);
        rtx regqi = gen_reg_rtx (QImode);
        rtx div_label = gen_label_rtx ();
        rtx div_label_ref = gen_rtx_LABEL_REF (VOIDmode, div_label);
        rtx div_end_label = gen_label_rtx ();
        rtx div_table_rtx = gen_rtx_SYMBOL_REF (QImode,"_divsi3_table");
        rtx mem_rtx;
        rtx op0, ret;
        rtx jump, cjump, insn;

        insn = emit_insn (gen_iorsi3 (regt1, operands[1], operands[2]));
        cjump = emit_jump_insn_after (gen_branch_compare_imm_uns (gen_rtx_GTU (SImode, regt1, GEN_INT (15)), regt1, GEN_INT (15), div_label_ref, pc_rtx), insn);
        LABEL_NUSES (div_label) = 1; 
        JUMP_LABEL (cjump) = div_label;
        emit_insn (gen_rtx_CLOBBER (SImode, reg18));

        emit_insn (gen_ashlsi3_bshift (regt1, operands[1], GEN_INT(4)));
        emit_insn (gen_addsi3 (regt1, regt1, operands[2]));
        mem_rtx = gen_rtx (MEM, QImode,
                            gen_rtx (PLUS, Pmode, regt1, div_table_rtx));

        insn = emit_insn (gen_movqi (regqi, mem_rtx)); 
        insn = emit_insn (gen_movsi (operands[0], gen_rtx_SUBREG (SImode, regqi, 0)));
        jump = emit_jump_insn_after (gen_jump (div_end_label), insn); 
        JUMP_LABEL (jump) = div_end_label;
        LABEL_NUSES (div_end_label) = 1; 
        emit_barrier ();

        emit_label (div_label);
        ret = emit_library_call_value (gen_rtx_SYMBOL_REF (Pmode, "__divsi3"), operands[0], LCT_NORMAL, GET_MODE (operands[0]), 
                                                           2, operands[1], GET_MODE (operands[1]), operands[2], GET_MODE (operands[2]));
        if (ret != operands[0])
                emit_move_insn (operands[0], ret);    

        emit_label (div_end_label);
        emit_insn (gen_blockage ());
        DONE;
    } else if (!TARGET_SOFT_DIV) {
        emit_insn (gen_divsi3_internal (operands[0], operands[1], operands[2]));
        DONE;
    }
  }     
)


(define_insn "divsi3_internal"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(div:SI (match_operand:SI 1 "register_operand" "d")
		(match_operand:SI 2 "register_operand" "d")))
  ]
  "!TARGET_SOFT_DIV"
  "idiv\t%0,%2,%1"
  [(set_attr "type"	"idiv")
  (set_attr "mode"	"SI")
  (set_attr "length"	"4")]
)

(define_expand "udivsi3"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(udiv:SI (match_operand:SI 1 "register_operand" "d")
                 (match_operand:SI 2 "register_operand" "d")))
  ]
  "!TARGET_SOFT_DIV"
  {
    emit_insn (gen_udivsi3_internal (operands[0], operands[1], operands[2]));
    DONE;
  }
)

(define_insn "udivsi3_internal"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(udiv:SI (match_operand:SI 1 "register_operand" "d")
                 (match_operand:SI 2 "register_operand" "d")))
  ]
  "!TARGET_SOFT_DIV"
  "idivu\t%0,%2,%1"
  [(set_attr "type"	"idiv")
  (set_attr "mode"	"SI")
  (set_attr "length"	"4")])


;;----------------------------------------------------------------
;; Negation and one's complement
;;----------------------------------------------------------------

(define_insn "negsi2"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(neg:SI (match_operand:SI 1 "register_operand" "d")))]
  ""
  {
        operands[2] = const0_rtx;
        return "rsubk\t%0,%1,%z2";
  }
  [(set_attr "type"	"arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])

(define_expand "negdi2"
  [(parallel [(set (match_operand:DI 0 "register_operand" "=d")
		   (neg:DI (match_operand:DI 1 "register_operand" "d")))
             (clobber (match_dup 2))])]
  ""
  {
    operands[2] = gen_reg_rtx (SImode);
  }
)

(define_insn "negdi2_internal"
  [(set (match_operand:DI 0 "register_operand" "=d")
	(neg:DI (match_operand:DI 1 "register_operand" "d")))
  (clobber (match_operand:SI 2 "register_operand" "=d"))]
  ""
  {
        operands[3] = const0_rtx;
        return "rsub\t%L0,%L1,%z3\;rsubc\t%M0,%M1,%z3";
  }
  [(set_attr "type"	"darith")
  (set_attr "mode"	"DI")
  (set_attr "length"	"2")])


(define_insn "one_cmplsi2"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(not:SI (match_operand:SI 1 "register_operand" "d")))]
  ""
  {
        return "xori\t%0,%1,-1";
  }
  [(set_attr "type"	"arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])

(define_insn "one_cmpldi2"
  [(set (match_operand:DI 0 "register_operand" "=d")
	(not:DI (match_operand:DI 1 "register_operand" "d")))]
  ""
  {
        operands[2] = const0_rtx;
        return "nor\t%M0,%z2,%M1\;nor\t%L0,%z2,%L1";
  }
  [(set_attr "type"	"darith")
  (set_attr "mode"	"DI")
  (set (attr "length") (const_int 2))])

(define_split
  [(set (match_operand:DI 0 "register_operand" "")
	(not:DI (match_operand:DI 1 "register_operand" "")))]
  "reload_completed && !TARGET_DEBUG_D_MODE && !TARGET_DEBUG_G_MODE
   && GET_CODE (operands[0]) == REG && GP_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == REG && GP_REG_P (REGNO (operands[1]))"

  [(set (subreg:SI (match_dup 0) 0) (not:SI (subreg:SI (match_dup 1) 0)))
  (set (subreg:SI (match_dup 0) 4) (not:SI (subreg:SI (match_dup 1) 4)))]
  "")


;;----------------------------------------------------------------
;; Logical
;;----------------------------------------------------------------

;; Many of these instructions uses trivial define_expands

(define_expand "andsi3"
  [(set (match_operand:SI 0 "register_operand" "")
	(and:SI (match_operand:SI 1 "uns_arith_operand" "")
		(match_operand:SI 2 "uns_arith_operand" "")))]
  ""
  ""
)

(define_insn "andsi3_internal"
  [(set (match_operand:SI 0 "register_operand" "=d,d,d,d")
	(and:SI (match_operand:SI 1 "uns_arith_operand" "%d,d,d,d")
		(match_operand:SI 2 "uns_arith_operand" "d,K,i,M")))]
  ""
  "@
   and\t%0,%1,%2  
   andi\t%0,%1,%2 #and1
   andi\t%0,%1,%2 #and2
   andi\t%0,%1,%2 #and3"
  [(set_attr "type"	"arith,arith,no_delay_arith,no_delay_arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])


(define_expand "anddi3"
  [(set (match_operand:DI 0 "register_operand" "=d")
	(and:DI (match_operand:DI 1 "register_operand" "d")
		(match_operand:DI 2 "register_operand" "d")))]
  "!TARGET_DEBUG_G_MODE"
  ""
)

(define_insn ""
  [(set (match_operand:DI 0 "register_operand" "=d")
	(and:DI (match_operand:DI 1 "register_operand" "d")
		(match_operand:DI 2 "register_operand" "d")))]
  "(!TARGET_DEBUG_G_MODE)"
  {
        return "and\t%M0,%M1,%M2\;and\t%L0,%L1,%L2";
  }
  [(set_attr "type"	"darith")
  (set_attr "mode"	"DI")
  (set (attr "length") (const_int 2))])


(define_split
  [(set (match_operand:DI 0 "register_operand" "")
	(and:DI (match_operand:DI 1 "register_operand" "")
		(match_operand:DI 2 "register_operand" "")))]
  "reload_completed && !TARGET_DEBUG_D_MODE && !TARGET_DEBUG_G_MODE
   && GET_CODE (operands[0]) == REG && GP_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == REG && GP_REG_P (REGNO (operands[1]))
   && GET_CODE (operands[2]) == REG && GP_REG_P (REGNO (operands[2]))"

  [(set (subreg:SI (match_dup 0) 0) (and:SI (subreg:SI (match_dup 1) 0) (subreg:SI (match_dup 2) 0)))
  (set (subreg:SI (match_dup 0) 4) (and:SI (subreg:SI (match_dup 1) 4) (subreg:SI (match_dup 2) 4)))]
  "")

(define_expand "iorsi3"
  [(set (match_operand:SI 0 "register_operand" "+d,d,d,d")
	(ior:SI (match_operand:SI 1 "uns_arith_operand" "%d,d,d,d")
		(match_operand:SI 2 "uns_arith_operand" "d,K,i,M")))]
  ""
  ""
)

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=d,d,d,d")
	(ior:SI (match_operand:SI 1 "uns_arith_operand" "%d,d,d,d")
		(match_operand:SI 2 "uns_arith_operand" "d,K,M,i")))]
  ""
  "@
   or\t%0,%1,%2 
   ori\t%0,%1,%2 
   ori\t%0,%1,%2 
   ori\t%0,%1,%2" 
  [(set_attr "type"	"no_delay_arith,no_delay_arith,no_delay_arith,no_delay_arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])


(define_expand "iordi3"
  [(set (match_operand:DI 0 "register_operand" "=d")
	(ior:DI (match_operand:DI 1 "register_operand" "d")
		(match_operand:DI 2 "register_operand" "d")))]
  "!TARGET_DEBUG_G_MODE"
  ""
)

(define_insn ""
  [(set (match_operand:DI 0 "register_operand" "=d")
	(ior:DI (match_operand:DI 1 "register_operand" "d")
		(match_operand:DI 2 "register_operand" "d")))]
  "(!TARGET_DEBUG_G_MODE)"
  {
        return "or\t%M0,%M1,%M2\;or\t%L0,%L1,%L2";
  }
  [(set_attr "type"	"darith")
  (set_attr "mode"	"DI")
  (set (attr "length") (const_int 2))])


(define_split
  [(set (match_operand:DI 0 "register_operand" "")
	(ior:DI (match_operand:DI 1 "register_operand" "")
		(match_operand:DI 2 "register_operand" "")))]
  "reload_completed && !TARGET_DEBUG_D_MODE && !TARGET_DEBUG_G_MODE
   && GET_CODE (operands[0]) == REG && GP_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == REG && GP_REG_P (REGNO (operands[1]))
   && GET_CODE (operands[2]) == REG && GP_REG_P (REGNO (operands[2]))"

  [(set (subreg:SI (match_dup 0) 0) (ior:SI (subreg:SI (match_dup 1) 0) (subreg:SI (match_dup 2) 0)))
  (set (subreg:SI (match_dup 0) 4) (ior:SI (subreg:SI (match_dup 1) 4) (subreg:SI (match_dup 2) 4)))]
  "")

(define_expand "xorsi3"
  [(set (match_operand:SI 0 "register_operand" "=d,d,d")
	(xor:SI (match_operand:SI 1 "uns_arith_operand" "%d,d,d")
		(match_operand:SI 2 "uns_arith_operand" "d,K,i")))]
  ""
  ""
)

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=d,d,d")
	(xor:SI (match_operand:SI 1 "uns_arith_operand" "%d,d,d")
		(match_operand:SI 2 "uns_arith_operand" "d,K,i")))]
  ""
  "@
   xor\t%0,%1,%2
   xori\t%0,%1,%2
   xori\t%0,%1,%2"
  [(set_attr "type"	"arith,arith,no_delay_arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])

;; ??? If delete the 32-bit long long patterns, then could merge this with
;; the following xordi3_internal pattern.
(define_expand "xordi3"
  [(set (match_operand:DI 0 "register_operand" "=d")
	(xor:DI (match_operand:DI 1 "register_operand" "d")
		(match_operand:DI 2 "register_operand" "d")))]
  "!TARGET_DEBUG_G_MODE"
  ""
)

(define_insn ""
  [(set (match_operand:DI 0 "register_operand" "=d")
	(xor:DI (match_operand:DI 1 "register_operand" "d")
		(match_operand:DI 2 "register_operand" "d")))]
  "(!TARGET_DEBUG_G_MODE) "
  {
        return "xor\t%M0,%M1,%M2\;xor\t%L0,%L1,%L2";
  }
  [(set_attr "type"	"darith")
  (set_attr "mode"	"DI")
  (set (attr "length") (const_int 2))])


(define_split
  [(set (match_operand:DI 0 "register_operand" "")
	(xor:DI (match_operand:DI 1 "register_operand" "")
		(match_operand:DI 2 "register_operand" "")))]
  "reload_completed && !TARGET_DEBUG_D_MODE && !TARGET_DEBUG_G_MODE
   && GET_CODE (operands[0]) == REG && GP_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == REG && GP_REG_P (REGNO (operands[1]))
   && GET_CODE (operands[2]) == REG && GP_REG_P (REGNO (operands[2]))"

  [(set (subreg:SI (match_dup 0) 0) (xor:SI (subreg:SI (match_dup 1) 0) (subreg:SI (match_dup 2) 0)))
  (set (subreg:SI (match_dup 0) 4) (xor:SI (subreg:SI (match_dup 1) 4) (subreg:SI (match_dup 2) 4)))]
  "")

;;----------------------------------------------------------------
;; Truncation
;;----------------------------------------------------------------

;;----------------------------------------------------------------
;; Zero extension
;;----------------------------------------------------------------

;; Extension insns.
;; Those for integer source operand are ordered widest source type first.

(define_expand "zero_extendsidi2"
  [(set (match_operand:DI 0 "register_operand" "")
	(zero_extend:DI (match_operand:SI 1 "nonimmediate_operand" "")))]
  ""
  {
    /* In GCC 3.4.1 all SUBREG have changed from 1 to 4 */
    rtx temp = gen_rtx_SUBREG (SImode,operands[0],4);
    gen_rtx_CLOBBER (SImode,temp);
    /*	operands[0] = temp; */
    emit_insn (gen_zero_extendsidi2_internal(operands[0], operands[1]));
    DONE;
  }
)

(define_insn "zero_extendsidi2_internal"
  [(set (match_operand:DI 0 "register_operand" "=d,d,d")
	(zero_extend:DI (match_operand:SI 1 "nonimmediate_operand" "d,R,m")))]
  ""
  {
        if (which_alternative == 0)
                return "addik\t%D0,%1,0\;addik\t%0,r0,0";
        else {
                char *temp2, *temp_final;
                char *temp = microblaze_move_1word(operands,insn,TRUE);

                operands[2] = gen_rtx_SUBREG(SImode, operands[0],4);
                temp2 = "\n\taddk\t%D0,r0,%0\;addk\t%0,r0,r0";

                temp_final = (char*) xmalloc(strlen(temp) + strlen(temp2) + 1);
                strcpy(temp_final,temp);
                strcat(temp_final,temp2);
                return temp_final;
        }
  }
  [(set_attr "type"	"no_delay_arith,no_delay_load,no_delay_load")
  (set_attr "mode"	"SI")
  (set_attr "length"	"2,2,2")])


(define_expand "zero_extendhisi2"
  [(set (match_operand:SI 0 "register_operand" "")
	(zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "")))]
  ""
  ""
)

;; [MB-Delay] Cannot have this load being done in the delay slot 

(define_insn "zero_extendhisi2_internal"
  [(set (match_operand:SI 0 "register_operand" "=d,d,d")
	(zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "d,R,m")))]
  ""
  {
        if (which_alternative == 0)
                return "andi\t%0,%1,0xffff";
        else {
                /*  fprintf(stderr,"move_1word 1 \n"); */
                return microblaze_move_1word (operands, insn, TRUE);
        }
  }
  [(set_attr "type"	"no_delay_arith,no_delay_load,no_delay_load")
  (set_attr "mode"	"SI")
  (set_attr "length"	"2,2,2")])


(define_expand "zero_extendqihi2"
  [(set (match_operand:HI 0 "register_operand" "")
	(zero_extend:HI (match_operand:QI 1 "nonimmediate_operand" "")))]
  ""
  ""
)

;; [MB-Delay] Cannot have this load being done in the delay slot 
(define_insn "zero_extendqihi2_internal"
  [(set (match_operand:HI 0 "register_operand" "=d,d,d")
	(zero_extend:HI (match_operand:QI 1 "nonimmediate_operand" "d,R,m")))]
  ""
  {
        if (which_alternative == 0)
                return "andi\t%0,%1,0x00ff";
        else {
                return microblaze_move_1word (operands, insn, TRUE);
        }
  }
  [(set_attr "type"	"arith,no_delay_load,no_delay_load")
  (set_attr "mode"	"HI")
  (set_attr "length"	"2,2,2")])

(define_expand "zero_extendqisi2"
  [(set (match_operand:SI 0 "register_operand" "")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "")))]
  ""
  ""
)

;; [MB-Delay] Cannot have this load being done in the delay slot 
(define_insn "zero_extendqisi2_internal"
  [(set (match_operand:SI 0 "register_operand" "=d,d,d")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "d,R,m")))]
  ""
  {
        if (which_alternative == 0)
                return "andi\t%0,%1,0x00ff";
        else {
        /*    fprintf(stderr,"move_1_word3 \n");*/
              return microblaze_move_1word (operands, insn, TRUE);
        }
  }
  [(set_attr "type"	"arith,no_delay_load,no_delay_load")
  (set_attr "mode"	"SI")
  (set_attr "length"	"2,2,2")])



;;----------------------------------------------------------------
;; Sign extension
;;----------------------------------------------------------------

;; basic Sign Extend Operations

(define_insn "sext16"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(sign_extend:SI (match_operand:HI 1 "register_operand" "d")))]
  ""
  "sext16\t%0,%1"
  [(set_attr "type"	"arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])

(define_insn "sext8"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(sign_extend:SI (match_operand:QI 1 "register_operand" "d")))]
  "(GET_CODE(operands[1]) != MEM)"
  "sext8\t%0,%1"
  [(set_attr "type"	"arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"1")])


;; Extension insns.  
;; Those for integer source operand are ordered
;; widest source type first.

(define_expand "extendsidi2"
  [(set (match_operand:DI 0 "register_operand" " ")
	(sign_extend:DI (match_operand:SI 1 "nonimmediate_operand" " ")))]
  ""
  { 
    rtx temp = gen_rtx_SUBREG (SImode,operands[0],4);
    gen_rtx_CLOBBER (SImode,temp);
    emit_insn (gen_extendsidi2_internal(operands[0], operands[1]));
    DONE;
  }
)

(define_insn "extendsidi2_internal"
  [(set (match_operand:DI 0 "register_operand" "=d,d,d")
	(sign_extend:DI (use (match_operand:SI 1 "nonimmediate_operand" "d,R,m"))))]
  ""
  { 
        char *temp, *temp2, *temp_final;
        /*    fprintf(stderr,"move_1word - II\n");*/
        temp = microblaze_move_1word(operands, insn, FALSE);
        /*     fprintf(stderr,"Sign Extending DI \n");*/
        /*      operands[2] = gen_rtx(REG,REGNO(operands[0]) + 1);*/
        operands[2] = gen_rtx_SUBREG(SImode,operands[0],4);
	temp2 = "%_\;add\t%D0,r0,%0\;add\t%0,%D0,%D0\;addc\t%0,r0,r0\;beqi\t%0,%-signextend\;addi\t%0,r0,0xffffffff\n%-signextend:";
  	temp_final = (char*) xmalloc(strlen(temp) + strlen(temp2) + 1);
	strcpy(temp_final,temp);
	strcat(temp_final,temp2);
        return temp_final;
        /*return temp; */
  }
  [(set_attr "type"	"multi,multi,multi")
  (set_attr "mode"	"DI")
  (set_attr "length"	"5,5,5")])


;; Sign extension from QI to DI mode

(define_expand "extendqidi2"
  [(set (match_operand:DI 0 "register_operand" " ")
	(sign_extend:DI (use(match_operand:QI 1 "nonimmediate_operand" " "))))]
  ""
  { 
    rtx temp = gen_rtx_SUBREG (SImode,operands[0],4);
    gen_rtx_CLOBBER (SImode,temp);
    /*     fprintf (stderr,"In the extendqidi2\n");*/
    emit_insn (gen_extendqidi2_internal(operands[0], operands[1]));
    DONE;
  }
)

;; Sign extension from QI to DI mode - internal step

(define_insn "extendqidi2_internal"
  [(set (match_operand:DI 0 "register_operand" "=d,d,d")
	(sign_extend:DI (use (match_operand:QI 1 "nonimmediate_operand" "d,R,m"))))]
  ""
  { 
        char *temp, *temp2, *temp_final;
        /*      fprintf(stderr,"move_1word - II\n");*/
        temp = microblaze_move_1word(operands, insn, FALSE);
        /*      fprintf(stderr,"Sign Extending DI \n");*/
        /*      operands[2] = gen_rtx(REG,REGNO(operands[0]) + 1);*/
        operands[2] = gen_rtx_SUBREG(SImode,operands[0],4);

	temp2 = "%_\n\tsext8\t%0,%0\;add\t%D0,r0,%0\;add\t%0,%D0,%D0\;addc\t%0,r0,r0\;beqi\t%0,%-signextend\;addi\t%0,r0,0xffffffff\n%-signextend:";
  	temp_final = (char*) xmalloc(strlen(temp) + strlen(temp2) + 1);
	strcpy(temp_final,temp);
	strcat(temp_final,temp2);
        return temp_final;
        /*	return temp;*/
  }
  [(set_attr "type"	"multi,multi,multi")
  (set_attr "mode"	"DI")
  (set_attr "length"	"5,5,5")])


;; Sign extension from HI to DI mode

(define_expand "extendhidi2"
  [(set (match_operand:DI 0 "register_operand" " ")
	(sign_extend:DI (use(match_operand:HI 1 "nonimmediate_operand" " "))))]
  ""
  { 
    rtx temp = gen_rtx_SUBREG(SImode,operands[0],4);
    gen_rtx_CLOBBER(SImode,temp);
    emit_insn(gen_extendhidi2_internal(operands[0], operands[1]));
    DONE;
  }
)

;; Sign extension from QI to DI mode - internal step

(define_insn "extendhidi2_internal"
  [(set (match_operand:DI 0 "register_operand" "=d,d,d")
	(sign_extend:DI (use (match_operand:HI 1 "nonimmediate_operand" "d,R,m"))))]
  ""
  { 
        char *temp, *temp2, *temp_final;
        /*      fprintf(stderr,"move_1word - II\n");*/
        temp = microblaze_move_1word(operands, insn, FALSE);
        /*      fprintf(stderr,"Sign Extending DI \n");*/
        /*      operands[2] = gen_rtx(REG,REGNO(operands[0]) + 1);*/
        operands[2] = gen_rtx_SUBREG(SImode,operands[0],4);

	temp2 = "%_\n\tsext16\t%0,%0\;add\t%D0,r0,%0\;add\t%0,%D0,%D0\;addc\t%0,r0,r0\;beqi\t%0,%-signextend\;addi\t%0,r0,0xffffffff\n%-signextend:";
  	temp_final = (char*) xmalloc(strlen(temp) + strlen(temp2) + 1);
	strcpy(temp_final,temp);
	strcat(temp_final,temp2);
        return temp_final;
        /*	return temp; */
  }
  [(set_attr "type"	"multi,multi,multi")
  (set_attr "mode"	"DI")
  (set_attr "length"	"5,5,5")])

;; Should change this extend to have check on the MSB and then
;; appropriate OR

(define_expand "extendhisi2"
  [(set (match_operand:SI 0 "register_operand" "")
	(sign_extend:SI (match_operand:HI 1 "nonimmediate_operand" "")))]
  ""
  {
    /* earlier this was being done only for optimize. Removed the optimize
       stuff from here */
    /*  if (optimize && GET_CODE (operands[1]) == MEM)*/
    if (GET_CODE (operands[1]) == MEM)
        operands[1] = force_not_mem (operands[1]);
  }
)


;; Need to change this similar lines as the HI->SI conversion

(define_expand "extendqihi2"
  [(set (match_operand:HI 0 "register_operand" "")
	(sign_extend:HI (match_operand:QI 1 "nonimmediate_operand" "")))]
  ""
  {
    if (GET_CODE (operands[1]) == MEM)
        operands[1] = force_not_mem (operands[1]);
    {
        rtx op0   = gen_lowpart (SImode, operands[0]);
        rtx op1   = gen_lowpart (SImode, operands[1]);
        rtx lowpart = GEN_INT(0x0000ffff);

        emit_insn (gen_sext8(op0,operands[1]));
        emit_insn (gen_andsi3(op1,op0,lowpart));
        emit_insn (gen_blockage ());
        DONE;
    }
}
)


(define_expand "extendqisi2"
  [(set (match_operand:SI 0 "register_operand" "")
	(sign_extend:SI (match_operand:QI 1 "nonimmediate_operand" "")))]
  ""
  {
    if (GET_CODE (operands[1]) == MEM)
        operands[1] = force_not_mem (operands[1]);
    emit_insn (gen_sext8 (operands[0], operands[1]));	
    DONE;
  }
)



;;----------------------------------------------------------------
;; Data movement
;;----------------------------------------------------------------

;; 64-bit integer moves

;; Unlike most other insns, the move insns can't be split with
;; different predicates, because register spilling and other parts of
;; the compiler, have memoized the insn number already.

(define_expand "movdi"
  [(set (match_operand:DI 0 "nonimmediate_operand" "")
	(match_operand:DI 1 "general_operand" ""))]
  ""
  {
    /* If we are generating embedded PIC code, and we are referring to a
       symbol in the .text section, we must use an offset from the start
       of the function.  */
    if (TARGET_EMBEDDED_PIC
        && (GET_CODE (operands[1]) == LABEL_REF
	|| (GET_CODE (operands[1]) == SYMBOL_REF
	&& ! SYMBOL_REF_FLAG (operands[1]))))
    {
        rtx temp;
        temp = embedded_pic_offset (operands[1]);
        temp = gen_rtx (PLUS, Pmode, embedded_pic_fnaddr_rtx,
	                force_reg (DImode, temp));
        emit_move_insn (operands[0], force_reg (DImode, temp));
        DONE;
    }

    /* If operands[1] is a constant address illegal for pic, then we need to
       handle it just like LEGITIMIZE_ADDRESS does.  */
    if (flag_pic && pic_address_needs_scratch (operands[1]))
    {
        rtx temp = force_reg (DImode, XEXP (XEXP (operands[1], 0), 0));
        rtx temp2 = XEXP (XEXP (operands[1], 0), 1);
        /* if (! SMALL_INT (temp2))
	temp2 = force_reg (DImode, temp2);
        */
        emit_move_insn (operands[0], gen_rtx (PLUS, DImode, temp, temp2));
        DONE;
    }


    if ((reload_in_progress | reload_completed) == 0
        && !register_operand (operands[0], DImode)
        && !register_operand (operands[1], DImode)
        && (((GET_CODE (operands[1]) != CONST_INT || INTVAL (operands[1]) != 0)
	       && operands[1] != CONST0_RTX (DImode))))
    {

      rtx temp = force_reg (DImode, operands[1]);
      emit_move_insn (operands[0], temp);
      DONE;
    }
  }
)



(define_insn "movdi_internal"
  [(set (match_operand:DI 0 "nonimmediate_operand" "=d,d ,d,d,R,m")
	(match_operand:DI 1 "general_operand"      " d,iF,R,m,d,d"))]
  "(register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)
       || (GET_CODE (operands[1]) == CONST_INT && INTVAL (operands[1]) == 0)
       || operands[1] == CONST0_RTX (DImode))"
  { 
        /*        fprintf(stderr,"move_2words-I\n");*/
	return microblaze_move_2words (operands, insn); 
  }
  [(set_attr "type"	"no_delay_move,no_delay_arith,no_delay_load,no_delay_load,no_delay_store,no_delay_store")
  (set_attr "mode"	"DI")
  (set_attr "length"   "2,4,2,4,2,4")])

;;(define_insn "movdi_internal_2"
;;  [(set (match_operand:DI 0 "nonimmediate_operand" "")
;;	(match_operand:DI 1 "general_operand" ""))]
;;  ""
;;  { 
;;      fprintf(stderr,"move_2words-II\n");
;;	return microblaze_move_2words (operands, insn); 
;;  }
;;  [(set_attr "type"	"no_delay_move")
;;   (set_attr "mode"	"DI")
;;   (set_attr "length"   "2")])

(define_split
  [(set (match_operand:DI 0 "register_operand" "")
	(match_operand:DI 1 "register_operand" ""))]
  "reload_completed && !TARGET_DEBUG_D_MODE && !TARGET_DEBUG_G_MODE
   && GET_CODE (operands[0]) == REG && GP_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == REG && GP_REG_P (REGNO (operands[1])) 
   && (REGNO(operands[0]) == (REGNO(operands[1]) + 1))"

  [(set (subreg:SI (match_dup 0) 4) (subreg:SI (match_dup 1) 4))
  (set (subreg:SI (match_dup 0) 0) (subreg:SI (match_dup 1) 0))]
  "")

(define_split
  [(set (match_operand:DI 0 "register_operand" "")
	(match_operand:DI 1 "register_operand" ""))]
  "reload_completed && !TARGET_DEBUG_D_MODE && !TARGET_DEBUG_G_MODE
   && GET_CODE (operands[0]) == REG && GP_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == REG && GP_REG_P (REGNO (operands[1])) 
   && (REGNO(operands[0]) != (REGNO(operands[1]) + 1))"

  [(set (subreg:SI (match_dup 0) 0) (subreg:SI (match_dup 1) 0))
  (set (subreg:SI (match_dup 0) 4) (subreg:SI (match_dup 1) 4))]
  "")


;; ;; 32-bit Integer moves
;;  This looks quite bogus.
;; (define_split
;;   [(set (match_operand:SI 0 "register_operand" "")
;; 	(match_operand:SI 1 "large_int" ""))]
;;   "!TARGET_DEBUG_D_MODE "
;;   [(set (match_dup 0)
;; 	(match_dup 2))
;;   (set (match_dup 0)
;;        (ior:SI (match_dup 0)
;;                (match_dup 3)))]
;;   "
;; {
;;   operands[2] = GEN_INT (INTVAL (operands[1]) & 0xffff0000);
;;   operands[3] = GEN_INT (INTVAL (operands[1]) & 0x0000ffff);
;; }")

;; Unlike most other insns, the move insns can't be split with
;; different predicates, because register spilling and other parts of
;; the compiler, have memoized the insn number already.

(define_expand "movsi"
  [(set (match_operand:SI 0 "nonimmediate_operand" "")
	(match_operand:SI 1 "general_operand" ""))]
  ""
  {

    /* If we are generating embedded PIC code, and we are referring to a
       symbol in the .text section, we must use an offset from the start
       of the function.  */
    if (TARGET_EMBEDDED_PIC
        && (GET_CODE (operands[1]) == LABEL_REF
	    || (GET_CODE (operands[1]) == SYMBOL_REF
	        && ! SYMBOL_REF_FLAG (operands[1]))))
    {
        rtx temp;

        temp = embedded_pic_offset (operands[1]);
        temp = gen_rtx (PLUS, Pmode, embedded_pic_fnaddr_rtx,
	                force_reg (SImode, temp));
        emit_move_insn (operands[0], force_reg (SImode, temp));
        DONE;
    }

    /* If operands[1] is a constant address invalid for pic, then we need to
       handle it just like LEGITIMIZE_ADDRESS does.  */
    if (flag_pic && pic_address_needs_scratch (operands[1]))
    {
        rtx temp = force_reg (SImode, XEXP (XEXP (operands[1], 0), 0));
        rtx temp2 = XEXP (XEXP (operands[1], 0), 1);

        /* if (! SMALL_INT (temp2))
	   temp2 = force_reg (SImode, temp2);
        */
        emit_move_insn (operands[0], gen_rtx (PLUS, SImode, temp, temp2));
        DONE;
    }

    if ((reload_in_progress | reload_completed) == 0
        && !register_operand (operands[0], SImode)
        && !register_operand (operands[1], SImode)
        && (GET_CODE (operands[1]) != CONST_INT
	    || INTVAL (operands[1]) != 0))
    {
        rtx temp = force_reg (SImode, operands[1]);
        emit_move_insn (operands[0], temp);
        DONE;
    }
  }
)


;; This is for the memory accessed using small data pointer to be moved in the delay slot.
;;(define_insn "movsi_internal4"
;;  [(set (match_operand:SI 0 "nonimmediate_operand" "=d,R")
;;	(match_operand:SI 1 "move_operand" "R,d"))]
;;  "((register_operand (operands[0], SImode) && (
;;           (VAR_SECTION (XEXP(operands[1],0)) == SDATA_VAR) ||
;;           (VAR_SECTION (XEXP(operands[1],0)) == SBSS_VAR) ||	
;;           (VAR_SECTION (XEXP(operands[1],0)) == SDATA2_VAR)
;;	)) ||
;;     (register_operand (operands[1], SImode) && (
;;           (VAR_SECTION (XEXP(operands[0],0)) == SDATA_VAR) ||
;;           (VAR_SECTION (XEXP(operands[0],0)) == SBSS_VAR) ||	
;;           (VAR_SECTION (XEXP(operands[0],0)) == SDATA2_VAR)
;;	))
;;    )"  
;;  { 
;;	 return microblaze_move_1word (operands, insn, FALSE);
;;  }
;;  [(set_attr "type"	"load,store")
;;   (set_attr "mode"	"SI")
;;   (set_attr "length"	"1")])

;; Added for status resgisters 
(define_insn "movsi_status"
  [(set (match_operand:SI 0 "register_operand" "=d,d,z")
        (match_operand:SI 1 "register_operand" "z,d,d"))]
  "interrupt_handler"
  "@
	mfs\t%0,%1  #mfs
	addk\t%0,%1,r0 #add movsi
	mts\t%0,%1  #mts"	
  [(set_attr "type" "move")
  (set_attr "mode" "SI")
  (set_attr "length" "1")])

;; This move will be not be moved to delay slot.	
(define_insn "movsi_internal3"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=d,d")
	(match_operand:SI 1 "immediate_operand" "IKL,Mnis"))]
  "(register_operand (operands[0], SImode) && 
           (GET_CODE (operands[1]) == CONST_INT && 
                 (INTVAL (operands[1]) <= 32767 && INTVAL(operands[1]) >= -32768)))"  
  { 
	 return microblaze_move_1word (operands, insn, FALSE);
  }
  [(set_attr "type"	"arith,no_delay_arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"4")])

(define_insn "movsi_internal2"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=d,d,d,d, d,   d,d,T ,R ")
	(match_operand:SI 1 "move_operand"         " d,S,K,IL,Mnis,R,m,dJ,dJ"))]
  "!TARGET_DEBUG_H_MODE
   && (register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode) 
      || (GET_CODE (operands[1]) == CONST_INT && INTVAL (operands[1]) == 0))"
  { 
	 return microblaze_move_1word (operands, insn, FALSE);
  }
  [(set_attr "type"	"load,no_delay_load,load,no_delay_load,no_delay_load,no_delay_load,no_delay_load,no_delay_store,store")
  (set_attr "mode"	"SI")
  (set_attr "length"	"4,8,4,8,8,4,8,4,8")])


;; 16-bit Integer moves

;; Unlike most other insns, the move insns can't be split with
;; different predicates, because register spilling and other parts of
;; the compiler, have memoized the insn number already.
;; Unsigned loads are used because BYTE_LOADS_ZERO_EXTEND is defined

(define_expand "movhi"
  [(set (match_operand:HI 0 "nonimmediate_operand" "")
	(match_operand:HI 1 "general_operand" ""))]
  ""
  {
    if ((reload_in_progress | reload_completed) == 0
        && !register_operand (operands[0], HImode)
        && !register_operand (operands[1], HImode)
        && ((GET_CODE (operands[1]) != CONST_INT
  	    || INTVAL (operands[1]) != 0)))
    {
        rtx temp = force_reg (HImode, operands[1]);
        emit_move_insn (operands[0], temp);
        DONE;
    }
  }
)

;; Defined to be the only movhi instruction, which can be placed in a
;; delay slot. Small integers

(define_insn "movhi_internal3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=d")
	(match_operand:HI 1 "immediate_operand"       "IK"))]
  "(register_operand (operands[0], HImode) && 
           (GET_CODE (operands[1]) == CONST_INT && 
                 (INTVAL (operands[1]) <= 32767 && INTVAL(operands[1]) >= -32768)))"
  {
	return microblaze_move_1word (operands, insn, TRUE);
  }
  [(set_attr "type"	"arith")
  (set_attr "mode"	"HI")
  (set_attr "length"	"2")])

;; The only movhi instruction, which can be placed in a
;; delay slot is the move..rest cannot be moved to delay slot. For the time being.

(define_insn "movhi_internal2"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=d,d,d,d,R,m")
	(match_operand:HI 1 "general_operand"       "d,IK,R,m,dJ,dJ"))]
  "(register_operand (operands[0], HImode)
       || register_operand (operands[1], HImode)
       || (GET_CODE (operands[1]) == CONST_INT && INTVAL (operands[1]) == 0))"
  {
	return microblaze_move_1word (operands, insn, TRUE);
  }
  [(set_attr "type"	"move,no_delay_arith,no_delay_load,no_delay_load,no_delay_store,no_delay_store")
  (set_attr "mode"	"HI")
  (set_attr "length"	"1,2,2,2,2,2")])

;; 8-bit Integer moves

;; Unlike most other insns, the move insns can't be split with
;; different predicates, because register spilling and other parts of
;; the compiler, have memoized the insn number already.
;; Unsigned loads are used because BYTE_LOADS_ZERO_EXTEND is defined

(define_expand "movqi"
  [(set (match_operand:QI 0 "nonimmediate_operand" "")
	(match_operand:QI 1 "general_operand" ""))]
  ""
  {
    if ((reload_in_progress | reload_completed) == 0
        && !register_operand (operands[0], QImode)
        && !register_operand (operands[1], QImode)
        && ((GET_CODE (operands[1]) != CONST_INT
            || INTVAL (operands[1]) != 0)))
    {
        rtx temp = force_reg (QImode, operands[1]);
        emit_move_insn (operands[0], temp);
        DONE;
    }
  }
)

;; Defined to be the only movhi instruction, which can be placed in a
;; delay slot. Small integers


(define_insn "movqi_internal3"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=d")
	(match_operand:QI 1 "immediate_operand"       "IK"))]
  "(register_operand (operands[0], QImode) && 
           (GET_CODE (operands[1]) == CONST_INT && 
                 (INTVAL (operands[1]) <= 32767 && INTVAL(operands[1]) >= -32768)))"
  {
        /*fprintf(stderr,"move_1word - 8\n");*/ 
        return microblaze_move_1word (operands, insn, TRUE);
  }
  [(set_attr "type"	"arith")
  (set_attr "mode"	"QI")
  (set_attr "length"	"2")])

;; The only movqi instruction, which can be placed in a
;; delay slot is the move..rest cannot be moved to delay slot. For the time being.

(define_insn "movqi_internal2"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=d,d,d,d,R,m")
	(match_operand:QI 1 "general_operand"       "d,IK,R,m,dJ,dJ"))]
  "(register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode)
       || (GET_CODE (operands[1]) == CONST_INT && INTVAL (operands[1]) == 0))"
  {
        /*fprintf(stderr,"move_1word - 8\n");*/ 
        return microblaze_move_1word (operands, insn, TRUE);
  }
  [(set_attr "type"	"move,no_delay_arith,no_delay_load,no_delay_load,no_delay_store,no_delay_store")
  (set_attr "mode"	"QI")
  (set_attr "length"	"2,2,2,2,2,2")])



;; 32-bit floating point moves

(define_expand "movsf"
  [(set (match_operand:SF 0 "nonimmediate_operand" "")
        (match_operand:SF 1 "general_operand" ""))]
  ""
  {
    if ((reload_in_progress | reload_completed) == 0
        && !register_operand (operands[0], SFmode)
        && !register_operand (operands[1], SFmode)
        && ( ((GET_CODE (operands[1]) != CONST_INT || INTVAL (operands[1]) != 0)
                 && operands[1] != CONST0_RTX (SFmode))))
    {
        rtx temp = force_reg (SFmode, operands[1]);
        emit_move_insn (operands[0], temp);
        DONE;
    }
  }
)

;; (define_insn "movsf_internal1"
;;   [(set (match_operand:SF 0 "nonimmediate_operand" "=d,d,d,R,m,d")
;;      (match_operand:SF 1 "general_operand" "d,R,m,d,d,*G"))]
;;         "TARGET_HARD_FLOAT && (register_operand (operands[0], SFmode)
;;        || register_operand (operands[1], SFmode)
;;        || (GET_CODE (operands[1]) == CONST_INT && INTVAL (operands[1]) == 0)
;;        || operands[1] == CONST0_RTX (SFmode))"
;;   { return microblaze_move_1word (operands, insn, FALSE);}
;;   [(set_attr "type"  "move,no_delay_load,no_delay_load,no_delay_store,no_delay_store,no_delay_move")
;;   (set_attr "mode"   "SF")
;;   (set_attr "length" "4,4,4,4,4,4")])

;; movsf_internal
;; Applies to both TARGET_SOFT_FLOAT and TARGET_HARD_FLOAT
;;
(define_insn "movsf_internal"
  [(set (match_operand:SF 0 "nonimmediate_operand" "=d,d,d,R,m")
        (match_operand:SF 1 "general_operand" "Gd,R,Fm,d,d"))]
  "(register_operand (operands[0], SFmode)
       || register_operand (operands[1], SFmode)
       || (GET_CODE (operands[1]) == CONST_INT && INTVAL (operands[1]) == 0)
       || operands[1] == CONST0_RTX (SFmode))"
  { 
        return microblaze_move_1word (operands, insn, FALSE);
  }
  [(set_attr "type"     "move,no_delay_load,no_delay_load,no_delay_store,no_delay_store")
  (set_attr "mode"      "SF")
  (set_attr "length"    "4,4,4,4,4")])

;; 64-bit floating point moves
(define_expand "movdf"
  [(set (match_operand:DF 0 "nonimmediate_operand" "")
        (match_operand:DF 1 "general_operand" ""))]
  ""
  {
    if ((reload_in_progress | reload_completed) == 0
        && !register_operand (operands[0], DFmode)
        && !register_operand (operands[1], DFmode)
        && (((GET_CODE (operands[1]) != CONST_INT || INTVAL (operands[1]) != 0)
                 && operands[1] != CONST0_RTX (DFmode))))
    {
        rtx temp = force_reg (DFmode, operands[1]);
        emit_move_insn (operands[0], temp);
        DONE;
    }
  }
)

;; (define_insn "movdf_internal1"
;;   [(set (match_operand:DF 0 "general_operand" "=d,d,m,d")
;;      (match_operand:DF 1 "general_operand" "d,m,d,*G"))]
;;   "TARGET_HARD_FLOAT"
;;   { 
;;       return microblaze_move_2words (operands, insn);
;;   }
;;   [(set_attr "type"  "no_delay_move,no_delay_load,no_delay_store,no_delay_move")
;;   (set_attr "mode"   "DF")
;;   (set_attr "length" "4,4,4,4")])

;; movdf_internal
;; Applies to both TARGET_SOFT_FLOAT and TARGET_HARD_FLOAT
;;
(define_insn "movdf_internal"
  [(set (match_operand:DF 0 "nonimmediate_operand" "=d,d,d,R,To")
        (match_operand:DF 1 "general_operand" "dG,R,ToF,d,d"))]
  "(register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)
       || (GET_CODE (operands[1]) == CONST_INT && INTVAL (operands[1]) == 0)
       || operands[1] == CONST0_RTX (DFmode))"
  {
        return microblaze_move_2words (operands, insn);
  }
  [(set_attr "type"     "no_delay_move,no_delay_arith,no_delay_load,no_delay_store,no_delay_store")
  (set_attr "mode"      "DF")
  (set_attr "length"    "4,4,8,4,8")])

(define_split
  [(set (match_operand:DF 0 "register_operand" "")
        (match_operand:DF 1 "register_operand" ""))]
  "reload_completed && !TARGET_DEBUG_D_MODE && !TARGET_DEBUG_G_MODE
   && GET_CODE (operands[0]) == REG && GP_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == REG && GP_REG_P (REGNO (operands[1]))
   && (REGNO(operands[0]) == (REGNO(operands[1]) + 1))"
  [(set (subreg:SI (match_dup 0) 4) (subreg:SI (match_dup 1) 4))
  (set (subreg:SI (match_dup 0) 0) (subreg:SI (match_dup 1) 0))]
  "")

(define_split
  [(set (match_operand:DF 0 "register_operand" "")
        (match_operand:DF 1 "register_operand" ""))]
  "reload_completed && !TARGET_DEBUG_D_MODE && !TARGET_DEBUG_G_MODE
   && GET_CODE (operands[0]) == REG && GP_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == REG && GP_REG_P (REGNO (operands[1]))
   && (REGNO(operands[0]) != (REGNO(operands[1]) + 1))"
  [(set (subreg:SI (match_dup 0) 0) (subreg:SI (match_dup 1) 0))
  (set (subreg:SI (match_dup 0) 4) (subreg:SI (match_dup 1) 4))]
  "")

;; Block moves, see microblaze.c for more details.
;; Argument 0 is the destination
;; Argument 1 is the source
;; Argument 2 is the length
;; Argument 3 is the alignment

(define_expand "movstrsi"
  [(parallel [(set (match_operand:BLK 0 "general_operand" "")
		   (match_operand:BLK 1 "general_operand" ""))
             (use (match_operand:SI 2 "arith32_operand" ""))
             (use (match_operand:SI 3 "immediate_operand" ""))])]
  ""
  {
    if (operands[0])		/* avoid unused code messages */
    {
        expand_block_move (operands);
        DONE;
    }
  }
)

;; Insn generated by block moves

(define_insn "movstrsi_internal"
  [(set (match_operand:BLK 0 "memory_operand" "=o") ;; destination
	(match_operand:BLK 1 "memory_operand" "o")) ;; source
  (clobber (match_scratch:SI 4 "=&d")) ;; temp 1
  (clobber (match_scratch:SI 5 "=&d")) ;; temp 2
  (clobber (match_scratch:SI 6 "=&d")) ;; temp 3
  (clobber (match_scratch:SI 7 "=&d")) ;; temp 4
  (use (match_operand:SI 2 "small_int" "I")) ;; # bytes to move
  (use (match_operand:SI 3 "small_int" "I")) ;; alignment
  (use (const_int 0))] ;; normal block move
  ""
  {
	 return output_block_move (insn, operands, 4, BLOCK_MOVE_NORMAL);
  }
  [(set_attr "type"	"no_delay_store")
  (set_attr "mode"	"none")
  (set_attr "length"	"20")])

;; Split a block move into 2 parts, the first part is everything
;; except for the last move, and the second part is just the last
;; store, which is exactly 1 instruction (ie, not a usw), so it can
;; fill a delay slot.  This also prevents a bug in delayed branches
;; from showing up, which reuses one of the registers in our clobbers.

(define_split
  [(set (mem:BLK (match_operand:SI 0 "register_operand" ""))
	(mem:BLK (match_operand:SI 1 "register_operand" "")))
  (clobber (match_operand:SI 4 "register_operand" ""))
  (clobber (match_operand:SI 5 "register_operand" ""))
  (clobber (match_operand:SI 6 "register_operand" ""))
  (clobber (match_operand:SI 7 "register_operand" ""))
  (use (match_operand:SI 2 "small_int" ""))
  (use (match_operand:SI 3 "small_int" ""))
  (use (const_int 0))]

  "reload_completed && !TARGET_DEBUG_D_MODE && INTVAL (operands[2]) > 0"

  ;; All but the last move
  [(parallel [(set (mem:BLK (match_dup 0))
		   (mem:BLK (match_dup 1)))
             (clobber (match_dup 4))
             (clobber (match_dup 5))
             (clobber (match_dup 6))
             (clobber (match_dup 7))
             (use (match_dup 2))
             (use (match_dup 3))
             (use (const_int 1))])

  ;; The last store, so it can fill a delay slot
  (parallel [(set (mem:BLK (match_dup 0))
                  (mem:BLK (match_dup 1)))
            (clobber (match_dup 4))
            (clobber (match_dup 5))
            (clobber (match_dup 6))
            (clobber (match_dup 7))
            (use (match_dup 2))
            (use (match_dup 3))
            (use (const_int 2))])]

  "")

(define_insn "movstrsi_internal2"
  [(set (match_operand:BLK 0 "memory_operand" "=o") ;; destination
	(match_operand:BLK 1 "memory_operand" "o")) ;; source
  (clobber (match_scratch:SI 4 "=&d")) ;; temp 1
  (clobber (match_scratch:SI 5 "=&d")) ;; temp 2
  (clobber (match_scratch:SI 6 "=&d")) ;; temp 3
  (clobber (match_scratch:SI 7 "=&d")) ;; temp 4
  (use (match_operand:SI 2 "small_int" "I")) ;; # bytes to move
  (use (match_operand:SI 3 "small_int" "I")) ;; alignment
  (use (const_int 1))] ;; all but last store
  ""
  {
     return output_block_move (insn, operands, 4, BLOCK_MOVE_NOT_LAST);
  } 
  [(set_attr "type"	"no_delay_store")
  (set_attr "mode"	"none")
  (set_attr "length"	"20")])


(define_insn "movstrsi_internal3"
  [(set (match_operand:BLK 0 "memory_operand" "=Ro") ;; destination
	(match_operand:BLK 1 "memory_operand" "Ro")) ;; source
  (clobber (match_scratch:SI 4 "=&d")) ;; temp 1
  (clobber (match_scratch:SI 5 "=&d")) ;; temp 2
  (clobber (match_scratch:SI 6 "=&d")) ;; temp 3
  (clobber (match_scratch:SI 7 "=&d")) ;; temp 4
  (use (match_operand:SI 2 "small_int" "I")) ;; # bytes to move
  (use (match_operand:SI 3 "small_int" "I")) ;; alignment
  (use (const_int 2))] ;; just last store of block move
  ""
  {
      return output_block_move (insn, operands, 4, BLOCK_MOVE_LAST);
  }
  [(set_attr "type"	"no_delay_store")
  (set_attr "mode"	"none")
  (set_attr "length"	"1")])




;;----------------------------------------------------------------
;; Shifts
;;----------------------------------------------------------------



;;----------------------------------------------------------------
;; 32-bit left shifts
;;----------------------------------------------------------------
(define_expand "ashlsi3"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(ashift:SI (match_operand:SI 1 "register_operand" "d")
		   (match_operand:SI 2 "arith_operand" "")))]
  ""
  {
    if (TARGET_BARREL_SHIFT) {
        emit_insn (gen_ashlsi3_bshift (operands[0],operands[1],operands[2]));
        DONE;
    }
  }
)

;; Irrespective of if we have a barrel-shifter or not, we want to match shifts by 1 with a special 
;; pattern. When a barrel shifter is present, saves a cycle. If not, allows us to annotate the
;; instruction for delay slot optimization
(define_insn "ashlsi3_byone"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(ashift:SI (match_operand:SI 1 "register_operand" "d")
                   (match_operand:SI 2 "arith_operand"    "I")))] 
  "(INTVAL(operands[2]) == 1)"
  {
    return "addk\t%0,%1,%1\;";
  }
  [(set_attr "type"	"arith")
   (set_attr "mode"	"SI")
   (set_attr "length"	"4")]
)

;; Barrel shift left
(define_insn "ashlsi3_bshift"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(ashift:SI (match_operand:SI 1 "register_operand" "d,d")
                   (match_operand:SI 2 "arith_operand"    "I,d")))]
  "TARGET_BARREL_SHIFT"
  {
    if (which_alternative == 0) {
        return "bslli\t%0,%1,%2";
    } else {                                    /* Shift amount in a register. No need to mask the lower 5 bits out. MB bshift ignores rB[0-26] */
        return "bsll\t%0,%1,%2";
    }
  }
  [(set_attr "type"	"bshift,bshift")
  (set_attr "mode"	"SI,SI")
  (set_attr "length"	"4,4")]
)

;; The following patterns apply when there is no barrel shifter present


;; Why should the backend do this? Shouldn't RTX costs help guide the 
;; base compiler to the advantage of using hard multiplies by itself?

(define_insn "ashlsi3_with_mul_delay"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(ashift:SI (match_operand:SI 1 "register_operand"  "d")
                   (match_operand:SI 2 "immediate_operand" "I")))] 
  "!TARGET_SOFT_MUL && ((1 << INTVAL (operands[2])) <= 32767 && (1 << INTVAL (operands[2])) >= -32768)"
  {
    int mul = (1 << INTVAL(operands[2]));
    char *asminsn = (char *)xmalloc (50);
    if (INTVAL (operands[2]) == 0) {        /* We might get this scenario at -O0 ! (Verify)*/
        strcpy (asminsn, "addk\t%0,%1,r0\;");
        return asminsn;
    }

    sprintf (asminsn, "muli\t%%0,%%1,%d", mul);
    return asminsn;
  }
  [(set_attr "type"	"arith")                        ;; This MUL will not generate an imm. Can go into a delay slot.
   (set_attr "mode"	"SI")
   (set_attr "length"	"4")]
)

(define_insn "ashlsi3_with_mul_nodelay"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(ashift:SI (match_operand:SI 1 "register_operand"  "d")
                   (match_operand:SI 2 "immediate_operand" "I")))] 
  "!TARGET_SOFT_MUL"
  {
    int mul = (1 << INTVAL(operands[2]));
    char *asminsn = (char *)xmalloc (50);
    sprintf (asminsn, "muli\t%%0,%%1,%d", mul);
    return asminsn;
  }
  [(set_attr "type"	"no_delay_arith")               ;; This MUL will generate an IMM. Cannot go into a delay slot
   (set_attr "mode"	"SI")
   (set_attr "length"	"8")]
)

(define_insn "ashlsi3_with_rotate"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(ashift:SI (match_operand:SI 1 "register_operand"  "d")
                   (match_operand:SI 2 "immediate_operand" "I")))] 
  "(INTVAL(operands[2]) > 17 && !optimize_size)"
  {
    unsigned int mask1;
    char *asminsn = (char *)xmalloc (400);
    int i, nshift;

    nshift = INTVAL (operands[2]);
    mask1 = (0xFFFFFFFF << nshift);
    
    /* We do one extra shift so that the first bit (carry) coming into the MSB will be masked out
     */
    strcpy (asminsn, "src\t%0,%1\;");           
    for (i = 0; i < (32 - nshift); i++) 
       strcat (asminsn, "src\t%0,%0\;");

    sprintf ((char*)(asminsn + strlen(asminsn)), "andi\t%%0,%%0,0x%x\;", mask1);
    return asminsn;
  }
  [(set_attr "type"	"multi")
  (set_attr "mode"	"SI")
  (set_attr "length"	"80")]
)

(define_insn "ashlsi3_with_size_opt"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(ashift:SI (match_operand:SI 1 "register_operand"  "d")
                   (match_operand:SI 2 "immediate_operand" "I")))] 
  "(INTVAL(operands[2]) > 5 && optimize_size)"
  {
    char *asminsn = (char *)xmalloc (200);
    strcpy (asminsn, "%_ori\tr18,r0,%2\;");
    if (REGNO (operands[0]) != REGNO (operands[1]))
        strcat (asminsn, "addk\t%0,%1,r0\n");
    else 
        strcat (asminsn, "\n");
    strcat (asminsn, "%-ashl_loop:\n\taddik\tr18,r18,-1\;bneid\tr18,%-ashl_loop\;addk\t%0,%0,%0\;");  
    return asminsn;
  }
  [(set_attr "type"	"multi")
   (set_attr "mode"	"SI")
   (set_attr "length"	"40")]
)

(define_insn "ashlsi3_serial"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(ashift:SI (match_operand:SI 1 "register_operand" "d,d")
                   (match_operand:SI 2 "arith_operand"    "I,d")))] 
  ""
  {
    if (which_alternative == 0) { 
        int i;
        char *asminsn = xmalloc (25 * 32);
        if (INTVAL (operands[2]) == 0) {        /* We are getting this scenario at -O0 ! */
            if (REGNO (operands[0]) != REGNO (operands[1]))
                strcpy (asminsn, "addk\t%0,%1,r0\;");
            else
                strcpy (asminsn, "\n");
            return asminsn;
        }
        strcpy (asminsn, "addk\t%0,%1,%1\;");
        for (i = 0; i < INTVAL (operands[2]) - 1; i++) 
            strcat (asminsn, "addk\t%0,%0,%0\;");
        return asminsn;       
    } else {                                            /* Shift amount in a register */
        return "%_andi\tr18,%2,31\;beqid\tr18,%-ashl_out\;addk\t%0,%1,r0\;%-ashl_loop:\n\taddik\tr18,r18,-1\;bneid\tr18,%-ashl_loop\;addk\t%0,%0,%0\n%-ashl_out:\n";  
    }
  }
  [(set_attr "type"	"multi,multi")
   (set_attr "mode"	"SI,SI")
   (set_attr "length"	"124,40")]
)

;;----------------------------------------------------------------
;; 32-bit right shifts
;;----------------------------------------------------------------
(define_expand "ashrsi3"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(ashiftrt:SI (match_operand:SI 1 "register_operand" "d")
                     (match_operand:SI 2 "arith_operand" "")))]
  ""
  {
    if (TARGET_BARREL_SHIFT) {
        emit_insn (gen_ashrsi3_bshift (operands[0],operands[1],operands[2]));
        DONE;
    }
  }
)

;; Irrespective of if we have a barrel-shifter or not, we want to match shifts by 1 with a special 
;; pattern. When a barrel shifter is present, saves a cycle. If not, allows us to annotate the
;; instruction for delay slot optimization
(define_insn "ashrsi3_byone"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(ashiftrt:SI (match_operand:SI 1 "register_operand" "d")
                     (match_operand:SI 2 "arith_operand"    "I")))] 
  "(INTVAL(operands[2]) == 1)"
  {
    return "sra\t%0,%1\;";
  }
  [(set_attr "type"	"arith")
   (set_attr "mode"	"SI")
   (set_attr "length"	"4")]
)


;; Barrel shift right logical
(define_insn "ashrsi3_bshift"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(ashiftrt:SI (match_operand:SI 1 "register_operand" "d,d")
                     (match_operand:SI 2 "arith_operand"    "I,d")))]
  "TARGET_BARREL_SHIFT"
  {
    if (which_alternative == 0) { 
       return "bsrai\t%0,%1,%2";
    } else {                                    /* Shift amount in a register. No need to mask the lower 5 bits out. MB bshift ignores rB[0-26] */
       return "bsra\t%0,%1,%2";
    }
  }
  [(set_attr "type"	"bshift,bshift")
  (set_attr "mode"	"SI,SI")
  (set_attr "length"	"4,4")]
)

;; The following patterns apply when there is no barrel shifter present
(define_insn "ashrsi3_with_size_opt"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(ashiftrt:SI (match_operand:SI 1 "register_operand"  "d")
                     (match_operand:SI 2 "immediate_operand" "I")))] 
  "(INTVAL(operands[2]) > 5 && optimize_size)"
  {
    char *asminsn = (char *)xmalloc (200);
    strcpy (asminsn, "%_ori\tr18,r0,%2\;");

    if (REGNO (operands[0]) != REGNO (operands[1]))
        strcat (asminsn, "addk\t%0,%1,r0\n");   
    else
        strcat (asminsn, "\n");

    strcat (asminsn, "%-ashr_loop:\n\taddik\tr18,r18,-1\;bneid\tr18,%-ashr_loop\;sra\t%0,%0\;");  
    return asminsn;
  }
  [(set_attr "type"	"multi")
   (set_attr "mode"	"SI")
   (set_attr "length"	"40")]
)

(define_insn "ashrsi3_serial"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(ashiftrt:SI (match_operand:SI 1 "register_operand" "d,d")
                     (match_operand:SI 2 "arith_operand"    "I,d")))] 
  ""
  {
    if (which_alternative == 0) { 
        int i;
        char *asminsn = (char *)xmalloc (25 * 32);
        if (INTVAL (operands[2]) == 0) {        /* We might get this scenario at -O0 ! (Verify) */
            if (REGNO (operands[0]) != REGNO (operands[1]))
                strcpy (asminsn, "addk\t%0,%1,r0\;");
            else
                strcpy (asminsn, "\n");
            return asminsn;
        }

        strcpy (asminsn, "sra\t%0,%1\;");
        for (i = 0; i < INTVAL (operands[2]) - 1; i++) 
            strcat (asminsn, "sra\t%0,%0\;");
        return asminsn;       
    } else {                                            /* Shift amount in a register */
        return "%_andi\tr18,%2,31\;beqid\tr18,%-ashr_out\;addk\t%0,%1,r0\;%-ashr_loop:\n\taddik\tr18,r18,-1\;bneid\tr18,%-ashr_loop\;sra\t%0,%0\n%-ashr_out:\n";  
    }
  }
  [(set_attr "type"	"multi,multi")
   (set_attr "mode"	"SI,SI")
   (set_attr "length"	"124,40")]
)

;;----------------------------------------------------------------
;; 32-bit right shifts (logical)
;;----------------------------------------------------------------

(define_expand "lshrsi3"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(lshiftrt:SI (match_operand:SI 1 "register_operand" "d")
                     (match_operand:SI 2 "arith_operand" "")))]
  ""
  {
    if (TARGET_BARREL_SHIFT) {
        emit_insn (gen_lshrsi3_bshift (operands[0],operands[1],operands[2]));
        DONE;
    }
  }
)

;; Irrespective of if we have a barrel-shifter or not, we want to match shifts by 1 with a special 
;; pattern. When a barrel shifter is present, saves a cycle. If not, allows us to annotate the
;; instruction for delay slot optimization
(define_insn "lshrsi3_byone"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(lshiftrt:SI (match_operand:SI 1 "register_operand" "d")
                     (match_operand:SI 2 "arith_operand"    "I")))] 
  "(INTVAL(operands[2]) == 1)"
  {
    return "srl\t%0,%1\;";
  }
  [(set_attr "type"	"arith")
   (set_attr "mode"	"SI")
   (set_attr "length"	"4")]
)


;; Barrel shift right logical
(define_insn "lshrsi3_bshift"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(lshiftrt:SI (match_operand:SI 1 "register_operand" "d,d")
                     (match_operand:SI 2 "arith_operand"    "I,d")))]
  "TARGET_BARREL_SHIFT"
  {
    if (which_alternative == 0) { 
       return "bsrli\t%0,%1,%2";
    } else {                                    /* Shift amount in a register. No need to mask the lower 5 bits out. MB bshift ignores rB[0-26] */
       return "bsrl\t%0,%1,%2";
    }
  }
  [(set_attr "type"	"bshift,bshift")
  (set_attr "mode"	"SI,SI")
  (set_attr "length"	"4,4")]
)

;; The following patterns apply when there is no barrel shifter present
(define_insn "lshrsi3_with_size_opt"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(lshiftrt:SI (match_operand:SI 1 "register_operand"  "d")
                     (match_operand:SI 2 "immediate_operand" "I")))] 
  "(INTVAL(operands[2]) > 5 && optimize_size)"
  {
    char *asminsn = (char *)xmalloc (200);
    strcpy (asminsn, "%_ori\tr18,r0,%2\;");

    if (REGNO (operands[0]) != REGNO (operands[1]))
        strcat (asminsn, "addk\t%0,%1,r0\n");   
    else
        strcat (asminsn, "\n");

    strcat (asminsn, "%-lshr_loop:\n\taddik\tr18,r18,-1\;bneid\tr18,%-lshr_loop\;srl\t%0,%0\;");  
    return asminsn;
  }
  [(set_attr "type"	"multi")
   (set_attr "mode"	"SI")
   (set_attr "length"	"40")]
)

(define_insn "lshrsi3_serial"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(lshiftrt:SI (match_operand:SI 1 "register_operand" "d,d")
                     (match_operand:SI 2 "arith_operand"    "I,d")))] 
  ""
  {
    if (which_alternative == 0) { 
        int i;
        char *asminsn = (char *)xmalloc (25 * 32);
        if (INTVAL (operands[2]) == 0) {        /* We might get this scenario at -O0 ! (Verify)*/
        if (REGNO (operands[0]) != REGNO (operands[1]))
            strcpy (asminsn, "addk\t%0,%1,r0\;");
        else
            strcpy (asminsn, "\n");
        return asminsn;
    }

        strcpy (asminsn, "srl\t%0,%1\;");
        for (i = 0; i < INTVAL (operands[2]) - 1; i++) 
            strcat (asminsn, "srl\t%0,%0\;");
        return asminsn;       
    } else {                                            /* Shift amount in a register */
        return "%_andi\tr18,%2,31\;beqid\tr18,%-lshr_out\;addk\t%0,%1,r0\;%-lshr_loop:\n\taddik\tr18,r18,-1\;bneid\tr18,%-lshr_loop\;srl\t%0,%0\n%-lshr_out:\n";  
    }
  }
  [(set_attr "type"	"multi,multi")
   (set_attr "mode"	"SI,SI")
   (set_attr "length"	"124,40")]
)


;;----------------------------------------------------------------
;; Comparisons
;;----------------------------------------------------------------
;; Flow here is rather complex:
;;
;;  1)	The cmp{si,di,sf,df} routine is called.  It deposits the
;;	arguments into the branch_cmp array, and the type into
;;	branch_type.  No RTL is generated.
;;
;;  2)	The appropriate branch define_expand is called, which then
;;	creates the appropriate RTL for the comparison and branch.
;;	Different CC modes are used, based on what type of branch is
;;	done, so that we can constrain things appropriately.  There
;;	are assumptions in the rest of GCC that break if we fold the
;;	operands into the branchs for integer operations, and use cc0
;;	for floating point, so we use the fp status register instead.
;;	If needed, an appropriate temporary is created to hold the
;;	of the integer compare.


(define_expand "cmpsi"
  [(set (cc0)
	(compare:CC (match_operand:SI 0 "register_operand" "")
		    (match_operand:SI 1 "arith_operand" "")))]
  ""
  {
    if (operands[0])		/* avoid unused code message */
    {
        branch_cmp[0] = operands[0];
        branch_cmp[1] = operands[1];
        branch_type = CMP_SI;
        DONE;
    }
  }
)


(define_expand "tstsi"
  [(set (cc0)
	(match_operand:SI 0 "register_operand" ""))]
  ""
  {
    if (operands[0])		/* avoid unused code message */
    {
        /*	fprintf(stderr,"In tstsi \n");*/
        branch_cmp[0] = operands[0];
        branch_cmp[1] = const0_rtx;
        branch_type = CMP_SI;
        DONE;
    }
  }
)


(define_expand "cmpsf"
  [(set (cc0)
        (compare:CC (match_operand:SF 0 "register_operand" "")
                    (match_operand:SF 1 "register_operand" "")))]
  "TARGET_HARD_FLOAT"
  {
    if (operands[0])              /* avoid unused code message */
    {
      branch_cmp[0] = operands[0];
      branch_cmp[1] = operands[1];
      branch_type = CMP_SF;
      DONE;
    }
  }
)

;;----------------------------------------------------------------
;; Setting a register from an integer point comparison. 
;;----------------------------------------------------------------
(define_expand "seq"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(eq:SI (match_dup 1)
	       (match_dup 2)))]
  ""
  {
    /* Setup operands */
    operands[1] = branch_cmp[0];
    operands[2] = branch_cmp[1];

    /* Find some other way for soft floating point */
    if ((GET_MODE (operands[1]) == SFmode || GET_MODE (operands[2]) == SFmode) && !TARGET_HARD_FLOAT)
        FAIL;

    if (!TARGET_PATTERN_COMPARE)
        FAIL;

    if (GET_CODE (operands[2]) == CONST_INT) {
        if (INTVAL (operands[2]) == 0)
            operands[2] = gen_rtx_REG (SImode, 0);
        else
            operands[2] = force_reg (SImode, operands[2]);
    } 

    /* Fall through */
  }
)

(define_expand "sne"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(ne:SI (match_dup 1)
	       (match_dup 2)))]
  ""
  {
    /* Setup operands */
    operands[1] = branch_cmp[0];
    operands[2] = branch_cmp[1];

    /* Find some other way for soft floating point */
    if ((GET_MODE (operands[1]) == SFmode || GET_MODE (operands[2]) == SFmode) && !TARGET_HARD_FLOAT)
        FAIL;

    if (!TARGET_PATTERN_COMPARE)
        FAIL;

    if (GET_CODE (operands[2]) == CONST_INT) {
        if (INTVAL (operands[2]) == 0)
            operands[2] = gen_rtx_REG (SImode, 0);
        else
            operands[2] = force_reg (SImode, operands[2]);
    }

    /* Fall through */
  }
)

(define_insn "seq_internal" 
  [(set (match_operand:SI 0 "register_operand" "=d")
        (eq:SI (match_operand:SI 1 "register_operand" "d")
               (match_operand:SI 2 "register_operand" "d")))
  ]
  "TARGET_PATTERN_COMPARE"
  {
    return "pcmpeq\t%0,%1,%2";
  }      
  [(set_attr "type"	"arith")
   (set_attr "mode"	"SI")
   (set_attr "length"	"4")]
)              

(define_insn "sne_internal" 
  [(set (match_operand:SI 0 "register_operand" "=d")
        (ne:SI (match_operand:SI 1 "register_operand" "d")
               (match_operand:SI 2 "register_operand" "d")))
  ]
  "TARGET_PATTERN_COMPARE"
  {
    return "pcmpne\t%0,%1,%2";
  }          
  [(set_attr "type"	"arith")
  (set_attr "mode"	"SI")
  (set_attr "length"	"4")]
)              

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=d,d")
        (eq:SI (match_operand:SI 1 "register_operand" "%d,d")
               (match_operand:SI 2 "arith_operand" "d,i")))
  (clobber (match_scratch:SI 3 "=d,d"))]
  "!(TARGET_PATTERN_COMPARE)"
  "@
    xor\t%0,%1,%2\;rsubi\t%3,%0,0\;addc\t%0,%3,%0
    xori\t%0,%1,%2\;rsubi\t%3,%0,0\;addc\t%0,%3,%0"
  [(set_attr "length"   "12,12")
   (set_attr "type"     "multi, multi")
   (set_attr "mode"     "SI,SI")]
)


;;----------------------------------------------------------------
;; Setting a register from a floating point comparison. 
;; These s<cond> patterns are for hardware floating point 
;; compares only
;;----------------------------------------------------------------
(define_expand "slt"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (lt:SI (match_dup 1)
               (match_dup 2)))]
  "TARGET_HARD_FLOAT"
  {               
    /* Setup operands */
    operands[1] = branch_cmp[0];
    operands[2] = branch_cmp[1];

    /* Only for floating point compares */
    if ((GET_MODE (operands[1]) != SFmode) || (GET_MODE (operands[2]) != SFmode))
        FAIL;

    if (GET_CODE (operands[2]) == CONST_DOUBLE)
        force_reg (SFmode, operands[2]);

    /* Fall through */
  }
)

(define_expand "sle"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (le:SI (match_dup 1)
               (match_dup 2)))]
  "TARGET_HARD_FLOAT"
  {               
    /* Setup operands */
    operands[1] = branch_cmp[0];
    operands[2] = branch_cmp[1];

    /* Only for floating point compares */
    if ((GET_MODE (operands[1]) != SFmode) || (GET_MODE (operands[2]) != SFmode))
        FAIL;

    if (GET_CODE (operands[2]) == CONST_DOUBLE)
        force_reg (SFmode, operands[2]);

    /* Fall through */
  }
)

(define_expand "sgt"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (gt:SI (match_dup 1)
               (match_dup 2)))]
  "TARGET_HARD_FLOAT"
  {               
    /* Setup operands */
    operands[1] = branch_cmp[0];
    operands[2] = branch_cmp[1];

    /* Only for floating point compares */
    if ((GET_MODE (operands[1]) != SFmode) || (GET_MODE (operands[2]) != SFmode))
        FAIL;

    if (GET_CODE (operands[2]) == CONST_DOUBLE)
        force_reg (SFmode, operands[2]);

    /* Fall through */
  }
)

(define_expand "sge"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (ge:SI (match_dup 1)
               (match_dup 2)))]
  "TARGET_HARD_FLOAT"
  {               
    /* Setup operands */
    operands[1] = branch_cmp[0];
    operands[2] = branch_cmp[1];

    /* Only for floating point compares */
    if ((GET_MODE (operands[1]) != SFmode) || (GET_MODE (operands[2]) != SFmode))
        FAIL;

    if (GET_CODE (operands[2]) == CONST_DOUBLE)
        force_reg (SFmode, operands[2]);

    /* Fall through */
  }
)

(define_insn "seq_sf"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (eq:SI (match_operand:SF 1 "register_operand" "d")
               (match_operand:SF 2 "register_operand" "d")))]
  "TARGET_HARD_FLOAT"
  {
        return "fcmp.eq\t%0,%2,%1";
  }
  [(set_attr "type"     "fcmp")
   (set_attr "mode"      "SF")
   (set_attr "length"    "4")]
)

(define_insn "sne_sf"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (ne:SI (match_operand:SF 1 "register_operand" "d")
               (match_operand:SF 2 "register_operand" "d")))]
  "TARGET_HARD_FLOAT"
  {
        return "fcmp.ne\t%0,%2,%1";
  }
  [(set_attr "type"     "fcmp")
   (set_attr "mode"      "SF")
   (set_attr "length"    "4")]
)

(define_insn "slt_sf"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (lt:SI (match_operand:SF 1 "register_operand" "d")
               (match_operand:SF 2 "register_operand" "d")))]
  "TARGET_HARD_FLOAT"
  {
        return "fcmp.lt\t%0,%2,%1";
  }
  [(set_attr "type"     "fcmp")
   (set_attr "mode"      "SF")
   (set_attr "length"    "4")]
)

(define_insn "sle_sf"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (le:SI (match_operand:SF 1 "register_operand" "d")
               (match_operand:SF 2 "register_operand" "d")))]
  "TARGET_HARD_FLOAT"
  {
        return "fcmp.le\t%0,%2,%1";
  }
  [(set_attr "type"     "fcmp")
   (set_attr "mode"      "SF")
   (set_attr "length"    "4")]
)

(define_insn "sgt_sf"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (gt:SI (match_operand:SF 1 "register_operand" "d")
               (match_operand:SF 2 "register_operand" "d")))]
  "TARGET_HARD_FLOAT"
  {
        return "fcmp.gt\t%0,%2,%1";
  }
  [(set_attr "type"     "fcmp")
  (set_attr "mode"      "SF")
  (set_attr "length"    "4")])

(define_insn "sge_sf"
  [(set (match_operand:SI 0 "register_operand" "=d")
        (ge:SI (match_operand:SF 1 "register_operand" "d")
               (match_operand:SF 2 "register_operand" "d")))]
  "TARGET_HARD_FLOAT"
  {
        return "fcmp.ge\t%0,%2,%1";
  }
  [(set_attr "type"     "fcmp")
   (set_attr "mode"      "SF")
   (set_attr "length"    "4")]
)

;;----------------------------------------------------------------
;; Conditional branches
;;----------------------------------------------------------------

(define_expand "beq"
  [(set (pc)
	(if_then_else (eq:CC (cc0)
			     (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    gen_conditional_branch (operands, EQ);
    DONE;
  }
)

(define_expand "bne"
  [(set (pc)
	(if_then_else (ne:CC (cc0)
			     (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    gen_conditional_branch (operands, NE);
    DONE;
  }
)

(define_expand "bgt"
  [(set (pc)
	(if_then_else (gt:CC (cc0)
			     (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    gen_conditional_branch (operands, GT);
    DONE;
  }
)

(define_expand "bge"
  [(set (pc)
	(if_then_else (ge:CC (cc0)
			     (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    gen_conditional_branch (operands, GE);
    DONE;
  }
)

(define_expand "blt"
  [(set (pc)
	(if_then_else (lt:CC (cc0)
			     (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    gen_conditional_branch (operands, LT);
    DONE;
  }
)

(define_expand "ble"
  [(set (pc)
	(if_then_else (le:CC (cc0)
			     (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    gen_conditional_branch (operands, LE);
    DONE;
  }
)

(define_expand "bgtu"
  [(set (pc)
	(if_then_else (gtu:CC (cc0)
			      (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    gen_conditional_branch (operands, GTU);
    DONE;
  }
)

(define_expand "bgeu"
  [(set (pc)
	(if_then_else (geu:CC (cc0)
			      (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    gen_conditional_branch (operands, GEU);
    DONE;
  }
)


(define_expand "bltu"
  [(set (pc)
	(if_then_else (ltu:CC (cc0)
			      (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    gen_conditional_branch (operands, LTU);
    DONE;
  }
)

(define_expand "bleu"
  [(set (pc)
	(if_then_else (leu:CC (cc0)
			      (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    gen_conditional_branch (operands, LEU);
    DONE;
  }
)

(define_insn "branch_zero"
  [(set (pc)
	(if_then_else (match_operator:SI 0 "cmp_op"
  					 [(match_operand:SI 1 "register_operand" "d")
                                          (const_int 0)])
                      (match_operand:SI 2 "pc_or_label_operand" "")
                      (match_operand:SI 3 "pc_or_label_operand" "")))
  ]
  ""
  {
    if (operands[2] != pc_rtx)           /* normal jump */
    {				
        switch (GET_CODE (operands[0]))
	{
            case EQ:  return "beqi%?\t%z1,%2";
	    case NE:  return "bnei%?\t%z1,%2";
            case GT:  return "bgti%?\t%z1,%2";
	    case LE:  return "blei%?\t%z1,%2";
	    case GE:  return "bgei%?\t%z1,%2";
	    case LT:  return "blti%?\t%z1,%2";
	    case GTU: return "bnei%?\t%z1,%2";
	    case LEU: return "beqi%?\t%z1,%2";
	    case GEU: return "bri%?\t%2";
	    case LTU: return "# Always false LTU comparison";
	    default:
	       break;
        }
        return "b%C0i%?\t%z1,%2";
    } else {                             /* inverted jump */

        switch (GET_CODE (operands[0]))
        {
            case EQ:  return "bnei%?\t%z1,%3";
	    case NE:  return "beqi%?\t%z1,%3";
	    case GT:  return "blei%?\t%z1,%3";
	    case LE:  return "bgti%?\t%z1,%3";
	    case GE:  return "blti%?\t%z1,%3";
	    case LT:  return "bgei%?\t%z1,%3";
	    case GTU: return "beqi%?\t%z1,%3";
	    case LEU: return "bnei%?\t%z1,%3";
	    case GEU: return "# Always false GEU comparison";
	    case LTU: return "bri%?\t%3";
	    default:
	       break;
        }

        return "b%N0i%?\t%z1,%3";
    }
  }
  [(set_attr "type"	"branch")
   (set_attr "mode"	"none")
   (set_attr "length"	"4")]
)



(define_insn "branch_compare_imm_uns"
  [(set (pc)
	(if_then_else (match_operator:SI 0 "unsigned_cmp_op"
					 [(match_operand:SI 1 "register_operand" "d")
                                          (match_operand:SI 2 "immediate_operand" "i")])
                      (match_operand:SI 3 "pc_or_label_operand" "")
                      (match_operand:SI 4 "pc_or_label_operand" "")))
  (clobber(reg:SI R_TMP))]
  ""
  {
    if (operands[3] != pc_rtx)                                  /* normal jump */
    {                               
        switch (GET_CODE (operands[0]))
        {
            case GTU: return  "addi\tr18,r0,%2\;cmpu\tr18,%z1,r18\;blti%?\tr18,%3";
            case LEU: return  "addi\tr18,r0,%2\;cmpu\tr18,%z1,r18\;bgei%?\tr18,%3";
            case GEU: return  "addi\tr18,r0,%2\;cmpu\tr18,r18,%z1\;bgei%?\tr18,%3";
            case LTU: return  "addi\tr18,r0,%2\;cmpu\tr18,r18,%z1\;blti%?\tr18,%3";
            default:
                break;
        }

        fatal_insn ("branch_compare_imm_uns: ", operands[0]);
   }
   else {                                                       /* inverted jump */
        switch (GET_CODE (operands[0]))
        {
            case GTU: return "addi\tr18,r0,%2\;cmpu\tr18,%z1,r18\;bgei%?\tr18,%3";
            case LEU: return "addi\tr18,r0,%2\;cmpu\tr18,%z1,r18\;blti%?\tr18,%3";
            case GEU: return "addi\tr18,r0,%2\;cmpu\tr18,r18,%z1\;blti%?\tr18,%3";
            case LTU: return "addi\tr18,r0,%2\;cmpu\tr18,r18,%z1\;bgei%?\tr18,%3";
            default:
                break;
        }

        fatal_insn ("branch_compare_imm_uns: ", operands[0]);
    }
  }
  [(set_attr "type"	"branch")   ;; Bit of a hack here. It's a multi rather than a branch, yet, we want delay slot optimization to occur on this.
   (set_attr "mode"	"none")
   (set_attr "length"	"12")]
)


(define_insn "branch_compare_imm"
  [(set (pc)
	(if_then_else (match_operator:SI 0 "signed_cmp_op"
					 [(match_operand:SI 1 "register_operand" "d")
                                          (match_operand:SI 2 "immediate_operand" "i")])
                      (match_operand:SI 3 "pc_or_label_operand" "")
                      (match_operand:SI 4 "pc_or_label_operand" "")))
  (clobber(reg:SI R_TMP))]
  ""
  {
    if (operands[3] != pc_rtx) {                                /* normal jump */
        switch (GET_CODE (operands[0]))
        {
            case GT: return  "addik\tr18,r0,%2\;cmp\tr18,%z1,r18\;blti%?\tr18,%3" ;
            case LE: return  "addik\tr18,r0,%2\;cmp\tr18,%z1,r18\;bgei%?\tr18,%3" ;
            case GE: return  "addik\tr18,r0,%2\;cmp\tr18,r18,%z1\;bgei%?\tr18,%3" ;
            case LT: return  "addik\tr18,r0,%2\;cmp\tr18,r18,%z1\;blti%?\tr18,%3" ;
            default:
                break;
        }
        if ((GET_CODE(operands[0]) != NE) && (GET_CODE(operands[0]) != EQ)) 
             fatal_insn ("branch_compare_imm: ", operands[0]);

        return  "xori\tr18,%z1,%2\;b%C0i%?\tr18,%3";            /* Handle NE, EQ */
    }
    else {                                                      /* inverted jump */
        switch (GET_CODE (operands[0]))
        {
            case GT: return  "addik\tr18,r0,%2\;cmp\tr18,%z1,r18\;bgei%?\tr18,%4";
            case LE: return  "addik\tr18,r0,%2\;cmp\tr18,%z1,r18\;blti%?\tr18,%4";
            case GE: return  "addik\tr18,r0,%2\;cmp\tr18,r18,%z1\;blti%?\tr18,%4";
            case LT: return  "addik\tr18,r0,%2\;cmp\tr18,r18,%z1\;bgei%?\tr18,%4";
            default:
                break;
        }
        if ((GET_CODE(operands[0]) != NE) && (GET_CODE(operands[0]) != EQ))
             fatal_insn ("branch_compare_imm: ", operands[0]);

        return  "xori\tr18,%z1,%2\;b%N0i%?\tr18,%4";            /* Handle NE, EQ */
    }
  }
  [(set_attr "type"	"branch")   ;; Bit of a hack here. It's a multi rather than a branch, yet, we want delay slot optimization to occur on this.
   (set_attr "mode"	"none")
   (set_attr "length"	"12")]
)


(define_insn "branch_compare"
  [(set (pc)
        (if_then_else (match_operator:SI 0 "cmp_op"
                                         [(match_operand:SI 1 "register_operand" "d")
                                          (match_operand:SI 2 "register_operand" "d")
                                         ])
                      (match_operand:SI 3 "pc_or_label_operand" "")
                      (match_operand:SI 4 "pc_or_label_operand" "")))
  (clobber(reg:SI R_TMP))]
  ""
  {

    if (operands[3] != pc_rtx)
    {                                                           /* normal jump */
        switch (GET_CODE (operands[0]))
        {

            case GT: return  "cmp\tr18,%z1,%z2\;blti%?\tr18,%3";
            case LE: return  "cmp\tr18,%z1,%z2\;bgei%?\tr18,%3";
            case GE: return  "cmp\tr18,%z2,%z1\;bgei%?\tr18,%3";
            case LT: return  "cmp\tr18,%z2,%z1\;blti%?\tr18,%3";
            case GTU:return  "cmpu\tr18,%z1,%z2\;blti%?\tr18,%3";
            case LEU:return  "cmpu\tr18,%z1,%z2\;bgei%?\tr18,%3";
            case GEU:return  "cmpu\tr18,%z2,%z1\;bgei%?\tr18,%3";
            case LTU:return  "cmpu\tr18,%z2,%z1\;blti%?\tr18,%3";
            default:
                break;
        }
        return  "rsubk\tr18,%z2,%z1\;b%C0i%?\tr18,%3";
    }
    else
    {                                                           /* inverted jump */
        switch (GET_CODE (operands[0]))
        {
            case GT: return  "cmp\tr18,%z1,%z2\;bgei%?\tr18,%3" ;
            case LE: return  "cmp\tr18,%z1,%z2\;blti%?\tr18,%3" ;
            case GE: return  "cmp\tr18,%z2,%z1\;blti%?\tr18,%3" ;
            case LT: return  "cmp\tr18,%z2,%z1\;bgei%?\tr18,%3" ;
            case GTU:return  "cmpu\tr18,%z1,%z2\;bgei%?\tr18,%3";
            case LEU:return  "cmpu\tr18,%z1,%z2\;blti%?\tr18,%3";
            case GEU:return  "cmpu\tr18,%z2,%z1\;blti%?\tr18,%3";
            case LTU:return  "cmpu\tr18,%z2,%z1\;bgei%?\tr18,%3";
            default:
                break;
        }

        return  "cmp\tr18,%z1,%z2\;b%N0i%?\tr18,%4 ";
    }
  }
  [(set_attr "type"	"branch")
   (set_attr "mode"	"none")
   (set_attr "length"	"12")]
)

;;----------------------------------------------------------------
;; Unconditional branches
;;----------------------------------------------------------------
(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  ""
  {
    if (GET_CODE (operands[0]) == REG)
        return "br%?\t%0";
    else	
        return "bri%?\t%l0";
  }
  [(set_attr "type"	"jump")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])

(define_expand "indirect_jump"
  [(set (pc) (match_operand 0 "register_operand" "d"))]
  ""
  {
    rtx dest;

    if (operands[0])		/* eliminate unused code warnings */
    {
        dest = operands[0];
        if (GET_CODE (dest) != REG || GET_MODE (dest) != Pmode)
            operands[0] = copy_to_mode_reg (Pmode, dest);

        if (!(Pmode == DImode))
            emit_jump_insn (gen_indirect_jump_internal1 (operands[0]));
        else
            emit_jump_insn (gen_indirect_jump_internal2 (operands[0]));
        DONE;
    }
  }
)

;; Indirect jumps. Jump to register values. Assuming absolute jumps

(define_insn "indirect_jump_internal1"
  [(set (pc) (match_operand:SI 0 "register_operand" "d"))]
  "!(Pmode == DImode)"
  "bra%?\t%0"
  [(set_attr "type"	"jump")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])

(define_insn "indirect_jump_internal2"
  [(set (pc) (match_operand:DI 0 "register_operand" "d"))]
  "Pmode == DImode"
  "bra%?\t%0"
  [(set_attr "type"	"jump")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])

(define_expand "tablejump"
  [(set (pc)
	(match_operand 0 "register_operand" "d"))
  (use (label_ref (match_operand 1 "" "")))]
  ""
  {
    if (operands[0])		/* eliminate unused code warnings */
    {
        if (GET_MODE (operands[0]) != Pmode)
            abort ();

        if (! flag_pic)
	{
            if (!(Pmode == DImode))
                emit_jump_insn (gen_tablejump_internal1 (operands[0], operands[1]));
            else
                emit_jump_insn (gen_tablejump_internal2 (operands[0], operands[1]));
	}
        else
	{
            if (!(Pmode == DImode))
                emit_jump_insn (gen_tablejump_internal3 (operands[0], operands[1]));
            else
                emit_jump_insn (gen_tablejump_internal4 (operands[0], operands[1]));
	}
        DONE;
    }
  }
)

(define_insn "tablejump_internal1"
  [(set (pc)
	(match_operand:SI 0 "register_operand" "d"))
  (use (label_ref (match_operand 1 "" "")))]
  "!(Pmode == DImode)"
  "bra%?\t%0 "
  [(set_attr "type"	"jump")
  (set_attr "mode"	"none")
  (set_attr "length"	"1")])

(define_insn "tablejump_internal2"
  [(set (pc)
	(match_operand:DI 0 "register_operand" "d"))
  (use (label_ref (match_operand 1 "" "")))]
  "Pmode == DImode"
  "bra%?\t%0"
  [(set_attr "type"	"jump")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])

(define_expand "tablejump_internal3"
  [(parallel [(set (pc)
		   (plus:SI (match_operand:SI 0 "register_operand" "d")
			    (label_ref:SI (match_operand:SI 1 "" ""))))
             (use (label_ref:SI (match_dup 1)))])]
  ""
  ""
)

;; need to change for MicroBlaze PIC
(define_insn ""
 [(set (pc)
	(plus:SI (match_operand:SI 0 "register_operand" "d")
		 (label_ref:SI (match_operand 1 "" ""))))
  (use (label_ref:SI (match_dup 1)))]
 "!(Pmode == DImode) && next_active_insn (insn) != 0
  && GET_CODE (PATTERN (next_active_insn (insn))) == ADDR_DIFF_VEC
  && PREV_INSN (next_active_insn (insn)) == operands[1]
  && flag_pic"
  {
    /* .cpadd expands to add REG,REG,$gp when pic, and nothing when not pic.  
     */
    output_asm_insn ("addk\t%0,%0,r20",operands);
    return "bra%?\t%0";
    /*  output_asm_insn (".cpadd\t%0", operands);
        return "MICROBLAZEbri%?11\t%0";
     */
}
 [(set_attr "type"	"jump")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])

(define_expand "tablejump_internal4"
  [(parallel [(set (pc)
		   (plus:DI (match_operand:DI 0 "register_operand" "d")
			    (label_ref:DI (match_operand:SI 1 "" ""))))
             (use (label_ref:DI (match_dup 1)))])]
  ""
  ""
)

;;; Make sure that this only matches the insn before ADDR_DIFF_VEC.  Otherwise
;;; it is not valid.  ??? With the USE, the condition tests may not be required
;;; any longer.

(define_insn ""
  [(set (pc)
	(plus:DI (match_operand:DI 0 "register_operand" "d")
		 (label_ref:DI (match_operand 1 "" ""))))
  (use (label_ref:DI (match_dup 1)))]
  "Pmode == DImode && next_active_insn (insn) != 0
   && GET_CODE (PATTERN (next_active_insn (insn))) == ADDR_DIFF_VEC
   && PREV_INSN (next_active_insn (insn)) == operands[1]"
  "MICROBLAZEbri%?12\t%0"
  [(set_attr "type"	"jump")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])


(define_expand "builtin_setjmp_setup"
  [(unspec [(match_operand 0 "register_operand" "r")] 20)]
  "TARGET_ABICALLS"
  {
    if (Pmode == DImode)
        emit_insn (gen_builtin_setjmp_setup_64 (operands[0]));
    else
        emit_insn (gen_builtin_setjmp_setup_32 (operands[0]));
    DONE;
  }
)

(define_expand "builtin_setjmp_setup_32"
  [(set (mem:SI (plus:SI (match_operand:SI 0 "register_operand" "r")
                         (const_int 12)))
        (reg:SI 28))]
  "TARGET_ABICALLS && ! (Pmode == DImode)"
  ""
)

(define_expand "builtin_setjmp_setup_64"
  [(set (mem:DI (plus:DI (match_operand:DI 0 "register_operand" "r")
                         (const_int 24)))
        (reg:DI 28))]
  "TARGET_ABICALLS && Pmode == DImode"
  ""
)

;; For o32/n32/n64, we need to arrange for longjmp to put the 
;; target address in t9 so that we can use it for loading $gp.

(define_expand "builtin_longjmp"
  [(unspec_volatile [(match_operand 0 "register_operand" "r")] 3)]
  "TARGET_ABICALLS"
  {
    /* The elements of the buffer are, in order:  */
    int W = (Pmode == DImode ? 8 : 4);
    rtx fp = gen_rtx_MEM (Pmode, operands[0]);
    rtx lab = gen_rtx_MEM (Pmode, plus_constant (operands[0], 1*W));
    rtx stack = gen_rtx_MEM (Pmode, plus_constant (operands[0], 2*W));
    rtx gpv = gen_rtx_MEM (Pmode, plus_constant (operands[0], 3*W));
    rtx pv = gen_rtx_REG (Pmode, 25);
    rtx gp = gen_rtx_REG (Pmode, 28);
    
    /* This bit is the same as expand_builtin_longjmp.  */
    emit_move_insn (hard_frame_pointer_rtx, fp);
    emit_move_insn (pv, lab);
    emit_stack_restore (SAVE_NONLOCAL, stack, NULL_RTX);
    emit_move_insn (gp, gpv);
    emit_insn (gen_rtx_USE (VOIDmode, hard_frame_pointer_rtx));
    emit_insn (gen_rtx_USE (VOIDmode, stack_pointer_rtx));
    emit_insn (gen_rtx_USE (VOIDmode, gp));
    emit_indirect_jump (pv);
    DONE;
  }
)

;;----------------------------------------------------------------
;; Function prologue/epilogue and stack allocation
;;----------------------------------------------------------------
(define_expand "prologue"
  [(const_int 1)]
  ""
  {
      microblaze_expand_prologue ();
      DONE;
  }
)

(define_expand "epilogue"
  [(use (const_int 0))]
  ""
  {
      microblaze_expand_epilogue ();
      DONE;
  }
)

;; An insn to allocate new stack space for dynamic use (e.g., alloca).
;; We copy the return address, decrement the stack pointer and save the 
;; return address again at the new stack top 

(define_expand "allocate_stack"
  [(set (match_operand 0 "register_operand" "=r")
	(minus (reg 1) (match_operand 1 "register_operand" "")))
   (set (reg 1)
	(minus (reg 1) (match_dup 1)))]
  ""
  { 
    rtx retaddr = gen_rtx_MEM (Pmode, stack_pointer_rtx);
    rtx rtmp    = gen_rtx_REG (SImode, R_TMP);
    rtx neg_op0;

    emit_move_insn (rtmp, retaddr);
    if (GET_CODE (operands[1]) != CONST_INT)
    {
        neg_op0 = gen_reg_rtx (Pmode);
	emit_insn (gen_negsi2 (neg_op0, operands[1]));
    } else
        neg_op0 = GEN_INT (- INTVAL (operands[1]));

    emit_insn (gen_addsi3 (stack_pointer_rtx, stack_pointer_rtx, neg_op0));
    emit_move_insn (gen_rtx_MEM (Pmode, stack_pointer_rtx), rtmp);
    emit_move_insn (operands[0], virtual_stack_dynamic_rtx);
    emit_insn (gen_rtx_CLOBBER (SImode, rtmp));
    DONE;
  }
)

;; These patterns say how to save and restore the stack pointer.  We need not
;; save the stack pointer at function/block level since we are careful to
;; use a different method for getting the return address off the stack

(define_expand "save_stack_function"
  [(match_operand 0 "" "")
   (match_operand 1 "" "")]
  ""
  "DONE;")

(define_expand "restore_stack_function"
  [(match_operand 0 "" "")
   (match_operand 1 "" "")]
  ""
  "DONE;")

(define_expand "save_stack_block"
  [(match_operand 0 "" "")
   (match_operand 1 "" "")]
  ""
  "DONE;")

(define_expand "restore_stack_block"
  [(match_operand 0 "" "")
   (match_operand 1 "" "")]
  ""
  "DONE;")

;; Trivial return.  Make it look like a normal return insn as that
;; allows jump optimizations to work better .
(define_insn "return"
  [(return)]
  "microblaze_can_use_return_insn ()"
  { 
    if (microblaze_is_interrupt_handler())
        return "rtid\tr14, 0\;%#";
    else
        return "rtsd\tr15, 8\;%#";
  }
  [(set_attr "type"	"jump")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])

;; Normal return.
;; We match any mode for the return address, so that this will work with
;; both 32 bit and 64 bit targets.

(define_insn "return_internal"
  [(parallel[(use (match_operand:SI 0 "register_operand" ""))
             (return)])]
  ""
  {	
    if (microblaze_is_interrupt_handler())
        return "rtid\tr14,0 \;%#";
    else
        return "rtsd\tr15,8 \;%#";
  }
  [(set_attr "type"	"jump")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])


;; Block any insns from across this point
;; Useful to group sequences together.
(define_insn "blockage"
  [(unspec_volatile [(const_int 0)] 0)]
  ""
  ""
  [(set_attr "type"	"unknown")
  (set_attr "mode"	"none")
  (set_attr "length"	"0")])

  
;;----------------------------------------------------------------
;; Function calls
;;----------------------------------------------------------------

;; calls.c now passes a third argument, make saber happy

(define_expand "call"
  [(parallel [(call (match_operand 0 "memory_operand" "m")
		    (match_operand 1 "" "i"))
             (clobber (reg:SI R_SR))
             (use (match_operand 2 "" "")) ;; next_arg_reg
             (use (match_operand 3 "" ""))])] ;; struct_value_size_rtx
  ""
  {
    rtx addr;
    if (operands[0])		/* eliminate unused code warnings */
    {
        addr = XEXP (operands[0], 0);
        if ((GET_CODE (addr) != REG && (!CONSTANT_ADDRESS_P (addr) || TARGET_LONG_CALLS))
	    || ! call_insn_operand (addr, VOIDmode))
            XEXP (operands[0], 0) = copy_to_mode_reg (Pmode, addr);

        /* In order to pass small structures by value in registers
	   compatibly with the MICROBLAZE compiler, we need to shift the value
	   into the high part of the register.  Function_arg has encoded
	   a PARALLEL rtx, holding a vector of adjustments to be made
	   as the next_arg_reg variable, so we split up the insns,
	   and emit them separately.  */

        if (operands[2] != (rtx)0 && GET_CODE (operands[2]) == PARALLEL)
	{
            rtvec adjust = XVEC (operands[2], 0);
            int num = GET_NUM_ELEM (adjust);
            int i;

            for (i = 0; i < num; i++)
                emit_insn (RTVEC_ELT (adjust, i));
	}

        emit_call_insn (gen_call_internal0 (operands[0], operands[1],
                                            gen_rtx (REG, SImode, GP_REG_FIRST + MICROBLAZE_PPC_ABI_SUB_RETURN_ADDR_REGNUM)));

        DONE;
    }
  }
)

(define_expand "call_internal0"
  [(parallel [(call (match_operand 0 "" "")
		    (match_operand 1 "" ""))
             (clobber (match_operand:SI 2 "" ""))])]
  ""
  {
    /*      fprintf(stderr,"expand \t call_internal0  \n");*/
  }
)

(define_insn "call_internal1"
  [(call (mem (match_operand:SI 0 "call_insn_operand" "ri"))
	 (match_operand:SI 1 "" "i"))
  (clobber (reg:SI R_SR))]
  "!TARGET_ABICALLS && !TARGET_LONG_CALLS"
  {
    register rtx target = operands[0];
    register rtx target2=gen_rtx (REG, Pmode,GP_REG_FIRST + MICROBLAZE_PPC_ABI_SUB_RETURN_ADDR_REGNUM);
    /*  fprintf(stderr,"insn\t call_internal1  \n");	*/
    if (GET_CODE (target) == SYMBOL_REF) {
        gen_rtx_CLOBBER(VOIDmode,target2);
        return "brlid\tr15,%0\;%#";
    } else if (GET_CODE (target) == CONST_INT)
        return "la\t%@,r0,%0\;brald\tr15,%@\;%#";
    else if (GET_CODE(target) == REG)
        return "brald\tr15,%0\;%#";	
    else {
        fprintf(stderr,"Unsupported call insn\n");
        return NULL;
    }
  }
  [(set_attr "type"	"call")
  (set_attr "mode"	"none")
  (set_attr "length"	"4")])

(define_insn "call_internal2"
  [(call (mem (match_operand 0 "call_insn_operand" "ri"))
	 (match_operand 1 "" "i"))
  (clobber (match_operand:SI 2 "register_operand" "=d"))]
  "TARGET_ABICALLS && !TARGET_LONG_CALLS"
  {
    register rtx target = operands[0];
    /*  fprintf(stderr,"expand \t call_internal2  \n");*/
    if (GET_CODE (target) == SYMBOL_REF)
    {
        if (GET_MODE (target) == SImode)
            return "la\t%^,%0\;brlid\t%2,%^";
        else
            return "dla\t%^,%0\;MICROBLAZEjal\t%2,%^";
    }
    else if (GET_CODE (target) == CONST_INT)
        return "li\t%^,%0\;MICROBLAZEjal\t%2,%^";
    else if (REGNO (target) != PIC_FUNCTION_ADDR_REGNUM)
        return "move\t%^,%0\;MICROBLAZEjal\t%2,%^";
    else
        return "MICROBLAZEjal\t%2,%0";
  }
  [(set_attr "type"	"call")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])

(define_insn "call_internal3a"
  [(call (mem:SI (match_operand:SI 0 "register_operand" "r"))
	 (match_operand 1 "" "i"))
  (clobber (match_operand:SI 2 "register_operand" "=d"))]
  "!(Pmode == DImode) && !TARGET_ABICALLS && TARGET_LONG_CALLS"
  "MICROBLAZEjal\t%2,%0"
  [(set_attr "type"	"call")
  (set_attr "mode"	"none")
  (set_attr "length"	"1")])

(define_insn "call_internal3b"
  [(call (mem:DI (match_operand:DI 0 "register_operand" "r"))
	 (match_operand 1 "" "i"))
  (clobber (match_operand:SI 2 "register_operand" "=d"))]
  "Pmode == DImode && !TARGET_ABICALLS && TARGET_LONG_CALLS"
  "MICROBLAZEjal\t%2,%0"
  [(set_attr "type"	"call")
  (set_attr "mode"	"none")
  (set_attr "length"	"1")])

(define_insn "call_internal4a"
  [(call (mem:SI (match_operand:SI 0 "register_operand" "r"))
	 (match_operand 1 "" "i"))
  (clobber (match_operand:SI 2 "register_operand" "=d"))]
  "!(Pmode == DImode) && TARGET_ABICALLS && TARGET_LONG_CALLS"
  {
    if (REGNO (operands[0]) != PIC_FUNCTION_ADDR_REGNUM)
        return "move\t%^,%0\;MICROBLAZEjal\t%2,%^";
    else
        return "MICROBLAZEjal\t%2,%0";
  }
  [(set_attr "type"	"call")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])

(define_insn "call_internal4b"
  [(call (mem:DI (match_operand:DI 0 "register_operand" "r"))
	 (match_operand 1 "" "i"))
  (clobber (match_operand:SI 2 "register_operand" "=d"))]
  "Pmode == DImode && TARGET_ABICALLS && TARGET_LONG_CALLS"
  {
    if (REGNO (operands[0]) != PIC_FUNCTION_ADDR_REGNUM)
        return "move\t%^,%0\;MICROBLAZEjal\t%2,%^";
    else
        return "MICROBLAZEjal\t%2,%0";
  }
  [(set_attr "type"	"call")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])

;; calls.c now passes a fourth argument, make saber happy

(define_expand "call_value"
  [(parallel [(set (match_operand 0 "register_operand" "=df")
		   (call (match_operand 1 "memory_operand" "m")
			 (match_operand 2 "" "i")))
             (clobber (reg:SI R_SR))
             (use (match_operand 3 "" ""))])] ;; next_arg_reg
  ""
  {
    rtx addr;
    if (operands[0])		/* eliminate unused code warning */
    {
        addr = XEXP (operands[1], 0);
        if ((GET_CODE (addr) != REG && (!CONSTANT_ADDRESS_P (addr) || TARGET_LONG_CALLS))
            || ! call_insn_operand (addr, VOIDmode))
            XEXP (operands[1], 0) = copy_to_mode_reg (Pmode, addr);

        /* In order to pass small structures by value in registers
	   compatibly with the MICROBLAZE compiler, we need to shift the value
	   into the high part of the register.  Function_arg has encoded
	   a PARALLEL rtx, holding a vector of adjustments to be made
	   as the next_arg_reg variable, so we split up the insns,
	   and emit them separately.  */

        if (operands[3] != (rtx)0 && GET_CODE (operands[3]) == PARALLEL)
	{
            rtvec adjust = XVEC (operands[3], 0);
	    int num = GET_NUM_ELEM (adjust);
	    int i;

	    for (i = 0; i < num; i++)
	        emit_insn (RTVEC_ELT (adjust, i));
	}

        /* We have a call returning a DImode structure in an FP reg.
	   Strip off the now unnecessary PARALLEL.  */
      
        if (GET_CODE (operands[0]) == PARALLEL)
            operands[0] = XEXP (XVECEXP (operands[0], 0, 0), 0);

        emit_call_insn (gen_call_value_internal0 (operands[0], operands[1], operands[2],
		        gen_rtx (REG, SImode, GP_REG_FIRST + MICROBLAZE_PPC_ABI_SUB_RETURN_ADDR_REGNUM)));

        DONE;
    }
  }
)


(define_expand "call_value_internal0"
  [(parallel [(set (match_operand 0 "" "")
		   (call (match_operand 1 "" "")
			 (match_operand 2 "" "")))
             (clobber (match_operand:SI 3 "" ""))
             ])]
  ""
  {
    /* fprintf(stderr,"expand \t call_value_internal0  \n"); */
  }
)

(define_expand "call_value_expand"
  [(parallel[(set (match_operand 0 "register_operand" "=df")
                  (call (mem (match_operand 1 "call_insn_operand" "ri"))
                        (match_operand 2 "" "i")))
             (clobber (match_operand:SI 3 "register_operand" "=d"))])]
  "!TARGET_ABICALLS && !TARGET_LONG_CALLS"
  {
    register rtx target = operands[1];
    register rtx target2=gen_rtx (REG, Pmode,GP_REG_FIRST + MICROBLAZE_PPC_ABI_SUB_RETURN_ADDR_REGNUM);

    rtx operand0 = operands[0]; 
    rtx operand1 = operands[1];
    rtx operand2 = operands[2]; 

    /*  fprintf(stderr,"expand \t call_value_expand  \n");*/

    if (GET_CODE (target) == SYMBOL_REF) {
    /*	fprintf(stderr,"In call_value_expand\n");*/
        gen_call_value_intern (operand0,operand1,operand2,target2);
	gen_rtx_CLOBBER(VOIDmode,target2);
	DONE;
    }
    else
        return "MICROBLAZEjal\t%3,%1 #Illegal Code ";
  }
)


(define_insn "call_value_intern"
  [(parallel[(set (match_operand:SI 0 "register_operand" "=df")
                  (call (mem (match_operand:SI 1 "call_insn_operand" "ri"))
                        (match_operand:SI 2 "" "i")))
             (clobber (match_operand:SI 3 "register_operand" "=d"))])]
  ""
  { 
    register rtx target = operands[1];
    register rtx target2=gen_rtx (REG, Pmode,GP_REG_FIRST + MICROBLAZE_PPC_ABI_SUB_RETURN_ADDR_REGNUM);

    if (GET_CODE (target) == SYMBOL_REF){
	gen_rtx_CLOBBER(VOIDmode,target2);
	return "brlid\tr15,%1\;%#";
    }
    else if (GET_CODE (target) == CONST_INT)
        return "la\t%@,r0,%1\;brald\tr15,%@\;%#";
    else if (GET_CODE(target) == REG)
        return "brald\tr15,%1\;%#";	
    else 
        fprintf(stderr,"Unsupported call insn\n");
  }
  [(set_attr "type"	"call")
  (set_attr "mode"	"none")
  (set_attr "length"	"4")])


(define_insn "call_value_internal1"
  [(set (match_operand 0 "register_operand" "=df")
        (call (mem (match_operand 1 "call_insn_operand" "ri"))
              (match_operand 2 "" "i")))
  (clobber (match_operand:SI 3 "register_operand" "=d"))]
  "!TARGET_ABICALLS && !TARGET_LONG_CALLS"
  {
    register rtx target = operands[1];

    if (CONSTANT_ADDRESS_P (target))
        return "brlid\tr15,%1 ## Need to verify this\;%#";
    else
        return "brald\t%3,%1 ## NEed to verify this 2\;%#";
  }
  [(set_attr "type"	"call")
   (set_attr "mode"	"none")
   (set_attr "length"	"4")]
)

(define_insn "call_value_internal2"
  [(set (match_operand 0 "register_operand" "=df")
        (call (mem (match_operand 1 "call_insn_operand" "ri"))
              (match_operand 2 "" "i")))
  (clobber (match_operand:SI 3 "register_operand" "=d"))]
  "TARGET_ABICALLS && !TARGET_LONG_CALLS"
  {
    register rtx target = operands[1];

    if (GET_CODE (target) == SYMBOL_REF)
    {
        if (GET_MODE (target) == SImode)
            return "la\t%^,%1\;MICROBLAZEjal\t%3,%^";
        else
            return "dla\t%^,%1\;MICROBLAZEjal\t%3,%^";
    }
    else if (GET_CODE (target) == CONST_INT)
        return "li\t%^,%1\;MICROBLAZEjal\t%3,%^";
    else if (REGNO (target) != PIC_FUNCTION_ADDR_REGNUM)
        return "move\t%^,%1\;MICROBLAZEjal\t%3,%^";
    else
        return "MICROBLAZEjal\t%3,%1";
  }
  [(set_attr "type"	"call")
  (set_attr "mode"	"none")
  (set_attr "length"	"2")])



;; Call subroutine returning any type.
(define_expand "untyped_call"
  [(parallel [(call (match_operand 0 "" "")
		    (const_int 0))
             (match_operand 1 "" "")
             (match_operand 2 "" "")])]
  ""
  {
    if (operands[0])		/* silence statement not reached warnings */
    {
        int i;

        emit_call_insn (gen_call (operands[0], const0_rtx, NULL, const0_rtx));

        for (i = 0; i < XVECLEN (operands[2], 0); i++)
	{
	    rtx set = XVECEXP (operands[2], 0, i);
	    emit_move_insn (SET_DEST (set), SET_SRC (set));
	}

        emit_insn (gen_blockage ());
        DONE;
      }
  }
)

;;----------------------------------------------------------------
;; Misc.
;;----------------------------------------------------------------

(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
  [(set_attr "type"	"nop")
  (set_attr "mode"	"none")
  (set_attr "length"	"4")])


;; This does not work reliably. Fails on very -simple- functions
;; CR 202429

;;(define_expand "stack_check"
;;   [(const_int 0)
;;    (use     (reg:SI R_SP))
;;    (use     (reg:SI R_TMP))]
;;   "TARGET_STACK_CHECK"
;;   {

;;     /* Define RTX */
;;     rtx reg18_rtx = gen_rtx_REG (SImode, R_TMP);
;;     rtx heap_rtx  = gen_rtx_SYMBOL_REF (SImode, "_malloc_base_addr");
;;     rtx heap_mem_rtx 
;;                   = gen_rtx_raw_MEM (SImode, heap_rtx);
;;     rtx label_ref = gen_rtx_SYMBOL_REF (SImode, "_stack_overflow_exit");
;;     rtx jinsn, insn;

;;     /* Load Heap */
;;     emit_move_insn (reg18_rtx, heap_mem_rtx);

;;     /* Negate the heap value */
;;     emit_insn (gen_negsi2 (reg18_rtx, reg18_rtx));

;;     /* Add the negated heap value to the Stack value */
;;     insn = emit_insn (gen_addsi3_internal (reg18_rtx, reg18_rtx, stack_pointer_rtx));

;;     /*  Check for overflow and jump  
;;         if (reg_18 < 0 ) Jump to stack_overflow_exit else continue; 
;;     */
;;     jinsn = emit_jump_insn_after (gen_branch_zero (gen_rtx_LE (VOIDmode, reg18_rtx, const0_rtx),
;;                                                    reg18_rtx, label_ref, pc_rtx), insn);
;;     DONE;
;;   }
;; )     
