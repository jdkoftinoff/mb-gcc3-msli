AR = mb-ar
RANLIB = mb-ranlib

# Check if we are compiling for hard-float. Choose which math library to compile based on that.
#MYFPU=$(findstring -mhard-float, ${EXTRA_CFLAGS})

## No longer do this. The mathfp library is not error free. The math library is IEEE compliant.
## It's ok to suffer performance loss to get compliant behavior.
# ifeq (${FPU},_fp)
# 	SUBLIBS = common/lib.a mathfp/lib.a 
# 	SUBDIRS = common mathfp
# else
	SUBLIBS = common/lib.a math/lib.a 
	SUBDIRS = common math
#endif

# Optimize for single precision
# ifeq (${DOUBLE},s)
# 	PRECISION = -fsingle-precision-constant
# else
# 	PRECISION = 
# endif
PRECISION = 
ALL_SUBDIRS = common mathfp math

MB_NEWLIB_SRCDIR = ../../../

ALL:
	for i in $(SUBDIRS); do \
	  cd $$i; \
	  make EXTRA_CFLAGS="${EXTRA_CFLAGS} $(PRECISION)" EXTRA_LFLAGS="${EXTRA_LFLAGS}" ; \
	  cd .. ; \
	 done
	make EXTRA_CFLAGS="${EXTRA_CFLAGS} $(PRECISION)" EXTRA_LFLAGS="${EXTRA_LFLAGS}" libm.a

libm.a: $(SUBLIBS)
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
	rm -f libm.a
	for i in $(ALL_SUBDIRS); do \
	  cd $$i; \
	  make clean; \
	  cd .. ; \
	 done

