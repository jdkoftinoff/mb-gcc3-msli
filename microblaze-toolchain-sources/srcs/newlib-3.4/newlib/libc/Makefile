AR = mb-ar
RANLIB = mb-ranlib
SUBLIBS = \
	stdlib/lib.a \
	ctype/lib.a \
	stdio/lib.a \
	string/lib.a \
	signal/lib.a \
	time/lib.a \
	locale/lib.a \
	reent/lib.a \
	errno/lib.a \
	misc/lib.a \
	posix/lib.a 

SUBDIRS = stdlib \
	ctype \
	stdio \
	string \
	signal \
	time \
	locale \
	reent \
	errno \
	misc \
	posix 

MB_NEWLIB_SRCDIR = ../../

ALL:
	for i in $(SUBDIRS); do \
	  cd $$i; \
	  make EXTRA_CFLAGS="${EXTRA_CFLAGS}" EXTRA_LFLAGS="${EXTRA_LFLAGS}"; \
	  cd .. ; \
	 done
	make libc.a EXTRA_CFLAGS="${EXTRA_CFLAGS}" EXTRA_LFLAGS="${EXTRA_LFLAGS}"

libc.a: $(SUBLIBS)
	rm -f $@
	rm -rf tmp
	mkdir tmp
	cd tmp; \
	 for i in $(SUBLIBS); do \
	  $(AR) x ../$$i; \
	 done; \
	$(AR) $(AR_FLAGS) r ../$@ *.o
	$(RANLIB) $@
	rm -rf tmp

clean: 
	rm -f libc.a
	for i in $(SUBDIRS); do \
	  cd $$i; \
	  make clean; \
	  cd .. ; \
	 done

