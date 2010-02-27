CSOURCES = atol.c exit.c mblen_r.c putenv_r.c strtoul.c __adjust.c \
	bsearch.c getenv.c mbstowcs.c qsort.c system.c __exp10.c calloc.c \
	getenv_r.c mbstowcs_r.c rand.c valloc.c __ten_mu.c div.c getopt.c \
	mbtowc.c rand_r.c wcstombs.c abort.c labs.c mbtowc_r.c \
	realloc.c wcstombs_r.c abs.c dtoastub.c ldiv.c mlock.c setenv.c \
	wctomb.c assert.c ecvtbuf.c ldtoa.c setenv_r.c wctomb_r.c \
	atexit.c efgcvt.c malign.c msize.c strdup.c atof.c environ.c malloc.c \
	mstats.c strdup_r.c atoff.c envlock.c mallocr.c mtrim.c strtod.c \
	atoi.c eprintf.c mblen.c putenv.c strtol.c dtoa.c mprec.c

# NOT COMPILING dtoa.c
ASMSOURCES = 

CC = mb-gcc
AS = mb-as
MB_NEWLIB_SRCDIR = ../../../
INCLUDES = -I$(MB_NEWLIB_SRCDIR)/newlib/libc/include  
CFLAGS = -O3 $(INCLUDES) -DMISSING_SYSCALL_NAMES -DMALLOC_PROVIDED ${EXTRA_CFLAGS}
LFLAGS = ${EXTRA_LFLAGS}
#CFLAGS = -O3 $(INCLUDES) -DMISSING_SYSCALL_NAMES -DMALLOC_PROVIDED
EXTRA_OBJS = mallocr.o freer.o reallocr.o callocr.o cfreer.o malignr.o \
        vallocr.o pvallocr.o mallinfor.o mallstatsr.o msizer.o malloptr.o 
OBJS = $(CSOURCES:.c=.o) $(ASMSOURCES:.s=.o)
MALLOC_COMPILE = $(CC) -O3 $(INCLUDES) -DMISSING_SYSCALL_NAMES -DINTERNAL_NEWLIB ${EXTRA_CFLAGS} ${LFLAGS}

lib.a : $(OBJS) $(EXTRA_OBJS)
	mb-ar r lib.a $?

mallocr.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_MALLOC -c mallocr.c -o $@

freer.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_FREE  -c mallocr.c -o $@

reallocr.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_REALLOC -c mallocr.c -o $@

callocr.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_CALLOC -c mallocr.c -o $@

cfreer.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_CFREE -c mallocr.c -o $@

malignr.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_MEMALIGN -c mallocr.c -o $@

vallocr.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_VALLOC -c mallocr.c -o $@

pvallocr.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_PVALLOC -c mallocr.c -o $@

mallinfor.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_MALLINFO -c mallocr.c -o $@

mallstatsr.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_MALLOC_STATS -c mallocr.c -o $@

msizer.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_MALLOC_USABLE_SIZE -c mallocr.c -o $@

malloptr.o: mallocr.c
	$(MALLOC_COMPILE) -DDEFINE_MALLOPT -c mallocr.c -o $@

clean:
	rm -f $(OBJS) $(EXTRA_OBJS) lib.a

