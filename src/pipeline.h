#ifndef PIPELINE_H
#define PIPELINE_H


typedef struct CPU_Pipeline_Reg_Struct{
	uint32_t PC;
	uint32_t IR;
	uint32_t A;
	uint32_t B;
	uint32_t imm;
	uint32_t ALUOutput;
	uint32_t LMD;
	
} CPU_Pipeline_Reg;


/***************************************************************/
/* Pipeline Registers.                                         */                                                                                                  */
/***************************************************************/
CPU_Pipeline_Reg IF_ID;
CPU_Pipeline_Reg ID_EX;
CPU_Pipeline_Reg EX_MEM;
CPU_Pipeline_Reg MEM_WB;


// function declarations
void handle_pipeline(); /*IMPLEMENT THIS*/
void WB();/*IMPLEMENT THIS*/
void MEM();/*IMPLEMENT THIS*/
void EX();/*IMPLEMENT THIS*/
void ID();/*IMPLEMENT THIS*/
void IF();/*IMPLEMENT THIS*/
void show_pipeline();/*IMPLEMENT THIS*/
void print_program(); /*IMPLEMENT THIS*/


#endif