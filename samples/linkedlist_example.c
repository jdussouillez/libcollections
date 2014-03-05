#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collections/errors.h" // for cperror()
#include "collections/linkedlist.h"

#define BUFSIZE 64

#define HANDLE_ERR(msg, list, ret) do {			\
    perror(msg);					\
    llist_destroy(&list);				\
    return ret;						\
  } while (0)

#define SCANF(format, var, e) do {				\
    if (scanf(format, var) == EOF) {				\
      perror("scanf");						\
      exit(e);							\
    }								\
  } while (0)

typedef struct {
  char lastname[BUFSIZE];
  char firstname[BUFSIZE];
} person_t;

int cmp_person_by_lastname(void* p1, void* p2) {
  person_t person1, person2;
  int cmp;
  person1 = *((person_t*) p1);
  person2 = *((person_t*) p2);
  if ((cmp = strncmp(person1.lastname, person2.lastname, BUFSIZE)) != 0)
    return cmp;
  // Same last name, sort by first name
  return strncmp(person1.firstname, person2.firstname, BUFSIZE);
}

int cmp_person_by_firstname(void* p1, void* p2) {
  person_t person1, person2;
  int cmp;
  person1 = *((person_t*) p1);
  person2 = *((person_t*) p2);
  if ((cmp = strncmp(person1.firstname, person2.firstname, BUFSIZE)) != 0)
    return cmp;
  // Same first name, sort by last name
  return strncmp(person1.lastname, person2.lastname, BUFSIZE);
}

void display(linkedlist_t* list) {
  person_t p;
  printf("\nList : \n");
  if (list->size == 0) {
    printf("Empty !\n\n");
    return;
  }
  LLIST_FOREACH(list, {
      p = *((person_t*) data);
      printf("%s %s\n", p.lastname, p.firstname);
    });
  printf("\n");
}

int main(void) {
  char buf[BUFSIZE];
  linkedlist_t* list = llist_new(sizeof(person_t));
  person_t person;
  int ret;
  if (list == NULL) {
    cperror("llist_new");
    return 1;
  }
  do {
    printf("\n*** Menu ***\n"
	   "add) Add a person\n"
	   "rm) Remove a person\n"
	   "rma) Remove all occurences of a person\n"
	   "sortl) Sort by last name\n"
	   "sortf) Sort by first name\n"
	   "quit) Quit\n"
	   "-> ");
    SCANF("%s", buf, 2);
    // Ask for last and first name
    if (!strcmp(buf, "add") || !strcmp(buf, "rm") || !strcmp(buf, "rma")) {
      printf("Last name : ");
      SCANF("%s", person.lastname, 2);
      printf("First name : ");
      SCANF("%s", person.firstname, 2);
    }
    // Modify and display the list
    if (!strcmp(buf, "add")) {
      if (!llist_add(list, &person)) {
        HANDLE_ERR("add", list, 3);
      }
      display(list);
    }
    else if (!strcmp(buf, "rm")) {
      ret = llist_remove(list, &person, NULL); // Here, we uses memcmp(3) instead of cmp_person_by_X().
      if (ret < 0) {
        HANDLE_ERR("rm", list, 4);
      }
      else if (ret == 0)
	printf("Person not found !\n");
      display(list);
    }
    else if (!strcmp(buf, "rma")) {
      ret = llist_removeall(list, &person, cmp_person_by_lastname);
      if (ret < 0) {
        HANDLE_ERR("rma", list, 5);
      }
      else if (ret == 0)
	printf("Person not found !\n");
      display(list);
    }
    else if (!strcmp(buf, "sortl")) {
      if (!llist_sort(list, cmp_person_by_lastname)) {
        HANDLE_ERR("sortl", list, 6);
      }
      display(list);
    }
    else if (!strcmp(buf, "sortf")) {
      if (!llist_sort(list, cmp_person_by_firstname)) {
        HANDLE_ERR("sortf", list, 7);
      }
      display(list);
    }
  } while (strcmp(buf,"quit"));
  llist_destroy(&list);
  return 0;
}
