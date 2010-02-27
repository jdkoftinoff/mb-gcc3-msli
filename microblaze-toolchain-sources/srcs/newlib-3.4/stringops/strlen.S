////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004 Xilinx, Inc.  All rights reserved.
//
// Xilinx, Inc.
// XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
// COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
// ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
// STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
// IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
// FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
// XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
// THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
// ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
// FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.
//
// strlen.S 
//
// Length of a string. This function is optimized for MicroBlaze 
// when pattern compare instructions are available
//       Input : String pointer in Reg r5
// 
// $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/newlib-3.4/stringops/strlen.S,v 1.2 2005/10/19 18:46:41 vasanth Exp $
// 
////////////////////////////////////////////////////////////////////////////


        .align 2
        .section .text
        .global strlen
        .ent strlen
strlen:
        or      r9, r0, r0              /* Index register */
check_alignment:        
        andi    r3, r5, 3
        bnei    r3, align_arg                
len_loop:       
        lw      r3, r5, r9
        pcmpbf  r4, r3, r0
        bnei    r4, end_len
        brid    len_loop
        addik   r9, r9, 4
end_len:        
        lbu     r3, r5, r9
        beqi    r3, done_len
        brid    end_len
        addik   r9, r9, 1
done_len:
        rtsd    r15, 8
        or      r3, r0, r9              /* Return len */
align_arg:
        rsubik  r10, r3, 4
align_loop:             
        lbu     r3, r5, r9
        beqi    r3, done_len
        addik   r10, r10, -1
        bneid   r10, align_loop
        addik   r9, r9, 1
        bri     len_loop
        .end strlen                
