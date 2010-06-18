#!/bin/tcsh -evf
echo "----------------------------------------------------------------"
echo "Building MicroBlaze binutils..."
setenv CURDIR  `pwd`
source common.csh
## Build Directories
setenv BLDDIR ${GNU_ENV}/build/${PLATFORM}/bld_binutils
setenv RELDIR ${GNU_ENV}/release/${PLATFORM}/microblaze
setenv SRCDIR ${GNU_ENV}/srcs/binutils/

if (-e ${BLDDIR}) then
   \rm -rf ${BLDDIR}
endif
mkdir -p ${BLDDIR}

cd ${BLDDIR}
${SRCDIR}/configure -target=microblaze -prefix=${RELDIR} -program-prefix="mb-" -v
make all CC="gcc -m32" CXX="g++ -m32" CFLAGS='-O2 -g -DMICROBLAZE'
echo "Installing MicroBlaze in ${RELDIR}"
make install CC="gcc -m32" CXX="g++ -m32"
# Configure and make gprof 
echo "Configuring gprof"
mkdir gprof
cd gprof
${SRCDIR}/gprof/configure -target=microblaze -prefix=${RELDIR} -program-prefix="mb-" -v
echo "Making gprof"
make install CC="gcc -m32" CXX="g++ -m32" CFLAGS="-g -O2 -DMICROBLAZE"
cd ${CURDIR}

if (${GNU_NO_CLEAN} == 0) then
    echo "Now cleaning -- BINUTILS..."
    rm -rf ${BLDDIR}
endif

echo "MicroBlaze binutils done."
echo "----------------------------------------------------------------"
