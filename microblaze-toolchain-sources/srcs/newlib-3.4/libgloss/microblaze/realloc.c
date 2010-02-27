
/* Dummy. Till we provide a nice clean intergrated malloc solution */

#include <stdlib.h>
void *realloc (void *ptr, size_t size)
{
  return NULL;
}
