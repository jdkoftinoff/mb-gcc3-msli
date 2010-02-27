CSOURCES = sysfork.c syskill.c sysread.c sysunlink.c sysclose.c \
	sysfstat.c syslink.c syssbrk.c syswait.c sysexecve.c sysgetpid.c \
	syslseek.c sysstat.c syswrite.c sysfcntl.c sysgettod.c sysopen.c systimes.c
ASMSOURCES = 

CC = mb-gcc
AS = mb-as
MB_NEWLIB_SRCDIR = ../../../
INCLUDES = -I$(MB_NEWLIB_SRCDIR)/newlib/libc/include  
CFLAGS = -O3 $(INCLUDES) -DMISSING_SYSCALL_NAMES  ${EXTRA_CFLAGS}
LFLAGS = ${EXTRA_LFLAGS}
OBJS = $(CSOURCES:.c=.o) $(ASMSOURCES:.s=.o)

lib.a : $(OBJS)
	mb-ar r lib.a $?

clean:
	rm -f $(OBJS) lib.a
