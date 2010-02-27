/* Definitions of target machine for GNU compiler.  MicroBlaze version.
   Copyright (C) 1989, 90-98, 1999 Free Software Foundation, Inc.
   This file is part of GNU CC.

   GNU CC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU CC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU CC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/*
 *
 * Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
 * 
 * microblaze.h
 * 
 * MicroBlaze specific file. Contains functions for generating MicroBlaze code. 
 * Certain lines of code are from Free Software Foundation
 * 
 * $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/gcc/src-3.4/gcc/config/microblaze/microblaze.h,v 1.25 2005/12/01 03:57:39 vasanth Exp $
 * 
 */

/* Standard GCC variables that we reference.  */

/* MICROBLAZE external variables defined in microblaze.c.  */

/* comparison type */
enum cmp_type {
    CMP_SI,				/* compare four byte integers */
    CMP_DI,				/* compare eight byte integers */
    CMP_SF,				/* compare single precision floats */
    CMP_DF,				/* compare double precision floats */
    CMP_MAX				/* max comparison type */
};

/* types of delay slot */
enum delay_type {
    DELAY_NONE,				/* no delay slot */
    DELAY_LOAD,				/* load from memory delay */
    DELAY_HILO,				/* move from/to hi/lo registers */
    DELAY_FCMP				/* delay after doing c.<xx>.{d,s} */
};

/* Which pipeline to schedule for. */
enum pipeline_type {
    MICROBLAZE_PIPE_3 = 0
};




/* Which ABI to use.  */

#define ABI_32  0

#define microblaze_abi ABI_32

/* Whether to emit abicalls code sequences or not.  */

enum microblaze_abicalls_type {
    MICROBLAZE_ABICALLS_NO,
    MICROBLAZE_ABICALLS_YES
};

/* Recast the abicalls class to be the abicalls attribute.  */
#define microblaze_abicalls_attr ((enum attr_abicalls)microblaze_abicalls)

/* Which type of block move to do (whether or not the last store is
   split out so it can fill a branch delay slot).  */

enum block_move_type {
    BLOCK_MOVE_NORMAL,			/* generate complete block move */
    BLOCK_MOVE_NOT_LAST,			/* generate all but last store */
    BLOCK_MOVE_LAST			/* generate just the last store */
};

/* microblaze_cpu_select contains CPU info */
struct microblaze_cpu_select
{
    const char *cpu;
    const char *tune;
    unsigned int flags;
};

#define MICROBLAZE_MASK_NO_DANGEROUS_DELAY              0x00000001

extern char microblaze_reg_names[][8];          /* register names (a0 vs. $4). */
extern char microblaze_print_operand_punct[];	/* print_operand punctuation chars */
extern const char *current_function_file;       /* filename current function is in */
extern int num_source_filenames;                /* current .file # */
extern int inside_function;                     /* != 0 if inside of a function */
extern int ignore_line_number;                  /* != 0 if we are to ignore next .loc */
extern int file_in_function_warning;            /* warning given about .file in func */
extern int sdb_label_count;                     /* block start/end next label # */
extern int sdb_begin_function_line;             /* Starting Line of current function */
extern int microblaze_section_threshold;	/* # bytes of data/sdata cutoff */
/* extern unsigned int g_switch_value;             /\* value of the -G xx switch *\/ */
/* extern int g_switch_set;                        /\* whether -G xx was passed.  *\/ */
extern int sym_lineno;                          /* sgi next label # for each stmt */
extern int set_noreorder;                       /* # of nested .set noreorder's  */
extern int set_nomacro;                         /* # of nested .set nomacro's  */
extern int set_noat;                            /* # of nested .set noat's  */
extern int set_volatile;                        /* # of nested .set volatile's  */
extern int microblaze_dbx_regno[];		/* Map register # to debug register # */
extern struct rtx_def *branch_cmp[2];           /* operands for compare */
extern enum microblaze_abicalls_type microblaze_abicalls;/* for svr4 abi pic calls */
extern int microblaze_isa;			/* architectural level */
extern int dslots_load_total;                   /* total # load related delay slots */
extern int dslots_load_filled;                  /* # filled load delay slots */
extern int dslots_jump_total;                   /* total # jump related delay slots */
extern int dslots_jump_filled;                  /* # filled jump delay slots */
extern int dslots_number_nops;                  /* # of nops needed by previous insn */
extern int num_refs[3];                         /* # 1/2/3 word references */
extern struct rtx_def *microblaze_load_reg;	/* register to check for load delay */
extern struct rtx_def *microblaze_load_reg2;	/* 2nd reg to check for load delay */
extern struct rtx_def *microblaze_load_reg3;	/* 3rd reg to check for load delay */
extern struct rtx_def *microblaze_load_reg4;	/* 4th reg to check for load delay */
extern struct rtx_def *embedded_pic_fnaddr_rtx;	/* function address */
extern const char       *asm_file_name;
extern char             call_used_regs[];
extern int              current_function_calls_alloca;
extern char             *language_string;
extern int              may_call_alloca;
extern int              target_flags;
extern struct microblaze_cpu_select 
                        microblaze_select;
extern int microblaze_no_dangerous_delay;
extern enum pipeline_type microblaze_pipe;
extern enum cmp_type branch_type;
extern char *microblaze_no_clearbss;

/*extern char    *version_string;*/

/* Functions within microblaze.c that we reference.  Some of these return
   type HOST_WIDE_INT, so define that here.  */

#include "hwint.h"

/* This is the only format we support */
#define OBJECT_FORMAT_ELF

/* Stubs for half-pic support if not OSF/1 reference platform.  */

#ifndef HALF_PIC_P
#define HALF_PIC_P() 0
#define HALF_PIC_NUMBER_PTRS 0
#define HALF_PIC_NUMBER_REFS 0
#define HALF_PIC_ENCODE(DECL)
#define HALF_PIC_DECLARE(NAME)
#define HALF_PIC_INIT()	error ("half-pic init called on systems that don't support it.")
#define HALF_PIC_ADDRESS_P(X) 0
#define HALF_PIC_PTR(X) X
#define HALF_PIC_FINISH(STREAM)
#endif


/* Run-time compilation parameters selecting different hardware subsets.  */

/* Macros used in the machine description to test the flags.  */

/* Bits for real switches */
#define MASK_SOFT_DIV           0x00000002      /* Mask for Soft Divide                         */
#define MASK_MEMCPY             0x00000008      /* Call memcpy instead of inline code           */
#define MASK_GAS                0x00000010	/* Gas used instead of MICROBLAZE as            */
#define MASK_EMPTY1             0x00000020	/* Empty 1 */
#define MASK_STATS              0x00000040	/* print statistics to stderr                   */
#define MASK_PATTERN_COMPARE    0x00000080      /* Use extended pattern compare instructions    */
#define MASK_SOFT_FLOAT         0x00000100	/* software floating point                      */
#define MASK_STACK_CHECK        0x00000200      /* Do Stack checking                            */ 
#define MASK_ABICALLS           0x00000400	/* emit .abicalls/.cprestore/.cpload            */
#define MASK_HALF_PIC           0x00000800	/* Emit OSF-style pic refs to externs           */
#define MASK_LONG_CALLS         0x00001000	/* Always call through a register               */
#define MASK_BARREL_SHIFT       0x00002000      /* Use a barrel shifter as this is being provided with MicroBlaze */
#define MASK_EMBEDDED_PIC       0x00004000	/* Generate embedded PIC code                   */
#define MASK_EMBEDDED_DATA      0x00008000	/* Reduce RAM usage, not fast code              */
#define MASK_BIG_ENDIAN         0x00010000	/* Generate big endian code                     */
#define MASK_XLGPOPT            0x00020000	/* Optimize for Xilinx global pointer           */ 
#define MASK_SOFT_MUL           0x00040000      /* Use software multiply instead of hardware    */
#define MASK_SMALL_DIVIDES      0x10000000      /* Use table lookup for divides.                */
                                                /* Dummy switches used only in spec's           */
/* Debug & special switches for internal testing. Not documented  */
#define MASK_DEBUG              0                       /* Eliminate version # in .s file               */
#define MASK_DEBUG_A            0x40000000              /* don't allow <label>($reg) addrs              */
#define MASK_DEBUG_B            0x20000000              /* GO_IF_LEGITIMATE_ADDRESS debug               */
#define MASK_DEBUG_D            0                       /* don't do define_split's                      */
#define MASK_DEBUG_E            0                       /* function_arg debug                           */
#define MASK_DEBUG_F            0
#define MASK_DEBUG_G            0                       /* don't support 64 bit arithmetic              */
#define MASK_DEBUG_H            0                       /* allow ints in FP registers                   */
#define MASK_DEBUG_I            0                       /* unused                                       */


#define TARGET_GAS		(target_flags & MASK_GAS)
#define TARGET_UNIX_ASM		(!TARGET_GAS)
#define TARGET_MICROBLAZE_AS		TARGET_UNIX_ASM
/*#define TARGET_MICROBLAZE_ASM         (target_flags & MASK_LCC_ASM)  */
#define TARGET_MICROBLAZE_ASM   0
#define TARGET_STACK_CHECK      (target_flags & MASK_STACK_CHECK)

/* Debug Mode */
#define TARGET_DEBUG_MODE               (target_flags & MASK_DEBUG)
#define TARGET_DEBUG_A_MODE             (target_flags & MASK_DEBUG_A)
#define TARGET_DEBUG_B_MODE             (target_flags & MASK_DEBUG_B)
#define TARGET_DEBUG_D_MODE             (target_flags & MASK_DEBUG_D)
#define TARGET_DEBUG_E_MODE             (target_flags & MASK_DEBUG_E)
#define TARGET_DEBUG_F_MODE             (target_flags & MASK_DEBUG_F)
#define TARGET_DEBUG_G_MODE             (target_flags & MASK_DEBUG_G)
#define TARGET_DEBUG_H_MODE             (target_flags & MASK_DEBUG_H)
#define TARGET_DEBUG_I_MODE             (target_flags & MASK_DEBUG_I)

#define TARGET_GP_OPT            0       /* Vasanth: Cleanup */

/* call memcpy instead of inline code */
#define TARGET_MEMCPY		(target_flags & MASK_MEMCPY)

/* Optimize for Sdata/Sbss */
#define TARGET_XLGP_OPT		(target_flags & MASK_XLGPOPT)
/* print program statistics */
#define TARGET_STATS		(target_flags & MASK_STATS)

/* .abicalls, etc from Pyramid V.4 */
#define TARGET_ABICALLS		(target_flags & MASK_ABICALLS)

/* OSF pic references to externs */
#define TARGET_HALF_PIC		(target_flags & MASK_HALF_PIC)

/* Use software floating point routines */
#define TARGET_SOFT_FLOAT	(target_flags & MASK_SOFT_FLOAT)

/* Use hardware FPU instructions */
#define TARGET_HARD_FLOAT       (!TARGET_SOFT_FLOAT)

/* always call through a register */
#define TARGET_LONG_CALLS	(target_flags & MASK_LONG_CALLS)

/* generate embedded PIC code;
   requires gas.  */
#define TARGET_EMBEDDED_PIC	(target_flags & MASK_EMBEDDED_PIC)

/* For embedded systems, optimize for reduced RAM space instead of for
   fastest code.  */
#define TARGET_EMBEDDED_DATA	(target_flags & MASK_EMBEDDED_DATA)

/* Generate big endian code.  */
#define TARGET_BIG_ENDIAN	(target_flags & MASK_BIG_ENDIAN)

/* Use software multiply routines */
#define TARGET_SOFT_MUL         (target_flags & MASK_SOFT_MUL)

/* Use software divide routines */
#define TARGET_SOFT_DIV         (target_flags & MASK_SOFT_DIV)

/* Use hardware barrel shifter */
#define TARGET_BARREL_SHIFT     (target_flags & MASK_BARREL_SHIFT)

/* Use extended compare instructions */
#define TARGET_PATTERN_COMPARE  (target_flags & MASK_PATTERN_COMPARE)

#define TARGET_SMALL_DIVIDES    (target_flags & MASK_SMALL_DIVIDES)

/* This is true if we must enable the assembly language file switching
   code.  */
/* [Changed to False for microblaze {04/17/02}]
   #define TARGET_FILE_SWITCHING	(TARGET_GP_OPT && ! TARGET_GAS)*/
#define TARGET_FILE_SWITCHING	0

/* We must disable the function end stabs when doing the file switching trick,
   because the Lscope stabs end up in the wrong place, making it impossible
   to debug the resulting code.  */
#define NO_DBX_FUNCTION_END TARGET_FILE_SWITCHING

/* Added by Sid for mb-objdump problem */
#define DBX_FUNCTION_FIRST 1
#define DBX_BLOCKS_FUNCTION_RELATIVE 1

/* Just to preserve old code */
#define TARGET_SINGLE_FLOAT             0

/* This table intercepts weirdo options whose names would interfere
   with normal driver conventions, and either translates them into
   standardly-named options, or adds a 'Z' so that they can get to
   specs processing without interference.

   Do not expand a linker option to "-Xlinker -<option>", since that
   forfeits the ability to control via spec strings later.  However,
   as a special exception, do this translation with -filelist, because
   otherwise the driver will think there are no input files and quit.
   (The alternative would be to hack the driver to recognize -filelist
   specially, but it's simpler to use the translation table.)

   Note that an option name with a prefix that matches another option
   name, that also takes an argument, needs to be modified so the
   prefix is different, otherwise a '*' after the shorter option will
   match with the longer one.  */
#define TARGET_OPTION_TRANSLATE_TABLE \
  { "-xl-mode-executable", "-Zxl-mode-executable" }, \
  { "-xl-mode-xmdstub", "-Zxl-mode-xmdstub" },  \
  { "-xl-mode-bootstrap", "-Zxl-mode-bootstrap" }, \
  { "-xl-mode-novectors", "-Zxl-mode-novectors" }, \
  { "-xl-mode-xilkernel", "-Zxl-mode-xilkernel" },  \
  { "-xl-blazeit", "-Zxl-blazeit" },    \
  { "-xl-no-libxil", "-Zxl-no-libxil" }


/* Macro to define tables used to set the flags.
   This is a list in braces of pairs in braces,
   each pair being { "NAME", VALUE }
   where VALUE is the bits to set or minus the bits to clear.
   An empty string NAME is used to identify the default VALUE.  */

#define TARGET_SWITCHES							\
{									\
  {"xl-soft-mul",	  MASK_SOFT_MUL,				\
     "Use the soft multiply emulation"},				\
  {"no-xl-soft-mul",	  -MASK_SOFT_MUL,				\
     "Use the hardware multiplier instead of emulation"},		\
  {"xl-soft-div",	  MASK_SOFT_DIV,				\
     "Use the soft divide emulation"},                                  \
  {"no-xl-soft-div",	  -MASK_SOFT_DIV,				\
     "Use the hardware divider instead of emulation"},	         	\
  {"xl-barrel-shift",	  MASK_BARREL_SHIFT,				\
     "Use the hardware barrel shifter instead of emulation"},           \
  {"soft-float",	  MASK_SOFT_FLOAT,				\
     "Use software floating point"},					\
  {"hard-float",          -MASK_SOFT_FLOAT,                             \
     "Don't use software floating point"},                              \
  {"xl-pattern-compare",  MASK_PATTERN_COMPARE,                         \
     "Use pattern compare instructions"},                               \
  {"small-divides",	  MASK_SMALL_DIVIDES,				\
     "Use table lookup optimization for small signed integer divisions"},\
  {"xl-stack-check",	  MASK_STACK_CHECK,				\
     "Check Stack at runtime"},						\
  {"memcpy",		  MASK_MEMCPY,					\
     "Don't optimize block moves"},					\
  {"no-memcpy",		 -MASK_MEMCPY,					\
     "Optimize block moves"},						\
  {"xl-gp-opt",		 MASK_XLGPOPT,					\
     "Use GP relative sdata/sbss sections[for xlnx]"},			\
  {"no-xl-gp-opt",	 -MASK_XLGPOPT,			 	        \
     "Use GP relative sdata/sbss sections[for xlnx]"},			\
  {"stats",		  MASK_STATS,					\
     "Output compiler statistics"},					\
  {"no-stats",		 -MASK_STATS,					\
     "Don't output compiler statistics"},				\
  {"debug",		  MASK_DEBUG,					\
     NULL},								\
  {"debuga",		  MASK_DEBUG_A,					\
     NULL},								\
  {"debugb",		  MASK_DEBUG_B,					\
     NULL},								\
  {"debugd",		  MASK_DEBUG_D,					\
     NULL},								\
  {"debuge",		  MASK_DEBUG_E,					\
     NULL},								\
  {"debugf",		  MASK_DEBUG_F,					\
     NULL},								\
  {"debugg",		  MASK_DEBUG_G,					\
     NULL},								\
  {"debugh",		  MASK_DEBUG_H,					\
     NULL},								\
  {"debugi",		  MASK_DEBUG_I,					\
     NULL},								\
  {"",			  (TARGET_DEFAULT				\
			   | TARGET_CPU_DEFAULT				\
			   | TARGET_ENDIAN_DEFAULT),			\
     NULL},								\
}     

/* Default target_flags if no switches are specified  */
#define TARGET_DEFAULT      (0)

#ifndef TARGET_CPU_DEFAULT
#define TARGET_CPU_DEFAULT 0
#endif

#ifndef TARGET_ENDIAN_DEFAULT
#define TARGET_ENDIAN_DEFAULT MASK_BIG_ENDIAN
#endif

#ifndef MULTILIB_DEFAULTS
#if TARGET_ENDIAN_DEFAULT == 0
#define MULTILIB_DEFAULTS { "EL", "microblaze" }
#else
#define MULTILIB_DEFAULTS { "EB", "microblaze" }
#endif
#endif

/* What is the default setting for -mcpu= */
#define MICROBLAZE_DEFAULT_CPU      "v4.00.a"

/* We must pass -EL to the linker by default for little endian embedded
   targets using linker scripts with a OUTPUT_FORMAT line.  Otherwise, the
   linker will default to using big-endian output files.  The OUTPUT_FORMAT
   line must be in the linker script, otherwise -EB/-EL will not work.  */

#ifndef LINKER_ENDIAN_SPEC
#if TARGET_ENDIAN_DEFAULT == 0
#define LINKER_ENDIAN_SPEC "%{!EB:%{!meb:-EL}}"
#else
#define LINKER_ENDIAN_SPEC ""
#endif
#endif

/* This macro is similar to `TARGET_SWITCHES' but defines names of
   command options that have values.  Its definition is an
   initializer with a subgrouping for each command option.

   Each subgrouping contains a string constant, that defines the
   fixed part of the option name, and the address of a variable. 
   The variable, type `char *', is set to the variable part of the
   given option if the fixed part matches.  The actual option name
   is made by appending `-m' to the specified name.

   Here is an example which defines `-mshort-data-NUMBER'.  If the
   given option is `-mshort-data-512', the variable `m88k_short_data'
   will be set to the string `"512"'.

   extern char *m88k_short_data;
   #define TARGET_OPTIONS { { "short-data-", &m88k_short_data } }  */

#define TARGET_OPTIONS                                                                  \
{                                                                                       \
  {"cpu=",  &microblaze_select.cpu,                                                     \
     N_("Use features of and schedule code for given CPU"), NULL},                      \
  {"tune=", &microblaze_select.tune,                                                    \
     N_("Schedule code for given CPU"), NULL},                                          \
  {"no-clearbss", &microblaze_no_clearbss,                                              \
     N_("Do not clear the BSS to zero and do not place zero initialized in BSS"), "yes"}\
}

/* Macros to decide whether certain features are available or not,
   depending on the instruction set architecture level.  */

#define HAVE_SQRT_P()		0

/* 
   The gen* programs link code that refers to MASK_64BIT.  They don't
   actually use the information in target_flags; they just refer to
   it.  */

/* Switch  Recognition by gcc.c.  Add -G xx support */

#ifdef SWITCH_TAKES_ARG
#undef SWITCH_TAKES_ARG
#endif

#define SWITCH_TAKES_ARG(CHAR)						\
  (DEFAULT_SWITCH_TAKES_ARG (CHAR) || (CHAR) == 'G')

/* Sometimes certain combinations of command options do not make sense
   on a particular target machine.  You can define a macro
   `OVERRIDE_OPTIONS' to take account of this.  This macro, if
   defined, is executed once just after all the command options have
   been parsed.

   On the MICROBLAZE, it is used to handle -G.  We also use it to set up all
   of the tables referenced in the other macros.  */

#define OVERRIDE_OPTIONS override_options ()

/* Zero or more C statements that may conditionally modify two
   variables `fixed_regs' and `call_used_regs' (both of type `char
   []') after they have been initialized from the two preceding
   macros.

   This is necessary in case the fixed or call-clobbered registers
   depend on target flags.

   You need not define this macro if it has no work to do.

   If the usage of an entire class of registers depends on the target
   flags, you may indicate this to GCC by using this macro to modify
   `fixed_regs' and `call_used_regs' to 1 for each of the registers in
   the classes which should not be used by GCC.  Also define the macro
   `REG_CLASS_FROM_LETTER' to return `NO_REGS' if it is called with a
   letter for a class that shouldn't be used.

   (However, if this class is not included in `GENERAL_REGS' and all
   of the insn patterns whose constraints permit this class are
   controlled by target switches, then GCC will automatically avoid
   using these registers when the target switches are opposed to
   them.)  */

#define CONDITIONAL_REGISTER_USAGE					\
do									\
  {									\
	int regno;							\
	for (regno = FP_REG_FIRST; regno <= FP_REG_LAST; regno++)	\
	  fixed_regs[regno] = call_used_regs[regno] = 1;		\
	for (regno = ST_REG_FIRST; regno <= ST_REG_LAST; regno++)	\
	  fixed_regs[regno] = call_used_regs[regno] = 1;		\
    SUBTARGET_CONDITIONAL_REGISTER_USAGE				\
  }									\
while (0)

/* This is meant to be redefined in the host dependent files.  */
#define SUBTARGET_CONDITIONAL_REGISTER_USAGE

/* Show we can debug even without a frame pointer.  */
#define CAN_DEBUG_WITHOUT_FP

/* Complain about missing specs and predefines that should be defined in each
   of the target tm files to override the defaults.  This is mostly a place-
   holder until I can get each of the files updated [mm].  */

#if defined(OSF_OS) \
    || defined(MICROBLAZE_SYSV) \
    || defined(MICROBLAZE_SVR4) \
    || defined(MICROBLAZE_BSD43)

#ifndef STARTFILE_SPEC
#error "Define STARTFILE_SPEC in the appropriate tm.h file"
#endif

#ifndef MACHINE_TYPE
#error "Define MACHINE_TYPE in the appropriate tm.h file"
#endif
#endif

/* Tell collect what flags to pass to nm.  */
#ifndef NM_FLAGS
#define NM_FLAGS "-Bn"
#endif

/* Names to predefine in the preprocessor for this target machine.  */

/* Target CPU builtins.  */
#define TARGET_CPU_CPP_BUILTINS()				\
  do								\
    {								\
        builtin_define ("microblaze");                          \
        builtin_define ("_BIG_ENDIAN");                         \
        builtin_define ("__MICROBLAZE__");                      \
                                                                \
        builtin_assert ("system=unix");                         \
        builtin_assert ("system=bsd");                          \
        builtin_assert ("cpu=microblaze");                      \
        builtin_assert ("machine=microblaze");                  \
} while (0)

/* Assembler specs.  */

/* MICROBLAZE_AS_ASM_SPEC is passed when using the MICROBLAZE assembler rather
   than gas.  */

#define MICROBLAZE_AS_ASM_SPEC "\
%{!.s:-nocpp} %{.s: %{cpp} %{nocpp}} \
%{pipe: %e-pipe is not supported.} \
%{K} %(subtarget_microblaze_as_asm_spec)"

/* SUBTARGET_MICROBLAZE_AS_ASM_SPEC is passed when using the MICROBLAZE assembler
   rather than gas.  It may be overridden by subtargets.  */

#ifndef SUBTARGET_MICROBLAZE_AS_ASM_SPEC
#define SUBTARGET_MICROBLAZE_AS_ASM_SPEC "%{v}"
#endif

/* GAS_ASM_SPEC is passed when using gas, rather than the MICROBLAZE
   assembler.  */

#define GAS_ASM_SPEC "%{v}"

/* TARGET_ASM_SPEC is used to select either MICROBLAZE_AS_ASM_SPEC or
   GAS_ASM_SPEC as the default, depending upon the value of
   TARGET_DEFAULT.  */

#if ((TARGET_CPU_DEFAULT | TARGET_DEFAULT) & MASK_GAS) != 0
/* GAS */

#define TARGET_ASM_SPEC "\
%{mmicroblaze-as: %(microblaze_as_asm_spec)} \
%{!mmicroblaze-as: %(gas_asm_spec)}"

#else /* not GAS */

#define TARGET_ASM_SPEC ""
/*#define TARGET_ASM_SPEC "\
  %{!mgas: %(microblaze_as_asm_spec)} \
  %{mgas: %(gas_asm_spec)}"
*/
#endif /* not GAS */

/* SUBTARGET_ASM_OPTIMIZING_SPEC handles passing optimization options
   to the assembler.  It may be overridden by subtargets.  */
#ifndef SUBTARGET_ASM_OPTIMIZING_SPEC
#define SUBTARGET_ASM_OPTIMIZING_SPEC " "
#endif

/* SUBTARGET_ASM_DEBUGGING_SPEC handles passing debugging options to
   the assembler.  It may be overridden by subtargets.  */
#ifndef SUBTARGET_ASM_DEBUGGING_SPEC
#define SUBTARGET_ASM_DEBUGGING_SPEC "\
%{g} %{g0} %{g1} %{g2} %{g3} \
%{ggdb:-g} %{ggdb0:-g0} %{ggdb1:-g1} %{ggdb2:-g2} %{ggdb3:-g3} \
%{gstabs:-g} %{gstabs0:-g0} %{gstabs1:-g1} %{gstabs2:-g2} %{gstabs3:-g3} \
%{gstabs+:-g} %{gstabs+0:-g0} %{gstabs+1:-g1} %{gstabs+2:-g2} %{gstabs+3:-g3}"
/* Old Specs */
/*#define SUBTARGET_ASM_DEBUGGING_SPEC "\
  %{g} %{g0} %{g1} %{g2} %{g3} \
  %{ggdb:-g} %{ggdb0:-g0} %{ggdb1:-g1} %{ggdb2:-g2} %{ggdb3:-g3} \
  %{gstabs:-g} %{gstabs0:-g0} %{gstabs1:-g1} %{gstabs2:-g2} %{gstabs3:-g3} \
  %{gstabs+:-g} %{gstabs+0:-g0} %{gstabs+1:-g1} %{gstabs+2:-g2} %{gstabs+3:-g3} \
  %{gcoff:-g} %{gcoff0:-g0} %{gcoff1:-g1} %{gcoff2:-g2} %{gcoff3:-g3}"
*/
#endif


/* SUBTARGET_ASM_SPEC is always passed to the assembler.  It may be
   overridden by subtargets.  */

#ifndef SUBTARGET_ASM_SPEC
#define SUBTARGET_ASM_SPEC ""
#endif

/* ASM_SPEC is the set of arguments to pass to the assembler.  */

#define ASM_SPEC "\
%{microblaze1} \
%(target_asm_spec) \
%(subtarget_asm_spec)"

/* old asm spec */
/*#define ASM_SPEC "\
  %{G*} %{EB} %{EL} %{microblaze1} \
  %{membedded-pic} \
  %(target_asm_spec) \
  %(subtarget_asm_spec)"
*/

/* Specify to run a post-processor, microblaze-tfile after the assembler
   has run to stuff the microblaze debug information into the object file.
   This is needed because the $#!%^ MICROBLAZE assembler provides no way
   of specifying such information in the assembly file.  If we are
   cross compiling, disable microblaze-tfile unless the user specifies
   -mmicroblaze-tfile.  */

#ifndef ASM_FINAL_SPEC
#define ASM_FINAL_SPEC ""
#endif	/* ASM_FINAL_SPEC */

/* Extra switches sometimes passed to the linker.  */
/* ??? The bestGnum will never be passed to the linker, because the gcc driver
   will interpret it as a -b option.  */

#ifndef LINK_SPEC
/*#define LINK_SPEC "\
  %{G*} %{EB} %{EL} \
  %{bestGnum} %{shared} %{non_shared} \
  %(linker_endian_spec) -relax -N \
  %{intrusive-debug:-defsym _TEXT_START_ADDR=0x400}"
*/
#define LINK_SPEC "-relax -N %{Zxl-mode-xmdstub:-defsym _TEXT_START_ADDR=0x800}"
#endif	/* LINK_SPEC defined */

/* Specs for the compiler proper */

/* SUBTARGET_CC1_SPEC is passed to the compiler proper.  It may be
   overridden by subtargets.  */
#ifndef SUBTARGET_CC1_SPEC
#define SUBTARGET_CC1_SPEC ""
#endif

/* CC1_SPEC is the set of arguments to pass to the compiler proper.  */

#ifndef CC1_SPEC
#define CC1_SPEC "\
%{G*} %{gline:%{!g:%{!g0:%{!g1:%{!g2: -g1}}}}} \
%{save-temps: } \
%(subtarget_cc1_spec)\
%{Zxl-blazeit: %{!mxl-soft-mul: -mno-xl-soft-mul} %{!mxl-soft-div: -mno-xl-soft-div} -mxl-barrel-shift}\
%{!mno-xl-soft-mul: %{!Zxl-blazeit: -mxl-soft-mul}}\
%{!mno-xl-soft-div: %{!Zxl-blazeit: -mxl-soft-div}}\
%{!mhard-float: -msoft-float}\
"
#endif

/* Preprocessor specs.  */

/* SUBTARGET_CPP_SIZE_SPEC defines SIZE_TYPE and PTRDIFF_TYPE.  It may
   be overridden by subtargets.  */
/* GCC 3.4.1 
 * Removed
 */

#ifndef SUBTARGET_CPP_SIZE_SPEC
#define SUBTARGET_CPP_SIZE_SPEC "-D__SIZE_TYPE__=unsigned\\ int -D__PTRDIFF_TYPE__=int"
/*#define SUBTARGET_CPP_SIZE_SPEC ""*/
#endif


/* SUBTARGET_CPP_SPEC is passed to the preprocessor.  It may be
   overridden by subtargets.  */
#ifndef SUBTARGET_CPP_SPEC
#define SUBTARGET_CPP_SPEC ""
#endif

/* CPP_SPEC is the set of arguments to pass to the preprocessor.  */

#ifndef CPP_SPEC
#define CPP_SPEC "\
%{.S:	-D__LANGUAGE_ASSEMBLY -D_LANGUAGE_ASSEMBLY %{!ansi:-DLANGUAGE_ASSEMBLY}} \
%{.s:	-D__LANGUAGE_ASSEMBLY -D_LANGUAGE_ASSEMBLY %{!ansi:-DLANGUAGE_ASSEMBLY}} \
%{!.S: %{!.s: %{!.cc: %{!.cxx: %{!.C: %{!.m: -D__LANGUAGE_C -D_LANGUAGE_C %{!ansi:-DLANGUAGE_C}}}}}}} \
%{mno-xl-soft-mul: -DHAVE_HW_MUL}       \
%{mno-xl-soft-div: -DHAVE_HW_DIV}       \
%{mxl-barrel-shift: -DHAVE_HW_BSHIFT}   \
%{mxl-pattern-compare: -DHAVE_HW_PCMP}  \
%{mhard-float: -DHAVE_HW_FPU}           \
"
#endif

/* This macro defines names of additional specifications to put in the specs
   that can be used in various specifications like CC1_SPEC.  Its definition
   is an initializer with a subgrouping for each command option.

   Each subgrouping contains a string constant, that defines the
   specification name, and a string constant that used by the GNU CC driver
   program.

   Do not define this macro if it does not need to do anything.  */

#define EXTRA_SPECS							\
  { "subtarget_cc1_spec", SUBTARGET_CC1_SPEC },				\
  { "subtarget_cpp_spec", SUBTARGET_CPP_SPEC },				\
  { "subtarget_cpp_size_spec", SUBTARGET_CPP_SIZE_SPEC },		\
  { "microblaze_as_asm_spec", MICROBLAZE_AS_ASM_SPEC },				\
  { "gas_asm_spec", GAS_ASM_SPEC },					\
  { "target_asm_spec", TARGET_ASM_SPEC },				\
  { "subtarget_microblaze_as_asm_spec", SUBTARGET_MICROBLAZE_AS_ASM_SPEC }, 	\
  { "subtarget_asm_optimizing_spec", SUBTARGET_ASM_OPTIMIZING_SPEC },	\
  { "subtarget_asm_debugging_spec", SUBTARGET_ASM_DEBUGGING_SPEC },	\
  { "subtarget_asm_spec", SUBTARGET_ASM_SPEC },				\
  { "linker_endian_spec", LINKER_ENDIAN_SPEC },				\
  SUBTARGET_EXTRA_SPECS

/* If defined, this macro is an additional prefix to try after
   `STANDARD_EXEC_PREFIX'.  */

#ifndef MD_EXEC_PREFIX
#define MD_EXEC_PREFIX "/usr/lib/cmplrs/cc/"
#endif

#ifndef MD_STARTFILE_PREFIX
/*#define MD_STARTFILE_PREFIX "/usr/lib/cmplrs/cc/"*/
#define MD_STARTFILE_PREFIX "/home/sid/comp/tests/gcctest/"
#endif


/* Print subsidiary information on the compiler version in use.  */
#define MICROBLAZE_VERSION MICROBLAZE_DEFAULT_CPU

#ifndef MACHINE_TYPE
#define MACHINE_TYPE "MicroBlaze/ELF"
#endif

#ifndef TARGET_VERSION_INTERNAL
#define TARGET_VERSION_INTERNAL(STREAM)					\
  fprintf (STREAM, " %s %s", MACHINE_TYPE, MICROBLAZE_VERSION)
#endif

#ifndef TARGET_VERSION
#define TARGET_VERSION TARGET_VERSION_INTERNAL (stderr)
#endif


/* If we are passing smuggling stabs through the MICROBLAZE ECOFF object
   format, put a comment in front of the .stab<x> operation so
   that the MICROBLAZE assembler does not choke.  The microblaze-tfile program
   will correctly put the stab into the object file.  */

/* #define ASM_STABS_OP	((TARGET_GAS) ? ".stabs" : " #.stabs") */
/* #define ASM_STABN_OP	((TARGET_GAS) ? ".stabn" : " #.stabn") */
/* #define ASM_STABD_OP	((TARGET_GAS) ? ".stabd" : " #.stabd") */

#define ASM_STABS_OP	".stabs "  
#define ASM_STABN_OP	".stabn " 
#define ASM_STABD_OP	".stabd " 

/* Local compiler-generated symbols must have a prefix that the assembler
   understands.   By default, this is $, although some targets (e.g.,
   NetBSD-ELF) need to override this. */

#ifndef LOCAL_LABEL_PREFIX
#define LOCAL_LABEL_PREFIX	"$"
#endif

/* By default on the microblaze, external symbols do not have an underscore
   prepended, but some targets (e.g., NetBSD) require this. */

#ifndef USER_LABEL_PREFIX
#define USER_LABEL_PREFIX	""
#endif

/* fixed registers */
#define MICROBLAZE_PPC_ABI_BASE_REGNUM                  0
#define MICROBLAZE_PPC_ABI_STACK_POINTER_REGNUM         1
#define MICROBLAZE_PPC_ABI_GPRO_REGNUM                  2
#define MICROBLAZE_PPC_ABI_GPRW_REGNUM                  13
#define MICROBLAZE_PPC_ABI_INTR_RETURN_ADDR_REGNUM      14
#define MICROBLAZE_PPC_ABI_SUB_RETURN_ADDR_REGNUM       15
#define MICROBLAZE_PPC_ABI_DEBUG_RETURN_ADDR_REGNUM     16
#define MICROBLAZE_PPC_ABI_EXCEPTION_RETURN_ADDR_REGNUM 17
#define MICROBLAZE_PPC_ABI_ASM_TEMP_REGNUM              18
#define MICROBLAZE_PPC_ABI_FRAME_POINTER_REGNUM         19
#define MICROBLAZE_PPC_ABI_PIC_ADDR_REGNUM              20
#define MICROBLAZE_PPC_ABI_PIC_FUNC_REGNUM              21
/* volatile registers */
#define MICROBLAZE_PPC_ABI_INT_RETURN_VAL_REGNUM        3
#define MICROBLAZE_PPC_ABI_INT_RETURN_VAL2_REGNUM       4
#define MICROBLAZE_PPC_ABI_FIRST_ARG_REGNUM             5
#define MICROBLAZE_PPC_ABI_LAST_ARG_REGNUM              10
#define MICROBLAZE_PPC_ABI_MAX_ARG_REGS                 (MICROBLAZE_PPC_ABI_LAST_ARG_REGNUM - MICROBLAZE_PPC_ABI_FIRST_ARG_REGNUM + 1)
#define MICROBLAZE_PPC_ABI_STATIC_CHAIN_REGNUM          3
#define MICROBLAZE_PPC_ABI_TEMP1_REGNUM                 11
#define MICROBLAZE_PPC_ABI_TEMP2_REGNUM                 12

/* Debug stuff */

/* #define SDB_DEBUGGING_INFO		/\* generate info for microblaze-tfile *\/ */
/* /\* Forward references to tags are allowed.  *\/ */
/* #define SDB_ALLOW_FORWARD_REFERENCES */

/* /\* Unknown tags are also allowed.  *\/ */
/* #define SDB_ALLOW_UNKNOWN_REFERENCES */

#define DBX_DEBUGGING_INFO		/* generate stabs (OSF/rose) */

/* On Sun 4, this limit is 2048.  We use 1500 to be safe,
   since the length can run past this up to a continuation point.  */
#define DBX_CONTIN_LENGTH 1500

/* How to renumber registers for dbx and gdb. */
#define DBX_REGISTER_NUMBER(REGNO) microblaze_dbx_regno[ (REGNO) ]

#define DWARF2_UNWIND_INFO 0
/* The mapping from gcc register number to DWARF 2 CFA column number.
 */
#define DWARF_FRAME_REGNUM(REG)				\
  (REG == GP_REG_FIRST + MICROBLAZE_PPC_ABI_SUB_RETURN_ADDR_REGNUM ? DWARF_FRAME_RETURN_COLUMN : REG)

/* The DWARF 2 CFA column which tracks the return address.  */
#define DWARF_FRAME_RETURN_COLUMN (FP_REG_LAST + 1)
#define INCOMING_RETURN_ADDR_RTX  gen_rtx (REG, VOIDmode, GP_REG_FIRST + MICROBLAZE_PPC_ABI_SUB_RETURN_ADDR_REGNUM)


/* /\* Overrides for the COFF debug format.  *\/ */
/* #define PUT_SDB_SCL(a)					\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "\t.scl\t%d;", (a));	\ */
/* } while (0) */

/* #define PUT_SDB_INT_VAL(a)				\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "\t.val\t%d;", (a));	\ */
/* } while (0) */

/* #define PUT_SDB_VAL(a)					\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fputs ("\t.val\t", asm_out_text_file);		\ */
/*   output_addr_const (asm_out_text_file, (a));		\ */
/*   fputc (';', asm_out_text_file);			\ */
/* } while (0) */

/* #define PUT_SDB_DEF(a)					\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "\t%s.def\t",		\ */
/* 	   (TARGET_GAS) ? "" : "#");			\ */
/*   ASM_OUTPUT_LABELREF (asm_out_text_file, a); 		\ */
/*   fputc (';', asm_out_text_file);			\ */
/* } while (0) */

/* #define PUT_SDB_PLAIN_DEF(a)				\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "\t%s.def\t.%s;",		\ */
/* 	   (TARGET_GAS) ? "" : "#", (a));		\ */
/* } while (0) */

/* #define PUT_SDB_ENDEF					\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "\t.endef\n");		\ */
/* } while (0) */

/* #define PUT_SDB_TYPE(a)					\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "\t.type\t0x%x;", (a));	\ */
/* } while (0) */

/* #define PUT_SDB_SIZE(a)					\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "\t.size\t%d;", (a));	\ */
/* } while (0) */

/* #define PUT_SDB_DIM(a)					\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "\t.dim\t%d;", (a));	\ */
/* } while (0) */

/* #ifndef PUT_SDB_START_DIM */
/* #define PUT_SDB_START_DIM				\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "\t.dim\t");		\ */
/* } while (0) */
/* #endif */

/* #ifndef PUT_SDB_NEXT_DIM */
/* #define PUT_SDB_NEXT_DIM(a)				\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "%d,", a);		\ */
/* } while (0) */
/* #endif */

/* #ifndef PUT_SDB_LAST_DIM */
/* #define PUT_SDB_LAST_DIM(a)				\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "%d;", a);		\ */
/* } while (0) */
/* #endif */

/* #define PUT_SDB_TAG(a)					\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file, "\t.tag\t");		\ */
/*   ASM_OUTPUT_LABELREF (asm_out_text_file, a); 		\ */
/*   fputc (';', asm_out_text_file);			\ */
/* } while (0) */

/* /\* For block start and end, we create labels, so that */
/*    later we can figure out where the correct offset is. */
/*    The normal .ent/.end serve well enough for functions, */
/*    so those are just commented out.  *\/ */

/* #define PUT_SDB_BLOCK_START(LINE)			\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file,				\ */
/* 	   "%sLb%d:\n\t%s.begin\t%sLb%d\t%d\n",		\ */
/* 	   LOCAL_LABEL_PREFIX,				\ */
/* 	   sdb_label_count,				\ */
/* 	   (TARGET_GAS) ? "" : "#",			\ */
/* 	   LOCAL_LABEL_PREFIX,				\ */
/* 	   sdb_label_count,				\ */
/* 	   (LINE));					\ */
/*   sdb_label_count++;					\ */
/* } while (0) */

/* #define PUT_SDB_BLOCK_END(LINE)				\ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   fprintf (asm_out_text_file,				\ */
/* 	   "%sLe%d:\n\t%s.bend\t%sLe%d\t%d\n",		\ */
/* 	   LOCAL_LABEL_PREFIX,				\ */
/* 	   sdb_label_count,				\ */
/* 	   (TARGET_GAS) ? "" : "#",			\ */
/* 	   LOCAL_LABEL_PREFIX,				\ */
/* 	   sdb_label_count,				\ */
/* 	   (LINE));					\ */
/*   sdb_label_count++;					\ */
/* } while (0) */

/* #define PUT_SDB_FUNCTION_START(LINE)\ */
/* do {                                                  \ */
/*   extern FILE *asm_out_text_file;             \ */
/*   ASM_OUTPUT_SOURCE_LINE (asm_out_text_file, LINE + sdb_begin_function_line, 0); \ */
/* } while (0) */

/* #define PUT_SDB_FUNCTION_END(LINE)            \ */
/* do {                                                  \ */
/*   extern FILE *asm_out_text_file;             \ */
/*   ASM_OUTPUT_SOURCE_LINE (asm_out_text_file, LINE + sdb_begin_function_line, 0); \ */
/* } while (0) */

/* #define PUT_SDB_EPILOGUE_END(NAME) */

/* #define PUT_SDB_SRC_FILE(FILENAME) \ */
/* do {							\ */
/*   extern FILE *asm_out_text_file;			\ */
/*   output_file_directive (asm_out_text_file, (FILENAME)); \ */
/* } while (0) */

/* #define SDB_GENERATE_FAKE(BUFFER, NUMBER) \ */
/*   sprintf ((BUFFER), ".%dfake", (NUMBER)); */

/* Correct the offset of automatic variables and arguments.  Note that
   the MICROBLAZE debug format wants all automatic variables and arguments
   to be in terms of the virtual frame pointer (stack pointer before
   any adjustment in the function), while the MICROBLAZE linker wants
   the frame pointer to be the stack pointer after the initial
   adjustment.  */

#define DEBUGGER_AUTO_OFFSET(X)  \
  microblaze_debugger_offset (X, (HOST_WIDE_INT) 0)
#define DEBUGGER_ARG_OFFSET(OFFSET, X)  \
  microblaze_debugger_offset (X, (HOST_WIDE_INT) OFFSET)

/* /\* Tell collect that the object format is ECOFF *\/ */
/* #define OBJECT_FORMAT_COFF	/\* Object file looks like COFF *\/ */
/* #define EXTENDED_COFF		/\* ECOFF, not normal coff *\/ */


/* Target machine storage layout */

/* Define in order to support both big and little endian float formats
   in the same gcc binary.  */
/*#define REAL_ARITHMETIC*/

/* Define this if most significant bit is lowest numbered
   in instructions that operate on numbered bit-fields.
*/
#define BITS_BIG_ENDIAN 0

/* Define this if most significant byte of a word is the lowest numbered. */
#define BYTES_BIG_ENDIAN (TARGET_BIG_ENDIAN != 0)

/* Define this if most significant word of a multiword number is the lowest. */
#define WORDS_BIG_ENDIAN (TARGET_BIG_ENDIAN != 0)

/* Define this to set the endianness to use in libgcc2.c, which can
   not depend on target_flags.  */
#if !defined(MICROBLAZEEL) && !defined(__MICROBLAZEEL__)
#define LIBGCC2_WORDS_BIG_ENDIAN 1
#else
#define LIBGCC2_WORDS_BIG_ENDIAN 0
#endif

/* Number of bits in an addressable storage unit */
#define BITS_PER_UNIT           8

/* Width in bits of a "word", which is the contents of a machine register.
   Note that this is not necessarily the width of data type `int';
   if using 16-bit ints on a 68000, this would still be 32.
   But on a machine with 16-bit registers, this would be 16.  */
#define BITS_PER_WORD           32
#define MAX_BITS_PER_WORD       64

/* Width of a word, in units (bytes).  */
#define UNITS_PER_WORD          4
#define MIN_UNITS_PER_WORD      4

/* For MICROBLAZE, width of a floating point register.  */
#define UNITS_PER_FPREG         4

/* A C expression for the size in bits of the type `int' on the
   target machine.  If you don't define this, the default is one
   word.  */
#define INT_TYPE_SIZE           32
/* GCC 3.4.1 
 * Removed the MAX_INT_TYPE_SIZE 
 */
/* #define MAX_INT_TYPE_SIZE 64 */

/* Tell the preprocessor the maximum size of wchar_t.  */
/* GCC 3.4.1 
 * Removed the MAX_WCHAR_TYPE_SIZE 
 */
/*
  #ifndef MAX_WCHAR_TYPE_SIZE
  #ifndef WCHAR_TYPE_SIZE
  #define MAX_WCHAR_TYPE_SIZE MAX_INT_TYPE_SIZE
  #endif
  #endif
*/

/* A C expression for the size in bits of the type `short' on the
   target machine.  If you don't define this, the default is half a
   word.  (If this would be less than one storage unit, it is
   rounded up to one unit.)  */
#define SHORT_TYPE_SIZE         16

/* A C expression for the size in bits of the type `long' on the
   target machine.  If you don't define this, the default is one
   word.  */
#define LONG_TYPE_SIZE          32
#define MAX_LONG_TYPE_SIZE      64

/* A C expression for the size in bits of the type `long long' on the
   target machine.  If you don't define this, the default is two
   words.  */
#define LONG_LONG_TYPE_SIZE     64

/* A C expression for the size in bits of the type `char' on the
   target machine.  If you don't define this, the default is one
   quarter of a word.  (If this would be less than one storage unit,
   it is rounded up to one unit.)  */
#define CHAR_TYPE_SIZE BITS_PER_UNIT

/* A C expression for the size in bits of the type `float' on the
   target machine.  If you don't define this, the default is one
   word.  */
#define FLOAT_TYPE_SIZE         32

/* A C expression for the size in bits of the type `double' on the
   target machine.  If you don't define this, the default is two
   words.  */
#define DOUBLE_TYPE_SIZE        64

/* A C expression for the size in bits of the type `long double' on
   the target machine.  If you don't define this, the default is two
   words.  */
#define LONG_DOUBLE_TYPE_SIZE   64

/* Width in bits of a pointer.
   See also the macro `Pmode' defined below.  */
#ifndef POINTER_SIZE
#define POINTER_SIZE            32
#endif

/* Allocation boundary (in *bits*) for storing arguments in argument list.  */
#define PARM_BOUNDARY           32

/* Allocation boundary (in *bits*) for the code of a function.  */
#define FUNCTION_BOUNDARY       32

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY    32

/* Every structure's size must be a multiple of this.  */
/* 8 is observed right on a DECstation and on riscos 4.02.  */
#define STRUCTURE_SIZE_BOUNDARY 8

/* There is no point aligning anything to a rounder boundary than this.  */
#define BIGGEST_ALIGNMENT       32

/* Set this nonzero if move instructions will actually fail to work
   when given unaligned data.  */
#define STRICT_ALIGNMENT        1

/* Define this if you wish to imitate the way many other C compilers
   handle alignment of bitfields and the structures that contain
   them.

   The behavior is that the type written for a bitfield (`int',
   `short', or other integer type) imposes an alignment for the
   entire structure, as if the structure really did contain an
   ordinary field of that type.  In addition, the bitfield is placed
   within the structure so that it would fit within such a field,
   not crossing a boundary for it.

   Thus, on most machines, a bitfield whose type is written as `int'
   would not cross a four-byte boundary, and would force four-byte
   alignment for the whole structure.  (The alignment used may not
   be four bytes; it is controlled by the other alignment
   parameters.)

   If the macro is defined, its definition should be a C expression;
   a nonzero value for the expression enables this behavior.  */

#define PCC_BITFIELD_TYPE_MATTERS 1

/* If defined, a C expression to compute the alignment given to a
   constant that is being placed in memory.  CONSTANT is the constant
   and ALIGN is the alignment that the object would ordinarily have.
   The value of this macro is used instead of that alignment to align
   the object.

   If this macro is not defined, then ALIGN is used.

   The typical use of this macro is to increase alignment for string
   constants to be word aligned so that `strcpy' calls that copy
   constants can be done inline.  */

#define CONSTANT_ALIGNMENT(EXP, ALIGN)					\
  ((TREE_CODE (EXP) == STRING_CST  || TREE_CODE (EXP) == CONSTRUCTOR)	\
   && (ALIGN) < BITS_PER_WORD						\
	? BITS_PER_WORD							\
	: (ALIGN))

/* If defined, a C expression to compute the alignment for a static
   variable.  TYPE is the data type, and ALIGN is the alignment that
   the object would ordinarily have.  The value of this macro is used
   instead of that alignment to align the object.

   If this macro is not defined, then ALIGN is used.

   One use of this macro is to increase alignment of medium-size
   data to make it all fit in fewer cache lines.  Another is to
   cause character arrays to be word-aligned so that `strcpy' calls
   that copy constants to character arrays can be done inline.  */

#undef DATA_ALIGNMENT
#define DATA_ALIGNMENT(TYPE, ALIGN)					\
  ((((ALIGN) < BITS_PER_WORD)						\
    && (TREE_CODE (TYPE) == ARRAY_TYPE					\
	|| TREE_CODE (TYPE) == UNION_TYPE				\
	|| TREE_CODE (TYPE) == RECORD_TYPE)) ? BITS_PER_WORD : (ALIGN))


/* Define this macro if an argument declared as `char' or `short' in a
   prototype should actually be passed as an `int'.  In addition to
   avoiding errors in certain cases of mismatch, it also makes for
   better code on certain machines. */

#define PROMOTE_PROTOTYPES 1

/* Define if operations between registers always perform the operation
   on the full register even if a narrower mode is specified.  */
#define WORD_REGISTER_OPERATIONS

/* Define if loading in MODE, an integral mode narrower than BITS_PER_WORD
   will either zero-extend or sign-extend.  The value of this macro should
   be the code that says which one of the two operations is implicitly
   done, NIL if none. 

   When in 64 bit mode, microblaze_move_1word will sign extend SImode and CCmode
   moves.  All other referces are zero extended.  */
#define LOAD_EXTEND_OP(MODE)  ZERO_EXTEND

/* Define this macro if it is advisable to hold scalars in registers
   in a wider mode than that declared by the program.  In such cases, 
   the value is constrained to be within the bounds of the declared
   type, but kept valid in the wider mode.  The signedness of the
   extension may differ from that of the type.

   We promote any value smaller than SImode up to SImode.  We don't
   want to promote to DImode when in 64 bit mode, because that would
   prevent us from using the faster SImode multiply and divide
   instructions.  */

#define PROMOTE_MODE(MODE, UNSIGNEDP, TYPE)	\
  if (GET_MODE_CLASS (MODE) == MODE_INT		\
      && GET_MODE_SIZE (MODE) < 4)		\
    (MODE) = SImode;

/* Define this if function arguments should also be promoted using the above
   procedure.  */

#define PROMOTE_FUNCTION_ARGS

/* Likewise, if the function return value is promoted.  */

#define PROMOTE_FUNCTION_RETURN

/* Standard register usage.  */

/* Number of actual hardware registers.
   The hardware registers are assigned numbers for the compiler
   from 0 to just below FIRST_PSEUDO_REGISTER.
   All registers that the compiler knows about must be given numbers,
   even those that are not normally considered general registers.

   On the MicroBlaze, we have 32 integer registers */

#define FIRST_PSEUDO_REGISTER 78

/* 1 for registers that have pervasive standard uses
   and are not available for the register allocator.
   does not include arg passing regs 
*/

#define FIXED_REGISTERS							\
{									\
  1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,			\
  1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 				\
}


/* 1 for registers not available across function calls.
   i.e. all non-volatiles are 0
   These must include the FIXED_REGISTERS and also any
   registers that can be used without being saved.
   The latter must include the registers where values are returned
   and the register where structure-value addresses are passed.
   Aside from that, you can include as many other registers as you like.  */

#define CALL_USED_REGISTERS						\
{									\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,			\
  1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,			\
  1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 				\
}

/* Internal macros to classify a register number as to whether it's a
   general purpose register, a floating point register, a
   multiply/divide register, or a status register.  */

#define GP_REG_FIRST    0
#define GP_REG_LAST     31
#define GP_REG_NUM      (GP_REG_LAST - GP_REG_FIRST + 1)
#define GP_DBX_FIRST    0

#define FP_REG_FIRST    32
#define FP_REG_LAST     63 
#define FP_REG_NUM      (FP_REG_LAST - FP_REG_FIRST + 1)
#define FP_DBX_FIRST    ((write_symbols == DBX_DEBUG) ? 38 : 32)

#define MD_REG_FIRST    64
#define MD_REG_LAST     66
#define MD_REG_NUM      (MD_REG_LAST - MD_REG_FIRST + 1)

#define ST_REG_FIRST    67
#define ST_REG_LAST     74
#define ST_REG_NUM      (ST_REG_LAST - ST_REG_FIRST + 1)

#define AP_REG_NUM      75
#define RAP_REG_NUM     76
#define FRP_REG_NUM     77

#define GP_REG_P(REGNO) ((unsigned) ((REGNO) - GP_REG_FIRST) < GP_REG_NUM)
#define FP_REG_P(REGNO) ((unsigned) ((REGNO) - FP_REG_FIRST) < FP_REG_NUM)
#define MD_REG_P(REGNO) ((unsigned) ((REGNO) - MD_REG_FIRST) < MD_REG_NUM)
#define ST_REG_P(REGNO) ((unsigned) ((REGNO) - ST_REG_FIRST) < ST_REG_NUM)

/* Return number of consecutive hard regs needed starting at reg REGNO
   to hold something of mode MODE.
   This is ordinarily the length in words of a value of mode MODE
   but can be less for certain modes in special long registers.

*/

#define HARD_REGNO_NREGS(REGNO, MODE)					\
  (! FP_REG_P (REGNO)							\
	? ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD) \
	: ((GET_MODE_SIZE (MODE) + UNITS_PER_FPREG - 1) / UNITS_PER_FPREG))

/* Value is 1 if hard register REGNO can hold a value of machine-mode
   MODE.  In 32 bit mode, require that DImode and DFmode be in even
   registers.  For DImode, this makes some of the insns easier to
   write, since you don't have to worry about a DImode value in
   registers 3 & 4, producing a result in 4 & 5.
   FIXME: Can we avoid restricting odd-numbered register bases for DImode and DFmode ?

   To make the code simpler HARD_REGNO_MODE_OK now just references an
   array built in override_options.  Because machmodes.h is not yet
   included before this file is processed, the MODE bound can't be
   expressed here.  */
extern char microblaze_hard_regno_mode_ok[][FIRST_PSEUDO_REGISTER];
#define HARD_REGNO_MODE_OK(REGNO, MODE)					\
            microblaze_hard_regno_mode_ok[ (int)(MODE) ][ (REGNO)]


/* Value is 1 if it is a good idea to tie two pseudo registers
   when one has mode MODE1 and one has mode MODE2.
   If HARD_REGNO_MODE_OK could produce different values for MODE1 and MODE2,
   for any hard reg, then this must be 0 for correct output.  */
#define MODES_TIEABLE_P(MODE1, MODE2)					\
  ((GET_MODE_CLASS (MODE1) == MODE_FLOAT ||				\
    GET_MODE_CLASS (MODE1) == MODE_COMPLEX_FLOAT)			\
   == (GET_MODE_CLASS (MODE2) == MODE_FLOAT ||				\
       GET_MODE_CLASS (MODE2) == MODE_COMPLEX_FLOAT))

/* MICROBLAZE pc is not overloaded on a register.	*/
/* #define PC_REGNUM xx				*/

/* Register to use for pushing function arguments.  */
#define STACK_POINTER_REGNUM    \
        (GP_REG_FIRST + MICROBLAZE_PPC_ABI_STACK_POINTER_REGNUM)

/* Offset from the stack pointer to the first location for outgoing arguments. */
#define STACK_POINTER_OFFSET    \
        FIRST_PARM_OFFSET(FNDECL) 

/* Base register for access to local variables of the function.  We
   pretend that the frame pointer is
   MICROBLAZE_PPC_ABI_INTR_RETURN_ADDR_REGNUM, and then eliminate it
   to HARD_FRAME_POINTER_REGNUM.  We can get away with this because
   rMICROBLAZE_PPC_ABI_INTR_RETUREN_ADDR_REGNUM is a fixed
   register(return address for interrupt), and will not be used for
   anything else.  
   
   [12/14/01] We need to check this out. This might be no longer true,
   now that we have interrupt controllers working. */
#define FRAME_POINTER_REGNUM            \
        (FRP_REG_NUM)

#define HARD_FRAME_POINTER_REGNUM       \
        (GP_REG_FIRST + MICROBLAZE_PPC_ABI_FRAME_POINTER_REGNUM)

/* Value should be nonzero if functions must have frame pointers.
   Zero means the frame pointer need not be set up (and parms
   may be accessed via the stack pointer) in functions that seem suitable.
   This is computed in `reload', in reload1.c.  */
#define FRAME_POINTER_REQUIRED          \
        (current_function_calls_alloca)

/* Base register for access to arguments of the function.  */
#define ARG_POINTER_REGNUM              \
        (AP_REG_NUM)

/* Fake register that holds the address on the stack of the
   current function's return address.  */
#define RETURN_ADDRESS_POINTER_REGNUM   \
        (RAP_REG_NUM)

/* Register in which static-chain is passed to a function.  */
#define STATIC_CHAIN_REGNUM             \
        (GP_REG_FIRST + MICROBLAZE_PPC_ABI_STATIC_CHAIN_REGNUM)

/* If the structure value address is passed in a register, then
   `STRUCT_VALUE_REGNUM' should be the number of that register.  */
/* #define STRUCT_VALUE_REGNUM (GP_REG_FIRST + 4) */

/* If the structure value address is not passed in a register, define
   `STRUCT_VALUE' as an expression returning an RTX for the place
   where the address is passed.  If it returns 0, the address is
   passed as an "invisible" first argument.  */
#define STRUCT_VALUE    0

/* registers used in prologue/epilogue code when the stack frame
   is larger than 32K bytes.  These registers must come from the
   scratch register set, and not used for passing and returning
   arguments and any other information used in the calling sequence
   (such as pic).  
*/

#define MICROBLAZE_TEMP1_REGNUM         \
        (GP_REG_FIRST + MICROBLAZE_PPC_ABI_TEMP1_REGNUM)

#define MICROBLAZE_TEMP2_REGNUM         \
        (GP_REG_FIRST + MICROBLAZE_PPC_ABI_TEMP2_REGNUM)

/* Define this macro if it is as good or better to call a constant
   function address than to call an address kept in a register.  */
#define NO_FUNCTION_CSE                 1

/* Define this macro if it is as good or better for a function to
   call itself with an explicit address than to call an address
   kept in a register.  */
#define NO_RECURSIVE_FUNCTION_CSE       1

/* The register number of the register used to address a table of
   static data addresses in memory.  In some cases this register is
   defined by a processor's "application binary interface" (ABI). 
   When this macro is defined, RTL is generated for this register
   once, as with the stack pointer and frame pointer registers.  If
   this macro is not defined, it is up to the machine-dependent
   files to allocate such a register (if necessary).  */
#define PIC_OFFSET_TABLE_REGNUM         \
        (GP_REG_FIRST + MICROBLAZE_PPC_ABI_PIC_ADDR_REGNUM)

#define PIC_FUNCTION_ADDR_REGNUM        \
        (GP_REG_FIRST + MICROBLAZE_PPC_ABI_PIC_FUNC_REGNUM)

/* Initialize embedded_pic_fnaddr_rtx before RTL generation for
   each function.  We used to do this in FINALIZE_PIC, but FINALIZE_PIC
   isn't always called for static inline functions.  */
#define INIT_EXPANDERS			\
do {					\
  embedded_pic_fnaddr_rtx = NULL;	\
} while (0)

/* Define the classes of registers for register constraints in the
   machine description.  Also define ranges of constants.

   One of the classes must always be named ALL_REGS and include all hard regs.
   If there is more than one class, another class must be named NO_REGS
   and contain no registers.

   The name GENERAL_REGS must be the name of a class (or an alias for
   another name such as ALL_REGS).  This is the class of registers
   that is allowed by "g" or "r" in a register constraint.
   Also, registers outside this class are allocated only when
   instructions express preferences for them.

   The classes must be numbered in nondecreasing order; that is,
   a larger-numbered class must never be contained completely
   in a smaller-numbered class.

   For any two classes, it is very desirable that there be another
   class that represents their union.  */

enum reg_class
{
    NO_REGS,			/* no registers in set */
    GR_REGS,			/* integer registers */
    FP_REGS,			/* floating point registers */
    HI_REG,			/* hi register */
    LO_REG,			/* lo register */
    HILO_REG,			/* hilo register pair for 64 bit mode mult */
    MD_REGS,			/* multiply/divide registers (hi/lo) */
    HI_AND_GR_REGS,		/* union classes */
    LO_AND_GR_REGS,
    HILO_AND_GR_REGS,
    ST_REGS,			/* status registers (fp status) */
    ALL_REGS,			/* all registers */
    LIM_REG_CLASSES		/* max value + 1 */
};

#define N_REG_CLASSES (int) LIM_REG_CLASSES

#define GENERAL_REGS GR_REGS

/* An initializer containing the names of the register classes as C
   string constants.  These names are used in writing some of the
   debugging dumps.  */

#define REG_CLASS_NAMES							\
{									\
  "NO_REGS",								\
  "GR_REGS",								\
  "FP_REGS",								\
  "HI_REG",								\
  "LO_REG",								\
  "HILO_REG",								\
  "MD_REGS",								\
  "HI_AND_GR_REGS",							\
  "LO_AND_GR_REGS",							\
  "HILO_AND_GR_REGS",							\
  "ST_REGS",								\
  "ALL_REGS"								\
}

/* An initializer containing the contents of the register classes,
   as integers which are bit masks.  The Nth integer specifies the
   contents of class N.  The way the integer MASK is interpreted is
   that register R is in the class if `MASK & (1 << R)' is 1.

   When the machine has more than 32 registers, an integer does not
   suffice.  Then the integers are replaced by sub-initializers,
   braced groupings containing several integers.  Each
   sub-initializer must be suitable as an initializer for the type
   `HARD_REG_SET' which is defined in `hard-reg-set.h'.  */

#define REG_CLASS_CONTENTS						\
{									\
  { 0x00000000, 0x00000000, 0x00000000 },	/* no registers */	\
  { 0xffffffff, 0x00000000, 0x00000000 },	/* integer registers */	\
  { 0x00000000, 0xffffffff, 0x00000000 },	/* floating registers*/	\
  { 0x00000000, 0x00000000, 0x00000001 },	/* hi register */	\
  { 0x00000000, 0x00000000, 0x00000002 },	/* lo register */	\
  { 0x00000000, 0x00000000, 0x00000004 },	/* hilo register */	\
  { 0x00000000, 0x00000000, 0x00000003 },	/* mul/div registers */	\
  { 0xffffffff, 0x00000000, 0x00000001 },	/* union classes */     \
  { 0xffffffff, 0x00000000, 0x00000002 },				\
  { 0xffffffff, 0x00000000, 0x00000004 },				\
  { 0x00000000, 0x00000000, 0x000007f8 },	/* status registers */	\
  { 0xffffffff, 0xffffffff, 0x000007ff }	/* all registers */	\
}


/* A C expression whose value is a register class containing hard
   register REGNO.  In general there is more that one such class;
   choose a class which is "minimal", meaning that no smaller class
   also contains the register.  */

extern enum reg_class microblaze_regno_to_class[];

#define REGNO_REG_CLASS(REGNO) microblaze_regno_to_class[ (REGNO) ]

/* A macro whose definition is the name of the class to which a
   valid base register must belong.  A base register is one used in
   an address which is the register value plus a displacement.  */

#define BASE_REG_CLASS  GR_REGS

/* A macro whose definition is the name of the class to which a
   valid index register must belong.  An index register is one used
   in an address where its value is either multiplied by a scale
   factor or added to another register (as well as added to a
   displacement).  */

#define INDEX_REG_CLASS GR_REGS


/* This macro is used later on in the file.  */
#define GR_REG_CLASS_P(CLASS) ((CLASS) == GR_REGS)

/* REG_ALLOC_ORDER is to order in which to allocate registers.  This
   is the default value (allocate the registers in numeric order).  We
   define it just so that we can override it if necessary in
   ORDER_REGS_FOR_LOCAL_ALLOC.  */

#define REG_ALLOC_ORDER							\
{  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 16,	\
  17, 18, 19, 20, 21, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 15,	\
  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,	\
  48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,	\
  64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75		        \
}

/* ORDER_REGS_FOR_LOCAL_ALLOC is a macro which permits reg_alloc_order
   to be rearranged based on a particular function.  
*/

#define ORDER_REGS_FOR_LOCAL_ALLOC microblaze_order_regs_for_local_alloc ()

/* REGISTER AND CONSTANT CLASSES */

/* Get reg_class from a letter such as appears in the machine
   description.

   DEFINED REGISTER CLASSES:

   'd'  General (aka integer) registers
   Normally this is GR_REGS
   'y'  General registers 
   'f'	Floating point registers
   'h'	Hi register
   'l'	Lo register
   'x'	Multiply/divide registers
   'a'	HILO_REG
   'z'	FP Status register
   'b'	All registers */

extern enum reg_class microblaze_char_to_class[];

#define REG_CLASS_FROM_LETTER(C) microblaze_char_to_class[(C)]

/* The letters I, J, K, L, M, N, O, and P in a register constraint
   string can be used to stand for particular ranges of immediate
   operands.  This macro defines what the ranges are.  C is the
   letter, and VALUE is a constant value.  Return 1 if VALUE is
   in the range specified by C.  */

/* 
`I'	is used for the range of constants an arithmetic insn can
actually contain (16 bits signed integers).

`J'	is used for the range which is just zero (ie, $r0).

`K'	is used for the range of constants a logical insn can actually
contain (16 bit zero-extended integers).

`L'	is used for the range of constants that be loaded with lui
(ie, the bottom 16 bits are zero).

`M'	is used for the range of constants that take two words to load
(ie, not matched by `I', `K', and `L').

`N'	is used for negative 16 bit constants other than -65536.

`O'	is a 15 bit signed integer.

`P'	is used for positive 16 bit constants.  */

#define SMALL_INT(X) ((unsigned HOST_WIDE_INT) (INTVAL (X) + 0x8000) < 0x10000)
#define SMALL_INT_UNSIGNED(X) ((unsigned HOST_WIDE_INT) (INTVAL (X)) < 0x10000)

/* Deifinition of K changed for MicroBlaze specific code */

#define CONST_OK_FOR_LETTER_P(VALUE, C)					\
  ((C) == 'I' ? ((unsigned HOST_WIDE_INT) ((VALUE) + 0x8000) < 0x10000)	\
   : (C) == 'J' ? ((VALUE) == 0)					\
   : (C) == 'K' ? (((VALUE) >= -32768) && ((VALUE) <= 32767))           \
   : (C) == 'L' ? (((VALUE) & 0x0000ffff) == 0				\
		   && (((VALUE) & ~2147483647) == 0			\
		       || ((VALUE) & ~2147483647) == ~2147483647))	\
   : (C) == 'M' ? ((((VALUE) & ~0x0000ffff) != 0)			\
		   && (((VALUE) & ~0x0000ffff) != ~0x0000ffff)		\
		   && (((VALUE) & 0x0000ffff) != 0			\
		       || (((VALUE) & ~2147483647) != 0			\
			   && ((VALUE) & ~2147483647) != ~2147483647)))	\
   : (C) == 'N' ? ((unsigned HOST_WIDE_INT) ((VALUE) + 0xffff) < 0xffff) \
   : (C) == 'O' ? ((unsigned HOST_WIDE_INT) ((VALUE) + 0x4000) < 0x8000) \
   : (C) == 'P' ? ((VALUE) != 0 && (((VALUE) & ~0x0000ffff) == 0))	\
   : 0)

/* Similar, but for floating constants, and defining letters G and H.
   Here VALUE is the CONST_DOUBLE rtx itself.  */

/* 'G'	: Floating point 0 */

#define CONST_DOUBLE_OK_FOR_LETTER_P(VALUE, C)				\
  ((C) == 'G'								\
   && (VALUE) == CONST0_RTX (GET_MODE (VALUE)))

/* Letters in the range `Q' through `U' may be defined in a
   machine-dependent fashion to stand for arbitrary operand types. 
   The machine description macro `EXTRA_CONSTRAINT' is passed the
   operand as its first argument and the constraint letter as its
   second operand.

   `R'	is for memory references which take 1 word for the instruction.
   `S'	is for references to extern items which are PIC for OSF/rose.
   `T'	is for memory addresses that can be used to load two words.  */

#define EXTRA_CONSTRAINT(OP,CODE)					\
  (((CODE) == 'T')	  ? double_memory_operand (OP, GET_MODE (OP))	\
   : ((CODE) == 'Q')	  ? FALSE                                       \
   : (GET_CODE (OP) != MEM) ? FALSE					\
   : ((CODE) == 'R')	  ? simple_memory_operand (OP, GET_MODE (OP))	\
   : ((CODE) == 'S')	  ? (HALF_PIC_P () && CONSTANT_P (OP)		\
			     && HALF_PIC_ADDRESS_P (OP))		\
   : ((CODE) == 's')      ? ST_REG_P(INTVAL(OP))   			\
   : FALSE)

/* Say which of the above are memory constraints.  */
#define EXTRA_MEMORY_CONSTRAINT(C, STR) ((C) == 'R' || (C) == 'T')

/* Given an rtx X being reloaded into a reg required to be
   in class CLASS, return the class of reg to actually use.
   In general this is just CLASS; but on some machines
   in some cases it is preferable to use a more restrictive class.  */

#define PREFERRED_RELOAD_CLASS(X,CLASS)					\
  ((CLASS) != ALL_REGS							\
   ? (CLASS)							\
   : ((GET_MODE_CLASS (GET_MODE (X)) == MODE_FLOAT			\
       || GET_MODE_CLASS (GET_MODE (X)) == MODE_COMPLEX_FLOAT)		\
      ? (GR_REGS)			\
      : ((GET_MODE_CLASS (GET_MODE (X)) == MODE_INT			\
	  || GET_MODE (X) == VOIDmode)					\
	 ? (GR_REGS) : (CLASS))))

/* Certain machines have the property that some registers cannot be
   copied to some other registers without using memory.  Define this
   macro on those machines to be a C expression that is non-zero if
   objects of mode MODE in registers of CLASS1 can only be copied to
   registers of class CLASS2 by storing a register of CLASS1 into
   memory and loading that memory location into a register of CLASS2.

   Do not define this macro if its value would always be zero.  */

#define SECONDARY_MEMORY_NEEDED(CLASS1, CLASS2, MODE)			\
  ((!TARGET_DEBUG_H_MODE						\
    && GET_MODE_CLASS (MODE) == MODE_INT				\
    && ((CLASS1 == FP_REGS && GR_REG_CLASS_P (CLASS2))			\
	|| (GR_REG_CLASS_P (CLASS1) && CLASS2 == FP_REGS))))

/* The HI and LO registers can only be reloaded via the general
   registers.  Condition code registers can only be loaded to the
   general registers, and from the floating point registers.  */

#define SECONDARY_INPUT_RELOAD_CLASS(CLASS, MODE, X)			\
  microblaze_secondary_reload_class (CLASS, MODE, X, 1)
#define SECONDARY_OUTPUT_RELOAD_CLASS(CLASS, MODE, X)			\
  microblaze_secondary_reload_class (CLASS, MODE, X, 0)

/* Not declared above, with the other functions, because enum
   reg_class is not declared yet.  */
/* enum reg_class	microblaze_secondary_reload_class ();*/

/* Return the maximum number of consecutive registers
   needed to represent mode MODE in a register of class CLASS.  */

#define CLASS_UNITS(mode, size)						\
  ((GET_MODE_SIZE (mode) + (size) - 1) / (size))

#define CLASS_MAX_NREGS(CLASS, MODE)					\
  ((CLASS) == FP_REGS							\
   ? 2 * CLASS_UNITS (MODE, 8)					\
   : CLASS_UNITS (MODE, UNITS_PER_WORD))


/* Stack layout; function entry, exit and calling.  */

/* Define this if pushing a word on the stack
   makes the stack pointer a smaller address.  */
#define STACK_GROWS_DOWNWARD

/* Offset within stack frame to start allocating local variables at.
   If FRAME_GROWS_DOWNWARD, this is the offset to the END of the
   first local allocated.  Otherwise, it is the offset to the BEGINNING
   of the first local allocated.  */
/* Changed the starting frame offset to including the new link stuff */
#define STARTING_FRAME_OFFSET						\
  (current_function_outgoing_args_size					\
   + (TARGET_ABICALLS ? MICROBLAZE_STACK_ALIGN (UNITS_PER_WORD) : 0)    \
   +  (FIRST_PARM_OFFSET(FNDECL)))

/* The return address for the current frame is in r31 is this is a leaf
   function.  Otherwise, it is on the stack.  It is at a variable offset
   from sp/fp/ap, so we define a fake hard register rap which is a
   poiner to the return address on the stack.  This always gets eliminated
   during reload to be either the frame pointer or the stack pointer plus
   an offset.  */

/* ??? This definition fails for leaf functions.  There is currently no
   general solution for this problem.  */

/* ??? There appears to be no way to get the return address of any previous
   frame except by disassembling instructions in the prologue/epilogue.
   So currently we support only the current frame.  */

#define RETURN_ADDR_RTX(count, frame)			\
  ((count == 0)						\
   ? gen_rtx (MEM, Pmode, gen_rtx (REG, Pmode, RETURN_ADDRESS_POINTER_REGNUM))\
   : (rtx) 0)

/* Structure to be filled in by compute_frame_size with register
   save masks, and offsets for the current function.  */

    struct microblaze_frame_info
    {
        long total_size;		/* # bytes that the entire frame takes up */
        long var_size;                  /* # bytes that variables take up */
        long args_size;                 /* # bytes that outgoing arguments take up */
        long extra_size;		/* # bytes of extra gunk */
        int  link_debug_size;           /* # bytes for the link reg and back pointer */
        int  gp_reg_size;		/* # bytes needed to store gp regs */
        int  fp_reg_size;		/* # bytes needed to store fp regs */
        long mask;			/* mask of saved gp registers */
        long fmask;			/* mask of saved fp registers */
        long gp_save_offset;		/* offset from vfp to store gp registers */
        long fp_save_offset;		/* offset from vfp to store fp registers */
        long gp_sp_offset;		/* offset from new sp to store gp registers */
        long fp_sp_offset;		/* offset from new sp to store fp registers */
        int  initialized;		/* != 0 if frame size already calculated */
        int  num_gp;			/* number of gp registers saved */
        int  num_fp;			/* number of fp registers saved */
        long insns_len;                 /* length of insns */
        int  alloc_stack;               /* Flag to indicate if the current function must not create stack space. (As an optimization) */
    };

extern struct microblaze_frame_info current_frame_info;

/* If defined, this macro specifies a table of register pairs used to
   eliminate unneeded registers that point into the stack frame.  If
   it is not defined, the only elimination attempted by the compiler
   is to replace references to the frame pointer with references to
   the stack pointer.

   The definition of this macro is a list of structure
   initializations, each of which specifies an original and
   replacement register.

   On some machines, the position of the argument pointer is not
   known until the compilation is completed.  In such a case, a
   separate hard register must be used for the argument pointer. 
   This register can be eliminated by replacing it with either the
   frame pointer or the argument pointer, depending on whether or not
   the frame pointer has been eliminated.

   In this case, you might specify:
   #define ELIMINABLE_REGS  \
   {{ARG_POINTER_REGNUM, STACK_POINTER_REGNUM}, \
   {ARG_POINTER_REGNUM, FRAME_POINTER_REGNUM}, \
   {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}}

   Note that the elimination of the argument pointer with the stack
   pointer is specified first since that is the preferred elimination.

*/

#define ELIMINABLE_REGS							\
{{ ARG_POINTER_REGNUM,   STACK_POINTER_REGNUM},				\
 { ARG_POINTER_REGNUM,   GP_REG_FIRST + MICROBLAZE_PPC_ABI_FRAME_POINTER_REGNUM},				\
 { RETURN_ADDRESS_POINTER_REGNUM, STACK_POINTER_REGNUM},		\
 { RETURN_ADDRESS_POINTER_REGNUM, GP_REG_FIRST + MICROBLAZE_PPC_ABI_FRAME_POINTER_REGNUM},			\
 { RETURN_ADDRESS_POINTER_REGNUM, GP_REG_FIRST + MICROBLAZE_PPC_ABI_SUB_RETURN_ADDR_REGNUM},			\
 { FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM},				\
 { FRAME_POINTER_REGNUM, GP_REG_FIRST + MICROBLAZE_PPC_ABI_FRAME_POINTER_REGNUM}}

/* A C expression that returns non-zero if the compiler is allowed to
   try to replace register number FROM-REG with register number
   TO-REG.  This macro need only be defined if `ELIMINABLE_REGS' is
   defined, and will usually be the constant 1, since most of the
   cases preventing register elimination are things that the compiler
   already knows about.

   We can always eliminate to the
   frame pointer.  We can eliminate to the stack pointer unless
   a frame pointer is needed.  
*/

#define CAN_ELIMINATE(FROM, TO)						\
  (((FROM) == RETURN_ADDRESS_POINTER_REGNUM && (! leaf_function_p ()	\
   || (TO == GP_REG_FIRST + MICROBLAZE_PPC_ABI_SUB_RETURN_ADDR_REGNUM && leaf_function_p)))   			\
  || ((FROM) != RETURN_ADDRESS_POINTER_REGNUM				\
   && ((TO) == HARD_FRAME_POINTER_REGNUM 				\
   || ((TO) == STACK_POINTER_REGNUM && ! frame_pointer_needed))))


/* This macro is similar to `INITIAL_FRAME_POINTER_OFFSET'.  It
   specifies the initial difference between the specified pair of
   registers.  This macro must be defined if `ELIMINABLE_REGS' is
   defined.  */

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)			 \
        (OFFSET) = microblaze_initial_elimination_offset ((FROM), (TO))

/* If defined, the maximum amount of space required for outgoing
   arguments will be computed and placed into the variable
   `current_function_outgoing_args_size'.  No space will be pushed
   onto the stack for each call; instead, the function prologue
   should increase the stack frame size by this amount.

   It is not proper to define both `PUSH_ROUNDING' and
   `ACCUMULATE_OUTGOING_ARGS'.  */
#define ACCUMULATE_OUTGOING_ARGS        1

/* Offset from the argument pointer register to the first argument's
   address.  On some machines it may depend on the data type of the
   function.

   If `ARGS_GROW_DOWNWARD', this is the offset to the location above
   the first argument's address.

   On the MICROBLAZE, we must skip the first argument position if we are
   returning a structure or a union, to account for its address being
   passed in $4.  However, at the current time, this produces a compiler
   that can't bootstrap, so comment it out for now.  */

/* The first Parameter is dependent wether the
   current_function_is_leaf and the wether in debug mode or not */

/*  GCC 3.4.1 
 *  Ideal definition is commented out below as "Old Code". Somehow, current_function_is_leaf seems to be incorrect when used in these contexts.
 *  Hence there is a hack in compute_frame_size to identify when not to allocate stack for leaf functions.
 */ 
#define FIRST_PARM_OFFSET(FNDECL)       4 

/* Old Code */		     
/* #define FIRST_PARM_OFFSET(FNDECL) (((current_function_is_leaf == 0 ? 1 : 0)  * (UNITS_PER_WORD)))  */


/* When a parameter is passed in a register, stack space is still
   allocated for it.  For the MICROBLAZE, stack space must be allocated 

   BEWARE that some space is also allocated for non existing arguments
   in register. In case an argument list is of form GF used registers
   are a0 (a2,a3), but we should push over a1...  */

#define REG_PARM_STACK_SPACE(FNDECL)        (MAX_ARGS_IN_REGISTERS * UNITS_PER_WORD) 

/* Define this if it is the responsibility of the caller to
   allocate the area reserved for arguments passed in registers. 
   If `ACCUMULATE_OUTGOING_ARGS' is also defined, the only effect
   of this macro is to determine whether the space is included in 
   `current_function_outgoing_args_size'.  */
#define OUTGOING_REG_PARM_STACK_SPACE       1

/* Align stack frames on 64 bits (Double Word ).  */
#ifndef STACK_BOUNDARY
#define STACK_BOUNDARY                      64
#endif

/* Make sure 4 words are always allocated on the stack.  */
/* XLNX : Need to see if this should be changed to 6 since there are 6
   registers for argument now */

/* Added NUM_OF_ARGS to have value 6, so that atleast 6 *
   UNITS_PER_WORD is stored for arguments */
    
#define NUM_OF_ARGS                         6

/* A C expression that should indicate the number of bytes of its
   own arguments that a function pops on returning, or 0
   if the function pops no arguments and the caller must therefore
   pop them all after the function returns.

   FUNDECL is the declaration node of the function (as a tree).

   FUNTYPE is a C variable whose value is a tree node that
   describes the function in question.  Normally it is a node of
   type `FUNCTION_TYPE' that describes the data type of the function.
   From this it is possible to obtain the data types of the value
   and arguments (if known).

   When a call to a library function is being considered, FUNTYPE
   will contain an identifier node for the library function.  Thus,
   if you need to distinguish among various library functions, you
   can do so by their names.  Note that "library function" in this
   context means a function used to perform arithmetic, whose name
   is known specially in the compiler and was not mentioned in the
   C code being compiled.

   STACK-SIZE is the number of bytes of arguments passed on the
   stack.  If a variable number of bytes is passed, it is zero, and
   argument popping will always be the responsibility of the
   calling function.  */

#define RETURN_POPS_ARGS(FUNDECL,FUNTYPE,SIZE) 0


/* Symbolic macros for the registers used to return integer and floating
   point values.  */

#define GP_RETURN (GP_REG_FIRST + MICROBLAZE_PPC_ABI_INT_RETURN_VAL_REGNUM)
#define FP_RETURN (GP_RETURN)

/* Symbolic macros for the first/last argument registers.  */

#define GP_ARG_FIRST (GP_REG_FIRST + MICROBLAZE_PPC_ABI_FIRST_ARG_REGNUM)
#define GP_ARG_LAST  (GP_REG_FIRST + MICROBLAZE_PPC_ABI_LAST_ARG_REGNUM)
#define FP_ARG_FIRST (FP_REG_FIRST + 12)
#define FP_ARG_LAST  (FP_REG_FIRST + 15)

#define MAX_ARGS_IN_REGISTERS	MICROBLAZE_PPC_ABI_MAX_ARG_REGS

/* Define how to find the value returned by a library function
   assuming the value has mode MODE.  Because we define
   PROMOTE_FUNCTION_RETURN, we must promote the mode just as
   PROMOTE_MODE does.  */

#define LIBCALL_VALUE(MODE)						\
  gen_rtx (REG,								\
	   ((GET_MODE_CLASS (MODE) != MODE_INT				\
	     || GET_MODE_SIZE (MODE) >= 4)				\
	    ? (MODE)							\
	    : SImode),							\
	   ((GET_MODE_CLASS (MODE) == MODE_FLOAT			\
	     && (! TARGET_SINGLE_FLOAT					\
		 || GET_MODE_SIZE (MODE) <= 4))				\
	    ? FP_RETURN							\
	    : GP_RETURN))

/* Define how to find the value returned by a function.
   VALTYPE is the data type of the value (as a tree).
   If the precise function being called is known, FUNC is its FUNCTION_DECL;
   otherwise, FUNC is 0.  */

#define FUNCTION_VALUE(VALTYPE, FUNC) LIBCALL_VALUE (TYPE_MODE (VALTYPE))


/* 1 if N is a possible register number for a function value.
   On the MICROBLAZE, R2 R3 and F0 F2 are the only register thus used.
   Currently, R2 and F0 are only implemented  here (C has no complex type)  */

#define FUNCTION_VALUE_REGNO_P(N) ((N) == GP_RETURN || (N) == FP_RETURN)

/* 1 if N is a possible register number for function argument passing.
   We have no FP argument registers when soft-float.  When FP registers
   are 32 bits, we can't directly reference the odd numbered ones.  */

#define FUNCTION_ARG_REGNO_P(N)					\
  (((N) >= GP_ARG_FIRST && (N) <= GP_ARG_LAST))

/* A C expression which can inhibit the returning of certain function
   values in registers, based on the type of value.  A nonzero value says
   to return the function value in memory, just as large structures are
   always returned.  Here TYPE will be a C expression of type
   `tree', representing the data type of the value.

   Note that values of mode `BLKmode' must be explicitly
   handled by this macro.  Also, the option `-fpcc-struct-return'
   takes effect regardless of this macro.  On most systems, it is
   possible to leave the macro undefined; this causes a default
   definition to be used, whose value is the constant 1 for BLKmode
   values, and 0 otherwise.

   GCC normally converts 1 byte structures into chars, 2 byte
   structs into shorts, and 4 byte structs into ints, and returns
   them this way.  Defining the following macro overrides this,
   to give us MICROBLAZE cc compatibility.  */

#define RETURN_IN_MEMORY(TYPE)	\
  (TYPE_MODE (TYPE) == BLKmode)

/* A code distinguishing the floating point format of the target
   machine.  There are three defined values: IEEE_FLOAT_FORMAT,
   VAX_FLOAT_FORMAT, and UNKNOWN_FLOAT_FORMAT.  */

#define TARGET_FLOAT_FORMAT IEEE_FLOAT_FORMAT


/* Define a data type for recording info about an argument list
   during the scan of that argument list.  This data type should
   hold all necessary information about the function itself
   and about the args processed so far, enough to enable macros
   such as FUNCTION_ARG to determine where the next arg should go.

*/

typedef struct microblaze_args {
    int gp_reg_found;		/* whether a gp register was found yet */
    int arg_number;		/* argument number */
    int arg_words;		/* # total words the arguments take */
    int fp_arg_words;		/* # words for FP args (MICROBLAZE_EABI only) */
    int last_arg_fp;		/* nonzero if last arg was FP (EABI only) */
    int fp_code;		/* Mode of FP arguments */
    int num_adjusts;		/* number of adjustments made */
				/* Adjustments made to args pass in regs.  */
				/* ??? The size is doubled to work around a 
				   bug in the code that sets the adjustments
				   in function_arg.  */
    struct rtx_def *adjust[MAX_ARGS_IN_REGISTERS*2];
} CUMULATIVE_ARGS;

/* Initialize a variable CUM of type CUMULATIVE_ARGS
   for a call to a function whose data type is FNTYPE.
   For a library call, FNTYPE is 0.

*/

#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,FNDECL,N_NAMED_ARGS)		\
  init_cumulative_args (&CUM, FNTYPE, LIBNAME)                                  

/* Update the data in CUM to advance over an argument
   of mode MODE and data type TYPE.
   (TYPE is null for libcalls where that information may not be available.)  */

#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED)			\
  function_arg_advance (&CUM, MODE, TYPE, NAMED)

/* Determine where to put an argument to a function.
   Value is zero to push the argument on the stack,
   or a hard register in which to store the argument.

   MODE is the argument's machine mode.
   TYPE is the data type of the argument (as a tree).
   This is null for libcalls where that information may
   not be available.
   CUM is a variable of type CUMULATIVE_ARGS which gives info about
   the preceding args and about the function being called.
   NAMED is nonzero if this argument is a named parameter
   (otherwise it is an extra parameter matching an ellipsis).  */

#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED) \
  function_arg( &CUM, MODE, TYPE, NAMED)

/* For an arg passed partly in registers and partly in memory,
   this is the number of registers used.
   For args passed entirely in registers or entirely in memory, zero. */

#define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED) \
  function_arg_partial_nregs (&CUM, MODE, TYPE, NAMED)

#define MSR_SAVE_REG 31

/* Tell prologue and epilogue if register REGNO should be saved / restored.  */

/* Added the link register for functions which are not leaf */
/* Also added the interrupt_handler temp reg save, which is being used for saving the MSR */

/* [02/13/02] : Added interrupt and exception registers to the list of
   registers being saved on interrupts */

/* [05/28/02] : Modified to make interrupt_handler save all the registers */
/* Save all volatiles if you have the interrupt handler attribute */

#define MUST_SAVE_REGISTER(regno) \
 (((regs_ever_live[regno] && !call_used_regs[regno])			\
  || (regno == HARD_FRAME_POINTER_REGNUM && frame_pointer_needed)       \
  || (((regs_ever_live[regno] || (regno == MSR_SAVE_REG)) && interrupt_handler))   \
  || (regs_ever_live[regno] && save_volatiles)                 \
  || (regno == MICROBLAZE_PPC_ABI_ASM_TEMP_REGNUM && ( save_volatiles || interrupt_handler)) \
  || (regno == MICROBLAZE_PPC_ABI_EXCEPTION_RETURN_ADDR_REGNUM && ( save_volatiles || interrupt_handler)) \
  || ((regno == MICROBLAZE_PPC_ABI_SUB_RETURN_ADDR_REGNUM) && (!current_function_is_leaf))) \
  || ((regno >= 3 && regno <= 12) && (interrupt_handler || save_volatiles) && (!current_function_is_leaf))\
    && regno != 0)

/* use the interrupt_handler here to save that save register */

/* ALIGN FRAMES on double word boundaries */
#ifndef MICROBLAZE_STACK_ALIGN
#define MICROBLAZE_STACK_ALIGN(LOC) (((LOC) + 7) & ~7)
#endif


/* Define this macro if the mcount subroutine on your system does not need 
   a counter variable allocated for each function. This is true for almost 
   all modern implementations. If you define this macro, you must not use 
   the labelno argument to FUNCTION_PROFILER. */

#define NO_PROFILE_COUNTERS         1

/* Output assembler code to FILE to call profiling routine '_mcount'
   for profiling a function entry.  */

#define FUNCTION_PROFILER(FILE, LABELNO) { \
  {                                        \
    char *fnname = XSTR (XEXP (DECL_RTL (current_function_decl), 0), 0); \
    fprintf (FILE, "\tbrki\tr16,_mcount\n");           \
  }                                                    \
 }

/* Define this macro if the code for function profiling should come
   before the function prologue.  Normally, the profiling code comes
   after.  
   
   #define PROFILE_BEFORE_PROLOGUE 
*/

/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  The value is tested only in
   functions that have frame pointers.
   No definition is equivalent to always zero.  */

#define EXIT_IGNORE_STACK 1


/* A C statement to output, on the stream FILE, assembler code for a
   block of data that contains the constant parts of a trampoline. 
   This code should not include a label--the label is taken care of
   automatically.  */

#define TRAMPOLINE_TEMPLATE(STREAM)					 \
{									 \
  fprintf (STREAM, "\t.word\t0x03e00821\t\t# move   $1,$31\n");		\
  fprintf (STREAM, "\t.word\t0x04110001\t\t# bgezal $0,.+8\n");		\
  fprintf (STREAM, "\t.word\t0x00000000\t\t# nop\n");			\
  fprintf (STREAM, "\t.word\t0x8fe30014\t\t# lw     $3,20($31)\n");	\
  fprintf (STREAM, "\t.word\t0x8fe20018\t\t# lw     $2,24($31)\n");	\
  fprintf (STREAM, "\t.word\t0x0060c821\t\t# move   $25,$3 (abicalls)\n"); \
  fprintf (STREAM, "\t.word\t0x00600008\t\t# jr     $3\n");		\
  fprintf (STREAM, "\t.word\t0x0020f821\t\t# move   $31,$1\n");		\
  fprintf (STREAM, "\t.word\t0x00000000\t\t# <function address>\n"); \
  fprintf (STREAM, "\t.word\t0x00000000\t\t# <static chain value>\n"); \
}

/* A C expression for the size in bytes of the trampoline, as an
   integer.  */

#define TRAMPOLINE_SIZE (32 + (8))

/* Alignment required for trampolines, in bits.  */

#define TRAMPOLINE_ALIGNMENT    32

/* INITIALIZE_TRAMPOLINE calls this library function to flush
   program and data caches.  */

#ifndef CACHE_FLUSH_FUNC
#define CACHE_FLUSH_FUNC "_flush_cache"
#endif

/* A C statement to initialize the variable parts of a trampoline. 
   ADDR is an RTX for the address of the trampoline; FNADDR is an
   RTX for the address of the nested function; STATIC_CHAIN is an
   RTX for the static chain value that should be passed to the
   function when it is called.  */

#define INITIALIZE_TRAMPOLINE(ADDR, FUNC, CHAIN)			    \
{									    \
  rtx addr = ADDR;							    \
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant (addr, 32)), FUNC);   \
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant (addr, 36)), CHAIN);  \
									    \
  /* Flush both caches.  We need to flush the data cache in case	    \
     the system has a write-back cache.  */				    \
  /* ??? Should check the return value for errors.  */			    \
  emit_library_call (gen_rtx (SYMBOL_REF, Pmode, CACHE_FLUSH_FUNC),	    \
		     0, VOIDmode, 3, addr, Pmode,			    \
		     GEN_INT (TRAMPOLINE_SIZE), TYPE_MODE (integer_type_node),\
		     GEN_INT (3), TYPE_MODE (integer_type_node));	    \
}

/* Addressing modes, and classification of registers for them.  */

#define REGNO_OK_FOR_BASE_P(regno)   \
  microblaze_regno_ok_for_base_p ((regno), 1)


#define REGNO_OK_FOR_INDEX_P(regno)  \
  microblaze_regno_ok_for_base_p ((regno), 1)

/* The macros REG_OK_FOR..._P assume that the arg is a REG rtx
   and check its validity for a certain class.
   We have two alternate definitions for each of them.
   The usual definition accepts all pseudo regs; the other rejects them all.
   The symbol REG_OK_STRICT causes the latter definition to be used.

   Most source files want to accept pseudo regs in the hope that
   they will get allocated to the class that the insn wants them to be in.
   Some source files that are used after register allocation
   need to be strict.  */

#ifndef REG_OK_STRICT
#define REG_STRICT_FLAG         0
#else
#define REG_STRICT_FLAG         1
#endif

/* Nonzero if X is a hard reg that can be used as an index
   or if it is a pseudo reg in the non-strict case.  */
#define REG_OK_FOR_BASE_P(X)    \
  microblaze_regno_ok_for_base_p (REGNO (X), REG_STRICT_FLAG)

/* Nonzero if X is a hard reg that can be used as an index
   or if it is a pseudo reg in the non-strict case.  */
#define REG_OK_FOR_INDEX_P(X)   \
  microblaze_regno_ok_for_base_p (REGNO (X), REG_STRICT_FLAG)


/* Maximum number of registers that can appear in a valid memory address.  */

#define MAX_REGS_PER_ADDRESS 2

/* A C compound statement with a conditional `goto LABEL;' executed
   if X (an RTX) is a legitimate memory address on the target
   machine for a memory operand of mode MODE.

   It usually pays to define several simpler macros to serve as
   subroutines for this one.  Otherwise it may be too complicated
   to understand.

   This macro must exist in two variants: a strict variant and a
   non-strict one.  The strict variant is used in the reload pass. 
   It must be defined so that any pseudo-register that has not been
   allocated a hard register is considered a memory reference.  In
   contexts where some kind of register is required, a
   pseudo-register with no hard register must be rejected.

   The non-strict variant is used in other passes.  It must be
   defined to accept all pseudo-registers in every context where
   some kind of register is required.

   Typically among the subroutines used to define
   `GO_IF_LEGITIMATE_ADDRESS' are subroutines to check for
   acceptable registers for various purposes (one for base
   registers, one for index registers, and so on).  Then only these
   subroutine macros need have two variants; the higher levels of
   macros may be the same whether strict or not.

   Normally, constant addresses which are the sum of a `symbol_ref'
   and an integer are stored inside a `const' RTX to mark them as
   constant.  Therefore, there is no need to recognize such sums
   specifically as legitimate addresses.  Normally you would simply
   recognize any `const' as legitimate.

   Usually `PRINT_OPERAND_ADDRESS' is not prepared to handle
   constant sums that are not marked with  `const'.  It assumes
   that a naked `plus' indicates indexing.  If so, then you *must*
   reject such naked constant sums as illegitimate addresses, so
   that none of them will be given to `PRINT_OPERAND_ADDRESS'.

   On some machines, whether a symbolic address is legitimate
   depends on the section that the address refers to.  On these
   machines, define the macro `ENCODE_SECTION_INFO' to store the
   information into the `symbol_ref', and then check for it here. 
   When you see a `const', you will have to look inside it to find
   the `symbol_ref' in order to determine the section.  */

#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)                     \
{                                                                   \
  if (microblaze_legitimate_address_p (MODE, X, REG_STRICT_FLAG))   \
    goto ADDR;                                                      \
}


/* A C statement or compound statement with a conditional `goto
   LABEL;' executed if memory address X (an RTX) can have different
   meanings depending on the machine mode of the memory reference it
   is used for.

   Autoincrement and autodecrement addresses typically have
   mode-dependent effects because the amount of the increment or
   decrement is the size of the operand being addressed.  Some
   machines have other mode-dependent addresses.  Many RISC machines
   have no mode-dependent addresses.

   You may assume that ADDR is a valid address for the machine.  */

#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR,LABEL) {}

#if 1
#define GO_PRINTF(x)	trace(x)
#define GO_PRINTF2(x,y)	trace(x,y)
#define GO_DEBUG_RTX(x) debug_rtx(x)
#else
#define GO_PRINTF(x)
#define GO_PRINTF2(x,y)
#define GO_DEBUG_RTX(x)
#endif

/* A C expression that is 1 if the RTX X is a constant which is a
   valid address.  This is defined to be the same as `CONSTANT_P (X)',
   but rejecting CONST_DOUBLE.  */
/* When pic, we must reject addresses of the form symbol+large int.
   This is because an instruction `sw $4,s+70000' needs to be converted
   by the assembler to `lw $at,s($gp);sw $4,70000($at)'.  Normally the
   assembler would use $at as a temp to load in the large offset.  In this
   case $at is already in use.  We convert such problem addresses to
   `la $5,s;sw $4,70000($5)' via LEGITIMIZE_ADDRESS.  */
#define CONSTANT_ADDRESS_P(X)						\
  ((GET_CODE (X) == LABEL_REF || GET_CODE (X) == SYMBOL_REF		\
    || GET_CODE (X) == CONST_INT 		                        \
    || (GET_CODE (X) == CONST						\
	&& ! (flag_pic && pic_address_needs_scratch (X))))		\
   && (!HALF_PIC_P () || !HALF_PIC_ADDRESS_P (X)))

/* Define this, so that when PIC, reload won't try to reload invalid
   addresses which require two reload registers.  */

#define LEGITIMATE_PIC_OPERAND_P(X)  (! pic_address_needs_scratch (X))

/* Nonzero if the constant value X is a legitimate general operand.
   It is given that X satisfies CONSTANT_P or is a CONST_DOUBLE.

   At present, GAS doesn't understand li.[sd], so don't allow it
   to be generated at present.  Also, the MICROBLAZE assembler does not
   grok li.d Infinity.  */

#define LEGITIMATE_CONSTANT_P(X)				\
  (GET_CODE (X) != CONST_DOUBLE					\
    || microblaze_const_double_ok (X, GET_MODE (X)))

/* Try a machine-dependent way of reloading an illegitimate address
   operand.  If we find one, push the reload and jump to WIN.  This
   macro is used in only one place: `find_reloads_address' in reload.c.

   Implemented on microblaze by microblaze_legitimize_reload_address.  
   Note that (X) is evaluated twice; this is safe in current usage.  */ 
#define LEGITIMIZE_ADDRESS(X,OLDX,MODE,WIN)			\
{  rtx result = microblaze_legitimize_address (X, OLDX, MODE);	\
   if (result != NULL_RTX) {					\
       (X) = result;						\
       goto WIN;						\
     }								\
}

/* Define this macro if references to a symbol must be treated
   differently depending on something about the variable or
   function named by the symbol (such as what section it is in).

   The macro definition, if any, is executed immediately after the
   rtl for DECL has been created and stored in `DECL_RTL (DECL)'. 
   The value of the rtl will be a `mem' whose address is a
   `symbol_ref'.

   The usual thing for this macro to do is to a flag in the
   `symbol_ref' (such as `SYMBOL_REF_FLAG') or to store a modified
   name string in the `symbol_ref' (if one bit is not enough
   information).

   The best way to modify the name string is by adding text to the
   beginning, with suitable punctuation to prevent any ambiguity. 
   Allocate the new name in `saveable_obstack'.  You will have to
   modify `ASM_OUTPUT_LABELREF' to remove and decode the added text
   and output the name accordingly.

   You can also check the information stored in the `symbol_ref' in
   the definition of `GO_IF_LEGITIMATE_ADDRESS' or
   `PRINT_OPERAND_ADDRESS'.

   When optimizing for the $gp pointer, SYMBOL_REF_FLAG is set for all
   small objects.

   When generating embedded PIC code, SYMBOL_REF_FLAG is set for
   symbols which are not in the .text section.

   When not embedded PIC, if a symbol is in a
   gp addresable section, SYMBOL_REF_FLAG is set prevent gcc from
   splitting the reference so that gas can generate a gp relative
   reference.

   When TARGET_EMBEDDED_DATA is set, we assume that all const
   variables will be stored in ROM, which is too far from %gp to use
   %gprel addressing.  Note that (1) we include "extern const"
   variables in this, which microblaze_select_section doesn't, and (2) we
   can't always tell if they're really const (they might be const C++
   objects with non-const constructors), so we err on the side of
   caution and won't use %gprel anyway (otherwise we'd have to defer
   this decision to the linker/loader).  The handling of extern consts
   is why the DECL_INITIAL macros differ from microblaze_select_section.

   If you are changing this macro, you should look at
   microblaze_select_section and see if it needs a similar change.  */

#ifndef UNIQUE_SECTION_P
#define UNIQUE_SECTION_P(DECL) (0)
#endif

#define TREE_STRING_RTL(NODE) (STRING_CST_CHECK (NODE)->string.rtl)

#define CONSTANT_POOL_BEFORE_FUNCTION TRUE


/* Specify the machine mode that this machine uses
   for the index in the tablejump instruction.
*/
#define CASE_VECTOR_MODE    (SImode)

/* Define as C expression which evaluates to nonzero if the tablejump
   instruction expects the table to contain offsets from the address of the
   table.
   Do not define this if the table should contain absolute addresses. */

/* GCC 3.4.1 
 * Removed
 */
/* Specify the tree operation to be used to convert reals to integers.  */
/* #define IMPLICIT_FIX_EXPR FIX_ROUND_EXPR*/

/* GCC 3.4.1 
 * Removed
 */
/* This is the kind of divide that is easiest to do in the general case.  */
/* #define EASY_DIV_EXPR TRUNC_DIV_EXPR*/

/* Define this as 1 if `char' should by default be signed; else as 0.  */
#ifndef DEFAULT_SIGNED_CHAR
#define DEFAULT_SIGNED_CHAR 1
#endif

/* Max number of bytes we can move from memory to memory
   in one reasonably fast instruction.  */
#define MOVE_MAX 4
#define MAX_MOVE_MAX 8

/* Define this macro as a C expression which is nonzero if
   accessing less than a word of memory (i.e. a `char' or a
   `short') is no faster than accessing a word of memory, i.e., if
   such access require more than one instruction or if there is no
   difference in cost between byte and (aligned) word loads.

   On RISC machines, it tends to generate better code to define
   this as 1, since it avoids making a QI or HI mode register.  */
#define SLOW_BYTE_ACCESS 1

/* We assume that the store-condition-codes instructions store 0 for false
   and some other value for true.  This is the value stored for true.  */

#define STORE_FLAG_VALUE 1

/* Define this if zero-extension is slow (more than one real instruction).  */
/* removed for GCC 3.3 */
/*#define SLOW_ZERO_EXTEND*/

/* Define this to be nonzero if shift instructions ignore all but the low-order
   few bits. */
#define SHIFT_COUNT_TRUNCATED 1

/* Value is 1 if truncating an integer of INPREC bits to OUTPREC bits
   is done just by pretending it is already truncated.  */
/* In 64 bit mode, 32 bit instructions require that register values be properly
   sign-extended to 64 bits.  As a result, a truncate is not a no-op if it
   converts a value >32 bits to a value <32 bits.  */
/* ??? This results in inefficient code for 64 bit to 32 conversions.
   Something needs to be done about this.  Perhaps not use any 32 bit
   instructions?  Perhaps use PROMOTE_MODE?  */
#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC)  1

/* Specify the machine mode that pointers have.
   After generation of rtl, the compiler makes no further distinction
   between pointers and any other objects of this machine mode. */

#ifndef Pmode
#define Pmode SImode
#endif

/* A function address in a call instruction
   is a word address (for indexing purposes)
   so give the MEM rtx a words's mode.  */

#define FUNCTION_MODE   (SImode)

/* Define TARGET_MEM_FUNCTIONS if we want to use calls to memcpy and
   memset, instead of the BSD functions bcopy and bzero.  */
#define TARGET_MEM_FUNCTIONS

/* GCC 3.4.1
 * Removed CONST_COSTS. Find a way to get behavior back ???
 */

/* /\* A part of a C `switch' statement that describes the relative */
/*    costs of constant RTL expressions.  It must contain `case' */
/*    labels for expression codes `const_int', `const', `symbol_ref', */
/*    `label_ref' and `const_double'.  Each case must ultimately reach */
/*    a `return' statement to return the relative cost of the use of */
/*    that kind of constant value in an expression.  The cost may */
/*    depend on the precise value of the constant, which is available */
/*    for examination in X. */

/*    CODE is the expression code--redundant, since it can be obtained */
/*    with `GET_CODE (X)'.  *\/ */

/* #define CONST_COSTS(X,CODE,OUTER_CODE)					\ */
/*   case CONST_INT:							\ */
/* 	/\* Always return 0, since we don't have different sized		\ */
/* 	   instructions, hence different costs according to Richard	\ */
/* 	   Kenner *\/							\ */
/* 	return 0;							\ */
/*   case LABEL_REF:							\ */
/*     return COSTS_N_INSNS (2);						\ */
/* 									\ */
/*   case CONST:								\ */
/*     {									\ */
/*       rtx offset = const0_rtx;						\ */
/*       rtx symref = eliminate_constant_term (XEXP (X, 0), &offset);	\ */
/* 									\ */
/*       if (GET_CODE (symref) == LABEL_REF)				\ */
/* 	return COSTS_N_INSNS (2);					\ */
/* 									\ */
/*       if (GET_CODE (symref) != SYMBOL_REF)				\ */
/* 	return COSTS_N_INSNS (4);					\ */
/* 									\ */
/*       /\* let's be paranoid.... *\/					\ */
/*       if (INTVAL (offset) < -32768 || INTVAL (offset) > 32767)		\ */
/* 	return COSTS_N_INSNS (2);					\ */
/* 									\ */
/*       return COSTS_N_INSNS (SYMBOL_REF_FLAG (symref) ? 1 : 2);		\ */
/*     }									\ */
/* 									\ */
/*   case SYMBOL_REF:							\ */
/*     return COSTS_N_INSNS (SYMBOL_REF_FLAG (X) ? 1 : 2);			\ */
/* 									\ */
/*   case CONST_DOUBLE:							\ */
/*     {									\ */
/*       rtx high, low;							\ */
/*       split_double (X, &high, &low);					\ */
/*       return COSTS_N_INSNS ((high == CONST0_RTX (GET_MODE (high))	\ */
/* 			     || low == CONST0_RTX (GET_MODE (low)))	\ */
/* 			    ? 2 : 4);					\ */
/*     } */

/* A C expression for the cost of moving data from a register in
   class FROM to one in class TO.  The classes are expressed using
   the enumeration values such as `GENERAL_REGS'.  A value of 2 is
   the default; other values are interpreted relative to that.

   It is not required that the cost always equal 2 when FROM is the
   same as TO; on some machines it is expensive to move between
   registers if they are not general registers.

   If reload sees an insn consisting of a single `set' between two
   hard registers, and if `REGISTER_MOVE_COST' applied to their
   classes returns a value of 2, reload does not check to ensure
   that the constraints of the insn are met.  Setting a cost of
   other than 2 will allow reload to verify that the constraints are
   met.  You should do this if the `movM' pattern's constraints do
   not allow such copying. */

/* SID : GCC move Initial definition was : REGISTER_MOVE_COST(FROM, TO) */
/* Mode should alwasy be SImode */
#define REGISTER_MOVE_COST(MODE, FROM, TO)	\
  ( GR_REG_CLASS_P (FROM) && GR_REG_CLASS_P (TO) ? 2 \
   : (FROM) == FP_REGS && (TO) == FP_REGS ? 2				\
   : GR_REG_CLASS_P (FROM) && (TO) == FP_REGS ? 4			\
   : (FROM) == FP_REGS && GR_REG_CLASS_P (TO) ? 4			\
   : (((FROM) == HI_REG || (FROM) == LO_REG				\
       || (FROM) == MD_REGS || (FROM) == HILO_REG)			\
      && GR_REG_CLASS_P (TO)) ? 6		\
   : (((TO) == HI_REG || (TO) == LO_REG					\
       || (TO) == MD_REGS || (TO) == HILO_REG)				\
      && GR_REG_CLASS_P (FROM)) ? 6		\
   : (FROM) == ST_REGS && GR_REG_CLASS_P (TO) ? 4			\
   : (FROM) == FP_REGS && (TO) == ST_REGS ? 8				\
   : 12)

#define MEMORY_MOVE_COST(MODE,CLASS,TO_P) \
  (4 + memory_move_secondary_cost ((MODE), (CLASS), (TO_P)))

/* Define if copies to/from condition code registers should be avoided.

This is needed for the MICROBLAZE because reload_outcc is not complete;
it needs to handle cases where the source is a general or another
condition code register.  */
#define AVOID_CCMODE_COPIES

/* A C expression for the cost of a branch instruction.  A value of
   1 is the default; other values are interpreted relative to that.  */

#define BRANCH_COST   3

/* A C statement (sans semicolon) to update the integer variable COST
   based on the relationship between INSN that is dependent on
   DEP_INSN through the dependence LINK.  The default is to make no
   adjustment to COST.  On the MICROBLAZE, ignore the cost of anti- and
   output-dependencies.  */

#define ADJUST_COST(INSN,LINK,DEP_INSN,COST)				\
  if (REG_NOTE_KIND (LINK) != 0)					\
    (COST) = 0; /* Anti or output dependence.  */

/* Optionally define this if you have added predicates to
   `MACHINE.c'.  This macro is called within an initializer of an
   array of structures.  The first field in the structure is the
   name of a predicate and the second field is an array of rtl
   codes.  For each predicate, list all rtl codes that can be in
   expressions matched by the predicate.  The list should have a
   trailing comma.  Here is an example of two entries in the list
   for a typical RISC machine:

   #define PREDICATE_CODES \
   {"gen_reg_rtx_operand", {SUBREG, REG}},  \
   {"reg_or_short_cint_operand", {SUBREG, REG, CONST_INT}},

   Defining this macro does not affect the generated code (however,
   incorrect definitions that omit an rtl code that may be matched
   by the predicate can cause the compiler to malfunction). 
   Instead, it allows the table built by `genrecog' to be more
   compact and efficient, thus speeding up the compiler.  The most
   important predicates to include in the list specified by this
   macro are thoses used in the most insn patterns.  */

#define PREDICATE_CODES							\
  {"uns_arith_operand",		{ REG, CONST_INT, SUBREG }},		\
  {"arith_operand",		{ REG, CONST_INT, SUBREG }},		\
  {"arith32_operand",		{ REG, CONST_INT, SUBREG }},		\
  {"reg_or_0_operand",		{ REG, CONST_INT, CONST_DOUBLE, SUBREG }}, \
  {"true_reg_or_0_operand",	{ REG, CONST_INT, CONST_DOUBLE, SUBREG }}, \
  {"small_int",			{ CONST_INT }},				\
  {"immediate32_operand",	{ CONST_INT }},				\
  {"microblaze_const_double_ok",	{ CONST_DOUBLE }},		\
  {"const_float_1_operand",	{ CONST_DOUBLE }},			\
  {"simple_memory_operand",	{ MEM, SUBREG }},			\
  {"imm_required_operand",	{ MEM, SUBREG }},			\
  {"equality_op",		{ EQ, NE }},				\
  {"lessthan_op",		{ LT, LTU }},				\
  {"cmp_op",			{ EQ, NE, GT, GE, GTU, GEU, LT, LE,	\
				  LTU, LEU }},				\
  {"signed_cmp_op",             { EQ, NE, GT, GE, LT, LE }},            \
  {"unsigned_cmp_op",           { GTU, GEU, LTU, LEU }},                \
  {"pc_or_label_operand",	{ PC, LABEL_REF }},			\
  {"call_insn_operand",		{ CONST_INT, CONST, SYMBOL_REF, REG}},	\
  {"move_operand", 		{ CONST_INT, CONST_DOUBLE, CONST,	\
				  SYMBOL_REF, LABEL_REF, SUBREG,	\
				  REG, MEM}},				\
  {"movdi_operand",		{ CONST_INT, CONST_DOUBLE, CONST,	\
				  SYMBOL_REF, LABEL_REF, SUBREG, REG,	\
				  MEM, SIGN_EXTEND }},			\
  {"extend_operator",           { SIGN_EXTEND, ZERO_EXTEND }},          \
  {"highpart_shift_operator",   { ASHIFTRT, LSHIFTRT, ROTATERT, ROTATE }}, 



/* If defined, a C statement to be executed just prior to the
   output of assembler code for INSN, to modify the extracted
   operands so they will be output differently.

   Here the argument OPVEC is the vector containing the operands
   extracted from INSN, and NOPERANDS is the number of elements of
   the vector which contain meaningful data for this insn.  The
   contents of this vector are what will be used to convert the
   insn template into assembler code, so you can change the
   assembler output by changing the contents of the vector.

   We use it to check if the current insn needs a nop in front of it
   because of load delays, and also to update the delay slot
   statistics.  */

#define FINAL_PRESCAN_INSN(INSN, OPVEC, NOPERANDS)			\
  final_prescan_insn (INSN, OPVEC, NOPERANDS)


/* Control the assembler format that we output.  */

/* Output to assembler file text saying following lines
   may contain character constants, extra white space, comments, etc.  */

#define ASM_APP_ON " #APP\n"

/* Output to assembler file text saying following lines
   no longer contain unusual constructs.  */

#define ASM_APP_OFF " #NO_APP\n"

/* How to refer to registers in assembler output.
   This sequence is indexed by compiler's hard-register-number (see above).

   In order to support the two different conventions for register names,
   we use the name of a table set up in microblaze.c, which is overwritten
   if -mrnames is used.  */

#define REGISTER_NAMES							\
{									\
  &microblaze_reg_names[ 0][0],						\
  &microblaze_reg_names[ 1][0],						\
  &microblaze_reg_names[ 2][0],						\
  &microblaze_reg_names[ 3][0],						\
  &microblaze_reg_names[ 4][0],						\
  &microblaze_reg_names[ 5][0],						\
  &microblaze_reg_names[ 6][0],						\
  &microblaze_reg_names[ 7][0],						\
  &microblaze_reg_names[ 8][0],						\
  &microblaze_reg_names[ 9][0],						\
  &microblaze_reg_names[10][0],						\
  &microblaze_reg_names[11][0],						\
  &microblaze_reg_names[12][0],						\
  &microblaze_reg_names[13][0],						\
  &microblaze_reg_names[14][0],						\
  &microblaze_reg_names[15][0],						\
  &microblaze_reg_names[16][0],						\
  &microblaze_reg_names[17][0],						\
  &microblaze_reg_names[18][0],						\
  &microblaze_reg_names[19][0],						\
  &microblaze_reg_names[20][0],						\
  &microblaze_reg_names[21][0],						\
  &microblaze_reg_names[22][0],						\
  &microblaze_reg_names[23][0],						\
  &microblaze_reg_names[24][0],						\
  &microblaze_reg_names[25][0],						\
  &microblaze_reg_names[26][0],						\
  &microblaze_reg_names[27][0],						\
  &microblaze_reg_names[28][0],						\
  &microblaze_reg_names[29][0],						\
  &microblaze_reg_names[30][0],						\
  &microblaze_reg_names[31][0],						\
  &microblaze_reg_names[32][0],						\
  &microblaze_reg_names[33][0],						\
  &microblaze_reg_names[34][0],						\
  &microblaze_reg_names[35][0],						\
  &microblaze_reg_names[36][0],						\
  &microblaze_reg_names[37][0],						\
  &microblaze_reg_names[38][0],						\
  &microblaze_reg_names[39][0],						\
  &microblaze_reg_names[40][0],						\
  &microblaze_reg_names[41][0],						\
  &microblaze_reg_names[42][0],						\
  &microblaze_reg_names[43][0],						\
  &microblaze_reg_names[44][0],						\
  &microblaze_reg_names[45][0],						\
  &microblaze_reg_names[46][0],						\
  &microblaze_reg_names[47][0],						\
  &microblaze_reg_names[48][0],						\
  &microblaze_reg_names[49][0],						\
  &microblaze_reg_names[50][0],						\
  &microblaze_reg_names[51][0],						\
  &microblaze_reg_names[52][0],						\
  &microblaze_reg_names[53][0],						\
  &microblaze_reg_names[54][0],						\
  &microblaze_reg_names[55][0],						\
  &microblaze_reg_names[56][0],						\
  &microblaze_reg_names[57][0],						\
  &microblaze_reg_names[58][0],						\
  &microblaze_reg_names[59][0],						\
  &microblaze_reg_names[60][0],						\
  &microblaze_reg_names[61][0],						\
  &microblaze_reg_names[62][0],						\
  &microblaze_reg_names[63][0],						\
  &microblaze_reg_names[64][0],						\
  &microblaze_reg_names[65][0],						\
  &microblaze_reg_names[66][0],						\
  &microblaze_reg_names[67][0],						\
  &microblaze_reg_names[68][0],						\
  &microblaze_reg_names[69][0],						\
  &microblaze_reg_names[70][0],						\
  &microblaze_reg_names[71][0],						\
  &microblaze_reg_names[72][0],						\
  &microblaze_reg_names[73][0],						\
  &microblaze_reg_names[74][0],						\
  &microblaze_reg_names[75][0],						\
  &microblaze_reg_names[76][0],						\
  &microblaze_reg_names[77][0],						\
}

/* print-rtl.c can't use REGISTER_NAMES, since it depends on microblaze.c.
   So define this for it.  */
#define DEBUG_REGISTER_NAMES						\
{									\
  "$0",   "sp", "rogp",   "v0",   "v1",   "a0",   "a1",   "a2",		\
  "a3",   "a4",   "a5",   "t0",   "t1",   "rwgp", "k0",   "k1",		\
  "k2",   "k3",   "at",   "s0",   "s1",   "s2",   "s3",   "s4",		\
  "s5",   "s6",   "s7",   "s8",   "s9",   "s10",  "s11",  "s12",	\
  "$f0",  "$f1",  "$f2",  "$f3",  "$f4",  "$f5",  "$f6",  "$f7",	\
  "$f8",  "$f9",  "$f10", "$f11", "$f12", "$f13", "$f14", "$f15",	\
  "$f16", "$f17", "$f18", "$f19", "$f20", "$f21", "$f22", "$f23",	\
  "$f24", "$f25", "$f26", "$f27", "$f28", "$f29", "$f30", "$f31",	\
  "hi",   "lo",   "accum","rmsr","$fcc1","$fcc2","$fcc3","$fcc4",	\
  "$fcc5","$fcc6","$fcc7","$rap"					\
}

/* If defined, a C initializer for an array of structures
   containing a name and a register number.  This macro defines
   additional names for hard registers, thus allowing the `asm'
   option in declarations to refer to registers using alternate
   names.

   We define both names for the integer registers here.  */

#define ADDITIONAL_REGISTER_NAMES					\
{									\
  { "r0",	 0 + GP_REG_FIRST },					\
  { "r1",	 1 + GP_REG_FIRST },					\
  { "r2",	 2 + GP_REG_FIRST },					\
  { "r3",	 3 + GP_REG_FIRST },					\
  { "r4",	 4 + GP_REG_FIRST },					\
  { "r5",	 5 + GP_REG_FIRST },					\
  { "r6",	 6 + GP_REG_FIRST },					\
  { "r7",	 7 + GP_REG_FIRST },					\
  { "r8",	 8 + GP_REG_FIRST },					\
  { "r9",	 9 + GP_REG_FIRST },					\
  { "r10",	10 + GP_REG_FIRST },					\
  { "r11",	11 + GP_REG_FIRST },					\
  { "r12",	12 + GP_REG_FIRST },					\
  { "r13",	13 + GP_REG_FIRST },					\
  { "r14",	14 + GP_REG_FIRST },					\
  { "r15",	15 + GP_REG_FIRST },					\
  { "r16",	16 + GP_REG_FIRST },					\
  { "r17",	17 + GP_REG_FIRST },					\
  { "r18",	18 + GP_REG_FIRST },					\
  { "r19",	19 + GP_REG_FIRST },					\
  { "r20",	20 + GP_REG_FIRST },					\
  { "r21",	21 + GP_REG_FIRST },					\
  { "r22",	22 + GP_REG_FIRST },					\
  { "r23",	23 + GP_REG_FIRST },					\
  { "r24",	24 + GP_REG_FIRST },					\
  { "r25",	25 + GP_REG_FIRST },					\
  { "r26",	26 + GP_REG_FIRST },					\
  { "r27",	27 + GP_REG_FIRST },					\
  { "r28",	28 + GP_REG_FIRST },					\
  { "r29",	29 + GP_REG_FIRST },					\
  { "r30",	30 + GP_REG_FIRST },					\
  { "r31",	31 + GP_REG_FIRST },					\
  { "rsp",	MICROBLAZE_PPC_ABI_STACK_POINTER_REGNUM + GP_REG_FIRST },					\
  { "rfp",	MICROBLAZE_PPC_ABI_FRAME_POINTER_REGNUM + GP_REG_FIRST },					\
  { "at",	 MICROBLAZE_PPC_ABI_ASM_TEMP_REGNUM + GP_REG_FIRST },					\
  { "rmsr",     ST_REG_FIRST}, \
  { "rogp",	 2 + GP_REG_FIRST },					\
  { "v0",	 3 + GP_REG_FIRST },					\
  { "v1",	 4 + GP_REG_FIRST },					\
  { "a0",	 5 + GP_REG_FIRST },					\
  { "a1",	 6 + GP_REG_FIRST },					\
  { "a2",	 7 + GP_REG_FIRST },					\
  { "t3",	 8 + GP_REG_FIRST },					\
  { "t4",	 9 + GP_REG_FIRST },					\
  { "t5",	10 + GP_REG_FIRST },					\
  { "t0",	11 + GP_REG_FIRST },					\
  { "t1",	12 + GP_REG_FIRST },					\
  { "rwgp",	13 + GP_REG_FIRST },					\
  { "k0",	14 + GP_REG_FIRST },					\
  { "k1",	15 + GP_REG_FIRST },					\
  { "k2",	16 + GP_REG_FIRST },					\
  { "k3",	17 + GP_REG_FIRST },					\
  { "at",	18 + GP_REG_FIRST },					\
  { "$fp",	19 + GP_REG_FIRST },					\
  { "s0",	20 + GP_REG_FIRST },					\
  { "s1",	21 + GP_REG_FIRST },					\
  { "s2",	22 + GP_REG_FIRST },					\
  { "s3",	23 + GP_REG_FIRST },					\
  { "s4",	24 + GP_REG_FIRST },					\
  { "s5",	25 + GP_REG_FIRST },					\
  { "s6",	26 + GP_REG_FIRST },					\
  { "s7",	27 + GP_REG_FIRST },					\
  { "s8",	28 + GP_REG_FIRST },					\
  { "s9",	29 + GP_REG_FIRST },					\
  { "s10",	30 + GP_REG_FIRST },					\
  { "s11",	31 + GP_REG_FIRST },					\
  { "s12",	29 + GP_REG_FIRST },					\
  { "s13",	30 + GP_REG_FIRST }					\
}

/* Define results of standard character escape sequences.  */
/* Removed since defined in Default */
/*
  #define TARGET_BELL	007
  #define TARGET_BS	010
  #define TARGET_TAB	011
  #define TARGET_NEWLINE	012
  #define TARGET_VT	013
  #define TARGET_FF	014
  #define TARGET_CR	015
*/

/* A C compound statement to output to stdio stream STREAM the
   assembler syntax for an instruction operand X.  X is an RTL
   expression.

   CODE is a value that can be used to specify one of several ways
   of printing the operand.  It is used when identical operands
   must be printed differently depending on the context.  CODE
   comes from the `%' specification that was used to request
   printing of the operand.  If the specification was just `%DIGIT'
   then CODE is 0; if the specification was `%LTR DIGIT' then CODE
   is the ASCII code for LTR.

   If X is a register, this macro should print the register's name.
   The names can be found in an array `reg_names' whose type is
   `char *[]'.  `reg_names' is initialized from `REGISTER_NAMES'.

   When the machine description has a specification `%PUNCT' (a `%'
   followed by a punctuation character), this macro is called with
   a null pointer for X and the punctuation character for CODE.

   See microblaze.c for the MICROBLAZE specific codes.  */

#define PRINT_OPERAND(FILE, X, CODE) print_operand (FILE, X, CODE)

/* A C expression which evaluates to true if CODE is a valid
   punctuation character for use in the `PRINT_OPERAND' macro.  If
   `PRINT_OPERAND_PUNCT_VALID_P' is not defined, it means that no
   punctuation characters (except for the standard one, `%') are
   used in this way.  */

#define PRINT_OPERAND_PUNCT_VALID_P(CODE) microblaze_print_operand_punct[CODE]

/* A C compound statement to output to stdio stream STREAM the
   assembler syntax for an instruction operand that is a memory
   reference whose address is ADDR.  ADDR is an RTL expression.

   On some machines, the syntax for a symbolic address depends on
   the section that the address refers to.  On these machines,
   define the macro `ENCODE_SECTION_INFO' to store the information
   into the `symbol_ref', and then check for it here.  */

#define PRINT_OPERAND_ADDRESS(FILE, ADDR) print_operand_address (FILE, ADDR)


/* A C statement, to be executed after all slot-filler instructions
   have been output.  If necessary, call `dbr_sequence_length' to
   determine the number of slots filled in a sequence (zero if not
   currently outputting a sequence), to decide how many no-ops to
   output, or whatever.

   Don't define this macro if it has nothing to do, but it is
   helpful in reading assembly output if the extent of the delay
   sequence is made explicit (e.g. with white space).

   Note that output routines for instructions with delay slots must
   be prepared to deal with not being output as part of a sequence
   (i.e.  when the scheduling pass is not run, or when no slot
   fillers could be found.)  The variable `final_sequence' is null
   when not processing a sequence, otherwise it contains the
   `sequence' rtx being output.  */

#define DBR_OUTPUT_SEQEND(STREAM)					\
do									\
  {									\
    if (set_nomacro > 0 && --set_nomacro == 0)				\
      {}								\
									\
    if (set_noreorder > 0 && --set_noreorder == 0)			\
      {}								\
									\
    dslots_jump_filled++;						\
    fputs ("\n", STREAM);						\
  }									\
while (0)


/* How to tell the debugger about changes of source files.  Note, the
   microblaze ECOFF format cannot deal with changes of files inside of
   functions, which means the output of parser generators like bison
   is generally not debuggable without using the -l switch.  Lose,
   lose, lose.  Silicon graphics seems to want all .file's hardwired
   to 1.  */

#ifndef SET_FILE_NUMBER
#define SET_FILE_NUMBER() ++num_source_filenames
#endif

#define ASM_OUTPUT_SOURCE_FILENAME(STREAM, NAME)			\
  microblaze_output_filename (STREAM, NAME)

#define ASM_OUTPUT_FILENAME(STREAM, NUM_SOURCE_FILENAMES, NAME) \
do								\
  {								\
    fprintf (STREAM, "\t.file\t%d ", NUM_SOURCE_FILENAMES);	\
    output_quoted_string (STREAM, NAME);			\
    fputs ("\n", STREAM);					\
  }								\
while (0)

/* This is how to output a note the debugger telling it the line number
   to which the following sequence of instructions corresponds.
   Silicon graphics puts a label after each .loc.  */

#ifndef LABEL_AFTER_LOC
#define LABEL_AFTER_LOC(STREAM)
#endif

#define ASM_OUTPUT_SOURCE_LINE(STREAM, LINE, COUNTER)		\
  microblaze_output_lineno (STREAM, LINE)

/* The MICROBLAZE implementation uses some labels for its own purpose.  The
   following lists what labels are created, and are all formed by the
   pattern $L[a-z].*.  The machine independent portion of GCC creates
   labels matching:  $L[A-Z][0-9]+ and $L[0-9]+.

   LM[0-9]+	Silicon Graphics/ECOFF stabs label before each stmt.
   $Lb[0-9]+	Begin blocks for MICROBLAZE debug support
   $Lc[0-9]+	Label for use in s<xx> operation.
   $Le[0-9]+	End blocks for MICROBLAZE debug support
   $Lp\..+		Half-pic labels. */

/* This is how to output the definition of a user-level label named NAME,
   such as the label on a static function or variable NAME.

   If we are optimizing the gp, remember that this label has been put
   out, so we know not to emit an .extern for it in microblaze_asm_file_end.
   We use one of the common bits in the IDENTIFIER tree node for this,
   since those bits seem to be unused, and we don't have any method
   of getting the decl nodes from the name.  */

#define ASM_OUTPUT_LABEL(STREAM,NAME)					\
do {									\
  assemble_name (STREAM, NAME);						\
  fputs (":\n", STREAM);						\
} while (0)


/* A C statement (sans semicolon) to output to the stdio stream
   STREAM any text necessary for declaring the name NAME of an
   initialized variable which is being defined.  This macro must
   output the label definition (perhaps using `ASM_OUTPUT_LABEL'). 
   The argument DECL is the `VAR_DECL' tree node representing the
   variable.

   If this macro is not defined, then the variable name is defined
   in the usual manner as a label (by means of `ASM_OUTPUT_LABEL').  */

#define ASM_DECLARE_OBJECT_NAME(STREAM, NAME, DECL)			\
do									\
 {									\
   microblaze_declare_object (STREAM, NAME, "", ":\n", 0);		\
   HALF_PIC_DECLARE (NAME);						\
 }									\
while (0)

/* This says how to define an aligned common symbol */
#define ASM_OUTPUT_ALIGNED_COMMON(STREAM, NAME, SIZE, ALIGNMENT)   \
    (microblaze_declare_comm_object (STREAM, NAME, "\n\t.comm\t", ",%u,%u\n", (SIZE), (ALIGNMENT)))

/* This says how to define an aligned static common symbol */
#define ASM_OUTPUT_ALIGNED_LOCAL(STREAM, NAME, SIZE, ALIGNMENT)     \
    (microblaze_declare_comm_object (STREAM, NAME, "\n\t.lcomm\t", ",%u,%u\n", (SIZE), (ALIGNMENT)))

/* This says how to output an aligned BSS symbol */
#define ASM_OUTPUT_ALIGNED_BSS(STREAM, DECL, NAME, SIZE, ALIGNMENT) \
    asm_output_aligned_bss (STREAM, DECL, NAME, SIZE, ALIGNMENT)

/* This says how to output an external.  It would be possible not to
   output anything and let undefined symbol become external. However
   the assembler uses length information on externals to allocate in
   data/sdata bss/sbss, thereby saving exec time.  */

#define ASM_OUTPUT_EXTERNAL(STREAM,DECL,NAME) \
  microblaze_output_external(STREAM,DECL,NAME)

/* This is how to declare a function name.  The actual work of
   emitting the label is moved to function_prologue, so that we can
   get the line number correctly emitted before the .ent directive,
   and after any .file directives.

   Also, switch files if we are optimizing the global pointer.  */

/* Older FUNCTION_NAME macro. has been replaced by a new one for the
   .size issue */
#if 0 
#define ASM_DECLARE_FUNCTION_NAME(STREAM,NAME,DECL)			\
{									\
  extern FILE *asm_out_text_file;					\
  if (TARGET_GP_OPT )					\
    {									\
      STREAM = asm_out_text_file;					\
      /* ??? text_section gets called too soon.  If the previous	\
	 function is in a special section and we're not, we have	\
	 to switch back to the text section.  We can't call		\
	 text_section again as gcc thinks we're already there.  */	\
      /* ??? See varasm.c.  There are other things that get output	\
	 too early, like alignment (before we've switched STREAM).  */	\
      if (DECL_SECTION_NAME (DECL) == NULL_TREE)			\
	fprintf (STREAM, "%s\n", TEXT_SECTION_ASM_OP);			\
    }									\
									\
  HALF_PIC_DECLARE (NAME);						\
}
#endif

#define ASM_DECLARE_FUNCTION_NAME(STREAM,NAME,DECL)                         \
{                                                                           \
}


/* This is how to store into the string LABEL
   the symbol_ref name of an internal numbered label where
   PREFIX is the class of label and NUM is the number within the class.
   This is suitable for output with `assemble_name'.  */

#define ASM_GENERATE_INTERNAL_LABEL(LABEL,PREFIX,NUM)			\
  sprintf ((LABEL), "*%s%s%ld", (LOCAL_LABEL_PREFIX), (PREFIX), (long)(NUM))

/* This is how to output an assembler line defining a `double' constant.  */

#define ASM_OUTPUT_DOUBLE(STREAM,VALUE)					\
  microblaze_output_double (STREAM, VALUE)


/* This is how to output an assembler line defining a `float' constant.  */

#define ASM_OUTPUT_FLOAT(STREAM,VALUE)					\
  microblaze_output_float (STREAM, VALUE)


/* This is how to output an assembler line defining an `int' constant.  */

#define ASM_OUTPUT_INT(STREAM,VALUE)					\
do {									\
  fprintf (STREAM, "\t.data32\t");					\
  output_addr_const (STREAM, (VALUE));					\
  fprintf (STREAM, "\n");						\
} while (0)

/* Likewise for 64 bit, `char' and `short' constants.  */

#define ASM_OUTPUT_DOUBLE_INT(STREAM,VALUE)				\
do {									\
      assemble_integer (operand_subword ((VALUE), 0, 0, DImode),	\
			UNITS_PER_WORD, 1);				\
      assemble_integer (operand_subword ((VALUE), 1, 0, DImode),	\
			UNITS_PER_WORD, 1);				\
} while (0)

#define ASM_OUTPUT_SHORT(STREAM,VALUE)					\
{									\
  fprintf (STREAM, "\t.data16\t");					\
  output_addr_const (STREAM, (VALUE));					\
  fprintf (STREAM, "\n");						\
}

#define ASM_OUTPUT_CHAR(STREAM,VALUE)					\
{									\
  fprintf (STREAM, "\t.data8\t");					\
  output_addr_const (STREAM, (VALUE));					\
  fprintf (STREAM, "\n");						\
}

/* This is how to output an assembler line for a numeric constant byte.  */

#define ASM_OUTPUT_BYTE(STREAM,VALUE)					\
  fprintf (STREAM, "\t.data8\t0x%x\n", (VALUE))

/* This is how to output an element of a case-vector that is absolute.  */

/*   Changed .word to .gpword*/
#define ASM_OUTPUT_ADDR_VEC_ELT(STREAM, VALUE)				\
  fprintf (STREAM, "\t%s\t%sL%d\n",					\
	   ".gpword",                                                   \
	   LOCAL_LABEL_PREFIX, VALUE)

/*   Changed .word to .gpword*/

/* This is how to output an element of a case-vector that is relative.
   This is used for pc-relative code (e.g. when TARGET_ABICALLS or
   TARGET_EMBEDDED_PIC).  */

#define ASM_OUTPUT_ADDR_DIFF_ELT(STREAM, BODY, VALUE, REL)		\
do {									\
  if (TARGET_EMBEDDED_PIC)						\
    fprintf (STREAM, "\t%s\t%sL%d-%sLS%d\n",				\
	     ".gpword",                                                 \
	     LOCAL_LABEL_PREFIX, VALUE, LOCAL_LABEL_PREFIX, REL);	\
  else                                                                  \
    fprintf (STREAM, "\t%s\t%sL%d\n",					\
	     ".gpword",                                                 \
	     LOCAL_LABEL_PREFIX, VALUE);				\
} while (0)

/* When generating embedded PIC code we want to put the jump
   table in the .text section.  In all other cases, we want to put the
   jump table in the .rdata section.  Unfortunately, we can't use
   JUMP_TABLES_IN_TEXT_SECTION, because it is not conditional.
   Instead, we use ASM_OUTPUT_CASE_LABEL to switch back to the .text
   section if appropriate.  */
#define ASM_OUTPUT_CASE_LABEL(FILE, PREFIX, NUM, INSN)			\
do {									\
  if (TARGET_EMBEDDED_PIC )                                             \
    function_section (current_function_decl);				\
  (*targetm.asm_out.internal_label) (FILE, PREFIX, NUM);		\
} while (0)

/* This is how to output an assembler line
   that says to advance the location counter
   to a multiple of 2**LOG bytes.  */

#define ASM_OUTPUT_ALIGN(STREAM,LOG)					\
  fprintf (STREAM, "\t.align\t%d\n", (LOG))

/* This is how to output an assembler line to advance the location
   counter by SIZE bytes.  */

#define ASM_OUTPUT_SKIP(STREAM,SIZE)					\
  fprintf (STREAM, "\t.space\t%u\n", (SIZE))

/* This is how to output a string.  */
/*   Changed the way the string is to be printed in the assembly file. It
     will output .data8 words instead of .ascii and the string
*/
#define ASM_OUTPUT_ASCII(STREAM, STRING, LEN)				\
do {									\
if(TARGET_MICROBLAZE_ASM){                                                    \
  register int i, c, len = (LEN) ;  				        \
  register unsigned char *string = (unsigned char *)(STRING);		\
  for (i = 0; i < len; i++)						\
       fprintf((STREAM),".data8 %d\n",string[i]);			\
  }                                                                     \
 else /* If not MICROBLAZE_ASM print ascii */                                 \
  {/* Original code for ASM_OUTPUT_ASCII */                             \
  register int i, c, len = (LEN), cur_pos = 17;				\
  register unsigned char *string = (unsigned char *)(STRING);		\
  fprintf ((STREAM), "\t.ascii\t\"");					\
  for (i = 0; i < len; i++)						\
    {									\
      register int c = string[i];					\
									\
      switch (c)							\
	{								\
	case '\"':							\
	case '\\':							\
	  putc ('\\', (STREAM));					\
	  putc (c, (STREAM));						\
	  cur_pos += 2;							\
	  break;							\
									\
	case TARGET_NEWLINE:						\
	  fputs ("\\n", (STREAM));					\
	  if (i+1 < len							\
	      && (((c = string[i+1]) >= '\040' && c <= '~')		\
		  || c == TARGET_TAB))					\
	    cur_pos = 32767;		/* break right here */		\
	  else								\
	    cur_pos += 2;						\
	  break;							\
									\
	case TARGET_TAB:						\
	  fputs ("\\t", (STREAM));					\
	  cur_pos += 2;							\
	  break;							\
									\
	case TARGET_FF:							\
	  fputs ("\\f", (STREAM));					\
	  cur_pos += 2;							\
	  break;							\
									\
	case TARGET_BS:							\
	  fputs ("\\b", (STREAM));					\
	  cur_pos += 2;							\
	  break;							\
									\
	case TARGET_CR:							\
	  fputs ("\\r", (STREAM));					\
	  cur_pos += 2;							\
	  break;							\
									\
	default:							\
	  if (c >= ' ' && c < 0177)					\
	    {								\
	      putc (c, (STREAM));					\
	      cur_pos++;						\
	    }								\
	  else								\
	    {								\
	      fprintf ((STREAM), "\\%03o", c);				\
	      cur_pos += 4;						\
	    }								\
	}								\
									\
      if (cur_pos > 72 && i+1 < len)					\
	{								\
	  cur_pos = 17;							\
	  fprintf ((STREAM), "\"\n\t.ascii\t\"");			\
	}								\
    }									\
   fprintf ((STREAM), "\"\n");						\
  } 								        \
} while (0)

/* Handle certain cpp directives used in header files on sysV.  */
/* GCC 3.4.1 - Poisoned */
/* #define SCCS_DIRECTIVE */

/* Output #ident as a in the read-only data section.  */
#define ASM_OUTPUT_IDENT(FILE, STRING)					\
{									\
  char *p = STRING;							\
  int size = strlen (p) + 1;						\
  if(size <= microblaze_section_threshold)				\
     sdata2_section ();							\
  else 									\
     rodata_section ();							\
  assemble_string (p, size);						\
}

/* Default to -G 8 */


#ifndef MICROBLAZE_DEFAULT_GVALUE
#define MICROBLAZE_DEFAULT_GVALUE 8
#endif

/* Given a decl node or constant node, choose the section to output it in
   and select that section.  */

/* Store in OUTPUT a string (made with alloca) containing
   an assembler-name for a local static variable named NAME.
   LABELNO is an integer which is different for each call.  */

#define ASM_FORMAT_PRIVATE_NAME(OUTPUT, NAME, LABELNO)			\
( (OUTPUT) = (char *) alloca (strlen ((NAME)) + 10),			\
  sprintf ((OUTPUT), "%s.%d", (NAME), (LABELNO)))

/* How to start an assembler comment.
   The leading space is important (the microblaze native assembler requires it).  */

#ifndef ASM_COMMENT_START
#define ASM_COMMENT_START " #"
#endif



/* Macros for microblaze-tfile.c to encapsulate stabs in ECOFF, and for
   and microblaze-tdump.c to print them out.

   These must match the corresponding definitions in gdb/microblazeread.c.
   Unfortunately, gcc and gdb do not currently share any directories. */

#define CODE_MASK 0x8F300
#define MICROBLAZE_IS_STAB(sym) (((sym)->index & 0xFFF00) == CODE_MASK)
#define MICROBLAZE_MARK_STAB(code) ((code)+CODE_MASK)
#define MICROBLAZE_UNMARK_STAB(code) ((code)-CODE_MASK)

#undef TARGET_FP_CALL_32

#define BSS_VAR         1
#define SBSS_VAR        2
#define DATA_VAR        4
#define SDATA_VAR       5
#define RODATA_VAR      6
#define SDATA2_VAR      7

/* These definitions are used in combination with the shift_type flag in the rtl */

#define SHIFT_CONST     1
#define SHIFT_REG       2
#define USE_ADDK        3

/* handle interrupt attribute */

/*extern int microblaze_valid_machine_decl_attribute ();*/
/*
  #define VALID_MACHINE_DECL_ATTRIBUTE(DECL, ATTRIBUTES, IDENTIFIER, ARGS) \
  microblaze_valid_machine_decl_attribute (DECL, ATTRIBUTES, IDENTIFIER, ARGS)
*/
extern int interrupt_handler;
extern int save_volatiles;

/*int microblaze_is_interrupt_handler();*/

/*extern int dbl_register_operand();*/
/*extern int shiftdone;*/

#define INTERRUPT_HANDLER_NAME "_interrupt_handler"

/* these #define added for C++ */
#ifdef OBJECT_FORMAT_ELF
#define UNALIGNED_SHORT_ASM_OP          ".data16"
#define UNALIGNED_INT_ASM_OP            ".data32"
#define UNALIGNED_DOUBLE_INT_ASM_OP     ".data8"
#endif /* OBJECT_FORMAT_ELF */

#define ASM_BYTE_OP                     ".data8"

/* the following #defines are used in the headers files. Always retain these */

/* Added for declaring size at the end of the function*/
#undef ASM_DECLARE_FUNCTION_SIZE
#define ASM_DECLARE_FUNCTION_SIZE(FILE, FNAME, DECL)			\
  do {									\
    if (!flag_inhibit_size_directive)					\
      {									\
        char label[256];						\
	static int labelno;						\
	labelno++;							\
	ASM_GENERATE_INTERNAL_LABEL (label, "Lfe", labelno);		\
        (*targetm.asm_out.internal_label) (FILE, "Lfe", labelno);	\
	fprintf (FILE, "\t%s\t ", SIZE_ASM_OP);				\
	assemble_name (FILE, (FNAME));					\
        fprintf (FILE, ",");						\
	assemble_name (FILE, label);					\
        fprintf (FILE, "-");						\
	assemble_name (FILE, (FNAME));					\
	putc ('\n', FILE);						\
      }									\
  } while (0)

/*  ASM_DECLARE_FUNCTION_NAME (asm_out_file, fnname, current_function_decl);*/
#if 0 
#define ASM_DECLARE_FUNCTION_NAME(FILE,NAME,DECL)		\
do{ \
   if (!flag_inhibit_size_directive) \
   { \
      fputs ("\t.ent\t", FILE); \
      if (interrupt_handler && strcmp(INTERRUPT_HANDLER_NAME,NAME)) \
         fputs("_interrupt_handler",FILE);\
      else\
         assemble_name (FILE, NAME);\
      fputs ("\n", FILE);\
   }\
\
   assemble_name (FILE, NAME);\
   fputs (":\n", FILE);\
   if (interrupt_handler && strcmp(INTERRUPT_HANDLER_NAME,NAME))\
      fputs ("_interrupt_handler:\n",FILE);\
   \
} while(0) 
#endif

#define GLOBAL_ASM_OP "\t.globl\t"

#define MAX_OFILE_ALIGNMENT (32768*8)

/* A C statement to output something to the assembler file to switch to section
   NAME for object DECL which is either a FUNCTION_DECL, a VAR_DECL or
   NULL_TREE.  Some target formats do not support arbitrary sections.  Do not
   define this macro in such cases.  */

#define ASM_OUTPUT_SECTION_NAME(F, DECL, NAME, RELOC) \
do {								\
  extern FILE *asm_out_text_file;				\
  if ((DECL) && TREE_CODE (DECL) == FUNCTION_DECL)		\
    fprintf (asm_out_text_file, "\t.section %s,\"ax\",@progbits\n", (NAME)); \
  else if ((DECL) && DECL_READONLY_SECTION (DECL, RELOC))	\
    fprintf (F, "\t.section %s,\"a\",@progbits\n", (NAME));	\
  else								\
    fprintf (F, "\t.section %s,\"aw\",@progbits\n", (NAME));	\
} while (0)

/* The following macro defines the format used to output the second
   operand of the .type assembler directive.  Different svr4 assemblers
   expect various different forms for this operand.  The one given here
   is just a default.  You may need to override it in your machine-
   specific tm.h file (depending upon the particulars of your assembler).  */

#define TYPE_OPERAND_FMT        "@%s"

/* Define the strings used for the special svr4 .type and .size directives.
   These strings generally do not vary from one system running svr4 to
   another, but if a given system (e.g. m88k running svr) needs to use
   different pseudo-op names for these, they may be overridden in the
   file which includes this one.  */

#undef TYPE_ASM_OP
#undef SIZE_ASM_OP
#define TYPE_ASM_OP	".type"
#define SIZE_ASM_OP	".size"

/* These macros generate the special .type and .size directives which
   are used to set the corresponding fields of the linker symbol table
   entries in an ELF object file under SVR4.  These macros also output
   the starting labels for the relevant functions/objects.  */

/* Write the extra assembler code needed to declare an object properly.  */

#undef ASM_DECLARE_OBJECT_NAME
#define ASM_DECLARE_OBJECT_NAME(FILE, NAME, DECL)			\
  do {									\
    fprintf (FILE, "\t%s\t ", TYPE_ASM_OP);				\
    assemble_name (FILE, NAME);						\
    putc (',', FILE);							\
    fprintf (FILE, TYPE_OPERAND_FMT, "object");				\
    putc ('\n', FILE);							\
    size_directive_output = 0;						\
    if (!flag_inhibit_size_directive && DECL_SIZE (DECL))		\
      {									\
	size_directive_output = 1;					\
	fprintf (FILE, "\t%s\t ", SIZE_ASM_OP);				\
	assemble_name (FILE, NAME);					\
	fprintf (FILE, ",%d\n",  int_size_in_bytes (TREE_TYPE (DECL)));	\
      }									\
    microblaze_declare_object (FILE, NAME, "", ":\n", 0);			\
  } while (0)

/* Output the size directive for a decl in rest_of_decl_compilation
   in the case where we did not do so before the initializer.
   Once we find the error_mark_node, we know that the value of
   size_directive_output was set
   by ASM_DECLARE_OBJECT_NAME when it was run for the same decl.  */

#undef ASM_FINISH_DECLARE_OBJECT
#define ASM_FINISH_DECLARE_OBJECT(FILE, DECL, TOP_LEVEL, AT_END)	 \
do {									 \
     char *name = XSTR (XEXP (DECL_RTL (DECL), 0), 0);			 \
     if (!flag_inhibit_size_directive && DECL_SIZE (DECL)		 \
         && ! AT_END && TOP_LEVEL					 \
	 && DECL_INITIAL (DECL) == error_mark_node			 \
	 && !size_directive_output)					 \
       {								 \
	 size_directive_output = 1;					 \
	 fprintf (FILE, "\t%s\t ", SIZE_ASM_OP);			 \
	 assemble_name (FILE, name);					 \
	 fprintf (FILE, ",%d\n", int_size_in_bytes (TREE_TYPE (DECL)));  \
       }								 \
   } while (0)

#define ASM_OUTPUT_DEF(FILE,LABEL1,LABEL2)                            \
 do { fputc ( '\t', FILE);                                            \
      assemble_name (FILE, LABEL1);                                   \
      fputs ( " = ", FILE);                                           \
      assemble_name (FILE, LABEL2);                                   \
      fputc ( '\n', FILE);                                            \
 } while (0)


/* No support for weak in MicroBlaze. Hence commenting out this part */

#define ASM_WEAKEN_LABEL(FILE,NAME) ASM_OUTPUT_WEAK_ALIAS(FILE,NAME,0)
#define ASM_OUTPUT_WEAK_ALIAS(FILE,NAME,VALUE)	\
  do {						\
  if (TARGET_GAS)                               \
  fputs ("\t.weak\t", FILE);		\
  else                                          \
  fputs ("\t.weakext\t", FILE);		\
  assemble_name (FILE, NAME);			\
  if (VALUE)					\
  {						\
  fputc (' ', FILE);			\
  assemble_name (FILE, VALUE);		\
  }						\
  fputc ('\n', FILE);				\
  } while (0)


/* Also take care of the changes in the end of this macro */
/* Real prefixes */
/*  static char *prefixes[4][2] = {					   \
    { ".text.", ".gnu.linkonce.t." },					   \
    { ".rodata.", ".gnu.linkonce.r." },					   \
    { ".data.", ".gnu.linkonce.d." },					   \
    { ".sdata.", ".gnu.linkonce.s." }					   \
  };									   \
*/
#define MAKE_DECL_ONE_ONLY(DECL) (DECL_WEAK (DECL) = 1)
#undef UNIQUE_SECTION_P
#define UNIQUE_SECTION_P(DECL) (DECL_ONE_ONLY (DECL))

#undef TARGET_ASM_NAMED_SECTION
#define TARGET_ASM_NAMED_SECTION        default_elf_asm_named_section

#undef TARGET_UNIQUE_SECTION
#define TARGET_UNIQUE_SECTION microblaze_unique_section

/* Define the strings to put out for each section in the object file.  
   
   Note: For ctors/dtors, we want to give these sections the SHF_WRITE attribute 
   to allow shared libraries to patch/resolve addresses into these locations.
   On Microblaze, there is no concept of shared libraries yet, so this is for
   future use. */
#define TEXT_SECTION_ASM_OP	"\t.text"	
#define DATA_SECTION_ASM_OP	"\t.data"	
#define READONLY_DATA_SECTION_ASM_OP    \
                                "\t.rodata"
#define BSS_SECTION_ASM_OP      "\t.bss"        
#define CTORS_SECTION_ASM_OP    "\t.section\t.ctors,\"aw\""
#define DTORS_SECTION_ASM_OP    "\t.section\t.dtors,\"aw\""
#define INIT_SECTION_ASM_OP     "\t.section\t.init,\"ax\""
#define FINI_SECTION_ASM_OP     "\t.section\t.fini,\"ax\""

#define SDATA_SECTION_ASM_OP	"\t.sdata"	/* Small RW initialized data   */
#define SDATA2_SECTION_ASM_OP	"\t.sdata2"	/* Small RO initialized data   */ 
#define SBSS_SECTION_ASM_OP     "\t.sbss"       /* Small RW uninitialized data */
#define SBSS2_SECTION_ASM_OP    "\t.sbss2"      /* Small RO uninitialized data */

#define HOT_TEXT_SECTION_NAME   ".text.hot"
#define UNLIKELY_EXECUTED_TEXT_SECTION_NAME \
                                ".text.unlikely"

#define READONLY_DATA_SECTION   rodata_section
#define SDATA_SECTION           sdata_section
#define READONLY_SDATA_SECTION  sdata2_section
#define SBSS_SECTION            sbss_section
#define READONLY_SBSS_SECTION   sbss2_section
#define BSS_SECTION             bss_section

/* A list of other sections which the compiler might be "in" at any
   given time.  */
#undef EXTRA_SECTIONS
#define EXTRA_SECTIONS in_rodata, in_sdata, in_sdata2, in_sbss, in_sbss2, in_init, in_fini

#undef EXTRA_SECTION_FUNCTIONS
#define EXTRA_SECTION_FUNCTIONS                                                     \
  SECTION_FUNCTION_TEMPLATE(rodata_section, in_rodata, READONLY_DATA_SECTION_ASM_OP)\
  SECTION_FUNCTION_TEMPLATE(sdata_section,  in_sdata,  SDATA_SECTION_ASM_OP)        \
  SECTION_FUNCTION_TEMPLATE(sdata2_section, in_sdata2, SDATA2_SECTION_ASM_OP)       \
  SECTION_FUNCTION_TEMPLATE(sbss_section,   in_sbss,   SBSS_SECTION_ASM_OP)         \
  SECTION_FUNCTION_TEMPLATE(sbss2_section,  in_sbss2,  SBSS2_SECTION_ASM_OP)        \
  SECTION_FUNCTION_TEMPLATE(init_section,  in_init,  INIT_SECTION_ASM_OP)           \
  SECTION_FUNCTION_TEMPLATE(fini_section,  in_fini,  FINI_SECTION_ASM_OP)        

#define SECTION_FUNCTION_TEMPLATE(FN, ENUM, OP)                                 \
void FN ()                                                                      \
{                                                                               \
  if (in_section != ENUM)                                                       \
    {                                                                           \
      fprintf (asm_out_file, "%s\n", OP);                                       \
      in_section = ENUM;                                                        \
    }                                                                           \
}

/* We do this to save a few 10s of code space that would be taken up
   by the call_FUNC () wrappers, used by the generic CRT_CALL_STATIC_FUNCTION
   definition in crtstuff.c */
#define CRT_CALL_STATIC_FUNCTION(SECTION_OP, FUNC)	\
    asm ( SECTION_OP "\n"                               \
          "\tbralid   r15, " #FUNC "\n\t nop\n"         \
          TEXT_SECTION_ASM_OP);

/* #define CTOR_LIST_BEGIN                                 \ */
/* asm (CTORS_SECTION_ASM_OP);                             \ */
/* func_ptr __CTOR_LIST__[1] = { (func_ptr) (-1) } */
 
/* #define CTOR_LIST_END                                   \ */
/* asm (CTORS_SECTION_ASM_OP);                             \ */
/* func_ptr __CTOR_END__[1] = { (func_ptr) 0 }; */
 
/* #define DTOR_LIST_BEGIN                                 \ */
/* asm (DTORS_SECTION_ASM_OP);                             \ */
/* func_ptr __DTOR_LIST__[1] = { (func_ptr) (-1) } */

/* #define DTOR_LIST_END                                   \ */
/* asm (DTORS_SECTION_ASM_OP);                             \ */
/* func_ptr __DTOR_END__[1] = { (func_ptr) 0 }; */

/* Don't set the target flags, this is done by the linker script */
#undef LIB_SPEC
#define LIB_SPEC "%{!pg:%{!nostdlib:%{!Zxl-no-libxil:-( -lxil -lc -lm -)}}} %{pg:%{!nostdlib:-( -lxilprofile -lxil -lc -lm -)}} %{Zxl-no-libxil:%{!nostdlib:-( -lc -lm -)}}"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "crtend.o%s crtn.o%s"


#define STARTFILE_EXECUTABLE_SPEC   "crt0.o%s crti.o%s crtbegin.o%s"
#define STARTFILE_XMDSTUB_SPEC      "crt1.o%s crti.o%s crtbegin.o%s"
#define STARTFILE_BOOTSTRAP_SPEC    "crt2.o%s crti.o%s crtbegin.o%s"
#define STARTFILE_NOVECTORS_SPEC    "crt3.o%s crti.o%s crtbegin.o%s"
#define STARTFILE_XILKERNEL_SPEC    "crt4.o%s crti.o%s crtbegin.o%s"
#define STARTFILE_CRTINIT_SPEC      "%{!pg: %{!mno-clearbss: crtinit.o%s} %{mno-clearbss: sim-crtinit.o%s}} \
                                     %{pg: %{!mno-clearbss: pgcrtinit.o%s} %{mno-clearbss: sim-pgcrtinit.o%s}}"


#define STARTFILE_DEFAULT_SPEC      STARTFILE_EXECUTABLE_SPEC


#undef SUBTARGET_EXTRA_SPECS
#define	SUBTARGET_EXTRA_SPECS						\
  { "startfile_executable",	STARTFILE_EXECUTABLE_SPEC },		\
  { "startfile_xmdstub",	STARTFILE_XMDSTUB_SPEC },		\
  { "startfile_bootstrap",	STARTFILE_BOOTSTRAP_SPEC },		\
  { "startfile_novectors",	STARTFILE_NOVECTORS_SPEC },		\
  { "startfile_xilkernel",	STARTFILE_XILKERNEL_SPEC },		\
  { "startfile_crtinit",        STARTFILE_CRTINIT_SPEC },               \
  { "startfile_default",	STARTFILE_DEFAULT_SPEC },		

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC  "\
%{Zxl-mode-executable   : %(startfile_executable)  ; \
  Zxl-mode-xmdstub      : %(startfile_xmdstub)     ; \
  Zxl-mode-bootstrap    : %(startfile_bootstrap)   ; \
  Zxl-mode-novectors    : %(startfile_novectors)   ; \
  Zxl-mode-xilkernel    : %(startfile_xilkernel)   ; \
                        : %(startfile_default)       \
} \
%(startfile_crtinit)"

