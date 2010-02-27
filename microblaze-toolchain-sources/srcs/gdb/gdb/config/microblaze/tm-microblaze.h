/* Parameters for execution on a Xilinx microblaze.
   Copyright (C) 1995 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA. */

/*
 * Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by Xilinx, Inc.  The name of the Company may not be used to endorse 
 * or promote products derived from this software without specific prior 
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	Xilinx, Inc.
 */

#ifndef TM_MICROBLAZE_H
#define TM_MICROBLAZE_H

/* The microblaze is big endian (by default) */
#define TARGET_BYTE_ORDER_DEFAULT BIG_ENDIAN

/* All registers are 32 bits */
#define REGISTER_SIZE 4
#define MAX_REGISTER_RAW_SIZE 4

#define REGISTER_VIRTUAL_TYPE(REG) builtin_type_int

#define REGISTER_BYTE(REG) ((REG) * REGISTER_SIZE)
#define REGISTER_VIRTUAL_SIZE(REG) 4
#define REGISTER_RAW_SIZE(REG) 4

#define MAX_REGISTER_VIRTUAL_SIZE 4

#define REGISTER_BYTES (NUM_REGS * REGISTER_SIZE)

extern char *microblaze_register_names[];
#define REGISTER_NAME(I) microblaze_register_names[I]

/* Registers. The Xilinx microblaze contains:

   32 32-bit general purpose registers (r0-r31)
    5 32-bit special registers (pc, msr, ear, esr, fsr)
   ------
   37 registers */
#define NUM_REGS 37
#define PC_REGNUM 32
#define MSR_REGNUM 33
#define EAR_REGNUM 34
#define ESR_REGNUM 35
#define FSR_REGNUM 36
#define ROSDP_REGNUM 2
#define SP_REGNUM 1

#ifndef TARGET_HAS_HARDWARE_WATCHPOINTS

#define TARGET_HAS_HARDWARE_WATCHPOINTS

#define TARGET_CAN_USE_HARDWARE_WATCHPOINT(type, count, ot) \
	microblaze_can_use_hardware_watchpoints(type, count, ot)
#endif

/*
#ifndef WIN
#define FP_REGNUM 35 
#endif
*/

#define RWSDP_REGNUM 13
#define LINK_REGNUM 15
#define PR_REGNUM 15
#define FIRST_ARGREG 5
#define LAST_ARGREG 10
#define RETVAL_REGNUM 3

/*
#ifdef WIN
*/
extern CORE_ADDR microblaze_target_read_fp();
#define TARGET_READ_FP() microblaze_target_read_fp()

extern void microblaze_target_write_fp(CORE_ADDR val);
#define TARGET_WRITE_FP(val) microblaze_target_write_fp(val)
/*
#endif
*/

/* Offset from address of function to start of its code.
   Zero on most machines.  */
#define FUNCTION_START_OFFSET 0

#define DECR_PC_AFTER_BREAK 0


/* BREAKPOINT_FROM_PC uses the program counter value to determine
   the breakpoint that should be used. microblaze breakpoint does not depend on PC*/
/* BIG_BREAKPOINT defines the big-endian breakpoint that should be used */
#ifdef __MICROBLAZE_UCLINUX__
#define BIG_BREAKPOINT {0xb9, 0xcc, 0x00, 0x60}
#define LITTLE_BREAKPOINT {0x60, 0x00, 0xcc, 0xb9}
#else
#define BIG_BREAKPOINT {0x98, 0x0c, 0x00, 0x00}
#define LITTLE_BREAKPOINT {0x00, 0x00, 0xc0, 0x89}
#endif


/* Stack grows downward in memory */
#define INNER_THAN(LHS,RHS) ((LHS) < (RHS))

/* usha #define SAVED_PC_AFTER_CALL(FRAME)  read_register (PR_REGNUM)*/
extern CORE_ADDR microblaze_saved_pc_after_call(int call_pc);
#define SAVED_PC_AFTER_CALL(FRAME)  microblaze_saved_pc_after_call (read_register(LINK_REGNUM))


struct frame_info;
struct type;
struct value;

extern void microblaze_init_extra_frame_info (struct frame_info *fi);
#define INIT_EXTRA_FRAME_INFO(FROMLEAF, FI) microblaze_init_extra_frame_info ((FI))
#define INIT_FRAME_PC		/* Not necessary */
#define FRAME_INIT_SAVED_REGS(FI)	/* handled by init_extra_frame_info */

extern CORE_ADDR microblaze_frame_chain (struct frame_info *fi);
#define FRAME_CHAIN(FI) microblaze_frame_chain ((FI))
#define FRAME_CHAIN_VALID(FP, FI) generic_file_frame_chain_valid ((FP), (FI))

extern CORE_ADDR microblaze_frame_saved_pc (struct frame_info *);
#define FRAME_SAVED_PC(FI) (microblaze_frame_saved_pc ((FI)))

/* Extracting/storing return values. */
extern void microblaze_store_return_value (struct type *type, char *valbuf);
#define DEPRECATED_STORE_RETURN_VALUE(TYPE, VALBUF) microblaze_store_return_value ((TYPE), (VALBUF))

extern void microblaze_extract_return_value (struct type *type, char *regbut, char *valbuf);
#define DEPRECATED_EXTRACT_RETURN_VALUE(TYPE,REGBUF,VALBUF) \
    microblaze_extract_return_value ((TYPE), (REGBUF), (VALBUF));

#define STORE_STRUCT_RETURN(ADDR, SP)	/* handled by microblaze_push_arguments */

extern CORE_ADDR microblaze_extract_struct_value_address (char *regbuf);
#define DEPRECATED_EXTRACT_STRUCT_VALUE_ADDRESS(REGBUF) \
    microblaze_extract_struct_value_address (REGBUF)

extern CORE_ADDR microblaze_skip_prologue (CORE_ADDR pc);
/*
#define SKIP_PROLOGUE(PC) (PC) = microblaze_skip_prologue ((PC))
*/
#define SKIP_PROLOGUE(PC) (microblaze_skip_prologue (PC))

#define FRAME_ARGS_SKIP 0
extern CORE_ADDR microblaze_frame_args_address (struct frame_info *fi);
#define FRAME_ARGS_ADDRESS(FI) microblaze_frame_args_address ((FI))
extern CORE_ADDR microblaze_frame_locals_address (struct frame_info *fi);
#define FRAME_LOCALS_ADDRESS(FI) microblaze_frame_locals_address ((FI))
#define FRAME_NUM_ARGS(FI) (-1)


extern void microblaze_pop_frame (struct frame_info *fi);
#define POP_FRAME microblaze_pop_frame (get_current_frame ())

#define USE_GENERIC_DUMMY_FRAMES 1 
#define CALL_DUMMY                   {0}
#define CALL_DUMMY_START_OFFSET      (0)
/*#define CALL_DUMMY_BREAKPOINT_OFFSET (0)*/
#define CALL_DUMMY_LOCATION          AT_ENTRY_POINT
/* #define FIX_CALL_DUMMY(DUMMY, START, FUNADDR, NARGS, ARGS, TYPE, GCCP)*/
#define CALL_DUMMY_ADDRESS()         entry_point_address ()
#define SIZEOF_CALL_DUMMY_WORDS      0
#define SAVE_DUMMY_FRAME_TOS(SP)     generic_save_dummy_frame_tos (SP)

extern void microblaze_fix_call_dummy PARAMS ((char*, CORE_ADDR, CORE_ADDR, int, struct value **, struct type *, int));
#define FIX_CALL_DUMMY(DUMMY, START, FUNADDR, NARGS, ARGS, TYPE, GCCP) \
  microblaze_fix_call_dummy(DUMMY, START, FUNADDR, NARGS, ARGS, TYPE, GCCP)

extern CORE_ADDR microblaze_push_return_address PARAMS ((CORE_ADDR, CORE_ADDR));
#define PUSH_RETURN_ADDRESS(PC, SP)  microblaze_push_return_address (PC, SP)

#define PUSH_DUMMY_FRAME	generic_push_dummy_frame ()

extern CORE_ADDR microblaze_push_arguments PARAMS ((int, struct value **, CORE_ADDR,
					       unsigned char, CORE_ADDR));
#define PUSH_ARGUMENTS(NARGS, ARGS, SP, STRUCT_RETURN, STRUCT_ADDR) \
  (SP) = microblaze_push_arguments (NARGS, ARGS, SP, STRUCT_RETURN, STRUCT_ADDR)

/*#define PC_IN_CALL_DUMMY(PC, SP, FP) generic_pc_in_call_dummy (PC, SP, FP)*/
#define PC_IN_CALL_DUMMY(PC, SP, FP) \
  (PC == entry_point_address()+4 || PC == entry_point_address()+8)

/* Microblaze will never pass a sturcture by reference. It will always be split
   between registers and stack. */
#define REG_STRUCT_HAS_ADDR(GCC_P, TYPE) 0

extern use_struct_convention_fn microblaze_use_struct_convention;
#define USE_STRUCT_CONVENTION(GCC_P, TYPE) microblaze_use_struct_convention (GCC_P, TYPE)

/* override the default get_saved_register function with
   one that takes account of generic CALL_DUMMY frames */
#define GET_SAVED_REGISTER(raw_buffer, optimized, addrp, frame, regnum, lval) \
    generic_get_saved_register (raw_buffer, optimized, addrp, frame, regnum, lval)

/* Cons up virtual frame pointer for trace */
extern void microblaze_virtual_frame_pointer PARAMS ((CORE_ADDR, long *, long *));
#define TARGET_VIRTUAL_FRAME_POINTER(PC, REGP, OFFP) \
	microblaze_virtual_frame_pointer ((PC), (REGP), (OFFP))

/* microblaze cannot be bi-endian. */
#define TARGET_BYTE_ORDER_SELECTABLE_P 0

/* For PE, gcc will tell us what th real type of
   arguments are when it promotes arguments. */
#define BELIEVE_PCC_PROMOTION 1

/* All .stabs information is absolute */
#define BLOCK_ADDRESS_ABSOLUTE 1

/* Hook which will be called after $HOME/.gdbinit and $(PWD)/.gdbinit */
#define BEFORE_MAIN_LOOP_HOOK microblaze_before_main_loop();

/* Hook which will be called after $HOME/.gdbinit and $(PWD)/.gdbinit */
extern void microblaze_before_main_loop(); /* in microblaze-tdep.c */

/* Enable software single stepping for uClinux debugging */
#ifdef __MICROBLAZE_UCLINUX__
#define SOFTWARE_SINGLE_STEP_P()    1
#define SOFTWARE_SINGLE_STEP(signal, insert_breakpoints_p)   microblaze_software_single_step (signal, insert_breakpoints_p)
#endif

/* siva/9/16/05: default_coerce_float_to_double does not work for us  */
#define COERCE_FLOAT_TO_DOUBLE(formal, actual) (1)

#endif
