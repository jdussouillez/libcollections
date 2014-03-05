#ifndef CORE_H
#define CORE_H

#define CBUFSIZE 2048

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function pointer to compare 2 objects.
 * Returns 0 if the objects are equals.
 */
typedef int (*comparefct_t)(void*, void*);

/*
 * Function pointer that describes a function which
 * returns a string describing the element.
 * Must returns NULL on error.
 */
typedef char* (*tostringfct_t)(void*);

#ifdef __cplusplus
}
#endif

#endif // CORE_H
