#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "mu-mips.h"
#include "Instructions.h"
#include "memory.h"
#include "pipeline.h"
#include "mu-cache.h"

/***************************************************************/
/* Print out a list of commands available                      */
/***************************************************************/
void help() 
{        
	printf("------------------------------------------------------------------\n\n");
	printf("\t**********MU-MIPS Help MENU**********\n\n");
	printf("sim\t-- simulate program to completion \n");
	printf("run <n>\t-- simulate program for <n> instructions\n");
	printf("rdump\t-- dump register values\n");
	printf("reset\t-- clears all registers/memory and re-loads the program\n");
	printf("input <reg> <val>\t-- set GPR <reg> to <val>\n");
	printf("mdump <start> <stop>\t-- dump memory from <start> to <stop> address\n");
	printf("high <val>\t-- set the HI register to <val>\n");
	printf("low <val>\t-- set the LO register to <val>\n");
	printf("print\t-- print the program loaded into memory\n");
	printf("pipe\t-- print the pipeline buffers in memory\n");
	printf("forward\t-- enable/disable forwarding\n");
	printf("?\t-- display help menu\n");
	printf("quit\t-- exit the simulator\n\n");
	printf("------------------------------------------------------------------\n\n");
}


/***************************************************************/
/* Execute one cycle                                           */
/***************************************************************/
void cycle() 
{                                                
	//handle_instruction();
	handle_pipeline();
	//CURRENT_STATE = NEXT_STATE;
	INSTRUCTION_COUNT++;
}


/***************************************************************/
/* Simulate MIPS for n cycles                                  */
/***************************************************************/
void run( int num_cycles ) 
{                                      
	
	if( RUN_FLAG == false ) 
	{
		printf("Simulation Stopped\n\n");
		return;
	}

	printf("Running simulator for %d cycles...\n\n", num_cycles);
	int i;
	for( i = 0; i < num_cycles; i++ ) 
	{
		if( RUN_FLAG == false ) 
		{
			printf("Simulation Stopped.\n\n");
			break;
		}
		printf( "[0x%x]\t", CURRENT_STATE.PC );
		print_instruction( CURRENT_STATE.PC );
		cycle();
	}
}


/***************************************************************/
/* simulate to completion                                      */
/***************************************************************/
void runAll() 
{                                                     
	if( RUN_FLAG == false ) 
	{
		printf( "Simulation Stopped.\n\n" );
		return;
	}

	printf( "Simulation Started...\n\n" );
	while( RUN_FLAG )
	{
		cycle();
	}
	printf( "Simulation Finished.\n\n" );
}


/***************************************************************/
/* Read a command from standard input.                         */  
/***************************************************************/
void handle_command() 
{                         
	char buffer[20];
	uint32_t start, stop, cycles;
	uint32_t register_no;
	int register_value;
	int hi_reg_value, lo_reg_value;

	printf( "\nMU-MIPS SIM:> " );

	if( scanf( "%s", buffer ) == EOF )
	{
		exit(0);
	}

	switch( buffer[0] ) 
	{
		case 'S':
		case 's':
			runAll(); 
			break;

		case 'M':
		case 'm':
			if( scanf( "%x %x", &start, &stop ) != 2 )
			{
				break;
			}
			mdump( start, stop );
			break;

		case '?':
			help();
			break;

		case 'Q':
		case 'q':
			printf( "**************************\n" );
			printf( "Exiting MU-MIPS! Good Bye...\n" );
			printf( "**************************\n" );
			exit(0);

		case 'R':
		case 'r':
			if( buffer[1] == 'd' || buffer[1] == 'D' )
			{
				rdump();
			}
			else if( buffer[1] == 'e' || buffer[1] == 'E' )
			{
				reset();
			}
			else 
			{
				if( scanf( "%d", &cycles ) != 1 )
				{
					break;
				}
				run(cycles);
			}
			break;

		case 'I':
		case 'i':
			if( scanf( "%u %i", &register_no, &register_value ) != 2 )
			{
				break;
			}
			CURRENT_STATE.REGS[register_no] = register_value;
			NEXT_STATE.REGS[register_no] = register_value;
			break;

		case 'H':
		case 'h':
			if( scanf("%i", &hi_reg_value ) != 1)
			{
				break;
			}
			CURRENT_STATE.HI = hi_reg_value; 
			NEXT_STATE.HI = hi_reg_value; 
			break;

		case 'L':
		case 'l':
			if( scanf( "%i", &lo_reg_value ) != 1 )
			{
				break;
			}
			CURRENT_STATE.LO = lo_reg_value;
			NEXT_STATE.LO = lo_reg_value;
			break;

		case 'P':
		case 'p':
			if( buffer[1] == 'r' || buffer[1] == 'R' )
			{
				print_program();
			}
			else if( buffer[1] == 'i' || buffer[1] == 'I' )
			{
				show_pipeline();
			}		 
			break;
		case 'f':
		case 'F':
			if (scanf("%d", (int*)&ENABLE_FORWARDING) != 1) {
				break;
			}
				ENABLE_FORWARDING == 0 ? printf("Forwarding OFF\n") : printf("Forwarding ON\n");
			break;
		default:
			printf( "Invalid Command.\n" );
			break;
	}
}


/***************************************************************/
/* reset registers/memory and reload program                   */
/***************************************************************/
void reset() 
{   
	int i;
	/*reset registers*/
	for( i = 0; i < MIPS_REGS; i++ )
	{
		CURRENT_STATE.REGS[i] = 0;
	}
	CURRENT_STATE.HI = 0;
	CURRENT_STATE.LO = 0;
	
	for( i = 0; i < NUM_MEM_REGION; i++ ) 
	{
		uint32_t region_size = MEM_REGIONS[i].end - MEM_REGIONS[i].begin + 1;
		memset( MEM_REGIONS[i].mem, 0, region_size );
	}
	
	memset( &IF_ID, 0, sizeof(CPU_Pipeline_Reg) );
	memset( &ID_EX, 0, sizeof(CPU_Pipeline_Reg) );
	memset( &EX_MEM, 0, sizeof(CPU_Pipeline_Reg) );
	memset( &MEM_WB, 0, sizeof(CPU_Pipeline_Reg) );
    memset( &L1Cache, 0, sizeof(Cache) );

	/*load program*/
	load_program();
	
	/*reset PC*/
	INSTRUCTION_COUNT = 0;
	CURRENT_STATE.PC =  MEM_TEXT_BEGIN;
	NEXT_STATE = CURRENT_STATE;
	RUN_FLAG = true;
    cache_misses = 0;
    cache_hits = 0;
}


/**************************************************************/
/* load program into memory                                   */
/**************************************************************/
void load_program() 
{                   
	FILE * fp;
	int i, word;
	uint32_t address;

	/* Open program file. */
	fp = fopen( prog_file, "r" );
	if( fp == NULL )
	{
		printf( "Error: Can't open program file %s\n", prog_file );
		exit(-1);
	}

	/* Read in the program. */
	i = 0;
	while( fscanf( fp, "%x\n", &word ) != EOF ) 
	{
		address = MEM_TEXT_BEGIN + i;
		mem_write_32( address, word );
		printf( "writing 0x%08x into address 0x%08x (%d)\n", word, address, address );
		i += 4;
	}
	PROGRAM_SIZE = i/4;
	printf( "Program loaded into memory.\n%d words written into memory.\n\n", PROGRAM_SIZE );
	fclose( fp );
}


/************************************************************/
/* decode and execute instruction                           */ 
/************************************************************/
void handle_instruction()
{
	uint32_t instr;
	mips_instr_t instr_info;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	// Decode instruction
	instr_info = mips_instr_decode( instr );

	// Call instruction handler
	( *( instr_info.funct ) )();
}


/************************************************************/
/* Initialize Memory                                        */ 
/************************************************************/
void initialize() 
{ 
	init_memory();
	CURRENT_STATE.PC = MEM_TEXT_BEGIN;
	NEXT_STATE = CURRENT_STATE;
	RUN_FLAG = true;
    cache_misses = 0;
    cache_hits = 0;
}


/************************************************************/
/* Print the program loaded into memory (in MIPS assembly format)    */ 
/************************************************************/
void print_program()
{
	int i;
	uint32_t addr;
	
	for( i=0; i<PROGRAM_SIZE; i++ )
	{
		addr = MEM_TEXT_BEGIN + (i*4);
		printf( "[0x%x]\t", addr );
		print_instruction( addr );
	}
}

void show_pipeline()
{
	printf("\nCurrent PC:\t\t[0x%x]\n",CURRENT_STATE.PC);
	if(IF_ID.IR != 0) {
		printf("IF/ID.IR:\t\t"); print_instruction(IF_ID.PC);
		printf("IF/ID.PC:\t\t0x%08x\n\n",IF_ID.PC);
	}
	if(ID_EX.IR != 0) {
		printf("ID/EX.IR:\t\t"); print_instruction(ID_EX.PC);
		printf("ID/EX.A:\t\t0x%08x\n",ID_EX.A);
		printf("ID/EX.B:\t\t0x%08x\n",ID_EX.B);
		printf("ID/EX.IMMED:\t\t0x%08x\n\n",ID_EX.IMMED);
	}
	if(EX_MEM.IR != 0) {
		printf("EX/MEM.IR:\t\t"); print_instruction(EX_MEM.PC);
		printf("EX/MEM.A:\t\t0x%08x\n",EX_MEM.A);
		printf("EX/MEM.B:\t\t0x%08x\n",EX_MEM.B);
		printf("EX/MEM.ALUOutput:\t0x%08x\n\n",EX_MEM.ALUOutput);
	}
	if(MEM_WB.IR != 0) {
		printf("MEM/WB.IR:\t\t"); print_instruction(MEM_WB.PC);
		printf("MEM/WB.ALUOutput:\t0x%08x\n",MEM_WB.ALUOutput);
		printf("MEM/WB.LMD:\t\t0x%08x\n\n",MEM_WB.LMD);
	}
}
/************************************************************/
/* Print the instruction at given memory address (in MIPS assembly format)    */
/************************************************************/
void print_instruction( uint32_t addr )
{
	uint32_t instr;
	uint8_t rd, rs, rt, sa;
	uint16_t immed;
	uint32_t address;
	mips_instr_t instr_info;
	
	instr = mem_read_32( addr );

	// Decode instruction
	instr_info = mips_instr_decode( instr );

	// Display hex instruction
	printf( "0x%08x\t", instr );

	// Display instruction mnumonic
	printf( "%s", instr_info.name );
	
	// Display opperands
	for( int i = 0; i < 3; i++ )
	{
		inst_op_type_t op_type = instr_info.skeleton[i];

		if( op_type == RS )
		{
			rs = GET_RS( instr );
			printf( " $%s", mips_reg_names[rs] );
		}
		else if( op_type == RT )
		{
			rt = GET_RT( instr );
			printf( " $%s", mips_reg_names[rt] );
		}
		else if( op_type == RD )
		{
			rd = GET_RD( instr );
			printf( " $%s", mips_reg_names[rd] );
		}
		else if (op_type == SA)
		{
			sa = GET_SA(instr);
			printf(" 0x%04x", sa );
		}
		else if( op_type == IMMED )
		{
			immed = GET_IMMED( instr );
			printf( " 0x%04x", immed );
		}
		else if( op_type == ADDRESS )
		{
			address = GET_ADDRESS( instr );
			printf( " 0x%07x", address );
		}
		else
		{
			break;
		}
	}

	printf( "\n" );

}	/* print_instruction() */


/***************************************************************/
/* main                                                        */
/***************************************************************/
int main( int argc, char *argv[] ) 
{                              
	printf( "\n**************************\n" );
	printf( "Welcome to MU-MIPS SIM...\n" );
	printf( "**************************\n\n" );
	
	if( argc < 2 ) 
	{
		printf( "Error: You should provide input file.\nUsage: %s <input program> \n\n",  argv[0] );
		exit(1);
	}
	ENABLE_FORWARDING = 0;
	strcpy( prog_file, argv[1] );
	initialize();
	load_program();
	help();
	while (1)
	{
		handle_command();
	}
	return 0;

}
