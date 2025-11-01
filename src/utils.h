#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <TE_Types.h>
#include <ctype.h>

#define LIST_CAPACITY_STEP 3
#define allocate(T, size) (T*)malloc(size*sizeof(T))
#define resize(T, mem, size) mem = (T*)realloc(mem, size*sizeof(T));
#define UNINIT_LIST ((List){.uninit = true})

void printSpaces(int amount);

typedef struct {
  InstructionInstance* head;
  size_t size;
  size_t capacity;
  bool uninit;
} List;

List newList();
int listPush(List* lst, InstructionInstance item);
int listPop(List* lst);
int listInsert(List* lst, InstructionInstance item, int index);
int listRemove(List* lst, int index);
void listReset(List* lst);

typedef struct {
  char** buf;
  size_t size;
} string_list;

string_list* new_string_list(int initsize);

string_list* split(char* buf, size_t size, char sep);

char* ltrim(char *s);

char* rtrim(char* s);

char* trim(char* s);
