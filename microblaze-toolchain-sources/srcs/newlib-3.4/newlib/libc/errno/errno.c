/* The errno variable is stored in the reentrancy structure.  This
   function returns its address for use by the macro errno defined in
   errno.h.  */

#include <errno.h>
#include <reent.h>

#if 0   /* Xilinx: We override this in the Standalone BSP */
#ifndef _REENT_ONLY

int *
__errno ()
{
  return &_REENT->_errno;
}

#endif
#endif  /* #if 0 */

