#!/bin/tcsh -evf
##################################################################
#
# build_gcc
#
# Builds MicroBlaze Binutils/GCC/newlib in that order. 
# $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/gnu_scripts/mb/build_gcc.csh,v 1.1 2005/10/01 01:17:09 vasanth Exp $
#
###################################################################
echo "GNU_ENV is set to $GNU_ENV"
echo "PLATFORM is set to $PLATFORM"

setenv CURDIR  $PWD
echo "----------------------------------------------------------------"
echo "Building Binutils/GCC/Newlib"
echo "Make is" `which make`

## RELEASEDIR
setenv RELDIR ${GNU_ENV}/release/${PLATFORM}/microblaze/
echo PATH is $PATH
echo "Build Microblaze binutils, GCC, newlib, uclinux tools in order"
./do_all_binutils.csh 
./do_all_gcc.csh
./do_all_newlib.csh 
#./do_all_uclinux.csh       - Cleanup script first.
echo "Build Microblaze binutils, GCC, newlib, uclinux tools in order -- Done."
echo "Microblaze GNU Done"


