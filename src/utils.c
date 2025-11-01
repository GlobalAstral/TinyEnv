#include <utils.h>

void printSpaces(int amount) {
  for (int i = 0; i < amount; i++)
    putchar(' ');
}

List newList() {
  List lst;
  lst.capacity = LIST_CAPACITY_STEP;
  lst.size = 0;
  lst.head = allocate(InstructionInstance, LIST_CAPACITY_STEP);
  lst.uninit = 0;
  return lst;
}

int listPush(List *lst, InstructionInstance item) {
  if (lst->uninit)
    return 1;
  if (lst->size >= lst->capacity) {
    lst->capacity += LIST_CAPACITY_STEP;
    resize(InstructionInstance, lst->head, lst->capacity);
  }

  lst->head[lst->size++] = item;
  return 0;
}

int listPop(List *lst) {
  if (lst->uninit) return 1;
  if (lst->size <= 0) return 1;

  lst->size--;
  memset(&(lst->head[lst->size]), 0, sizeof(InstructionInstance));

  return 0;
}

int listInsert(List *lst, InstructionInstance item, int index) {
  if (lst->uninit) return 1;
  if (index < 0 || index > lst->size) return 1;
  if (index == lst->size) return listPush(lst, item);

  if (lst->size >= lst->capacity) {
    lst->capacity += LIST_CAPACITY_STEP;
    resize(InstructionInstance, lst->head, lst->capacity);
  }
  memmove(&(lst->head[index+1]), &(lst->head[index]), (lst->size - index)*sizeof(InstructionInstance));
  lst->head[index] = item;
  lst->size++;
  return 0;
}

int listRemove(List *lst, int index) {
  if (lst->uninit) return 1;
  if (index < 0 || index >= lst->size) return 1;

  memmove(&(lst->head[index]), &(lst->head[index+1]), (lst->size - index - 1)*sizeof(InstructionInstance));
  lst->size--;

  return 0;
}

void listReset(List *lst) {
  lst->capacity = LIST_CAPACITY_STEP;
  resize(InstructionInstance, lst->head, lst->capacity);
  lst->size = 0;
  lst->uninit = 0;
}

string_list* new_string_list(int initsize) {
  string_list* ret = allocate(string_list, 1);
  if (initsize <= 0) {
    ret->buf = NULL;
    ret->size = 0;
  } else {
    ret->buf = allocate(char*, initsize);
    ret->size = 0;
  }
  return ret;
}

string_list *split(char *buf, size_t size, char sep) {
  int line_count = 1;
  for (int i = 0; i < size; i++) {
    if (buf[i] == sep)
      line_count++;
  }
  string_list* tokens = new_string_list(line_count);
  char* base = buf;
  for (int i = 0; i < size; i++) {
    if (buf[i] == sep) {
      buf[i] = 0;
      tokens->buf[tokens->size++] = base;
      base = &(buf[i+1]);
    }
  }
  tokens->buf[tokens->size++] = base;
  return tokens;
}

char* ltrim(char* s) {
  while(isspace(*s)) s++;
  return s;
}

char* rtrim(char* s) {
  char* back = s + strlen(s);
  while(isspace(*--back));
  *(back+1) = '\0';
  return s;
}

char* trim(char* s) {
  return rtrim(ltrim(s)); 
}
