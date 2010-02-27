/*-----------------------------------------------------------------------------
//     $Date: 2006/03/06 18:14:59 $
//     $RCSfile: xil-sim-pgcrt0.asm,v $
//-----------------------------------------------------------------------------
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
//---------------------------------------------------------------------------*/

	.file	"xil-sim-pgcrt0.S"
	.section ".got2","aw"
	.align	2

.LCTOC1 = . + 32768

.Lsbss_start = .-.LCTOC1
	.long	__sbss_start

.Lsbss_end = .-.LCTOC1
	.long	__sbss_end

.Lbss_start = .-.LCTOC1
	.long	__bss_start

.Lbss_end = .-.LCTOC1
	.long	__bss_end

.Lstack = .-.LCTOC1
	.long	__stack

.Lsda = .-.LCTOC1
    .long   _SDA_BASE_                      /* address of the first small data area */

.Lsda2 = .-.LCTOC1
    .long   _SDA2_BASE_                     /* address of the second small data area */

	.text
	.globl	_start
_start:
        /* CR 203746 Patch for PPC errata */
        mfccr0  0
        oris    0,0,0x50000000@h
        mtccr0  0
    
	lis	5,.LCTOC1@h
	ori	5,5,.LCTOC1@l

        lwz     13,.Lsda(5)             /* load r13 with _SDA_BASE_ address */
        lwz     2,.Lsda2(5)             /* load r2 with _SDA2_BASE_ address */

        /* We don't clear the BSS here.
           The simulator and the user program need to be aware of this 

            ....
    
        */
     
	/* set stack pointer */
	lwz	1,.Lstack(5)		/* stack address */

	/* set up initial stack frame */
	addi	1,1,-8			/* location of back chain */
	lis	0,0
	stw	0,0(1)			/* set end of back chain */
	
	/* initialize base timer to zero */
	mtspr	0x11c,0
	mtspr	0x11d,0

#ifdef HAVE_XFPU   
	/* On the Xilinx PPC, the MSR
           must be explicitly set to mark the prescence
           of an FPU */
        mfmsr   0
        ori     0,0,8192
        oris    0,0,512
        mtmsr   0
#endif    
        /* Setup profiling stuff */
        bl      _profile_init
    
	/* Let her rip */
	bl	main

        /* Invoke the language cleanup functions */        
        bl      __fini

        /* Cleanup profiling stuff */
        bl      _profile_clean
    
        /* All done */
	bl	exit


/* Trap has been removed for both simulation and hardware */
	.globl _exit
_exit:
	b _exit

.Lstart:
	.size	_start,.Lstart-_start

