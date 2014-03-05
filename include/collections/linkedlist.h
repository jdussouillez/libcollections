#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "collections/core.h"

/*
 * Iterates over the linked list "listp" and run the instructions "instrs".
 * The "data" variable contains the data of the current node.
 */
#define LLIST_FOREACH(listp, instrs) do {	\
    void* data;					\
    linkedlistnode_t* node = listp->head;	\
    while (node != NULL) {			\
      data = node->data;			\
      instrs;					\
      node = node->next;			\
    }						\
  } while (0)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s_linkedlistnode {
  void* data;
  struct s_linkedlistnode* next;
} linkedlistnode_t;

typedef struct {
  int size; // Size of the list
  size_t data_size; // Size of the elements in byte
  linkedlistnode_t* head; // Pointer to the head of the list
  linkedlistnode_t* tail; // Pointer to the tail of the list
} linkedlist_t;

/*
 * Appends the specified element to the end of this list (memory copy).
 * On success, returns 1. On error, 0 is returned and "cerrno" is set appropriately.
 */
int llist_add(linkedlist_t* list, void* e);

/*
 * Appends all of the elements in the list "src" to the end of this list "dest" (memory copy).
 * "dest" and "src" cannot be the same list (cerrno is set to CERR_FORBIDDEN).
 * On success, returns 1. On error, 0 is returned and "cerrno" is set appropriately.
 */
int llist_addall(linkedlist_t* dest, linkedlist_t* src);

/*
 * Inserts the specified element at the beginning of this list (memory copy).
 * On success, returns 1. On error, 0 is returned and "cerrno" is set appropriately.
 */
int llist_addfirst(linkedlist_t* list, void* e);

/*
 * Appends all the elements to the end of the list (memory copy).
 * The second element is the number of arguments after it.
 * Example : llist_addv(list, 3, &i, &j, &k).
 * On success, returns the number of elements added.
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int llist_addv(linkedlist_t* list, int nbargs, ...);

/*
 * Removes all of the elements from this list.
 */
void llist_clear(linkedlist_t* list);

/*
 * Returns a copy of the list.
 * On success, returns the copy of the list.
 * On error, NULL is returned and "cerrno" is set appropriately.
 */
linkedlist_t* llist_clone(linkedlist_t* list);

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
int llist_cmp(linkedlist_t* l1, linkedlist_t* l2, comparefct_t compare);

/*
 * Check if an element is in the list using the function "compare".
 * If "compare" is NULL, the function uses memcmp(3).
 * Returns 1 if the element was found, 0 if not.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int llist_contains(linkedlist_t* list, void* e, comparefct_t compare);

/*
 * Count the occurences of an element in the list.
 * Returns the number of occurences.
 * If "compare" is NULL, the function uses memcmp(3).
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int llist_count(linkedlist_t* list, void* e, comparefct_t compare);

/*
 * Destroys the list.
 * Free all the elements and the list. The pointer "list" is set to NULL.
 */
void llist_destroy(linkedlist_t** list);

/*
 * Fill the list "duplist" with all the duplicates element in the list.
 * All the elements in "duplist" are removed before the process.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns the number of different duplicated elements found (size of duplist).
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int llist_finddup(linkedlist_t* list, linkedlist_t* duplist, comparefct_t compare);

/*
 * Returns a list containing all of the elements in the array in
 * proper sequence (from array[0] to array[size - 1]).
 * At the end of your program, you must call "llist_destroy" to free the memory.
 */
linkedlist_t* llist_fromarray(void* array, int size, size_t data_size);

/*
 * Returns 1 if the list is sorted (using the compare function, ascending order), 0 otherwise.
 * If "compare" is NULL, the function uses memcmp(3).
 * On error, -1 is returned and "cerrno" is set approprialety.
 */
int llist_issorted(linkedlist_t* list, comparefct_t compare);

/*
 * Creates a new list.
 * On success, returns the list.
 * On error, NULL is returned and "cerrno" is set appropriately.
 * At the end of your program, you must call "llist_destroy" to free the memory.
 */
linkedlist_t* llist_new(size_t data_size);

/*
 * Retrieves, but does not remove, the first element of the list.
 * Returns NULL if the list is empty.
 */
void* llist_peekfirst(linkedlist_t* list);

/*
 * Retrieves, but does not remove, the last element of the list.
 * Returns NULL if the list is empty.
 */
void* llist_peeklast(linkedlist_t* list);

/*
 * Removes the first occurrence of the specified element from the list, if it is present.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns 1 if the element was found and removed, 0 otherwise.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int llist_remove(linkedlist_t* list, void* e, comparefct_t compare);

/*
 * Removes all the occurrences of the specified element from the list, if it is present.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns the number of elements removed, 0 if there is no such element.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int llist_removeall(linkedlist_t* list, void* e, comparefct_t compare);

/*
 * Removes the first element of the list.
 * Returns 1 if the first element is removed, 0 if the list is empty.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int llist_removefirst(linkedlist_t* list);

/*
 * Removes the last element of the list.
 * Returns 1 if the last element is removed, 0 if the list is empty.
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int llist_removelast(linkedlist_t* list);

/*
 * Sorts the list (using the quick sort algorithm).
 * It transforms the list into an array, clears the list,
 * sorts the array (using qsort(3)) and finally fills the list.
 * If "compare" function can not be NULL (error CERR_NULLVALUE).
 * On success, returns 1.
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int llist_sort(linkedlist_t* list, comparefct_t compare);

/*
 * Returns an array containing all of the elements in the
 * list in proper sequence (from first to last element).
 * The new array is obtained with malloc(3) and can be freed with free(3).
 * On error, NULL is returned and "cerrno" is set appropriately.
 */
void* llist_toarray(linkedlist_t* list);

/*
 * Returns a string describing the list.
 * The format is [elem1, elem2, elem3], where elem3
 * is a string returned by the function tostring(). The tostring function must returned an
 * pointer to a memory allocated with malloc(3) (the memory is freed in llist_tostring with free(3)).
 * The maximum length of the returned string is BUFSIZE (defined in core.h).
 * On error, returns NULL and "cerrno" is set appropriately.
 */
char* llist_tostring(linkedlist_t* list, tostringfct_t tostring);

#ifdef __cplusplus
}
#endif

#endif // LINKEDLIST_H
