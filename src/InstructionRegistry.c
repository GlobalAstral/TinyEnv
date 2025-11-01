#include <InstructionRegistry.h>

InstructionRegistry REGISTRY;

void createInstruction(Instruction* i, InsType type, InstructionCallback callback) {
  i->type = type;
  i->callback = callback;
}

Instruction* getInstruction(InsType type) {
  return &REGISTRY[type];
}

void registerInstructions() { 
  createInstruction(&REGISTRY[INS_NOP], INS_NOP, nop_exec);
  createInstruction(&REGISTRY[INS_MVA], INS_MVA, mva_exec);
  createInstruction(&REGISTRY[INS_MOV], INS_MOV, mov_exec);
  createInstruction(&REGISTRY[INS_STO], INS_STO, sto_exec);
  createInstruction(&REGISTRY[INS_ADD], INS_ADD, add_exec);
  createInstruction(&REGISTRY[INS_SUB], INS_SUB, sub_exec);
  createInstruction(&REGISTRY[INS_CMP], INS_CMP, cmp_exec);
  createInstruction(&REGISTRY[INS_JMP], INS_JMP, jmp_exec);
  createInstruction(&REGISTRY[INS_JEQ], INS_JEQ, jeq_exec);
  createInstruction(&REGISTRY[INS_JLS], INS_JLS, jls_exec);
  createInstruction(&REGISTRY[INS_JGR], INS_JGR, jgr_exec);
  createInstruction(&REGISTRY[INS_JLE], INS_JLE, jle_exec);
  createInstruction(&REGISTRY[INS_JGE], INS_JGE, jge_exec);
  createInstruction(&REGISTRY[INS_JNE], INS_JNE, jne_exec);
  createInstruction(&REGISTRY[INS_AND], INS_AND, and_exec);
  createInstruction(&REGISTRY[INS_NOT], INS_NOT, not_exec);
  createInstruction(&REGISTRY[INS_XOR], INS_XOR, xor_exec);
  createInstruction(&REGISTRY[INS_SWP], INS_SWP, swp_exec);
  createInstruction(&REGISTRY[INS_PSH], INS_PSH, psh_exec);
  createInstruction(&REGISTRY[INS_POP], INS_POP, pop_exec);
  createInstruction(&REGISTRY[INS_HLT], INS_HLT, hlt_exec);
  createInstruction(&REGISTRY[INS_SHL], INS_SHL, shl_exec);
  createInstruction(&REGISTRY[INS_SHR], INS_SHR, shr_exec);
  createInstruction(&REGISTRY[INS_JRF], INS_JRF, jrf_exec);
  createInstruction(&REGISTRY[INS_JRB], INS_JRB, jrb_exec);
  createInstruction(&REGISTRY[INS_INC], INS_INC, inc_exec);
  createInstruction(&REGISTRY[INS_DEC], INS_DEC, dec_exec);
}

bool nop_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type != INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  return true;
}

bool mva_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type == INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  
  if (lparam->val_type != INSP_REGISTER) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 address = (rparam->val_type == INSP_LITERAL) ? rparam->value.literal : cpu->registers[rparam->value.reg];
  if (address >= MEMORY_SIZE) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  Register* reg = &(cpu->registers[lparam->value.reg]);
  *reg = cpu->memory[address];

  return true;
}

bool mov_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type == INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  if (lparam->val_type == INSP_LITERAL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  Register* reg = &(cpu->registers[lparam->value.reg]);

  if (rparam->val_type == INSP_LITERAL)
    *reg = rparam->value.literal;
  else
    *reg = cpu->registers[rparam->value.reg];

  return true;
}

bool sto_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type == INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 address = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  if (address >= MEMORY_SIZE) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  u16 value = (rparam->val_type == INSP_LITERAL) ? rparam->value.literal : cpu->registers[rparam->value.reg];
  cpu->memory[address] = value;

  return true;
}

bool add_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type == INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  if (lparam->val_type != INSP_REGISTER) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  Register* reg = &(cpu->registers[lparam->value.reg]);
  u16 value = (rparam->val_type == INSP_LITERAL) ? rparam->value.literal : cpu->registers[rparam->value.reg];
  *reg = *reg + value;

  return true;
}

bool sub_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type == INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  if (lparam->val_type != INSP_REGISTER) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  Register* reg = &(cpu->registers[lparam->value.reg]);
  u16 value = (rparam->val_type == INSP_LITERAL) ? rparam->value.literal : cpu->registers[rparam->value.reg];
  *reg = *reg - value;

  return true;
}

bool cmp_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type == INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value1 = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  u16 value2 = (rparam->val_type == INSP_LITERAL) ? rparam->value.literal : cpu->registers[rparam->value.reg];

  i16 sub = (value1 - value2);

  cpu->FLAG_Z = sub == 0;
  cpu->FLAG_G = sub > 0;
  cpu->FLAG_L = sub < 0;

  return true;
}

bool jmp_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  cpu->IP = value;
  return true;
}

bool jeq_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  if (cpu->FLAG_Z)
    cpu->IP = value;
  return true;
}

bool jls_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  if (cpu->FLAG_L)
    cpu->IP = value;
  return true;
}

bool jgr_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  if (cpu->FLAG_G)
    cpu->IP = value;
  return true;
}

bool jle_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  if (cpu->FLAG_L || cpu->FLAG_Z)
    cpu->IP = value;
  return true;
}

bool jge_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  if (cpu->FLAG_G || cpu->FLAG_Z)
    cpu->IP = value;
  return true;
}

bool jne_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  if (!cpu->FLAG_Z)
    cpu->IP = value;
  return true;
}

bool and_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type == INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  
  if (lparam->val_type != INSP_REGISTER) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (rparam->val_type == INSP_LITERAL) ? rparam->value.literal : cpu->registers[rparam->value.reg];
  
  Register* reg = &(cpu->registers[lparam->value.reg]);
  *reg = *reg & value;

  return true;
}

bool not_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  if (lparam->val_type != INSP_REGISTER) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  Register* reg = &(cpu->registers[lparam->value.reg]);
  *reg = ~(*reg);
  return true;
}

bool xor_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type == INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  
  if (lparam->val_type != INSP_REGISTER) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (rparam->val_type == INSP_LITERAL) ? rparam->value.literal : cpu->registers[rparam->value.reg];
  
  Register* reg = &(cpu->registers[lparam->value.reg]);
  *reg = *reg ^ value;

  return true;
}

bool swp_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type == INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  
  if (lparam->val_type != INSP_REGISTER || rparam->val_type != INSP_REGISTER) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  
  Register* reg1 = &(cpu->registers[lparam->value.reg]);
  Register* reg2 = &(cpu->registers[rparam->value.reg]);
  Register temp = *reg1;
  *reg1 = *reg2;
  *reg2 = temp;

  return true;
}

bool psh_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  
  Register* sp = &(cpu->registers[RG_SP]);
  cpu->memory[*sp] = value;
  *sp = *sp - 1;

  return true;
}

bool pop_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  if (lparam->val_type == INSP_LITERAL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  Register* reg = &(cpu->registers[lparam->value.reg]);
  Register* sp = &(cpu->registers[RG_SP]);
  *sp = *sp + 1;
  *reg = cpu->memory[*sp];

  return true;
}

bool hlt_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type != INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  cpu->FLAG_HLT = true;

  return true;
}

bool shl_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type == INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  if (lparam->val_type == INSP_LITERAL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  Register* reg = &(cpu->registers[lparam->value.reg]);

  u16 value = (rparam->val_type == INSP_LITERAL) ? rparam->value.literal : cpu->registers[rparam->value.reg];

  *reg = *reg << value;

  return true;
}

bool shr_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type == INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  if (lparam->val_type == INSP_LITERAL) {
    cpu->FLAG_ERR = 1;
    return false;
  }
  Register* reg = &(cpu->registers[lparam->value.reg]);

  u16 value = (rparam->val_type == INSP_LITERAL) ? rparam->value.literal : cpu->registers[rparam->value.reg];

  *reg = *reg >> value;

  return true;
}

bool jrf_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  cpu->IP += value;
  return true;
}

bool jrb_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  u16 value = (lparam->val_type == INSP_LITERAL) ? lparam->value.literal : cpu->registers[lparam->value.reg];
  cpu->IP += value;
  return true;
}

bool inc_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  if (lparam->val_type != INSP_REGISTER) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  Register* reg = &cpu->registers[lparam->value.reg];
  *reg = *reg + 1;
  return true;
}

bool dec_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam) {
  if (lparam->val_type == INSP_NULL || rparam->val_type != INSP_NULL) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  if (lparam->val_type != INSP_REGISTER) {
    cpu->FLAG_ERR = 1;
    return false;
  }

  Register* reg = &cpu->registers[lparam->value.reg];
  *reg = *reg - 1;
  return true;
}
