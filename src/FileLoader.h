#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <utils.h>
#include <InstructionRegistry.h>

#define LABEL_PREFIX '@'
#define COMMENT_PREFIX '#'

size_t allocateFileBuffer(FILE* f, char** buf);
void initializeFileBuffer(FILE* f, char* buf);
void parseFileBuffer(char* buf, size_t size, IIList* code);
bool substrEqual(char* s, size_t start, size_t end, char* s2);
int string2Param(char* s, InstructionParam* param, char** keys, int* values, size_t size);
bool fileExists(char* fname);
