#pragma once

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LABEL_PREFIX "@"
#define COMMENT_PREFIX "#"
#define CHAR(T) T[0]

#define INT_SIZE 16
#define PAGES_LEN (sizeof(HELP_PAGES) / sizeof(HELP_PAGES[0]))

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
  RG_A, RG_B, RG_C, RG_D, RG_SI, RG_DI, RG_BP, RG_SP, RG_IP, __RG_COUNT
} Reg;
static char* REGISTERS[] = {"AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP", "IP"};
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
  INS_HIG, INS_MRG, INS_CAL, INS_RET,
  __INS_COUNT
} InsType;
static char* INSTRUCTIONS[] = {
  "nop", "mva", "mov", "sto", 
  "add", "sub", "cmp", "jmp", 
  "jeq", "jls", "jgr", "jle", 
  "jge", "jne", "and", "not", 
  "xor", "swp", "psh", "pop", 
  "hlt", "shl", "shr", "jrf",
  "jrb", "inc", "dec", "low",
  "hig", "mrg", "cal", "ret"
};
static char* instructionSelectOptions[__INS_COUNT+1] = {0};

//!   PARAMETER VALUE TYPE
typedef enum {
  INSP_REGISTER,
  INSP_LITERAL,
  INSP_NULL
} ParamValType;
static char* paramTypeSelectOptions[] = {"REGISTER", "LITERAL", "UNDO"};
typedef union {
  Reg reg;
  Literal literal;
} ParamValue;

//!   HELP PAGES

static char* HELP_PAGES_TITLES[] = {
  "Table of Contents", "Introduction", "Controls", "Parser Features", "Registers", "Flags", "Instructions", "Instructions", 
  "Semantics", "Instruction Usage", "Instruction Usage"
};

static char* HELP_PAGES[] = {
  "Table of Contents:\n"
  "\t1. Introduction\n"
  "\t2. Controls\n"
  "\t3. Parser Features\n"
  "\t4. Registers\n"
  "\t5. Flags\n"
  "\t6. Instructions\n"
  "\t8. Semantics\n"
  "\t9. Instruction Usage\n",

  "THE GUI IS DEPRECATED. IT IS HIGHLY RECOMMENDED TO USE EXTERNAL FILES!!\n"
  "This Microprocessor is a simplified version of the real microprocessors.\n"
  "Each register is 16 bits wide. You can find registers on the next chapters.\n"
  "The memory is organized in 256 bytes, of which 64 are dedicated to the stack.\n"
  "In the memory the bytes are shown in HEX, while in the stack they are shown in decimal.\n"
  "There are five flags that will be covered in the next chapters.\n"
  "There are some additional features when loading files that will be covered in the next chapters",

  "  H: Open and close this page.\n"
  " ^Q: Exit.\n"
  "  E: Opens GUI (DEPRECATED)\n"
  "  SPACE: Starts the simulation.\n"
  " ^SPACE: Steps simulation one instruction at a time.\n"
  "  A: Previous Page.\n"
  "  D: Next Page.\n"
  "1-9: Go to Page.\n"
  "  W: Scroll up in shown code.\n"
  "  S: Scroll down in shown code.\n"
  " ^W: Scroll up in shown stack.\n"
  " ^S: Scroll down in shown stack.\n"
  " ^R: Scroll down in shown stack.\n",

  "The Parser for .tne files offers some additional features. Also, trailing spaces are ignored.\n"
  "LABELS:\n"
  "\tBy starting a line with '" LABEL_PREFIX "', you can create a label. To use a label, you must include the '" LABEL_PREFIX "' prefix.\n"
  "\tLabels are considered literals and are shown as literals, their value being the IP of the instruction they\n"
  "\tare referred to. After a label, a valid instruction is mandatory. The lack of such instruction will\n"
  "\tresult in undefined behaviour.\n"
  "EXAMPLE:\n"
  "\t" LABEL_PREFIX "main\n"
  "\t\t...\n"
  "\tjmp " LABEL_PREFIX "main\n"
  "COMMENTS:\n"
  "\tBy starting a line with '" COMMENT_PREFIX "', you can create a comment. Any comment will be ignored.\n"
  "\tYou CANNOT use comments if not at the start of a line.\n"
  "EXAMPLE:\n"
  "\t" COMMENT_PREFIX " Hello, this is a comment!\n",

  "Each register is a 16-bit register that can hold an unsigned short int.\n"
  "\tAX: A register\n"
  "\tBX: B register\n"
  "\tCX: C register\n"
  "\tDX: D register\n"
  "\tSI: SOURCE INDEX register\n"
  "\tDI: DESTINATION INDEX register\n"
  "\tBP: BASE POINTER register\n"
  "\tSP: STACK POINTER register\n"
  "\tIP: INSTRUCTION POINTER register\n",

  "FLAGS:\n"
  "\tZ: True when a CMP instruction has 0 as result.\n"
  "\tG: True when a CMP instruction has a positive result.\n"
  "\tL: True when a CMP instruction has a negative result.\n"
  "\tE: True when the microprocessor runs into an error.\n"
  "\tH: True when the microprocessor is into an halt state.\n",

  "Each instruction is case sensitive to lowercase and can hold zero to two arguments.\n"
  "\tNOP: Does nothing, used for timings or as placeholder.\n"
  "\tMVA: Loads a byte from the specified memory address into a register.\n"
  "\tMOV: Moves a value to a register\n"
  "\tSTO: Stores a value into a specified memory address\n"
  "\tADD: Adds a value to a register's\n"
  "\tSUB: Subtracts a value to a register's\n"
  "\tCMP: Compares a value with another, updating flags accordingly\n"
  "\tJMP: Moves the specified offset to IP, causing a Jump to another instruction\n"
  "\tJEQ: Jumps to a specified offset if Zero Flag is 1\n"
  "\tJLS: Jumps to a specified offset if Less Flag is 1\n"
  "\tJGR: Jumps to a specified offset if Greater Flag is 1\n"
  "\tJLE: Jumps to a specified offset if Less Flag is 1 or Zero Flag is 1\n"
  "\tJGE: Jumps to a specified offset if Greater Flag is 1 or Zero Flag is 1\n"
  "\tJNE: Jumps to a specified offset if Zero Flag is 0\n"
  "\tAND: Performs bitwise-and of a specified register with a value.\n"
  "\tNOT: Performs bitwise-not of a specified register.\n"
  "\tXOR: Performs bitwise-xor of a specified register with a value.\n"
  "\tSWP: Swaps values of two registers.\n"
  "\tPSH: Pushes a value into the stack.\n"
  "\tPOP: Pops a value from the stack.\n"
  "\tHLT: Causes the microprocessor to halt. Reset is required to run again.\n"
  "\tSHL: Shifts the value inside a register to the left by a specified amount.\n"
  "\tSHR: Shifts the value inside a register to the right by a specified amount.\n"
  "\tJRF: Jumps relatively forward by a specified amount, adding it to IP.\n"
  "\tJRB: Jumps relatively backwards by a specified amount, subtracting it to IP.\n"
  "\tINC: Increments the value of a specified register by one.\n",

  "Each instruction is case sensitive to lowercase and can hold zero to two arguments.\n"
  "\tDEC: Decrements the value of a specified register by one.\n"
  "\tLOW: Moves the least significant half of a specified register's value into another.\n"
  "\tHIG: Moves the most significant half of a specified register's value into another.\n"
  "\tMRG: Merges the least significant bits of a value with the most significant bits of the specified register's\n"
  "\tCAL: Pushes return's IP to the stack and jumps to an offset.\n"
  "\tRET: Pops return's IP to IP register, returning from an offset.\n",
  

  "The semantics of values are the following:\n"
  "\tANY: Any value is accepted. Can be a literal or a register.\n"
  "\tREG: Only a register is accepted. The instruction usually needs such register to store its result and/or\n" 
  "\t\tuse its value.\n"
  "\tADDR: Any value is accepted. Can be a literal or a register. This value will be used as address for memory\n"
  "\t\taccess or storage.\n"
  "\tOFFSET: Any value is accepted. Can be a literal or a register. This value will be used as offset for IP\n"
  "\t\tmanipulation.\n",

  "Instruction Usage:\n"
  "\tNOP: nop\n"
  "\tMVA: mva REG, ADDR\n"
  "\tMOV: mov REG, ANY\n"
  "\tSTO: sto ADDR, ANY\n"
  "\tADD: add REG, ANY\n"
  "\tSUB: sub REG, ANY\n"
  "\tCMP: cmp ANY, ANY\n"
  "\tJMP: jmp OFFSET\n"
  "\tJEQ: jeq OFFSET\n"
  "\tJLS: jls OFFSET\n"
  "\tJGR: jgr OFFSET\n"
  "\tJLE: jle OFFSET\n"
  "\tJGE: jge OFFSET\n"
  "\tJNE: jne OFFSET\n"
  "\tAND: and REG, ANY\n"
  "\tNOT: not REG\n"
  "\tXOR: xor REG, ANY\n"
  "\tSWP: swp REG, REG\n"
  "\tPSH: psh ANY\n"
  "\tPOP: pop REG\n"
  "\tHLT: hlt\n"
  "\tSHL: shl REG, ANY\n"
  "\tSHR: shr REG, ANY\n"
  "\tJRF: jrf OFFSET\n"
  "\tJRB: jrb OFFSET\n"
  "\tINC: inc REG\n",

  "Instruction Usage:\n"
  "\tDEC: dec REG\n"
  "\tLOW: low REG, ANY\n"
  "\tHIG: hig REG, ANY\n"
  "\tMRG: mrg REG, ANY\n"
  "\tCAL: cal OFFSET\n"
  "\tRET: ret\n",
};

//!   OTHER

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
