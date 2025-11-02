#pragma once

#include <TE_Types.h>

typedef Instruction InstructionRegistry[__INS_COUNT];
void registerInstructions();
void createInstruction(Instruction* i, InsType type, InstructionCallback callback);
Instruction* getInstruction(InsType type);
static InstructionParam EMPTY_PARAM = {.val_type = INSP_NULL};

bool nop_exec(CPU* cpu, InstructionParam* lparam, InstructionParam* rparam);
bool mva_exec(CPU* cpu, InstructionParam* lparam, InstructionParam* rparam);
bool mov_exec(CPU* cpu, InstructionParam* lparam, InstructionParam* rparam);
bool sto_exec(CPU* cpu, InstructionParam* lparam, InstructionParam* rparam);
bool add_exec(CPU* cpu, InstructionParam* lparam, InstructionParam* rparam);
bool sub_exec(CPU* cpu, InstructionParam* lparam, InstructionParam* rparam);
bool cmp_exec(CPU* cpu, InstructionParam* lparam, InstructionParam* rparam);
bool jmp_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool jeq_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool jls_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool jgr_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool jle_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool jge_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool jne_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool and_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool not_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool xor_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool swp_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool psh_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool pop_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool hlt_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool shl_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool shr_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool jrf_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool jrb_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool inc_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool dec_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool low_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool hig_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool mrg_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool cal_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
bool ret_exec(CPU *cpu, InstructionParam *lparam, InstructionParam *rparam);
