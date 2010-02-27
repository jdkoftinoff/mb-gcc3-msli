AR = mb-ar

ALL : libm_target libc_target

libc_target:
	cd libc ; \
	make EXTRA_CFLAGS="${EXTRA_CFLAGS}" EXTRA_LFLAGS="${EXTRA_LFLAGS}"; 

libm_target:
	cd libm ; \
	make EXTRA_CFLAGS="${EXTRA_CFLAGS}" EXTRA_LFLAGS="${EXTRA_LFLAGS}"; 
clean:
	cd libc ; \
	make clean ; \
	cd .. ;
	cd libm ; \
	make clean ; \
	cd .. ;
