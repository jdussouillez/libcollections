#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collections/errors.h"
#include "collections/queue.h"

int queue_addall(queue_t* dest, queue_t* src) {
  return deque_addall(dest, src);
}


int queue_add(queue_t* queue, void* e) {
  return deque_addfirst(queue, e);
}


void queue_clear(queue_t* queue) {
  deque_clear(queue);
}


queue_t* queue_clone(queue_t* queue) {
  return deque_clone(queue);
}


int queue_cmp(queue_t* d1, queue_t* d2, comparefct_t compare) {
  return deque_cmp(d1, d2, compare);
}


int queue_contains(queue_t* queue, void* e, comparefct_t compare) {
  return queue_count(queue, e, compare) > 0;
}


int queue_count(queue_t* queue, void* e, comparefct_t compare) {
  return deque_count(queue, e, compare);
}


void queue_destroy(queue_t** queue) {
  deque_destroy(queue);
}


int queue_empty(queue_t* queue) {
  return deque_empty(queue);
}


queue_t* queue_fromarray(void* array, int size, size_t data_size) {
  return deque_fromarray(array, size, data_size);
}


queue_t* queue_new(size_t data_size) {
  return deque_new(data_size);
}


void* queue_peek(queue_t* queue) {
  return deque_peeklast(queue);
}


void* queue_poll(queue_t* queue) {
  void* e;
  if (queue == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  if (queue->size == 0)
    return NULL;
  if ((e = malloc(queue->data_size)) == NULL) {
    cerrno = CERR_SYSTEM;
    return NULL;
  }
  memcpy(e, queue->tail->data, queue->data_size);
  if (deque_removefirst(queue) < 0) {
    free(e);
    return NULL;
  }
  return e;
}


int queue_removeall(queue_t* queue, void* e, comparefct_t compare) {
  return deque_removeall(queue, e, compare);
}


int queue_remove(queue_t* queue) {
  return deque_removelast(queue);
}


int queue_size(queue_t* queue) {
  return deque_size(queue);
}


void* queue_toarray(queue_t* queue) {
  return deque_toarray(queue);
}


char* queue_tostring(queue_t* queue, tostringfct_t tostring) {
  return deque_tostring(queue, tostring);
}

