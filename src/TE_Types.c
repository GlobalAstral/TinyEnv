#include "TE_Types.h"

int printRegister(Reg rg) {
  char* s = "NULL";
  switch (rg) {
    case RG_A :
      s = "ax";
      break;
    case RG_B :
      s = "bx";
      break;
    case RG_BP :
      s = "bp";
      break;
    case RG_C :
      s = "cx";
      break;
    case RG_D :
      s = "dx";
      break;
    case RG_DI :
      s = "di";
      break;
    case RG_SI :
      s = "si";
      break;
    case RG_SP :
      s = "sp";
      break;
  }
  return printf("%s", s);
}

int printII(InstructionInstance *instance) {
  int chars = 0;
  char* str = "NULL";
  switch (instance->ins->type) {
    case INS_ADD :
      str = "add ";
      break;
    case INS_HLT :
      str = "hlt ";
      break;
    case INS_JMP :
      str = "jmp ";
      break;
    case INS_JEQ :
      str = "jeq  ";
      break;
    case INS_JGE :
      str = "jge  ";
      break;
    case INS_JGR :
      str = "jgr  ";
      break;
    case INS_JLE :
      str = "jle  ";
      break;
    case INS_JLS :
      str = "jls  ";
      break;
    case INS_JNE :
      str = "jne  ";
      break;
    case INS_AND :
      str = "and  ";
      break;
    case INS_NOT :
      str = "not  ";
      break;
    case INS_XOR :
      str = "xor  ";
      break;
    case INS_MOV :
      str = "mov ";
      break;
    case INS_MVA :
      str = "mva ";
      break;
    case INS_NOP :
      str = "nop ";
      break;
    case INS_POP :
      str = "pop ";
      break;
    case INS_PSH :
      str = "psh ";
      break;
    case INS_STO :
      str = "sto ";
      break;
    case INS_SUB :
      str = "sub ";
      break;
    case INS_CMP :
      str = "cmp ";
      break;
    case INS_SWP :
      str = "swp ";
      break;
    case INS_SHL :
      str = "shl ";
      break;
    case INS_SHR :
      str = "shr ";
      break;
    case INS_JRF :
      str = "jrf ";
      break;
    case INS_JRB :
      str = "jrb ";
      break;
    case INS_INC :
      str = "inc ";
      break;
    case INS_DEC :
      str = "dec ";
      break;
  };
  chars += printf("%s", str);
  if (instance->lparam.val_type != INSP_NULL) {
    if (instance->lparam.val_type == INSP_REGISTER)
      chars += printRegister(instance->lparam.value.reg);
    else
      chars += printf("%hu", instance->lparam.value.literal);
  }

  if (instance->rparam.val_type != INSP_NULL) {
    chars += printf(", ");
    if (instance->rparam.val_type == INSP_REGISTER)
      chars += printRegister(instance->rparam.value.reg);
    else
      chars += printf("%hu", instance->rparam.value.literal);
  }
  return chars;
}

int parametersNeeded(InsType instruction) {
  switch (instruction) {
    case INS_NOP :
    case INS_HLT :
      return 0;
    case INS_ADD :
    case INS_SUB :
    case INS_MOV :
    case INS_MVA :
    case INS_SWP :
    case INS_STO :
    case INS_SHL :
    case INS_SHR :
    case INS_CMP :
    case INS_AND :
    case INS_XOR :
      return 2;
    case INS_JEQ :
    case INS_JGE :
    case INS_JGR :
    case INS_JLE :
    case INS_JLS :
    case INS_JMP :
    case INS_JNE :
    case INS_POP :
    case INS_PSH :
    case INS_NOT :
    case INS_JRF :
    case INS_JRB :
    case INS_INC :
    case INS_DEC :
      return 1;
    default:
      return 0;
  };
}

CPU createCPU() {
  CPU ret = {
    .registers = {0}, 
    .IP = 0, 
    .FLAG_Z = false,
    .FLAG_ERR = false, 
    .FLAG_G = false,
    .FLAG_L = false,
    .FLAG_HLT = false,
    .memory = {0}
  };
  ret.registers[RG_BP] = MEMORY_SIZE-1;
  ret.registers[RG_SP] = ret.registers[RG_BP];
  return ret;
}

void resetCPU(CPU* cpu) {
  cpu->FLAG_ERR = 0;
  cpu->FLAG_G = 0;
  cpu->FLAG_HLT = 0;
  cpu->FLAG_L = 0;
  cpu->FLAG_Z = 0;
  cpu->IP = 0;
  for (int i = 0; i < MEMORY_SIZE; i++) {
    cpu->memory[i] = 0;
    if (i < __RG_COUNT)
      cpu->registers[i] = 0;
  }
  cpu->registers[RG_BP] = MEMORY_SIZE-1;
  cpu->registers[RG_SP] = cpu->registers[RG_BP];
}
