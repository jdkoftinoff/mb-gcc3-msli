###################################-*-asm*- 
# 
# Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
# 
# Xilinx, Inc. CONFIDENTIAL 
# 
# eqdf2.s 
# 
# Compare two Double precision floating point numbers
#       Input : Op1-High in Reg r5
#		Op1-Low  in Reg r6
#		Op2-High in Reg r7
#		Op2-Low  in Reg r8
#       Output: Return 0 in Reg r3 and r4 if equal
#	Basic operation : Op2-Op1	
# 
# $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/newlib-3.4/libgloss/microblaze/floatlib/eqdf2.s,v 1.1 2005/03/18 20:00:31 vasanth Exp $
# 
#######################################

gcc2_compiled.:
__gnu_compiled_c:
	.text
	.align	2
	.globl	__eqdf2
	.ent	__eqdf2
__eqdf2:
	rsub	r3,r6,r8
	rtsd	r15,8 # MICROBLAZE rtsd2
	rsub	r4,r7,r9	
	.end	__eqdf2
