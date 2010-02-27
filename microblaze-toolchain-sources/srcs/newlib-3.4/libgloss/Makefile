CSOURCES = open.c close.c read.c write.c \
	print.c putnum.c sbrk.c stat.c \
	fstat.c getpid.c isatty.c kill.c lseek.c unlink.c
ASMSOURCES = 

CC = mb-gcc
AS = mb-as
AR = mb-ar
RANLIB = mb-ranlib
MB_NEWLIB_SRCDIR = ../
INCLUDES = -I$(MB_NEWLIB_SRCDIR)/newlib/libc/include
CFLAGS = -O3 $(INCLUDES) ${EXTRA_CFLAGS}
LFLAGS = ${EXTRA_LFLAGS}
OBJS = $(CSOURCES:.c=.o) $(ASMSOURCES:.s=.o)

libglossa :
	echo "Makefile:libgloss:libglossa" `pwd`
	make EXTRA_CFLAGS="${EXTRA_CFLAGS}" EXTRA_LFLAGS="${EXTRA_LFLAGS}" MICROBLAZE_target
	make EXTRA_CFLAGS="${EXTRA_CFLAGS}" EXTRA_LFLAGS="${EXTRA_LFLAGS}" liba
	rm -rf tmp ; \
	mkdir tmp ; \
	cd tmp ; \
	$(AR) x ../lib.a ; \
	$(AR) x ../microblaze/lib.a ; \
	$(AR) r ../libgloss.a *.o ; 
	rm -rf tmp
	cp microblaze/crt0.o microblaze/crt1.o microblaze/crt2.o microblaze/crt3.o microblaze/crt4.o microblaze/crtinit.o microblaze/pgcrtinit.o microblaze/sim-pgcrtinit.o microblaze/sim-crtinit.o . 
	cp libgloss.a ../
MICROBLAZE_target:
	cd microblaze; \
	make EXTRA_CFLAGS="${EXTRA_CFLAGS}" EXTRA_LFLAGS="${EXTRA_LFLAGS}" ;

liba : $(OBJS)
	mb-ar r lib.a $? 

clean:
	rm -f $(OBJS) lib.a libgloss.a
	cd microblaze; \
	make EXTRA_CFLAGS="${EXTRA_CFLAGS}" EXTRA_LFLAGS="${EXTRA_LFLAGS}" clean ;

