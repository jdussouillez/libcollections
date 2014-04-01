#include <stdio.h>
#include <stdlib.h>

#include "collections/errors.h"
#include "collections/arraylist.h"

#define HANDLE_ERR(msg) do {		\
    cperror(msg);				\
    return 1;					\
  } while (0)


static int compare_int(void* p1, void* p2) {
  return *((int*) p1) - *((int*) p2);
}

static char* tostring_int(void* p) {
  char* str = malloc(5 * sizeof(char));
  snprintf(str, 5, "%d", *((int*) p));
  return str;
}

int main(void) {
  char* str;
  int i, j;
  arraylist_t* list;
  if ((list = alist_new(sizeof(int))) == NULL)
    HANDLE_ERR("alist_new");
  // list = []

  i = 2;
  if (!alist_add(list, &i))
    HANDLE_ERR("alist_add");
  i = 4;
  if (!alist_add(list, &i))
    HANDLE_ERR("alist_add");
  // list = [2, 4]

  i = 8;
  j = 16;
  if (!alist_addv(list, 2, &i, &j))
    HANDLE_ERR("alist_addv");
  // list = [2, 4, 8, 16]

  i = 32;
  if (!alist_addat(list, 3, &i))
    HANDLE_ERR("alist_addat");
  // list = [2, 4, 8, 32, 16]

  i = 64;
  if (!alist_addfirst(list, &i))
    HANDLE_ERR("alist_addfirst");
  // list = [64, 2, 4, 8, 32, 16]

  if (!alist_sort(list, compare_int))
    HANDLE_ERR("alist_sort");
  // list = [2, 4, 8, 16, 32, 64]

  i = 66;
  if (!alist_add(list, &i))
    HANDLE_ERR("alist_add");
  // list = [64, 2, 4, 8, 32, 16, 66]

  i = 128;
  if (alist_setat(list, 6, &i, NULL))
    HANDLE_ERR("alist_setat");
  // list = [64, 2, 4, 8, 32, 16, 128]

  // Affichage de la liste ([2, 4, 8, 16, 32, 64, 128])
  if ((str = alist_tostring(list, tostring_int)) == NULL)
    HANDLE_ERR("alist_tostring");
  printf("%s\n", str);
  free(str);
  alist_destroy(&list);
  return 0;
}
