###################################-*-asm*- 
# 
# Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
# 
# XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
# COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
# ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
# STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
# IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
# FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.  
# XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
# THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
# ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
# FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
# AND FITNESS FOR A PARTICULAR PURPOSE.
#
# crt1.s 
# 
# Default C run-time initialization for MicroBlaze standalone 
# executables compiled with -xl-mode-xmdstub for 
# debugging with XMDSTUB 
#    
# $Id: crt1.s,v 1.9 2005/11/21 21:23:37 vasanth Exp $
# 
#######################################


/*
        
        MicroBlaze Vector Map for standalone executables
        downloaded and executed with XMDSTUB

         Address                Vector type                 Label
         -------                -----------                 ------        
       
	# 0x00 #		(-- IMM --)
	# 0x04 #                Reset                       (-- Don't Care --)
    
	# 0x08 #		(-- IMM --)
	# 0x0c #		Software Exception          _exception_handler
    
	# 0x10 #		(-- IMM --)
	# 0x14 #		Hardware Interrupt          _interrupt_handler
    
        # 0x18 #                (-- IMM --)
        # 0x1C #                Breakpoint Exception        (-- Don't Care --)
    
        # 0x20 #                (-- IMM --) 
        # 0x24 #                Hardware Exception          _hw_exception_handler

*/      
    

        .section .vectors.sw_exception, "ax"
        .align 2
_vector_sw_exception:       
        brai    _exception_handler

        .section .vectors.interrupt, "ax"
        .align 2
_vector_interrupt:      
        brai    _interrupt_handler

        .section .vectors.hw_exception, "ax"
        .align 2
_vector_hw_exception:       
        brai    _hw_exception_handler

        .section .text
        .globl _start
        .align 2
_start:
	la	r13, r0, _SDA_BASE_         /* Set the Small Data Anchors and the stack pointer */
	la	r2, r0, _SDA2_BASE_
	la	r1, r0, _stack-16           /* 16 bytes (4 words are needed by crtinit for args and link reg */

	brlid	r15, _crtinit               /* Initialize BSS and run program */
	nop

	addi	r1, r1, 16

        /* Fall through to exit */
/* 
        exit 
        Our simple exit      
*/    
        .globl exit
        .align 2
        .ent exit
exit:
	add	r3, r0, r5
	brki    r16, 0x4                    /* Return to hook in XMDSTUB  */    
	.end exit        

