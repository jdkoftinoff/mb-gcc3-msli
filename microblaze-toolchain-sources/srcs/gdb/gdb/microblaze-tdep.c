/* Target-machine dependent code for Xilinx microblaze for GDB, the GNU debugger
   Copyright (C) 1999 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

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

#include <string.h>
#include "defs.h"
#include "frame.h"
#include "symtab.h"
#include "value.h"
#include "gdbcmd.h"
#include "breakpoint.h"
#include "../opcodes/microblaze-opcm.h"

/* Functions declared and used only in this file */

static CORE_ADDR microblaze_analyze_prologue (struct frame_info *fi, CORE_ADDR pc);

static struct frame_info *analyze_dummy_frame (CORE_ADDR pc, CORE_ADDR frame);

static int get_insn (CORE_ADDR pc);

static CORE_ADDR microblaze_read_memory_integer(CORE_ADDR addr);

/* Functions exported from this file */

void _initialize_microblaze (void);

void microblaze_init_extra_frame_info (struct frame_info *fi);

CORE_ADDR microblaze_saved_pc_after_call(int regnum);

CORE_ADDR microblaze_frame_saved_pc (struct frame_info *fi);

CORE_ADDR microblaze_find_callers_reg (struct frame_info *fi, int regnum);

CORE_ADDR microblaze_frame_args_address (struct frame_info *fi);

CORE_ADDR microblaze_frame_locals_address (struct frame_info *fi);

void microblaze_virtual_frame_pointer (CORE_ADDR pc, long *reg, long *offset);

CORE_ADDR microblaze_target_read_fp ();

void microblaze_target_write_fp (CORE_ADDR val);

CORE_ADDR microblaze_push_return_address (CORE_ADDR pc, CORE_ADDR sp);

CORE_ADDR microblaze_push_arguments (int nargs, struct value ** args, CORE_ADDR sp,
			unsigned char struct_return, CORE_ADDR struct_addr);

void microblaze_pop_frame (struct frame_info *fi);

CORE_ADDR microblaze_skip_prologue (CORE_ADDR pc);

CORE_ADDR microblaze_frame_chain (struct frame_info *fi);

int microblaze_use_struct_convention (int gcc_p, struct type *type);

void microblaze_store_return_value (struct type *type, char *valbuf);

CORE_ADDR microblaze_extract_struct_value_address (char *regbuf);

void microblaze_extract_return_value (struct type *type, char *regbuf, char *valbuf);

void microblaze_fix_call_dummy( char* dummy, CORE_ADDR start_sp, CORE_ADDR funaddr, int nargs, struct value **args, struct type * func_value_type, int using_gcc);

void microblaze_software_single_step (unsigned int ignore, int insert_breakpoints_p);

int microblaze_can_use_hardware_watchpoints (enum bptype type, int count, int ot);

//#define MICROBLAZE_DEBUG
#ifdef MICROBLAZE_DEBUG
int microblaze_debug = 0;
#endif

/* The registers of the Xilinx microblaze processor */
/* *INDENT-OFF* */
char *microblaze_register_names[] =
{ "r0",   "r1",  "r2",    "r3",   "r4",   "r5",   "r6",   "r7",
  "r8",   "r9",  "r10",   "r11",  "r12",  "r13",  "r14",  "r15",
  "r16",  "r17", "r18",   "r19",  "r20",  "r21",  "r22",  "r23",
  "r24",  "r25", "r26",   "r27",  "r28",  "r29",  "r30",  "r31",
  "rpc",  "rmsr", "rear", "resr", "rfsr" };
/* *INDENT-ON* */



/* Additional info that we use for managing frames */
struct frame_extra_info
  {
    /* A generic status word */
    int status;

    /* Size of this frame */
    int framesize;

    /* The register that is acting as a frame pointer, if
       it is being used.  This is undefined if status
       does not contain the flag MY_FRAME_IN_FP. */
    int fp_regnum;
  };

/* frame_extra_info status flags */

/* The base of the current frame is actually in the stack pointer.
   This happens when there is no frame pointer (microblaze ABI does not
   require a frame pointer) or when we're stopped in the prologue or
   epilogue itself.  In these cases, microblaze_analyze_prologue will need
   to update fi->frame before returning or analyzing the register
   save instructions. */
#define MY_FRAME_IN_SP 0x1

/* The base of the current frame is in a frame pointer register.
   This register is noted in frame_extra_info->fp_regnum.

   Note that the existance of an FP might also indicate that the
   function has called alloca. */
#define MY_FRAME_IN_FP 0x2

/* This flag is set to indicate that this frame is the top-most
   frame. This tells frame chain not to bother trying to unwind
   beyond this frame. */
#define NO_MORE_FRAMES 0x4

/* Instruction macros used for analyzing the prologue */
/* This set of instruction macros need to be changed whenever the prologue generated
 * by the compiler could have more instructions or different type of instructions
 * This set also needs to be verified if it is complete 
 */
#define IS_RETURN(op) (op == rtsd || op == rtid)
#define IS_UPDATE_SP(op, rd, ra) ((op == addik || op == addi) && rd == REG_SP && ra == REG_SP)
#define IS_SPILL_SP(op, rd, ra) ((op == swi || op == sw) && rd == REG_SP && ra == REG_SP)
#define IS_SPILL_REG(op, rd, ra) ((op == swi || op == sw) && rd != REG_SP && ra == REG_SP)
#define IS_ALSO_SPILL_REG(op, rd, ra, rb) ((op == swi || op == sw) && rd != REG_SP && ra == 0 && rb == REG_SP)
#define IS_SETUP_FP(op, ra, rb) ((op == add || op == addik) && ra == REG_SP && rb == 0)
#define IS_SPILL_REG_FP(op, rd, ra, fpregnum) ((op == swi || op == sw) && rd != REG_SP && ra == fpregnum && ra != 0)
#define IS_SAVE_HIDDEN_PTR(op, rd, ra, rb) ((op == add || op == addik) && ra == FIRST_ARGREG && rb == 0)



#ifdef MICROBLAZE_DEBUG
static void
microblaze_dump_insn (char *commnt, CORE_ADDR pc, int insn)
{
  if (microblaze_debug)
    {
      printf_filtered ("MICROBLAZE:  %s %08x %08x ",
		       commnt, (unsigned int) pc, (unsigned int) insn);
      (*tm_print_insn) (pc, &tm_print_insn_info);
      printf_filtered ("\n");
    }
}
#define microblaze_insn_debug(args) { if (microblaze_debug) printf_filtered args; }
#else /* !MICROBLAZE_DEBUG */
#define microblaze_dump_insn(a,b,c) {}
#define microblaze_insn_debug(args) {}
#endif

/* Helper function for several routines below.  This funtion simply
   sets up a fake, aka dummy, frame (not a _call_ dummy frame) that
   we can analyze with microblaze_analyze_prologue. */

static struct frame_info *
analyze_dummy_frame (CORE_ADDR pc, CORE_ADDR frame)
{
  static struct frame_info *dummy = NULL;

  if (dummy == NULL)
    {
      dummy = (struct frame_info *) xmalloc (sizeof (struct frame_info));
      dummy->saved_regs = (CORE_ADDR *) xmalloc (SIZEOF_FRAME_SAVED_REGS);
      dummy->extra_info =
	(struct frame_extra_info *) xmalloc (sizeof (struct frame_extra_info));
    }

  dummy->next = NULL;
  dummy->prev = NULL;
  dummy->pc = pc;
  dummy->frame = frame;
  dummy->extra_info->status = 0;
  dummy->extra_info->framesize = 0;
  memset (dummy->saved_regs, '\000', SIZEOF_FRAME_SAVED_REGS);
  microblaze_analyze_prologue (dummy, 0);
  return dummy;
}

/* Function prologues on the Xilinx microblaze processors consist of:

   - adjustments to the stack pointer (r1) (addi r1, r1, imm)
   - making a copy of r1 into another register (a "frame" pointer) (add r?, r1, r0)
   - store word/multiples that use r1 or the frame pointer as the base address (swi r?, r1, imm OR swi r?, fp, imm)

   Note that microblaze really doesn't have a real frame pointer.
   Instead, the compiler may copy the SP into a register (usually
   r19) to act as an arg pointer.  For our target-dependent purposes,
   the frame info's "frame" member will be the beginning of the
   frame. The SP could, in fact, point below this.

   The prologue ends when an instruction fails to meet either of
   these criteria. */

/* Analyze the prologue of frame FI to determine where registers are saved,
   the end of the prologue, etc. Return the address of the first line
   of "real" code (i.e., the end of the prologue). */

static CORE_ADDR
microblaze_analyze_prologue (struct frame_info *fi, CORE_ADDR pc )
{
  CORE_ADDR func_addr, func_end, addr, stop, prologue_end_addr;
  CORE_ADDR stack_size;
  int insn, rn, rd, ra, rb, imm;
  enum microblaze_instr op;
  int status, fp_regnum, flags;
  int framesize;
  int register_offsets[NUM_REGS];
  char *name;
  boolean save_hidden_pointer_found = false;
  boolean non_stack_instruction_found = false;

  /* If provided, use the PC in the frame to look up the
     start of this function. */
  pc = (fi == NULL ? pc : fi->pc);

  /* Find the start of this function. */
  status = find_pc_partial_function (pc, &name, &func_addr, &func_end);

  /* If the start of this function could not be found or if the debbuger
     is stopped at the first instruction of the prologue, do nothing. */
  if (status == 0)
    return pc;

  /* If the debugger is entry function, give up. */
  if (func_addr == entry_point_address ())
    {
      if (fi != NULL)
	fi->extra_info->status |= NO_MORE_FRAMES;
      return pc;
    }

  /* At the start of a function, our frame is in the stack pointer. */
  flags = MY_FRAME_IN_SP;

  /* Start decoding the prologue.  We start by checking two special cases:

     1. We're about to return
     2. We're at the first insn of the prologue.

     If we're about to return, our frame has already been deallocated.
     If we are stopped at the first instruction of a prologue,
     then our frame has not yet been set up. */

  /* Get the first insn from memory */
  microblaze_insn_debug (("MICROBLAZE: starting prologue decoding\n"));
  insn = get_insn (pc);
  microblaze_dump_insn ("got 1: ", pc, insn);
  op = microblaze_decode_insn (insn, &rd, &ra, &rb, &imm);

  /* Check for return. */
  if (fi != NULL && IS_RETURN(op))
    {
      microblaze_insn_debug (("MICROBLAZE: got return"));
      if (fi->next == NULL)
	fi->frame = read_sp ();
      fi->extra_info->status = flags;
      return fi->pc;
    }

  /* Check for first insn of prologue */
  if (fi != NULL && fi->pc == func_addr)
    {
      if (fi->next == NULL)
	fi->frame = read_sp ();
      fi->extra_info->status = flags;
      return fi->pc;
    }

  /* Figure out where to stop scanning */
  stop = (fi ? fi->pc : func_end);

  /* Don't walk off the end of the function */
  stop = (stop > func_end ? func_end : stop);

  /* REGISTER_OFFSETS will contain offsets, from the top of the frame
     (NOT the frame pointer), for the various saved registers or -1
     if the register is not saved. */
  for (rn = 0; rn < NUM_REGS; rn++)
    register_offsets[rn] = -1;

  /* Analyze the prologue. Things we determine from analyzing the
     prologue include:
     * the size of the frame
     * where saved registers are located (and which are saved)
     * FP used? */
  microblaze_insn_debug (("MICROBLAZE: Scanning prologue: func_addr=0x%x, stop=0x%x\n",
		     (unsigned int) func_addr, (unsigned int) stop));

  framesize = 0;
  for (prologue_end_addr = addr = func_addr; addr < stop; addr += INST_WORD_SIZE)
    {
      /* Get next insn */
      insn = get_insn (addr);
      microblaze_dump_insn ("got 2: ", addr, insn);
      op = microblaze_decode_insn (insn, &rd, &ra, &rb, &imm);

      /* This code is very sensitive to what functions are present in the prologue. It assumes
	 that the (addi, addik, swi, sw) can be the only instructions in the prologue.
	 */
      if (IS_UPDATE_SP(op, rd, ra))
	{
	  microblaze_insn_debug (("MICROBLAZE: got addi r1,r1,%d; contnuing\n", imm));
	  framesize = -1 * imm; /* Since stack grows towards low memory */
	  save_hidden_pointer_found = false;
	  continue;
	}
      else if (IS_SPILL_SP(op, rd, ra))
	{
	  /* Spill stack pointer */
	  register_offsets[rd] = imm; /* SP spilled before updating */

	  microblaze_insn_debug (("MICROBLAZE: swi r1 r1 %d, continuing\n", imm));
	  save_hidden_pointer_found = false;
	  continue;
	}
      else if (IS_SPILL_REG(op, rd, ra))
	{
	  /* Spill register */
	  register_offsets[rd] = imm - framesize; /* reg spilled after updating */

	  microblaze_insn_debug (("MICROBLAZE: swi %d r1 %d, continuing\n", rd, imm));
	  save_hidden_pointer_found = false;
	  continue;

	}
      else if (IS_ALSO_SPILL_REG(op, rd, ra, rb))
	{
	  /* Spill register */
	  register_offsets[rd] = 0 - framesize; /* reg spilled after updating */

	  microblaze_insn_debug (("MICROBLAZE: sw %d r0 r1, continuing\n", rd));
	  save_hidden_pointer_found = false;
	  continue;

	}
      else if (IS_SETUP_FP(op, ra, rb))
	{
	  /* We have a frame pointer.  Note
	     the register which is acting as the frame pointer. */
	  flags |= MY_FRAME_IN_FP;
	  flags &= ~MY_FRAME_IN_SP;
	  fp_regnum = rd;
	  microblaze_insn_debug (("MICROBLAZE: Found a frame pointer: r%d\n", fp_regnum));
	  save_hidden_pointer_found = false;
	  continue;
	}
      else if (IS_SPILL_REG_FP(op, rd, ra, fp_regnum))
	{
	  register_offsets[rd] = imm - framesize; /* reg spilled after updating */

	  microblaze_insn_debug (("MICROBLAZE: swi %d %d %d, continuing\n", rd, ra, imm));
	  save_hidden_pointer_found = false;
	  continue;
	}
      else if (IS_SAVE_HIDDEN_PTR(op, rd, ra, rb))
	{
	  /* If the first argument is a hidden pointer to the area where the 
	     return structure is to be saved, then it is saved as part of the 
	     prologue */

	  microblaze_insn_debug (("MICROBLAZE: add %d %d %d, continuing\n", rd, ra, rb));
	  save_hidden_pointer_found = true;
	  continue;
	}

      /* as a result of the modification in the next step where we continue to analyze the 
	 prologue till we reach a control flow instruction, we need another variable to store
	 when exactly a non-stack instruction was encountered, which is the current definition
	 of a prologue
       */
      if (!non_stack_instruction_found)
      		prologue_end_addr = addr;
      non_stack_instruction_found = true;

      /* When optimizations are enabled, it is not guaranteed that prologue instructions
	 are not mixed in with other instructions from the program. Some programs show this 
	 behavior at -O2. This can be avoided by adding -fno-schedule-insns2 switch as of now (edk 8.1)
	 In such cases, we scan the function until we see the first control instruction.
       */

	   
      {
	      unsigned op = (unsigned)insn >> 26;

	      if (!(op == 0x26 || op == 0x27 || op == 0x2d || op == 0x2e || op == 0x2f))
		      continue;    /* continue if not control flow (branch, return) */
	      else if (op == 0x2c)
		      continue;    /* continue if imm */
      }

      /* This is not a prologue insn, so stop here. */
      microblaze_insn_debug (("microblaze: insn is not a prologue insn -- ending scan\n"));
      break;
    }

  microblaze_insn_debug (("microblaze: done analyzing prologue\n"));
  microblaze_insn_debug (("microblaze: prologue end = 0x%x\n", addr));

  /* If the last instruction was an add rd, r5, r0 then don't count it as part of
     the prologue */
  if (save_hidden_pointer_found) {
    addr -= INST_WORD_SIZE;
    prologue_end_addr -= INST_WORD_SIZE;
  }

  /* Save everything we have learned about this frame into FI. */
  if (fi != NULL)
    {
      fi->extra_info->framesize = framesize;
      fi->extra_info->fp_regnum = fp_regnum;
      fi->extra_info->status = flags;

      /* Fix the frame pointer. When gcc uses r8 as a frame pointer,
         it is really an arg ptr. We adjust fi->frame to be a "real"
         frame pointer. */
      if (fi->next == NULL)
	{
	  if (fi->extra_info->status & MY_FRAME_IN_SP)
	    fi->frame = read_sp () + framesize;
	  else
	    fi->frame = read_register (fp_regnum) + framesize;
	}

      /* Note where saved registers are stored. The offsets in REGISTER_OFFSETS
         are computed relative to the top of the frame. */
      for (rn = 0; rn < NUM_REGS; rn++)
	{
	  if (register_offsets[rn] != -1)
	    {
	      fi->saved_regs[rn] = fi->frame + register_offsets[rn];
	      microblaze_insn_debug (("Saved register %s stored at 0x%08x, value=0x%08x\n",
			       microblaze_register_names[rn], fi->saved_regs[rn],
				microblaze_read_memory_integer (fi->saved_regs[rn])));
	    }
	}
    }

  /* Return addr of first non-prologue insn. */
  return prologue_end_addr;
}

/* Given a GDB frame, determine the address of the calling function's frame.
   This will be used to create a new GDB frame struct, and then
   INIT_EXTRA_FRAME_INFO and INIT_FRAME_PC will be called for the new frame. */

CORE_ADDR
microblaze_frame_chain (struct frame_info * fi)
{
  struct frame_info *dummy;
  CORE_ADDR callers_addr;

  /* Analyze the prologue of this function. */
  if (fi->extra_info->status == 0)
    microblaze_analyze_prologue (fi, 0);

  /* If microblaze_analyze_prologue set NO_MORE_FRAMES, quit now. */
  if (fi->extra_info->status & NO_MORE_FRAMES)
    return 0;

  /* Now that we've analyzed our prologue, we can start to ask
     for information about our caller. The easiest way to do
     this is to analyze our caller's prologue. 

     If our caller has a frame pointer, then we need to find
     the value of that register upon entry to our frame.
     This value is either in fi->saved_regs[rn] if it's saved,
     or it's still in a register.

     If our caller does not have a frame pointer, then his frame base
     is <our base> + -<caller's frame size>. */
  dummy = analyze_dummy_frame (FRAME_SAVED_PC (fi), fi->frame);

  if (dummy->extra_info->status & MY_FRAME_IN_FP)
    {
      int fp = dummy->extra_info->fp_regnum;

      /* Our caller has a frame pointer. */
      if (fi->saved_regs[fp] != 0)
	{
	  /* The "FP" was saved on the stack.  Don't forget to adjust
	     the "FP" with the framesize to get a real FP. */
	  callers_addr = microblaze_read_memory_integer (fi->saved_regs[fp]) + dummy->extra_info->framesize;
	}
      else
	{
	  /* It's still in the register.  Don't forget to adjust
	     the "FP" with the framesize to get a real FP. */
  	  callers_addr = read_register (fp) + dummy->extra_info->framesize;
	}
    }
  else
    {
      /* Our caller does not have a frame pointer. */
      callers_addr = fi->frame + dummy->extra_info->framesize;
    }
  return callers_addr;
}

/* Skip the prologue of the function at PC. */

CORE_ADDR
microblaze_skip_prologue (CORE_ADDR pc)
{
  CORE_ADDR func_addr, func_end;
  struct symtab_and_line sal;
  CORE_ADDR start_pc = 0;
  CORE_ADDR ostart_pc = 0;

  /* If we have line debugging information, then the end of the
     prologue should the first assembly instruction of  the first
     source line */
  /* Debugging info does not always give the right answer since 
     parameters are stored on stack after this.  Always analyze the prologue */
  if (find_pc_partial_function (pc, NULL, &func_addr, &func_end))
    {
      sal = find_pc_line (func_addr, 0);
      if (sal.end && sal.end < func_end)
	start_pc = sal.end;
    }
  ostart_pc = microblaze_analyze_prologue (NULL, pc);
  if (ostart_pc > start_pc)
    return ostart_pc;
  else
    return start_pc;
}

/* Return the address at which function arguments are offset. */
CORE_ADDR
microblaze_frame_args_address (struct frame_info * fi)
{
  return fi->frame;
}

CORE_ADDR
microblaze_frame_locals_address (struct frame_info * fi)
{
  return fi->frame;
}

/* Return the frame pointer in use at address PC. */

void
microblaze_virtual_frame_pointer (CORE_ADDR pc, long *reg, long *offset)
{
  struct frame_info *dummy = analyze_dummy_frame (pc, 0);
  if (dummy->extra_info->status & MY_FRAME_IN_FP)
    {
      *reg = dummy->extra_info->fp_regnum;
      *offset = 0;
    }
  else
    {
      *reg = SP_REGNUM;
      *offset = 0;
    }
}

/* Functions to read and write the frame pointer */
CORE_ADDR
microblaze_target_read_fp()
{
  CORE_ADDR pc;
  long reg, offset;

  pc = read_pc();
  microblaze_virtual_frame_pointer( pc, &reg, &offset);
  return read_register(reg) + offset;
}

void
microblaze_target_write_fp( CORE_ADDR val )
{
  /* Should not be writing into the frame pointer */
  return;
}

/* Find the value of register REGNUM in frame FI. */

CORE_ADDR
microblaze_find_callers_reg (struct frame_info *fi, int regnum)
{
  LONGEST ret;
  for (; fi != NULL; fi = fi->next)
    {
      if (PC_IN_CALL_DUMMY (fi->pc, fi->frame, fi->frame))
	return generic_read_register_dummy (fi->pc, fi->frame, regnum);
      else if (fi->saved_regs[regnum] != 0) 
	return microblaze_read_memory_integer (fi->saved_regs[regnum]);
    }

  return read_register (regnum);
}

/* Find the saved pc in frame FI. */

CORE_ADDR
microblaze_frame_saved_pc (struct frame_info * fi)
{

  if (PC_IN_CALL_DUMMY (fi->pc, fi->frame, fi->frame))
    return generic_read_register_dummy (fi->pc, fi->frame, PC_REGNUM);
  else
    return microblaze_find_callers_reg (fi, PR_REGNUM);
  /*    return microblaze_find_callers_reg (fi, PC_REGNUM); */
}

/* INFERIOR FUNCTION CALLS */

/* This routine gets called when either the user uses the "return"
   command, or the call dummy breakpoint gets hit. */

void
microblaze_pop_frame (struct frame_info *fi)
{
  int rn;
  CORE_ADDR func_addr, func_end, addr;
  enum microblaze_instr op;
  int insn, rd, ra, rb, imm;
  int status;
  char *name;
  int offset = 0;	/* offset that the return instruction specifies */

  /* Find the start and end of this function. */
  /* siva/9/19/05: pop frame was not computing this offset. copied code from
     microblaze_fix_call_dummy to find the return insn & the offset */
  status = find_pc_partial_function (fi->pc, &name, &func_addr, &func_end);

  for (addr = func_addr; addr < func_end; addr += INST_WORD_SIZE) {
	  /* Start decoding the function looking for rtsd */
	  insn = get_insn (addr);
	  op = microblaze_decode_insn (insn, &rd, &ra, &rb, &imm);

	  /* Check for return. */
	  if (IS_RETURN(op)) {
		  offset = imm;
		  break; 
	  }
  }

  if (PC_IN_CALL_DUMMY (fi->pc, fi->frame, fi->frame))
    generic_pop_dummy_frame ();
  else
    {
      /* Write out the PC we saved. */
      write_register (PC_REGNUM, FRAME_SAVED_PC (fi) + offset);

      /* Restore any saved registers. */
      for (rn = 0; rn < NUM_REGS; rn++)
	{
	  if (fi->saved_regs[rn] != 0)
	    {
	      ULONGEST value;

	      value = read_memory_unsigned_integer (fi->saved_regs[rn],
						    REGISTER_SIZE);
	      write_register (rn, value);
	    }
	}

      /* Actually cut back the stack. */
      write_register (SP_REGNUM, FRAME_FP (fi));
    }

  /* Finally, throw away any cached frame information. */
  flush_cached_frames ();
}

/* Setup arguments and PR for a call to the target. First six arguments
   go in FIRST_ARGREG -> LAST_ARGREG, subsequent args go on to the stack.

   * If the function returns a struct, we must allocate for that, too. Gdb will tell
   us where this buffer is (STRUCT_ADDR), and we simply place it into
   FIRST_ARGREG, since the microblaze treats struct returns as hidden first arguments. */

CORE_ADDR
microblaze_push_arguments (int nargs, struct value ** args, CORE_ADDR sp,
		      unsigned char struct_return, CORE_ADDR struct_addr)
{
  int argreg;
  int argnum;
  struct stack_arg
    {
      int len;
      char *val;
    }
   *stack_args;
  char buf[REGISTER_SIZE];
  int nstack_args = 0;
  CORE_ADDR oldsp = sp;
 

  stack_args = (struct stack_arg *) alloca (nargs * sizeof (struct stack_arg));

  argreg = FIRST_ARGREG;

  /* Align the stack. This is mostly a nop, but not always. It will be needed
     if we call a function which has argument overflow. */
  sp &= ~3;

  /* If this function returns a struct, we must pass a buffer to the function
     which it can use to save the return value. */
  if (struct_return)
    write_register (argreg++, struct_addr);

  /* FIXME: what about unions? */
  for (argnum = 0; argnum < nargs; argnum++)
    {
      char *val = (char *) VALUE_CONTENTS (args[argnum]);
      int len = TYPE_LENGTH (VALUE_TYPE (args[argnum]));
      struct type *type = VALUE_TYPE (args[argnum]);
      int olen;

      microblaze_insn_debug (("MICROBLAZE PUSH: argreg=%d; len=%d; %s\n",
			 argreg, len, TYPE_CODE (type) == TYPE_CODE_STRUCT ? "struct" : "not struct"));

      olen = len;

      if (TYPE_CODE (type) == TYPE_CODE_STRUCT && olen < REGISTER_SIZE)
	{
	  /* Small structs must be right aligned within the register,
	     the most significant bits are undefined. */
	  write_register (argreg, extract_unsigned_integer (val, len));
	  argreg++;
	  len = 0;
	}

      while (len > 0 && argreg <= LAST_ARGREG)
	{
	  write_register (argreg, extract_unsigned_integer (val, REGISTER_SIZE));
	  argreg++;
	  val += REGISTER_SIZE;
	  len -= REGISTER_SIZE;
	}

      if (len > 0)
	{
	  /* Note that this must be saved onto the stack */
	  microblaze_insn_debug (("MICROBLAZE PUSH: adding arg %d to stack\n", argnum));
	  stack_args[nstack_args].val = val;
	  stack_args[nstack_args].len = len;
	  nstack_args++;
	}
    }

  /* Set up the stack - leave space for register function parameters (LAST_ARGREG-FIRST_ARGREG+1)
  sp -= (LAST_ARGREG-FIRST_ARGREG + 1) * REGISTER_SIZE;

  /* We're done with registers and stack allocation. Now do the actual
     stack pushes. */
  while (nstack_args--)
    {
      sp -= stack_args[nstack_args].len;
      write_memory (sp, stack_args[nstack_args].val, stack_args[nstack_args].len);
      sp &= ~3;
    }

  /* Set up the stack - leave space for register function parameters (LAST_ARGREG-FIRST_ARGREG+1) */
  sp -= (LAST_ARGREG-FIRST_ARGREG + 0) * REGISTER_SIZE;

  /* Now leave space for link register and back pointer */
  sp -= REGISTER_SIZE;
  store_signed_integer(buf, REGISTER_SIZE, read_register(PR_REGNUM));
  write_memory(sp, buf, REGISTER_SIZE);
  sp -= REGISTER_SIZE;
  store_signed_integer(buf, REGISTER_SIZE, oldsp);
  write_memory(sp, buf, REGISTER_SIZE);
  
  /* Return adjusted stack pointer.  */
  return sp;
}

/* Store the return address for the call dummy. For microblaze, we've
   opted to use generic call dummies, so we simply store the
   CALL_DUMMY_ADDRESS into the PR register (r15). */

CORE_ADDR
microblaze_push_return_address (CORE_ADDR pc, CORE_ADDR sp)
{
  write_register (PR_REGNUM, CALL_DUMMY_ADDRESS ());
  return sp;
}

/* Setting/getting return values from functions.

   The Xilinx processors use r3/r4 to return non-structures.
   All structures go into a caller-
   supplied buffer, which is passed in via a hidden first
   argument.

   For gdb, this leaves us two routes, based on what
   USE_STRUCT_CONVENTION (microblaze_use_struct_convention) returns.
   If this macro returns 1, gdb will call STORE_STRUCT_RETURN and
   EXTRACT_STRUCT_VALUE_ADDRESS.

   If USE_STRUCT_CONVENTION retruns 0, then gdb uses STORE_RETURN_VALUE
   and EXTRACT_RETURN_VALUE to store/fetch the functions return value. */

/* Should we use EXTRACT_STRUCT_VALUE_ADDRESS instead of
   EXTRACT_RETURN_VALUE?  GCC_P is true if compiled with gcc
   and TYPE is the type (which is known to be struct, union or array). */

int
microblaze_use_struct_convention (int gcc_p, struct type *type)
{
  /* All structures are always returned on stack */
  return 1;
}

/* Where is the return value saved? For microblaze, a pointer to 
   this buffer was passed as a hidden first argument, so
   just return that address. */

CORE_ADDR
microblaze_extract_struct_value_address (char *regbuf)
{
  return extract_address (regbuf + REGISTER_BYTE (FIRST_ARGREG), REGISTER_SIZE);
}

/* Given a function which returns a value of type TYPE, extract the
   the function's return value and place the result into VALBUF.
   REGBUF is the register contents of the target. */

void
microblaze_extract_return_value (struct type *type, char *regbuf, char *valbuf)
{
  /* Copy the return value (starting) in RETVAL_REGNUM to VALBUF. */
  char *reg = regbuf + REGISTER_BYTE(RETVAL_REGNUM);
  switch (TYPE_LENGTH(type)) {
	  case 1:	/* return last byte in the register */
		  memcpy(valbuf, reg + REGISTER_SIZE - 1, 1);
		  return;
	  case 2:	/* return last 2 bytes in register */
		  memcpy(valbuf, reg + REGISTER_SIZE - 2, 2);
		  return;
	  case 4:	/* for sizes 4 or 8, copy the required length */
	  case 8:
		  memcpy (valbuf, reg, TYPE_LENGTH (type));
		  return;
	  default:
		  printf_filtered("Fatal error: unsupported return value size requested (%s @ %d)\n", 
				  __FILE__, __LINE__);
  }
}

/* Store the return value in VALBUF (of type TYPE) where the caller
   expects to see it.

   Integers are stored in r3.

   Values less than 32 bits (short, boolean) are stored in r2, right justified and
   sign or zero extended.

   Longs are stored in r3 (most
   significant word) and r4 (least significant word).
   Small structures are always returned on stack.
*/

void
microblaze_store_return_value (struct type *type, char *valbuf)
{
  int value_size;
  int return_size;
  int offset;
  char *zeros;

  value_size = TYPE_LENGTH (type);

  /* Return value fits into registers. */
  return_size = (value_size + REGISTER_SIZE - 1) & ~(REGISTER_SIZE - 1);
  offset = REGISTER_BYTE (RETVAL_REGNUM) + (return_size - value_size);
  zeros = alloca (return_size);
  memset (zeros, 0, return_size);

  write_register_bytes (REGISTER_BYTE (RETVAL_REGNUM), zeros, return_size);
  write_register_bytes (offset, valbuf, value_size);
}

/* Initialize our target-dependent "stuff" for this newly created frame.

   This includes allocating space for saved registers and analyzing
   the prologue of this frame. */

void
microblaze_init_extra_frame_info (struct frame_info *fi)
{
  if (fi->next)
    fi->pc = FRAME_SAVED_PC (fi->next);

  frame_saved_regs_zalloc (fi);

  fi->extra_info = (struct frame_extra_info *)
    frame_obstack_alloc (sizeof (struct frame_extra_info));
  fi->extra_info->status = 0;
  fi->extra_info->framesize = 0;

  if (PC_IN_CALL_DUMMY (fi->pc, fi->frame, fi->frame))
    {
      /* We need to setup fi->frame here because run_stack_dummy gets it wrong
         by assuming it's always FP.  */
      fi->frame = generic_read_register_dummy (fi->pc, fi->frame, SP_REGNUM);
    }
  else
    microblaze_analyze_prologue (fi, 0);
}

/* Get an insturction from memory. */

static int
get_insn (CORE_ADDR pc)
{
  char buf[INST_WORD_SIZE];
  int status = read_memory_nobpt (pc, buf, INST_WORD_SIZE);
  if (status != 0)
    return 0;

  return extract_unsigned_integer (buf, INST_WORD_SIZE);
}

CORE_ADDR microblaze_saved_pc_after_call(call_pc)
int call_pc;
{
  int num_delay_slots;
  long inst = (long) get_insn(call_pc); /* get the instruction */
  num_delay_slots = get_delay_slots_microblaze( inst );
  /* Check if there was a delay slot */
  return call_pc + (INST_WORD_SIZE * (num_delay_slots+1));
}

void
_initialize_microblaze_tdep ()
{
  extern int print_insn_microblaze (bfd_vma, disassemble_info *);
  tm_print_insn = print_insn_microblaze;

#ifdef MICROBLAZE_DEBUG
  add_show_from_set (add_set_cmd ("microblazedebug", no_class,
				  var_boolean, (char *) &microblaze_debug,
				  "Set microblaze debugging.\n", &setlist),
		     &showlist);
#endif
}

/* The standard read_memory_integer does not work as expected - always returns 0 */
static CORE_ADDR
microblaze_read_memory_integer(addr)
CORE_ADDR addr;
{
  char buf[REGISTER_SIZE];
  read_memory (addr, buf, REGISTER_SIZE);
  return extract_signed_integer (buf, REGISTER_SIZE);
}

/* When gdb calls a function using a call dummy (e.g., when user specifies "call f"),
   the return address is supposed to be the entry_point_address.  However, we don't
   know where to put the momentary breakpoint - we have to look at the function code
   to see if it is rtsd r15,4 or rtsd r15, 8.  Hence inserting the breakpoint has to 
   be handled here, since run_stack_dummy does not do the right thing */
void
microblaze_fix_call_dummy( dummy, start_sp, funaddr, nargs, args, func_value_type, using_gcc) 
char* dummy;
CORE_ADDR start_sp;
CORE_ADDR funaddr;
int nargs;
struct value **args;
struct type * func_value_type;
int using_gcc;
{
  CORE_ADDR func_addr, func_end, addr, break_addr;
  char *name;
  int status;
  enum microblaze_instr op;
  int insn, rn, rd, ra, rb, imm;
  struct breakpoint *bpt;
  struct symtab_and_line sal;

  break_addr = CALL_DUMMY_ADDRESS();

  /* Find the start and end of this function. */
  status = find_pc_partial_function (funaddr, &name, &func_addr, &func_end);

  /* If the start of this function could not be found */
  /* If the debugger is entry function, give up. */
  if (status == 0 || func_addr == entry_point_address ())
    return;

  for (addr = func_addr; addr < func_end; addr += INST_WORD_SIZE)
    {
      /* Start decoding the function looking for rtsd */

      insn = get_insn (addr);
      op = microblaze_decode_insn (insn, &rd, &ra, &rb, &imm);

      /* Check for return. */
      if (IS_RETURN(op))
	{
	  break_addr += imm;
	  break; 
	}
    }

  /* Now put a breakpoint at this location - code got from run_stack_dummy */

  INIT_SAL (&sal);		/* initialize to zeroes */
  sal.pc = break_addr;
  sal.section = find_pc_overlay (sal.pc);

  /* Set up a FRAME for the dummy frame so we can pass it to
     set_momentary_breakpoint.  We need to give the breakpoint a
     frame in case there is only one copy of the dummy (e.g.
     CALL_DUMMY_LOCATION == AFTER_TEXT_END).  */
  flush_cached_frames ();
  set_current_frame (create_new_frame (read_fp (), sal.pc));

  bpt = set_momentary_breakpoint (sal,
				  get_current_frame (),
				  bp_call_dummy);
  bpt->disposition = disp_del; 

  /* If all error()s out of proceed ended up calling normal_stop (and
     perhaps they should; it already does in the special case of error
     out of resume()), then we wouldn't need this.  */
  /*make_cleanup (breakpoint_auto_delete_contents, &stop_bpstat);*/

  return;
}

/* Simulate single-step calls, since microblaze does not support 
   single-stepping */

/* single_step() is called just before we want to resume the inferior,
   if we want to single-step it but there is no hardware or kernel single-step
   support.  We find all the possible targets of the
   coming instruction and breakpoint them.

   single_step is also called just after the inferior stops.  If we had
   set up a simulated single-step, we undo our damage.  */

void
microblaze_software_single_step (ignore, insert_breakpoints_p)
     unsigned int ignore;	/* pid, but we don't need it */
     int insert_breakpoints_p;
{
  static char le_breakp[] = LITTLE_BREAKPOINT;
  static char be_breakp[] = BIG_BREAKPOINT;
  char *breakp = TARGET_BYTE_ORDER == BFD_ENDIAN_BIG ? be_breakp : le_breakp;

  /* Save the address and the values of the next_pc and the target */
  static struct sstep_breaks
  {
    CORE_ADDR address;
    boolean valid;
    /* Shadow contents.  */
    char data[INST_WORD_SIZE];
  } stepbreaks[2];
  int ii;

  if (insert_breakpoints_p)
    {
      CORE_ADDR pc;
      long insn;
      enum microblaze_instr minstr;
      boolean isunsignednum;
      enum microblaze_instr_type insn_type;
      short delay_slots;
      int imm;
      boolean immfound = false;

      /* Set a breakpoint at the next instruction */
      /* If the current instruction is an imm, set it at the inst after */
      /* If the instruction has a delay slot, skip the delay slot */
      pc = read_register(PC_REGNUM);
      insn = get_insn (pc);
      minstr = get_insn_microblaze (insn, &isunsignednum, &insn_type, &delay_slots);
      if (insn_type == immediate_inst) 
	{
	  int rd, ra, rb;
	  immfound = true;
	  minstr = microblaze_decode_insn (insn, &rd, &ra, &rb, &imm);
	  pc = pc + INST_WORD_SIZE;
	  insn = get_insn (pc);
	  minstr = get_insn_microblaze (insn, &isunsignednum, &insn_type, &delay_slots);
	}
      stepbreaks[0].address = pc + (delay_slots * INST_WORD_SIZE) + INST_WORD_SIZE;
      if (insn_type != return_inst) {
	stepbreaks[0].valid = true;
      } else {
	stepbreaks[0].valid = false;
      }

      /* Now check for branch or return instructions */
      if (insn_type == branch_inst || insn_type == return_inst) {
	int limm;
	int lrd, lra, lrb;
	int ra, rb;
	boolean targetvalid;
	boolean unconditionalbranch;
	microblaze_decode_insn(insn, &lrd, &lra, &lrb, &limm);
	if (lra >= 0 && lra < NUM_REGS)
	  ra = read_register(lra);
	else
	  ra = 0;
	if (lrb >= 0 && lrb < NUM_REGS)
	  rb = read_register(lrb);
	else
	  rb = 0;
	stepbreaks[1].address = microblaze_get_target_address (insn, immfound, imm, pc, ra, rb, &targetvalid, &unconditionalbranch);
	if (unconditionalbranch)
	  stepbreaks[0].valid = false; /* This is a unconditional branch: will not come to the next address */
	if (targetvalid && (stepbreaks[0].valid == false || 
			    (stepbreaks[0].address != stepbreaks[1].address))
	                && (stepbreaks[1].address != pc)) {
	  stepbreaks[1].valid = true;
	} else {
	  stepbreaks[1].valid = false;
	}
      } else {
	stepbreaks[1].valid = false;
      }

      /* Insert the breakpoints */
      for (ii = 0; ii < 2; ++ii)
        {

          /* ignore invalid breakpoint. */
          if (stepbreaks[ii].valid) {
	    read_memory (stepbreaks[ii].address, stepbreaks[ii].data, INST_WORD_SIZE);
	    write_memory (stepbreaks[ii].address, breakp, INST_WORD_SIZE);
	  }
	}

    } else {
      /* Remove the breakpoints */
      for (ii = 0; ii < 2; ++ii)
        {
          /* ignore invalid breakpoint. */
          if (stepbreaks[ii].valid) {
	    write_memory (stepbreaks[ii].address, stepbreaks[ii].data, INST_WORD_SIZE);
	  }
	}
    }
}

#include "top.h"

void microblaze_before_main_loop() {
  /* PATH to ENV variable XILINX_EDK */
  char* xilinx_edk_dir;
  /* String to hold the path to mb-gdb's custom init file 
     $XILINX_EDK/data/mb-gdb.ini */
  char* mb_gdb_init;
  char mbgdbinit_filename[] = "/data/mb-gdb.ini";
  
#ifdef __CYGWIN__
  {
    char *tmp = getenv ("XILINX_EDK");

    if (tmp != NULL)
      {
	xilinx_edk_dir = (char *) alloca (PATH_MAX + 1);
	cygwin32_conv_to_posix_path (tmp, xilinx_edk_dir);
      }
    else
      xilinx_edk_dir = NULL;
  }
#else
  xilinx_edk_dir = getenv ("XILINX_EDK");
#endif
  if (xilinx_edk_dir)
    {
      mb_gdb_init = (char *) alloca (strlen (xilinx_edk_dir) +
				     strlen (mbgdbinit_filename) + 10);
      strcpy (mb_gdb_init, xilinx_edk_dir);
      strcat (mb_gdb_init, "/");
      strcat (mb_gdb_init, mbgdbinit_filename);

      if (!inhibit_gdbinit)
	{
	  catch_command_errors (source_command, mb_gdb_init, 0, RETURN_MASK_ALL);
	}
    }
}

int microblaze_can_use_hardware_watchpoints (enum bptype type, int len, int ot) {
    int count = 0;
    switch (type) {
        case bp_hardware_breakpoint:
            count = 2;
            break;
        case bp_read_watchpoint:
            count = 1;
            break;
        case bp_hardware_watchpoint:
            count = 1;
            break;
        default:
            count = 0;
    }
    return count;
}

