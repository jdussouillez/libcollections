#ifndef CORE_H
#define CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function pointer to compare 2 objects.
 * Returns 0 if the objects are equals.
 */
typedef int (*comparefct_t)(void*, void*);

#ifdef __cplusplus
}
#endif

#endif // CORE_H
