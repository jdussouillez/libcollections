#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "collections/errors.h"

cerr_t cerrno = CERR_SUCCESS;

static const char* cerr_msg[] = {"Success",
				 "NULL parameter",
				 "System error", // Useless because we use errno to display the system error
				 "Forbidden parameter"};

const char* cstrerror(int errnum) {
  if (errnum < 0 || errnum > 3)
    return NULL;
  return (errnum == CERR_SYSTEM) ? strerror(errno) : cerr_msg[errnum];
}

void cperror(const char* msg) {
  printf("%s: %s\n", msg, cstrerror(cerrno));
}
