#
# Old test suite for [incr Tcl] v1.5
# ----------------------------------------------------------------------
#   AUTHOR:  Michael J. McLennan
#            Bell Labs Innovations for Lucent Technologies
#            mmclennan@lucent.com
#            http://www.tcltk.com/itcl
#
#      RCS:  $Id: BarFoo.tcl,v 1.2 1999/01/27 18:56:09 jingham Exp $
# ----------------------------------------------------------------------
#            Copyright (c) 1993-1998  Lucent Technologies, Inc.
# ======================================================================
# See the file "license.terms" for information on usage and
# redistribution of this file, and for a DISCLAIMER OF ALL WARRANTIES.

itcl_class BarFoo {
	inherit Bar Foo

	#
	#  Constructor/destructor add their name to a global var for
	#  tracking implicit constructors/destructors
	#
	constructor {config} {
		global WATCH
		lappend WATCH [namespace current]
	}
	destructor {
		global WATCH
		lappend WATCH [namespace current]
	}
}
