#ifndef PIPELINE_H
#define PIPELINE_H

#include "Instructions.h"
#include "mu-mips.h"

// project types
typedef enum 
{
	DEFAULT_TYPE = 0,
	LOAD_TYPE, 
	STORE_TYPE, 
	REGISTER_TYPE,
} CONTROL_TYPE;

typedef struct CPU_Pipeline_Reg_Struct{
	uint32_t PC;
	uint32_t IR;   // Machine language instruction
	uint32_t A;    // ALU input
	uint32_t B;    // ALU input
	int32_t IMMED;
	uint32_t ALUOutput;
	uint32_t LMD;
	mips_instr_t instr_data;
	CONTROL_TYPE Control;
} CPU_Pipeline_Reg;


/***************************************************************/
/* Pipeline Registers.                                         */
/***************************************************************/
CPU_Pipeline_Reg IF_ID;
CPU_Pipeline_Reg ID_EX;
CPU_Pipeline_Reg EX_MEM;
CPU_Pipeline_Reg MEM_WB;


// function declarations
void handle_pipeline(); /*IMPLEMENT THIS*/
void WB();  /*IMPLEMENT THIS*/
void MEM(); /*IMPLEMENT THIS*/
void EX();  /*IMPLEMENT THIS*/
void ID();  /*IMPLEMENT THIS*/
void IF();  /*IMPLEMENT THIS*/
void show_pipeline();/*IMPLEMENT THIS*/
void print_program(); /*IMPLEMENT THIS*/


#endif