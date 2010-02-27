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
// strcmp.S 
//
// Compare strings, This function is optimized for MicroBlaze.
//       Input : String1 in Reg r5
//               String2 in Reg r6
//       Output: If the strings are equal R3 == 0
//               else R3 = difference between the value of the first differing byte
// 
// $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/newlib-3.4/stringops/strcmp.S,v 1.2.2.1 2006/01/06 22:23:28 vasanth Exp $
//
////////////////////////////////////////////////////////////////////////////

        

        .align 2
        .section .text
        .global strcmp
        .ent strcmp
strcmp:
        or      r9, r0, r0              /* Index register */
check_alignment:        
        andi    r3, r5, 3
        andi    r4, r6, 3
        bnei    r3, try_align_args
        bnei    r4, regular_strcmp      /* At this point we don't have a choice */                       
cmp_loop:       
        lw      r3, r5, r9
        lw      r4, r6, r9
        pcmpbf  r7, r3, r0              /* See if there is Null byte */  
        bnei    r7, end_cmp_loop        /* IF yes (r7 > 0) use byte compares in end_cmp_loop */
        cmpu    r7, r4, r3              /* ELSE compare whole word */
        bnei    r7, end_cmp                   
        brid    cmp_loop
        addik   r9, r9, 4               /* delay slot */
end_cmp_loop:        
        lbu     r3, r5, r9              /* byte compare loop */
        lbu     r4, r6, r9        
        cmpu    r7, r4, r3              /* Compare bytes */
        bnei    r7, end_cmp_early
        bneid   r3, end_cmp_loop        /* If reached null on one string, terminate */
        addik   r9, r9, 1               /* delay slot */
end_cmp_early:  
        rtsd    r15, 8
        or      r3, r0, r7                        
try_align_args: 
        xor     r7, r4, r3
        bnei    r7, regular_strcmp      /* cannot align args */
        rsubik  r10, r3, 4              /* Number of initial bytes to align */
align_loop:     
        lbu     r3, r5, r9
        lbu     r4, r6, r9
        cmpu    r7, r4, r3
        bnei    r7, end_cmp
        beqi    r3, end_cmp
        addik   r10, r10, -1
        beqid   r10, cmp_loop
        addik   r9, r9, 1
        bri     align_loop        
regular_strcmp:
        lbu     r3, r5, r9
        lbu     r4, r6, r9
        cmpu    r7, r4, r3
        bnei    r7, end_cmp
        beqi    r3, end_cmp
        brid    regular_strcmp
        addik   r9, r9, 1
end_cmp:
        rtsd    r15, 8               
        or      r3, r0, r7              /* Return strcmp result */
        .end strcmp                
