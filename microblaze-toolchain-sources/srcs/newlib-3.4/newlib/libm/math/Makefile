# CSOURCES = e_acos.c ef_atan2.c k_sin.c sf_asinh.c w_exp.c wf_exp.c \
# 	e_acosh.c ef_atanh.c k_standard.c sf_atan.c w_fmod.c wf_fmod.c \
# 	e_asin.c ef_cosh.c k_tan.c sf_ceil.c w_gamma.c wf_gamma.c e_atan2.c \
# 	kf_cos.c sf_cos.c w_hypot.c wf_hypot.c e_atanh.c  \
# 	kf_rem_pio2.c sf_erf.c w_j0.c wf_j0.c e_cosh.c ef_hypot.c kf_sin.c \
# 	sf_fabs.c w_j1.c wf_j1.c e_exp.c ef_j0.c kf_tan.c sf_floor.c w_jn.c \
# 	wf_jn.c e_fmod.c ef_j1.c s_asinh.c sf_frexp.c w_lgamma.c wf_lgamma.c \
# 	e_hypot.c ef_jn.c sf_isinf.c w_log.c wf_log.c e_j0.c ef_log.c \
# 	s_ceil.c sf_isnan.c w_log10.c wf_log10.c e_j1.c ef_log10.c s_cos.c \
# 	sf_ldexp.c w_pow.c wf_pow.c e_jn.c ef_pow.c s_erf.c sf_signif.c \
# 	w_remainder.c wf_remainder.c e_log.c ef_rem_pio2.c s_fabs.c sf_sin.c \
# 	w_scalb.c wf_scalb.c e_log10.c ef_remainder.c s_floor.c sf_tan.c \
# 	w_sinh.c wf_sinh.c e_pow.c ef_scalb.c s_frexp.c sf_tanh.c w_sqrt.c \
# 	wf_sqrt.c e_rem_pio2.c ef_sinh.c s_infconst.c w_acos.c wf_acos.c \
# 	wr_gamma.c e_remainder.c ef_sqrt.c s_isinf.c w_acosh.c wf_acosh.c \
# 	wr_lgamma.c e_scalb.c er_gamma.c s_isnan.c w_asin.c wf_asin.c \
# 	wrf_gamma.c e_sinh.c er_lgamma.c s_ldexp.c w_atan2.c wf_atan2.c \
# 	wrf_lgamma.c e_sqrt.c erf_gamma.c s_signif.c w_atanh.c wf_atanh.c \
# 	ef_acos.c erf_lgamma.c s_sin.c w_cabs.c wf_cabs.c ef_acosh.c k_cos.c \
# 	s_tan.c w_cosh.c wf_cosh.c ef_asin.c k_rem_pio2.c s_tanh.c w_drem.c wf_drem.c

CSOURCES = ef_sqrt.c s_atan.c s_tan.c  wf_log.c \
           e_acos.c	e_hypot.c      s_ceil.c      s_tanh.c	  w_fmod.c \
	   e_acosh.c	e_j0.c	       s_cos.c	     w_acos.c	  wf_pow.c \
	   e_asin.c	e_j1.c	       s_erf.c	     w_acosh.c	  wf_remainder.c \
	   e_atan2.c	e_jn.c	       s_fabs.c      w_asin.c	  wf_scalb.c \
	   e_atanh.c	e_log10.c      sf_asinh.c    w_atan2.c	  wf_sinh.c \
	   e_cosh.c	e_log.c        sf_atan.c     w_atanh.c	  wf_sqrt.c \
	   e_exp.c		e_pow.c        sf_ceil.c     w_cabs.c	  w_gamma.c \
	   ef_acos.c	e_remainder.c  sf_cos.c      w_cosh.c	  w_hypot.c \
	   ef_acosh.c	e_rem_pio2.c   sf_erf.c      w_drem.c	  w_j0.c \
	   ef_asin.c	erf_gamma.c    sf_fabs.c     w_exp.c	  w_j1.c \
	   ef_atan2.c	erf_lgamma.c   sf_floor.c    wf_acos.c	  w_jn.c \
	   ef_atanh.c	er_gamma.c     sf_frexp.c    wf_acosh.c   w_lgamma.c \
	   ef_cosh.c	er_lgamma.c    sf_isinf.c    wf_asin.c	  w_log10.c \
	   ef_exp.c	e_scalb.c      sf_isnan.c    wf_atan2.c   w_log.c \
	   ef_fmod.c	e_sinh.c       sf_ldexp.c    wf_atanh.c   w_pow.c \
	   ef_hypot.c	e_sqrt.c       s_floor.c     wf_cabs.c	  w_remainder.c \
	   ef_j0.c		k_cos.c        s_frexp.c     wf_cosh.c	  wrf_gamma.c \
	   ef_j1.c		kf_cos.c       sf_signif.c   wf_drem.c	  wrf_lgamma.c \
	   ef_jn.c		kf_rem_pio2.c  sf_sin.c      wf_exp.c	  wr_gamma.c \
	   ef_log10.c	kf_sin.c       sf_tan.c      wf_fmod.c	  wr_lgamma.c \
	   ef_log.c	kf_tan.c       sf_tanh.c     wf_gamma.c   w_scalb.c \
	   e_fmod.c	k_rem_pio2.c   s_infconst.c  wf_hypot.c   w_sinh.c \
	   ef_pow.c	k_sin.c        s_isinf.c     wf_j0.c	  w_sqrt.c \
	   ef_remainder.c	k_standard.c   s_isnan.c     wf_j1.c \
	   ef_rem_pio2.c	k_tan.c        s_ldexp.c     wf_jn.c \
	   ef_scalb.c	s_signif.c    wf_lgamma.c \
	   ef_sinh.c	s_asinh.c      s_sin.c	     wf_log10.c

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

