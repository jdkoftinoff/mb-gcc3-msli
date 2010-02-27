CSOURCES =_tolower.c _toupper.c ctype_.c isalnum.c isalpha.c \
	isascii.c iscntrl.c isdigit.c islower.c isprint.c ispunct.c \
	isspace.c isupper.c isxdigit.c toascii.c tolower.c toupper.c
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

