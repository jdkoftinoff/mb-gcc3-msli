/* tc-microblaze.c -- Assemble code for Xilinx MicroBlaze
   Copyright (C) 1999, 2000 Free Software Foundation.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

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

#include <stdio.h>
#include "as.h"
#include "bfd.h"
#include "subsegs.h"
#define DEFINE_TABLE
#include "../opcodes/microblaze-opc.h"
#include "../opcodes/microblaze-opcm.h"
#include <ctype.h>
#include <string.h>
#include "aout/stab_gnu.h"

#ifndef streq
#define streq(a,b) (strcmp (a, b) == 0)
#endif

static char * parse_reg    PARAMS ((char *, unsigned *));
static char * parse_exp    PARAMS ((char *, expressionS *));
static char * parse_imm   PARAMS ((char *, expressionS *, int, int));
static void   microblaze_s_file    PARAMS ((int));
static void   microblaze_s_line    PARAMS ((int));
static void   microblaze_s_stabs   PARAMS ((int));
static void   microblaze_s_text    PARAMS ((int));
static void   microblaze_s_data    PARAMS ((int));
static void   microblaze_s_sdata    PARAMS ((int));
static void   microblaze_s_rdata   PARAMS ((int));
static void   microblaze_s_bss     PARAMS ((int));
static void   microblaze_s_comm    PARAMS ((int));
static void   microblaze_s_func    PARAMS ((int));
static void   microblaze_stabs_generate_asm_func PARAMS ((const char *, const char *));
static void   microblaze_stabs_generate_asm_endfunc PARAMS ((const char *, const char *));
static void   microblaze_s_weakext   PARAMS ((int));
static void   microblaze_s_lcomm PARAMS ((int));

/* Modified obj_elf_common() function for MB */
static void   microblaze_elf_common PARAMS ((int));

extern char *demand_copy_string PARAMS ((int *lenP));
/* Several places in this file insert raw instructions into the
   object. They should generate the instruction
   and then use these four macros to crack the instruction value into
   the appropriate byte values.  */
#define	INST_BYTE0(x)  (target_big_endian ? (((x) >> 24) & 0xFF) : ((x) & 0xFF))
#define	INST_BYTE1(x)  (target_big_endian ? (((x) >> 16) & 0xFF) : (((x) >> 8) & 0xFF))
#define	INST_BYTE2(x)  (target_big_endian ? (((x) >> 8) & 0xFF) : (((x) >> 16) & 0xFF))
#define	INST_BYTE3(x)  (target_big_endian ? ((x) & 0xFF) : (((x) >> 24) & 0xFF))

/* This array holds the chars that always start a comment.  If the
   pre-processor is disabled, these aren't very useful.  */
const char comment_chars[] = "#";

const char line_separator_chars[] = "";

/* This array holds the chars that only start a comment at the beginning of
   a line.
*/
const char line_comment_chars[] = "#";

const int md_reloc_size = 8; /* Size of relocation record */


/* Chars that can be used to separate mant
   from exp in floating point numbers.  */
const char EXP_CHARS[] = "eE";

/* Chars that mean this number is a floating point constant */
/* As in 0f12.456 */
/* or    0d1.2345e12 */
const char FLT_CHARS[] = "rRsSfFdDxXpP";

/* INST_PC_OFFSET and INST_NO_OFFSET are 0 and 1 */
#define UNDEFINED_PC_OFFSET  2
#define DEFINED_ABS_SEGMENT  3
#define DEFINED_PC_OFFSET    4
#define DEFINED_RO_SEGMENT   5
#define DEFINED_RW_SEGMENT   6
#define LARGE_DEFINED_PC_OFFSET 7

/* Initialize the relax table */
const relax_typeS md_relax_table[] = 
{
   {          1,          1,                0, 0 },  /* 0: unused */  
   {          1,          1,                0, 0 },  /* 1: unused */  
   {          1,          1,                0, 0 },  /* 2: unused */  
   {          1,          1,                0, 0 },  /* 3: unused */  
   {      32767,   -32768, INST_WORD_SIZE, LARGE_DEFINED_PC_OFFSET }, /* 4: DEFINED_PC_OFFSET */  
   {    1,     1,       0, 0 },                      /* 5: unused */  
   {    1,     1,       0, 0 },                      /* 6: unused */  
   { 0x7fffffff, 0x80000000, INST_WORD_SIZE*2, 0 }     /* 7: LARGE_DEFINED_PC_OFFSET */  
};

static struct hash_control * opcode_hash_control;	/* Opcode mnemonics */

/*static int dont_use_small = 0; If 0, assume that data and comm section are small data sections */
static segT sbss_segment = 0; /* Small bss section */
static segT sbss2_segment = 0; /* Small bss section */
static segT sdata_segment = 0; /* Small data section */
static segT sdata2_segment = 0; /* Small read-only section */
static segT rodata_segment = 0; /* read-only section */

/* This table describes all the machine specific pseudo-ops the assembler
   has to support.  The fields are:
     Pseudo-op name without dot
     Function to call to execute this pseudo-op
     Integer arg to pass to the function   */
/* If the pseudo-op is not found in this table, it searches in the obj-elf.c, 
   and then in the read.c table */
const pseudo_typeS md_pseudo_table[] =
{
  //{"comm", microblaze_s_comm, 0}, 
  {"comm", microblaze_elf_common, 0}, 
  {"lcomm", microblaze_s_lcomm, 1}, 
  {"data", microblaze_s_data, 0},
  {"data8", cons, 1},      /* same as byte */
  {"data16", cons, 2},     /* same as hword */
  {"data32", cons, 4},     /* same as word */
  {"ent", s_func, 0}, /* treat ent as function entry point */
  {"end", microblaze_s_func, 1}, /* treat end as function end point */
  {"gpword", s_rva, 4}, /* gpword label => store resolved label address in data section */
  {"weakext", microblaze_s_weakext, 0},
   {"rodata", microblaze_s_rdata, 0}, 
   {"sdata2", microblaze_s_rdata, 1},
   {"sdata", microblaze_s_sdata, 0},
   {"bss", microblaze_s_bss, 0},
   {"sbss", microblaze_s_bss, 1},
   {"text", microblaze_s_text, 0},
   {"loc", microblaze_s_line, 0},
   {"file", microblaze_s_file, 0},
   {"word", cons, 4},
   {"frame", s_ignore, 0},
   {"mask", s_ignore, 0}, /* emitted by gcc */
   {NULL, NULL, 0}
};

/* Generate a symbol for stabs information */
void
microblaze_generate_symbol(sym)
   char *sym;
{
#define MICROBLAZE_FAKE_LABEL_NAME "XL0\001"
   static int microblaze_label_count;
   sprintf (sym, "%sL%d", MICROBLAZE_FAKE_LABEL_NAME, microblaze_label_count);
   ++microblaze_label_count;
}

/* The lcc loc directive is changed to an elf .line directive */
static void
microblaze_s_line (ignore)
   int ignore ATTRIBUTE_UNUSED;
{
  
   /* Skip over the first parameter */
   /*  get_absolute_expression();
       SKIP_WHITESPACE();
       if (*input_line_pointer == ',')
       input_line_pointer++;
       new_logical_line ((char *) NULL, (int) (get_absolute_expression ()));
       demand_empty_rest_of_line ();
   */

   char *hold;
   unsigned int lineno;
   char *buf;
   char sym[30];

   /* Rather than try to do this in some efficient fashion, we just
      generate a string and then parse it again.  That lets us use the
      existing stabs hook, which expect to see a string, rather than
      inventing new ones.  */

   get_absolute_expression();
   SKIP_WHITESPACE();
   if (*input_line_pointer == ',')
      input_line_pointer++;
   lineno = (int) get_absolute_expression ();
   demand_empty_rest_of_line ();

   hold = input_line_pointer;

   microblaze_generate_symbol(&sym);
   buf = (char *) xmalloc (100);
   sprintf (buf, "%d,0,%d,%s\n", N_SLINE, lineno, sym);

   input_line_pointer = buf;
   s_stab ('n');
   colon (sym);

   input_line_pointer = hold;
   free(buf);
}

/* The mb-gcc file has a file number: remove it, and pass the rest of the 
   line for regular .file processing */
static void
microblaze_s_file (ignore)
   int ignore ATTRIBUTE_UNUSED;
{

   /* Skip over the first parameter */
   get_absolute_expression();
   SKIP_WHITESPACE();
   if (*input_line_pointer == ',')
      input_line_pointer++;
   s_app_file(0); 
}

/* The function size stab - stabs "",36,0,0,$a-b
   gets converted into an absolute number.  If the relax
   option is used with the linker, this value is incorrect.
   Instead generate a stab of the form 
   .stabs "",36,0,0,$a
   Code borrowed from s_stabs_generic 
*/
static void
microblaze_s_stabs(ignore)
   int ignore ATTRIBUTE_UNUSED;
{
   char *string;
   int length;
   char *hold = input_line_pointer;
   char *buf;
   long longint;
   int type, other, desc;
  
   string = demand_copy_C_string (&length);
   if (length == 0) {
      SKIP_WHITESPACE();
      if (*input_line_pointer == ',')
         input_line_pointer++;
      else {
         as_warn (_(".stabs: Missing comma"));
         ignore_rest_of_line ();
         return;
      }
      if (get_absolute_expression_and_terminator (&longint) != ',') {
         as_warn (_(".stabs: Missing comma"));
         ignore_rest_of_line ();
         return;
      }
      type = longint;

      if (get_absolute_expression_and_terminator (&longint) != ',') {
         as_warn (_(".stabs: Missing comma"));
         ignore_rest_of_line ();
         return;
      }
      other = longint;

      desc = get_absolute_expression ();
      if (*input_line_pointer != ',') {
         as_warn (_(".stabs: Missing comma"));
         ignore_rest_of_line ();
         return;
      }
      input_line_pointer++;
      SKIP_WHITESPACE ();
  
      /* Find an expression of the form a-b */
      while (*input_line_pointer != '\0' && *input_line_pointer != '\n' && *input_line_pointer != '-')
         input_line_pointer++;

      if (*input_line_pointer == '-' && type == N_FUN) {
         buf = xmalloc((input_line_pointer-hold)+10);
         strncpy(buf, hold, input_line_pointer-hold);
         input_line_pointer = buf;
         s_stab('s');
         input_line_pointer = hold;
         discard_rest_of_line();
         free(buf);
         return;
      }    
   }
   input_line_pointer = hold;
   s_stab('s');
}


/* Handle the section changing pseudo-ops. */
static void
microblaze_s_text (ignore)
   int ignore ATTRIBUTE_UNUSED;
{
  
#ifdef OBJ_ELF
   obj_elf_text (ignore);
#else
   s_text (ignore);
#endif
}

static void
microblaze_s_data (ignore)
   int ignore ATTRIBUTE_UNUSED;
{
#ifdef OBJ_ELF
   obj_elf_change_section(".data", SHT_PROGBITS, SHF_ALLOC+SHF_WRITE, 0);
#else
   s_data (ignore);
#endif
}

/* Things in the .sdata segment are always considered to be in the small data section */
static void
microblaze_s_sdata (ignore)
   int ignore ATTRIBUTE_UNUSED;
{
#ifdef OBJ_ELF
   obj_elf_change_section(".sdata", SHT_PROGBITS, SHF_ALLOC+SHF_WRITE, 0);
   if (sdata_segment == 0)
      sdata_segment = subseg_new(".sdata", 0);
#else
   s_data (ignore);
#endif
}

/* Pseudo op to make file scope bss items.  */
static void
microblaze_s_lcomm (xxx)
     int xxx ATTRIBUTE_UNUSED;
{
  register char *name;
  register char c;
  register char *p;
  offsetT size;
  register symbolS *symbolP;
  offsetT align;
  segT old_sec;
  int old_subsec;
  char *pfrag;
  int align2;
  // from old code
  segT current_seg = now_seg;
  subsegT current_subseg = now_subseg;
  
  name = input_line_pointer;
  c = get_symbol_end ();

  /* just after name is now '\0'.  */
  p = input_line_pointer;
  *p = c;
  SKIP_WHITESPACE ();
  if (*input_line_pointer != ',')
    {
      as_bad (_("Expected comma after symbol-name: rest of line ignored."));
      ignore_rest_of_line ();
      return;
    }

  input_line_pointer++;		/* skip ',' */
  if ((size = get_absolute_expression ()) < 0)
    {
      as_warn (_(".COMMon length (%ld.) <0! Ignored."), (long) size);
      ignore_rest_of_line ();
      return;
    }

  /* The third argument to .lcomm is the alignment.  */
  if (*input_line_pointer != ',')
    align = 8;
  else
    {
      ++input_line_pointer;
      align = get_absolute_expression ();
      if (align <= 0)
	{
	  as_warn (_("ignoring bad alignment"));
	  align = 8;
	}
    }

  *p = 0;
  symbolP = symbol_find_or_make (name);
  *p = c;

  if (S_IS_DEFINED (symbolP) && ! S_IS_COMMON (symbolP))
    {
      as_bad (_("Ignoring attempt to re-define symbol `%s'."),
	      S_GET_NAME (symbolP));
      ignore_rest_of_line ();
      return;
    }

  if (S_GET_VALUE (symbolP) && S_GET_VALUE (symbolP) != (valueT) size)
    {
      as_bad (_("Length of .lcomm \"%s\" is already %ld. Not changed to %ld."),
	      S_GET_NAME (symbolP),
	      (long) S_GET_VALUE (symbolP),
	      (long) size);

      ignore_rest_of_line ();
      return;
    }

  /* Allocate_bss.  */
  old_sec = now_seg;
  old_subsec = now_subseg;
  if (align)
    {
      /* Convert to a power of 2 alignment.  */
      for (align2 = 0; (align & 1) == 0; align >>= 1, ++align2);
      if (align != 1)
	{
	  as_bad (_("Common alignment not a power of 2"));
	  ignore_rest_of_line ();
	  return;
	}
    }
  else
    align2 = 0;

  //record_alignment (bss_section, align2);
  //subseg_set (bss_section, 0);
  record_alignment (current_seg, align2);
  subseg_set (current_seg, current_subseg);
  if (align2)
    frag_align (align2, 0, 0);
  //if (S_GET_SEGMENT (symbolP) == bss_section)
  if (S_GET_SEGMENT (symbolP) == current_seg)
    symbol_get_frag (symbolP)->fr_symbol = 0;
  symbol_set_frag (symbolP, frag_now);
  pfrag = frag_var (rs_org, 1, 1, (relax_substateT) 0, symbolP, size,
		    (char *) 0);
  *pfrag = 0;
  S_SET_SIZE (symbolP, size);
  //S_SET_SEGMENT (symbolP, bss_section);
  S_SET_SEGMENT (symbolP, current_seg);
  //subseg_set (old_sec, old_subsec);
  subseg_set (current_seg, current_subseg);
  demand_empty_rest_of_line ();
}

/* The following code is taken from config/obj-elf.c from obj_elf_common
 * The code modifications are primarily done for following::
 * 1. Change symbol section to be of now_seg instead of .comm section
 * 2. Handle alignment for local and external symbols
 * Fixes CR 165167, 218259:: Sathya
 */
void
microblaze_elf_common (is_common)
     int is_common;
{
  char *name;
  char c;
  char *p;
  int temp, size;
  symbolS *symbolP;
  int have_align;

  if (flag_mri && is_common)
    {
      s_mri_common (0);
      return;
    }

  name = input_line_pointer;
  c = get_symbol_end ();
  /* just after name is now '\0' */
  p = input_line_pointer;
  *p = c;
  SKIP_WHITESPACE ();
  if (*input_line_pointer != ',')
    {
      as_bad (_("Expected comma after symbol-name"));
      ignore_rest_of_line ();
      return;
    }
  input_line_pointer++;		/* skip ',' */
  if ((temp = get_absolute_expression ()) < 0)
    {
      as_bad (_(".COMMon length (%d.) <0! Ignored."), temp);
      ignore_rest_of_line ();
      return;
    }
  size = temp;
  *p = 0;
  symbolP = symbol_find_or_make (name);
  *p = c;
  if (S_IS_DEFINED (symbolP) && ! S_IS_COMMON (symbolP))
    {
      as_bad (_("Ignoring attempt to re-define symbol"));
      ignore_rest_of_line ();
      return;
    }
  if (S_GET_VALUE (symbolP) != 0)
    {
      if (S_GET_VALUE (symbolP) != (valueT) size)
	{
	  as_warn (_("Length of .comm \"%s\" is already %ld. Not changed to %d."),
		   S_GET_NAME (symbolP), (long) S_GET_VALUE (symbolP), size);
	}
    }
  know (symbolP->sy_frag == &zero_address_frag);
  if (*input_line_pointer != ',')
    have_align = 0;
  else
    {
      have_align = 1;
      input_line_pointer++;
      SKIP_WHITESPACE ();
    }
  if (! have_align || *input_line_pointer != '"')
    {
      if (! have_align)
	temp = 0;
      else
	{
	  temp = get_absolute_expression ();
	  if (temp < 0)
	    {
	      temp = 0;
	      as_warn (_("Common alignment negative; 0 assumed"));
	    }
	}
      //if (symbol_get_obj (symbolP)->local)
	{
	  segT old_sec;
	  int old_subsec;
	  char *pfrag;
	  int align;

	/* allocate_bss: */
	  old_sec = now_seg;
	  old_subsec = now_subseg;
	  if (temp)
	    {
	      /* convert to a power of 2 alignment */
	      for (align = 0; (temp & 1) == 0; temp >>= 1, ++align);
	      if (temp != 1)
		{
		  as_bad (_("Common alignment not a power of 2"));
		  ignore_rest_of_line ();
		  return;
		}
	    }
	  else
	    align = 0;
	  //record_alignment (bss_section, align);
	  record_alignment (now_seg, align);
	  //subseg_set (bss_section, 0);
	  subseg_set (now_seg, now_subseg);
	  if (align)
	    frag_align (align, 0, 0);
	  //if (S_GET_SEGMENT (symbolP) == bss_section)
	  if (S_GET_SEGMENT (symbolP) == now_seg)
	    symbol_get_frag (symbolP)->fr_symbol = 0;
	  symbol_set_frag (symbolP, frag_now);
	  pfrag = frag_var (rs_org, 1, 1, (relax_substateT) 0, symbolP,
			    (offsetT) size, (char *) 0);
	  *pfrag = 0;
	  S_SET_SIZE (symbolP, size);
	  //S_SET_SEGMENT (symbolP, bss_section);
	  S_SET_SEGMENT (symbolP, now_seg);
	  if (symbol_get_obj (symbolP)->local)
	    S_CLEAR_EXTERNAL (symbolP);
	  else
	    S_SET_EXTERNAL (symbolP);
	  subseg_set (old_sec, old_subsec);
	}
	/*else
	  {
	  allocate_common:
	  S_SET_VALUE (symbolP, (valueT) size);
	  S_SET_ALIGN (symbolP, temp);
	  S_SET_EXTERNAL (symbolP);
	  S_SET_SEGMENT (symbolP, bfd_com_section_ptr);
	  }
	*/
    }
  else
    {
      input_line_pointer++;
      /* @@ Some use the dot, some don't.  Can we get some consistency??  */
      if (*input_line_pointer == '.')
	input_line_pointer++;
      /* @@ Some say data, some say bss.  */
      if (strncmp (input_line_pointer, "bss\"", 4)
	  && strncmp (input_line_pointer, "data\"", 5))
	{
	  while (*--input_line_pointer != '"')
	    ;
	  input_line_pointer--;
	  goto bad_common_segment;
	}
      while (*input_line_pointer++ != '"')
	;
      //goto allocate_common;
      S_SET_VALUE (symbolP, (valueT) size);
      S_SET_ALIGN (symbolP, temp);
      S_SET_EXTERNAL (symbolP);
      //S_SET_SEGMENT (symbolP, bfd_com_section_ptr);
      S_SET_SEGMENT (symbolP, now_seg);
    }

  symbol_get_bfdsym (symbolP)->flags |= BSF_OBJECT;

  demand_empty_rest_of_line ();
  return;

  {
  bad_common_segment:
    p = input_line_pointer;
    while (*p && *p != '\n')
      p++;
    c = *p;
    *p = '\0';
    as_bad (_("bad .common segment %s"), input_line_pointer + 1);
    *p = c;
    input_line_pointer = p;
    ignore_rest_of_line ();
    return;
  }
}


/* localvar is 0 if it is a .comm and 1 if it is a .lcomm */
static void
microblaze_s_comm (localvar)
   int localvar;
{
#ifdef OBJ_ELF
   /*
     segT current_seg = now_seg;
     subsegT current_subseg = now_subseg;
     obj_elf_change_section(".sbss", SHT_NOBITS, SHF_ALLOC+SHF_WRITE, 0);
     if (sbss_segment == 0)
     sbss_segment = subseg_new(".sbss", 0);
   */
   /* The following code is taken from s_lcomm_internal */
   {
      register char *name;
      register char c;
      register char *p;
      offsetT size;
      register int temp;
      register symbolS *symbolP;
      segT current_seg = now_seg;
      subsegT current_subseg = now_subseg;
      
      int align = 2;

      name = input_line_pointer;
      c = get_symbol_end ();
      p = input_line_pointer;
      *p = c;
      SKIP_WHITESPACE ();
      
      if (*input_line_pointer != ',')
	{
	  as_bad (_("Expected comma after symbol-name: rest of line ignored."));
	  ignore_rest_of_line ();
	  return;
	}
      
      input_line_pointer++;		/* skip ',' */
      if ((size = get_absolute_expression ()) < 0)
	{
	  as_warn (_(".COMMon length (%ld.) <0! Ignored."), (long) size);
	  ignore_rest_of_line ();
	  return;
	}
      
      /* The third argument to .lcomm is the alignment.  */
      if (*input_line_pointer != ',')
	align = 8;
      else
	{
	  ++input_line_pointer;
	  align = get_absolute_expression ();
	  if (align <= 0)
	    {
	      as_warn (_("ignoring bad alignment"));
	      align = 8;
	    }
	}
      
      /* Accept an optional comma after the name.  The comma used to be
         required, but Irix 5 cc does not generate it.  */
      if (*input_line_pointer == ',')
      {
         ++input_line_pointer;
         SKIP_WHITESPACE ();
      }

      if (*input_line_pointer == '\n')
      {
         as_bad (_("Missing size expression"));
         return;
      }

      if ((temp = get_absolute_expression ()) < 0)
      {
         as_warn (_("BSS length (%d.) <0! Ignored."), temp);
         ignore_rest_of_line ();
         return;
      }
      

      /* Still zero unless TC_IMPLICIT_LCOMM_ALIGNMENT set it.  */
      if (align)
         /*record_alignment(sbss_segment, align);*/
         record_alignment(current_seg, align);
      *p = 0;
      symbolP = symbol_find_or_make (name);
      *p = c;

      /*       if ((S_GET_SEGMENT (symbolP) == sbss_segment
               || (!S_IS_DEFINED (symbolP) && S_GET_VALUE (symbolP) == 0)))*/
      if ((S_GET_SEGMENT (symbolP) == current_seg
           || (!S_IS_DEFINED (symbolP) && S_GET_VALUE (symbolP) == 0)))
      {
         char *pfrag;

         /* subseg_set (sbss_segment, 1);*/
         subseg_set (current_seg, current_subseg);
         if (align)
            frag_align (align, 0, 0);
         /* detach from old frag	*/
         /*	   if (S_GET_SEGMENT (symbolP) == sbss_segment)*/
         if (S_GET_SEGMENT (symbolP) == current_seg)
            symbol_get_frag (symbolP)->fr_symbol = NULL;

         symbol_set_frag (symbolP, frag_now);
         pfrag = frag_var (rs_org, 1, 1, (relax_substateT)0, symbolP,
                           (offsetT) temp, (char *) 0);
         *pfrag = 0;

         /*S_SET_SEGMENT (symbolP, sbss_segment);*/
         S_SET_SEGMENT (symbolP, current_seg);
         if (localvar == 0)
            S_SET_EXTERNAL (symbolP);

#ifdef S_SET_SIZE
         S_SET_SIZE (symbolP, temp);
#endif
      }
      else
         as_bad (_("Ignoring attempt to re-define symbol `%s'."),
                 S_GET_NAME (symbolP));

      subseg_set (current_seg, current_subseg);

      demand_empty_rest_of_line ();
   }				
#else
   s_data (ignore);
#endif
}

static void
microblaze_s_rdata (localvar)
   int localvar;
{
#ifdef OBJ_ELF
   if (localvar == 0) /* rodata */{
      obj_elf_change_section(".rodata", SHT_PROGBITS, SHF_ALLOC, 0); 
      if (rodata_segment == 0)
         rodata_segment = subseg_new(".rodata", 0);
   }
   else /* 1 .sdata2*/ {
      obj_elf_change_section(".sdata2", SHT_PROGBITS, SHF_ALLOC, 0);
      if (sdata2_segment == 0)
         sdata2_segment = subseg_new(".sdata2", 0);
   }
#else
   s_data (ignore);
#endif
}

static void
microblaze_s_bss (localvar)
   int localvar;
{
#ifdef OBJ_ELF
   if (localvar == 0) /* bss */
      obj_elf_change_section(".bss", SHT_NOBITS, SHF_ALLOC+SHF_WRITE, 0);
   else if (localvar == 1) /* sbss */ {
      obj_elf_change_section(".sbss", SHT_NOBITS, SHF_ALLOC+SHF_WRITE, 0);
      if (sbss_segment == 0)
         sbss_segment = subseg_new(".sbss", 0);
   }
#else
   s_data (ignore);
#endif
}

/* endp_p is always 1 as this func is called only for .end <funcname>
   This func consumes the <funcname> and calls regular processing
   s_func(1) with arg 1 (1 for end) */
void
microblaze_s_func (end_p)
   int end_p;
{
   *input_line_pointer = get_symbol_end ();
   s_func(1);
}

/* Emit a function stab.
   All assembler functions are assumed to have return type `void'.  */
/* This is done here because the microblaze stabs are all absolute */
/* The standard function in stabs.c sets in_dot_func_p that causes all */
/* future stabs to be generated relative to this function - don't want that */

void
microblaze_stabs_generate_asm_func (funcname, startlabname)
   const char *funcname;
const char *startlabname;
{
   static int void_emitted_p;
   char *hold = input_line_pointer;
   char *buf;
   char *file;
   unsigned int lineno;

   if (! void_emitted_p)
   {
      input_line_pointer = "\"void:t1=1\",128,0,0,0";
      s_stab ('s');
      void_emitted_p = 1;
   }

   as_where (&file, &lineno);
   asprintf (&buf, "\"%s:F1\",%d,0,%d,%s",
             funcname, N_FUN, lineno + 1, startlabname);
   input_line_pointer = buf;
   s_stab ('s');
   free (buf);

   input_line_pointer = hold;
}

/* Emit a stab to record the end of a function.  */
/* All microblaze stabs are absolute - the standard function in stabs.c */
/* generates the difference of two symbols. */
/* This function is outdated as stabs are generated by regular processing - Sathya 05/01/02*/

void
microblaze_stabs_generate_asm_endfunc (funcname, startlabname)
   const char *funcname ATTRIBUTE_UNUSED;
const char *startlabname ATTRIBUTE_UNUSED;
{
   char *hold = input_line_pointer;
   char *buf;
   char sym[30];

   microblaze_generate_symbol(&sym);
   colon (sym);

   asprintf (&buf, "\"\",%d,0,0,%s", N_FUN, sym);
   input_line_pointer = buf;
   s_stab ('s');
   free (buf);

   input_line_pointer = hold;
}

/* Handle the .weakext pseudo-op as defined in Kane and Heinrich.
 */

static void
microblaze_s_weakext (ignore)
     int ignore;
{
  char *name;
  int c;
  symbolS *symbolP;
  expressionS exp;

  name = input_line_pointer;
  c = get_symbol_end ();
  symbolP = symbol_find_or_make (name);
  S_SET_WEAK (symbolP);
  *input_line_pointer = c;

  SKIP_WHITESPACE ();

  if (! is_end_of_line[(unsigned char) *input_line_pointer])
  {
     if (S_IS_DEFINED (symbolP))
     {
        as_bad ("Ignoring attempt to redefine symbol `%s'.",
                S_GET_NAME (symbolP));
        ignore_rest_of_line ();
        return;
     }
     
     if (*input_line_pointer == ',')
     {
        ++input_line_pointer;
        SKIP_WHITESPACE ();
     }
     
     expression (&exp);
     if (exp.X_op != O_symbol)
     {
        as_bad ("bad .weakext directive");
        ignore_rest_of_line();
        return;
     }
     symbol_set_value_expression (symbolP, &exp);
  }
  
  demand_empty_rest_of_line ();
}


/* This function is called once, at assembler startup time.  This should
   set up all the tables, etc that the MD part of the assembler needs.  */
void
md_begin ()
{
   struct op_code_struct * opcode;

   opcode_hash_control = hash_new ();

   /* Insert unique names into hash table */
   for (opcode = opcodes; opcode->name; opcode ++)
   {
      hash_insert (opcode_hash_control, opcode->name, (char *) opcode);
   }
}

/* Try to parse a reg name.  */
static char *
parse_reg (s, reg)
   char * s;
unsigned * reg;
{
   unsigned tmpreg = 0;

   /* Strip leading whitespace.  */
   while (isspace (* s))
      ++ s;
  
   if (strncasecmp (s, "rpc", 3) == 0) {
      *reg = REG_PC;
      return s + 3;
   }
   else if (strncasecmp (s, "rmsr", 4) == 0) {
      *reg = REG_MSR;
      return s + 4;
   }   
   else if (strncasecmp (s, "rear", 4) == 0) {
      *reg = REG_EAR;
      return s + 4;
   }
   else if (strncasecmp (s, "resr", 4) == 0) {
      *reg = REG_ESR;
      return s + 4;
   }
   else if (strncasecmp (s, "rfsr", 4) == 0) {
      *reg = REG_FSR;
      return s + 4;
   }
   else if (strncasecmp (s, "rsp", 3) == 0) {
      *reg = REG_SP;
      return s + 3;
   }
   // PATCH START for FSL specific instructions: sathya
   else if (strncasecmp (s, "rfsl", 4) == 0) {
      if (isdigit(s[4]) && isdigit(s[5]))
      {
         tmpreg = (s[4]-'0')*10 + s[5] - '0';
         s += 6;
      }
      
      else if (isdigit(s[4]))
      {
         tmpreg = s[4] - '0';
         s += 5;
      }
      else
         as_bad (_("register expected, but saw '%.6s'"), s);
      if (tmpreg >= MIN_REGNUM && tmpreg <= MAX_REGNUM)
         *reg = tmpreg;
      else {
         as_bad(_("Invalid register number at '%.6s'"), s);
         *reg = 0;
      }
      return s;
      
   }
   // PATCH END
   else {
      if (tolower (s[0]) == 'r')
      {
         if (isdigit(s[1]) && isdigit(s[2]))
         {
            tmpreg = (s[1]-'0')*10 + s[2] - '0';
            s += 3;
         }
      
         else if (isdigit(s[1]))
         {
            tmpreg = s[1] - '0';
            s += 2;
         }
         else
            as_bad (_("register expected, but saw '%.6s'"), s);
         if (tmpreg >= MIN_REGNUM && tmpreg <= MAX_REGNUM)
            *reg = tmpreg;
         else {
            as_bad(_("Invalid register number at '%.6s'"), s);
            *reg = 0;
         }
         return s;
      }
   }
   as_bad (_("register expected, but saw '%.6s'"), s);
   * reg = 0;
   return s;
}

static char *
parse_exp (s, e)
   char * s;
expressionS * e;
{
   char * save;
   char * new;

   /* Skip whitespace.  */
   while (isspace (* s))
      ++ s;
  
   save = input_line_pointer;
   input_line_pointer = s;

   expression (e);
  
   if (e->X_op == O_absent)
      as_bad (_("missing operand"));
  
   new = input_line_pointer;
   input_line_pointer = save;
  
   return new;
}

static char *
parse_imm (s, e, min, max)
   char * s;
expressionS * e;
int min;
int max;
{
   char * new;
  
   new = parse_exp (s, e);
  
   if (e->X_op == O_absent)
      ; /* An error message has already been emitted.  */
   /*  else if (e.X_op != O_constant)
       as_bad (_("operand must be a constant"));
   */
   else if ((e->X_op != O_constant && e->X_op != O_symbol) )
      /*	   || (e->X_op == O_symbol   && e->X_add_number != 0 )) */
      as_bad(_("operand must be a constant or a label"));
   else if ((e->X_op == O_constant) && (e->X_add_number < min || e->X_add_number > max))
      as_bad (_("operand must be absolute in range %d..%d, not %d"),
              min, max, e->X_add_number);

   return new;
}

/* This is the guts of the machine-dependent assembler.  STR points to a
   machine dependent instruction.  This function is supposed to emit
   the frags/bytes it assembles to.  */

static char *str_microblaze_ro_anchor = "RO";
static char *str_microblaze_rw_anchor = "RW";

static boolean
check_spl_reg(reg)
   unsigned * reg;
{
   if ( (*reg == REG_MSR) || (*reg == REG_PC) ||
        (*reg == REG_EAR) || (*reg == REG_ESR) || 
        (*reg == REG_FSR) )
      return true;
   
   return false;
}

void
md_assemble (str)
   char * str;
{
   char * op_start;
   char * op_end;
   struct op_code_struct * opcode, *opcode1;
   char * output;
   int nlen = 0;
   int i;
   unsigned long inst, inst1;
   unsigned reg1;
   unsigned reg2;
   unsigned reg3;
   unsigned isize;
   int imm, temp;
   expressionS exp;
   char name[20];

   /* Drop leading whitespace.  */
   while (isspace (* str))
      str ++;

   /* Find the op code end.  */
   for (op_start = op_end = str;
        * op_end && nlen < 20 && !is_end_of_line [*op_end] && *op_end != ' ';
        op_end++)
   {
      name[nlen] = op_start[nlen];
      nlen++;
   }
  
   name [nlen] = 0;
  
   if (nlen == 0)
   {
      as_bad (_("can't find opcode "));
      return;
   }

   opcode = (struct op_code_struct *) hash_find (opcode_hash_control, name);
   if (opcode == NULL)
   {
      as_bad (_("unknown opcode \"%s\""), name);
      return;
   }
  
   inst = opcode->bit_sequence;
   isize = 4;
  
   switch (opcode->inst_type)
   {
   case INST_TYPE_RD_R1_R2:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg2);  /* get r1 */
      else {
         as_bad(_("Error in statement syntax"));
         reg2 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg3);  /* get r2 */
      else {
         as_bad(_("Error in statement syntax"));
         reg3 = 0;
      }

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      if (check_spl_reg(&reg2))
         as_bad(_("Cannot use special register with this instruction"));
      if (check_spl_reg(&reg3))
         as_bad(_("Cannot use special register with this instruction"));
      
      if (!strcmp(name, "sub")) {
         /* sub rd, r1, r2 becomes rsub rd, r2, r1 */
         inst |= (reg1 << RD_LOW) & RD_MASK;
         inst |= (reg3 << RA_LOW) & RA_MASK;
         inst |= (reg2 << RB_LOW) & RB_MASK;
      } else {
         inst |= (reg1 << RD_LOW) & RD_MASK;
         inst |= (reg2 << RA_LOW) & RA_MASK;
         inst |= (reg3 << RB_LOW) & RB_MASK;
      }
      output = frag_more (isize);
      break;

   case INST_TYPE_RD_R1_IMM:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg2);  /* get r1 */
      else {
         as_bad(_("Error in statement syntax"));
         reg2 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_imm (op_end + 1, & exp, MIN_IMM, MAX_IMM);
      else 
         as_bad(_("Error in statement syntax"));

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      if (check_spl_reg(&reg2))
         as_bad(_("Cannot use special register with this instruction"));

      
      if (exp.X_op != O_constant) {
         char *opc;
         if (!strcmp(name, "lmi")) {
            as_bad (_("lmi pseudo instruction should not use a label in imm field"));
         }
         else if (!strcmp(name, "smi")) {
            as_bad (_("smi pseudo instruction should not use a label in imm field"));
         }
	

         if (reg2 == REG_ROSDP)
            opc = str_microblaze_ro_anchor;
         else if (reg2 == REG_RWSDP)
            opc = str_microblaze_rw_anchor;
         else
            opc = NULL;
         output = frag_var(rs_machine_dependent,
                           isize * 2, /* maxm of 2 words */
                           isize,     /* minm of 1 word */
                           opcode->inst_offset_type, /* PC-relative or not */
                           exp.X_add_symbol,
                           exp.X_add_number,
                           opc);
         imm = 0;
      } else {
         output = frag_more (isize);
         imm = exp.X_add_number;
      }

      if (!strcmp(name, "lmi") || !strcmp(name, "smi")) {
         /* Load/store 32-d consecutive registers.  Used on exit/entry */
         /* to subroutines to save and restore registers to stack */
         /* Generate 32-d insts */
         int count;
         count = 32 - reg1;
         if (!strcmp(name, "lmi")) 
            opcode = (struct op_code_struct *) hash_find (opcode_hash_control, "lwi");
         else
            opcode = (struct op_code_struct *) hash_find (opcode_hash_control, "swi");
         if (opcode == NULL)
         {
            as_bad (_("unknown opcode \"%s\""), "lwi");
            return;
         }
         inst  = opcode->bit_sequence;
         inst |= (reg1 << RD_LOW) & RD_MASK;
         inst |= (reg2 << RA_LOW) & RA_MASK;
         inst |= (imm << IMM_LOW) & IMM_MASK;	
         for (i=0; i<count-1; i++) {
            output[0] = INST_BYTE0 (inst);
            output[1] = INST_BYTE1 (inst);
            output[2] = INST_BYTE2 (inst);
            output[3] = INST_BYTE3 (inst);
            output = frag_more (isize);
            imm = imm + 4;
            reg1++;
            inst = opcode->bit_sequence;
            inst |= (reg1 << RD_LOW) & RD_MASK;
            inst |= (reg2 << RA_LOW) & RA_MASK;
            inst |= (imm << IMM_LOW) & IMM_MASK;	
         }
      }
      else {
         temp = imm & 0xFFFF8000;
         if ((temp != 0) && (temp != 0xFFFF8000)) {
            /* needs an immediate inst */
            opcode1 = (struct op_code_struct *) hash_find (opcode_hash_control, "imm");
            if (opcode1 == NULL)
            {
               as_bad (_("unknown opcode \"%s\""), "imm");
               return;
            }
  
            inst1 = opcode1->bit_sequence;
            inst1 |= ((imm & 0xFFFF0000) >> 16) & IMM_MASK;
            output[0] = INST_BYTE0 (inst1);
            output[1] = INST_BYTE1 (inst1);
            output[2] = INST_BYTE2 (inst1);
            output[3] = INST_BYTE3 (inst1);
            output = frag_more (isize);
         }
         inst |= (reg1 << RD_LOW) & RD_MASK;
         inst |= (reg2 << RA_LOW) & RA_MASK;
         inst |= (imm << IMM_LOW) & IMM_MASK;
      }
      break;

   case INST_TYPE_RD_R1_IMM5:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg2);  /* get r1 */
      else {
         as_bad(_("Error in statement syntax"));
         reg2 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_imm (op_end + 1, & exp, MIN_IMM, MAX_IMM);
      else 
         as_bad(_("Error in statement syntax"));

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      if (check_spl_reg(&reg2))
         as_bad(_("Cannot use special register with this instruction"));
      
      if (exp.X_op != O_constant) {
         as_warn(_("Symbol used as immediate for shift instruction"));
      } else {
         output = frag_more (isize);
         imm = exp.X_add_number;
      }
      
      if (imm != (imm % 32)) {
         as_warn(_("Shift value > 32. using <value % 32>"));
         imm = imm % 32;
      }
      inst |= (reg1 << RD_LOW) & RD_MASK;
      inst |= (reg2 << RA_LOW) & RA_MASK;
      inst |= (imm << IMM_LOW) & IMM5_MASK;
      break;

   case INST_TYPE_R1_R2:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get r1 */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg2);  /* get r2 */
      else {
         as_bad(_("Error in statement syntax"));
         reg2 = 0;
      }

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      if (check_spl_reg(&reg2))
         as_bad(_("Cannot use special register with this instruction"));
      
      inst |= (reg1 << RA_LOW) & RA_MASK;
      inst |= (reg2 << RB_LOW) & RB_MASK;
      output = frag_more (isize);
      break;

   case INST_TYPE_RD_R1:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg2);  /* get r1 */
      else {
         as_bad(_("Error in statement syntax"));
         reg2 =0;
      }

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      if (check_spl_reg(&reg2))
         as_bad(_("Cannot use special register with this instruction"));
      
      inst |= (reg1 << RD_LOW) & RD_MASK;
      inst |= (reg2 << RA_LOW) & RA_MASK;
      output = frag_more (isize);
      break;
      
   case INST_TYPE_RD_IMM12: 
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &imm);  /* get rfslN */
      else {
         as_bad(_("Error in statement syntax"));
         imm = 0;
      }

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      
      inst |= (reg1 << RD_LOW) & RD_MASK;
      inst |= (imm << IMM_LOW) & IMM12_MASK;
      output = frag_more (isize);
      break;

   case INST_TYPE_RD_IMM14:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      
      if (strcmp(op_end, ""))
         op_end = parse_imm (op_end + 1, & exp, MIN_IMM14, MAX_IMM14);
      else 
         as_bad(_("Error in statement syntax"));

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      
      if (exp.X_op != O_constant) {
         as_bad(_("Symbol used as immediate value for msrset/msrclr instructions"));
      } else {
         output = frag_more (isize);
         imm = exp.X_add_number;
      }
      inst |= (reg1 << RD_LOW) & RD_MASK;
      inst |= (imm << IMM_LOW) & IMM14_MASK;
      break;
      
   case INST_TYPE_R1_IMM12:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get r1 */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &imm);  /* get rfslN */
      else {
         as_bad(_("Error in statement syntax"));
         imm = 0;
      }

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      
      inst |= (reg1 << RA_LOW) & RA_MASK;
      inst |= (imm << IMM_LOW) & IMM12_MASK;
      output = frag_more (isize);
      break;
      
   case INST_TYPE_R1:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get r1 */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      
      inst |= (reg1 << RA_LOW) & RA_MASK;
      output = frag_more (isize);
      break;

      /* For tuqula insn...:) */
   case INST_TYPE_RD:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      
      inst |= (reg1 << RD_LOW) & RD_MASK;
      output = frag_more (isize);
      break;
      
   case INST_TYPE_RD_SPECIAL:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg2);  /* get r1 */
      else {
         as_bad(_("Error in statement syntax"));
         reg2 = 0;
      }
      
      if (reg2 == REG_MSR)
         imm = opcode->immval_mask | REG_MSR_MASK;
      else if (reg2 == REG_PC)
         imm = opcode->immval_mask | REG_PC_MASK;
      else if (reg2 == REG_EAR)
         imm = opcode->immval_mask | REG_EAR_MASK;
      else if (reg2 == REG_ESR)
         imm = opcode->immval_mask | REG_ESR_MASK;
      else if (reg2 == REG_FSR)
         imm = opcode->immval_mask | REG_FSR_MASK;      
      else
         as_bad(_("invalid value for special purpose register"));
      inst |= (reg1 << RD_LOW) & RD_MASK;
      inst |= (imm << IMM_LOW) & IMM_MASK;
      output = frag_more (isize);
      break;

   case INST_TYPE_SPECIAL_R1:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg2);  /* get r1 */
      else {
         as_bad(_("Error in statement syntax"));
         reg2 = 0;
      }

      if (reg1 == REG_MSR)
         imm = opcode->immval_mask | REG_MSR_MASK;
      else if (reg1 == REG_PC)
         imm = opcode->immval_mask | REG_PC_MASK;
      else if (reg1 == REG_EAR)
         imm = opcode->immval_mask | REG_EAR_MASK;
      else if (reg1 == REG_ESR)
         imm = opcode->immval_mask | REG_ESR_MASK;
      else if (reg1 == REG_FSR)
         imm = opcode->immval_mask | REG_FSR_MASK;      
      else
         as_bad(_("invalid value for special purpose register"));
      inst |= (reg2 << RA_LOW) & RA_MASK;
      inst |= (imm << IMM_LOW) & IMM_MASK;
      output = frag_more (isize);
      break;
      
   case INST_TYPE_RD_R1_SPECIAL:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg2);  /* get r1 */
      else {
         as_bad(_("Error in statement syntax"));
         reg2 =0;
      }

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      if (check_spl_reg(&reg2))
         as_bad(_("Cannot use special register with this instruction"));
      
      
      /* insn wic ra, rb => wic ra, ra, rb */
      inst |= (reg1 << RD_LOW) & RD_MASK;
      inst |= (reg1 << RA_LOW) & RA_MASK;
      inst |= (reg2 << RB_LOW) & RB_MASK;
      
      output = frag_more (isize);
      break;
      
   case INST_TYPE_RD_R2:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg2);  /* get r2 */
      else {
         as_bad(_("Error in statement syntax"));
         reg2 = 0;
      }

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      if (check_spl_reg(&reg2))
         as_bad(_("Cannot use special register with this instruction"));
      
      inst |= (reg1 << RD_LOW) & RD_MASK;
      inst |= (reg2 << RB_LOW) & RB_MASK;
      output = frag_more (isize);
      break;

   case INST_TYPE_R1_IMM:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get r1 */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_imm (op_end + 1, & exp, MIN_IMM, MAX_IMM);
      else 
         as_bad(_("Error in statement syntax"));
      
      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      
      if (exp.X_op != O_constant) {
         char *opc = NULL;
         output = frag_var(rs_machine_dependent,
                           isize * 2, /* maxm of 2 words */
                           isize,     /* minm of 1 word */
                           opcode->inst_offset_type, /* PC-relative or not */
                           exp.X_add_symbol,
                           exp.X_add_number,
                           opc);
         imm = 0;
      } else {
         output = frag_more (isize);
         imm = exp.X_add_number;
      }

      temp = imm & 0xFFFF8000;
      if ((temp != 0) && (temp != 0xFFFF8000)) {
         /* needs an immediate inst */
         opcode1 = (struct op_code_struct *) hash_find (opcode_hash_control, "imm");
         if (opcode1 == NULL)
         {
            as_bad (_("unknown opcode \"%s\""), "imm");
            return;
         }
  
         inst1 = opcode1->bit_sequence;
         inst1 |= ((imm & 0xFFFF0000) >> 16) & IMM_MASK;
         output[0] = INST_BYTE0 (inst1);
         output[1] = INST_BYTE1 (inst1);
         output[2] = INST_BYTE2 (inst1);
         output[3] = INST_BYTE3 (inst1);
         output = frag_more (isize);
      }

      inst |= (reg1 << RA_LOW) & RA_MASK;
      inst |= (imm << IMM_LOW) & IMM_MASK;
      break;
      
   case INST_TYPE_RD_IMM:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg1);  /* get rd */
      else {
         as_bad(_("Error in statement syntax"));
         reg1 = 0;
      }
      if (strcmp(op_end, ""))
         op_end = parse_imm (op_end + 1, & exp, MIN_IMM, MAX_IMM);
      else
         as_bad(_("Error in statement syntax"));

      // Check for spl registers
      if (check_spl_reg(&reg1))
         as_bad(_("Cannot use special register with this instruction"));
      
      if (exp.X_op != O_constant) {
         char *opc = NULL;
         output = frag_var(rs_machine_dependent,
                           isize * 2, /* maxm of 2 words */
                           isize,     /* minm of 1 word */
                           opcode->inst_offset_type, /* PC-relative or not */
                           exp.X_add_symbol,
                           exp.X_add_number,
                           opc);
         imm = 0;
      } else {
         output = frag_more (isize);
         imm = exp.X_add_number;
      }
      
      temp = imm & 0xFFFF8000;
      if ((temp != 0) && (temp != 0xFFFF8000)) {
         /* needs an immediate inst */
         opcode1 = (struct op_code_struct *) hash_find (opcode_hash_control, "imm");
         if (opcode1 == NULL)
         {
            as_bad (_("unknown opcode \"%s\""), "imm");
            return;
         }
  
         inst1 = opcode1->bit_sequence;
         inst1 |= ((imm & 0xFFFF0000) >> 16) & IMM_MASK;
         output[0] = INST_BYTE0 (inst1);
         output[1] = INST_BYTE1 (inst1);
         output[2] = INST_BYTE2 (inst1);
         output[3] = INST_BYTE3 (inst1);
         output = frag_more (isize);
      }

      inst |= (reg1 << RD_LOW) & RD_MASK;
      inst |= (imm << IMM_LOW) & IMM_MASK;
      break;

   case INST_TYPE_R2:
      if (strcmp(op_end, ""))
         op_end = parse_reg(op_end + 1, &reg2);  /* get r2 */
      else {
         as_bad(_("Error in statement syntax"));
         reg2 = 0;
      }

      // Check for spl registers
      if (check_spl_reg(&reg2))
         as_bad(_("Cannot use special register with this instruction"));
      
      inst |= (reg2 << RB_LOW) & RB_MASK;
      output = frag_more (isize);
      break;

   case INST_TYPE_IMM:
      if (!strcmp(name, "imm")) {
         as_bad(_("An IMM instruction should not be present in the .s file"));
      }
      op_end = parse_imm (op_end + 1, & exp, MIN_IMM, MAX_IMM);
      
      if (exp.X_op != O_constant) {
         char *opc = NULL;
         output = frag_var(rs_machine_dependent,
                           isize * 2, /* maxm of 2 words */
                           isize,     /* minm of 1 word */
                           opcode->inst_offset_type, /* PC-relative or not */
                           exp.X_add_symbol,
                           exp.X_add_number,
                           opc);
         imm = 0;
      } else {
         output = frag_more (isize);
         imm = exp.X_add_number;
      }


      temp = imm & 0xFFFF8000;
      if ((temp != 0) && (temp != 0xFFFF8000)) {
         /* needs an immediate inst */
         opcode1 = (struct op_code_struct *) hash_find (opcode_hash_control, "imm");
         if (opcode1 == NULL)
         {
            as_bad (_("unknown opcode \"%s\""), "imm");
            return;
         }
  
         inst1 = opcode1->bit_sequence;
         inst1 |= ((imm & 0xFFFF0000) >> 16) & IMM_MASK;
         output[0] = INST_BYTE0 (inst1);
         output[1] = INST_BYTE1 (inst1);
         output[2] = INST_BYTE2 (inst1);
         output[3] = INST_BYTE3 (inst1);
         output = frag_more (isize);
      }
      inst |= (imm << IMM_LOW) & IMM_MASK;
      break;

   case INST_TYPE_NONE:
      output = frag_more (isize);
      break;
      
   default:
      as_bad (_("unimplemented opcode \"%s\""), name);
   }
  
   /* Drop whitespace after all the operands have been parsed.  */
   while (isspace (* op_end))
      op_end ++;

   /* Give warning message if the insn has more operands than required. */
   if (strcmp (op_end, opcode->name) && strcmp (op_end, ""))
      as_warn (_("ignoring operands: %s "), op_end);
  
   output[0] = INST_BYTE0 (inst);
   output[1] = INST_BYTE1 (inst);
   output[2] = INST_BYTE2 (inst);
   output[3] = INST_BYTE3 (inst);
  
}

symbolS *
md_undefined_symbol (name)
   char * name ATTRIBUTE_UNUSED;
{
   return 0;
}

/* Various routines to kill one day.  */
/* Equal to MAX_PRECISION in atof-ieee.c */
#define MAX_LITTLENUMS 6

/* Turn a string in input_line_pointer into a floating point constant of type
   type, and store the appropriate bytes in *litP.  The number of LITTLENUMS
   emitted is stored in *sizeP.  An error message is returned, or NULL on OK.*/
char *
md_atof (type, litP, sizeP)
   int type;
char * litP;
int * sizeP;
{
   int prec;
   LITTLENUM_TYPE words[MAX_LITTLENUMS];
   int    i;
   char * t;
   char * atof_ieee ();

   switch (type)
   {
   case 'f':
   case 'F':
   case 's':
   case 'S':
      prec = 2;
      break;

   case 'd':
   case 'D':
   case 'r':
   case 'R':
      prec = 4;
      break;

   case 'x':
   case 'X':
      prec = 6;
      break;

   case 'p':
   case 'P':
      prec = 6;
      break;

   default:
      *sizeP = 0;
      return _("Bad call to MD_NTOF()");
   }
  
   t = atof_ieee (input_line_pointer, type, words);
  
   if (t)
      input_line_pointer = t;

   *sizeP = prec * sizeof (LITTLENUM_TYPE);
  
   if (! target_big_endian)
   {
      for (i = prec - 1; i >= 0; i--)
      {
         md_number_to_chars (litP, (valueT) words[i],
                             sizeof (LITTLENUM_TYPE));
         litP += sizeof (LITTLENUM_TYPE);
      }
   }
   else
      for (i = 0; i < prec; i++)
      {
         md_number_to_chars (litP, (valueT) words[i],
                             sizeof (LITTLENUM_TYPE));
         litP += sizeof (LITTLENUM_TYPE);
      }
  
   return 0;
}

CONST char * md_shortopts = "";

/*#define OPTION_DONT_USE_SMALL	(OPTION_MD_BASE + 0)*/

struct option md_longopts[] =
{
   { NULL,          no_argument, NULL, 0}
};

size_t md_longopts_size = sizeof (md_longopts);

int md_short_jump_size;

void
md_create_short_jump (ptr, from_Nddr, to_Nddr, frag, to_symbol)
   char * ptr ATTRIBUTE_UNUSED;
addressT from_Nddr ATTRIBUTE_UNUSED;
addressT to_Nddr ATTRIBUTE_UNUSED;
fragS * frag ATTRIBUTE_UNUSED;
symbolS * to_symbol ATTRIBUTE_UNUSED;
{
   as_fatal (_("failed sanity check: short_jump"));
}

void
md_create_long_jump (ptr, from_Nddr, to_Nddr, frag, to_symbol)
   char * ptr ATTRIBUTE_UNUSED;
addressT from_Nddr ATTRIBUTE_UNUSED;
addressT to_Nddr ATTRIBUTE_UNUSED;
fragS * frag ATTRIBUTE_UNUSED;
symbolS * to_symbol ATTRIBUTE_UNUSED;
{
   as_fatal (_("failed sanity check: long_jump"));
}

/* Called after relaxing, change the frags so they know how big they are.  */
void
md_convert_frag (abfd, sec, fragP)
   bfd * abfd ATTRIBUTE_UNUSED;
segT sec ATTRIBUTE_UNUSED;
register fragS * fragP;
{
   switch (fragP->fr_subtype)
   {
   case UNDEFINED_PC_OFFSET:
      fix_new(fragP, fragP->fr_fix, INST_WORD_SIZE*2, fragP->fr_symbol, fragP->fr_offset, true, BFD_RELOC_64_PCREL);
      fragP->fr_fix += INST_WORD_SIZE * 2;
      fragP->fr_var = 0;
      break;
   case DEFINED_ABS_SEGMENT:
      fix_new(fragP, fragP->fr_fix, 2*INST_WORD_SIZE, fragP->fr_symbol, fragP->fr_offset, false, BFD_RELOC_64);
      fragP->fr_fix += INST_WORD_SIZE * 2;
      fragP->fr_var = 0;
      break;
   case DEFINED_RO_SEGMENT:
      fix_new(fragP, fragP->fr_fix, INST_WORD_SIZE, fragP->fr_symbol, fragP->fr_offset, false, BFD_RELOC_MICROBLAZE_32_ROSDA);
      fragP->fr_fix += INST_WORD_SIZE;
      fragP->fr_var = 0;
      break;
   case DEFINED_RW_SEGMENT:
      fix_new(fragP, fragP->fr_fix, INST_WORD_SIZE, fragP->fr_symbol, fragP->fr_offset, false, BFD_RELOC_MICROBLAZE_32_RWSDA);
      fragP->fr_fix += INST_WORD_SIZE;
      fragP->fr_var = 0;
      break;
   case DEFINED_PC_OFFSET:
      fix_new(fragP, fragP->fr_fix, INST_WORD_SIZE, fragP->fr_symbol, fragP->fr_offset, true, BFD_RELOC_MICROBLAZE_32_LO_PCREL);	
      fragP->fr_fix += INST_WORD_SIZE;
      fragP->fr_var = 0;
      break;
   case LARGE_DEFINED_PC_OFFSET:
      fix_new(fragP, fragP->fr_fix, INST_WORD_SIZE*2, fragP->fr_symbol, fragP->fr_offset, true, BFD_RELOC_64_PCREL);	
      fragP->fr_fix += INST_WORD_SIZE * 2;
      fragP->fr_var = 0;
      break;
   default:
      abort ();
   }
}

/* Applies the desired value to the specified location.
   Also sets up addends for 'rela' type relocations.  */
int
md_apply_fix3 (fixP, valp, segment)
   fixS *   fixP;
valueT * valp;
segT     segment;
{
   char *       buf  = fixP->fx_where + fixP->fx_frag->fr_literal;
   char *       file = fixP->fx_file ? fixP->fx_file : _("unknown");
   const char * symname;
   /* Note: use offsetT because it is signed, valueT is unsigned.  */
   offsetT      val  = (offsetT) * valp; 
   int          i;
   struct op_code_struct *opcode1;
   unsigned long inst1;

   symname = fixP->fx_addsy ? S_GET_NAME (fixP->fx_addsy) : _("<unknown>");


  /* If we aren't adjusting this fixup to be against the section
     symbol, we need to adjust the value.  */
   if (fixP->fx_addsy != NULL)
    {
      if (S_IS_WEAK (fixP->fx_addsy)
	  || (symbol_used_in_reloc_p (fixP->fx_addsy)
	      && (((bfd_get_section_flags (stdoutput,
					   S_GET_SEGMENT (fixP->fx_addsy))
                  & SEC_LINK_ONCE) != 0)
                || !strncmp (segment_name (S_GET_SEGMENT (fixP->fx_addsy)),
                             ".gnu.linkonce",
                             sizeof (".gnu.linkonce") - 1))))
	
	{
	  val -= S_GET_VALUE (fixP->fx_addsy);
	  if (val != 0 && ! fixP->fx_pcrel)
          {
            /* In this case, the bfd_install_relocation routine will
               incorrectly add the symbol value back in.  We just want
               the addend to appear in the object file.  
	       FIXME: If this makes VALUE zero, we're toast.  */
            val -= S_GET_VALUE (fixP->fx_addsy);
          }
	}
    }

   /* If the fix is relative to a symbol which is not defined, or not
      in the same segment as the fix, we cannot resolve it here.  */
   /* fixP->fx_addsy is NULL if valp contains the entire relocation */
   if (fixP->fx_addsy != NULL
       && ( ! S_IS_DEFINED (fixP->fx_addsy)
            || (S_GET_SEGMENT (fixP->fx_addsy) != segment)))
   {
      fixP->fx_done = 0;
#ifdef OBJ_ELF
      /* For ELF we can just return and let the reloc that will be generated
         take care of everything.  For COFF we still have to insert 'val'
         into the insn since the addend field will be ignored.  */
      /* return 0; */
#endif
   }
   /* All fixups in the text section must be handled in the linker */
   //else if (segment == text_section) {
   else if (segment->flags & SEC_CODE) {
      fixP->fx_done = 0;
   }
   else if (!fixP->fx_pcrel && fixP->fx_addsy != NULL)
      fixP->fx_done = 0;
   else
      fixP->fx_done = 1;
  
   switch (fixP->fx_r_type)
   {
   case BFD_RELOC_MICROBLAZE_32_LO:
   case BFD_RELOC_MICROBLAZE_32_LO_PCREL:
      /*      if (fixP->fx_addsy)
              val -= S_GET_VALUE (fixP->fx_addsy);
      */
      /* Don't do anything if the symbol is not defined */
      /*
       * COMMENTING THIS OUT FOR CUSTOMER FIX: WEB CASE #534665
       if (fixP->fx_addsy == NULL || S_IS_DEFINED (fixP->fx_addsy)) {
       
       //	if (((val  & ~0xffff) != 0) && ((val | 0xffff) != -1))
       if (((val & 0xFFFF8000) != 0) && ((val & 0xFFFF8000) != 0xFFFF8000))
       as_bad_where (file, fixP->fx_line,
       _("pcrel for branch to %s too far (0x%x)"),
       symname, val);
       if (target_big_endian)
       {
       buf[2] |= ((val >> 8) & 0xff);
       buf[3] |= (val & 0xff);
       }
       else
       {
       buf[1] |= ((val >> 8) & 0xff);
       buf[0] |= (val & 0xff);
       }
       } else if (!fixP->fx_pcrel) {
       fixP->fx_addnumber = val; 
       }
      */
      if (target_big_endian)
      {
         //buf[0] |= ((val >> 24) & 0xff);
         //buf[1] |= ((val >> 16) & 0xff);
         buf[2] |= ((val >> 8) & 0xff);
         buf[3] |= (val & 0xff);
      }
      else
      {
         //buf[3] |= ((val >> 24) & 0xff);
         //buf[2] |= ((val >> 16) & 0xff);
         buf[1] |= ((val >> 8) & 0xff);
         buf[0] |= (val & 0xff);
      }
      if (!fixP->fx_pcrel) {
         fixP->fx_addnumber = val; 
      }
      break;
   case BFD_RELOC_MICROBLAZE_32_ROSDA:
   case BFD_RELOC_MICROBLAZE_32_RWSDA:
      /*      if (fixP->fx_addsy)
              val -= S_GET_VALUE (fixP->fx_addsy);
      */
      /* Don't do anything if the symbol is not defined */
      
      if (fixP->fx_addsy == NULL || S_IS_DEFINED (fixP->fx_addsy)) {
         
         //	if (((val  & ~0xffff) != 0) && ((val | 0xffff) != -1))
         if (((val & 0xFFFF8000) != 0) && ((val & 0xFFFF8000) != 0xFFFF8000))
            as_bad_where (file, fixP->fx_line,
                          _("pcrel for branch to %s too far (0x%x)"),
                          symname, val);
         if (target_big_endian)
         {
            buf[2] |= ((val >> 8) & 0xff);
            buf[3] |= (val & 0xff);
         }
         else
         {
            buf[1] |= ((val >> 8) & 0xff);
            buf[0] |= (val & 0xff);
         }
      } else if (!fixP->fx_pcrel) {
         fixP->fx_addnumber = val; 
      }
      break;
   case BFD_RELOC_32:
   case BFD_RELOC_RVA:
   case BFD_RELOC_32_PCREL:
   case BFD_RELOC_MICROBLAZE_32_SYM_OP_SYM:
      /* Don't do anything if the symbol is not defined */
      if (fixP->fx_addsy == NULL || S_IS_DEFINED (fixP->fx_addsy)) {
         if (target_big_endian)
         {
            buf[0] |= ((val >> 24) & 0xff);
            buf[1] |= ((val >> 16) & 0xff);
            buf[2] |= ((val >> 8) & 0xff);
            buf[3] |= (val & 0xff);
         }
         else
         {
            buf[3] |= ((val >> 24) & 0xff);
            buf[2] |= ((val >> 16) & 0xff);
            buf[1] |= ((val >> 8) & 0xff);
            buf[0] |= (val & 0xff);
         }
      } else if (!fixP->fx_pcrel) {
         fixP->fx_addnumber = val; 
      }
      break;


   case BFD_RELOC_64_PCREL:
   case BFD_RELOC_64:
      /*      if (fixP->fx_addsy)
              val -= S_GET_VALUE (fixP->fx_addsy);
      */
      /* Add an imm instruction.  First save the current instruction */
      for (i=0; i<INST_WORD_SIZE; i++) {
         buf[i+INST_WORD_SIZE] = buf[i];
      }
      /* Generate the imm instruction */
      opcode1 = (struct op_code_struct *) hash_find (opcode_hash_control, "imm");
      if (opcode1 == NULL)
      {
         as_bad (_("unknown opcode \"%s\""), "imm");
         return;
      }
  
      inst1 = opcode1->bit_sequence;
      if (fixP->fx_addsy == NULL || S_IS_DEFINED (fixP->fx_addsy)) {
         inst1 |= ((val & 0xFFFF0000) >> 16) & IMM_MASK;
      }

      buf[0] = INST_BYTE0 (inst1);
      buf[1] = INST_BYTE1 (inst1);
      buf[2] = INST_BYTE2 (inst1);
      buf[3] = INST_BYTE3 (inst1);

      /* Add the value only if the symbol is defined */
      if (fixP->fx_addsy == NULL || S_IS_DEFINED (fixP->fx_addsy))
      {
         if (target_big_endian)
         {
            buf[6] |= ((val >> 8) & 0xff);
            buf[7] |= (val & 0xff);
         }
         else
         {
            buf[5] |= ((val >> 8) & 0xff);
            buf[4] |= (val & 0xff);
         }
      } else if (!fixP->fx_pcrel) {
         fixP->fx_addnumber = val; 
      }
      break;
      
   default:
      break;
   }

   if (fixP->fx_addsy == NULL) {
      /* This fixup has been resolved.  Create a reloc in case the linker 
         moves code around due to relaxing */
      if (fixP->fx_r_type == BFD_RELOC_64_PCREL)
         fixP->fx_r_type = BFD_RELOC_MICROBLAZE_64_NONE;
      else
         fixP->fx_r_type = BFD_RELOC_NONE;
      fixP->fx_addsy = section_symbol(absolute_section);
   }
   return 0; /* Return value is ignored.  */
}

void
md_operand (expressionP)
   expressionS * expressionP;
{
   /* Ignore leading hash symbol, if poresent.  */
   if (* input_line_pointer == '#')
   {
      input_line_pointer ++;
      expression (expressionP);
   }
}

/* Called just before address relaxation, return the length
   by which a fragment must grow to reach it's destination.  */
int
md_estimate_size_before_relax (fragP, segment_type)
   register fragS * fragP;
register segT segment_type;
{
   switch (fragP->fr_subtype)
   {
   case INST_PC_OFFSET:
      /* Used to be a PC-relative branch */
      if (!fragP->fr_symbol)
      {
         /* We know the abs value: Should never happen */
         as_bad(_("Absolute PC-relative value in relaxation code.  Assembler error....."));
         abort();
      }
      else if ((S_GET_SEGMENT (fragP->fr_symbol) == segment_type))
      {
         fragP->fr_subtype = DEFINED_PC_OFFSET;
         /* Don't know now whether we need an imm instruction */
         fragP->fr_var = INST_WORD_SIZE;
      }
      else if (S_IS_DEFINED (fragP->fr_symbol) && 
               //       (strncmp (segment_name (S_GET_SEGMENT (fragP->fr_symbol)), ".gnu.linkonce.t",
               //                          sizeof (".gnu.linkonce.t") - 1)) &&
               //                (strncmp (segment_name (S_GET_SEGMENT (fragP->fr_symbol)), ".text",
               //                          sizeof (".text") - 1))
               (!((S_GET_SEGMENT (fragP->fr_symbol))->flags) & SEC_CODE)
               )
         
      {
         /* cannot have a PC-relative branch to a diff segment */
         as_bad(_("PC relative branch to label %s which is not in the instruction space"), S_GET_NAME(fragP->fr_symbol));
         fragP->fr_subtype = UNDEFINED_PC_OFFSET;
         fragP->fr_var = INST_WORD_SIZE*2;
      }
      else
	{
	  fragP->fr_subtype = UNDEFINED_PC_OFFSET;
	  fragP->fr_var = INST_WORD_SIZE*2;
	}
      break;
      
   case INST_NO_OFFSET:
      /* Used to be a reference to somewhere which was unknown. */
      if (fragP->fr_symbol) 
      {
         if (fragP->fr_opcode == NULL) {
            /* Used as an absolute value */
            fragP->fr_subtype = DEFINED_ABS_SEGMENT;
            /* Variable part does not change */
            fragP->fr_var = INST_WORD_SIZE*2;
         } else if (!strcmp(fragP->fr_opcode, str_microblaze_ro_anchor)) {
            /* It is accessed using the small data read only anchor */
            if ((S_GET_SEGMENT (fragP->fr_symbol) == sbss2_segment) ||
                (S_GET_SEGMENT (fragP->fr_symbol) == sdata2_segment) ||
                (! S_IS_DEFINED (fragP->fr_symbol))) {
               fragP->fr_subtype = DEFINED_RO_SEGMENT;
               fragP->fr_var = INST_WORD_SIZE;
            } else {
               /* Variable not in small data read only segment accessed using small data read only anchor */
               char *file = fragP->fr_file ? fragP->fr_file : _("unknown");
               as_bad_where(file, fragP->fr_line,
                            _("Variable is accessed using small data read only anchor, but it is not in the small data read only section"));
               fragP->fr_subtype = DEFINED_RO_SEGMENT;
               fragP->fr_var = INST_WORD_SIZE;
            }
         } else if (!strcmp(fragP->fr_opcode, str_microblaze_rw_anchor)) { 
            if ((S_GET_SEGMENT (fragP->fr_symbol) == sbss_segment) ||
                (S_GET_SEGMENT (fragP->fr_symbol) == sdata_segment) ||
                (! S_IS_DEFINED (fragP->fr_symbol))) {
               /* It is accessed using the small data read write anchor */
               fragP->fr_subtype = DEFINED_RW_SEGMENT;
               fragP->fr_var = INST_WORD_SIZE;
            } else {
               char *file = fragP->fr_file ? fragP->fr_file : _("unknown");
               as_bad_where(file, fragP->fr_line,
                            _("Variable is accessed using small data read write anchor, but it is not in the small data read write section"));
               fragP->fr_subtype = DEFINED_RW_SEGMENT;
               fragP->fr_var = INST_WORD_SIZE;
            }
         } else {
            as_bad(_("Incorrect fr_opcode value in frag.  Internal error....."));
            abort();
         }
      }
      else
      {
         /* We know the abs value: Should never happen */
         as_bad(_("Absolute value in relaxation code.  Assembler error....."));
         abort();
      }

      break;
   case UNDEFINED_PC_OFFSET:
   case LARGE_DEFINED_PC_OFFSET:
   case DEFINED_ABS_SEGMENT:
      fragP->fr_var = INST_WORD_SIZE*2;
      break;
   case DEFINED_RO_SEGMENT:
   case DEFINED_RW_SEGMENT:
   case DEFINED_PC_OFFSET:
      fragP->fr_var = INST_WORD_SIZE;
      break;
   default:
      abort ();
      
   }
  
   return fragP->fr_var;
}

/* Put number into target byte order.  */
void
md_number_to_chars (ptr, use, nbytes)
   char * ptr;
valueT use;
int nbytes;
{
   if (! target_big_endian)
      switch (nbytes)
      {
      case 4: ptr[3] = (use >> 24) & 0xff; /* fall through */
      case 3: ptr[2] = (use >> 16) & 0xff; /* fall through */
      case 2: ptr[1] = (use >>  8) & 0xff; /* fall through */
      case 1: ptr[0] = (use >>  0) & 0xff;    break;
      default: abort ();
      }
   else
      switch (nbytes)
      {
      case 4: *ptr++ = (use >> 24) & 0xff; /* fall through */
      case 3: *ptr++ = (use >> 16) & 0xff; /* fall through */
      case 2: *ptr++ = (use >>  8) & 0xff; /* fall through */
      case 1: *ptr++ = (use >>  0) & 0xff;    break;
      default: abort ();
      }
}

/* Round up a section size to the appropriate boundary.  */
valueT
md_section_align (segment, size)
   segT segment ATTRIBUTE_UNUSED;
valueT size;
{
   return size;			/* Byte alignment is fine */
}


/* The location from which a PC relative jump should be calculated,
   given a PC relative reloc.  */

long
md_pcrel_from_section (fixp, sec)
   fixS * fixp;
segT sec ATTRIBUTE_UNUSED;
{
#ifdef OBJ_ELF
  /* If the symbol is undefined or defined in another section
     we leave the add number alone for the linker to fix it later.
     Only account for the PC pre-bump (No PC-pre-bump on the Microblaze). */
   
   if (fixp->fx_addsy != (symbolS *) NULL
       && (! S_IS_DEFINED (fixp->fx_addsy)
           || (S_GET_SEGMENT (fixp->fx_addsy) != sec)))
      
   {
      /* assert (fixp->fx_size == 4);	*//* must be an insn */
      return 0;
   }
   else {
      /* The case where we are going to resolve things... */
      if (fixp->fx_r_type == BFD_RELOC_64_PCREL)
         return  fixp->fx_where + fixp->fx_frag->fr_address + INST_WORD_SIZE;
      else
         return  fixp->fx_where + fixp->fx_frag->fr_address;
   }
#endif
   
}


#define F(SZ,PCREL)		(((SZ) << 1) + (PCREL))
#define MAP(SZ,PCREL,TYPE)	case F (SZ, PCREL): code = (TYPE); break

arelent *
tc_gen_reloc (section, fixp)
   asection * section ATTRIBUTE_UNUSED;
fixS * fixp;
{
   arelent * rel;
   bfd_reloc_code_real_type code;

   switch (fixp->fx_r_type)
   {
   case BFD_RELOC_NONE:
   case BFD_RELOC_MICROBLAZE_64_NONE:
   case BFD_RELOC_32:
   case BFD_RELOC_MICROBLAZE_32_LO:
   case BFD_RELOC_MICROBLAZE_32_LO_PCREL:
   case BFD_RELOC_RVA:
   case BFD_RELOC_64:
   case BFD_RELOC_64_PCREL:
   case BFD_RELOC_MICROBLAZE_32_ROSDA:
   case BFD_RELOC_MICROBLAZE_32_RWSDA:
   case BFD_RELOC_MICROBLAZE_32_SYM_OP_SYM:      
      code = fixp->fx_r_type;
      break;
    
   default:
      printf("Got a reloc of size %d and pcrel %d\n", fixp->fx_size, fixp->fx_pcrel);
      switch (F (fixp->fx_size, fixp->fx_pcrel))
      {
         MAP (1, 0, BFD_RELOC_8);
         MAP (2, 0, BFD_RELOC_16);
         MAP (4, 0, BFD_RELOC_32);
         MAP (1, 1, BFD_RELOC_8_PCREL);
         MAP (2, 1, BFD_RELOC_16_PCREL);
         MAP (4, 1, BFD_RELOC_32_PCREL);
      default:
         code = fixp->fx_r_type;
         as_bad (_("Can not do %d byte %srelocation"),
                 fixp->fx_size,
                 fixp->fx_pcrel ? _("pc-relative") : "");
      }
      break;
   }

   rel = (arelent *) xmalloc (sizeof (arelent));
   rel->sym_ptr_ptr = (asymbol **) xmalloc (sizeof (asymbol *));
   
   if (code == BFD_RELOC_MICROBLAZE_32_SYM_OP_SYM)
      *rel->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_subsy);
   else
      *rel->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);
   
   rel->address = fixp->fx_frag->fr_address + fixp->fx_where;
   /* Always pass the addend along!  */
   rel->addend = fixp->fx_addnumber; 
   rel->howto = bfd_reloc_type_lookup (stdoutput, code);
  
   if (rel->howto == NULL)
   {
      as_bad_where (fixp->fx_file, fixp->fx_line,
                    _("Cannot represent relocation type %s"),
                    bfd_get_reloc_code_name (code));
      
      /* Set howto to a garbage value so that we can keep going.*/
      rel->howto = bfd_reloc_type_lookup (stdoutput, BFD_RELOC_32);
      assert (rel->howto != NULL);
   }
   return rel;
}

int
md_parse_option (c, arg)
   int c;
char * arg ATTRIBUTE_UNUSED;
{
   switch (c) {
   default:
      return 0;
   }
   return 1;
}

void
md_show_usage (stream)
   FILE * stream ATTRIBUTE_UNUSED;
{
   /*  fprintf(stream, _("\
       MicroBlaze options:\n\
       -noSmall         Data in the comm and data sections do not go into the small data section\n")); */
}


/* Create a fixup for a cons expression.  If parse_cons_expression_microblaze
   found a machine specific op in an expression,
   then we create relocs accordingly
*/
void
cons_fix_new_microblaze (frag, where, size, exp)
   fragS * frag;
int where;
int size;
expressionS *exp;
{

   bfd_reloc_code_real_type r;
   
   if ( (exp->X_op == O_subtract) && (exp->X_add_symbol) &&
        (exp->X_op_symbol) && (now_seg != absolute_section) && (size == 4)
        && (!S_IS_LOCAL(exp->X_op_symbol))
        ) {
      r = BFD_RELOC_MICROBLAZE_32_SYM_OP_SYM;
   }
   else {
      switch (size)
      {
      case 1:
         r = BFD_RELOC_8;
         break;
      case 2:
         r = BFD_RELOC_16;
      break;
      case 4:
         r = BFD_RELOC_32;
         break;
      case 8:
         r = BFD_RELOC_64;
         break;
      default:
         as_bad (_("unsupported BFD relocation size %u"), size);
         r = BFD_RELOC_32;
         break;
      }
   }
   fix_new_exp (frag, where, size, exp, 0, r);
}

