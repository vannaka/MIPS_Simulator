#include <stdio.h>
#include <stdint.h>

#include "Instructions.h"
#include "mu-mips.h"

mips_instr_t opcode_0x00_table[0x2A + 1] =
{
			// Name			type	OCode	FCode	RD?		RS?		RT?		Offset?	Addr?	FPtr	SubTable
	[0x00] = { "SLL",		R_TYPE, 0x00,	0x00,	true,	true,	true,	false,	false,	NULL,	NULL },
	[0x02] = { "SRL",		R_TYPE, 0x00,	0x02,	true,	true,	true,	false,	false,	NULL,	NULL },
	[0x03] = { "SRA",		R_TYPE, 0x00,	0x03,	true,	true,	true,	false,	false,	NULL,	NULL },
	[0x08] = { "JR",		R_TYPE, 0x00,	0x08,	false,	true,	false,	false,	false,	NULL,	NULL },
	[0x09] = { "JALR",		R_TYPE, 0x00,	0x09,	true,	true,	false,	false,	false,	NULL,	NULL },
	[0x0C] = { "SYSCALL",	R_TYPE, 0x00,	0x0C,	false,	false,	false,	false,	false,	NULL,	NULL },
	[0x10] = { "MFHI",		R_TYPE, 0x00,	0x10,	true,	false,	false,	false,	false,	NULL,	NULL },
	[0x11] = { "MTHI",		R_TYPE, 0x00,	0x11,	false,	true,	false,	false,	false,	NULL,	NULL },
	[0x12] = { "MFLO",		R_TYPE, 0x00,	0x12,	true,	false,	false,	false,	false,	NULL,	NULL },
	[0x13] = { "MTLO",		R_TYPE, 0x00,	0x13,	false,	true,	false,	false,	false,	NULL,	NULL },
	[0x18] = { "MULT",		R_TYPE, 0x00,	0x18,	false,	true,	true,	false,	false,	NULL,	NULL },
	[0x19] = { "MULTU",		R_TYPE, 0x00,	0x19,	false,	true,	true,	false,	false,	NULL,	NULL },
	[0x1A] = { "DIV",		R_TYPE, 0x00,	0x1A,	false,	true,	true,	false,	false,	NULL,	NULL },
	[0x1B] = { "DIVU",		R_TYPE, 0x00,	0x1B,	false,	true,	true,	false,	false,	NULL,	NULL },
	[0x20] = { "ADD",		R_TYPE,	0x00,	0x20,	true,	true,	true,	false,	false,	NULL,	NULL },
	[0x21] = { "ADDU",		R_TYPE, 0x00,	0x21,	true,	true,	true,	false,	false,	NULL,	NULL },
	[0x22] = { "SUB",		R_TYPE, 0x00,	0x22,	true,	true,	true,	false,	false,	NULL,	NULL },
	[0x23] = { "SUBU",		R_TYPE, 0x00,	0x23,	true,	true,	true,	false,	false,	NULL,	NULL },
	[0x24] = { "AND",		R_TYPE, 0x00,	0x24,	true,	true,	true,	false,	false,	NULL,	NULL },
	[0x25] = { "OR",		R_TYPE, 0x00,	0x25,	true,	true,	true,	false,	false,	NULL,	NULL },
	[0x26] = { "XOR",		R_TYPE, 0x00,	0x26,	true,	true,	true,	false,	false,	NULL,	NULL },
	[0x27] = { "NOR",		R_TYPE, 0x00,	0x27,	true,	true,	true,	false,	false,	NULL,	NULL },
	[0x2A] = { "SLT",		R_TYPE, 0x00,	0x2A,	true,	true,	true,	false,	false,	NULL,	NULL }
};

mips_instr_t opcode_0x01_table[2] =
{
		//  Name	type	OCode	FCode	RD?		RS?		RT?		Offset?	Addr?	FPtr	SubTable
	[0] = { "BLTZ",	I_TYPE,	0x01,	0x00,	false,	true,	false,	true,	false,	NULL,	NULL },
	[1] = { "BGEZ",	I_TYPE, 0x01,	0x00,	false,	true,	false,	true,	false,	NULL,	NULL }
};


mips_instr_t mips_instr_lookup[0x2B + 1] =
{
			//  Name		type	OCode	FCode	RD?		RS?		RT?		Offset?	Addr?	FunctPtr			SubTable
	[0x00] = { "XXXX",		M_TYPE,	0x00,	0x00,	false,	false,	false,	false,	false,	NULL,				opcode_0x00_table },
	[0x01] = { "XXXX",		M_TYPE,	0x01,	0x00,	false,	false,	false,	false,	false,	NULL,				opcode_0x01_table },
	[0x02] = { "J",			J_TYPE,	0x02,	0x00,	false,	false,	false,	false,	true,	NULL,				NULL },
	[0x03] = { "JAL",		J_TYPE,	0x03,	0x00,	false,	false,	false,	false,	true,	NULL,				NULL },
	[0x04] = { "BEQ",		I_TYPE,	0x04,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x05] = { "BNE",		I_TYPE,	0x05,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x06] = { "BLEZ",		I_TYPE,	0x06,	0x00,	false,	true,	false,	true,	false,	NULL,				NULL },
	[0x07] = { "BGTZ",		I_TYPE,	0x07,	0x00,	false,	true,	false,	true,	false,	NULL,				NULL },
	[0x08] = { "ADDI",		I_TYPE,	0x08,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x09] = { "ADDIU",		I_TYPE,	0x09,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x0A] = { "SLTI",		I_TYPE,	0x0A,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x0C] = { "ANDI",		I_TYPE,	0x0C,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x0D] = { "ORI",		I_TYPE,	0x0D,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x0E] = { "XORI",		I_TYPE,	0x0E,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x0F] = { "LUI",		I_TYPE,	0x0F,	0x00,	false,	false,	true,	true,	false,	&instr_handler_LUI,	NULL },
	[0x20] = { "LB",		I_TYPE,	0x20,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x21] = { "LH",		I_TYPE,	0x21,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x23] = { "LW",		I_TYPE,	0x23,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x28] = { "SB",		I_TYPE,	0x28,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x29] = { "SH",		I_TYPE,	0x29,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL },
	[0x2B] = { "SW",		I_TYPE,	0x2B,	0x00,	false,	true,	true,	true,	false,	NULL,				NULL }
};

char mips_reg_names[][5] =
{
	[0] = "zero",
	[1] = "at",
	[2] = "v0",
	[3] = "v1",
	[4] = "a0",
	[5] = "a1",
	[6] = "a2",
	[7] = "a3",
	[8] = "t0",
	[9] = "t1",
	[10] = "t2",
	[11] = "t3",
	[12] = "t4",
	[13] = "t5",
	[14] = "t6",
	[15] = "t7",
	[16] = "s0",
	[17] = "s1",
	[18] = "s2",
	[19] = "s3",
	[20] = "s4",
	[21] = "s5",
	[22] = "s6",
	[23] = "s7",
	[24] = "t8",
	[25] = "t9",
	[26] = "k0",
	[27] = "k1",
	[28] = "gp",
	[29] = "sp",
	[30] = "fp",
	[31] = "ra"
};

mips_instr_t mips_instr_decode( uint32_t instr )
{
	uint8_t opcode;
	uint8_t funct_code;
	uint8_t rt;
	mips_instr_t instr_info;


	opcode = GET_OPCODE( instr );
	funct_code = -1;
	rt = -1;

	if( opcode == 0 )
	{
		funct_code = GET_FUNCTCODE( instr );
		instr_info = mips_instr_lookup[opcode].subtable[funct_code];
	}
	else if( opcode == 1 )
	{
		rt = GET_RT( instr );
		instr_info = mips_instr_lookup[opcode].subtable[rt];
	}
	else
	{
		instr_info = mips_instr_lookup[opcode];

	}

	return instr_info;
}

void instr_handler_LUI()
{
	printf( "LUI executed\n\n" );
}

// ADD INSTRUCTION HANDLERS HERE

// DON'T FORGET TO ADD THEIR DECLARATIONS IN THE HEADER FILE
