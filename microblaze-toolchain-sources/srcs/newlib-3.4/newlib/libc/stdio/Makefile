CSOURCES = clearerr.c fiprintf.c ftell.c perror.c scanf.c vfprintf.c \
	fclose.c flags.c fvwrite.c printf.c setbuf.c vfscanf.c \
	fdopen.c fopen.c fwalk.c putc.c setvbuf.c vprintf.c feof.c \
	fprintf.c fwrite.c putchar.c siprintf.c vsnprintf.c ferror.c \
	fputc.c getc.c puts.c snprintf.c vsprintf.c fflush.c fputs.c \
	getchar.c putw.c sprintf.c wbuf.c fgetc.c fread.c gets.c \
	refill.c sscanf.c wsetup.c fgetpos.c freopen.c getw.c remove.c \
	stdio.c fgets.c fscanf.c iprintf.c rename.c tmpfile.c fileno.c \
	fseek.c makebuf.c rewind.c tmpnam.c findfp.c fsetpos.c \
	mktemp.c rget.c ungetc.c
ASMSOURCES = 

CC = mb-gcc
AS = mb-as
MB_NEWLIB_SRCDIR = ../../../
INCLUDES = -I$(MB_NEWLIB_SRCDIR)/newlib/libc/include  
CFLAGS = -O3 $(INCLUDES) -DMISSING_SYSCALL_NAMES -DMICROBLAZE_NEWLIB ${EXTRA_CFLAGS}
LFLAGS = ${EXTRA_LFLAGS}
OBJS = $(CSOURCES:.c=.o) $(ASMSOURCES:.s=.o) vfiprintf.o

lib.a : $(OBJS)
	mb-ar r lib.a $?

vfiprintf.o : vfprintf.c
	$(CC) $(CFLAGS) -DINTEGER_ONLY -c vfprintf.c -o vfiprintf.o ${LFLAGS}

clean:
	rm -f $(OBJS) lib.a

