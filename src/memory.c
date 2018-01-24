#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "memory.h"
#include "mu-mips.h"

mem_region_t MEM_REGIONS[] =
{	// Start			End				Addr
	{ MEM_TEXT_BEGIN,	MEM_TEXT_END,	NULL },
	{ MEM_DATA_BEGIN,	MEM_DATA_END,	NULL },
	{ MEM_KDATA_BEGIN,	MEM_KDATA_END,	NULL },
	{ MEM_KTEXT_BEGIN,	MEM_KTEXT_END,	NULL }
};


/******************************************************************************/
/* Function definitions                                                       */
/******************************************************************************/

/***************************************************************/
/* Read a 32-bit word from memory                              */
/***************************************************************/
uint32_t mem_read_32( uint32_t address )
{
	int i;
	for( i = 0; i < NUM_MEM_REGION; i++ )
	{
		if( ( address >= MEM_REGIONS[i].begin ) &&
			( address <= MEM_REGIONS[i].end ) )
		{
			uint32_t offset = address - MEM_REGIONS[i].begin;
			return ( MEM_REGIONS[i].mem[offset + 3] << 24 ) |
				( MEM_REGIONS[i].mem[offset + 2] << 16 ) |
				( MEM_REGIONS[i].mem[offset + 1] << 8 ) |
				( MEM_REGIONS[i].mem[offset + 0] << 0 );
		}
	}
	return 0;
}


/***************************************************************/
/* Write a 32-bit word to memory                               */
/***************************************************************/
void mem_write_32( uint32_t address, uint32_t value )
{
	int i;
	uint32_t offset;
	for( i = 0; i < NUM_MEM_REGION; i++ )
	{
		if( ( address >= MEM_REGIONS[i].begin ) &&
			( address <= MEM_REGIONS[i].end ) )
		{
			offset = address - MEM_REGIONS[i].begin;

			MEM_REGIONS[i].mem[offset + 3] = ( value >> 24 ) & 0xFF;
			MEM_REGIONS[i].mem[offset + 2] = ( value >> 16 ) & 0xFF;
			MEM_REGIONS[i].mem[offset + 1] = ( value >> 8 ) & 0xFF;
			MEM_REGIONS[i].mem[offset + 0] = ( value >> 0 ) & 0xFF;
		}
	}
}


/***************************************************************/
/* Dump a word-aligned region of memory to the terminal        */
/***************************************************************/
void mdump( uint32_t start, uint32_t stop )
{
	uint32_t address;

	printf( "-------------------------------------------------------------\n" );
	printf( "Memory content [0x%08x..0x%08x] :\n", start, stop );
	printf( "-------------------------------------------------------------\n" );
	printf( "\t[Address in Hex (Dec) ]\t[Value]\n" );
	for( address = start; address <= stop; address += 4 )
	{
		printf( "\t0x%08x (%d) :\t0x%08x\n", address, address, mem_read_32( address ) );
	}
	printf( "\n" );
}


/***************************************************************/
/* Dump current values of registers to the teminal             */
/***************************************************************/
void rdump()
{
	int i;
	printf( "-------------------------------------\n" );
	printf( "Dumping Register Content\n" );
	printf( "-------------------------------------\n" );
	printf( "# Instructions Executed\t: %u\n", INSTRUCTION_COUNT );
	printf( "PC\t: 0x%08x\n", CURRENT_STATE.PC );
	printf( "-------------------------------------\n" );
	printf( "[Register]\t[Value]\n" );
	printf( "-------------------------------------\n" );
	for( i = 0; i < MIPS_REGS; i++ )
	{
		printf( "[R%d]\t: 0x%08x\n", i, CURRENT_STATE.REGS[i] );
	}
	printf( "-------------------------------------\n" );
	printf( "[HI]\t: 0x%08x\n", CURRENT_STATE.HI );
	printf( "[LO]\t: 0x%08x\n", CURRENT_STATE.LO );
	printf( "-------------------------------------\n" );
}


/***************************************************************/
/* Allocate and set memory to zero                             */
/***************************************************************/
void init_memory()
{
	int i;
	for( i = 0; i < NUM_MEM_REGION; i++ )
	{
		uint32_t region_size = MEM_REGIONS[i].end - MEM_REGIONS[i].begin + 1;
		MEM_REGIONS[i].mem = malloc( region_size );
		memset( MEM_REGIONS[i].mem, 0, region_size );
	}
}
