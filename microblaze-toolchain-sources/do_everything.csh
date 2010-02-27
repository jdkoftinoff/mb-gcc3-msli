#!/bin/tcsh -evf
##################################################################
#
# Do everything
#
# Builds entire mb-uclinux toolchain
# $Header: $
#
###################################################################

setenv GNU_ENV `pwd`

# Force building for linux, only supported platform
if ( `uname -m`  == "x86_64" ) then
setenv PLATFORM lin64
else
setenv PLATFORM lin
endif

./build_gcc.csh
./build_gdb.csh
./do_all_uclinux.csh

