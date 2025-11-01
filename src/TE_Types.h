#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <TE_Constants.h>

#define MEMORY_SIZE 256
#define STACK_SIZE 64

#define STACK_START (MEMORY_SIZE-STACK_SIZE)
#define STACK_END (MEMORY_SIZE)

typedef unsigned char byte;

typedef short i16;
typedef u16 Register;


int printRegister(Reg rg);

typedef Register Registers[__RG_COUNT];

typedef struct {
  Registers registers;
  int IP;
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
