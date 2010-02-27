# Generated automatically from Makefile.in:Make-rules by configure.
# Plugin definitions
PLUGIN 			= rhabout
PLUGIN_OBJS 		= rhabout.$(OBJEXT)
PLUGIN_IMAGE_BASE 	= 0x65000000
PLUGIN_INSTALL_SRCDIR	= rhabout.itcl
# Comment out this to install the example plugin
PLUGIN_INSTALL_TARGET   = nothing

nothing:
# Variables to be defined by plugin's Makefile.in:
#
# PLUGIN (required)
# PLUGIN_CFLAGS - CFLAGS to use for build
# PLUGIN_SHLIB_LIBS - shared libs to link against
# PLUGIN_INSTALL - anything to install from builddir
# PLUGIN_INSTALL_SRCDIR - anything to install from srcdir
# PLUGIN_INSTALL_TARGET - install target (overrides default)
# PLUGIN_TARGETS - targets to build
# PLUGIN_CLEAN - clean targets (overrides default)
# PLUGIN_DISTCLEAN - distclean targets (overrides default)
#
# Cygwin-specific variables
# PLUGIN_DLL_ENTRY - cygwin DLL entry point default _DllMain@12)
# PLUGIN_IMAGE_BASE (required) - DLL image base
# PLUGIN_CYGWIN_LIBS - any additional cygwin libs to link against
# PLUGIN_OBJS - objects to put in DLL (hack)

prefix = /usr/local
exec_prefix = ${prefix}

srcdir = .
datadir = ${prefix}/share
libdir = ${exec_prefix}/lib
mandir = ${prefix}/man
plugin_srcdir = ..
plugin_builddir = /sourceware/snapshot-tmp/gdbadmin-tmp/gdb_5_3-branch/5.3/insight/src/gdb/gdbtk/plugins

SHELL = /bin/sh
CC = gcc
CFLAGS = -g -O2

OBJEXT = o
TCL_SHLIB_CFLAGS = -fPIC
TCL_SHLIB_LD = gcc -pipe -shared
TCL_DBGX = 
TCL_SHLIB_SUFFIX = .so
TCL_INCLUDES = -I/sourceware/snapshot-tmp/gdbadmin-tmp/gdb_5_3-branch/5.3/insight/src/tcl/generic
TCL_BUILD_LIB_SPEC = -L/sourceware/snapshot-tmp/gdbadmin-tmp/gdb_5_3-branch/5.3/insight/src/tcl/unix -ltcl8.3${TCL_DBGX}

mkinstalldirs = $(srcdir)/../../../../mkinstalldirs
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644

PLUGIN_MODULE = $(PLUGIN)$(TCL_SHLIB_SUFFIX)

# Must include a path to tclWinInt.h on cygwin
ifeq ($(TCL_SHLIB_SUFFIX),.dll)
CYGWIN_CFLAGS=-I$(srcdir)/../../../../tcl/win
else
CYGWIN_CFLAGS=
endif
GENERIC_CFLAGS = $(CFLAGS) $(TCL_SHLIB_CFLAGS) $(TCL_INCLUDES) $(CYGWIN_CFLAGS)
ALL_CFLAGS = $(GENERIC_CFLAGS) $(PLUGIN_CFLAGS)

# It would be nice to copy all the plugin's files to the build dir so
# that we can use the plugin without installing it...
#GENERIC_TARGETS = tclIndex pkgIndex.tcl
ifeq ($(PLUGIN_OBJS),)
ALL_TARGETS = $(GENERIC_TARGETS) $(PLUGIN_TARGETS)
else
ALL_TARGETS = $(PLUGIN_MODULE) $(GENERIC_TARGETS) $(PLUGIN_TARGETS)
endif

GENERIC_SHLIB_LIBS = $(TCL_BUILD_LIB_SPEC)
CYGWIN_DLL_LIBS = $(PLUGIN_CYGWIN_LIBS) 
ALL_SHLIB_LIBS = $(GENERIC_SHLIB_LIBS) $(PLUGIN_SHLIB_LIBS)

GENERIC_INSTALL = $(PLUGIN).tcl
ifeq ($(PLUGIN_OBJS),)
ALL_INSTALL = $(GENERIC_INSTALL) $(PLUGIN_INSTALL)
else
ALL_INSTALL = $(PLUGIN_MODULE) $(GENERIC_INSTALL) $(PLUGIN_INSTALL)
endif

ifeq ($(PLUGIN_INSTALL_TARGET),)
INSTALL_TARGET = install-generic
else
INSTALL_TARGET = $(PLUGIN_INSTALL_TARGET)
endif

ifeq ($(PLUGIN_CLEAN),)
CLEAN_TARGET = clean-generic
else
CLEAN_TARGET = $(PLUGIN_CLEAN)
endif

ifeq ($(PLUGIN_DISTCLEAN),)
DISTCLEAN_TARGET = distclean-generic
else
DISTCLEAN_TARGET = $(PLUGIN_DISTCLEAN)
endif

# Makefile dependencies
Makefile: $(plugin_srcdir)/Make-rules $(plugin_builddir)/config.status
	cd $(plugin_builddir) \
	  && CONFIG_FILES=$(PLUGIN)/Makefile:$(PLUGIN)/Makefile.in:Make-rules CONFIG_HEADERS= $(SHELL) ./config.status

all: $(ALL_TARGETS)

%.$(OBJEXT): $(srcdir)/%.c
	$(CC) -c $(ALL_CFLAGS) $<

install: $(INSTALL_TARGET)

install-generic:
	$(mkinstalldirs) $(libdir)/insight1.0/$(PLUGIN)
	for i in $(ALL_INSTALL); do \
            $(INSTALL_DATA) $$i $(libdir)/insight1.0/$(PLUGIN)/$$i ; \
        done ;
	for i in pkgIndex.tcl tclIndex $(PLUGIN_INSTALL_SRCDIR) ; do \
	    $(INSTALL_DATA) $(srcdir)/$$i $(libdir)/insight1.0/$(PLUGIN)/$$i ; \
	done ;

info:
install-info:
clean: $(CLEAN_TARGET)

clean-generic:
	rm -f *.$(OBJEXT) *.base *.def *.exp $(PLUGIN_MODULE) $(PLUGIN_CLEAN)

distclean-generic: $(CLEAN_TARGET)

distclean: $(DISTCLEAN_TARGET)

#
# Build rules for the DLL/shlib
#

ifeq ($(TCL_SHLIB_SUFFIX),.dll)

# cygwin stuff
DLLTOOL = 
LD = 
AS = 
NM = 

PLUGIN_DEFS = $(PLUGIN).def
PLUGIN_BASE = $(PLUGIN).base
PLUGIN_EXP = $(PLUGIN).exp
ifeq ($(PLUGIN_DLL_ENTRY),)
PLUGIN_DLL_ENTRY = _DllMain@12
endif

$(PLUGIN_DEFS): $(PLUGIN_OBJS)
	echo 'EXPORTS' > tmp.def
	-for o in $(PLUGIN_OBJS) ; do \
	  $(NM) --extern-only --defined-only $$o | sed -e 's/[^ ]* [^ ]* //' -e 's/^_//' | fgrep -v DllMain | fgrep -v impure_ptr >> tmp.def ; \
	done
	mv tmp.def $(PLUGIN_DEFS)

$(PLUGIN_MODULE): $(PLUGIN_OBJS) $(PLUGIN_DEFS)
	$(LD) -s --base-file $(PLUGIN_BASE) -dll -o $(PLUGIN_MODULE) $(PLUGIN_OBJS) $(ALL_SHLIB_LIBS) $(CYGWIN_DLL_LIBS) -e $(PLUGIN_DLL_ENTRY) --image-base $(PLUGIN_IMAGE_BASE)
	$(DLLTOOL) --as=$(AS) --dllname $(PLUGIN_MODULE) --def $(PLUGIN_DEFS) --base-file $(PLUGIN_DEFS) --output-exp $(PLUGIN_EXP)
	$(LD) -s --base-file $(PLUGIN_BASE) $(PLUGIN_EXP) -dll -o $(PLUGIN_MODULE) $(PLUGIN_OBJS) $(ALL_SHLIB_LIBS) $(CYGWIN_DLL_LIBS) -e $(PLUGIN_DLL_ENTRY) --image-base $(PLUGIN_IMAGE_BASE)
	$(DLLTOOL) --as=$(AS) --dllname $(PLUGIN_MODULE) --def $(PLUGIN_DEFS) --base-file $(PLUGIN_BASE) --output-exp $(PLUGIN_EXP)
	$(LD) $(PLUGIN_EXP) -dll -o $(PLUGIN_MODULE) $(PLUGIN_OBJS) $(ALL_SHLIB_LIBS) $(CYGWIN_DLL_LIBS) -e $(PLUGIN_DLL_ENTRY) --image-base $(PLUGIN_IMAGE_BASE)

else

# For unixen only
$(PLUGIN_MODULE): $(PLUGIN_OBJS)
	$(TCL_SHLIB_LD) -o $(PLUGIN_MODULE) $(PLUGIN_OBJS) $(ALL_SHLIB_LIBS)

endif
