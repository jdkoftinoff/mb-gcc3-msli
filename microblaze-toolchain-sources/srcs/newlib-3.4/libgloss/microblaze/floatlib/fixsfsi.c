//////////////////////////////////////////////////////////////////////-*-C-*- 
// 
// Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
// 
// fixsfsi.c
//
// This file converts a float number to integer. 
// Used for truncation 
// 
// $Id: fixsfsi.c,v 1.1 2005/03/18 20:00:31 vasanth Exp $
// 
///////////////////////////////////////////////////////////////////////////////


#include "floatlib.h"

long
__fixsfsi (float a1)
{
  register union float_long fl1;
  register int exp;
  register long l;

  fl1.f = a1;

  if (fl1.l == 0)
    return (0);

  // For CR# 181496, values of float 0x1000000 and above were not
  // being converted back to fix properly. The fix is to use up the
  // higher 8 bits in addition to the mantissa.
  
  /*  exp = 24 - (EXP (fl1.l) - EXCESS);
  l = MANT(fl1.l);
  */
  
  exp = 32 - (EXP (fl1.l) - EXCESS);
  l = MANT(fl1.l) << 8;   //l = ( (((fl1.l) & 0x7FFFFF) << 8) | (1 << 31));

  if (exp < 0)
     return 0;
  else
     l = (unsigned int) l >> exp;
     
  return (SIGN (fl1.l) ? -l : l);
}
