#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>

#include "Instructions.h"

char* instr_str;

mips_instr_t assem_decode_opcode( char* str );
void assem_operand_decode( inst_op_type_t, char*, uint32_t* );

#endif // ! ASSEMBLER_H