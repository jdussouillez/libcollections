#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collections/errors.h"
#include "collections/linkedlist.h"

#include "CUnit/Basic.h"

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
linkedlist_t* list;

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
  list = llist_new(size_int);
  CU_ASSERT_PTR_NOT_NULL(list);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&list);
  CU_ASSERT_PTR_NULL(list);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
}

void linkedlist_add_TTP() {
  list = llist_new(size_int);
  // Add an element when the list is empty
  i = 1;
  CU_ASSERT_TRUE(llist_add(list, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(list->size, 1);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(list), &i), 0);
  // Add an element when the list is not empty
  i = 2;
  CU_ASSERT_TRUE(llist_add(list, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(list->size, 2);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(list), &i), 0);
  llist_destroy(&list);
}

void linkedlist_addall_TTP() {
  linkedlist_t* list2 = llist_new(size_int);
  i = 1;
  llist_add(list2, &i);
  i = 2;
  llist_add(list2, &i);
  list = llist_new(size_int);
  // list = [], list2 = [1, 2]
  llist_addall(list, list2);
  // list = [1, 2], list2 = [1, 2]
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(list->size, 2);
  // Check elemens of the list
  // i = 2
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(list), &i), 0);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(list), &i), 0);
  llist_destroy(&list2);
  llist_destroy(&list);
}

void linkedlist_addfirst_TTP() {
  list = llist_new(size_int);
  // Add an element at the beginning of an empty list
  i = 1;
  CU_ASSERT_TRUE(llist_addfirst(list, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(list), &i), 0);
  // Add an element at the beginning of a non-empty list
  i = 2;
  CU_ASSERT_TRUE(llist_addfirst(list, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(list), &i), 0);
  llist_destroy(&list);
}

void linkedlist_addv_TTP() {
  int j;
  list = llist_new(size_int);
  // Add 3 elements
  i = 1;
  j = 2;
  CU_ASSERT_EQUAL(llist_addv(list, 3, &i, &i, &j), 3);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(list->size, 3);
  // list = [1, 1, 2]
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(list), &i), 0);
  CU_ASSERT_EQUAL(compare_int(list->head->next->data, &i), 0);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(list), &j), 0);
  llist_destroy(&list);
}

void linkedlist_clear_TTP() {
  list = llist_new(size_int);
  i = 1;
  // Add 3 elements and clear the list
  llist_addv(list, 3, &i, &i, &i);
  llist_clear(list);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(list->size, 0);
  llist_destroy(&list);
}

void linkedlist_clone_TTP() {
  linkedlist_t* list2;
  list = llist_new(size_int);
  i = 1;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  list2 = llist_clone(list);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(list2);
  CU_ASSERT_EQUAL(list->size, list2->size);
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(list), llist_peekfirst(list2)), 0);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(list), llist_peeklast(list2)), 0);
  llist_destroy(&list);
  llist_destroy(&list2);
}

void linkedlist_cmp_TTP() {
  linkedlist_t* list2 = llist_new(size_int);
  // Different size of data
  list = llist_new(sizeof(float));
  CU_ASSERT_EQUAL(llist_cmp(list, list2, compare_int), (sizeof(float) - size_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&list);
  // Different size
  list = llist_new(size_int); // list = [] (size = 0)
  i = 3;
  llist_add(list2, &i); // list2 = [3] (size = 1)
  CU_ASSERT_EQUAL(llist_cmp(list, list2, compare_int), -1); // 0 - 1 = -1
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Different elements
  i = 7;
  llist_add(list, &i); // list = [7], list2 = [3]
  CU_ASSERT_EQUAL(llist_cmp(list2, list, compare_int), (3 - 7));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Same lists
  llist_add(list2, &i); // list2 = [3, 7]
  i = 3;
  llist_addfirst(list, &i); // list = [3, 7]
  CU_ASSERT_EQUAL(llist_cmp(list, list2, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&list);
  llist_destroy(&list2);
}

void linkedlist_contains_TTP() {
  list = llist_new(size_int);
  // The list is empty (so 4 is not found)
  i = 4;
  CU_ASSERT_FALSE(llist_contains(list, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // list = [1, 2]
  i = 1;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  // The list does not contain 4
  i = 4;
  CU_ASSERT_FALSE(llist_contains(list, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // The list contains 2
  i = 2;
  CU_ASSERT_TRUE(llist_contains(list, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&list);
}

void linkedlist_count_TTP() {
  list = llist_new(size_int);
  i = 0;
  llist_add(list, &i);
  i = 1;
  llist_addv(list, 2, &i, &i);
  // list = [0, 1, 1]
  // The list contains twice the element 1
  CU_ASSERT_EQUAL(llist_count(list, &i, compare_int), 2);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 2;
  // The list does not contain an element 2
  CU_ASSERT_EQUAL(llist_count(list, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&list);
}

void linkedlist_finddup_TTP() {
  linkedlist_t* duplist = llist_new(size_int);
  list = llist_new(size_int);
  i = 1;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  i = 3;
  llist_add(list, &i);
  // No duplicates (list = [1, 2, 3] --> duplist = [], returned value = 0)
  CU_ASSERT_EQUAL(llist_finddup(list, duplist, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Duplicates (list = [1, 2, 3, 3, 3, 1] --> duplist = [1, 3], returned value = 2)
  llist_add(list, &i);
  llist_add(list, &i);
  i = 1;
  llist_add(list, &i);
  CU_ASSERT_EQUAL(llist_finddup(list, duplist, compare_int), 2);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Check duplist elements
  CU_ASSERT_EQUAL(duplist->size, 2);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(duplist), &i), 0);
  i = 3;
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(duplist), &i), 0);
  llist_destroy(&duplist);
  llist_destroy(&list);
}

void linkedlist_fromarray_TTP() {
  int i = 1, j = 2;
  int array[2] = {i, j};
  list = llist_fromarray(array, 2, sizeof(int));
  CU_ASSERT_PTR_NOT_NULL(list);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(list->size, 2);
  // Check values
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(list), &i), 0);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(list), &j), 0);
  llist_destroy(&list);
}

void linkedlist_issorted_TTP() {
  list = llist_new(size_int);
  i = 1;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  i = 3;
  llist_add(list, &i);
  // On a sorted list ([1, 2, 3])
  CU_ASSERT_EQUAL(llist_issorted(list, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  i = 1;
  llist_add(list, &i);
  i = -5;
  llist_add(list, &i);
  // On a non-sorted list ([1, 2, 3, 1, -5])
  CU_ASSERT_EQUAL(llist_issorted(list, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&list);
}

void linkedlist_peekfirst_TTP() {
  list = llist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(llist_peekfirst(list), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a list of 2 elements
  i = 1;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(list), &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&list);
}

void linkedlist_peeklast_TTP() {
  list = llist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(llist_peeklast(list), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a list of 2 elements
  i = 1;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(list), &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&list);
}

void linkedlist_remove_TTP() {
  list = llist_new(size_int);
  // Remove an element which is in the list
  i = 1;
  llist_add(list, &i);
  CU_ASSERT_EQUAL(list->size, 1);
  CU_ASSERT_EQUAL(llist_remove(list, &i, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(list->size, 0);
  // Remove an element in an empty list
  CU_ASSERT_EQUAL(llist_remove(list, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove an element which is not in the list (non empty list)
  llist_add(list, &i);
  i = 2;
  CU_ASSERT_EQUAL(list->size, 1);
  CU_ASSERT_EQUAL(llist_remove(list, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(list->size, 1);
  llist_destroy(&list);
}

void linkedlist_removeall_TTP() {
  list = llist_new(size_int);
  // Remove all the elements 1 in the list (2 elements removed)
  i = 2;
  llist_add(list, &i);
  i = 1;
  llist_add(list, &i);
  llist_add(list, &i);
  // list = [2, 1, 1]
  CU_ASSERT_EQUAL(list->size, 3);
  CU_ASSERT_EQUAL(llist_removeall(list, &i, compare_int), 2);
  CU_ASSERT_EQUAL(list->size, 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove all the elements 1 in the list (no elements removed), list = [2]
  i = 1;
  CU_ASSERT_EQUAL(llist_removeall(list, &i, compare_int), 0);
  CU_ASSERT_EQUAL(list->size, 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Remove on an empty list
  llist_clear(list);
  CU_ASSERT_EQUAL(llist_removeall(list, &i, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  llist_destroy(&list);
}

void linkedlist_removefirst_TTP() {
  list = llist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(llist_removefirst(list), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a non-empty list
  i = 1;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  CU_ASSERT_EQUAL(llist_removefirst(list), 1);
  CU_ASSERT_EQUAL(list->size, 1);
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(list), &i), 0);
  llist_destroy(&list);
}

void linkedlist_removelast_TTP() {
  list = llist_new(size_int);
  // On an empty list
  CU_ASSERT_EQUAL(llist_removelast(list), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // On a non-empty list
  i = 1;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  CU_ASSERT_EQUAL(llist_removelast(list), 1);
  CU_ASSERT_EQUAL(list->size, 1);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(list), &i), 0);
  llist_destroy(&list);
}

void linkedlist_sort_TTP() {
  list = llist_new(size_int);
  // Sort an empty list
  CU_ASSERT_EQUAL(llist_sort(list, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Already sorted, list = [1, 2] --> list = [1, 2]
  i = 1;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  CU_ASSERT_EQUAL(llist_sort(list, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(compare_int(llist_peeklast(list), &i), 0);
  i = 1;
  CU_ASSERT_EQUAL(compare_int(llist_peekfirst(list), &i), 0);
  // Sorted by llist_sort(), list = [1, 2, -5, 9, 2] --> list = [-5, 1, 2, 2, 9]
  i = -5;
  llist_add(list, &i);
  i = 9;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  CU_ASSERT_EQUAL(llist_sort(list, compare_int), 1);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_TRUE(llist_issorted(list, compare_int));
  llist_destroy(&list);
}

void linkedlist_toarray_TTP() {
  void* array = NULL;
  list = llist_new(size_int);
  // Empty list
  array = llist_toarray(list);
  CU_ASSERT_PTR_NOT_NULL(array);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  free(array);
  array = NULL;
  // Non-empty list
  i = 1;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  array = llist_toarray(list);
  CU_ASSERT_PTR_NOT_NULL(array);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  // Check values
  i = 1;
  CU_ASSERT_EQUAL(compare_int(array, &i), 0);
  i = 2;
  CU_ASSERT_EQUAL(compare_int(array + list->data_size, &i), 0);
  free(array);
  llist_destroy(&list);
}

void linkedlist_tostring_TTP() {
  char* str;
  list = llist_new(size_int);
  // On an empty list
  str = llist_tostring(list, inttostring);
  CU_ASSERT_PTR_NOT_NULL(str);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(strcmp(str, "[]"), 0);
  free(str);
  str = NULL;
  // On a non-empty list
  i = 1;
  llist_add(list, &i);
  i = 2;
  llist_add(list, &i);
  i = 4;
  llist_add(list, &i);
  i = 8;
  llist_add(list, &i);
  i = 16;
  llist_add(list, &i);
  str = llist_tostring(list, inttostring);
  CU_ASSERT_PTR_NOT_NULL(str);
  CU_ASSERT_EQUAL(cerrno, CERR_SUCCESS);
  CU_ASSERT_EQUAL(strcmp(str, "[1, 2, 4, 8, 16]"), 0);
  free(str);
  llist_destroy(&list);
}

/*
 ********************************
 * Test-to-fail
 ********************************
 */
void linkedlist_destroy_TTF() {
  // NULL list and NULL pointer
  list = NULL;
  llist_destroy(&list);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_add_TTF() {
  list = llist_new(size_int);
  // Add a NULL element
  CU_ASSERT_FALSE(llist_add(list, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(list->size, 0);
  // Add an element to NULL
  i = 1;
  CU_ASSERT_FALSE(llist_add(NULL, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Both parameters are NULL
  CU_ASSERT_FALSE(llist_add(NULL, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&list);
}

void linkedlist_addall_TTF() {
  list = llist_new(size_int);
  // Source list is NULL
  CU_ASSERT_FALSE(llist_addall(list, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Destination list is NULL
  CU_ASSERT_FALSE(llist_addall(NULL, list));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Both parameters are NULL
  CU_ASSERT_FALSE(llist_addall(NULL, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Addall on the same list is forbidden (infinite loop if not forbidden)
  CU_ASSERT_FALSE(llist_addall(list, list));
  CU_ASSERT_EQUAL(cerrno, CERR_FORBIDDEN);
  llist_destroy(&list);
}

void linkedlist_addfirst_TTF() {
  list = llist_new(size_int);
  // Add NULL at the beginning of the list
  CU_ASSERT_FALSE(llist_addfirst(list, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // Add a value at the beginning of NULL
  i = 1;
  CU_ASSERT_FALSE(llist_addfirst(NULL, &i));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&list);
}

void linkedlist_addv_TTF() {
  // NULL list
  i = 1;
  CU_ASSERT_EQUAL(llist_addv(NULL, 1, &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  list = llist_new(size_int);
  CU_ASSERT_EQUAL(llist_addv(list, 3, &i, NULL, &i), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  CU_ASSERT_EQUAL(list->size, 0);
  llist_destroy(&list);
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
  linkedlist_t* list2 = llist_new(size_int);
  list = llist_new(size_int);
  // NULL list 1
  CU_ASSERT_EQUAL(llist_cmp(NULL, list2, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL list 2
  CU_ASSERT_EQUAL(llist_cmp(list, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL compare function
  CU_ASSERT_EQUAL(llist_cmp(list, list2, NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&list);
  llist_destroy(&list2);
}

void linkedlist_contains_TTF() {
  // NULL list
  i = 2;
  CU_ASSERT_FALSE(llist_contains(NULL, &i, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  list = llist_new(size_int);
  CU_ASSERT_FALSE(llist_contains(list, NULL, compare_int));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&list);
  // NULL compare function
  CU_ASSERT_FALSE(llist_contains(list, &i, NULL));
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
}

void linkedlist_count_TTF() {
  i = 1;
  list = llist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(llist_count(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(llist_count(list, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL compare function
  CU_ASSERT_EQUAL(llist_count(list, &i, NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&list);
}

void linkedlist_finddup_TTF() {
  linkedlist_t* duplist = llist_new(size_int);
  i = 1;
  list = llist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(llist_finddup(NULL, list, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL duplicate list
  CU_ASSERT_EQUAL(llist_finddup(list, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL compare function
  CU_ASSERT_EQUAL(llist_finddup(list, duplist, NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&list);
  llist_destroy(&duplist);
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
  // NULL compare function
  list = llist_new(size_int);
  CU_ASSERT_EQUAL(llist_issorted(list, NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&list);
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
  list = llist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(llist_remove(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(llist_remove(list, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL compare function
  CU_ASSERT_EQUAL(llist_remove(list, &i, NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&list);
}

void linkedlist_removeall_TTF() {
  i = 1;
  list = llist_new(size_int);
  // NULL list
  CU_ASSERT_EQUAL(llist_removeall(NULL, &i, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL element
  CU_ASSERT_EQUAL(llist_removeall(list, NULL, compare_int), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL compare function
  CU_ASSERT_EQUAL(llist_removeall(list, &i, NULL), -1);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&list);
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

void linkedlist_sort_TTF() {
  // NULL list
  CU_ASSERT_EQUAL(llist_sort(NULL, compare_int), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  // NULL compare function
  list = llist_new(size_int);
  CU_ASSERT_EQUAL(llist_sort(list, NULL), 0);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&list);
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
  list = llist_new(size_int);
  CU_ASSERT_EQUAL(llist_tostring(list, NULL), NULL);
  CU_ASSERT_EQUAL(cerrno, CERR_NULLVALUE);
  llist_destroy(&list);
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
   * Linked list suite
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
      CU_add_test(pSuite, "linkedlist_removefirst_TTP", linkedlist_removeall_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_removefirst_TTF", linkedlist_removeall_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_removelast_TTP", linkedlist_removeall_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_removelast_TTF", linkedlist_removeall_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_sort_TTP", linkedlist_sort_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_sort_TTF", linkedlist_sort_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_toarray_TTP", linkedlist_toarray_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_toarray_TTF", linkedlist_toarray_TTF) == NULL ||
      CU_add_test(pSuite, "linkedlist_tostring_TTP", linkedlist_tostring_TTP) == NULL ||
      CU_add_test(pSuite, "linkedlist_tostring_TTF", linkedlist_tostring_TTF) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Run all tests using the CUnit Basic interface
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  printf("\n\n");
  CU_basic_show_failures(CU_get_failure_list());
  printf("\n");
  num_failures = CU_get_number_of_failures();
  CU_cleanup_registry();
  return num_failures;
}
