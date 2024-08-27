#include <stdlib.h>
#include <string.h>

#include "error.h"

void error_info(const char *msg, size_t errnum, size_t status)
{
  fprintf(stderr, "%s: %s\n",
      msg, strerror(errnum));

  exit(status);
}

