#ifndef STACK_H
#define STACK_H

#include "collections/arraylist.h"
#include "collections/core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef arraylist_t stack_t;

/*
 * Appends the specified element to the end of this stack (memory copy).
 * NULL elements can not be inserted in the stack.
 * On success, returns 1. On error, 0 is returned and "cerrno" is set appropriately.
 */
int stack_add(stack_t* stack, void* e);

/*
 * Appends all of the elements in the stack "src" to the end of this stack "dest" (memory copy).
 * "dest" and "src" cannot be the same stack (cerrno is set to CERR_FORBIDDEN).
 * On success, returns 1. On error, 0 is returned and "cerrno" is set appropriately.
 */
int stack_addall(stack_t* dest, stack_t* src);

/*
 * Inserts the specified element "e" at the index "index" in the stack (memory copy).
 * NULL elements are not allowed.
 * On success, returns 1. On error, 0 is returned and "cerrno" is set appropriately.
 */
int stack_addat(stack_t *stack, int index, void* e);

/*
 * Inserts the specified element at the beginning of this stack (memory copy).
 * NULL elements are not allowed.
 * On success, returns 1. On error, 0 is returned and "cerrno" is set appropriately.
 */
int stack_addfirst(stack_t* stack, void* e);

/*
 * Appends all the elements to the end of the stack (memory copy).
 * The second element is the number of arguments after it.
 * Example : stack_addv(stack, 3, &i, &j, &k).
 * NULL elements are not allowed.
 * On success, returns the number of elements added.
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int stack_addv(stack_t* stack, int nbargs, ...);

/*
 * Removes all of the elements from this stack.
 */
void stack_clear(stack_t* stack);

/*
 * Returns a copy of the stack.
 * On success, returns the copy of the stack.
 * On error, NULL is returned and "cerrno" is set appropriately.
 */
stack_t* stack_clone(stack_t* stack);

/*
 * Compares the stacks l1 and l2.
 * If the parameters are not valid (NULL value...), returns -1.
 * Compares the data_size of the stacks to check if the elements
 * have the same size. If the size are different, return the difference
 * of the data size (l1->data_size - l2->data_size).
 * Then, compares the size (= number of elements) of the stacks. If different,
 * returns the difference of size (l1->size - l2->size).
 * Then, the function iterates over the elements.
 * If a different element is found (using compare function), returns the returned value
 * of compare(elem_of_l1, elem_of_l2).
 * If "compare" is NULL, the function uses memcmp(3).
 * If the stacks are equals, returns 0.
 */
int stack_cmp(stack_t* l1, stack_t* l2, comparefct_t compare);

/*
 * Check if an element is in the stack using the function "compare".
 * If "compare" is NULL, the function uses memcmp(3).
 * Returns 1 if the element was found, 0 if not.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int stack_contains(stack_t* stack, void* e, comparefct_t compare);

/*
 * Count the occurences of an element in the stack.
 * Returns the number of occurences.
 * If "compare" is NULL, the function uses memcmp(3).
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int stack_count(stack_t* stack, void* e, comparefct_t compare);

/*
 * Destroys the stack.
 * Free all the elements and the stack. The pointer "stack" is set to NULL.
 */
void stack_destroy(stack_t** stack);

/*
 * Tests if this stack is empty.
 * On success, returns 1 if the stack is empty, 0 otherwise.
 * On error, returns -1 and "cerrno" is set appropriately.
 */
int stack_empty(stack_t* stack);

/*
 * Fill the stack "dupstack" with all the duplicates element in the stack.
 * All the elements in "dupstack" are removed before the process.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns the number of different duplicated elements found (size of dupstack).
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int stack_finddup(stack_t* stack, stack_t* dupstack, comparefct_t compare);

/*
 * Returns a stack containing all of the elements in the array in
 * proper sequence (from array[0] to array[size - 1]).
 * At the end of your program, you must call "stack_destroy" to free the memory.
 */
stack_t* stack_fromarray(void* array, int size, size_t data_size);

/*
 * Returns the element at the specified index in the stack.
 * On success, returns the element. On error, NULL is returned and "cerrno" is set appropriately.
 */
void* stack_get(stack_t* stack, int index);

/*
 * Returns the index of the element "e".
 * On success, returns the index of the element, or -1 if the stack does not contain the element.
 * On error, returns -1 and "cerrno" is set appropriately.
 */
int stack_indexof(stack_t* stack, void* e, comparefct_t compare);

/*
 * Returns 1 if the stack is sorted (using the compare function, ascending order), 0 otherwise.
 * If "compare" is NULL, the function uses memcmp(3).
 * On error, -1 is returned and "cerrno" is set approprialety.
 */
int stack_issorted(stack_t* stack, comparefct_t compare);

/*
 * Creates a new stack.
 * On success, returns the stack.
 * On error, NULL is returned and "cerrno" is set appropriately.
 * At the end of your program, you must call "stack_destroy" to free the memory.
 */
stack_t* stack_new(size_t data_size);

/*
 * Looks at the object at the top of this stack without removing it from the stack.
 * On success, returns the last element of the stack.
 * On error, returns NULL and "cerrno" is set appropriately.
 */
void* stack_peek(stack_t* stack);

/*
 * Retrieves, but does not remove, the first element of the stack.
 * On success, returns the first element of the stack.
 * On error, returns NULL and "cerrno" is set appropriately.
 */
void* stack_peekfirst(stack_t* stack);

/*
 * Retrieves, but does not remove, the last element of the stack.
 * On success, returns the last element of the stack.
 * On error, returns NULL and "cerrno" is set appropriately.
 */
void* stack_peeklast(stack_t* stack);

/*
 * Removes the object at the top of this stack and returns that object as the value of this function.
 * On success, returns the last element of the stack (or NULL if the stack is empty).
 * ON error, returns NULL and "cerrno" is set appropriately.
 */
void* stack_pop(stack_t* stack);

/*
 * Pushes an item onto the top of this stack.
 * On success, returns a non-zero value.
 * On error, returns 0 and "cerrno" is set appropriately.
 */
int stack_push(stack_t* stack, void* e);

/*
 * Removes the first occurrence of the specified element from the stack, if it is present.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns 1 if the element was found and removed, 0 otherwise.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int stack_remove(stack_t* stack, void* e, comparefct_t compare);

/*
 * Removes all the occurrences of the specified element from the stack, if it is present.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns the number of elements removed, 0 if there is no such element.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int stack_removeall(stack_t* stack, void* e, comparefct_t compare);

/*
 * Removes the element at the specified index.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns 1.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int stack_removeat(stack_t* stack, int index);

/*
 * Removes the first element of the stack.
 * Returns 1 if the first element is removed, 0 if the stack is empty.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int stack_removefirst(stack_t* stack);

/*
 * Removes the last element of the stack.
 * Returns 1 if the last element is removed, 0 if the stack is empty.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int stack_removelast(stack_t* stack);

/*
 * Replaces the element at the specified position in this stack with the specified element.
 * If "previous" pointer param is NULL, the previous element is deleted.
 * If "previous" pointer param is a valid pointer (memory allocated), it contains the previous element.
 * On success, returns 1 and "previous" points to the previous element (if a valid memory pointer was passed).
 * On error, returns 0 and "cerrno" is set appropriately.
 */
int stack_setat(stack_t* stack, int index, void* e, void* previous);

/*
 * Sorts the stack (using the quick sort algorithm).
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int stack_sort(stack_t* stack, comparefct_t compare);

/*
 * Returns an array containing all of the elements in the
 * stack in proper sequence (from first to last element).
 * The new array is obtained with malloc(3) and can be freed with free(3).
 * On error, NULL is returned and "cerrno" is set appropriately.
 */
void* stack_toarray(stack_t* stack);

/*
 * Returns a string describing the stack.
 * The format is [elem1, elem2, elem3], where elemX
 * is a string returned by the function tostring(). The tostring function must returned an
 * pointer to a memory allocated with malloc(3) (the memory is freed in lstack_tostring with free(3)).
 * The maximum length of the returned string is CBUFSIZE (defined in core.h).
 * On error, returns NULL and "cerrno" is set appropriately.
 */
char* stack_tostring(stack_t* stack, tostringfct_t tostring);

#ifdef __cplusplus
}
#endif

#endif // STACK_H

