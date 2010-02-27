# CSOURCES = s_acos.c s_ldexp.c sf_atan.c sf_pow.c s_asin.c s_log.c \
# 	sf_atan2.c sf_signif.c e_acosh.c s_asine.c s_log10.c sf_atangent.c \
# 	sf_sin.c e_atanh.c s_asinh.c s_logarithm.c sf_ceil.c sf_sine.c \
# 	e_hypot.c s_atan.c s_mathcnst.c sf_cos.c sf_sineh.c e_j0.c s_atan2.c \
# 	s_numtest.c sf_cosh.c sf_sinh.c e_j1.c s_atangent.c s_pow.c sf_erf.c \
# 	sf_sqrt.c e_remainder.c s_ceil.c s_signif.c sf_exp.c sf_tan.c \
# 	e_scalb.c s_cos.c s_sin.c sf_fabs.c sf_tanh.c ef_acosh.c s_cosh.c \
# 	s_sine.c sf_floor.c w_cabs.c ef_atanh.c s_erf.c s_sineh.c sf_fmod.c \
# 	w_drem.c ef_hypot.c s_exp.c s_sinf.c sf_frexp.c w_jn.c ef_j0.c \
# 	s_fabs.c s_sinh.c sf_isinf.c wf_cabs.c ef_j1.c s_floor.c s_sqrt.c \
# 	sf_isnan.c wf_drem.c ef_remainder.c s_fmod.c s_tan.c sf_ispos.c \
# 	wf_jn.c ef_scalb.c s_frexp.c s_tanh.c sf_ldexp.c er_gamma.c \
# 	s_infconst.c sf_acos.c sf_log.c er_lgamma.c s_isinf.c sf_asin.c \
# 	sf_log10.c erf_gamma.c s_isnan.c sf_asine.c sf_logarithm.c \
# 	erf_lgamma.c s_ispos.c sf_asinh.c sf_numtest.c

CSOURCES = s_acos.c       sf_ceil.c       s_frexp.c      s_signif.c \
	   e_acosh.c	s_asin.c       sf_cos.c        sf_signif.c    s_sin.c \
	   e_atanh.c	s_asine.c      sf_cosh.c       sf_sin.c       s_sine.c \
	   ef_acosh.c	s_asinh.c      sf_erf.c        sf_sine.c      s_sineh.c \
	   ef_atanh.c	s_atan2.c      sf_exp.c        sf_sineh.c     s_sinf.c \
	   ef_hypot.c	s_atan.c       sf_fabs.c       sf_sinh.c      s_sinh.c \
	   ef_j0.c		s_atangent.c   sf_floor.c      sf_sqrt.c      s_sqrt.c \
	   ef_j1.c		s_ceil.c       sf_fmod.c       sf_tan.c       s_tan.c \
	   ef_remainder.c	s_cos.c        sf_frexp.c      sf_tanh.c      s_tanh.c \
	   ef_scalb.c	s_cosh.c       sf_isinf.c      s_infconst.c   w_cabs.c \
	   e_hypot.c	s_erf.c        sf_isnan.c      s_isinf.c      w_drem.c \
	   e_j0.c		s_exp.c        sf_ispos.c      s_isnan.c      wf_cabs.c \
	   e_j1.c		s_fabs.c       sf_ldexp.c      s_ispos.c      wf_drem.c \
	   e_remainder.c	sf_acos.c      sf_log10.c      s_ldexp.c      wf_jn.c \
	   erf_gamma.c	sf_asin.c      sf_logarithm.c  s_log10.c      w_jn.c \
	   erf_lgamma.c	sf_asine.c     sf_log.c        s_logarithm.c  \
	   er_gamma.c	sf_asinh.c     s_floor.c       s_log.c \
	   er_lgamma.c	sf_atan2.c     s_fmod.c        s_mathcnst.c \
	   e_scalb.c	sf_atan.c      sf_numtest.c    s_numtest.c \
	   sf_atangent.c  sf_pow.c        s_pow.c 

ASMSOURCES = 

CC = mb-gcc
AS = mb-as
MB_NEWLIB_SRCDIR = ../../../
INCLUDES = -I$(MB_NEWLIB_SRCDIR)/newlib/libc/include   -I$(MB_NEWLIB_SRCDIR)/newlib/libm/common/
CFLAGS = $(INCLUDES)  ${EXTRA_CFLAGS}
LFLAGS = ${EXTRA_LFLAGS}
OBJS = $(CSOURCES:.c=.o) $(ASMSOURCES:.s=.o)

lib.a : $(OBJS)
	mb-ar r lib.a $?

clean:
	rm -f $(OBJS) lib.a

