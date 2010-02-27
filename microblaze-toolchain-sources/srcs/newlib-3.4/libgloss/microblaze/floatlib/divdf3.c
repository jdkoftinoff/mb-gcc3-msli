/*
** libgcc support for software floating point.
** Copyright (C) 1991 by Pipeline Associates, Inc.  All rights reserved.
** Permission is granted to do *anything* you want with this file,
** commercial or otherwise, provided this message remains intact.  So there!
** I would appreciate receiving any updates/patches/changes that anyone
** makes, and am willing to be the repository for said changes (am I
** making a big mistake?).

Warning! Only single-precision is actually implemented.  This file
won't really be much use until double-precision is supported.

However, once that is done, this file might eventually become a
replacement for libgcc1.c.  It might also make possible
cross-compilation for an IEEE target machine from a non-IEEE
host such as a VAX.

If you'd like to work on completing this, please talk to rms@gnu.ai.mit.edu.

--> Double precision floating support added by James Carlson on 20 April 1998.

**
** Pat Wood
** Pipeline Associates, Inc.
** pipeline!phw@motown.com or
** sun!pipeline!phw or
** uunet!motown!pipeline!phw
**
** 05/01/91 -- V1.0 -- first release to gcc mailing lists
** 05/04/91 -- V1.1 -- added float and double prototypes and return values
**                  -- fixed problems with adding and subtracting zero
**                  -- fixed rounding in truncdfsf2
**                  -- fixed SWAP define and tested on 386
*/

/*
** The following are routines that replace the libgcc soft floating point
** routines that are called automatically when -msoft-float is selected.
** The support single and double precision IEEE format, with provisions
** for byte-swapped machines (tested on 386).  Some of the double-precision
** routines work at full precision, but most of the hard ones simply punt
** and call the single precision routines, producing a loss of accuracy.
** long long support is not assumed or included.
** Overall accuracy is close to IEEE (actually 68882) for single-precision
** arithmetic.  I think there may still be a 1 in 1000 chance of a bit
** being rounded the wrong way during a multiply.  I'm not fussy enough to
** bother with it, but if anyone is, knock yourself out.
**
** Efficiency has only been addressed where it was obvious that something
** would make a big difference.  Anyone who wants to do this right for
** best speed should go in and rewrite in assembler.
**
** I have tested this only on a 68030 workstation and 386/ix integrated
** in with -msoft-float.
*/

#include "floatlib.h"


/* divide two doubles */
double
__divdf3 (double a1, double a2)
{
    register union double_long fl1, fl2;
    register long long mask,result;
    register int exp, sign;

    fl1.d = a1;
    fl2.d = a2;

    /* subtract exponents */
    exp = EXPD(fl1) - EXPD(fl2) + EXCESSD;

    /* compute sign */
    sign = SIGND(fl1) ^ SIGND(fl2);

    /* numerator zero??? */
    if (fl1.ll == 0) {
	/* divide by zero??? */
	if (fl2.ll == 0)
	    fl1.ll = ((unsigned long long)1<<63)-1;	/* NaN */
	else
	    fl1.ll = 0;
	goto test_done;
    }

    /* return +Inf or -Inf */
    if (fl2.ll == 0) {
	fl1.ll = PACKD_LL(SIGND(fl1),2047,0);
	goto test_done;
    }


    /* now get mantissas */
    fl1.ll = MANTD_LL(fl1);
    fl2.ll = MANTD_LL(fl2);

    /* this assures we have 54 bits of precision in the end */
    if (fl1.ll < fl2.ll) {
	fl1.ll <<= 1;
	exp--;
    }

    /* now we perform repeated subtraction of fl2.ll from fl1.ll */
    mask = (long long)1<<53;
    result = 0;
    while (mask) {
	if (fl1.ll >= fl2.ll)
	{
	    result |= mask;
	    fl1.ll -= fl2.ll;
	}
	fl1.ll <<= 1;
	mask >>= 1;
    }

    /* round */
    result += 1;

    /* normalize down */
    exp++;
    result >>= 1;

    result &= ~HIDDEND_LL;

    /* pack up and go home */
    fl1.ll = PACKD_LL(sign, exp, result);

test_done:
    return (fl1.d);
}
