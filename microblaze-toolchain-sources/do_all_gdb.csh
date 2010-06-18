#!/bin/tcsh -evf
setenv CURDIR  `pwd`
source common.csh
echo "----------------------------------------------------------------"
echo "Building MicroBlaze GDB"

setenv BLDDIR ${GNU_ENV}/build/${PLATFORM}/bld_gdb/
setenv RELDIR ${GNU_ENV}/release/${PLATFORM}/microblaze
setenv BIN_DIR ${GNU_ENV}/release/${PLATFORM}/microblaze/bin
setenv GDB_SRC_DIR ${GNU_ENV}/srcs/gdb

if (-e ${BLDDIR}) then
    \rm -rf ${BLDDIR}
endif
mkdir -p ${BLDDIR}

if (-e ${RELDIR}) then
    echo "${RELDIR} exists"
else
    mkdir -p ${RELDIR}
endif

chmod -R 755 ${GDB_SRC_DIR}
    
# do uclinux mb-uclinux-gdb first
cd ${BLDDIR}
${GDB_SRC_DIR}/configure -srcdir=${GDB_SRC_DIR} -target=microblaze -prefix=${RELDIR} -program-prefix="mb-uclinux-" -enable-threads
gmake all CC="gcc -m32" CXX="g++ -m32" CFLAGS='-O2 -DMICROBLAZE -D__MICROBLAZE_UCLINUX__ -DMICROBLAZE_DEBUG -fPIC'
gmake install CC="gcc -m32" CXX="g++ -m32"

# Rename executables to add -user suffix
cd ${RELDIR}/bin/
mv mb-uclinux-insight${EXT} mb-uclinux-gdb${EXT}
strip mb-uclinux-gdb${EXT}
strip mb-uclinux-run${EXT}
if ($PLATFORM == "nt") then
    strip *.dll
endif

# Force removal of BLDDIR so standalone mb-gdb build works fine
# A little slow, but safer
rm -rf ${BLDDIR}

mkdir -p ${BLDDIR}

# do standalone microblaze gdb
cd ${BLDDIR}
${GDB_SRC_DIR}/configure -srcdir=${GDB_SRC_DIR} -target=microblaze -prefix=${RELDIR} -program-prefix="mb-" 
# To build for uclinux platform, add '-D__MICROBLAZE_UCLINUX__' to CFLAGS below
gmake all CC="gcc -m32" CXX="g++ -m32" CFLAGS='-O2 -DMICROBLAZE -fPIC' 
gmake install CC="gcc -m32" CXX="g++ -m32"
    
cd ${RELDIR}/bin/
mv mb-insight${EXT} mb-gdb${EXT}
strip mb-gdb${EXT}
strip mb-run${EXT}
if ($PLATFORM == "nt") then
    strip *.dll
endif

cd ${CURDIR}

if (${GNU_NO_CLEAN} == 0) then
    echo "Now cleaning -- GDB"
    rm -rf ${BLDDIR}
    rm -rf ${RELDIR}/man
endif
echo "MicroBlaze GDB Done"


