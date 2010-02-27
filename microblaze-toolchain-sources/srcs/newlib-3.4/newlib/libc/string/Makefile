CSOURCES = bcmp.c memcpy.c strchr.c strlen.c strpbrk.c strupr.c bcopy.c \
	memmove.c strcmp.c strlwr.c strrchr.c strxfrm.c bzero.c memset.c \
	strcoll.c strncasecmp.c strspn.c swab.c index.c rindex.c strcpy.c \
	strncat.c strstr.c u_strerr.c memchr.c strcasecmp.c strcspn.c \
	strncmp.c strtok.c memcmp.c strcat.c strerror.c strncpy.c strtok_r.c
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

