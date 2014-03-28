#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "collections/errors.h"
#include "collections/arraylist.h"

int alist_add(arraylist_t* list, void* e) {
  if (list == NULL || e == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  list->data = realloc(list->data, (list->size + 1) * list->data_size);
  if (list->data == NULL) {
    cerrno = CERR_SYSTEM;
    return 0;
  }
  memcpy(ALIST_INDEXOF(list, list->size), e, list->data_size);
  list->size++;
  cerrno = CERR_SUCCESS;
  return 1;
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
  if (index < 0 || index >= list->size) {
    cerrno = CERR_BADINDEX;
    return 0;
  }
  return -1;
}

int alist_addfirst(arraylist_t* list, void* e) {
  if (list == NULL || e == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  return -1;
}

int alist_addv(arraylist_t* list, int nbargs, ...) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  return -1;
}

void alist_clear(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return;
  }
}

arraylist_t* alist_clone(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  return NULL;
}

int alist_cmp(arraylist_t* l1, arraylist_t* l2, comparefct_t compare) {
  if (l1 == NULL || l2 == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  return 0;
}

int alist_contains(arraylist_t* list, void* e, comparefct_t compare) {
  return alist_count(list, e, compare) > 0;
}

int alist_count(arraylist_t* list, void* e, comparefct_t compare) {
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  return -1;
}

void alist_destroy(arraylist_t** list) {
  //
}

int alist_finddup(arraylist_t* list, arraylist_t* duplist, comparefct_t compare) {
  if (list == NULL || duplist == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  return -1;
}

arraylist_t* alist_fromarray(void* array, int size, size_t data_size) {
  if (array == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
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
  return NULL;
}

int alist_indexof(arraylist_t* list, void* e, comparefct_t compare) {
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return -1;
  }
  return -1;
}

int alist_issorted(arraylist_t* list, comparefct_t compare) {
  if (list == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  return -1;
}

arraylist_t* alist_new(size_t data_size) {
  return NULL;
}

void* alist_peekfirst(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  return NULL;
}

void* alist_peeklast(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  return NULL;
}

int alist_remove(arraylist_t* list, void* e, comparefct_t compare) {
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  return -1;
}

int alist_removeall(arraylist_t* list, void* e, comparefct_t compare) {
  if (list == NULL || e == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  return -1;
}

int alist_removefirst(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  return -1;
}

int alist_removelast(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  return -1;
}

int alist_sort(arraylist_t* list, comparefct_t compare) {
  if (list == NULL || compare == NULL) {
    cerrno = CERR_NULLVALUE;
    return 0;
  }
  return -1;
}

void* alist_toarray(arraylist_t* list) {
  if (list == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  return NULL;
}

char* alist_tostring(arraylist_t* list, tostringfct_t tostring) {
  if (list == NULL || tostring == NULL) {
    cerrno = CERR_NULLVALUE;
    return NULL;
  }
  return NULL;
}

