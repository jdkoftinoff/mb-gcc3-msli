# CSOURCES = s_cbrt.c s_infinity.c s_modf.c sf_cbrt.c sf_infinity.c \
# 	sf_nextafter.c s_copysign.c s_lib_ver.c s_nan.c sf_copysign.c \
# 	sf_log1p.c  s_expm1.c s_log1p.c s_nextafter.c  \
# 	sf_logb.c sf_scalbn.c s_finite.c s_logb.c  sf_finite.c \
# 	sf_modf.c s_ilogb.c s_matherr.c s_scalbn.c sf_ilogb.c sf_nan.c \

## EDK 7.1 - Compiling everything. 
CSOURCES = s_cbrt.c s_copysign.c s_expm1.c sf_cbrt.c sf_copysign.c sf_expm1.c sf_finite.c sf_ilogb.c sf_infinity.c \
	   s_finite.c sf_log1p.c sf_logb.c sf_modf.c sf_nan.c sf_nextafter.c sf_rint.c  sf_scalbn.c s_ilogb.c \
	   s_infinity.c s_lib_ver.c s_log1p.c s_logb.c s_matherr.c s_modf.c s_nan.c s_nextafter.c s_rint.c s_scalbn.c


ASMSOURCES = 

CC = mb-gcc
AS = mb-as
MB_NEWLIB_SRCDIR = ../../../
INCLUDES = -I$(MB_NEWLIB_SRCDIR)/newlib/libc/include  
CFLAGS = $(INCLUDES)  ${EXTRA_CFLAGS}
LFLAGS = ${EXTRA_LFLAGS}
OBJS = $(CSOURCES:.c=.o) $(ASMSOURCES:.s=.o)

lib.a : $(OBJS)
	mb-ar r lib.a $?

clean:
	rm -f $(OBJS) lib.a

