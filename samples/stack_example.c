#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collections/errors.h"
#include "collections/stack.h"

#define NAMESIZE 32
#define BUFSIZE 64

#define HANDLE_ERR(msg, s) do {			\
    cperror(msg);				\
    stack_destroy(&s);				\
    return 1;					\
  } while (0)

struct s_proc {
  int pid;
  char name[NAMESIZE];
};

static int compare_proc(void* p1, void* p2) {
  // Compare by pid
  return ((struct s_proc*) p1)->pid - ((struct s_proc*) p2)->pid;
}

static char* tostring_proc(void* p) {
  struct s_proc proc = *((struct s_proc*) p);
  char* str;
  if ((str = malloc(BUFSIZE * sizeof(char))) == NULL) {
    perror("malloc");
    return NULL;
  }
  memset(str, 0, BUFSIZE);
  snprintf(str, BUFSIZE, "[%d] %s", proc.pid, proc.name);
  return str;
}

static struct s_proc* create_proc(const char* name, const int pid) {
  struct s_proc* p;
  if ((p = malloc(sizeof(struct s_proc))) == NULL) {
    perror("malloc");
    return NULL;
  }
  strncpy(p->name, name, NAMESIZE);
  p->pid = pid;
  return p;
}

int main(void) {
  stack_t* s;
  struct s_proc* p;
  char* str = NULL;
  if ((s = stack_new(sizeof(struct s_proc))) == NULL)
    HANDLE_ERR("stack_new", s);
  // s = []

  // stack_push() has the same effect as stack_add()
  p = create_proc("init", 1);
  if (!stack_push(s, p))
    HANDLE_ERR("stack_push", s);
  free(p);
  p = create_proc("x-session-manager", 15354);
  if (!stack_push(s, p))
    HANDLE_ERR("stack_push", s);
  free(p);
  p = create_proc("bash", 5420);
  if (!stack_push(s, p))
    HANDLE_ERR("stack_push", s);
  free(p);
  p = create_proc("htop", 8465);
  if (!stack_push(s, p))
    HANDLE_ERR("stack_push", s);
  free(p);
  // s = [[1] init, [15354] x-session-manager, [5420] bash, [8465] htop]

  if ((p = stack_pop(s)) == NULL)
    HANDLE_ERR("stack_pop", s);
  free(p);
  // s = [[1] init, [15354] x-session-manager, [5420] bash]

  // Sort the stack by the PID
  if (!stack_sort(s, compare_proc))
    HANDLE_ERR("stack_sort", s);

  // Print the stack
  if ((str = stack_tostring(s, tostring_proc)) == NULL)
    HANDLE_ERR("stack_tostring", s);
  printf("%s\n", str);
  free(str);

  // Try to push a NULL element
  if (!stack_push(s, NULL))
    HANDLE_ERR("stack_push", s);

  // Destroy the list
  // Useless because we do not reach this instruction (because push(NULL) exits the program).
  stack_destroy(&s);
  return 0;
}
