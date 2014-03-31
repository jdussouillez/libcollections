#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "collections/arraylist.h"
#include "collections/errors.h"
#include "collections/stack.h"

int stack_add(stack_t* stack, void* e) {
  return alist_add(stack, e);
}


int stack_addall(stack_t* dest, stack_t* src) {
  return alist_addall(dest, src);
}


int stack_addat(stack_t *stack, int index, void* e) {
  return alist_addat(stack, index, e);
}


int stack_addfirst(stack_t* stack, void* e) {
  return alist_addfirst(stack, e);
}


int stack_addv(stack_t* stack, int nbargs, ...) {
  // TODO:
  return -1;
}


void stack_clear(stack_t* stack) {
  alist_clear(stack);
}


stack_t* stack_clone(stack_t* stack) {
  return alist_clone(stack);
}


int stack_cmp(stack_t* l1, stack_t* l2, comparefct_t compare) {
  return alist_cmp(l1, l2, compare);
}


int stack_contains(stack_t* stack, void* e, comparefct_t compare) {
  return alist_contains(stack, e, compare);
}


int stack_count(stack_t* stack, void* e, comparefct_t compare) {
  return alist_count(stack, e, compare);
}


void stack_destroy(stack_t** stack) {
  alist_destroy(stack);
}


int stack_empty(stack_t* stack) {
  if (stack == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  return (stack->size == 0) ? 1 : 0;
}


int stack_finddup(stack_t* stack, stack_t* dupstack, comparefct_t compare) {
  return alist_finddup(stack, dupstack, compare);
}


stack_t* stack_fromarray(void* array, int size, size_t data_size) {
  return alist_fromarray(array, size, data_size);
}


void* stack_get(stack_t* stack, int index) {
  return alist_get(stack, index);
}


int stack_indexof(stack_t* stack, void* e, comparefct_t compare) {
  return alist_indexof(stack, e, compare);
}


int stack_issorted(stack_t* stack, comparefct_t compare) {
  return alist_issorted(stack, compare);
}


stack_t* stack_new(size_t data_size) {
  return alist_new(data_size);
}


void* stack_peek(stack_t* stack) {
  return stack_peeklast(stack);
}


void* stack_peekfirst(stack_t* stack) {
  return alist_peekfirst(stack);
}


void* stack_peeklast(stack_t* stack) {
  return alist_peeklast(stack);
}


void* stack_pop(stack_t* stack) {
  // TODO:
  return NULL;
}


int stack_push(stack_t* stack, void* e) {
  return stack_add(stack, e);
}


int stack_remove(stack_t* stack, void* e, comparefct_t compare) {
  return alist_remove(stack, e, compare);
}


int stack_removeall(stack_t* stack, void* e, comparefct_t compare) {
  return alist_removeall(stack, e, compare);
}


int stack_removeat(stack_t* stack, int index) {
  return alist_removeat(stack, index);
}


int stack_removefirst(stack_t* stack) {
  return alist_removefirst(stack);
}


int stack_removelast(stack_t* stack) {
  return alist_removelast(stack);
}


void* stack_setat(stack_t* stack, int index, void* e) {
  return alist_setat(stack, index, e);
}


int stack_sort(stack_t* stack, comparefct_t compare) {
  return alist_sort(stack, compare);
}


void* stack_toarray(stack_t* stack) {
  return alist_toarray(stack);
}


char* stack_tostring(stack_t* stack, tostringfct_t tostring) {
  return alist_tostring(stack, tostring);
}

