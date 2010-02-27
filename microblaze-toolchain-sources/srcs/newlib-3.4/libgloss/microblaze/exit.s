###################################-*-asm*- 
# 
# Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
# 
# Xilinx, Inc.  
# 
# exit.s 
# 
# exit from the program. Currently takes in no arguments and doesnt return.
# 
# $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/newlib-3.4/libgloss/microblaze/exit.s,v 1.2 2005/10/19 18:46:41 vasanth Exp $
# 
#######################################

	.globl exit
	.text
	.align 2
	.ent exit
exit:
	bri	exit
	.end exit
