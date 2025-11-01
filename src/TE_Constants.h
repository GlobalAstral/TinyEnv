#pragma once

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define INT_SIZE 16

#if INT_SIZE == 8
  #define SIGNED_TYPE char
#elif INT_SIZE == 16
  #define SIGNED_TYPE short
#elif INT_SIZE == 32
  #define SIGNED_TYPE int
#elif INT_SIZE == 64
  #define SIGNED_TYPE long long
#endif

typedef unsigned SIGNED_TYPE uint_t;
typedef SIGNED_TYPE int_t;

#undef SIGNED_TYPE

typedef unsigned char byte;
typedef uint_t Literal;

//!   REGISTERS
typedef enum {
  RG_A, RG_B, RG_C, RG_D, RG_SI, RG_DI, RG_BP, RG_SP, __RG_COUNT
} Reg;
static char* REGISTERS[] = {"AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP"};
static char* registerSelectOptions[__RG_COUNT+1] = {0};
static char* REGISTERS_LOWER[__RG_COUNT] = {0};

//!   FLAGS
static char* FLAGS[] = {"Z", "G", "L", "E", "H"};

//!   MAIN MENU OPTIONS
static char* mainMenuOptions[] = {"Add Instruction", "Insert Instruction", "Pop Instruction", "Remove Instruction", "Load from File", "Undo", "Exit"};

//!   INSTRUCTIONS
typedef enum {
  INS_NOP, INS_MVA, INS_MOV, INS_STO,
  INS_ADD, INS_SUB, INS_CMP, INS_JMP, 
  INS_JEQ, INS_JLS, INS_JGR, INS_JLE,
  INS_JGE, INS_JNE, INS_AND, INS_NOT,
  INS_XOR, INS_SWP, INS_PSH, INS_POP,
  INS_HLT, INS_SHL, INS_SHR, INS_JRF,
  INS_JRB, INS_INC, INS_DEC, INS_LOW,
  INS_HIG, INS_MRG, __INS_COUNT
} InsType;
static char* INSTRUCTIONS[] = {
  "nop", "mva", "mov", "sto", 
  "add", "sub", "cmp", "jmp", 
  "jeq", "jls", "jgr", "jle", 
  "jge", "jne", "and", "not", 
  "xor", "swp", "psh", "pop", 
  "hlt", "shl", "shr", "jrf",
  "jrb", "inc", "dec", "low",
  "hig", "mrg"
};
static char* instructionSelectOptions[__INS_COUNT+1] = {0};

//!   PARAMETER VALUE TYPE
typedef enum {
  INSP_REGISTER,
  INSP_LITERAL,
  INSP_NULL
} ParamValType;
static char* paramTypeSelectOptions[] = {"REGISTER", "LITERAL", "UNDO"};

//!   OTHER
typedef union {
  Reg reg;
  Literal literal;
} ParamValue;
static char* toLower(char* s) {
  size_t size = strlen(s)+1;
  char* ret = (char*) malloc(size);
  for (int i = 0; i < size; i++)
    ret[i] = tolower(s[i]);
  return ret;
}

static char* toUpper(char* s) {
  size_t size = strlen(s)+1;
  char* ret = (char*) malloc(size);
  for (int i = 0; i < size; i++)
    ret[i] = toupper(s[i]);
  return ret;
}

__attribute__((constructor))
static void init_registerSelectOptions(void) {
  for (size_t i = 0; i < __RG_COUNT; i++)
    registerSelectOptions[i] = REGISTERS[i];
  registerSelectOptions[__RG_COUNT] = "UNDO";
}

__attribute__((constructor))
static void init_REGISTERS_LOWER(void) {
  for (size_t i = 0; i < __RG_COUNT; i++)
    REGISTERS_LOWER[i] = toLower(REGISTERS[i]);
}

__attribute__((constructor))
static void init_instructionSelectOptions(void) {
  for (size_t i = 0; i < __INS_COUNT; i++)
    instructionSelectOptions[i] = toUpper(INSTRUCTIONS[i]);
  instructionSelectOptions[__INS_COUNT] = "UNDO";
}
