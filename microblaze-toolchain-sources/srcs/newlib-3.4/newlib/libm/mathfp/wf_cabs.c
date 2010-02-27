/*
 * cabsf() wrapper for hypotf().
 * 
 * Written by J.T. Conklin, <jtc@wimsey.com>
 * Placed into the Public Domain, 1994.
 */

#include "fdlibm.h"

struct complex {
	float x;
	float y;
};

/*  GCC 3.4.1
 *  The following routine is builtin to GCC. There is a bug causing this to not compile.
 *  Hence removed.
 */

/* float */
/* cabsf(z) */
/* 	struct complex z; */
/* { */
/* 	return hypotf(z.x, z.y); */
/* } */
