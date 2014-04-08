#ifndef DEQUE_H
#define DEQUE_H

#include "collections/core.h"
#include "collections/linkedlist.h"

/*
 * Iterates over the deque pointer "dequep" and run the instructions "instrs".
 * The "data" variable contains the data of the current element.
 */
#define DEQUE_FOREACH(dequep, instrs) LLIST_FOREACH(dequep, instrs)

#ifdef __cplusplus
extern "C" {
#endif

typedef linkedlist_t deque_t;

/*
 * Inserts the specified element to the end of the deque (memory copy).
 * On success, returns 1.
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int deque_addall(deque_t* dest, deque_t* src);

/*
 * Inserts the specified element at the beginning of this deque.
 * On success, returns 1.
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int deque_addfirst(deque_t* deque, void* e);

/*
 * Inserts the specified element at the end of this deque.
 * On success, returns 1.
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int deque_addlast(deque_t* deque, void* e);

/*
 * Removes all the elements in the deque.
 */
void deque_clear(deque_t* deque);

/*
 * Returns a copy of the deque.
 * On success, returns the copy of the deque.
 * On error, NULL is returned and "cerrno" is set appropriately.
 */
deque_t* deque_clone(deque_t* deque);

/*
 * Compares the deques d1 and d2.
 * If the parameters are not valid (NULL value...), returns -1.
 * Compares the data_size of the deques to check if the elements
 * have the same size. If the size are different, return the difference
 * of the data size (d1->data_size - d2->data_size).
 * Then, compares the size (= number of elements) of the deques. If different,
 * returns the difference of size (d1->size - d2->size).
 * Then, the function iterates over the elements.
 * If a different element is found (using compare function), returns the returned value
 * of compare(elem_of_d1, elem_of_d2).
 * If "compare" is NULL, the function uses memcmp(3).
 * If the deques are equals, returns 0.
 */
int deque_cmp(deque_t* d1, deque_t* d2, comparefct_t compare);

/*
 * Check if an element is in the deque using the function "compare".
 * If "compare" is NULL, the function uses memcmp(3).
 * Returns 1 if the element was found, 0 if not.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int deque_contains(deque_t* deque, void* e, comparefct_t compare);

/*
 * Count the occurences of an element in the deque.
 * Returns the number of occurences.
 * If "compare" is NULL, the function uses memcmp(3).
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int deque_count(deque_t* deque, void* e, comparefct_t compare);

/*
 * Destroys the deque.
 * Free all the elements and the deque. The pointer "deque" is set to NULL.
 */
void deque_destroy(deque_t** deque);

/*
 * Returns 1 if the deque is empty, 0 otherwise.
 * Returns -1 if the deque is NULL and "cerrno" is set appropriately.
 */
int deque_empty(deque_t* deque);

/*
 * Returns a deque containing all of the elements in the array in
 * proper sequence (from array[0] (head) to array[size - 1] (tail)).
 * At the end of your program, you must call "deque_destroy" to free the memory.
 */
deque_t* deque_fromarray(void* array, int size, size_t data_size);

/*
 * Creates a new deque.
 * On success, returns the deque.
 * On error, NULL is returned and "cerrno" is set appropriately.
 * At the end of your program, you must call "deque_destroy" to free the memory.
 */
deque_t* deque_new(size_t data_size);

/*
 * Retrieves, but does not remove, the first element of this deque, or returns NULL if this deque is empty.
 * On success, returns the first element of the deque.
 * On error, returns NULL and "cerrno" is set appropriately.
 */
void* deque_peekfirst(deque_t* deque);

/*
 * Retrieves, but does not remove, the last element of this deque, or returns NULL if this deque is empty.
 * On success, returns the last element of the deque (or NULL if the deque is empty).
 * On error, returns NULL and "cerrno" is set appropriately.
 */
void* deque_peeklast(deque_t* deque);

/*
 * Retrieves and removes the first element of this deque, or returns NULL if this deque is empty.
 * On success, returns the first element of the deque (or NULL if the deque is empty).
 * On error, returns NULL and "cerrno" is set appropriately.
 */
void* deque_pollfirst(deque_t* deque);

/*
 * Retrieves and removes the last element of this deque, or returns NULL if this deque is empty.
 * On success, returns the last element of the deque (or NULL if deque is empty).
 * On error, returns NULL and "cerrno" is set appropriately.
 */
void* deque_polllast(deque_t* deque);

/*
 * Removes all the occurrences of the specified element from the deque, if it is present.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns the number of elements removed, 0 if there is no such element.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int deque_removeall(deque_t* deque, void* e, comparefct_t compare);

/*
 * Removes the first element (head) of the deque.
 * Returns 1 if the first element is removed, 0 if the deque is empty.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int deque_removefirst(deque_t* deque);

/*
 * Removes the last element (tail) of the deque.
 * Returns 1 if the last element is removed, 0 if the deque is empty.
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int deque_removelast(deque_t* deque);

/*
 * Returns the size (number of elements) of the deque.
 * Returns -1 if the deque is NULL and "cerrno" is set appropriately.
 */
int deque_size(deque_t* deque);

/*
 * Returns an array containing all of the elements in the
 * deque in proper sequence (from first (head) to last (tail) element).
 * The new array is obtained with malloc(3) and can be freed with free(3).
 * On error, NULL is returned and "cerrno" is set appropriately.
 */
void* deque_toarray(deque_t* deque);

/*
 * Returns a string describing the deque.
 * The format is [elem1, elem2, elem3], where elemX
 * is a string returned by the function tostring(). The tostring function must returned an
 * pointer to a memory allocated with malloc(3) (the memory is freed in llist_tostring with free(3)).
 * The maximum length of the returned string is CBUFSIZE (defined in core.h).
 * On error, returns NULL and "cerrno" is set appropriately.
 */
char* deque_tostring(deque_t* deque, tostringfct_t tostring);

#ifdef __cplusplus
}
#endif

#endif // DEQUE_H

