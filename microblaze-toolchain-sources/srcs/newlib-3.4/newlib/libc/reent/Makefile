CSOURCES = impure.c timer.c reent.c sbrkr.c fstatr.c closer.c fstatr.c lseekr.c readr.c signalr.c unlinkr.c impure.c reent.c statr.c writer.c execr.c linkr.c openr.c sbrkr.c timer.c

ASMSOURCES = 

CC = mb-gcc
AS = mb-as
MB_NEWLIB_SRCDIR = ../../../
INCLUDES = -I$(MB_NEWLIB_SRCDIR)/newlib/libc/include  
CFLAGS = -O3 $(INCLUDES) -DMISSING_SYSCALL_NAMES ${EXTRA_CFLAGS}
LFLAGS = ${EXTRA_LFLAGS}
OBJS = $(CSOURCES:.c=.o) $(ASMSOURCES:.s=.o)

lib.a : $(OBJS)
	mb-ar r lib.a $?

clean:
	rm -f $(OBJS) lib.a

