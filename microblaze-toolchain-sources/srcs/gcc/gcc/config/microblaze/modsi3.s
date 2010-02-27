###################################-*-asm*- 
# 
# Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
# 
# Xilinx, Inc. CONFIDENTIAL 
# 
# modsi3.s 
# 
# modulo operation for 32 bit integers.
#	Input :	op1 in Reg r5
#		op2 in Reg r6
#	Output: op1 mod op2 in Reg r3
# 
# $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/gcc/src-3.4/gcc/config/microblaze/modsi3.s,v 1.1 2005/03/18 20:00:25 vasanth Exp $
# 
#######################################

	.globl	__modsi3
	.ent	__modsi3
__modsi3:
	.frame	r1,0,r15	

	addik	r1,r1,-16
	swi	r28,r1,0
	swi	r29,r1,4
	swi	r30,r1,8
	swi	r31,r1,12

	BEQI	r6,$LaDiv_By_Zero       # Div_by_Zero   # Division Error
	BEQI	r5,$LaResult_Is_Zero    # Result is Zero 
	BGEId	r5,$LaR5_Pos 
	ADD	r28,r5,r0               # Get the sign of the result [ Depends only on the first arg]
	RSUBI	r5,r5,0	                # Make r5 positive
$LaR5_Pos:
	BGEI	r6,$LaR6_Pos
	RSUBI	r6,r6,0	    # Make r6 positive
$LaR6_Pos:
	ADDIK	r3,r0,0      # Clear mod
	ADDIK	r30,r0,0     # clear div
	ADDIK	r29,r0,32    # Initialize the loop count
   # First part try to find the first '1' in the r5
$LaDIV1:
	ADD	r5,r5,r5         # left shift logical r5
	BGEID	r5,$LaDIV1       #
	ADDIK	r29,r29,-1
$LaDIV2:
	ADD	r5,r5,r5         # left shift logical  r5 get the '1' into the Carry
	ADDC	r3,r3,r3         # Move that bit into the Mod register
	rSUB	r31,r6,r3        # Try to subtract (r30 a r6)
	BLTi	r31,$LaMOD_TOO_SMALL
	OR	r3,r0,r31       # Move the r31 to mod since the result was positive
	ADDIK	r30,r30,1
$LaMOD_TOO_SMALL:
	ADDIK	r29,r29,-1
	BEQi	r29,$LaLOOP_END
	ADD	r30,r30,r30         # Shift in the '1' into div
	BRI	$LaDIV2          # Div2
$LaLOOP_END:
	BGEI	r28,$LaRETURN_HERE
	BRId	$LaRETURN_HERE
	rsubi	r3,r3,0 # Negate the result
$LaDiv_By_Zero:
$LaResult_Is_Zero:
	or	r3,r0,r0        # set result to 0 [Both mod as well as div are 0]
$LaRETURN_HERE:
# Restore values of CSRs and that of r3 and the divisor and the dividend
	lwi	r28,r1,0
	lwi	r29,r1,4
	lwi	r30,r1,8
	lwi	r31,r1,12
	rtsd	r15,8
	addik	r1,r1,16
        .end __modsi3
	
