CSOURCES = execl.c execv.c isatty.c readdir.c seekdir.c closedir.c \
	execle.c execve.c opendir.c rewinddir.c telldir.c creat.c execlp.c \
	execvp.c popen.c 
ASMSOURCES =

CC = mb-gcc
AS = mb-as
MB_NEWLIB_SRCDIR = ../../../
INCLUDES = -I$(MB_NEWLIB_SRCDIR)/newlib/libc/include  
CFLAGS = -O3 $(INCLUDES) -DMISSING_SYSCALL_NAMES -DHAVE_OPENDIR ${EXTRA_CFLAGS}
LFLAGS = ${EXTRA_LFLAGS}
OBJS = $(CSOURCES:.c=.o) $(ASMSOURCES:.s=.o)

lib.a : $(OBJS)
	mb-ar r lib.a $?

clean:
	rm -f $(OBJS) lib.a

