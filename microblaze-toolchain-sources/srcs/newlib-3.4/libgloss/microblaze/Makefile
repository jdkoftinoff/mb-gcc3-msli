CSOURCES = timer.c xil_printf.c realloc.c 
C_ASMSOURCES = malloc.S free.S 
ASMSOURCES = strcmp.s exit.s strcpy.s 
# memcpy.s memset.s -- Removed. Let's use the default newlib ones. They are faster.

CC = mb-gcc
AS = mb-as
MB_NEWLIB_SRCDIR = ../../
INCLUDES = -I$(MB_NEWLIB_SRCDIR)/newlib/libc/include
CFLAGS = -O3 $(INCLUDES) ${EXTRA_CFLAGS}
LFLAGS = ${EXTRA_LFLAGS}
OBJS = $(CSOURCES:.c=.o) $(C_ASMSOURCES:.S=.o) $(ASMSOURCES:.s=.o)

ALL : liba crt0.o crt1.o crt2.o crt3.o crt4.o crtinit.o pgcrtinit.o sim-crtinit.o sim-pgcrtinit.o

liba : $(OBJS) 
	mb-ar r lib.a $? 

crt0.o : crt0.s
	$(CC) -c crt0.s

crt1.o : crt1.s
	$(CC) -c crt1.s

crt2.o : crt2.s
	$(CC) -c crt2.s

crt3.o : crt3.s
	$(CC) -c crt3.s

crt4.o : crt4.s
	$(CC) -c crt4.s

crtinit.o : crtinit.s
	$(CC) -c crtinit.s

sim-crtinit.o : sim-crtinit.s
	$(CC) -c sim-crtinit.s

pgcrtinit.o : pgcrtinit.s
	$(CC) -c pgcrtinit.s

sim-pgcrtinit.o : sim-pgcrtinit.s
	$(CC) -c sim-pgcrtinit.s

clean:
	rm -f $(OBJS) lib.a *.o
