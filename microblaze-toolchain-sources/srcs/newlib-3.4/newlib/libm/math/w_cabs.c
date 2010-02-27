/*
 * cabs() wrapper for hypot().
 * 
 * Written by J.T. Conklin, <jtc@wimsey.com>
 * Placed into the Public Domain, 1994.
 */

#include "fdlibm.h"

struct complex {
	double x;
	double y;
};

/*  GCC 3.4.1
 *  The following routine is builtin to GCC. There is a bug causing this to not compile.
 *  Hence removed.
 */

/* double */
/* cabs(z) */
/* 	struct complex z; */
/* { */
/* 	return hypot(z.x, z.y); */
/* } */
