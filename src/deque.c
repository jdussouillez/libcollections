#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collections/deque.h"
#include "collections/errors.h"

int deque_addall(deque_t* dest, deque_t* src) {
  return llist_addall(dest, src);
}


int deque_addfirst(deque_t* deque, void* e) {
  return llist_addfirst(deque, e);
}


int deque_addlast(deque_t* deque, void* e) {
  return llist_add(deque, e);
}


void deque_clear(deque_t* deque) {
  llist_clear(deque);
}


deque_t* deque_clone(deque_t* deque) {
  return llist_clone(deque);
}


int deque_cmp(deque_t* d1, deque_t* d2, comparefct_t compare) {
  return llist_cmp(d1, d2, compare);
}


int deque_contains(deque_t* deque, void* e, comparefct_t compare) {
  return deque_count(deque, e, compare) > 0;
}


int deque_count(deque_t* deque, void* e, comparefct_t compare) {
  return llist_count(deque, e, compare);
}


void deque_destroy(deque_t** deque) {
  llist_destroy(deque);
}


int deque_empty(deque_t* deque) {
  return llist_empty(deque);
}


deque_t* deque_fromarray(void* array, int size, size_t data_size) {
  return llist_fromarray(array, size, data_size);
}


deque_t* deque_new(size_t data_size) {
  return llist_new(data_size);
}


void* deque_peekfirst(deque_t* deque) {
  return llist_peekfirst(deque);
}


void* deque_peeklast(deque_t* deque) {
  return llist_peeklast(deque);
}


void* deque_pollfirst(deque_t* deque) {
  void* e;
  if (deque == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  if (deque->size == 0)
    return NULL;
  if ((e = malloc(deque->data_size)) == NULL) {
    cerrno = CERR_SYSTEM;
    return NULL;
  }
  memcpy(e, deque->head->data, deque->data_size);
  if (deque_removefirst(deque) < 0) {
    free(e);
    return NULL;
  }
  return e;
}


void* deque_polllast(deque_t* deque) {
  void* e;
  if (deque == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  if (deque->size == 0)
    return NULL;
  if ((e = malloc(deque->data_size)) == NULL) {
    cerrno = CERR_SYSTEM;
    return NULL;
  }
  memcpy(e, deque->tail->data, deque->data_size);
  if (deque_removelast(deque) < 0) {
    free(e);
    return NULL;
  }
  return e;
}


int deque_removeall(deque_t* deque, void* e, comparefct_t compare) {
  return llist_removeall(deque, e, compare);
}


int deque_removefirst(deque_t* deque) {
  return llist_removefirst(deque);
}


int deque_removelast(deque_t* deque) {
  return llist_removelast(deque);
}


int deque_size(deque_t* deque) {
  return llist_size(deque);
}


void* deque_toarray(deque_t* deque) {
  return llist_toarray(deque);
}


char* deque_tostring(deque_t* deque, tostringfct_t tostring) {
  return llist_tostring(deque, tostring);
}

