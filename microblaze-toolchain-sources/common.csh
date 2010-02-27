#!/bin/tcsh -evf

setenv GNU_NO_CLEAN 0
if ($PLATFORM == "nt") then
  setenv EXT .exe
else
  setenv EXT
endif
