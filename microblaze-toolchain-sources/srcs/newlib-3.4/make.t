#! cd .
###############################################################################
# Copyright 2000 Xilinx, Inc.  All rights reserved.
#
# make.t file for newlib 
#
# $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/newlib-3.4/make.t,v 1.1 2005/03/18 20:00:31 vasanth Exp $
#
###############################################################################


#EDK_RTF = ../../../../../../../rtf/
!RtfFILE EDK_MB_RTF=gnu/microblaze/${PLATFORM}
!RtfFile EDK_PPC_RTF=gnu/powerpc-eabi/${PLATFORM}
!RtfFile EDK_LIB_SRC=sw/lib/microblaze/src

!EnvFile EDK_BLDTOOLS=Jobs/MDT/BuildTools
!EnvFile EDK_GNUDIR=Jobs/MDT/sw/ThirdParty/gnu/src/newlib


all: execs rellibs

clean:
	@echo "Nothing to be done for Clean on EDK subdirectories..."
execs:
	@echo "Nothing to be done for Execs for Newlib in GNU"

!X nt
rellibs:
	perl ${EDK_BLDTOOLS}/release_files.pl ${EDK_GNUDIR}/relall/microblaze ${EDK_MB_RTF} ${EDK_GNUDIR}/rellists/rellist.microblaze.all
	perl ${EDK_BLDTOOLS}/release_files.pl ${EDK_GNUDIR}/relall/powerpc-eabi ${EDK_PPC_RTF} ${EDK_GNUDIR}/rellists/rellist.powerpc-eabi.all
!O sol
	cp -rf ${EDK_GNUDIR}/relall/microblaze/microblaze/lib/src/crt*.s ${EDK_LIB_SRC}/

!O nt
rellibs:
	cp -rf ${EDK_GNUDIR}/relall/microblaze/* ${EDK_MB_RTF}
	cp -rf ${EDK_GNUDIR}/relall/powerpc-eabi/* ${EDK_PPC_RTF}
	cd ${EDK_PPC_RTF}; source ${EDK_BLDTOOLS}/remove-xcs 
	cd ${EDK_MB_RTF}; source ${EDK_BLDTOOLS}/remove-xcs 	
