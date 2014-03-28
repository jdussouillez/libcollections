#ifndef ERRORS_H
#define ERRORS_H

/*
 * Error values
 */
typedef enum {
  CERR_SUCCESS = 0, // Success
  CERR_NULLVALUE, // A parameter is NULL
  CERR_SYSTEM, // A system error (malloc...)
  CERR_FORBIDDEN, // A parameter is forbidden (for instance : same list as parameter when calling llist_addall)
  CERR_BADINDEX // Bad index (for arraylists)
} cerr_t;

#ifdef __cplusplus
extern "C" {
#endif

// Collections error number
extern cerr_t cerrno;

// Returns string describing the error.
const char* cstrerror(int cerrnum);

// Print an error message.
void cperror(const char* msg);

#ifdef __cplusplus
}
#endif

#endif // ERRORS_H
