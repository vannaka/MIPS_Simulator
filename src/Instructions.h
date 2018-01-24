#ifndef  INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include <stdbool.h>

#define GET_OPCODE(x)		( ( x >> 26 ) & 0x3F )
#define GET_RS(x)			( ( x >> 21 ) & 0x1F )
#define GET_RT(x)			( ( x >> 16 ) & 0x1F )
#define GET_RD(x)			( ( x >> 11 ) & 0x1F )
#define GET_FUNCTCODE(x)	( x & 0x0000003F )
#define GET_OFFSET(x)		( x & 0xFFFF )
#define GET_ADDRESS(x)		( x & 0x3FFFFFF )

// project types
typedef enum 
{
	INVAL = 0,
	R_TYPE, 
	I_TYPE, 
	J_TYPE,
	M_TYPE
} mips_inst_type_t;

typedef struct mips_instr
{
	char name[10];
	mips_inst_type_t type;
	uint8_t opcode;
	uint8_t funct_code;
	bool rd;
	bool rs;
	bool rt;
	bool offset;
	bool addr;
	void(*funct)();
	struct mips_instr *subtable;
} mips_instr_t;

// Declare lookup tables
extern mips_instr_t opcode_0x00_table[];
extern mips_instr_t opcode_0x01_table[];
extern mips_instr_t mips_instr_lookup[];
extern char mips_reg_names[][5];

// Declare instruction handlers
void mips_instr_LUI();
mips_instr_t mips_instr_decode( uint32_t instr );

#endif // ! INSTRUCTIONS_H
