# Makefile for floatlib
# $Id: Makefile,v 1.1 2005/03/18 20:00:31 vasanth Exp $

# [02/15/03] Removed the fix functions 
# fixdfdi
# fixunsdfsi
# fixunsdfdi

CSOURCES = adddf3.c floatsidf.c ltdf2.c subdf3.c addsf3.c \
	extendsfdf2.c floatsisf.c ltsf2.c subsf3.c cmpdf2.c gedf2.c \
	muldf3.c cmpsf2.c fixdfsi.c gesf2.c mulsf3.c \
	truncdfsf2.c divdf3.c gtdf2.c nedf2.c divsf3.c \
	gtsf2.c negdf2.c floatdidf.c ledf2.c negsf2.c \
	floatdisf.c lesf2.c nesf2.c fixsfsi.c
ASMSOURCES = eqdf2.s eqsf2.s
CC = mb-gcc
AS = mb-as
MB_NEWLIB_SRCDIR = ../../../
INCLUDES = -I.
CFLAGS = $(INCLUDES) -g -O2
OBJS = $(CSOURCES:.c=.o) $(ASMSOURCES:.s=.o) 

lib.a : $(OBJS)
	mb-ar r lib.a $?
.c.o:
	$(CC) ${EXTRA_CFLAGS} $(CFLAGS) ${EXTRA_LFLAGS} -c $<

.s.o:
	$(AS) $< -o $@

clean:
	rm -f $(OBJS) lib.a

