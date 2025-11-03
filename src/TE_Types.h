#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <TE_Constants.h>

#define MEMORY_SIZE 256
#define STACK_SIZE 64

#define STACK_START (MEMORY_SIZE-STACK_SIZE)
#define STACK_END (MEMORY_SIZE)

typedef uint_t Register;

int printRegister(Reg rg);

typedef Register Registers[__RG_COUNT];

typedef struct {
  Registers registers;
  bool FLAG_Z;
  bool FLAG_G;
  bool FLAG_L;
  bool FLAG_ERR;
  bool FLAG_HLT;
  byte memory[MEMORY_SIZE]; //? IK IT DOESNT MAKE SENSE
} CPU;

typedef struct {
  ParamValType val_type;
  ParamValue value;
} InstructionParam;

typedef bool (*InstructionCallback)(CPU*, InstructionParam*, InstructionParam*);

typedef struct {
  InsType type;
  InstructionCallback callback;
} Instruction; //? Will be singleton in a registry system.

typedef struct {
  Instruction* ins;
  InstructionParam lparam;
  InstructionParam rparam;
} InstructionInstance;

int printII(InstructionInstance* instance);
int parametersNeeded(InsType instruction);

CPU createCPU();
void resetCPU(CPU* cpu);

typedef struct {
  char name[DEFINE_NAME_MAX];
  char* define;
  size_t size;
} Definition;
