//////////////////////////////////////////////////////////////////////-*-C++-*- 
// 
// Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
// 
// Xilinx, Inc. 
// 
// free.s
//
// prototype:	void free(void *ptr) 		
// 
// procedure to free memory.  Free is not supported, and does nothing.
//
// Input:	Pointer to memory in reg r5
// Return:	None
//
// 
// $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/newlib-3.4/libgloss/microblaze/free.S,v 1.1 2005/10/18 00:11:41 vasanth Exp $
// 
///////////////////////////////////////////////////////////////////////////////

gcc2_compiled.:
__gnu_compiled_c:

	.globl	free
	.text
	.align	2
	.ent	free
free:
	.frame	r1,0,r15		
	rtsd    r15,8
	nop
	.end    free

