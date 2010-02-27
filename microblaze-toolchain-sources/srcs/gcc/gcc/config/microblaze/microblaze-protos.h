/* Definitions of target machine for GNU compiler, for Xilinx MicroBlaze.
   Copyright (C) 2000, 2001, 2002, 2003, 2004
   Free Software Foundation, Inc.
   Contributed by Richard Kenner (kenner@vlsi1.ultra.nyu.edu)

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the
   Free Software Foundation, 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA.  */

#ifndef __MICROBLAZE_PROTOS__
#define __MICROBLAZE_PROTOS__

#ifdef RTX_CODE
extern void barrel_shift_left_imm(rtx operands[]);
extern void shift_left_imm(rtx operands[]);
extern void shift_right_imm(rtx operands[]);
extern rtx embedded_pic_offset       PARAMS ((rtx));
extern int pic_address_needs_scratch PARAMS ((rtx));
extern void expand_block_move        PARAMS ((rtx *));
extern const char* microblaze_move_1word PARAMS ((  rtx[], rtx, int));
extern void shift_left_imm  PARAMS ((rtx []));
extern void microblaze_expand_prologue (void);
extern void microblaze_expand_epilogue (void);
extern void shift_double_left_imm    PARAMS ((rtx []));
extern void override_options (void);
extern void machine_dependent_reorg PARAMS ((void));
#endif  /* RTX_CODE */
#endif  /* __MICROBLAZE_PROTOS__ */
