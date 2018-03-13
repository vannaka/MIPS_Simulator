#ifndef MU_MIPS_H
#define MU_MIPS_H

#include <stdlib.h>
#include <stdint.h>

#define MIPS_REGS 32

typedef struct CPU_State_Struct
{
	uint32_t PC;		            /* program counter */
	uint32_t REGS[MIPS_REGS]; 	/* register file. */
	uint32_t HI, LO;              /* special regs for mult/div. */
} CPU_State;

/***************************************************************/
/* CPU State info.                                             */
/***************************************************************/
CPU_State CURRENT_STATE, NEXT_STATE;
int RUN_FLAG;	/* run flag*/
uint32_t INSTRUCTION_COUNT;
uint32_t PROGRAM_SIZE; /*in words*/

char prog_file[64];

uint8_t STALL;
uint8_t ENABLE_FORWARDING;

/***************************************************************/
/* Function Declerations.                                      */
/***************************************************************/
void help();
void cycle();
void run( int num_cycles );
void runAll();
void handle_command();
void reset();
void load_program();
void handle_instruction();
void initialize();
void print_program();
void print_instruction( uint32_t );

#endif // !MU_MIPS_H
