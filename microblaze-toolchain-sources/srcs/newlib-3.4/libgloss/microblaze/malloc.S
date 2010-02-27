//////////////////////////////////////////////////////////////////////-*-C++-*- 
// 
// Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
// 
// Xilinx, Inc. 
// 
// malloc.s
//
// prototype:	void* malloc(unsigned size) 		
// 
// procedure to allocate memory.
//
// Input:	Number of bytes in reg r5
// Return: Pointer to memory in reg r3: This will always be on a word
// boundary 0 if no free memory available
// ChangeLog  :
// 02/17/2004 : Changed code after Lsucc label to move swi out of the
//               delay slot
// 
// $Id: malloc.S,v 1.1 2005/10/18 00:09:05 vasanth Exp $
// 
///////////////////////////////////////////////////////////////////////////////

gcc2_compiled.:
__gnu_compiled_c:

	.globl _malloc_base_addr
	.sdata
	.align  2
_malloc_base_addr:		
	.data32 _heap
	.text
	.globl	malloc
	.text
	.align	2
	.ent	malloc
malloc:
	.frame	r1,0,r15		
	addi    r5,r5,3
	andni   r5,r5,3
	lwi	r3,r13,_malloc_base_addr
	add	r5,r3,r5 
	rsub	r18,rsp,r5 
	blei	r18,$Lsucc
	rtsd	r15, 8
	add	r3,r0,r0 

$Lsucc:
	swi	r5,r13,_malloc_base_addr 
	rtsd	r15, 8 
        nop
	.end	malloc
