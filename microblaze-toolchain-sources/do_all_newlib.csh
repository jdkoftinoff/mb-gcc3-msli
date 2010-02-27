#!/bin/tcsh -evf
setenv CURDIR  `pwd`
source common.csh
echo "----------------------------------------------------------------"
echo "Building MicroBlaze Newlib"
setenv SRCDIR ${GNU_ENV}/srcs/newlib-3.4
setenv BLDDIR ${GNU_ENV}/build/${PLATFORM}/bld_newlib
setenv RELDIR ${GNU_ENV}/release/${PLATFORM}/microblaze
setenv BINDIR ${GNU_ENV}/release/${PLATFORM}/microblaze/bin
set path = ( ${BINDIR} $path )
echo $PATH
rehash
which mb-gcc
if !(-e ${RELDIR}/lib/src) then
    mkdir -p ${RELDIR}/lib/src
    echo "Created directory ${RELDIR}"
endif

if (-e ${BLDDIR}) then
    \rm -rf ${BLDDIR}
endif
mkdir -p ${BLDDIR}
echo "Created directory ${BLDDIR} for build"
    
# Copy the source files in the Build Area
cp -Rf ${SRCDIR}/Makefile  ${BLDDIR}
cp -Rf ${SRCDIR}/libgloss  ${BLDDIR}
cp -Rf ${SRCDIR}/newlib    ${BLDDIR}
cp -Rf ${SRCDIR}/stringops ${BLDDIR}
    
# Copy Xilinx modified header files [.h files] into the src/include area
# This will go when we go the delta way.
cp -Rf ${SRCDIR}/libgloss/include/* ${BLDDIR}/newlib/libc/include
cd ${BLDDIR}

    # Build all flavors of libc.a and libm.a

    # libm_m_bs_p_fps.a
    make MULT=_m SHIFT=_bs ECMP=_p FPU=_fp DOUBLE=s EXTRA_CFLAGS="-O3 -mno-xl-soft-mul -mxl-barrel-shift -mxl-pattern-compare -mhard-float -D_DOUBLE_IS_32BITS" libm_t
    make clean

    # libm_m_bs_p_fpd.a
    make MULT=_m SHIFT=_bs ECMP=_p FPU=_fp DOUBLE=d EXTRA_CFLAGS="-O3 -mno-xl-soft-mul -mxl-barrel-shift -mxl-pattern-compare -mhard-float" libm_t
    make clean

    # libm_m_p_fps.a
    make MULT=_m ECMP=_p FPU=_fp DOUBLE=s EXTRA_CFLAGS="-O3 -mno-xl-soft-mul -mxl-pattern-compare -mhard-float -D_DOUBLE_IS_32BITS" libm_t
    make clean

    # libm_m_p_fpd.a
    make MULT=_m ECMP=_p FPU=_fp DOUBLE=d EXTRA_CFLAGS="-O3 -mno-xl-soft-mul -mxl-pattern-compare -mhard-float" libm_t
    make clean

    # libm_bs_p_fps.a
    make SHIFT=_bs ECMP=_p FPU=_fp DOUBLE=s EXTRA_CFLAGS="-O3 -mxl-barrel-shift -mxl-pattern-compare -mhard-float -D_DOUBLE_IS_32BITS" libm_t
    make clean

    # libm_bs_p_fpd.a
    make SHIFT=_bs ECMP=_p FPU=_fp DOUBLE=d EXTRA_CFLAGS="-O3 -mxl-barrel-shift -mxl-pattern-compare -mhard-float" libm_t
    make clean

    # libm_p_fps.a
    make FPU=_fp ECMP=_p DOUBLE=s EXTRA_CFLAGS="-O3 -mxl-pattern-compare -mhard-float -D_DOUBLE_IS_32BITS" libm_t
    make clean

    # libm_p_fpd.a
    make FPU=_fp ECMP=_p DOUBLE=d EXTRA_CFLAGS="-O3 -mxl-pattern-compare -mhard-float" libm_t
    make clean

    # libm_m_bs_fps.a
    make MULT=_m SHIFT=_bs FPU=_fp DOUBLE=s EXTRA_CFLAGS="-O3 -mno-xl-soft-mul -mxl-barrel-shift -mhard-float -D_DOUBLE_IS_32BITS" libm_t
    make clean

    # libm_m_bs_fpd.a
    make MULT=_m SHIFT=_bs FPU=_fp DOUBLE=d EXTRA_CFLAGS="-O3 -mno-xl-soft-mul -mxl-barrel-shift -mhard-float" libm_t
    make clean

    # libm_m_fps.a
    make MULT=_m FPU=_fp DOUBLE=s EXTRA_CFLAGS="-O3 -mno-xl-soft-mul -mhard-float -D_DOUBLE_IS_32BITS" libm_t
    make clean

    # libm_m_fpd.a
    make MULT=_m FPU=_fp DOUBLE=d EXTRA_CFLAGS="-O3 -mno-xl-soft-mul -mhard-float" libm_t
    make clean

    # libm_bs_fps.a
    make SHIFT=_bs FPU=_fp DOUBLE=s EXTRA_CFLAGS="-O3 -mxl-barrel-shift -mhard-float -D_DOUBLE_IS_32BITS" libm_t
    make clean

    # libm_bs_fpd.a
    make SHIFT=_bs FPU=_fp DOUBLE=d EXTRA_CFLAGS="-O3 -mxl-barrel-shift -mhard-float" libm_t
    make clean

    # libm_fps.a
    make FPU=_fp DOUBLE=s EXTRA_CFLAGS="-O3 -mhard-float -D_DOUBLE_IS_32BITS" libm_t
    make clean

    # libm_fpd.a
    make FPU=_fp DOUBLE=d EXTRA_CFLAGS="-O3 -mhard-float" libm_t
    make clean

    # lib[c/m]_m_bs_p.a
    make MULT=_m ECMP=_p SHIFT=_bs EXTRA_CFLAGS="-O3 -mno-xl-soft-mul -mxl-barrel-shift -mxl-pattern-compare" combined_t
    make clean

    # lib[c/m]_m_p.a
    make MULT=_m ECMP=_p EXTRA_CFLAGS="-O3 -mno-xl-soft-mul -mxl-pattern-compare" combined_t
    make clean

    # lib[c/m]_bs_p.a
    make SHIFT=_bs ECMP=_p EXTRA_CFLAGS="-O3 -mxl-barrel-shift -mxl-pattern-compare" combined_t
    make clean

    # lib[c/m]_p.a
    make ECMP=_p EXTRA_CFLAGS="-O3 -mxl-pattern-compare" combined_t
    make clean

    # lib[c/m]_m_bs.a
    make MULT=_m SHIFT=_bs EXTRA_CFLAGS="-O3 -mno-xl-soft-mul -mxl-barrel-shift" combined_t
    make clean

    # lib[c/m]_m.a
    make MULT=_m EXTRA_CFLAGS="-O3 -mno-xl-soft-mul" combined_t
    make clean

    # lib[c/m]_bs.a
    make SHIFT=_bs EXTRA_CFLAGS="-O3 -mxl-barrel-shift" combined_t
    make clean

    # lib[c/m].a
    make EXTRA_CFLAGS="-O3" combined_t
    # Don't do a clean here.

    echo "Created all versions of libc.a, libm.a, ..."
    echo "Now creating versions based on extended compare instructions ..."
    echo "----------------------------------------------------------------"

    # Generate str*.o which use extended compare instructions
    make stringop

    # Now replace the str*.o files in each libc that had pattern compare enabled, with the optimized versions

    mb-ar r  ${BLDDIR}/libc_m_bs_p.a  stringops/*.o
    mb-ar r  ${BLDDIR}/libc_m_p.a  stringops/*.o
    mb-ar r  ${BLDDIR}/libc_bs_p.a  stringops/*.o
    mb-ar r  ${BLDDIR}/libc_p.a  stringops/*.o

    echo "----------------------------------------------------------------"
    echo "Done."

    
    # Copy the libs to ${RELDIR}/lib
    # also clean CVS... Help build a cleaner rtf
    cp -f ${BLDDIR}/libc*.a ${RELDIR}/microblaze/lib
    cp -f ${BLDDIR}/libm*.a ${RELDIR}/microblaze/lib
    echo "libc.a and libm.a copied to ${RELDIR}/microblaze/lib"
    \rm -rf `find * -name "CVS"`
    
    # Copy the includes to the include release area
    cp -R -f ${BLDDIR}/newlib/libc/include/* ${RELDIR}/microblaze/sys-include/
    echo "newlib include files copied to ${RELDIR}/microblaze"
    
    # Copy the crts into the same area
    cp ${BLDDIR}/libgloss/microblaze/crt0.o ${RELDIR}/microblaze/lib
    cp ${BLDDIR}/libgloss/microblaze/crt1.o ${RELDIR}/microblaze/lib
    cp ${BLDDIR}/libgloss/microblaze/crt2.o ${RELDIR}/microblaze/lib
    cp ${BLDDIR}/libgloss/microblaze/crt3.o ${RELDIR}/microblaze/lib
    cp ${BLDDIR}/libgloss/microblaze/crt4.o ${RELDIR}/microblaze/lib
    cp ${BLDDIR}/libgloss/microblaze/crtinit.o ${RELDIR}/microblaze/lib
    cp ${BLDDIR}/libgloss/microblaze/pgcrtinit.o ${RELDIR}/microblaze/lib
    cp ${BLDDIR}/libgloss/microblaze/sim-crtinit.o ${RELDIR}/microblaze/lib
    cp ${BLDDIR}/libgloss/microblaze/sim-pgcrtinit.o ${RELDIR}/microblaze/lib
    echo "crt0.o, crt1.o, crt4.o, (sim-)crtinit.o and (sim-)pgcrtinit.o copied to ${RELDIR}/microblaze/lib"
    
    # Copy crt sources 
    mkdir -p ${RELDIR}/microblaze/lib/src
    cp -f ${BLDDIR}/libgloss/microblaze/crt0.s ${RELDIR}/microblaze/lib/src
    cp -f ${BLDDIR}/libgloss/microblaze/crt1.s ${RELDIR}/microblaze/lib/src
    cp -f ${BLDDIR}/libgloss/microblaze/crt2.s ${RELDIR}/microblaze/lib/src
    cp -f ${BLDDIR}/libgloss/microblaze/crt3.s ${RELDIR}/microblaze/lib/src
    cp -f ${BLDDIR}/libgloss/microblaze/crt4.s ${RELDIR}/microblaze/lib/src
    cp -f ${BLDDIR}/libgloss/microblaze/crtinit.s ${RELDIR}/microblaze/lib/src
    cp -f ${BLDDIR}/libgloss/microblaze/pgcrtinit.s ${RELDIR}/microblaze/lib/src
    cp -f ${BLDDIR}/libgloss/microblaze/sim-crtinit.s ${RELDIR}/microblaze/lib/src
    cp -f ${BLDDIR}/libgloss/microblaze/sim-pgcrtinit.s ${RELDIR}/microblaze/lib/src
    echo "crt0.s, crt1.s , crt4.s, (sim-)crtinit.s and (sim-)pgcrtinit.s copied to ${RELDIR}/microblaze/lib/src"

cd ${CURDIR}
if (${GNU_NO_CLEAN} == 0) then
    echo "Now cleaning -- Newlib"
    rm -rf ${BLDDIR}
endif
echo "Done MicroBlaze Newlib"    
echo "----------------------------------------------------------------"
