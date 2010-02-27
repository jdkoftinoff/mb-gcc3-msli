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
setenv PLATFORM lin

./build_gcc.csh
./build_gdb.csh
./do_all_uclinux.csh

