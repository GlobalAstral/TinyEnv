#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <utils.h>
#include <InstructionRegistry.h>

size_t allocateFileBuffer(FILE* f, char** buf);
void initializeFileBuffer(FILE* f, char* buf);
void parseFileBuffer(char* buf, size_t size, List* code);
bool substrEqual(char* s, size_t start, size_t end, char* s2);
int string2Param(char* s, InstructionParam* param);
bool fileExists(char* fname);
