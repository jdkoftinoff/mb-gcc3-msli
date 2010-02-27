#!/bin/tcsh -evf
setenv CURDIR  `pwd`
echo "----------------------------------------------------------------"
echo "Building MicroBlaze GCC"
source common.csh
setenv BLDDIR ${GNU_ENV}/build/${PLATFORM}/bld_gcc/
setenv RELDIR ${GNU_ENV}/release/${PLATFORM}/microblaze
setenv BIN_DIR ${GNU_ENV}/release/${PLATFORM}/microblaze/bin
setenv GCC_SRC_DIR ${GNU_ENV}/srcs/gcc

set path = (${BIN_DIR} $path)
which mb-as
if (-e ${BLDDIR}) then
    \rm -rf ${BLDDIR}
endif
mkdir -p ${BLDDIR}
rehash
mkdir -p ${RELDIR}/microblaze/sys-include
cp -rf ${GNU_ENV}/srcs/newlib-3.4/newlib/libc/include/*  ${RELDIR}/microblaze/sys-include/
# Add build date encoding to version info
echo '#define XILINX_GNU_BUILD_DATE "' `date "+%d%m%y"` '"' > ${GCC_SRC_DIR}/gcc/edk_bld_date.h         
## Hack for Version. This is the only way we can get version.c to pass through our configure scripts.
cpp ${GCC_SRC_DIR}/gcc/version.c.2 | sed -e 's/\" \"//g' -e 's/\"  \"//g' -e 's/\"   \"//g' -e '/static/d' -e '/^#/d' -e '/extern/d' > ${GCC_SRC_DIR}/gcc/version.c
## End Hack
cd ${BLDDIR}
## OPTIMIZE_FOR_LOW_MEMORY  --  To prevent verbose termination handler
## _NEWLIB_VERSION          --  To prevent testsuite from invoking mkfifo
${GCC_SRC_DIR}/configure --srcdir=${GCC_SRC_DIR} --target=microblaze  --prefix=${RELDIR} --program-prefix="mb-" --nfp  -v --enable-languages="c++" --with-newlib --with-headers --enable-multilib --enable-cxx-flags="-DOPTIMIZE_FOR_LOW_MEMORY -D_NEWLIB_VERSION"
make all CFLAGS="-O2 -DMICROBLAZE -DEDK" CXXFLAGS="-O2 -DMICROBLAZE -DEDK"
echo "make install"
make install CFLAGS="-O2 -DMICROBLAZE -DEDK" CXXFLAGS="-O2 -DMICROBLAZE -DEDK"
cd ${BIN_DIR}

# if ( -e microblaze-gcc${EXT} ) then
#     mv microblaze-gcc${EXT} mb-gcc${EXT}
# endif
    
# if ( -e microblaze-c++${EXT} ) then
#     mv microblaze-c++${EXT} mb-c++${EXT}
# endif

# if ( -e microblaze-g++${EXT} ) then
#     mv microblaze-g++${EXT} mb-g++${EXT}
# endif
    
# if ( -e microblaze-protoize${EXT} ) then
#     mv microblaze-protoize${EXT} mb-protoize${EXT}
# endif
    
# if ( -e microblaze-unprotoize${EXT} ) then
#     mv microblaze-unprotoize${EXT} mb-unprotoize${EXT}
# endif

# if ( -e microblaze-c++filt${EXT} ) then
#    mv microblaze-c++filt${EXT} mb-c++filt${EXT}
# endif

cd $CURDIR

## Links do not work properly on the NT platform.
## Only for NT platform use copy instead of Links. 
if ( ${PLATFORM} == "nt" ) then
    rm -rf ${RELDIR}/bin/mb-g++.exe \
       ${RELDIR}/bin/microblaze-gcc-3.4.1.exe \
       ${RELDIR}/microblaze/bin/ar.exe \
       ${RELDIR}/microblaze/bin/as.exe \
       ${RELDIR}/microblaze/bin/ld.exe \
       ${RELDIR}/microblaze/bin/nm.exe \
       ${RELDIR}/microblaze/bin/ranlib.exe \
       ${RELDIR}/microblaze/bin/strip.exe 
       

	cp ${RELDIR}/bin/mb-c++.exe ${RELDIR}/bin/mb-g++.exe
	cp ${RELDIR}/bin/mb-gcc.exe ${RELDIR}/bin/microblaze-gcc-3.4.1.exe
	cp ${RELDIR}/bin/mb-ar.exe ${RELDIR}/microblaze/bin/ar.exe
	cp ${RELDIR}/bin/mb-as.exe ${RELDIR}/microblaze/bin/as.exe
	cp ${RELDIR}/bin/mb-ld.exe ${RELDIR}/microblaze/bin/ld.exe
	cp ${RELDIR}/bin/mb-nm.exe ${RELDIR}/microblaze/bin/nm.exe
	cp ${RELDIR}/bin/mb-ranlib.exe ${RELDIR}/microblaze/bin/ranlib.exe
	cp ${RELDIR}/bin/mb-strip.exe ${RELDIR}/microblaze/bin/strip.exe
endif

if (${GNU_NO_CLEAN} == 0) then
    echo "Now cleaning -- GCC"
    rm -rf ${BLDDIR}
endif

echo "Copying dummy libxil into place"
cp ${GNU_ENV}/lib-stub/libxil.a ${RELDIR}/microblaze/lib

echo "Microblaze GCC Done"
echo "----------------------------------------------------------------"




