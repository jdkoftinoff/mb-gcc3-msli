RANLIB = mb-ranlib
AR  = mb-ar 

# Use these extra options for building newlib libc.a with different options
EXTRA_CFLAGS=
EXTRA_LFLAGS=

combined_t: libc_t libm_t

libc_t: LIBGLOSS_target
	cd newlib ; \
	make EXTRA_CFLAGS="${EXTRA_CFLAGS}" EXTRA_LFLAGS="${EXTRA_LFLAGS}" libc_target; 
	rm -f libc${MULT}${SHIFT}${ECMP}.a ; \
	rm -rf tmp ; \
	mkdir tmp ; \
	cd tmp ; \
	$(AR) x ../newlib/libc/libc.a ; \
	$(AR) x ../libgloss.a ; \
	$(AR) $(AR_FLAGS) r ../libc${MULT}${SHIFT}${ECMP}.a *.o ; 
	$(RANLIB) libc${MULT}${SHIFT}${ECMP}.a ;
	rm -rf tmp

libm_t: 
	cd newlib ; \
	make EXTRA_CFLAGS="${EXTRA_CFLAGS}" EXTRA_LFLAGS="${EXTRA_LFLAGS}" libm_target; 
	rm -f libm${MULT}${SHIFT}${ECMP}${FPU}${DOUBLE}.a ; \
	rm -rf tmp ; \
	mkdir tmp ; \
	cd tmp ; \
	$(AR) x ../newlib/libm/libm.a ; \
	$(AR) $(AR_FLAGS) r ../libm${MULT}${SHIFT}${ECMP}${FPU}${DOUBLE}.a *.o ; 
	$(RANLIB) libm${MULT}${SHIFT}${ECMP}${FPU}${DOUBLE}.a ;
	rm -rf tmp


LIBGLOSS_target: 
	cd libgloss ; \
	make EXTRA_CFLAGS="${EXTRA_CFLAGS}" EXTRA_LFLAGS="${EXTRA_LFLAGS}";

stringop:
	cd stringops ; \
	make  ;
clean: 
	rm -f libc.a libm.a
	cd newlib ;\
	make clean ;
	cd libgloss ; \
	make clean; 
	cd stringops ;\
	make clean 
