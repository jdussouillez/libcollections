#ifndef QUEUE_H
#define QUEUE_H

#include "collections/core.h"
#include "collections/deque.h"

/*
 * Iterates over the queue pointer "queuep" and run the instructions "instrs".
 * The "data" variable contains the data of the current element.
 */
#define QUEUE_FOREACH(queuep, instrs) DEQUE_FOREACH(queuep, instrs)

#ifdef __cplusplus
extern "C" {
#endif

typedef deque_t queue_t;

/*
 * Inserts the specified element into the queue (memory copy).
 * On success, returns 1.
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int queue_addall(queue_t* dest, queue_t* src);

/*
 * Inserts the specified element into the queue.
 * On success, returns 1.
 * On error, 0 is returned and "cerrno" is set appropriately.
 */
int queue_add(queue_t* queue, void* e);

/*
 * Removes all the elements in the queue.
 */
void queue_clear(queue_t* queue);

/*
 * Returns a copy of the queue.
 * On success, returns the copy of the queue.
 * On error, NULL is returned and "cerrno" is set appropriately.
 */
queue_t* queue_clone(queue_t* queue);

/*
 * Compares the queues d1 and d2.
 * If the parameters are not valid (NULL value...), returns -1.
 * Compares the data_size of the queues to check if the elements
 * have the same size. If the size are different, return the difference
 * of the data size (d1->data_size - d2->data_size).
 * Then, compares the size (= number of elements) of the queues. If different,
 * returns the difference of size (d1->size - d2->size).
 * Then, the function iterates over the elements.
 * If a different element is found (using compare function), returns the returned value
 * of compare(elem_of_d1, elem_of_d2).
 * If "compare" is NULL, the function uses memcmp(3).
 * If the queues are equals, returns 0.
 */
int queue_cmp(queue_t* d1, queue_t* d2, comparefct_t compare);

/*
 * Check if an element is in the queue using the function "compare".
 * If "compare" is NULL, the function uses memcmp(3).
 * Returns 1 if the element was found, 0 if not.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int queue_contains(queue_t* queue, void* e, comparefct_t compare);

/*
 * Count the occurences of an element in the queue.
 * Returns the number of occurences.
 * If "compare" is NULL, the function uses memcmp(3).
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int queue_count(queue_t* queue, void* e, comparefct_t compare);

/*
 * Destroys the queue.
 * Free all the elements and the queue. The pointer "queue" is set to NULL.
 */
void queue_destroy(queue_t** queue);

/*
 * Returns 1 if the queue is empty, 0 otherwise.
 * Returns -1 if the queue is NULL and "cerrno" is set appropriately.
 */
int queue_empty(queue_t* queue);

/*
 * Returns a queue containing all of the elements in the array in
 * proper sequence (from array[0] (head) to array[size - 1] (tail)).
 * At the end of your program, you must call "queue_destroy" to free the memory.
 */
queue_t* queue_fromarray(void* array, int size, size_t data_size);

/*
 * Creates a new queue.
 * On success, returns the queue.
 * On error, NULL is returned and "cerrno" is set appropriately.
 * At the end of your program, you must call "queue_destroy" to free the memory.
 */
queue_t* queue_new(size_t data_size);

/*
 * Retrieves, but does not remove, the head of this queue, or returns NULL if this queue is empty.
 * On success, returns the first element of the queue.
 * On error, returns NULL and "cerrno" is set appropriately.
 */
void* queue_peek(queue_t* queue);

/*
 * Retrieves and removes the head of this queue, or returns NULL if this queue is empty.
 * On success, returns the first element of the queue (or NULL if the queue is empty).
 * On error, returns NULL and "cerrno" is set appropriately.
 */
void* queue_poll(queue_t* queue);

/*
 * Removes all the occurrences of the specified element from the queue, if it is present.
 * If "compare" is NULL, the function uses memcmp(3).
 * On success, returns the number of elements removed, 0 if there is no such element.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int queue_removeall(queue_t* queue, void* e, comparefct_t compare);

/*
 * Retrieves and removes the head of this queue.
 * Returns 1 if the first element is removed, 0 if the queue is empty.
 * On error, -1 is returned and "cerrno" is set appropriately.
 */
int queue_remove(queue_t* queue);

/*
 * Returns the size (number of elements) of the queue.
 * Returns -1 if the queue is NULL and "cerrno" is set appropriately.
 */
int queue_size(queue_t* queue);

/*
 * Returns an array containing all of the elements in the
 * queue in proper sequence (from first (head) to last (tail) element).
 * The new array is obtained with malloc(3) and can be freed with free(3).
 * On error, NULL is returned and "cerrno" is set appropriately.
 */
void* queue_toarray(queue_t* queue);

/*
 * Returns a string describing the queue.
 * The format is [elem1, elem2, elem3], where elemX
 * is a string returned by the function tostring(). The tostring function must returned an
 * pointer to a memory allocated with malloc(3) (the memory is freed in llist_tostring with free(3)).
 * The maximum length of the returned string is CBUFSIZE (defined in core.h).
 * On error, returns NULL and "cerrno" is set appropriately.
 */
char* queue_tostring(queue_t* queue, tostringfct_t tostring);

#ifdef __cplusplus
}
#endif

#endif // QUEUE_H

