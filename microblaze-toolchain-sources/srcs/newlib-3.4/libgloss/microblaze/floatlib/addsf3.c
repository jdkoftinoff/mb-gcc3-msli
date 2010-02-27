#include "floatlib.h"
/* add two floats */
float
__addsf3 (float a1, float a2)
{
  register long mant1, mant2;
  register union float_long fl1, fl2;
  register int exp1, exp2;
  int sign = 0;

  fl1.f = a1;
  fl2.f = a2;

  /* check for zero args */
  if (!fl1.l) {
      fl1.f = fl2.f;
      goto test_done;
  }
  if (!fl2.l)
      goto test_done;

  exp1 = EXP (fl1.l);
  exp2 = EXP (fl2.l);

  if (exp1 > exp2 + 25)
      goto test_done;
  if (exp2 > exp1 + 25) {
      fl1.f = fl2.f;
      goto test_done;
  }

  /* do everything in excess precision so's we can round later */
  mant1 = MANT (fl1.l) << 6;
  mant2 = MANT (fl2.l) << 6;

  if (SIGN (fl1.l))
    mant1 = -mant1;
  if (SIGN (fl2.l))
    mant2 = -mant2;

  if (exp1 > exp2)
    {
      mant2 >>= exp1 - exp2;
    }
  else
    {
      mant1 >>= exp2 - exp1;
      exp1 = exp2;
    }
  mant1 += mant2;

  if (mant1 < 0)
    {
      mant1 = -mant1;
      sign = SIGNBIT;
    }
  else if (!mant1) {
      fl1.f = 0;
      goto test_done;
  }

  /* normalize up */
  while (!(mant1 & 0xE0000000))
    {
      mant1 <<= 1;
      exp1--;
    }

  /* normalize down? */
  if (mant1 & (1 << 30))
    {
      mant1 >>= 1;
      exp1++;
    }

  /* round to even */
  mant1 += (mant1 & 0x40) ? 0x20 : 0x1F;

  /* normalize down? */
  if (mant1 & (1 << 30))
    {
      mant1 >>= 1;
      exp1++;
    }

  /* lose extra precision */
  mant1 >>= 6;

  /* turn off hidden bit */
  mant1 &= ~HIDDEN;

  /* pack up and go home */
  fl1.l = PACK (sign, exp1, mant1);
test_done:
  return (fl1.f);
}
