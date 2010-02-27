# tixConfig.sh --
# 
# This shell script (for sh) is generated automatically by Tix's
# configure script.  It will create shell variables for most of
# the configuration options discovered by the configure script.
# This script is intended to be included by the configure scripts
# for Tcl extensions so that they don't have to figure this all
# out for themselves.
#
# The information in this file is specific to a single platform.
#
# SCCS: @(#) tclConfig.sh.in 1.20 97/07/01 11:40:19

# String to pass to linker to pick up the Tcl library from its
# build directory.
TIX_BUILD_LIB_SPEC='-L/sourceware/snapshot-tmp/gdbadmin-tmp/gdb_5_3-branch/5.3/insight/src/tix/unix/tk8.3 -ltix4.1.8.1'

# The name of the tix library.  Used for dependencies...
TIX_LIB_FILE='libtix4.1.8.1.a' 

# The directory where tix was build.
TIX_BUILD_LOCATION='/sourceware/snapshot-tmp/gdbadmin-tmp/gdb_5_3-branch/5.3/insight/src/tix/unix/tk8.3'

# The full pathname to the tix library.
TIX_LIB_FULL_PATH='/sourceware/snapshot-tmp/gdbadmin-tmp/gdb_5_3-branch/5.3/insight/src/tix/unix/tk8.3/libtix4.1.8.1.a'
