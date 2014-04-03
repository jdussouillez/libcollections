#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collections/arraylist.h"
#include "collections/errors.h"
#include "collections/linkedlist.h"
#include "collections/stack.h"

#include "CUnit/Basic.h"

#define DEBUG 0

/*
 *
 * Data for the tests
 *
 */
int compare_int(void* i1, void* i2) {
  return *((int*) i1) - *((int*) i2);
}

char* inttostring(void* i) {
  char* str;
  if ((str = malloc(16 * sizeof(char))) == NULL)
    return NULL;
  snprintf(str, 16, "%d", *((int*) i));
  return str;
}

int i, size_int = sizeof(int);
linkedlist_t* llist;
arraylist_t* alist;
stack_t* stack;

/*
 * CUnit Test Suite
 */
int init_suite(void) {
  return 0;
}

int clean_suite(void) {
  return 0;
}

/*
***************************************************
 **************** Linked list suite ***************
 **************************************************
 */
/*
 * Test-to-pass
 */
void linkedlist_new_destroy_TTP() {
  llist = llist_new(size_int);
  CU_ASSERT_PTR_NOT_NULL(llist);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&llist);
  CU_ASSERT_PTR_NULL(llist);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
}

void linkedlist_add_TTP() {
  llist = llist_new(size_int);
  // Add an element when the list is empty
  i = 1;
  CU_ASSERT_TRUE(llist_add(llist, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(llist->size, 1);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(llist), &i), 0);
  // Add an element when the list is not empty
  i = 2;
  CU_ASSERT_TRUE(llist_add(llist, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(llist->size, 2);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(llist), &i), 0);
  llist_destroy(&llist);
}

void linkedlist_addall_TTP() {
  linkedlist_t* llist2 = llist_new(size_int);
  i = 1;
  llist_add(llist2, &i);
  i = 2;
  llist_add(llist2, &i);
  llist = llist_new(size_int);
  // llist = [], llist2 = [1, 2]
  llist_addall(llist, llist2);
  // llist = [1, 2], llist2 = [1, 2]
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(llist->size, 2);
  // Check elemens of the list
  // i = 2
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(llist), &i), 0);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(llist), &i), 0);
  llist_destroy(&llist2);
  llist_destroy(&llist);
}

void linkedlist_addfirst_TTP() {
  llist = llist_new(size_int);
  // Add an element at the beginning of an empty list
  i = 1;
  CU_ASSERT_TRUE(llist_addfirst(llist, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(llist), &i), 0);
  // Add an element at the beginning of a non-empty list
  i = 2;
  CU_ASSERT_TRUE(llist_addfirst(llist, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(llist), &i), 0);
  llist_destroy(&llist);
}

void linkedlist_addv_TTP() {
  int j;
  llist = llist_new(size_int);
  // Add 3 elements
  i = 1;
  j = 2;
  CU_ASSERT_EQUAL(llist_addv(llist, 3, &i, &i, &j), 3);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(llist->size, 3);
  // llist = [1, 1, 2]
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(llist), &i), 0);
  CU_ASSERT_EQUAL(compare_int(llist->head->next->data, &i), 0);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(llist), &j), 0);
  llist_destroy(&llist);
}

void linkedlist_clear_TTP() {
  llist = llist_new(size_int);
  i = 1;
  // Add 3 elements and clear the list
  llist_addv(llist, 3, &i, &i, &i);
  llist_clear(llist);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(llist->size, 0);
  llist_destroy(&llist);
}

void linkedlist_clone_TTP() {
  linkedlist_t* llist2;
  llist = llist_new(size_int);
  i = 1;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  llist2 = llist_clone(llist);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(llist2);
  CU_ASSERT_EQUAL(llist->size, llist2->size);
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(llist), llist_peekfirst(llist2)), 0);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(llist), llist_peeklast(llist2)), 0);
  llist_destroy(&llist);
  llist_destroy(&llist2);
}

void linkedlist_cmp_TTP() {
  linkedlist_t* llist2 = llist_new(size_int);
  // Different size of data
  llist = llist_new(sizeof(float));
  CU_ASSERT_EQUAL(llist_cmp(llist, llist2, compare_int), (sizeof(float) - size_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&llist);
  // Different size
  llist = llist_new(size_int); // llist = [] (size = 0)
  i = 3;
  llist_add(llist2, &i); // llist2 = [3] (size = 1)
  CU_ASSERT_EQUAL(llist_cmp(llist, llist2, compare_int), -1); // 0 - 1 = -1
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Different elements
  i = 7;
  llist_add(llist, &i); // llist = [7], llist2 = [3]
  CU_ASSERT_EQUAL(llist_cmp(llist2, llist, compare_int), (3 - 7));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Same lists
  llist_add(llist2, &i); // llist2 = [3, 7]
  i = 3;
  llist_addfirst(llist, &i); // llist = [3, 7]
  CU_ASSERT_EQUAL(llist_cmp(llist, llist2, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&llist);
  llist_destroy(&llist2);
}

void linkedlist_contains_TTP() {
  llist = llist_new(size_int);
  // The list is empty (so 4 is not found)
  i = 4;
  CU_ASSERT_FALSE(llist_contains(llist, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // llist = [1, 2]
  i = 1;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  // The list does not contain 4
  i = 4;
  CU_ASSERT_FALSE(llist_contains(llist, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // The list contains 2
  i = 2;
  CU_ASSERT_TRUE(llist_contains(llist, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&llist);
}

void linkedlist_count_TTP() {
  llist = llist_new(size_int);
  i = 0;
  llist_add(llist, &i);
  i = 1;
  llist_addv(llist, 2, &i, &i);
  // llist = [0, 1, 1]
  // The list contains twice the element 1
  CU_ASSERT_EQUAL(llist_count(llist, &i, compare_int), 2);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 2;
  // The list does not contain an element 2
  CU_ASSERT_EQUAL(llist_count(llist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&llist);
}

void linkedlist_empty_TTP() {
  llist = llist_new(size_int);
  CU_ASSERT_EQUAL(llist_empty(llist), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 1;
  llist_add(llist, &i);
  CU_ASSERT_EQUAL(llist_empty(llist), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&llist);
}

void linkedlist_finddup_TTP() {
  linkedlist_t* dupllist = llist_new(size_int);
  llist = llist_new(size_int);
  i = 1;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  i = 3;
  llist_add(llist, &i);
  // No duplicates (llist = [1, 2, 3] --> dupllist = [], returned value = 0)
  CU_ASSERT_EQUAL(llist_finddup(llist, dupllist, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Duplicates (llist = [1, 2, 3, 3, 3, 1] --> dupllist = [1, 3], returned value = 2)
  llist_add(llist, &i);
  llist_add(llist, &i);
  i = 1;
  llist_add(llist, &i);
  CU_ASSERT_EQUAL(llist_finddup(llist, dupllist, compare_int), 2);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Check dupllist elements
  CU_ASSERT_EQUAL(dupllist->size, 2);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(dupllist), &i), 0);
  i = 3;
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(dupllist), &i), 0);
  llist_destroy(&dupllist);
  llist_destroy(&llist);
}

void linkedlist_fromarray_TTP() {
  int i = 1, j = 2;
  int array[2] = {i, j};
  llist = llist_fromarray(array, 2, sizeof(int));
  CU_ASSERT_PTR_NOT_NULL(llist);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(llist->size, 2);
  // Check values
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(llist), &i), 0);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(llist), &j), 0);
  llist_destroy(&llist);
}

void linkedlist_issorted_TTP() {
  llist = llist_new(size_int);
  i = 1;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  i = 3;
  llist_add(llist, &i);
  // On a sorted llist ([1, 2, 3])
  CU_ASSERT_EQUAL(llist_issorted(llist, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 1;
  llist_add(llist, &i);
  i = -5;
  llist_add(llist, &i);
  // On a non-sorted llist ([1, 2, 3, 1, -5])
  CU_ASSERT_EQUAL(llist_issorted(llist, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&llist);
}

void linkedlist_peekfirst_TTP() {
  llist = llist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(llist_peekfirst(llist), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a list of 2 elements
  i = 1;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(llist), &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&llist);
}

void linkedlist_peeklast_TTP() {
  llist = llist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(llist_peeklast(llist), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a list of 2 elements
  i = 1;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(llist), &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&llist);
}

void linkedlist_remove_TTP() {
  llist = llist_new(size_int);
  // Remove an element which is in the list
  i = 1;
  llist_add(llist, &i);
  CU_ASSERT_EQUAL(llist->size, 1);
  CU_ASSERT_EQUAL(llist_remove(llist, &i, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(llist->size, 0);
  // Remove an element in an empty list
  CU_ASSERT_EQUAL(llist_remove(llist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove an element which is not in the list (non empty list)
  llist_add(llist, &i);
  i = 2;
  CU_ASSERT_EQUAL(llist->size, 1);
  CU_ASSERT_EQUAL(llist_remove(llist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(llist->size, 1);
  llist_destroy(&llist);
}

void linkedlist_removeall_TTP() {
  llist = llist_new(size_int);
  // Remove all the elements 1 in the list (2 elements removed)
  i = 2;
  llist_add(llist, &i);
  i = 1;
  llist_add(llist, &i);
  llist_add(llist, &i);
  // llist = [2, 1, 1]
  CU_ASSERT_EQUAL(llist->size, 3);
  CU_ASSERT_EQUAL(llist_removeall(llist, &i, compare_int), 2);
  CU_ASSERT_EQUAL(llist->size, 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove all the elements 1 in the list (no elements removed), llist = [2]
  i = 1;
  CU_ASSERT_EQUAL(llist_removeall(llist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(llist->size, 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove all the elements of the list
  llist_clear(llist);
  i = 1;
  llist_addv(llist, 3, &i, &i, &i);
  CU_ASSERT_EQUAL(llist->size, 3);
  CU_ASSERT_EQUAL(llist_removeall(llist, &i, compare_int), 3);
  CU_ASSERT_EQUAL(llist->size, 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove on an empty list
  llist_clear(llist);
  CU_ASSERT_EQUAL(llist_removeall(llist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&llist);
}

void linkedlist_removefirst_TTP() {
  llist = llist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(llist_removefirst(llist), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a non-empty list
  i = 1;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  CU_ASSERT_EQUAL(llist_removefirst(llist), 1);
  CU_ASSERT_EQUAL(llist->size, 1);
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(llist), &i), 0);
  llist_destroy(&llist);
}

void linkedlist_removelast_TTP() {
  llist = llist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(llist_removelast(llist), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a non-empty list
  i = 1;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  CU_ASSERT_EQUAL(llist_removelast(llist), 1);
  CU_ASSERT_EQUAL(llist->size, 1);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(llist), &i), 0);
  llist_destroy(&llist);
}

void linkedlist_size_TTP() {
  llist = llist_new(size_int);
  CU_ASSERT_EQUAL(llist_size(llist), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 1;
  llist_add(llist, &i);
  CU_ASSERT_EQUAL(llist_size(llist), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&llist);
}

void linkedlist_sort_TTP() {
  llist = llist_new(size_int);
  // Sort an empty list
  CU_ASSERT_EQUAL(llist_sort(llist, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Already sorted, llist = [1, 2] --> llist = [1, 2]
  i = 1;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  CU_ASSERT_EQUAL(llist_sort(llist, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(llist), &i), 0);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(llist), &i), 0);
  // Sorted by llist_sort(), llist = [1, 2, -5, 9, 2] --> llist = [-5, 1, 2, 2, 9]
  i = -5;
  llist_add(llist, &i);
  i = 9;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  CU_ASSERT_EQUAL(llist_sort(llist, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_TRUE(llist_issorted(llist, compare_int));
  llist_destroy(&llist);
}

void linkedlist_toarray_TTP() {
  void* array = NULL;
  llist = llist_new(size_int);
  // Empty list
  array = llist_toarray(llist);
  CU_ASSERT_PTR_NOT_NULL(array);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  free(array);
  array = NULL;
  // Non-empty list
  i = 1;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  array = llist_toarray(llist);
  CU_ASSERT_PTR_NOT_NULL(array);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Check values
  i = 1;
  CU_ASSERT_EQUAL(compare_int(array, &i), 0);
  i = 2;
  CU_ASSERT_EQUAL(compare_int(array + llist->data_size, &i), 0);
  free(array);
  llist_destroy(&llist);
}

void linkedlist_tostring_TTP() {
  char* str;
  llist = llist_new(size_int);
  // On an empty list
  str = llist_tostring(llist, inttostring);
  CU_ASSERT_PTR_NOT_NULL(str);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(strcmp(str, "[]"), 0);
  free(str);
  str = NULL;
  // On a non-empty list
  i = 1;
  llist_add(llist, &i);
  i = 2;
  llist_add(llist, &i);
  i = 4;
  llist_add(llist, &i);
  i = 8;
  llist_add(llist, &i);
  i = 16;
  llist_add(llist, &i);
  str = llist_tostring(llist, inttostring);
  CU_ASSERT_PTR_NOT_NULL(str);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(strcmp(str, "[1, 2, 4, 8, 16]"), 0);
  free(str);
  llist_destroy(&llist);
}

/*
 ********************************
 * Test-to-fail
 ********************************
 */
void linkedlist_destroy_TTF() {
  // NULL list and NULL pointer
  llist = NULL;
  llist_destroy(&llist);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_add_TTF() {
  llist = llist_new(size_int);
  // Add a NULL element
  CU_ASSERT_FALSE(llist_add(llist, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(llist->size, 0);
  // Add an element to NULL
  i = 1;
  CU_ASSERT_FALSE(llist_add(NULL, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Both parameters are NULL
  CU_ASSERT_FALSE(llist_add(NULL, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&llist);
}

void linkedlist_addall_TTF() {
  llist = llist_new(size_int);
  // Source list is NULL
  CU_ASSERT_FALSE(llist_addall(llist, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Destination list is NULL
  CU_ASSERT_FALSE(llist_addall(NULL, llist));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Both parameters are NULL
  CU_ASSERT_FALSE(llist_addall(NULL, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Addall on the same list is forbidden (infinite loop if not forbidden)
  CU_ASSERT_FALSE(llist_addall(llist, llist));
  CU_ASSERT_EQUAL(cerrno, CERR_FORBIDDEN);
  llist_destroy(&llist);
}

void linkedlist_addfirst_TTF() {
  llist = llist_new(size_int);
  // Add NULL at the beginning of the list
  CU_ASSERT_FALSE(llist_addfirst(llist, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Add a value at the beginning of NULL
  i = 1;
  CU_ASSERT_FALSE(llist_addfirst(NULL, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&llist);
}

void linkedlist_addv_TTF() {
  // NULL list
  i = 1;
  CU_ASSERT_EQUAL(llist_addv(NULL, 1, &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  llist = llist_new(size_int);
  CU_ASSERT_EQUAL(llist_addv(llist, 3, &i, NULL, &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(llist->size, 0);
  llist_destroy(&llist);
}

void linkedlist_clear_TTF() {
  // NULL list
  llist_clear(NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_clone_TTF() {
  // NULL list
  CU_ASSERT_PTR_NULL(llist_clone(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_cmp_TTF() {
  linkedlist_t* llist2 = llist_new(size_int);
  llist = llist_new(size_int);
  // NULL list 1
  CU_ASSERT_EQUAL(llist_cmp(NULL, llist2, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL list 2
  CU_ASSERT_EQUAL(llist_cmp(llist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&llist);
  llist_destroy(&llist2);
}

void linkedlist_contains_TTF() {
  // NULL list
  i = 2;
  CU_ASSERT_FALSE(llist_contains(NULL, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  llist = llist_new(size_int);
  CU_ASSERT_FALSE(llist_contains(llist, NULL, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&llist);
}

void linkedlist_count_TTF() {
  i = 1;
  llist = llist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(llist_count(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(llist_count(llist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&llist);
}

void linkedlist_empty_TTF() {
  CU_ASSERT_EQUAL(llist_empty(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_finddup_TTF() {
  linkedlist_t* dupllist = llist_new(size_int);
  i = 1;
  llist = llist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(llist_finddup(NULL, llist, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL duplicate list
  CU_ASSERT_EQUAL(llist_finddup(llist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&llist);
  llist_destroy(&dupllist);
}

void linkedlist_fromarray_TTF() {
  // NULL list
  CU_ASSERT_PTR_NULL(llist_fromarray(NULL, 2, sizeof(int)));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_issorted_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(llist_issorted(NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&llist);
}

void linkedlist_peekfirst_TTF() {
  // NULL list
  CU_ASSERT_PTR_NULL(llist_peekfirst(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_peeklast_TTF() {
  // NULL list
  CU_ASSERT_PTR_NULL(llist_peekfirst(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_remove_TTF() {
  i = 1;
  llist = llist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(llist_remove(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(llist_remove(llist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&llist);
}

void linkedlist_removeall_TTF() {
  i = 1;
  llist = llist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(llist_removeall(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(llist_removeall(llist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&llist);
}

void linkedlist_removefirst_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(llist_removefirst(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_removelast_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(llist_removelast(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_size_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(llist_size(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_sort_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(llist_sort(NULL, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL compare function
  llist = llist_new(size_int);
  CU_ASSERT_EQUAL(llist_sort(llist, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&llist);
}

void linkedlist_toarray_TTF() {
  // NULL list
  CU_ASSERT_PTR_NULL(llist_toarray(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_tostring_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(llist_tostring(NULL, inttostring), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL tostring function
  llist = llist_new(size_int);
  CU_ASSERT_EQUAL(llist_tostring(llist, NULL), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&llist);
}


/*
***************************************************
 **************** Array list suite ***************
 **************************************************
 */
/*
 * Test-to-pass
 */
void arraylist_new_destroy_TTP() {
  alist = alist_new(size_int);
  CU_ASSERT_PTR_NOT_NULL(alist);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
  CU_ASSERT_PTR_NULL(alist);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
}

void arraylist_add_TTP() {
  alist = alist_new(size_int);
  // Add an element when the list is empty
  i = 1;
  CU_ASSERT_TRUE(alist_add(alist, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 1);
  CU_ASSERT_EQUAL(compare_int(alist_peeklast(alist), &i), 0);
  // Add an element when the list is not empty
  i = 2;
  CU_ASSERT_TRUE(alist_add(alist, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 2);
  CU_ASSERT_EQUAL(compare_int(alist_peeklast(alist), &i), 0);
  alist_destroy(&alist);
}

void arraylist_addall_TTP() {
  arraylist_t* alist2 = alist_new(size_int);
  i = 1;
  alist_add(alist2, &i);
  i = 2;
  alist_add(alist2, &i);
  alist = alist_new(size_int);
  // alist = [], alist2 = [1, 2]
  alist_addall(alist, alist2);
  // alist = [1, 2], alist2 = [1, 2]
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 2);
  // Check elemens of the list
  // i = 2
  CU_ASSERT_EQUAL(compare_int(alist_peeklast(alist), &i), 0);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(alist_peekfirst(alist), &i), 0);
  alist_destroy(&alist2);
  alist_destroy(&alist);
}

void arraylist_addat_TTP() {
  alist = alist_new(size_int);
  // Add an element at index 0 when the list is empty
  i = 1;
  CU_ASSERT_TRUE(alist_addat(alist, 0, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 1);
  CU_ASSERT_EQUAL(compare_int(alist_get(alist, 0), &i), 0);
  // Add an element before
  // alist = [1] --> alist = [2, 1]
  i = 2;
  CU_ASSERT_TRUE(alist_addat(alist, 0, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 2);
  CU_ASSERT_EQUAL(compare_int(alist_get(alist, 0), &i), 0);
  // Add an element after
  // alist = [2, 1] --> alist = [2, 1, 3]
  i = 3;
  CU_ASSERT_TRUE(alist_addat(alist, 2, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 3);
  CU_ASSERT_EQUAL(compare_int(alist_get(alist, 2), &i), 0);
  // Add an element in the middle (at index 1)
  // alist = [2, 1, 3] --> alist = [2, 7, 1, 3]
  i = 7;
  CU_ASSERT_TRUE(alist_addat(alist, 1, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 4);
  CU_ASSERT_EQUAL(compare_int(alist_get(alist, 1), &i), 0);
  alist_destroy(&alist);
}

void arraylist_addfirst_TTP() {
  alist = alist_new(size_int);
  // Add an element at the beginning of an empty list
  i = 1;
  CU_ASSERT_TRUE(alist_addfirst(alist, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(compare_int(alist_peekfirst(alist), &i), 0);
  // Add an element at the beginning of a non-empty list
  i = 2;
  CU_ASSERT_TRUE(alist_addfirst(alist, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(compare_int(alist_peekfirst(alist), &i), 0);
  alist_destroy(&alist);
}

void arraylist_addv_TTP() {
  int j;
  alist = alist_new(size_int);
  // Add 3 elements
  i = 1;
  j = 2;
  CU_ASSERT_EQUAL(alist_addv(alist, 3, &i, &i, &j), 3);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 3);
  // alist = [1, 1, 2]
  CU_ASSERT_EQUAL(compare_int(alist_peekfirst(alist), &i), 0);
  CU_ASSERT_EQUAL(compare_int(alist_get(alist, 1), &i), 0);
  CU_ASSERT_EQUAL(compare_int(alist_peeklast(alist), &j), 0);
  alist_destroy(&alist);
}

void arraylist_clear_TTP() {
  alist = alist_new(size_int);
  i = 1;
  // Add 3 elements and clear the list
  alist_addv(alist, 3, &i, &i, &i);
  alist_clear(alist);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 0);
  alist_destroy(&alist);
}

void arraylist_clone_TTP() {
  arraylist_t* alist2;
  alist = alist_new(size_int);
  i = 1;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  alist2 = alist_clone(alist);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(alist2);
  CU_ASSERT_EQUAL(alist->size, alist2->size);
  CU_ASSERT_EQUAL(compare_int(alist_peekfirst(alist), alist_peekfirst(alist2)), 0);
  CU_ASSERT_EQUAL(compare_int(alist_peeklast(alist), alist_peeklast(alist2)), 0);
  alist_destroy(&alist);
  alist_destroy(&alist2);
}

void arraylist_cmp_TTP() {
  arraylist_t* alist2 = alist_new(size_int);
  // Different size of data
  alist = alist_new(sizeof(float));
  CU_ASSERT_EQUAL(alist_cmp(alist, alist2, compare_int), (sizeof(float) - size_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
  // Different size
  alist = alist_new(size_int); // alist = [] (size = 0)
  i = 3;
  alist_add(alist2, &i); // alist2 = [3] (size = 1)
  CU_ASSERT_EQUAL(alist_cmp(alist, alist2, compare_int), -1); // 0 - 1 = -1
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Different elements
  i = 7;
  alist_add(alist, &i); // alist = [7], alist2 = [3]
  CU_ASSERT_EQUAL(alist_cmp(alist2, alist, compare_int), (3 - 7));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Same lists
  alist_add(alist2, &i); // alist2 = [3, 7]
  i = 3;
  alist_addfirst(alist, &i); // alist = [3, 7]
  CU_ASSERT_EQUAL(alist_cmp(alist, alist2, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
  alist_destroy(&alist2);
}

void arraylist_contains_TTP() {
  alist = alist_new(size_int);
  // The list is empty (so 4 is not found)
  i = 4;
  CU_ASSERT_FALSE(alist_contains(alist, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // alist = [1, 2]
  i = 1;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  // The list does not contain 4
  i = 4;
  CU_ASSERT_FALSE(alist_contains(alist, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // The list contains 2
  i = 2;
  CU_ASSERT_TRUE(alist_contains(alist, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_count_TTP() {
  alist = alist_new(size_int);
  i = 0;
  alist_add(alist, &i);
  i = 1;
  alist_addv(alist, 2, &i, &i);
  // alist = [0, 1, 1]
  // The list contains twice the element 1
  CU_ASSERT_EQUAL(alist_count(alist, &i, compare_int), 2);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 2;
  // The list does not contain an element 2
  CU_ASSERT_EQUAL(alist_count(alist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_empty_TTP() {
  alist = alist_new(size_int);
  CU_ASSERT_EQUAL(alist_empty(alist), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 1;
  alist_add(alist, &i);
  CU_ASSERT_EQUAL(alist_empty(alist), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_finddup_TTP() {
  arraylist_t* dupalist = alist_new(size_int);
  alist = alist_new(size_int);
  i = 1;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  i = 3;
  alist_add(alist, &i);
  // No duplicates (alist = [1, 2, 3] --> dupalist = [], returned value = 0)
  CU_ASSERT_EQUAL(alist_finddup(alist, dupalist, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Duplicates (alist = [1, 2, 3, 3, 3, 1] --> dupalist = [1, 3], returned value = 2)
  alist_add(alist, &i);
  alist_add(alist, &i);
  i = 1;
  alist_add(alist, &i);
  CU_ASSERT_EQUAL(alist_finddup(alist, dupalist, compare_int), 2);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Check dupalist elements
  CU_ASSERT_EQUAL(dupalist->size, 2);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(alist_peekfirst(dupalist), &i), 0);
  i = 3;
  CU_ASSERT_EQUAL(compare_int(alist_peeklast(dupalist), &i), 0);
  alist_destroy(&dupalist);
  alist_destroy(&alist);
}

void arraylist_fromarray_TTP() {
  i = 1;
  int j = 2;
  int array[2] = {i, j};
  alist = alist_fromarray(array, 2, sizeof(int));
  CU_ASSERT_PTR_NOT_NULL(alist);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 2);
  // Check values
  CU_ASSERT_EQUAL(compare_int(alist_peekfirst(alist), &i), 0);
  CU_ASSERT_EQUAL(compare_int(alist_peeklast(alist), &j), 0);
  alist_destroy(&alist);
}

void arraylist_get_TTP() {
  alist = alist_new(size_int);
  i = 1;
  int j = 2;
  alist_addv(alist, 2, &i, &j);
  // alist = [1, 2]
  // Get 1st element
  CU_ASSERT_EQUAL(*((int*)alist_get(alist, 0)), i);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Get 2sd element
  CU_ASSERT_EQUAL(*((int*)alist_get(alist, 1)), j);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_indexof_TTP() {
  alist = alist_new(size_int);
  i = 1;
  int j = 2;
  alist_addv(alist, 2, &i, &j);
  // alist = [1, 2]
  // Index of "1" (&i)
  CU_ASSERT_EQUAL(alist_indexof(alist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Index of "2" (&j)
  CU_ASSERT_EQUAL(alist_indexof(alist, &j, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_issorted_TTP() {
  alist = alist_new(size_int);
  i = 1;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  i = 3;
  alist_add(alist, &i);
  // On a sorted alist ([1, 2, 3])
  CU_ASSERT_EQUAL(alist_issorted(alist, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 1;
  alist_add(alist, &i);
  i = -5;
  alist_add(alist, &i);
  // On a non-sorted list ([1, 2, 3, 1, -5])
  CU_ASSERT_EQUAL(alist_issorted(alist, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_lastindexof_TTP() {
  alist = alist_new(size_int);
  i = 1;
  int j = 2;
  alist_addv(alist, 3, &i, &j, &j);
  // alist = [1, 2, 2]
  // Last index of "1" (&i)
  CU_ASSERT_EQUAL(alist_lastindexof(alist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Last index of "2" (&j)
  CU_ASSERT_EQUAL(alist_lastindexof(alist, &j, compare_int), 2);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_peekfirst_TTP() {
  alist = alist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(alist_peekfirst(alist), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a list of 2 elements
  i = 1;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(alist_peekfirst(alist), &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_peeklast_TTP() {
  alist = alist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(alist_peeklast(alist), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a list of 2 elements
  i = 1;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  CU_ASSERT_EQUAL(compare_int(alist_peeklast(alist), &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_remove_TTP() {
  alist = alist_new(size_int);
  // Remove an element which is in the list
  i = 1;
  alist_add(alist, &i);
  CU_ASSERT_EQUAL(alist->size, 1);
  CU_ASSERT_EQUAL(alist_remove(alist, &i, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 0);
  // Remove an element in an empty list
  CU_ASSERT_EQUAL(alist_remove(alist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove an element which is not in the list (non empty list)
  alist_add(alist, &i);
  i = 2;
  CU_ASSERT_EQUAL(alist->size, 1);
  CU_ASSERT_EQUAL(alist_remove(alist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 1);
  alist_destroy(&alist);
}

void arraylist_removeall_TTP() {
  alist = alist_new(size_int);
  // Remove all the elements 1 in the list (2 elements removed)
  i = 2;
  alist_add(alist, &i);
  i = 1;
  alist_add(alist, &i);
  alist_add(alist, &i);
  // alist = [2, 1, 1]
  CU_ASSERT_EQUAL(alist->size, 3);
  CU_ASSERT_EQUAL(alist_removeall(alist, &i, compare_int), 2);
  CU_ASSERT_EQUAL(alist->size, 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove all the elements 1 in the list (no elements removed), alist = [2]
  i = 1;
  CU_ASSERT_EQUAL(alist_removeall(alist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(alist->size, 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove all the elements of the list
  alist_clear(alist);
  i = 1;
  alist_addv(alist, 3, &i, &i, &i);
  CU_ASSERT_EQUAL(alist->size, 3);
  CU_ASSERT_EQUAL(alist_removeall(alist, &i, compare_int), 3);
  CU_ASSERT_EQUAL(alist->size, 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove on an empty list
  alist_clear(alist);
  CU_ASSERT_EQUAL(alist_removeall(alist, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_removeat_TTP() {
  alist = alist_new(size_int);
  i = 2;
  alist_add(alist, &i);
  i = 4;
  alist_add(alist, &i);
  i = 6;
  alist_add(alist, &i);
  i = 8;
  alist_add(alist, &i);
  i = 10;
  alist_add(alist, &i);
  // list = [2, 4, 6, 8, 10] --> list = [2, 4, 6, 8]
  CU_ASSERT_TRUE(alist_removeat(alist, 4));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 4);
  i = 2;
  CU_ASSERT_EQUAL(compare_int(alist->data[0], &i), 0);
  i = 4;
  CU_ASSERT_EQUAL(compare_int(alist->data[1], &i), 0);
  i = 6;
  CU_ASSERT_EQUAL(compare_int(alist->data[2], &i), 0);
  i = 8;
  CU_ASSERT_EQUAL(compare_int(alist->data[3], &i), 0);
  // list = [2, 4, 6, 8] --> list = [2, 6, 8]
  CU_ASSERT_TRUE(alist_removeat(alist, 1));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 3);
  i = 2;
  CU_ASSERT_EQUAL(compare_int(alist->data[0], &i), 0);
  i = 6;
  CU_ASSERT_EQUAL(compare_int(alist->data[1], &i), 0);
  i = 8;
  CU_ASSERT_EQUAL(compare_int(alist->data[2], &i), 0);
  alist_destroy(&alist);
}

void arraylist_removefirst_TTP() {
  alist = alist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(alist_removefirst(alist), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a non-empty list
  i = 1;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  CU_ASSERT_EQUAL(alist_removefirst(alist), 1);
  CU_ASSERT_EQUAL(alist->size, 1);
  CU_ASSERT_EQUAL(compare_int(alist_peekfirst(alist), &i), 0);
  alist_destroy(&alist);
}

void arraylist_removelast_TTP() {
  alist = alist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(alist_removelast(alist), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a non-empty list [1, 2]
  i = 1;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  CU_ASSERT_EQUAL(alist_removelast(alist), 1);
  CU_ASSERT_EQUAL(alist->size, 1);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(alist_peeklast(alist), &i), 0);
  alist_destroy(&alist);
}

void arraylist_setat_TTP() {
  void* previous;
  alist = alist_new(size_int);
  i = 4;
  alist_add(alist, &i);
  i = 8;
  alist_add(alist, &i);
  // alist = [4, 8] -> alist = [4, 16];
  i = 16;
  previous = malloc(size_int);
  CU_ASSERT_EQUAL(alist_setat(alist, 1, &i, previous), 1);
  CU_ASSERT_EQUAL(*((int*) previous), 8);
  free(previous);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 2);
  CU_ASSERT_EQUAL(*((int*) alist->data[0]), 4);
  CU_ASSERT_EQUAL(*((int*) alist->data[1]), 16);
  alist_destroy(&alist);
}

void arraylist_size_TTP() {
  alist = alist_new(size_int);
  CU_ASSERT_EQUAL(alist_size(alist), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 1;
  alist_add(alist, &i);
  CU_ASSERT_EQUAL(alist_size(alist), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_sort_TTP() {
  alist = alist_new(size_int);
  // Sort an empty list
  CU_ASSERT_TRUE(alist_sort(alist, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Already sorted, alist = [1, 2] --> alist = [1, 2]
  i = 1;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  CU_ASSERT_TRUE(alist_sort(alist, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 2);
  CU_ASSERT_EQUAL(compare_int(alist_peeklast(alist), &i), 0);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(alist_peekfirst(alist), &i), 0);
  // Sorted by alist_sort(), alist = [1, 2, -5, 9, 2] --> alist = [-5, 1, 2, 2, 9]
  i = -5;
  alist_add(alist, &i);
  i = 9;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  CU_ASSERT_EQUAL(alist_sort(alist, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(alist->size, 5);
  CU_ASSERT_TRUE(alist_issorted(alist, compare_int));
  alist_destroy(&alist);
}

void arraylist_toarray_TTP() {
  void* array = NULL;
  alist = alist_new(size_int);
  // Empty list
  array = alist_toarray(alist);
  CU_ASSERT_PTR_NOT_NULL(array);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  free(array);
  array = NULL;
  // Non-empty list
  i = 1;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  array = alist_toarray(alist);
  CU_ASSERT_PTR_NOT_NULL(array);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Check values
  i = 1;
  CU_ASSERT_EQUAL(compare_int(array, &i), 0);
  i = 2;
  CU_ASSERT_EQUAL(compare_int(array + alist->data_size, &i), 0);
  free(array);
  alist_destroy(&alist);
}

void arraylist_tostring_TTP() {
  char* str;
  alist = alist_new(size_int);
  // On an empty list
  str = alist_tostring(alist, inttostring);
  CU_ASSERT_PTR_NOT_NULL(str);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(strcmp(str, "[]"), 0);
  free(str);
  str = NULL;
  // On a non-empty list
  i = 1;
  alist_add(alist, &i);
  i = 2;
  alist_add(alist, &i);
  i = 4;
  alist_add(alist, &i);
  i = 8;
  alist_add(alist, &i);
  i = 16;
  alist_add(alist, &i);
  str = alist_tostring(alist, inttostring);
  CU_ASSERT_PTR_NOT_NULL(str);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(strcmp(str, "[1, 2, 4, 8, 16]"), 0);
  free(str);
  alist_destroy(&alist);
}

/*
 ********************************
 * Test-to-fail
 ********************************
 */
void arraylist_destroy_TTF() {
  // NULL list and NULL pointer
  alist = NULL;
  alist_destroy(&alist);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  alist_destroy(NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void arraylist_add_TTF() {
  alist = alist_new(size_int);
  // Add a NULL element
  CU_ASSERT_FALSE(alist_add(alist, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(alist->size, 0);
  // Add an element to NULL
  i = 1;
  CU_ASSERT_FALSE(alist_add(NULL, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(alist->size, 0);
  // Both parameters are NULL
  CU_ASSERT_FALSE(alist_add(NULL, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(alist->size, 0);
  alist_destroy(&alist);
}

void arraylist_addall_TTF() {
  alist = alist_new(size_int);
  // Source list is NULL
  CU_ASSERT_FALSE(alist_addall(alist, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Destination list is NULL
  CU_ASSERT_FALSE(alist_addall(NULL, alist));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Both parameters are NULL
  CU_ASSERT_FALSE(alist_addall(NULL, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Addall on the same list is forbidden (infinite loop if not forbidden)
  CU_ASSERT_FALSE(alist_addall(alist, alist));
  CU_ASSERT_EQUAL(cerrno, CERR_FORBIDDEN);
  alist_destroy(&alist);
}

void arraylist_addat_TTF() {
  alist = alist_new(size_int);
  // Add a NULL element
  CU_ASSERT_FALSE(alist_addat(alist, 0, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(alist->size, 0);
  // Add an element to NULL
  i = 1;
  CU_ASSERT_FALSE(alist_addat(NULL, 2, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Both parameters are NULL
  CU_ASSERT_FALSE(alist_addat(NULL, 1, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Not valid index (size of list = 0)
  CU_ASSERT_FALSE(alist_addat(alist, -1, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  CU_ASSERT_FALSE(alist_addat(alist, 3, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  alist_destroy(&alist);
}

void arraylist_addfirst_TTF() {
  alist = alist_new(size_int);
  // Add NULL at the beginning of the list
  CU_ASSERT_FALSE(alist_addfirst(alist, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Add a value at the beginning of NULL
  i = 1;
  CU_ASSERT_FALSE(alist_addfirst(NULL, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  alist_destroy(&alist);
}

void arraylist_addv_TTF() {
  // NULL list
  i = 1;
  CU_ASSERT_EQUAL(alist_addv(NULL, 1, &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  alist = alist_new(size_int);
  CU_ASSERT_EQUAL(alist_addv(alist, 3, &i, NULL, &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(alist->size, 0);
  alist_destroy(&alist);
}

void arraylist_clear_TTF() {
  // NULL list
  alist_clear(NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void arraylist_clone_TTF() {
  // NULL list
  CU_ASSERT_PTR_NULL(alist_clone(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void arraylist_cmp_TTF() {
  arraylist_t* alist2 = alist_new(size_int);
  alist = alist_new(size_int);
  // NULL list 1
  CU_ASSERT_EQUAL(alist_cmp(NULL, alist2, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL list 2
  CU_ASSERT_EQUAL(alist_cmp(alist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  alist_destroy(&alist);
  alist_destroy(&alist2);
}

void arraylist_contains_TTF() {
  // NULL list
  i = 2;
  CU_ASSERT_FALSE(alist_contains(NULL, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  alist = alist_new(size_int);
  CU_ASSERT_FALSE(alist_contains(alist, NULL, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  alist_destroy(&alist);
}

void arraylist_count_TTF() {
  i = 1;
  alist = alist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(alist_count(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(alist_count(alist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  alist_destroy(&alist);
}

void arraylist_empty_TTF() {
  CU_ASSERT_EQUAL(alist_empty(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void arraylist_finddup_TTF() {
  arraylist_t* dupalist = alist_new(size_int);
  i = 1;
  alist = alist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(alist_finddup(NULL, alist, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL duplicate list
  CU_ASSERT_EQUAL(alist_finddup(alist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  alist_destroy(&alist);
  alist_destroy(&dupalist);
}

void arraylist_fromarray_TTF() {
  // NULL list
  CU_ASSERT_PTR_NULL(alist_fromarray(NULL, 2, sizeof(int)));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void arraylist_get_TTF() {
  alist = alist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(alist_get(NULL, 1), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Get with bad index (2sd element on an empty list)
  CU_ASSERT_EQUAL(alist_get(alist, 1), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  // Get with bad index (negative index)
  CU_ASSERT_EQUAL(alist_get(alist, -1), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  alist_destroy(&alist);
}

void arraylist_indexof_TTF() {
  alist = alist_new(size_int);
  // NULL list
  i = 1;
  CU_ASSERT_EQUAL(alist_indexof(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(alist_indexof(alist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // element not in the list
  CU_ASSERT_EQUAL(alist_indexof(alist, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_issorted_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(alist_issorted(NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  alist_destroy(&alist);
}

void arraylist_lastindexof_TTF() {
  alist = alist_new(size_int);
  // NULL list
  i = 1;
  CU_ASSERT_EQUAL(alist_lastindexof(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(alist_lastindexof(alist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // element not in the list
  CU_ASSERT_EQUAL(alist_lastindexof(alist, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  alist_destroy(&alist);
}

void arraylist_peekfirst_TTF() {
  // NULL list
  CU_ASSERT_PTR_NULL(alist_peekfirst(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void arraylist_peeklast_TTF() {
  // NULL list
  CU_ASSERT_PTR_NULL(alist_peekfirst(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void arraylist_remove_TTF() {
  i = 1;
  alist = alist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(alist_remove(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(alist_remove(alist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  alist_destroy(&alist);
}

void arraylist_removeall_TTF() {
  i = 1;
  alist = alist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(alist_removeall(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(alist_removeall(alist, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  alist_destroy(&alist);
}

void arraylist_removeat_TTF() {
  i = 1;
  alist = alist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(alist_removeat(NULL, 1), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Bad index
  i = 2;
  alist_add(alist, &i);
  i = 4;
  alist_add(alist, &i);
  i = 6;
  alist_add(alist, &i);
  CU_ASSERT_EQUAL(alist_removeat(alist, -2), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  CU_ASSERT_EQUAL(alist_removeat(alist, 4), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  alist_destroy(&alist);
}

void arraylist_removefirst_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(alist_removefirst(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void arraylist_removelast_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(alist_removelast(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void arraylist_setat_TTF() {
  i = 1;
  // NULL list
  CU_ASSERT_EQUAL(alist_setat(NULL, 0, &i, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  alist = alist_new(size_int);
  CU_ASSERT_EQUAL(alist_setat(alist, 0, NULL, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Bad index
  CU_ASSERT_EQUAL(alist_setat(alist, -1, &i, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  CU_ASSERT_EQUAL(alist_setat(alist, 0, &i, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  alist_destroy(&alist);
}

void arraylist_size_TTF() {
  CU_ASSERT_EQUAL(alist_size(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void arraylist_sort_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(alist_sort(NULL, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL compare function
  alist = alist_new(size_int);
  CU_ASSERT_EQUAL(alist_sort(alist, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  alist_destroy(&alist);
}

void arraylist_toarray_TTF() {
  // NULL list
  CU_ASSERT_PTR_NULL(alist_toarray(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void arraylist_tostring_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(alist_tostring(NULL, inttostring), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL tostring function
  alist = alist_new(size_int);
  CU_ASSERT_EQUAL(alist_tostring(alist, NULL), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  alist_destroy(&alist);
}


/*
 **************************************************
 ******************* Stack suite ******************
 **************************************************
 */
/*
 * Test-to-pass
 */
void stack_new_destroy_TTP() {
  stack = stack_new(size_int);
  CU_ASSERT_PTR_NOT_NULL(stack);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
  CU_ASSERT_PTR_NULL(stack);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
}

void stack_add_TTP() {
  stack = stack_new(size_int);
  // Add an element when the stack is empty
  i = 1;
  CU_ASSERT_TRUE(stack_add(stack, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 1);
  CU_ASSERT_EQUAL(compare_int(stack_peeklast(stack), &i), 0);
  // Add an element when the stack is not empty
  i = 2;
  CU_ASSERT_TRUE(stack_add(stack, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 2);
  CU_ASSERT_EQUAL(compare_int(stack_peeklast(stack), &i), 0);
  stack_destroy(&stack);
}

void stack_addall_TTP() {
  stack_t* stack2 = stack_new(size_int);
  i = 1;
  stack_add(stack2, &i);
  i = 2;
  stack_add(stack2, &i);
  stack = stack_new(size_int);
  // stack = [], stack2 = [1, 2]
  stack_addall(stack, stack2);
  // stack = [1, 2], stack2 = [1, 2]
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 2);
  // Check elemens of the stack
  // i = 2
  CU_ASSERT_EQUAL(compare_int(stack_peeklast(stack), &i), 0);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(stack_peekfirst(stack), &i), 0);
  stack_destroy(&stack2);
  stack_destroy(&stack);
}

void stack_addat_TTP() {
  stack = stack_new(size_int);
  // Add an element at index 0 when the stack is empty
  i = 1;
  CU_ASSERT_TRUE(stack_addat(stack, 0, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 1);
  CU_ASSERT_EQUAL(compare_int(stack_get(stack, 0), &i), 0);
  // Add an element before
  // stack = [1] --> stack = [2, 1]
  i = 2;
  CU_ASSERT_TRUE(stack_addat(stack, 0, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 2);
  CU_ASSERT_EQUAL(compare_int(stack_get(stack, 0), &i), 0);
  // Add an element after
  // stack = [2, 1] --> stack = [2, 1, 3]
  i = 3;
  CU_ASSERT_TRUE(stack_addat(stack, 2, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 3);
  CU_ASSERT_EQUAL(compare_int(stack_get(stack, 2), &i), 0);
  // Add an element in the middle (at index 1)
  // stack = [2, 1, 3] --> stack = [2, 7, 1, 3]
  i = 7;
  CU_ASSERT_TRUE(stack_addat(stack, 1, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 4);
  CU_ASSERT_EQUAL(compare_int(stack_get(stack, 1), &i), 0);
  stack_destroy(&stack);
}

void stack_addfirst_TTP() {
  stack = stack_new(size_int);
  // Add an element at the beginning of an empty stack
  i = 1;
  CU_ASSERT_TRUE(stack_addfirst(stack, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(compare_int(stack_peekfirst(stack), &i), 0);
  // Add an element at the beginning of a non-empty stack
  i = 2;
  CU_ASSERT_TRUE(stack_addfirst(stack, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(compare_int(stack_peekfirst(stack), &i), 0);
  stack_destroy(&stack);
}

void stack_addv_TTP() {
  int j;
  stack = stack_new(size_int);
  // Add 3 elements
  i = 1;
  j = 2;
  CU_ASSERT_EQUAL(stack_addv(stack, 3, &i, &i, &j), 3);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 3);
  // stack = [1, 1, 2]
  CU_ASSERT_EQUAL(compare_int(stack_peekfirst(stack), &i), 0);
  CU_ASSERT_EQUAL(compare_int(stack_get(stack, 1), &i), 0);
  CU_ASSERT_EQUAL(compare_int(stack_peeklast(stack), &j), 0);
  stack_destroy(&stack);
}

void stack_clear_TTP() {
  stack = stack_new(size_int);
  i = 1;
  // Add 3 elements and clear the stack
  stack_addv(stack, 3, &i, &i, &i);
  stack_clear(stack);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 0);
  stack_destroy(&stack);
}

void stack_clone_TTP() {
  stack_t* stack2;
  stack = stack_new(size_int);
  i = 1;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  stack2 = stack_clone(stack);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(stack2);
  CU_ASSERT_EQUAL(stack->size, stack2->size);
  CU_ASSERT_EQUAL(compare_int(stack_peekfirst(stack), stack_peekfirst(stack2)), 0);
  CU_ASSERT_EQUAL(compare_int(stack_peeklast(stack), stack_peeklast(stack2)), 0);
  stack_destroy(&stack);
  stack_destroy(&stack2);
}

void stack_cmp_TTP() {
  stack_t* stack2 = stack_new(size_int);
  // Different size of data
  stack = stack_new(sizeof(float));
  CU_ASSERT_EQUAL(stack_cmp(stack, stack2, compare_int), (sizeof(float) - size_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
  // Different size
  stack = stack_new(size_int); // stack = [] (size = 0)
  i = 3;
  stack_add(stack2, &i); // stack2 = [3] (size = 1)
  CU_ASSERT_EQUAL(stack_cmp(stack, stack2, compare_int), -1); // 0 - 1 = -1
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Different elements
  i = 7;
  stack_add(stack, &i); // stack = [7], stack2 = [3]
  CU_ASSERT_EQUAL(stack_cmp(stack2, stack, compare_int), (3 - 7));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Same stacks
  stack_add(stack2, &i); // stack2 = [3, 7]
  i = 3;
  stack_addfirst(stack, &i); // stack = [3, 7]
  CU_ASSERT_EQUAL(stack_cmp(stack, stack2, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
  stack_destroy(&stack2);
}

void stack_contains_TTP() {
  stack = stack_new(size_int);
  // The stack is empty (so 4 is not found)
  i = 4;
  CU_ASSERT_FALSE(stack_contains(stack, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // stack = [1, 2]
  i = 1;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  // The stack does not contain 4
  i = 4;
  CU_ASSERT_FALSE(stack_contains(stack, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // The stack contains 2
  i = 2;
  CU_ASSERT_TRUE(stack_contains(stack, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_count_TTP() {
  stack = stack_new(size_int);
  i = 0;
  stack_add(stack, &i);
  i = 1;
  stack_addv(stack, 2, &i, &i);
  // stack = [0, 1, 1]
  // The stack contains twice the element 1
  CU_ASSERT_EQUAL(stack_count(stack, &i, compare_int), 2);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 2;
  // The stack does not contain an element 2
  CU_ASSERT_EQUAL(stack_count(stack, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_empty_TTP() {
  stack = stack_new(size_int);
  CU_ASSERT_TRUE(stack_empty(stack));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 1;
  stack_add(stack, &i);
  CU_ASSERT_FALSE(stack_empty(stack));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_finddup_TTP() {
  stack_t* dupstack = stack_new(size_int);
  stack = stack_new(size_int);
  i = 1;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  i = 3;
  stack_add(stack, &i);
  // No duplicates (stack = [1, 2, 3] --> dupstack = [], returned value = 0)
  CU_ASSERT_EQUAL(stack_finddup(stack, dupstack, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Duplicates (stack = [1, 2, 3, 3, 3, 1] --> dupstack = [1, 3], returned value = 2)
  stack_add(stack, &i);
  stack_add(stack, &i);
  i = 1;
  stack_add(stack, &i);
  CU_ASSERT_EQUAL(stack_finddup(stack, dupstack, compare_int), 2);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Check dupstack elements
  CU_ASSERT_EQUAL(dupstack->size, 2);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(stack_peekfirst(dupstack), &i), 0);
  i = 3;
  CU_ASSERT_EQUAL(compare_int(stack_peeklast(dupstack), &i), 0);
  stack_destroy(&dupstack);
  stack_destroy(&stack);
}

void stack_fromarray_TTP() {
  i = 1;
  int j = 2;
  int array[2] = {i, j};
  stack = stack_fromarray(array, 2, sizeof(int));
  CU_ASSERT_PTR_NOT_NULL(stack);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 2);
  // Check values
  CU_ASSERT_EQUAL(compare_int(stack_peekfirst(stack), &i), 0);
  CU_ASSERT_EQUAL(compare_int(stack_peeklast(stack), &j), 0);
  stack_destroy(&stack);
}

void stack_get_TTP() {
  stack = stack_new(size_int);
  i = 1;
  int j = 2;
  stack_addv(stack, 2, &i, &j);
  // stack = [1, 2]
  // Get 1st element
  CU_ASSERT_EQUAL(*((int*)stack_get(stack, 0)), i);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Get 2sd element
  CU_ASSERT_EQUAL(*((int*)stack_get(stack, 1)), j);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_indexof_TTP() {
  stack = stack_new(size_int);
  i = 1;
  int j = 2;
  stack_addv(stack, 2, &i, &j);
  // stack = [1, 2]
  // Index of "1" (&i)
  CU_ASSERT_EQUAL(stack_indexof(stack, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Index of "2" (&j)
  CU_ASSERT_EQUAL(stack_indexof(stack, &j, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_issorted_TTP() {
  stack = stack_new(size_int);
  i = 1;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  i = 3;
  stack_add(stack, &i);
  // On a sorted stack ([1, 2, 3])
  CU_ASSERT_EQUAL(stack_issorted(stack, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 1;
  stack_add(stack, &i);
  i = -5;
  stack_add(stack, &i);
  // On a non-sorted stack ([1, 2, 3, 1, -5])
  CU_ASSERT_EQUAL(stack_issorted(stack, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_lastindexof_TTP() {
  stack = stack_new(size_int);
  i = 1;
  int j = 2;
  stack_addv(stack, 3, &i, &j, &j);
  // stack = [1, 2, 2]
  // Last index of "1" (&i)
  CU_ASSERT_EQUAL(stack_lastindexof(stack, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Last index of "2" (&j)
  CU_ASSERT_EQUAL(stack_lastindexof(stack, &j, compare_int), 2);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_peekfirst_TTP() {
  stack = stack_new(size_int);
  // On an empty stack
  CU_ASSERT_EQUAL(stack_peekfirst(stack), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a stack of 2 elements
  i = 1;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(stack_peekfirst(stack), &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_peeklast_TTP() {
  stack = stack_new(size_int);
  // On an empty stack
  CU_ASSERT_EQUAL(stack_peeklast(stack), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a stack of 2 elements
  i = 1;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  CU_ASSERT_EQUAL(compare_int(stack_peeklast(stack), &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_pop_TTP() {
  void* val;
  i = 1;
  stack = stack_new(size_int);
  stack_push(stack, &i);
  val = stack_pop(stack);
  CU_ASSERT_EQUAL(*((int*) val), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  free(val);
  stack_destroy(&stack);
}

void stack_push_TTP() {
  i = 7;
  stack = stack_new(size_int);
  CU_ASSERT_TRUE(stack_push(stack, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 1);
  CU_ASSERT_EQUAL(*((int*) stack->data[0]), 7);
  stack_destroy(&stack);
}

void stack_remove_TTP() {
  stack = stack_new(size_int);
  // Remove an element which is in the stack
  i = 1;
  stack_add(stack, &i);
  CU_ASSERT_EQUAL(stack->size, 1);
  CU_ASSERT_EQUAL(stack_remove(stack, &i, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 0);
  // Remove an element in an empty stack
  CU_ASSERT_EQUAL(stack_remove(stack, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove an element which is not in the stack (non empty stack)
  stack_add(stack, &i);
  i = 2;
  CU_ASSERT_EQUAL(stack->size, 1);
  CU_ASSERT_EQUAL(stack_remove(stack, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 1);
  stack_destroy(&stack);
}

void stack_removeall_TTP() {
  stack = stack_new(size_int);
  // Remove all the elements 1 in the stack (2 elements removed)
  i = 2;
  stack_add(stack, &i);
  i = 1;
  stack_add(stack, &i);
  stack_add(stack, &i);
  // stack = [2, 1, 1]
  CU_ASSERT_EQUAL(stack->size, 3);
  CU_ASSERT_EQUAL(stack_removeall(stack, &i, compare_int), 2);
  CU_ASSERT_EQUAL(stack->size, 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove all the elements 1 in the stack (no elements removed), stack = [2]
  i = 1;
  CU_ASSERT_EQUAL(stack_removeall(stack, &i, compare_int), 0);
  CU_ASSERT_EQUAL(stack->size, 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove all the elements of the stack
  stack_clear(stack);
  i = 1;
  stack_addv(stack, 3, &i, &i, &i);
  CU_ASSERT_EQUAL(stack->size, 3);
  CU_ASSERT_EQUAL(stack_removeall(stack, &i, compare_int), 3);
  CU_ASSERT_EQUAL(stack->size, 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove on an empty stack
  stack_clear(stack);
  CU_ASSERT_EQUAL(stack_removeall(stack, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_removeat_TTP() {
  stack = stack_new(size_int);
  i = 2;
  stack_add(stack, &i);
  i = 4;
  stack_add(stack, &i);
  i = 6;
  stack_add(stack, &i);
  i = 8;
  stack_add(stack, &i);
  i = 10;
  stack_add(stack, &i);
  // stack = [2, 4, 6, 8, 10] --> stack = [2, 4, 6, 8]
  CU_ASSERT_TRUE(stack_removeat(stack, 4));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 4);
  i = 2;
  CU_ASSERT_EQUAL(compare_int(stack->data[0], &i), 0);
  i = 4;
  CU_ASSERT_EQUAL(compare_int(stack->data[1], &i), 0);
  i = 6;
  CU_ASSERT_EQUAL(compare_int(stack->data[2], &i), 0);
  i = 8;
  CU_ASSERT_EQUAL(compare_int(stack->data[3], &i), 0);
  // stack = [2, 4, 6, 8] --> stack = [2, 6, 8]
  CU_ASSERT_TRUE(stack_removeat(stack, 1));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 3);
  i = 2;
  CU_ASSERT_EQUAL(compare_int(stack->data[0], &i), 0);
  i = 6;
  CU_ASSERT_EQUAL(compare_int(stack->data[1], &i), 0);
  i = 8;
  CU_ASSERT_EQUAL(compare_int(stack->data[2], &i), 0);
  stack_destroy(&stack);
}

void stack_removefirst_TTP() {
  stack = stack_new(size_int);
  // On an empty stack
  CU_ASSERT_EQUAL(stack_removefirst(stack), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a non-empty stack
  i = 1;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  CU_ASSERT_EQUAL(stack_removefirst(stack), 1);
  CU_ASSERT_EQUAL(stack->size, 1);
  CU_ASSERT_EQUAL(compare_int(stack_peekfirst(stack), &i), 0);
  stack_destroy(&stack);
}

void stack_removelast_TTP() {
  stack = stack_new(size_int);
  // On an empty stack
  CU_ASSERT_EQUAL(stack_removelast(stack), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a non-empty stack [1, 2]
  i = 1;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  CU_ASSERT_EQUAL(stack_removelast(stack), 1);
  CU_ASSERT_EQUAL(stack->size, 1);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(stack_peeklast(stack), &i), 0);
  stack_destroy(&stack);
}

void stack_setat_TTP() {
  void* previous;
  stack = stack_new(size_int);
  i = 4;
  stack_add(stack, &i);
  i = 8;
  stack_add(stack, &i);
  // stack = [4, 8] -> stack = [4, 16];
  i = 16;
  previous = malloc(size_int);
  CU_ASSERT_EQUAL(stack_setat(stack, 1, &i, previous), 1);
  CU_ASSERT_EQUAL(*((int*) previous), 8);
  free(previous);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 2);
  CU_ASSERT_EQUAL(*((int*) stack->data[0]), 4);
  CU_ASSERT_EQUAL(*((int*) stack->data[1]), 16);
  stack_destroy(&stack);
}

void stack_size_TTP() {
  stack = stack_new(size_int);
  CU_ASSERT_EQUAL(stack_size(stack), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 1;
  stack_push(stack, &i);
  CU_ASSERT_EQUAL(stack_size(stack), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_sort_TTP() {
  stack = stack_new(size_int);
  // Sort an empty stack
  CU_ASSERT_TRUE(stack_sort(stack, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Already sorted, stack = [1, 2] --> stack = [1, 2]
  i = 1;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  CU_ASSERT_TRUE(stack_sort(stack, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 2);
  CU_ASSERT_EQUAL(compare_int(stack_peeklast(stack), &i), 0);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(stack_peekfirst(stack), &i), 0);
  // Sorted by stack_sort(), stack = [1, 2, -5, 9, 2] --> stack = [-5, 1, 2, 2, 9]
  i = -5;
  stack_add(stack, &i);
  i = 9;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  CU_ASSERT_EQUAL(stack_sort(stack, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(stack->size, 5);
  CU_ASSERT_TRUE(stack_issorted(stack, compare_int));
  stack_destroy(&stack);
}

void stack_toarray_TTP() {
  void* array = NULL;
  stack = stack_new(size_int);
  // Empty stack
  array = stack_toarray(stack);
  CU_ASSERT_PTR_NOT_NULL(array);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  free(array);
  array = NULL;
  // Non-empty stack
  i = 1;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  array = stack_toarray(stack);
  CU_ASSERT_PTR_NOT_NULL(array);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Check values
  i = 1;
  CU_ASSERT_EQUAL(compare_int(array, &i), 0);
  i = 2;
  CU_ASSERT_EQUAL(compare_int(array + stack->data_size, &i), 0);
  free(array);
  stack_destroy(&stack);
}

void stack_tostring_TTP() {
  char* str;
  stack = stack_new(size_int);
  // On an empty stack
  str = stack_tostring(stack, inttostring);
  CU_ASSERT_PTR_NOT_NULL(str);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(strcmp(str, "[]"), 0);
  free(str);
  str = NULL;
  // On a non-empty stack
  i = 1;
  stack_add(stack, &i);
  i = 2;
  stack_add(stack, &i);
  i = 4;
  stack_add(stack, &i);
  i = 8;
  stack_add(stack, &i);
  i = 16;
  stack_add(stack, &i);
  str = stack_tostring(stack, inttostring);
  CU_ASSERT_PTR_NOT_NULL(str);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(strcmp(str, "[1, 2, 4, 8, 16]"), 0);
  free(str);
  stack_destroy(&stack);
}

/*
 ********************************
 * Test-to-fail
 ********************************
 */
void stack_destroy_TTF() {
  // NULL stack and NULL pointer
  stack = NULL;
  stack_destroy(&stack);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void stack_add_TTF() {
  stack = stack_new(size_int);
  // Add a NULL element
  CU_ASSERT_FALSE(stack_add(stack, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(stack->size, 0);
  // Add an element to NULL
  i = 1;
  CU_ASSERT_FALSE(stack_add(NULL, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(stack->size, 0);
  // Both parameters are NULL
  CU_ASSERT_FALSE(stack_add(NULL, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(stack->size, 0);
  stack_destroy(&stack);
}

void stack_addall_TTF() {
  stack = stack_new(size_int);
  // Source stack is NULL
  CU_ASSERT_FALSE(stack_addall(stack, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Destination stack is NULL
  CU_ASSERT_FALSE(stack_addall(NULL, stack));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Both parameters are NULL
  CU_ASSERT_FALSE(stack_addall(NULL, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Addall on the same stack is forbidden (infinite loop if not forbidden)
  CU_ASSERT_FALSE(stack_addall(stack, stack));
  CU_ASSERT_EQUAL(cerrno, CERR_FORBIDDEN);
  stack_destroy(&stack);
}

void stack_addat_TTF() {
  stack = stack_new(size_int);
  // Add a NULL element
  CU_ASSERT_FALSE(stack_addat(stack, 0, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(stack->size, 0);
  // Add an element to NULL
  i = 1;
  CU_ASSERT_FALSE(stack_addat(NULL, 2, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Both parameters are NULL
  CU_ASSERT_FALSE(stack_addat(NULL, 1, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Not valid index (size of stack = 0)
  CU_ASSERT_FALSE(stack_addat(stack, -1, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  CU_ASSERT_FALSE(stack_addat(stack, 3, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  stack_destroy(&stack);
}

void stack_addfirst_TTF() {
  stack = stack_new(size_int);
  // Add NULL at the beginning of the stack
  CU_ASSERT_FALSE(stack_addfirst(stack, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Add a value at the beginning of NULL
  i = 1;
  CU_ASSERT_FALSE(stack_addfirst(NULL, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
}

void stack_addv_TTF() {
  // NULL stack
  i = 1;
  CU_ASSERT_EQUAL(stack_addv(NULL, 1, &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  stack = stack_new(size_int);
  CU_ASSERT_EQUAL(stack_addv(stack, 3, &i, NULL, &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(stack->size, 0);
  stack_destroy(&stack);
}

void stack_clear_TTF() {
  // NULL stack
  stack_clear(NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void stack_clone_TTF() {
  // NULL stack
  CU_ASSERT_PTR_NULL(stack_clone(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void stack_cmp_TTF() {
  stack_t* stack2 = stack_new(size_int);
  stack = stack_new(size_int);
  // NULL stack 1
  CU_ASSERT_EQUAL(stack_cmp(NULL, stack2, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL stack 2
  CU_ASSERT_EQUAL(stack_cmp(stack, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
  stack_destroy(&stack2);
}

void stack_contains_TTF() {
  // NULL stack
  i = 2;
  CU_ASSERT_FALSE(stack_contains(NULL, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  stack = stack_new(size_int);
  CU_ASSERT_FALSE(stack_contains(stack, NULL, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
}

void stack_count_TTF() {
  i = 1;
  stack = stack_new(size_int);
  // NULL stack
  CU_ASSERT_EQUAL(stack_count(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(stack_count(stack, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
}

void stack_empty_TTF() {
  // NULL stack
  CU_ASSERT_EQUAL(stack_empty(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void stack_finddup_TTF() {
  stack_t* dupstack = stack_new(size_int);
  i = 1;
  stack = stack_new(size_int);
  // NULL stack
  CU_ASSERT_EQUAL(stack_finddup(NULL, stack, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL duplicate stack
  CU_ASSERT_EQUAL(stack_finddup(stack, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
  stack_destroy(&dupstack);
}

void stack_fromarray_TTF() {
  // NULL stack
  CU_ASSERT_PTR_NULL(stack_fromarray(NULL, 2, sizeof(int)));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void stack_get_TTF() {
  stack = stack_new(size_int);
  // NULL stack
  CU_ASSERT_EQUAL(stack_get(NULL, 1), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Get with bad index (2sd element on an empty stack)
  CU_ASSERT_EQUAL(stack_get(stack, 1), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  // Get with bad index (negative index)
  CU_ASSERT_EQUAL(stack_get(stack, -1), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  stack_destroy(&stack);
}

void stack_indexof_TTF() {
  stack = stack_new(size_int);
  // NULL stack
  i = 1;
  CU_ASSERT_EQUAL(stack_indexof(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(stack_indexof(stack, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // element not in the stack
  CU_ASSERT_EQUAL(stack_indexof(stack, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_issorted_TTF() {
  // NULL stack
  CU_ASSERT_EQUAL(stack_issorted(NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
}

void stack_lastindexof_TTF() {
  stack = stack_new(size_int);
  // NULL stack
  i = 1;
  CU_ASSERT_EQUAL(stack_lastindexof(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(stack_lastindexof(stack, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // element not in the stack
  CU_ASSERT_EQUAL(stack_lastindexof(stack, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  stack_destroy(&stack);
}

void stack_peekfirst_TTF() {
  // NULL stack
  CU_ASSERT_PTR_NULL(stack_peekfirst(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void stack_peeklast_TTF() {
  // NULL stack
  CU_ASSERT_PTR_NULL(stack_peekfirst(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void stack_pop_TTF() {
  i = 1;
  stack = stack_new(size_int);
  // NULL stack
  CU_ASSERT_EQUAL(stack_pop(NULL), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
}

void stack_push_TTF() {
  i = 1;
  stack = stack_new(size_int);
  // NULL stack
  CU_ASSERT_EQUAL(stack_push(NULL, &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(stack_push(stack, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
}

void stack_remove_TTF() {
  i = 1;
  stack = stack_new(size_int);
  // NULL stack
  CU_ASSERT_EQUAL(stack_remove(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(stack_remove(stack, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
}

void stack_removeall_TTF() {
  i = 1;
  stack = stack_new(size_int);
  // NULL stack
  CU_ASSERT_EQUAL(stack_removeall(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(stack_removeall(stack, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
}

void stack_removeat_TTF() {
  i = 1;
  stack = stack_new(size_int);
  // NULL stack
  CU_ASSERT_EQUAL(stack_removeat(NULL, 1), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Bad index
  i = 2;
  stack_add(stack, &i);
  i = 4;
  stack_add(stack, &i);
  i = 6;
  stack_add(stack, &i);
  CU_ASSERT_EQUAL(stack_removeat(stack, -2), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  CU_ASSERT_EQUAL(stack_removeat(stack, 4), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  stack_destroy(&stack);
}

void stack_removefirst_TTF() {
  // NULL stack
  CU_ASSERT_EQUAL(stack_removefirst(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void stack_removelast_TTF() {
  // NULL stack
  CU_ASSERT_EQUAL(stack_removelast(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void stack_setat_TTF() {
  i = 1;
  // NULL stack
  CU_ASSERT_EQUAL(stack_setat(NULL, 0, &i, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  stack = stack_new(size_int);
  CU_ASSERT_EQUAL(stack_setat(stack, 0, NULL, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Bad index
  CU_ASSERT_EQUAL(stack_setat(stack, -1, &i, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  CU_ASSERT_EQUAL(stack_setat(stack, 0, &i, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_BADINDEX);
  stack_destroy(&stack);
}

void stack_size_TTF() {
  CU_ASSERT_EQUAL(stack_size(NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void stack_sort_TTF() {
  // NULL stack
  CU_ASSERT_EQUAL(stack_sort(NULL, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL compare function
  stack = stack_new(size_int);
  CU_ASSERT_EQUAL(stack_sort(stack, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
}

void stack_toarray_TTF() {
  // NULL stack
  CU_ASSERT_PTR_NULL(stack_toarray(NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void stack_tostring_TTF() {
  // NULL stack
  CU_ASSERT_EQUAL(stack_tostring(NULL, inttostring), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL tostring function
  stack = stack_new(size_int);
  CU_ASSERT_EQUAL(stack_tostring(stack, NULL), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  stack_destroy(&stack);
}


/*
 ********************************************************
 ************************* Main *************************
 ********************************************************
 */
int main(void) {
  CU_pSuite pSuite = NULL;
  int num_failures;

  // Initialize the CUnit test registry
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /*
   *********************************
   * Linked list suite
   *********************************
   */
  pSuite = CU_add_suite("Linked List", init_suite, clean_suite);
  if (pSuite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (CU_add_test(pSuite, "linkedlist_new_destroy_TTP", linkedlist_new_destroy_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_destroy_TTF", linkedlist_destroy_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_add_TTP", linkedlist_add_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_add_TTF", linkedlist_add_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_addv_TTP", linkedlist_addv_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_addv_TTF", linkedlist_addv_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_addall_TTP", linkedlist_addall_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_addall_TTF", linkedlist_addall_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_addfirst_TTP", linkedlist_addfirst_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_addfirst_TTF", linkedlist_addfirst_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_clear_TTP", linkedlist_clear_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_clear_TTF", linkedlist_clear_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_clone_TTP", linkedlist_clone_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_clone_TTF", linkedlist_clone_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_cmp_TTP", linkedlist_cmp_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_cmp_TTF", linkedlist_cmp_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_contains_TTP", linkedlist_contains_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_contains_TTF", linkedlist_contains_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_count_TTP", linkedlist_count_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_count_TTF", linkedlist_count_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_empty_TTP", linkedlist_empty_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_empty_TTF", linkedlist_empty_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_finddup_TTP", linkedlist_finddup_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_finddup_TTF", linkedlist_finddup_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_fromarray_TTP", linkedlist_fromarray_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_fromarray_TTF", linkedlist_fromarray_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_issorted_TTP", linkedlist_issorted_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_issorted_TTF", linkedlist_issorted_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_peekfirst_TTP", linkedlist_peekfirst_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_peekfirst_TTF", linkedlist_peekfirst_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_peeklast_TTP", linkedlist_peeklast_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_peeklast_TTF", linkedlist_peeklast_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_remove_TTP", linkedlist_remove_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_remove_TTF", linkedlist_remove_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_removeall_TTP", linkedlist_removeall_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_removeall_TTF", linkedlist_removeall_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_removefirst_TTP", linkedlist_removefirst_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_removefirst_TTF", linkedlist_removefirst_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_removelast_TTP", linkedlist_removelast_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_removelast_TTF", linkedlist_removelast_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_size_TTP", linkedlist_size_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_size_TTF", linkedlist_size_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_sort_TTP", linkedlist_sort_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_sort_TTF", linkedlist_sort_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_toarray_TTP", linkedlist_toarray_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_toarray_TTF", linkedlist_toarray_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_tostring_TTP", linkedlist_tostring_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_tostring_TTF", linkedlist_tostring_TTF) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /*
   *********************************
   * Array list suite
   *********************************
   */
  pSuite = CU_add_suite("Array List", init_suite, clean_suite);
  if (pSuite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (CU_add_test(pSuite, "arraylist_new_destroy_TTP", arraylist_new_destroy_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_destroy_TTF", arraylist_destroy_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_add_TTP", arraylist_add_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_add_TTF", arraylist_add_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_addv_TTP", arraylist_addv_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_addv_TTF", arraylist_addv_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_addall_TTP", arraylist_addall_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_addall_TTF", arraylist_addall_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_addat_TTP", arraylist_addat_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_addat_TTF", arraylist_addat_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_addfirst_TTP", arraylist_addfirst_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_addfirst_TTF", arraylist_addfirst_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_clear_TTP", arraylist_clear_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_clear_TTF", arraylist_clear_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_clone_TTP", arraylist_clone_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_clone_TTF", arraylist_clone_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_cmp_TTP", arraylist_cmp_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_cmp_TTF", arraylist_cmp_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_contains_TTP", arraylist_contains_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_contains_TTF", arraylist_contains_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_count_TTP", arraylist_count_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_count_TTF", arraylist_count_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_empty_TTP", arraylist_empty_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_empty_TTF", arraylist_empty_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_finddup_TTP", arraylist_finddup_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_finddup_TTF", arraylist_finddup_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_fromarray_TTP", arraylist_fromarray_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_fromarray_TTF", arraylist_fromarray_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_get_TTP", arraylist_get_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_get_TTF", arraylist_get_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_indexof_TTP", arraylist_indexof_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_indexof_TTF", arraylist_indexof_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_issorted_TTP", arraylist_issorted_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_issorted_TTF", arraylist_issorted_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_lastindexof_TTP", arraylist_lastindexof_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_lastindexof_TTF", arraylist_lastindexof_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_peekfirst_TTP", arraylist_peekfirst_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_peekfirst_TTF", arraylist_peekfirst_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_peeklast_TTP", arraylist_peeklast_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_peeklast_TTF", arraylist_peeklast_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_remove_TTP", arraylist_remove_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_remove_TTF", arraylist_remove_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_removeat_TTP", arraylist_removeat_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_removeat_TTF", arraylist_removeat_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_removeall_TTP", arraylist_removeall_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_removeall_TTF", arraylist_removeall_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_removefirst_TTP", arraylist_removefirst_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_removefirst_TTF", arraylist_removefirst_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_removelast_TTP", arraylist_removelast_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_removelast_TTF", arraylist_removelast_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_setat_TTP", arraylist_setat_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_setat_TTF", arraylist_setat_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_size_TTP", arraylist_size_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_size_TTF", arraylist_size_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_sort_TTP", arraylist_sort_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_sort_TTF", arraylist_sort_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_toarray_TTP", arraylist_toarray_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_toarray_TTF", arraylist_toarray_TTF) == NULL ||
      CU_add_test(pSuite, "arraylist_tostring_TTP", arraylist_tostring_TTP) == NULL ||
      CU_add_test(pSuite, "arraylist_tostring_TTF", arraylist_tostring_TTF) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /*
   *********************************
   * Stack suite
   *********************************
   */
  pSuite = CU_add_suite("Stack", init_suite, clean_suite);
  if (pSuite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (CU_add_test(pSuite, "stack_new_destroy_TTP", stack_new_destroy_TTP) == NULL ||
      CU_add_test(pSuite, "stack_destroy_TTF", stack_destroy_TTF) == NULL ||
      CU_add_test(pSuite, "stack_add_TTP", stack_add_TTP) == NULL ||
      CU_add_test(pSuite, "stack_add_TTF", stack_add_TTF) == NULL ||
      CU_add_test(pSuite, "stack_addv_TTP", stack_addv_TTP) == NULL ||
      CU_add_test(pSuite, "stack_addv_TTF", stack_addv_TTF) == NULL ||
      CU_add_test(pSuite, "stack_addall_TTP", stack_addall_TTP) == NULL ||
      CU_add_test(pSuite, "stack_addall_TTF", stack_addall_TTF) == NULL ||
      CU_add_test(pSuite, "stack_addat_TTP", stack_addat_TTP) == NULL ||
      CU_add_test(pSuite, "stack_addat_TTF", stack_addat_TTF) == NULL ||
      CU_add_test(pSuite, "stack_addfirst_TTP", stack_addfirst_TTP) == NULL ||
      CU_add_test(pSuite, "stack_addfirst_TTF", stack_addfirst_TTF) == NULL ||
      CU_add_test(pSuite, "stack_clear_TTP", stack_clear_TTP) == NULL ||
      CU_add_test(pSuite, "stack_clear_TTF", stack_clear_TTF) == NULL ||
      CU_add_test(pSuite, "stack_clone_TTP", stack_clone_TTP) == NULL ||
      CU_add_test(pSuite, "stack_clone_TTF", stack_clone_TTF) == NULL ||
      CU_add_test(pSuite, "stack_cmp_TTP", stack_cmp_TTP) == NULL ||
      CU_add_test(pSuite, "stack_cmp_TTF", stack_cmp_TTF) == NULL ||
      CU_add_test(pSuite, "stack_contains_TTP", stack_contains_TTP) == NULL ||
      CU_add_test(pSuite, "stack_contains_TTF", stack_contains_TTF) == NULL ||
      CU_add_test(pSuite, "stack_count_TTP", stack_count_TTP) == NULL ||
      CU_add_test(pSuite, "stack_count_TTF", stack_count_TTF) == NULL ||
      CU_add_test(pSuite, "stack_empty_TTP", stack_empty_TTP) == NULL ||
      CU_add_test(pSuite, "stack_empty_TTF", stack_empty_TTF) == NULL ||
      CU_add_test(pSuite, "stack_finddup_TTP", stack_finddup_TTP) == NULL ||
      CU_add_test(pSuite, "stack_finddup_TTF", stack_finddup_TTF) == NULL ||
      CU_add_test(pSuite, "stack_fromarray_TTP", stack_fromarray_TTP) == NULL ||
      CU_add_test(pSuite, "stack_fromarray_TTF", stack_fromarray_TTF) == NULL ||
      CU_add_test(pSuite, "stack_get_TTP", stack_get_TTP) == NULL ||
      CU_add_test(pSuite, "stack_get_TTF", stack_get_TTF) == NULL ||
      CU_add_test(pSuite, "stack_indexof_TTP", stack_indexof_TTP) == NULL ||
      CU_add_test(pSuite, "stack_indexof_TTF", stack_indexof_TTF) == NULL ||
      CU_add_test(pSuite, "stack_issorted_TTP", stack_issorted_TTP) == NULL ||
      CU_add_test(pSuite, "stack_issorted_TTF", stack_issorted_TTF) == NULL ||
      CU_add_test(pSuite, "stack_lastindexof_TTP", stack_lastindexof_TTP) == NULL ||
      CU_add_test(pSuite, "stack_lastindexof_TTF", stack_lastindexof_TTF) == NULL ||
      CU_add_test(pSuite, "stack_peekfirst_TTP", stack_peekfirst_TTP) == NULL ||
      CU_add_test(pSuite, "stack_peekfirst_TTF", stack_peekfirst_TTF) == NULL ||
      CU_add_test(pSuite, "stack_peeklast_TTP", stack_peeklast_TTP) == NULL ||
      CU_add_test(pSuite, "stack_peeklast_TTF", stack_peeklast_TTF) == NULL ||
      CU_add_test(pSuite, "stack_pop_TTP", stack_pop_TTP) == NULL ||
      CU_add_test(pSuite, "stack_pop_TTF", stack_pop_TTF) == NULL ||
      CU_add_test(pSuite, "stack_push_TTP", stack_push_TTP) == NULL ||
      CU_add_test(pSuite, "stack_push_TTF", stack_push_TTF) == NULL ||
      CU_add_test(pSuite, "stack_remove_TTP", stack_remove_TTP) == NULL ||
      CU_add_test(pSuite, "stack_remove_TTF", stack_remove_TTF) == NULL ||
      CU_add_test(pSuite, "stack_removeat_TTP", stack_removeat_TTP) == NULL ||
      CU_add_test(pSuite, "stack_removeat_TTF", stack_removeat_TTF) == NULL ||
      CU_add_test(pSuite, "stack_removeall_TTP", stack_removeall_TTP) == NULL ||
      CU_add_test(pSuite, "stack_removeall_TTF", stack_removeall_TTF) == NULL ||
      CU_add_test(pSuite, "stack_removefirst_TTP", stack_removefirst_TTP) == NULL ||
      CU_add_test(pSuite, "stack_removefirst_TTF", stack_removefirst_TTF) == NULL ||
      CU_add_test(pSuite, "stack_removelast_TTP", stack_removelast_TTP) == NULL ||
      CU_add_test(pSuite, "stack_removelast_TTF", stack_removelast_TTF) == NULL ||
      CU_add_test(pSuite, "stack_setat_TTP", stack_setat_TTP) == NULL ||
      CU_add_test(pSuite, "stack_setat_TTF", stack_setat_TTF) == NULL ||
      CU_add_test(pSuite, "stack_size_TTP", stack_size_TTP) == NULL ||
      CU_add_test(pSuite, "stack_size_TTF", stack_size_TTF) == NULL ||
      CU_add_test(pSuite, "stack_sort_TTP", stack_sort_TTP) == NULL ||
      CU_add_test(pSuite, "stack_sort_TTF", stack_sort_TTF) == NULL ||
      CU_add_test(pSuite, "stack_toarray_TTP", stack_toarray_TTP) == NULL ||
      CU_add_test(pSuite, "stack_toarray_TTF", stack_toarray_TTF) == NULL ||
      CU_add_test(pSuite, "stack_tostring_TTP", stack_tostring_TTP) == NULL ||
      CU_add_test(pSuite, "stack_tostring_TTF", stack_tostring_TTF) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Run all tests using the CUnit Basic interface
  CU_basic_set_mode((DEBUG ? CU_BRM_VERBOSE : CU_BRM_NORMAL));
  CU_basic_run_tests();
  printf("\n\n");
  CU_basic_show_failures(CU_get_failure_list());
  printf("\n");
  num_failures = CU_get_number_of_failures();
  CU_cleanup_registry();
  return num_failures;
}

