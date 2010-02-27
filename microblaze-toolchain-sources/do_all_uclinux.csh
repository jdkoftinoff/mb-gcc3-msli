#!/bin/tcsh -evf
setenv CURDIR  `pwd`
source common.csh
echo "----------------------------------------------------------------"
echo "Building MicroBlaze uClinux tools"
## Orig Sources
setenv ELF2FLT_SRC ${GNU_ENV}/srcs/elf2flt
setenv GENROMFS_SRC ${GNU_ENV}/srcs/genromfs
setenv BINUTILS_INCLUDE_DIR 

## Build Directories
setenv BLDDIR ${GNU_ENV}/build/${PLATFORM}/bld_mb_uclinux
setenv ELF2FLT_BLDDIR ${BLDDIR}/bld_elf2flt
setenv TMP_ELF2FLT_SRC ${GNU_ENV}/build/${PLATFORM}/tmp_elf2flt_src
setenv TMP_GENROMFS_SRC ${GNU_ENV}/build/${PLATFORM}/tmp_genromfs_src

## RELEASEDIR
setenv RELDIR ${GNU_ENV}/release/${PLATFORM}/microblaze

if (-e ${TMP_ELF2FLT_SRC}) then
 \rm -rf ${TMP_ELF2FLT_SRC}
endif
mkdir -p ${TMP_ELF2FLT_SRC}

if (-e ${TMP_GENROMFS_SRC}) then
 \rm -rf ${TMP_GENROMFS_SRC}
endif
mkdir -p ${TMP_GENROMFS_SRC}

# Copy original sources
echo ""
cp -rf ${ELF2FLT_SRC}/* ${TMP_ELF2FLT_SRC}
cp -rf ${GENROMFS_SRC}/* ${TMP_GENROMFS_SRC}
rm -rf ${ELF2FLT_BLDDIR}
mkdir -p ${ELF2FLT_BLDDIR}

## Build elf2flt first
cd ${ELF2FLT_BLDDIR}
setenv BINUTILS_SRCS ${GNU_ENV}/srcs/binutils
cp -rf ${BINUTILS_SRCS}/include/elf ${TMP_ELF2FLT_SRC}/
if ( -e Makefile ) then
    make clean
endif
${TMP_ELF2FLT_SRC}/configure --target=microblaze --prefix=${RELDIR} \
        --with-libbfd=${RELDIR}/lib/libbfd.a \
        --with-libiberty=${RELDIR}/lib/libiberty.a --program-prefix="mb"\
        --with-bfd-include-dir=${RELDIR}/include \
        --with-binutils-include-dir=${TMP_ELF2FLT_SRC}/

make all 
make install

## Build genromfs next
cd ${TMP_GENROMFS_SRC}
make clean
make
cp -r ${TMP_GENROMFS_SRC}/genromfs${EXT} ${RELDIR}/bin/genromfs${EXT}
cd ${CURDIR}

if (${GNU_NO_CLEAN} == 0) then
   echo "Now cleaning -- uClinux tools"
   rm -rf ${TMP_ELF2FLT_SRC}
   rm -rf ${TMP_GENROMFS_SRC}
   rm -rf ${BLDDIR}
endif 
echo "Microblaze uClinux tools Done"
echo "----------------------------------------------------------------"




