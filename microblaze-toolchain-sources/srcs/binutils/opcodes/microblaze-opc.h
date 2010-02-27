#ifndef MICROBLAZE_OPC
#define MICROBLAZE_OPC
/* Assembler instructions for Xilinx's microblaze processor
   Copyright (C) 1999, 2000 Free Software Foundation, Inc.

   
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

#include "microblaze-opcm.h"


#define INST_TYPE_RD_R1_R2 0
#define INST_TYPE_RD_R1_IMM 1
#define INST_TYPE_RD_R1_UNSIGNED_IMM 2
#define INST_TYPE_RD_R1 3
#define INST_TYPE_RD_R2 4
#define INST_TYPE_RD_IMM 5
#define INST_TYPE_R2 6
#define INST_TYPE_R1_R2 7
#define INST_TYPE_R1_IMM 8
#define INST_TYPE_IMM 9
#define INST_TYPE_SPECIAL_R1 10
#define INST_TYPE_RD_SPECIAL 11
#define INST_TYPE_R1 12
  // new instn type for barrel shift imms
#define INST_TYPE_RD_R1_IMM5  13
#define INST_TYPE_RD_IMM12    14
#define INST_TYPE_R1_IMM12    15

  // new insn type for insn cache
#define INST_TYPE_RD_R1_SPECIAL 16

// new insn type for msrclr, msrset insns.
#define INST_TYPE_RD_IMM14    17

// new insn type for tuqula rd - addik rd, r0, 42
#define INST_TYPE_RD    18

#define INST_TYPE_NONE 25



#define INST_PC_OFFSET 1 /* instructions where the label address is resolved as a PC offset (for branch label)*/
#define INST_NO_OFFSET 0 /* instructions where the label address is resolved as an absolute value (for data mem or abs address)*/

#define IMMVAL_MASK_NON_SPECIAL 0x0000
#define IMMVAL_MASK_MTS 0x4000
#define IMMVAL_MASK_MFS 0x0000

#define OPCODE_MASK_H   0xFC000000 /* High 6 bits only */
#define OPCODE_MASK_H1  0xFFE00000 /* High 11 bits */
#define OPCODE_MASK_H2  0xFC1F0000 /* High 6 and bits 20-16 */
#define OPCODE_MASK_H12 0xFFFF0000 /* High 16 */
#define OPCODE_MASK_H4  0xFC0007FF /* High 6 and low 11 bits */
#define OPCODE_MASK_H13S 0xFFE0FFF0 /* High 11 and 15:1 bits and last nibble of last byte for spr */
#define OPCODE_MASK_H23S 0xFC1FFFF0 /* High 6, 20-16 and 15:1 bits and last nibble of last byte for spr */
#define OPCODE_MASK_H34 0xFC00FFFF /* High 6 and low 16 bits */
#define OPCODE_MASK_H14 0xFFE007FF /* High 11 and low 11 bits */
#define OPCODE_MASK_H24 0xFC1F07FF /* High 6, bits 20-16 and low 11 bits */
#define OPCODE_MASK_H124  0xFFFF07FF /* High 16, and low 11 bits */
#define OPCODE_MASK_H1234 0xFFFFFFFF /* All 32 bits */
#define OPCODE_MASK_H3  0xFC000600 /* High 6 bits and bits 21, 22 */  
#define OPCODE_MASK_H32 0xFC00F000 /* High 6 bits and bit 16, 17, 18 and 19*/
#define OPCODE_MASK_H34B   0xFC0000FF /* High 6 bits and low 8 bits */

// New Mask for msrset, msrclr insns.
#define OPCODE_MASK_H23N  0xFC1FC000 /* High 6 and bits 12 - 18 */

#define DELAY_SLOT 1
#define NO_DELAY_SLOT 0

#define MAX_OPCODES 149

struct op_code_struct {
  char *name;
  short inst_type; /* registers and immediate values involved */
  short inst_offset_type; /* immediate vals offset from PC? (= 1 for branches) */
  short delay_slots; /* info about delay slots needed after this instr. */
  short immval_mask;
  unsigned long bit_sequence; /* all the fixed bits for the op are set and all the variable bits (reg names, imm vals) are set to 0 */ 
  unsigned long opcode_mask; /* which bits define the opcode */
  enum microblaze_instr instr;
  enum microblaze_instr_type instr_type;
  /* more info about output format here */
} opcodes[MAX_OPCODES] = 

{ 
  {"add",   INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x00000000, OPCODE_MASK_H4, add, arithmetic_inst },
  {"rsub",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x04000000, OPCODE_MASK_H4, rsub, arithmetic_inst },
  {"addc",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x08000000, OPCODE_MASK_H4, addc, arithmetic_inst },
  {"rsubc", INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x0C000000, OPCODE_MASK_H4, rsubc, arithmetic_inst },
  {"addk",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x10000000, OPCODE_MASK_H4, addk, arithmetic_inst },
  {"rsubk", INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x14000000, OPCODE_MASK_H4, rsubk, arithmetic_inst },
  {"cmp",   INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x14000001, OPCODE_MASK_H4, cmp, arithmetic_inst },
  {"cmpu",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x14000003, OPCODE_MASK_H4, cmpu, arithmetic_inst },
  {"addkc", INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x18000000, OPCODE_MASK_H4, addkc, arithmetic_inst },
  {"rsubkc",INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x1C000000, OPCODE_MASK_H4, rsubkc, arithmetic_inst },
  {"addi",  INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x20000000, OPCODE_MASK_H, addi, arithmetic_inst },
  {"rsubi", INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x24000000, OPCODE_MASK_H, rsubi, arithmetic_inst },
  {"addic", INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x28000000, OPCODE_MASK_H, addic, arithmetic_inst },
  {"rsubic",INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x2C000000, OPCODE_MASK_H, rsubic, arithmetic_inst },
  {"addik", INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x30000000, OPCODE_MASK_H, addik, arithmetic_inst },
  {"rsubik",INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x34000000, OPCODE_MASK_H, rsubik, arithmetic_inst },
  {"addikc",INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x38000000, OPCODE_MASK_H, addikc, arithmetic_inst },
  {"rsubikc",INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x3C000000, OPCODE_MASK_H, rsubikc, arithmetic_inst },
  {"mul",   INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x40000000, OPCODE_MASK_H4, mul, mult_inst },
  {"idiv",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x48000000, OPCODE_MASK_H4, idiv, div_inst },
  {"idivu", INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x48000002, OPCODE_MASK_H4, idivu, div_inst },
  {"bsll",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x44000400, OPCODE_MASK_H3, bsll, barrel_shift_inst },
  {"bsra",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x44000200, OPCODE_MASK_H3, bsra, barrel_shift_inst },
  {"bsrl",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x44000000, OPCODE_MASK_H3, bsrl, barrel_shift_inst },
  {"get",   INST_TYPE_RD_IMM12, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x6C000000, OPCODE_MASK_H32, get, anyware_inst },
  {"put",   INST_TYPE_R1_IMM12, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x6C008000, OPCODE_MASK_H32, put, anyware_inst },
  {"nget",  INST_TYPE_RD_IMM12, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x6C004000, OPCODE_MASK_H32, nget, anyware_inst },
  {"nput",  INST_TYPE_R1_IMM12, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x6C00C000, OPCODE_MASK_H32, nput, anyware_inst },
  {"cget",  INST_TYPE_RD_IMM12, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x6C002000, OPCODE_MASK_H32, cget, anyware_inst },
  {"cput",  INST_TYPE_R1_IMM12, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x6C00A000, OPCODE_MASK_H32, cput, anyware_inst },
  {"ncget", INST_TYPE_RD_IMM12, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x6C006000, OPCODE_MASK_H32, ncget, anyware_inst },
  {"ncput", INST_TYPE_R1_IMM12, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x6C00E000, OPCODE_MASK_H32, ncput, anyware_inst },
  {"muli",  INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x60000000, OPCODE_MASK_H, muli, mult_inst },
  {"bslli", INST_TYPE_RD_R1_IMM5, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x64000400, OPCODE_MASK_H3, bslli, barrel_shift_inst },
  {"bsrai", INST_TYPE_RD_R1_IMM5, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x64000200, OPCODE_MASK_H3, bsrai, barrel_shift_inst },
  {"bsrli", INST_TYPE_RD_R1_IMM5, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x64000000, OPCODE_MASK_H3, bsrli, barrel_shift_inst },
  {"or",    INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x80000000, OPCODE_MASK_H4, or, logical_inst },
  {"and",   INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x84000000, OPCODE_MASK_H4, and, logical_inst },
  {"xor",   INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x88000000, OPCODE_MASK_H4, xor, logical_inst },
  {"andn",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x8C000000, OPCODE_MASK_H4, andn, logical_inst },
  {"pcmpbf",INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x80000400, OPCODE_MASK_H4, pcmpbf, logical_inst },
  {"pcmpbc",INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x84000400, OPCODE_MASK_H4, pcmpbc, logical_inst },
  {"pcmpeq",INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x88000400, OPCODE_MASK_H4, pcmpeq, logical_inst },
  {"pcmpne",INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x8C000400, OPCODE_MASK_H4, pcmpne, logical_inst },
  {"sra",   INST_TYPE_RD_R1, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x90000001, OPCODE_MASK_H34, sra, logical_inst },
  {"src",   INST_TYPE_RD_R1, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x90000021, OPCODE_MASK_H34, src, logical_inst },
  {"srl",   INST_TYPE_RD_R1, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x90000041, OPCODE_MASK_H34, srl, logical_inst },
  {"sext8", INST_TYPE_RD_R1, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x90000060, OPCODE_MASK_H34, sext8, logical_inst },
  {"sext16",INST_TYPE_RD_R1, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x90000061, OPCODE_MASK_H34, sext16, logical_inst },
  {"wic",   INST_TYPE_RD_R1_SPECIAL, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x90000068, OPCODE_MASK_H34B, wic, special_inst },
  {"wdc",   INST_TYPE_RD_R1_SPECIAL, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x90000064, OPCODE_MASK_H34B, wdc, special_inst },
  {"mts",   INST_TYPE_SPECIAL_R1, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_MTS, 0x9400C000, OPCODE_MASK_H13S, mts, special_inst },
  {"mfs",   INST_TYPE_RD_SPECIAL, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_MFS, 0x94008000, OPCODE_MASK_H23S, mfs, special_inst },
  {"br",    INST_TYPE_R2, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x98000000, OPCODE_MASK_H124, br, branch_inst },
  {"brd",   INST_TYPE_R2, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x98100000, OPCODE_MASK_H124, brd, branch_inst },
  {"brld",  INST_TYPE_RD_R2, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x98140000, OPCODE_MASK_H24, brld, branch_inst },
  {"bra",   INST_TYPE_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x98080000, OPCODE_MASK_H124, bra, branch_inst },
  {"brad",  INST_TYPE_R2, INST_NO_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x98180000, OPCODE_MASK_H124, brad, branch_inst },
  {"brald", INST_TYPE_RD_R2, INST_NO_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x981C0000, OPCODE_MASK_H24, brald, branch_inst },
  {"brk",   INST_TYPE_RD_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x980C0000, OPCODE_MASK_H24, microblaze_brk, branch_inst },
  {"beq",   INST_TYPE_R1_R2, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9C000000, OPCODE_MASK_H14, beq, branch_inst },
  {"beqd",  INST_TYPE_R1_R2, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9E000000, OPCODE_MASK_H14, beqd, branch_inst },
  {"bne",   INST_TYPE_R1_R2, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9C200000, OPCODE_MASK_H14, bne, branch_inst },
  {"bned",  INST_TYPE_R1_R2, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9E200000, OPCODE_MASK_H14, bned, branch_inst },
  {"blt",   INST_TYPE_R1_R2, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9C400000, OPCODE_MASK_H14, blt, branch_inst },
  {"bltd",  INST_TYPE_R1_R2, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9E400000, OPCODE_MASK_H14, bltd, branch_inst },
  {"ble",   INST_TYPE_R1_R2, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9C600000, OPCODE_MASK_H14, ble, branch_inst },
  {"bled",  INST_TYPE_R1_R2, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9E600000, OPCODE_MASK_H14, bled, branch_inst },
  {"bgt",   INST_TYPE_R1_R2, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9C800000, OPCODE_MASK_H14, bgt, branch_inst },
  {"bgtd",  INST_TYPE_R1_R2, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9E800000, OPCODE_MASK_H14, bgtd, branch_inst },
  {"bge",   INST_TYPE_R1_R2, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9CA00000, OPCODE_MASK_H14, bge, branch_inst },
  {"bged",  INST_TYPE_R1_R2, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x9EA00000, OPCODE_MASK_H14, bged, branch_inst },
  {"ori",   INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xA0000000, OPCODE_MASK_H, ori, logical_inst },
  {"andi",  INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xA4000000, OPCODE_MASK_H, andi, logical_inst },
  {"xori",  INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xA8000000, OPCODE_MASK_H, xori, logical_inst },
  {"andni", INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xAC000000, OPCODE_MASK_H, andni, logical_inst },
  {"imm",   INST_TYPE_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB0000000, OPCODE_MASK_H12, imm, immediate_inst },
  {"rtsd",  INST_TYPE_R1_IMM, INST_NO_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB6000000, OPCODE_MASK_H1, rtsd, return_inst },
  {"rtid",  INST_TYPE_R1_IMM, INST_NO_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB6200000, OPCODE_MASK_H1, rtid, return_inst },
  {"rtbd",  INST_TYPE_R1_IMM, INST_NO_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB6400000, OPCODE_MASK_H1, rtbd, return_inst },
  {"rted",  INST_TYPE_R1_IMM, INST_NO_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB6800000, OPCODE_MASK_H1, rted, return_inst },
  {"bri",   INST_TYPE_IMM, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB8000000, OPCODE_MASK_H12, bri, branch_inst },
  {"brid",  INST_TYPE_IMM, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB8100000, OPCODE_MASK_H12, brid, branch_inst },
  {"brlid", INST_TYPE_RD_IMM, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB8140000, OPCODE_MASK_H2, brlid, branch_inst },
  {"brai",  INST_TYPE_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB8080000, OPCODE_MASK_H12, brai, branch_inst },
  {"braid", INST_TYPE_IMM, INST_NO_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB8180000, OPCODE_MASK_H12, braid, branch_inst },
  {"bralid",INST_TYPE_RD_IMM, INST_NO_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB81C0000, OPCODE_MASK_H2, bralid, branch_inst },
  {"brki",  INST_TYPE_RD_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB80C0000, OPCODE_MASK_H2, brki, branch_inst },
  {"beqi",  INST_TYPE_R1_IMM, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBC000000, OPCODE_MASK_H1, beqi, branch_inst },
  {"beqid", INST_TYPE_R1_IMM, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBE000000, OPCODE_MASK_H1, beqid, branch_inst },
  {"bnei",  INST_TYPE_R1_IMM, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBC200000, OPCODE_MASK_H1, bnei, branch_inst },
  {"bneid", INST_TYPE_R1_IMM, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBE200000, OPCODE_MASK_H1, bneid, branch_inst },
  {"blti",  INST_TYPE_R1_IMM, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBC400000, OPCODE_MASK_H1, blti, branch_inst },
  {"bltid", INST_TYPE_R1_IMM, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBE400000, OPCODE_MASK_H1, bltid, branch_inst },
  {"blei",  INST_TYPE_R1_IMM, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBC600000, OPCODE_MASK_H1, blei, branch_inst },
  {"bleid", INST_TYPE_R1_IMM, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBE600000, OPCODE_MASK_H1, bleid, branch_inst },
  {"bgti",  INST_TYPE_R1_IMM, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBC800000, OPCODE_MASK_H1, bgti, branch_inst },
  {"bgtid", INST_TYPE_R1_IMM, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBE800000, OPCODE_MASK_H1, bgtid, branch_inst },
  {"bgei",  INST_TYPE_R1_IMM, INST_PC_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBCA00000, OPCODE_MASK_H1, bgei, branch_inst },
  {"bgeid", INST_TYPE_R1_IMM, INST_PC_OFFSET, DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xBEA00000, OPCODE_MASK_H1, bgeid, branch_inst },
  {"lbu",   INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xC0000000, OPCODE_MASK_H4, lbu, memory_load_inst },
  {"lhu",   INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xC4000000, OPCODE_MASK_H4, lhu, memory_load_inst },
  {"lw",    INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xC8000000, OPCODE_MASK_H4, lw, memory_load_inst },
  {"sb",    INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xD0000000, OPCODE_MASK_H4, sb, memory_store_inst },
  {"sh",    INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xD4000000, OPCODE_MASK_H4, sh, memory_store_inst },
  {"sw",    INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xD8000000, OPCODE_MASK_H4, sw, memory_store_inst },
  {"lbui",  INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xE0000000, OPCODE_MASK_H, lbui, memory_load_inst },
  {"lhui",  INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xE4000000, OPCODE_MASK_H, lhui, memory_load_inst },
  {"lwi",   INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xE8000000, OPCODE_MASK_H, lwi, memory_load_inst },
  {"sbi",   INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xF0000000, OPCODE_MASK_H, sbi, memory_store_inst },
  {"shi",   INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xF4000000, OPCODE_MASK_H, shi, memory_store_inst },
  {"swi",   INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xF8000000, OPCODE_MASK_H, swi, memory_store_inst },
  {"nop",   INST_TYPE_NONE, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x80000000, OPCODE_MASK_H1234, invalid_inst, logical_inst }, /* translates to or r0, r0, r0 */
  {"la",    INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x30000000, OPCODE_MASK_H, invalid_inst, arithmetic_inst }, /* la translates to addik */
  {"tuqula",INST_TYPE_RD, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x3000002A, OPCODE_MASK_H, invalid_inst, arithmetic_inst }, /* tuqula rd translates to addik rd, r0, 42 */
  {"not",   INST_TYPE_RD_R1, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xA800FFFF, OPCODE_MASK_H34, invalid_inst, logical_inst }, /* not translates to xori rd,ra,-1 */
  {"neg",   INST_TYPE_RD_R1, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x04000000, OPCODE_MASK_H, invalid_inst, arithmetic_inst }, /* neg translates to rsub rd, ra, r0 */
  {"rtb",   INST_TYPE_R1, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xB6000004, OPCODE_MASK_H1, invalid_inst, return_inst }, /* rtb translates to rts rd, 4 */
  {"sub",   INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x04000000, OPCODE_MASK_H, invalid_inst, arithmetic_inst }, /* sub translates to rsub rd, rb, ra */
  {"lmi",   INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xE8000000, OPCODE_MASK_H, invalid_inst, memory_load_inst },
  {"smi",   INST_TYPE_RD_R1_IMM, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0xF8000000, OPCODE_MASK_H, invalid_inst, memory_store_inst },
  {"msrset",INST_TYPE_RD_IMM14, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x94100000, OPCODE_MASK_H23N, msrset, special_inst },
  {"msrclr",INST_TYPE_RD_IMM14, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x94110000, OPCODE_MASK_H23N, msrclr, special_inst },
  {"fadd",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x58000000, OPCODE_MASK_H4, fadd, arithmetic_inst },
  {"frsub",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x58000080, OPCODE_MASK_H4, frsub, arithmetic_inst },
  {"fmul",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x58000100, OPCODE_MASK_H4, fmul, arithmetic_inst },
  {"fdiv",  INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x58000180, OPCODE_MASK_H4, fdiv, arithmetic_inst },
  {"fcmp.lt", INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x58000210, OPCODE_MASK_H4, fcmp_lt, arithmetic_inst },
  {"fcmp.eq", INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x58000220, OPCODE_MASK_H4, fcmp_eq, arithmetic_inst },
  {"fcmp.le", INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x58000230, OPCODE_MASK_H4, fcmp_le, arithmetic_inst },
  {"fcmp.gt", INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x58000240, OPCODE_MASK_H4, fcmp_gt, arithmetic_inst },
  {"fcmp.ne", INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x58000250, OPCODE_MASK_H4, fcmp_ne, arithmetic_inst },
  {"fcmp.ge", INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x58000260, OPCODE_MASK_H4, fcmp_ge, arithmetic_inst },
  {"fcmp.un", INST_TYPE_RD_R1_R2, INST_NO_OFFSET, NO_DELAY_SLOT, IMMVAL_MASK_NON_SPECIAL, 0x58000200, OPCODE_MASK_H4, fcmp_un, arithmetic_inst },
  {""}
};

/* prefix for register names */
char register_prefix[] = "r";
char special_register_prefix[] = "spr";
char fsl_register_prefix[] = "rfsl";


/* #defines for valid immediate range */
#define MIN_IMM  0x80000000
#define MAX_IMM  0x7fffffff 

#define MIN_IMM12  0x000
#define MAX_IMM12  0x7ff

#define MIN_IMM14  0x0000
#define MAX_IMM14  0x1fff

#endif /* MICROBLAZE_OPC */

