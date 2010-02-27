###################################-*-asm*- 
# 
# Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
# 
# Xilinx, Inc.  
# 
# strcpy.s 
#
#      char* strcpy (char *dst, const char * src)
#
# Copy string from source to destination
#	Input :	Destination address in Reg r5
#		Source string address in Reg r6
#	Return: Destination address in Reg r3
# 
# $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/newlib-3.4/libgloss/microblaze/strcpy.s,v 1.2 2005/10/19 18:46:41 vasanth Exp $
# 
#######################################

gcc2_compiled.:
__gnu_compiled_c:
	.text
	.align	2
	.globl	strcpy
	.ent	strcpy
	strcpy:
	.frame r1,0,r15
	add	r3,r0,r5
	addik 	r10,r0,0
	lbu   	r18,r6,r10
	.LL170:
	sb  	r18,r5,r10
	addik 	r10,r10,1
	bneid 	r18,.LL170
	lbu   	r18,r6,r10
	rtsd 	r15,8
	nop
	.end strcpy
	.align	2
