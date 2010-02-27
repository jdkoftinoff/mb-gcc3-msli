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
// strcpy.S 
//
// Copy one string to another. This function is optimized for MicroBlaze 
// when pattern compare instructions are available
//      Input : src pointer in r6, dst pointer in r5.
//      Output: dst pointer in r3.        
//       
// $Id: strcpy.S,v 1.2 2005/10/19 18:46:41 vasanth Exp $
//
//////////////////////////////////////////////////////////////////////////////

        .align 2
        .section .text
        .global strcpy
        .ent strcpy
strcpy:
        or      r9, r0, r0              /* Index register */
check_alignment:        
        andi    r3, r5, 3
        andi    r4, r6, 3
        bnei    r3, try_align_args
        bnei    r4, regular_strcpy      /* At this point we don't have a choice */                       
cpy_loop:      
        lw      r3, r6, r9
        pcmpbf  r4, r0, r3
        bnei    r4, cpy_bytes           /* If r4 != 0, then null present within string */
        sw      r3, r5, r9
        brid    cpy_loop
        addik   r9, r9, 4
cpy_bytes:               
        lbu     r3, r6, r9
        sb      r3, r5, r9
        addik   r4, r4, -1
        bneid   r4, cpy_bytes
        addik   r9, r9, 1               /* delay slot */
cpy_null:      
        rtsd    r15, 8
        or      r3, r0, r5              /* Return strcpy result */
try_align_args: 
        xor     r7, r4, r3
        bnei    r7, regular_strcpy      /* cannot align args */
        rsubik  r10, r3, 4              /* Number of initial bytes to align */
align_loop:     
        lbu     r3, r6, r9
        sb      r3, r5, r9
        addik   r10, r10, -1
        bneid   r10, align_loop
        addik   r9, r9, 1
        bri     cpy_loop
regular_strcpy:
        lbu     r3, r6, r9
        sb      r3, r5, r9
        bneid   r3, regular_strcpy
        addik   r9, r9, 1
end_cpy:
        rtsd    r15, 8               
        or      r3, r0, r5              /* Return strcpy result */
        .end strcpy







