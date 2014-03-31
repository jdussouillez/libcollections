#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "collections/core.h"

#define ALIST_FOREACH(alistp, instrs) do { \
    void* data;				   \
    int i;				   \
    for (i = 0; i < alistp->size; i++) {   \
      data = alistp->data[i];		   \
      instrs;				   \
    }					   \
  } while (0)


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int size; // Size of the array list
  int data_size; // Size of the elements in byte
  void** data; // Array of data
} arraylist_t;

/*
 * Appends the specified element to the end of this list (memory copy).
 * On success, returns 1. On error, 0 is returned and "cerrno" is set appropriately.
 */
int alist_add(arraylist_t* list, void* e);

/*
 * Appends all of the elements in the list "src" to the end of this list "dest" (memory copy).
 * "dest" and "src" cannot be the same list (cerrno is set to CERR_FORBIDDEN).
 * On success, returns 1. On error, 0 is returned and "cerrno" is set appropriately.
 */
int alist_addall(arraylist_t* dest, arraylist_t* src);

/*
 * Inserts the specified element "e" at the index "index" in the list (memory copy).
 * On success, returns 1. On error, 0 is returned and "cerrno" is set appropriately.
 */
int alist_addat(arraylist_t *list, int index, void* e);

/*
 * Inserts the specified element at the beginning of this list (memory copy).
 * On success, returns 1. On error, 0 is returned and "cerrno" is set appropriately.
 */
int alist_addfirst(arraylist_t* list, void* e);

/*
 * Appends all the elements to the end of the list (memory copy).
 * The second element is the number of arguments after it.
 * Example : alist_addv(list, 3, &i, &j, &k).
 * On success, returns the number of elements added.
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int alist_addv(arraylist_t* list, int nbargs, ...);

/*
 * Removes all of the elements from this list.
 */
void alist_clear(arraylist_t* list);

/*
 * Returns a copy of the list.
 * On success, returns the copy of the list.
 * On error, NULL is returned and "cerrno" is set appropriately.
 */
arraylist_t* alist_clone(arraylist_t* list);

/*
 * Compares the lists l1 and l2.
 * If the parameters are not valid (NULL value...), returns -1.
 * Compares the data_size of the lists to check if the elements
 * have the same size. If the size are different, return the difference
 * of the data size (l1->data_size - l2->data_size).
 * Then, compares the size (= number of elements) of the lists. If different,
 * returns the difference of size (l1->size - l2->size).
 * Then, the function iterates over the elements.
 * If a different element is found (using compare function), returns the returned value
 * of compare(elem_of_l1, elem_of_l2).
 * If "compare" is NULL, the function uses memcmp(3).
 * If the lists are equals, returns 0.
 */
int alist_cmp(arraylist_t* l1, arraylist_t* l2, comparefct_t compare);

/*
 * Check if an element is in the list using the function "compare".
 * If "compare" is NULL, the function uses memcmp(3).
 * Returns 1 if the element was found, 0 if not.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int alist_contains(arraylist_t* list, void* e, comparefct_t compare);

/*
 * Count the occurences of an element in the list.
 * Returns the number of occurences.
 * If "compare" is NULL, the function uses memcmp(3).
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int alist_count(arraylist_t* list, void* e, comparefct_t compare);

/*
 * Destroys the list.
 * Free all the elements and the list. The pointer "list" is set to NULL.
 */
void alist_destroy(arraylist_t** list);

/*
 * Fill the list "duplist" with all the duplicates element in the list.
 * All the elements in "duplist" are removed before the process.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns the number of different duplicated elements found (size of duplist).
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int alist_finddup(arraylist_t* list, arraylist_t* duplist, comparefct_t compare);

/*
 * Returns a list containing all of the elements in the array in
 * proper sequence (from array[0] to array[size - 1]).
 * At the end of your program, you must call "alist_destroy" to free the memory.
 */
arraylist_t* alist_fromarray(void* array, int size, size_t data_size);

/*
 * Returns the element at the specified index in the list.
 * On success, returns the element. On error, NULL is returned and "cerrno" is set appropriately.
 */
void* alist_get(arraylist_t* list, int index);

/*
 * Returns the index of the element "e".
 * On success, returns the index of the element, or -1 if the list does not contain the element.
 * On error, returns -1 and "cerrno" is set appropriately.
 */
int alist_indexof(arraylist_t* list, void* e, comparefct_t compare);

/*
 * Returns 1 if the list is sorted (using the compare function, ascending order), 0 otherwise.
 * If "compare" is NULL, the function uses memcmp(3).
 * On error, -1 is returned and "cerrno" is set approprialety.
 */
int alist_issorted(arraylist_t* list, comparefct_t compare);

/*
 * Creates a new list.
 * On success, returns the list.
 * On error, NULL is returned and "cerrno" is set appropriately.
 * At the end of your program, you must call "alist_destroy" to free the memory.
 */
arraylist_t* alist_new(size_t data_size);

/*
 * Retrieves, but does not remove, the first element of the list.
 * Returns NULL if the list is empty.
 */
void* alist_peekfirst(arraylist_t* list);

/*
 * Retrieves, but does not remove, the last element of the list.
 * Returns NULL if the list is empty.
 */
void* alist_peeklast(arraylist_t* list);

/*
 * Removes the first occurrence of the specified element from the list, if it is present.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns 1 if the element was found and removed, 0 otherwise.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int alist_remove(arraylist_t* list, void* e, comparefct_t compare);

/*
 * Removes all the occurrences of the specified element from the list, if it is present.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns the number of elements removed, 0 if there is no such element.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int alist_removeall(arraylist_t* list, void* e, comparefct_t compare);

/*
 * Removes the element at the specified index.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns 1.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int alist_removeat(arraylist_t* list, int index);

/*
 * Removes the first element of the list.
 * Returns 1 if the first element is removed, 0 if the list is empty.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int alist_removefirst(arraylist_t* list);

/*
 * Removes the last element of the list.
 * Returns 1 if the last element is removed, 0 if the list is empty.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int alist_removelast(arraylist_t* list);

/*
 * Sorts the list (using the quick sort algorithm).
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int alist_sort(arraylist_t* list, comparefct_t compare);

/*
 * Returns an array containing all of the elements in the
 * list in proper sequence (from first to last element).
 * The new array is obtained with malloc(3) and can be freed with free(3).
 * On error, NULL is returned and "cerrno" is set appropriately.
 */
void* alist_toarray(arraylist_t* list);

/*
 * Returns a string describing the list.
 * The format is [elem1, elem2, elem3], where elem3
 * is a string returned by the function tostring(). The tostring function must returned an
 * pointer to a memory allocated with malloc(3) (the memory is freed in llist_tostring with free(3)).
 * The maximum length of the returned string is BUFSIZE (defined in core.h).
 * On error, returns NULL and "cerrno" is set appropriately.
 */
char* alist_tostring(arraylist_t* list, tostringfct_t tostring);

#ifdef __cplusplus
}
#endif

#endif // ARRAYLIST_H

