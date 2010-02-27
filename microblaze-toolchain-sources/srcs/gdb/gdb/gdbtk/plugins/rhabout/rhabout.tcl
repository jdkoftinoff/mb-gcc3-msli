package provide RHABOUT 1.0
set dirname [file dirname [info script]]
lappend auto_path $dirname
set TCL_DBGX "" ; # needed for cygwin
catch {load [file join $dirname rhabout.so]}
