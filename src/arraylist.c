#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "collections/errors.h"
#include "collections/arraylist.h"

static void _shift_left(arraylist_t* list, int index) {
  int i;
  for (i = index; i < list->size - 1; i++) {
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
  int ret;
  va_list arglist;
  va_start(arglist, nbargs);
  ret = alist_addvlist(list, nbargs, arglist);
  va_end(arglist);
  return ret;
}


int alist_addvlist(arraylist_t* list, int nbargs, va_list ap) {
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


int alist_empty(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  return (list->size == 0) ? 1 : 0;
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
  return list;
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
  cerrno = CERR_SUCCESS;
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


int alist_lastindexof(arraylist_t* list, void* e, comparefct_t compare) {
  int i;
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  for (i = list->size - 1; i >= 0; i--) {
      if (((compare == NULL) ?
	   memcmp(list->data[i], e, list->data_size) :
	   compare(list->data[i], e)) == 0) {
	return i;
      }
  }
  return -1;
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
  int index;
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  if ((index = alist_indexof(list, e, compare)) == -1)
    return 0;
  return alist_removeat(list, index);
}


int alist_removeall(arraylist_t* list, void* e, comparefct_t compare) {
  int i, cpt = 0;
  arraylist_t* tmp;
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  // Create a copy of the list
  if ((tmp = alist_clone(list)) == NULL)
    return -1;
  // Clear the list and add only the elements different from e
  alist_clear(list);
  for (i = 0; i < tmp->size; i++) {
    if (((compare == NULL) ?
	 memcmp(tmp->data[i], e, tmp->data_size) :
	 compare(tmp->data[i], e)) != 0) {
      if (!alist_add(list, tmp->data[i])) {
	return -1;
      }
    }
    else {
      cpt++;
    }
  }
  alist_destroy(&tmp);
  cerrno = CERR_SUCCESS;
  return cpt;
}


int alist_removeat(arraylist_t* list, int index) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  if (index < 0 || index >= list->size) {
    cerrno = CERR_BADINDEX;
    return -1;
  }
  if (list->size == 1) {
    free(list->data[index]);
    free(list->data);
    list->data = NULL;
  }
  else {
    // Shift elements to the left (-1), then remove the last element.
    _shift_left(list, index);
    free(list->data[list->size - 1]);
    list->data = realloc(list->data, (list->size - 1) * sizeof(void*));
    if (list->data == NULL) {
      cerrno = CERR_SYSTEM;
      return -1;
    }
  }
  cerrno = CERR_SUCCESS;
  list->size--;
  return 1;
}


int alist_removefirst(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  return (list->size == 0) ? 0 : alist_removeat(list, 0);
}


int alist_removelast(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  return (list->size == 0) ? 0 : alist_removeat(list, list->size - 1);
}


int alist_setat(arraylist_t* list, int index, void* e, void* previous) {
  if (list == NULL || e == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  if (index < 0 || index >= list->size) {
    cerrno = CERR_BADINDEX;
    return 0;
  }
  cerrno = CERR_SUCCESS;
  if (previous == NULL) {
    memcpy(list->data[index], e, list->data_size);
    return 1;
  }
  memcpy(previous, list->data[index], list->data_size);
  memcpy(list->data[index], e, list->data_size);
  return 1;
}


int alist_size(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  cerrno = CERR_SUCCESS;
  return list->size;
}


int alist_sort(arraylist_t* list, comparefct_t compare) {
  void* tmp_array;
  int i, size;
  if (list == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  if (list->size > 0) {
    // Export values in an array
    tmp_array = alist_toarray(list);
    if (tmp_array == NULL) {
      return 0;
    }
    // Sort the array
    qsort(tmp_array, list->size, list->data_size, (__compar_fn_t)compare);
    // Clear the list and add the element of the array in the list
    size = list->size;
    alist_clear(list);
    for (i = 0; i < size; i++) {
      if (!alist_add(list, (tmp_array + (i * list->data_size)))) {
	free(tmp_array);
	return 0;
      }
    }
    free(tmp_array);
  }
  cerrno = CERR_SUCCESS;
  return 1;
}


void* alist_toarray(arraylist_t* list) {
  void* array = NULL;
  int i;
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  if ((array = malloc(list->size * list->data_size)) == NULL) {
    cerrno = CERR_SYSTEM;
    return NULL;
  }
  for (i = 0; i < list->size; i++) {
    memcpy(array + (i * list->data_size), list->data[i], list->data_size);
  }
  cerrno = CERR_SUCCESS;
  return array;
}

char* alist_tostring(arraylist_t* list, tostringfct_t tostring) {
  char *strlist = NULL, *strelem = NULL;
  int i, bufsize = CBUFSIZE * sizeof(char);
  if (list == NULL || tostring == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  if ((strlist = malloc(bufsize)) == NULL) {
    cerrno = CERR_SYSTEM;
    return NULL;
  }
  memset(strlist, 0, bufsize);
  strncpy(strlist, "[", bufsize - strlen(strlist) - 1);
  for (i = 0; i < list->size; i++) {
    if ((strelem = tostring(list->data[i])) != NULL) {
      strncat(strlist, strelem, bufsize - strlen(strlist) - 1);
      free(strelem);
      if (i < list->size - 1)
	strncat(strlist, ", ", bufsize - strlen(strlist) - 1);
    }
  }
  strncat(strlist, "]", bufsize - strlen(strlist) - 1);
  cerrno = CERR_SUCCESS;
  return strlist;
}

