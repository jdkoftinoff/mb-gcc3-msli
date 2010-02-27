###################################-*-asm*- 
# 
# Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
# 
# Xilinx, Inc. CONFIDENTIAL 
# 
# eqsf2.s 
# 
# Compare two single precision floating point numbers
#       Input : Op1 in Reg r5
#		Op2 in Reg r6
#       Output: Return 0 in Reg r3 if equal
#	Basic operation : Op2-Op1	
# 
# $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/newlib-3.4/libgloss/microblaze/floatlib/eqsf2.s,v 1.1 2005/03/18 20:00:31 vasanth Exp $
# 
#######################################

gcc2_compiled.:
__gnu_compiled_c:
	.text
	.align	2
	.globl	__eqsf2
	.ent	__eqsf2
__eqsf2:
	rtsd	r15,8 # MICROBLAZE rtsd2
	rsub	r3,r5,r6
.end	__eqsf2
