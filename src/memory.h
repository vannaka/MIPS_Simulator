#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

/******************************************************************************/
/* MIPS memory layout                                                         */
/******************************************************************************/
#define MEM_TEXT_BEGIN  0x00400000
#define MEM_TEXT_END    0x0FFFFFFF
/*Memory address 0x10000000 to 0x1000FFFF access by $gp*/
#define MEM_DATA_BEGIN	0x10010000
#define MEM_DATA_END	0x7FFFFFFF

#define MEM_KTEXT_BEGIN 0x80000000
#define MEM_KTEXT_END	0x8FFFFFFF

#define MEM_KDATA_BEGIN 0x90000000
#define MEM_KDATA_END	0xFFFEFFFF

/*stack and data segments occupy the same memory space. Stack grows backward (from higher address to lower address) */
#define MEM_STACK_BEGIN 0x7FFFFFFF
#define MEM_STACK_END	0x10010000

#define NUM_MEM_REGION 4


/******************************************************************************/
/* Memory Structures                                                          */
/******************************************************************************/
typedef struct
{
	uint32_t begin, end;
	uint8_t *mem;
} mem_region_t;


/* memory will be dynamically allocated at initialization */
extern mem_region_t MEM_REGIONS[];


/******************************************************************************/
/* Function declarations                                                      */
/******************************************************************************/
uint32_t mem_read_32( uint32_t address );
void mem_write_32( uint32_t address, uint32_t value );
void mdump( uint32_t start, uint32_t stop );
void rdump();
void init_memory();

#endif // !MEMORY_H
