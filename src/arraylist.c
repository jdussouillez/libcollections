#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "collections/errors.h"
#include "collections/arraylist.h"

// TODO: add function "setat(list, index, e)"

static int _alist_remove(arraylist_t* list, void* e, comparefct_t compare, int removeall) {
  int cpt = 0;
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  // TODO
  return cpt;
}

static void _shift_left(arraylist_t* list, int index) {
  int i;
  for (i = index; i < list->size - 2; i++) {
    memmove(list->data[i], list->data[i + 1], list->data_size);
  }
}

static void _shift_right(arraylist_t* list, int index) {
  int i;
  for (i = list->size - 1; i > index; i--) {
    memmove(list->data[i], list->data[i - 1], list->data_size);
  }
}

int alist_add(arraylist_t* list, void* e) {
  if (list == NULL || e == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  return alist_addat(list, list->size, e);
}

int alist_addall(arraylist_t* dest, arraylist_t* src) {
  if (dest == NULL || src == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  if (src == dest) {
    // addall on the same list is forbidden (would cause infinite loop)
    cerrno = CERR_FORBIDDEN;
    return 0;
  }
  ALIST_FOREACH(src, {
      if (!alist_add(dest, data)) {
	return 0;
      }
    });
  cerrno = CERR_SUCCESS;
  return 1;
}

int alist_addat(arraylist_t* list, int index, void* e) {
  if (list == NULL || e == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  if (index < 0 || index > list->size) {
    cerrno = CERR_BADINDEX;
    return 0;
  }
  list->size++;
  list->data = realloc(list->data, list->size * sizeof(void*));
  list->data[list->size - 1] = malloc(list->data_size);
  if (list->data == NULL || list->data[list->size - 1] == NULL) {
    cerrno = CERR_SYSTEM;
    return 0;
  }
  // Shift elements (current index + 1) from "index" to the end.
  _shift_right(list, index);
  // Copy the element
  memcpy(list->data[index], e, list->data_size);
  cerrno = CERR_SUCCESS;
  return 1;
}

int alist_addfirst(arraylist_t* list, void* e) {
  return alist_addat(list, 0, e);
}

int alist_addv(arraylist_t* list, int nbargs, ...) {
  va_list ap;
  void *e, *tmp_array;
  int i, nb = 0, tmp_size = nbargs * sizeof(void*);
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  if ((tmp_array = malloc(tmp_size)) == NULL) {
    cerrno = CERR_SYSTEM;
    return 0;
  }
  memset(tmp_array, 0, tmp_size);
  // Copy the arguments in an array. Check for NULL values
  va_start(ap, nbargs);
  for (i = 0; i < nbargs; i++) {
    e = va_arg(ap, void*);
    if (e == NULL) {
      free(tmp_array);
      cerrno = CERR_NULLVALUE;
      return 0;
    }
    memcpy(tmp_array + (i * list->data_size), e, list->data_size);
  }
  // Copy the elements in the list
  for (i = 0; i < nbargs; i++) {
    e = tmp_array + (i * list->data_size);
    if (alist_add(list, e))
      nb++;
  }
  va_end(ap);
  free(tmp_array);
  cerrno = CERR_SUCCESS;
  return nb;
}

void alist_clear(arraylist_t* list) {
  int i;
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return;
  }
  for (i = 0; i < list->size; i++) {
    free(list->data[i]);
    list->data[i] = NULL;
  }
  free(list->data);
  list->data = NULL;
  list->size = 0;
  cerrno = CERR_SUCCESS;
}

arraylist_t* alist_clone(arraylist_t* list) {
  arraylist_t* clone;
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  clone = alist_new(list->data_size);
  if (clone == NULL || !alist_addall(clone, list))
    return NULL;
  cerrno = CERR_SUCCESS;
  return clone;
}

int alist_cmp(arraylist_t* l1, arraylist_t* l2, comparefct_t compare) {
  int cmp, i;
  if (l1 == NULL || l2 == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  // Compare size of data
  cmp = l1->data_size - l2->data_size;
  if (cmp != 0)
    return cmp;
  // Compare size of list
  cmp = l1->size - l2->size;
  if (cmp != 0)
    return cmp;
  // Compare elements
  for (i = 0; i < l1->size; i++) {
    cmp = ((compare == NULL) ?
	   memcmp(l1->data[i], l2->data[i], l1->data_size) :
	   compare(l1->data[i], l2->data[i]));
    if (cmp != 0)
      return cmp;
  }
  return 0;
}

int alist_contains(arraylist_t* list, void* e, comparefct_t compare) {
  return alist_count(list, e, compare) > 0;
}

int alist_count(arraylist_t* list, void* e, comparefct_t compare) {
  int cpt = 0;
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  ALIST_FOREACH(list, {
      if (((compare != NULL) ?
	   compare(data, e) :
	   memcmp(data, e, list->data_size)) == 0) {
	cpt++;
      }
    });
  cerrno = CERR_SUCCESS;
  return cpt;
}

void alist_destroy(arraylist_t** list) {
  if (list == NULL || *list == NULL) {
    cerrno = CERR_NULLVALUE;
    return;
  }
  alist_clear(*list);
  free(*list);
  *list = NULL;
  cerrno = CERR_SUCCESS;
}

int alist_finddup(arraylist_t* list, arraylist_t* duplist, comparefct_t compare) {
  if (list == NULL || duplist == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  alist_clear(duplist);
  ALIST_FOREACH(list, {
      if (alist_count(list, data, compare) > 1 &&
	  !alist_contains(duplist, data, compare)) {
	if (!alist_add(duplist, data)) {
	  alist_clear(duplist);
	  return -1;
	}
      }
    });
  cerrno = CERR_SUCCESS;
  return duplist->size;
}

arraylist_t* alist_fromarray(void* array, int size, size_t data_size) {
  arraylist_t* list;
  int i;
  if (array == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  if ((list = alist_new(data_size)) == NULL) {
    return NULL;
  }
  for (i = 0; i < size; i++) {
    if (!alist_add(list, array + (i * data_size))) {
      alist_destroy(&list);
      return NULL;
    }
  }
  cerrno = CERR_SUCCESS;
  return NULL;
}

void* alist_get(arraylist_t* list, int index) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  if (index < 0 || index >= list->size) {
    cerrno = CERR_BADINDEX;
    return NULL;
  }
  return list->data[index];
}

int alist_indexof(arraylist_t* list, void* e, comparefct_t compare) {
  int i;
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  for (i = 0; i < list->size; i++) {
      if (((compare == NULL) ?
	   memcmp(list->data[i], e, list->data_size) :
	   compare(list->data[i], e)) == 0) {
	return i;
      }
  }
  return -1;
}

int alist_issorted(arraylist_t* list, comparefct_t compare) {
  int i;
  void* previous_data = NULL;
  if (list == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  previous_data = list->data[0];
  for (i = 1; i < list->size; i++) {
    if (((compare == NULL) ?
	 memcmp(previous_data, list->data[i], list->data_size) :
	 compare(previous_data, list->data[i])) > 0) { // previous element is greater than current
      return 0;
    }
    previous_data = list->data[i];
  }
  cerrno = CERR_SUCCESS;
  return 1;
}

arraylist_t* alist_new(size_t data_size) {
  arraylist_t* list = malloc(sizeof(arraylist_t));
  if (list == NULL) {
    cerrno = CERR_SYSTEM;
    return NULL;
  }
  memset(list, 0, sizeof(arraylist_t));
  list->data_size = data_size;
  cerrno = CERR_SUCCESS;
  return list;
}

void* alist_peekfirst(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  cerrno = CERR_SUCCESS; 
  return (list->size == 0) ? NULL : list->data[0];
}

void* alist_peeklast(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  cerrno = CERR_SUCCESS;
  return (list->size == 0) ? NULL : list->data[list->size - 1];
}

int alist_remove(arraylist_t* list, void* e, comparefct_t compare) {
  return _alist_remove(list, e, compare, 0);
}

int alist_removeall(arraylist_t* list, void* e, comparefct_t compare) {
  return _alist_remove(list, e, compare, 1);
}

int alist_removefirst(arraylist_t* list) {
  void* e = malloc(list->data_size);
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  if (list->size == 0)
    return 0;
  memmove(e, list->data, list->data_size);
  _shift_left(list, 0);
  free(e);
  list->size--;
  return 1;
}

int alist_removelast(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  if (list->size == 0)
    return 0;
  free(list->data[list->size - 1]);
  list->data = realloc(list->data, (list->data_size - 1) * sizeof(void*));
  if (list->data == NULL) {
    cerrno = CERR_SYSTEM;
    return -1;
  }
  list->size--;
  return 1;
}

int alist_sort(arraylist_t* list, comparefct_t compare) {
  if (list == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  // TODO
  return -1;
}

void* alist_toarray(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  // TODO
  return NULL;
}

char* alist_tostring(arraylist_t* list, tostringfct_t tostring) {
  if (list == NULL || tostring == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  // TODO
  return NULL;
}
