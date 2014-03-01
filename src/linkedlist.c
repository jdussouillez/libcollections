#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collections/errors.h"
#include "collections/linkedlist.h"

static int _llist_remove(linkedlist_t* list, void* e, comparefct_t compare, int removeall);

int llist_add(linkedlist_t* list, void* e) {
  linkedlistnode_t* node;
  if (list == NULL || e == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  if ((node = malloc(sizeof(linkedlistnode_t))) == NULL ||
      (node->data = malloc(list->data_size)) == NULL) {
    cerrno = CERR_SYSTEM;
    return 0;
  }
  memcpy(node->data, e, list->data_size);
  node->next = NULL;
  if (list->head == NULL) {
    // First element
    list->head = node;
    list->tail = node;
  }
  else {
    list->tail->next = node;
    list->tail = node;
  }
  list->size++;
  cerrno = CERR_SUCCESS;
  return 1;
}

int llist_addall(linkedlist_t* dest, linkedlist_t* src) {
  if (dest == NULL || src == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  if (src == dest) {
    // addall on the same list is forbidden (would cause infinite loop)
    cerrno = CERR_FORBIDDEN;
    return 0;
  }
  LLIST_FOREACH(src, {
      if (!llist_add(dest, data))
	return 0;
    });
  cerrno = CERR_SUCCESS;
  return 1;
}

int llist_addfirst(linkedlist_t* list, void* e) {
  linkedlistnode_t* node;
  if (list == NULL || e == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  if ((node = malloc(sizeof(linkedlistnode_t))) == NULL ||
      (node->data = malloc(list->data_size)) == NULL) {
    cerrno = CERR_SYSTEM;
    return 0;
  }
  memcpy(node->data, e, list->data_size);
  if (list->head == NULL) {
    // No element yet
    node->next = NULL;
    list->head = node;
    list->tail = node;
  }
  else {
    node->next = list->head;
    list->head = node;
  }
  list->size++;
  cerrno = CERR_SUCCESS;
  return 1;
}

void llist_clear(linkedlist_t* list) {
  int i;
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return;
  }
  int nbelem = list->size;
  for (i = 0; i < nbelem; i++) {
    llist_removefirst(list);
  }
  list->size = 0;
  cerrno = CERR_SUCCESS;
}

linkedlist_t* llist_clone(linkedlist_t* list) {
  linkedlist_t* clone;
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  clone = llist_new(list->data_size);
  if (clone == NULL)
    return NULL;
  if (!llist_addall(clone, list))
    return NULL;
  cerrno = CERR_SUCCESS;
  return clone;
}

int llist_cmp(linkedlist_t* l1, linkedlist_t* l2, comparefct_t compare) {
  linkedlistnode_t *nodel1, *nodel2;
  int cmp;
  if (l1 == NULL || l2 == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  // Compare size of data
  cmp = l1->data_size - l2->data_size;
  if (cmp != 0)
    return cmp;
  // Compare size of list
  cmp = l1->size - l2->size;
  if (cmp != 0)
    return cmp;
  // Compare elements
  nodel1 = l1->head;
  nodel2 = l2->head;
  while (nodel1 != NULL) {
    if ((cmp = compare(nodel1->data, nodel2->data)) != 0)
      return cmp;
    nodel1 = nodel1->next;
    nodel2 = nodel2->next;
  }
  return 0;
}

int llist_contains(linkedlist_t* list, void* e, comparefct_t compare) {
  return llist_count(list, e, compare) > 0;
}

int llist_count(linkedlist_t* list, void* e, comparefct_t compare) {
  int cpt = 0;
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  LLIST_FOREACH(list, {
      if (compare(data, e) == 0)
	cpt++;
    });
  cerrno = CERR_SUCCESS;
  return cpt;
}

void llist_destroy(linkedlist_t** list) {
  if (list == NULL || *list == NULL) {
    cerrno = CERR_NULLVALUE;
    return;
  }
  llist_clear(*list);
  free(*list);
  *list = NULL;
  cerrno = CERR_SUCCESS;
}

int llist_finddup(linkedlist_t* list, linkedlist_t* duplist, comparefct_t compare) {
  if (list == NULL || duplist == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  llist_clear(duplist);
  LLIST_FOREACH(list, {
      if (llist_count(list, data, compare) > 1 && !llist_contains(duplist, data, compare)) {
	if (!llist_add(duplist, data)) {
	  llist_clear(duplist);
	  return -1;
	}
      }
    });
  cerrno = CERR_SUCCESS;
  return duplist->size;
}

linkedlist_t* llist_fromarray(void* array, int size, size_t data_size) {
  linkedlist_t* list;
  int i;
  if (array == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  list = llist_new(data_size);
  if (list == NULL)
    return NULL;
  for (i = 0; i < size; i++) {
    if (!llist_add(list, array + (i * data_size))) {
      llist_destroy(&list);
      return NULL;
    }
  }
  cerrno = CERR_SUCCESS;
  return list;
}

int llist_issorted(linkedlist_t* list, comparefct_t compare) {
  void* previous_data = NULL;
  if (list == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  LLIST_FOREACH(list, {
      if (previous_data != NULL &&
	  compare(previous_data, data) > 0) // previous element is greater than current
	return 0;
      previous_data = data;
    });
  return 1;
}

linkedlist_t* llist_new(size_t data_size) {
  linkedlist_t* list = malloc(sizeof(linkedlist_t));
  if (list == NULL) {
    cerrno = CERR_SYSTEM;
    return NULL;
  }
  list->size = 0;
  list->data_size = data_size;
  list->head = NULL;
  list->tail = NULL;
  cerrno = CERR_SUCCESS;
  return list;
}

void* llist_peekfirst(linkedlist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  cerrno = CERR_SUCCESS;
  return (list->size == 0) ? NULL : list->head->data;
}

void* llist_peeklast(linkedlist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  cerrno = CERR_SUCCESS;
  return (list->size == 0) ? NULL : list->tail->data;
}

static int _llist_remove(linkedlist_t* list, void* e, comparefct_t compare, int removeall) {
  linkedlistnode_t *node, *previous = NULL;
  int cpt = 0;
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  node = list->head;
  while (node != NULL) {
    if (compare(node->data, e) == 0) {
      if (previous == NULL)
	list->head = list->head->next; // Remove first element of the list
      else
	previous->next = node->next;
      free(node->data);
      free(node);
      cpt++;
      list->size--;
      if (!removeall)
	return 1;
      node = previous->next;
    }
    else {
      // Go to the next node
      previous = node;
      node = node->next;
    }
  }
  return cpt;
}

int llist_remove(linkedlist_t* list, void* e, comparefct_t compare) {
  return _llist_remove(list, e, compare, 0);
}

int llist_removeall(linkedlist_t* list, void* e, comparefct_t compare) {
  return _llist_remove(list, e, compare, 1);
}

int llist_removefirst(linkedlist_t* list) {
  linkedlistnode_t* tmp;
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  if (list->size == 0)
    return 0;
  free(list->head->data);
  if (list->size == 1) {
    free(list->head);
    list->head = NULL;
    list->tail = NULL;
    return 1;
  }
  tmp = list->head;
  list->head = list->head->next;
  free(tmp);
  list->size--;
  return 1;
}

int llist_removelast(linkedlist_t* list) {
  linkedlistnode_t* node;
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  if (list->size == 0)
    return 0;
  if (list->size == 1) {
    free(list->tail->data);
    free(list->tail);
    list->head = NULL;
    list->tail = NULL;
    return 1;
  }
  node = list->head;
  while (node->next != list->tail) {
    node = node->next;
  }
  free(list->tail->data);
  free(list->tail);
  list->tail = node;
  list->tail->next = NULL;
  list->size--;
  return 1;
}

int llist_sort(linkedlist_t* list, comparefct_t compare) {
  int size, i;
  void* array;
  if (list == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  array = llist_toarray(list);
  if (array == NULL)
    return 0;
  size = list->size;
  llist_clear(list);
  // If no (__compar_fn_t) cast, gcc produces a warning (incompatible pointer type)
  qsort(array, size, list->data_size, (__compar_fn_t)compare);
  for (i = 0; i < size; i++) {
    if (!llist_add(list, array + (i * list->data_size))) {
      free(array);
      return 0;
    }
  }
  free(array);
  return 1;
}

void* llist_toarray(linkedlist_t* list) {
  void* array;
  int i = 0;
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  array = malloc(list->size * list->data_size);
  if (array == NULL) {
    cerrno = CERR_SYSTEM;
    return NULL;
  }
  LLIST_FOREACH(list, {
      memcpy(array + (i * list->data_size), data, list->data_size);
      i++;
    });
  cerrno = CERR_SUCCESS;
  return array;
}
