#ifndef MICROBLAZE_H
#define MICROBLAZE_H

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
 * $Header:
 */
#include "../../opcodes/microblaze-opcm.h"

#define GET_RD	((inst & RD_MASK) >> RD_LOW)
#define GET_RA	((inst & RA_MASK) >> RA_LOW)
#define GET_RB	((inst & RB_MASK) >> RB_LOW)

#define CPU     microblaze_state.cpu[0].microblaze_cpu

#define RD      CPU.regs[rd]
#define RA      CPU.regs[ra]
#define RB      CPU.regs[rb]
/* #define IMM     immword */

#define SA	CPU.spregs[IMM & 0x1]

#define IMM_H	CPU.imm_high
#define IMM_L	((inst & IMM_MASK) >> IMM_LOW)

#define IMM_ENABLE CPU.imm_enable

#define IMM             (IMM_ENABLE ?					\
                         (((uhalf)IMM_H << 16) | (uhalf)IMM_L) :	\
                         (imm_unsigned ?				\
			  (0xFFFF & IMM_L) :				\
                          (IMM_L & 0x8000 ?                             \
			   (0xFFFF0000 | IMM_L) :                       \
			   (0x0000FFFF & IMM_L))))

#define PC	CPU.spregs[0]
#define	MSR	CPU.spregs[1]
#define SP      CPU.regs[29]
#define RETREG  CPU.regs[1]


#define MEM(X)	memory[X]

#define MEM_RD_BYTE(X)	rbat(X)
#define MEM_RD_HALF(X)	rhat(X)
#define MEM_RD_WORD(X)	rlat(X)
#define MEM_RD_UBYTE(X) (ubyte)MEM_RD_BYTE(X)
#define MEM_RD_UHALF(X) (uhalf)MEM_RD_HALF(X) 
#define MEM_RD_UWORD(X) (uword)MEM_RD_WORD(X) 

#define MEM_WR_BYTE(X,D) wbat(X, D)
#define MEM_WR_HALF(X,D) what(X, D)
#define MEM_WR_WORD(X,D) wlat(X, D)


#define MICROBLAZE_SEXT8(X)	((char) X)
#define MICROBLAZE_SEXT16(X)	((short) X)


#define CARRY		carry
#define C_rd		((MSR & 0x4) >> 2)
#define C_wr(D)		MSR = (D ? MSR | 0x80000004 : MSR & 0x7FFFFFFB)
//#define OVF		overflow
//#define O_rd		(MSR & 0x2)
//#define O_wr(D)		MSR = (D ? MSR | 0x2 : MSR & 0xFFFD)

#define C_calc(X,Y,C)	((((uword)Y == MAX_WORD) && (C == 1)) ?		 \
			 1 :						 \
			 ((MAX_WORD - (uword)X) < ((uword)Y + C)))

#define BIP_MASK	0x00000008
#define CARRY_MASK	0x00000004
#define INTR_EN_MASK	0x00000002
#define BUSLOCK_MASK	0x00000001

#define DELAY_SLOT      delay_slot_enable = 1
#define BRANCH          branch_taken = 1

#define NUM_REGS 	32
#define NUM_SPECIAL 	2
#define INST_SIZE 	4

#define MAX_WORD	0xFFFFFFFF
#define MICROBLAZE_HALT_INST  0xb8000000

typedef char		byte;
typedef short		half;
typedef int		word;
typedef unsigned char	ubyte;
typedef unsigned short	uhalf;
typedef unsigned int	uword;

#endif /* MICROBLAZE_H */

