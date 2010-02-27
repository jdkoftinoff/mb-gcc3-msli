#!/bin/tcsh -evf
##################################################################
#
# build_gcc
#
# Builds MicroBlaze Binutils/GCC/newlib in that order. 
# $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/gnu_scripts/mb/build_gdb.csh,v 1.1 2005/10/01 01:17:09 vasanth Exp $
#
###################################################################
echo "GNU_ENV is set to $GNU_ENV"
echo "PLATFORM is set to $PLATFORM"

setenv CURDIR  $PWD
echo "----------------------------------------------------------------"
echo "Building GDB"
echo "Make is" `which make`

## RELEASEDIR
setenv RELDIR ${GNU_ENV}/release/${PLATFORM}/microblaze/
echo PATH is $PATH
./do_all_gdb.csh 
echo "Microblaze GDB Done"


