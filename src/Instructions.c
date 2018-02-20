#include <stdio.h>
#include <stdint.h>

#include "Instructions.h"
#include "mu-mips.h"
#include "memory.h"

mips_instr_t opcode_0x00_table[0x2A + 1] =
{
			// Name			type	OCode	FCode	Makeup			FPtr	                SubTable
	[0x00] = { "SLL",		R_TYPE, 0x00,	0x00,	{RD, RT, SA},	&instr_handler_SLL,		NULL },
	[0x02] = { "SRL",		R_TYPE, 0x00,	0x02,	{RD, RT, SA},	&instr_handler_SRL,		NULL },
	[0x03] = { "SRA",		R_TYPE, 0x00,	0x03,	{RD, RT, SA},	&instr_handler_SRA,		NULL },
	[0x08] = { "JR",		R_TYPE, 0x00,	0x08,	{RS},			&instr_handler_JR,		NULL },
	[0x09] = { "JALR",		R_TYPE, 0x00,	0x09,	{RD, RS},		&instr_handler_JALR,	NULL },
	[0x0C] = { "SYSCALL",	R_TYPE, 0x00,	0x0C,	{ },			&instr_handler_SYSCALL,	NULL },
	[0x10] = { "MFHI",		R_TYPE, 0x00,	0x10,	{RD},			&instr_handler_MFHI,	NULL },
	[0x11] = { "MTHI",		R_TYPE, 0x00,	0x11,	{RS},			&instr_handler_MTHI,	NULL },
	[0x12] = { "MFLO",		R_TYPE, 0x00,	0x12,	{RD},			&instr_handler_MFLO,	NULL },
	[0x13] = { "MTLO",		R_TYPE, 0x00,	0x13,	{RS},			&instr_handler_MTLO,	NULL },
	[0x18] = { "MULT",		R_TYPE, 0x00,	0x18,	{RS, RT},		&instr_handler_MULT,	NULL },
	[0x19] = { "MULTU",		R_TYPE, 0x00,	0x19,	{RS, RT},		&instr_handler_MULTU,	NULL },
	[0x1A] = { "DIV",		R_TYPE, 0x00,	0x1A,	{RS, RT},		&instr_handler_DIV,		NULL },
	[0x1B] = { "DIVU",		R_TYPE, 0x00,	0x1B,	{RS, RT},		&instr_handler_DIVU,	NULL },
	[0x20] = { "ADD",		R_TYPE,	0x00,	0x20,	{RD, RS, RT},	&instr_handler_ADD,		NULL },
	[0x21] = { "ADDU",		R_TYPE, 0x00,	0x21,	{RD, RS, RT},	&instr_handler_ADDU,	NULL },
	[0x22] = { "SUB",		R_TYPE, 0x00,	0x22,	{RD, RS, RT},	&instr_handler_SUB,		NULL },
	[0x23] = { "SUBU",		R_TYPE, 0x00,	0x23,	{RD, RS, RT},	&instr_handler_SUBU,	NULL },
	[0x24] = { "AND",		R_TYPE, 0x00,	0x24,	{RD, RS, RT},	&instr_handler_AND,		NULL },
	[0x25] = { "OR",		R_TYPE, 0x00,	0x25,	{RD, RS, RT},	&instr_handler_OR,		NULL },
	[0x26] = { "XOR",		R_TYPE, 0x00,	0x26,	{RD, RS, RT},	&instr_handler_XOR,		NULL },
	[0x27] = { "NOR",		R_TYPE, 0x00,	0x27,	{RD, RS, RT},	&instr_handler_NOR,		NULL },
	[0x2A] = { "SLT",		R_TYPE, 0x00,	0x2A,	{RD, RS, RT},	&instr_handler_SLT,		NULL }
};

mips_instr_t opcode_0x01_table[2] =
{
			// Name			type	OCode	FCode	Makeup			FPtr					SubTable
	[0x00] = { "BLTZ",		I_TYPE,	0x01,	0x00,	{RS, IMMED},	&instr_handler_BLTZ,	NULL },
	[0x01] = { "BGEZ",		I_TYPE, 0x01,	0x00,	{RS, IMMED},	&instr_handler_BGEZ,	NULL }
};


mips_instr_t mips_instr_lookup[0x2B + 1] =
{
			//  Name		type	OCode	FCode	Makeup				FunctPtr					SubTable
	[0x00] = { "XXXX",		M_TYPE,	0x00,	0x00,	{ },				NULL,						opcode_0x00_table },
	[0x01] = { "XXXX",		M_TYPE,	0x01,	0x00,	{ },				NULL,						opcode_0x01_table },
	[0x02] = { "J",			J_TYPE,	0x02,	0x00,	{ADDRESS},			&instr_handler_J,			NULL },
	[0x03] = { "JAL",		J_TYPE,	0x03,	0x00,	{ADDRESS},			&instr_handler_JAL,			NULL },
	[0x04] = { "BEQ",		I_TYPE,	0x04,	0x00,	{RS, RT, IMMED},	&instr_handler_BEQ,			NULL },
	[0x05] = { "BNE",		I_TYPE,	0x05,	0x00,	{RS, RT, IMMED},	&instr_handler_BNE,			NULL },
	[0x06] = { "BLEZ",		I_TYPE,	0x06,	0x00,	{RS, IMMED},		&instr_handler_BLEZ,		NULL },
	[0x07] = { "BGTZ",		I_TYPE,	0x07,	0x00,	{RS, IMMED},		&instr_handler_BGTZ,		NULL },
	[0x08] = { "ADDI",		I_TYPE,	0x08,	0x00,	{RT, RS, IMMED},	&instr_handler_ADDI,		NULL },
	[0x09] = { "ADDIU",		I_TYPE,	0x09,	0x00,	{RT, RS, IMMED},	&instr_handler_ADDIU,		NULL },
	[0x0A] = { "SLTI",		I_TYPE,	0x0A,	0x00,	{RT, RS, IMMED},	&instr_handler_SLTI,		NULL },
	[0x0C] = { "ANDI",		I_TYPE,	0x0C,	0x00,	{RT, RS, IMMED},	&instr_handler_ANDI,		NULL },
	[0x0D] = { "ORI",		I_TYPE,	0x0D,	0x00,	{RT, RS, IMMED},	&instr_handler_ORI,			NULL },
	[0x0E] = { "XORI",		I_TYPE,	0x0E,	0x00,	{RT, RS, IMMED},	&instr_handler_XORI,		NULL },
	[0x0F] = { "LUI",		I_TYPE,	0x0F,	0x00,	{RT, IMMED},		&instr_handler_LUI,         NULL },
	[0x20] = { "LB",		I_TYPE,	0x20,	0x00,	{RT, IMMED, RS},	&instr_handler_LB,          NULL },
	[0x21] = { "LH",		I_TYPE,	0x21,	0x00,	{RT, IMMED, RS},	&instr_handler_LH,			NULL },
	[0x23] = { "LW",		I_TYPE,	0x23,	0x00,	{RT, IMMED, RS},	&instr_handler_LW,			NULL },
	[0x28] = { "SB",		I_TYPE,	0x28,	0x00,	{RT, IMMED, RS},	&instr_handler_SB,			NULL },
	[0x29] = { "SH",		I_TYPE,	0x29,	0x00,	{RT, IMMED, RS},	&instr_handler_SH,			NULL },
	[0x2B] = { "SW",		I_TYPE,	0x2B,	0x00,	{RT, IMMED, RS},	&instr_handler_SW,			NULL }
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


void instr_handler_SLL()
{
	uint32_t instr;
	uint32_t rt_val;
	uint8_t rt, rd, sa;

	// Get instruction
	instr = mem_read_32(CURRENT_STATE.PC);

	// Get values
	rt = GET_RT( instr );
	rd = GET_RD( instr );
	sa = GET_SA( instr );
	rt_val = CURRENT_STATE.REGS[rt];

	// Shift left
	NEXT_STATE.REGS[rd] = rt_val << sa;

	// Increment program counter
	NEXT_STATE.PC += 4;
}

void instr_handler_SRL()
{
	uint32_t instr;
	uint32_t rt_val;
	uint8_t rt, rd, sa;

	// Get instruction
	instr = mem_read_32(CURRENT_STATE.PC);

	// Get values
	rt = GET_RT( instr );
	rd = GET_RD( instr );
	sa = GET_SA( instr );
	rt_val = CURRENT_STATE.REGS[rt];

	// Shift right and zero extend
	NEXT_STATE.REGS[rd] = rt_val >> sa;

	// Increment program counter
	NEXT_STATE.PC += 4;
}


void instr_handler_SRA()
{
	uint32_t instr;
	int32_t rt_val;
	uint8_t rt, rd, sa;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	// Get values
	rt = GET_RT( instr );
	rd = GET_RD( instr );
	sa = GET_SA( instr );
	rt_val = CURRENT_STATE.REGS[rt];

	// Shift right and sign extend
	NEXT_STATE.REGS[rd] = rt_val >> sa;

	// Increment program counter
	NEXT_STATE.PC += 4;
}


void instr_handler_JR()
{
	uint32_t instr;
	int32_t rs_val;
	uint8_t rs;

	// Get instruction
	instr = mem_read_32(CURRENT_STATE.PC);

	// Get values
	rs = GET_RS(instr);
	rs_val = CURRENT_STATE.REGS[rs];

	// jump to specified address
	NEXT_STATE.PC = rs_val;
}


void instr_handler_JALR()
{
	uint32_t instr;
	int32_t rs_val;
	uint8_t rs, rd;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	// Get values
	rs = GET_RS( instr );
	rd = GET_RD( instr );
	rs_val = CURRENT_STATE.REGS[rs];

	// save return addr
	NEXT_STATE.REGS[rd] = CURRENT_STATE.PC + 4;

	// jmp to new addr
	NEXT_STATE.PC = rs_val;
}


void instr_handler_SYSCALL()
{
	int32_t v0_val;
	int32_t a0_val;

	v0_val = CURRENT_STATE.REGS[2];
	a0_val = CURRENT_STATE.REGS[4];
	switch( v0_val )
	{
	// print integer
	case 0x01:
		printf("%d\n", a0_val);
		NEXT_STATE.PC += 4;
		break;
	// exit program
	case 0x0A:
		RUN_FLAG = 0;
		break;
	default:
		return;
	}
}


void instr_handler_MFHI()
{
	uint32_t instr;
	int32_t HI_val;
	uint8_t rd;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	// Get values
	rd = GET_RD( instr );
	HI_val = NEXT_STATE.HI;

	// put contents of HI into rd
	NEXT_STATE.REGS[rd] = HI_val;

	// Increment program counter
	NEXT_STATE.PC += 4;
}


void instr_handler_MTHI()
{
	uint32_t instr;
	int32_t rs_val;
	uint8_t rs;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	rs = GET_RS( instr );
	rs_val = CURRENT_STATE.REGS[rs];

	// put contents of rs into HI
	NEXT_STATE.HI = rs_val;

	// Increment program counter
	NEXT_STATE.PC += 4;
}


void instr_handler_MFLO()
{
	uint32_t instr;
	int32_t LO_val;
	uint8_t rd;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	// Get values
	rd = GET_RD( instr );
	LO_val = NEXT_STATE.LO;

	// put contents of LO into rd
	NEXT_STATE.REGS[rd] = LO_val;

	// Increment program counter
	NEXT_STATE.PC += 4;
}


void instr_handler_MTLO()
{
	uint32_t instr;
	int32_t rs_val;
	uint8_t rs;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	rs = GET_RS( instr );
	rs_val = CURRENT_STATE.REGS[rs];

	// put contents of rs into LO
	NEXT_STATE.LO = rs_val;

	// Increment program counter
	NEXT_STATE.PC += 4;
}


void instr_handler_MULT()
{
	uint32_t instr;
	int32_t rs_val, rd_val;
	uint8_t rs, rd;
	int64_t result;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	// Get values
	rs = GET_RS( instr );
	rd = GET_RD( instr );
	rs_val = CURRENT_STATE.REGS[rs];
	rd_val = CURRENT_STATE.REGS[rd];

	// multiply
	result = rs_val * rd_val;

	// Save result
	NEXT_STATE.HI = ( result >> 32 );
	NEXT_STATE.LO = ( result & 0xFFFFFFFF );

	// Increment program counter
	NEXT_STATE.PC += 4;
}


void instr_handler_MULTU()
{
	uint32_t instr;
	uint32_t rs_val, rd_val;
	uint8_t rs, rd;
	uint64_t result;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	// Get values
	rs = GET_RS( instr );
	rd = GET_RD( instr );
	rs_val = CURRENT_STATE.REGS[rs];
	rd_val = CURRENT_STATE.REGS[rd];

	// multiply
	result = rs_val * rd_val;

	// Save result
	NEXT_STATE.HI = ( result >> 32 );
	NEXT_STATE.LO = ( result & 0xFFFFFFFF );

	// Increment program counter
	NEXT_STATE.PC += 4;
}


void instr_handler_DIV()
{
	uint32_t instr;
	int32_t rs_val, rt_val;
	uint8_t rs, rt;
	int32_t quotient, remainder;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	// Get values
	rs = GET_RS( instr );
	rt = GET_RT( instr );
	rs_val = CURRENT_STATE.REGS[rs];
	rt_val = CURRENT_STATE.REGS[rt];

	// divide
	quotient = rs_val / rt_val;
	remainder = rs_val % rt_val;

	// Save result
	NEXT_STATE.HI = remainder;
	NEXT_STATE.LO = quotient;

	// Increment program counter
	NEXT_STATE.PC += 4;
}


void instr_handler_DIVU()
{
	uint32_t instr;
	uint32_t rs_val, rt_val;
	uint8_t rs, rt;
	uint32_t quotient, remainder;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	// Get values
	rs = GET_RS( instr );
	rt = GET_RT( instr );
	rs_val = CURRENT_STATE.REGS[rs];
	rt_val = CURRENT_STATE.REGS[rt];

	// divide
	quotient = rs_val / rt_val;
	remainder = rs_val % rt_val;

	// Save result
	NEXT_STATE.HI = remainder;
	NEXT_STATE.LO = quotient;

	// Increment program counter
	NEXT_STATE.PC += 4;
}


void instr_handler_ADD()
{
	uint32_t instr;
	int32_t rs_val, rt_val;
	uint8_t rs, rt, rd;
	int32_t result;

	// Get instruction
	instr = mem_read_32( CURRENT_STATE.PC );

	// Get values
	rs = GET_RS( instr );
	rt = GET_RT( instr );
	rd = GET_RD( instr );
	rs_val = CURRENT_STATE.REGS[rs];
	rt_val = CURRENT_STATE.REGS[rt];

	// Add numbers
	result = rs_val + rt_val;

	// Save results
	NEXT_STATE.REGS[rd] = result;

	// Increment program counter
	NEXT_STATE.PC += 4;
}


void instr_handler_ADDU()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint8_t rd = GET_RD( instr );

	NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];

	NEXT_STATE.PC += 4;
}


void instr_handler_SUB()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint8_t rd = GET_RD( instr );

	NEXT_STATE.REGS[rd] = (int)CURRENT_STATE.REGS[rs] - (int)CURRENT_STATE.REGS[rt];

	NEXT_STATE.PC += 4;
}


void instr_handler_SUBU()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint8_t rd = GET_RD( instr );

	NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];

	NEXT_STATE.PC += 4;
}


void instr_handler_AND()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint8_t rd = GET_RD( instr );

	NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];

	NEXT_STATE.PC += 4;
}


void instr_handler_OR()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint8_t rd = GET_RD( instr );

	NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];

	NEXT_STATE.PC += 4;
}


void instr_handler_XOR()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint8_t rd = GET_RD( instr );

	NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];

	NEXT_STATE.PC += 4;
}


void instr_handler_NOR()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint8_t rd = GET_RD( instr );

	NEXT_STATE.REGS[rd] = ~( CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt] );

	NEXT_STATE.PC += 4;
}


void instr_handler_SLT()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint8_t rd = GET_RD( instr );

	NEXT_STATE.REGS[rd] = ( CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt] ) ? 1 : 0;

	NEXT_STATE.PC += 4;
}


void instr_handler_BLTZ()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint8_t rs = GET_RS( instr );
	uint16_t immed = GET_IMMED( instr );

	if( CURRENT_STATE.REGS[rs] < 0x0 )
	{
		NEXT_STATE.PC = ( CURRENT_STATE.PC + immed);
	}
	else
	{
		NEXT_STATE.PC += 4;
	}
}


void instr_handler_BGEZ()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint8_t rs = GET_RS( instr );
	uint16_t immed = GET_IMMED( instr );

	if( CURRENT_STATE.REGS[rs] >= 0x0 )
	{
		NEXT_STATE.PC = ( CURRENT_STATE.PC + immed);
	}
	else
	{
		NEXT_STATE.PC += 4;
	}
}


void instr_handler_J()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint32_t target = GET_ADDRESS( instr );
	NEXT_STATE.PC = ( CURRENT_STATE.PC & 0xF0000000 ) | ( target << 2 );
}


void instr_handler_JAL()
{
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );
	uint32_t target = GET_ADDRESS( instr );
	NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
	NEXT_STATE.PC = ( CURRENT_STATE.PC & 0xF0000000 ) | ( target << 2 );
}


//branch on equal
void instr_handler_BEQ()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	int16_t immed = GET_IMMED( instr );

	uint32_t rs_val = CURRENT_STATE.REGS[rs];
	uint32_t rt_val = CURRENT_STATE.REGS[rt];

	//if equal, branch
	if( rs_val == rt_val )
	{
		NEXT_STATE.PC = ( CURRENT_STATE.PC + immed);
	}
	else
	{
		NEXT_STATE.PC += 4;
	}

}


//branch on not equal
void instr_handler_BNE()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	int16_t immed = GET_IMMED( instr );

	uint32_t rs_val = CURRENT_STATE.REGS[rs];
	uint32_t rt_val = CURRENT_STATE.REGS[rt];

	if( rs_val != rt_val )
	{
		NEXT_STATE.PC = ( CURRENT_STATE.PC + immed);
	}
	else
	{
		NEXT_STATE.PC += 4;
	}
}


//branch on less than or equal to zero
void instr_handler_BLEZ()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	int16_t immed = GET_IMMED( instr );

	uint32_t rs_val = CURRENT_STATE.REGS[rs];

	//if less than equal to zero, branch
	if( rs_val <= 0x0 )
	{
		NEXT_STATE.PC = ( CURRENT_STATE.PC + immed);
	}
	else
	{
		NEXT_STATE.PC += 4;
	}

}


//brnach on greater than or equal to zero
void instr_handler_BGTZ()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	int16_t immed = GET_IMMED( instr );

	uint32_t rs_val = CURRENT_STATE.REGS[rs];

	//if greater than equal to zero, branch
	if( rs_val >= 0x0 )
	{
		NEXT_STATE.PC = ( CURRENT_STATE.PC + immed);
	}
	else
	{
		NEXT_STATE.PC += 4;
	}
}


//add a registers value and an immediate - signed
void instr_handler_ADDI()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	int16_t immed = GET_IMMED( instr );

	//get the value from register rs - signed
	int32_t rs_val = CURRENT_STATE.REGS[rs];

	//add the immediate value to the value in register rs and place it in rt - signed
	NEXT_STATE.REGS[rt] = rs_val + immed;

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//add a registers value and an immediate - unsigned
void instr_handler_ADDIU()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint16_t immed = GET_IMMED( instr );

	//get the value from register rs - unsigned
	uint32_t rs_val = CURRENT_STATE.REGS[rs];

	//add the immediate value to the value in register rs and place it in rt - unsigned
	NEXT_STATE.REGS[rt] = rs_val + immed;

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//set less then immediate - signed
void instr_handler_SLTI()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	int16_t immed = GET_IMMED( instr );

	//get the value from register rs - signed
	int32_t rs_val = CURRENT_STATE.REGS[rs];

	//if the value in rs is less then immediate, rt is set to 1, otherwise 0
	if( rs_val < immed )
		NEXT_STATE.REGS[rt] = 0x1;
	else
		NEXT_STATE.REGS[rt] = 0x0;

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//and immediate
void instr_handler_ANDI()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint16_t immed = GET_IMMED( instr );

	//get the value from register rs
	uint32_t rs_val = CURRENT_STATE.REGS[rs];

	//the value in rs is anded with the zer0 extended immediate and stored in rt
	NEXT_STATE.REGS[rt] = rs_val & immed;

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//or immediate
void instr_handler_ORI()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint16_t immed = GET_IMMED( instr );

	//get the value from register rs
	uint32_t rs_val = CURRENT_STATE.REGS[rs];

	//the value in rs is ored with the zer0 extended immediate and stored in rt
	NEXT_STATE.REGS[rt] = rs_val | immed;

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//exclusive or immediate
void instr_handler_XORI()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	uint16_t immed = GET_IMMED( instr );

	//get the value from register rs
	uint32_t rs_val = CURRENT_STATE.REGS[rs];

	//the value in rs is exclusive ored with the zer0 extended immediate and stored in rt
	NEXT_STATE.REGS[rt] = rs_val ^ immed;

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//load upper immediate
void instr_handler_LUI()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rt = GET_RT( instr );
	uint32_t immed = GET_IMMED( instr );

	//shift the immediate left 16 bits and 16 bits of zeros will be added to the end
	immed <<= 16;

	//assign the shifted immediate to register rt
	NEXT_STATE.REGS[rt] = immed;

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//load byte
void instr_handler_LB()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	int16_t immed = GET_IMMED( instr );

	//get the value from rs and sign extend it
	int32_t rs_val = CURRENT_STATE.REGS[rs];

	//add the immediate and value from rs to generate a virtual address
	int32_t address = rs_val + immed;

	//rt register gets 1 byte from address, bit masked by 0xFF
	NEXT_STATE.REGS[rt] = 0xFF & mem_read_32( address );

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//load halfword
void instr_handler_LH()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	int16_t immed = GET_IMMED( instr );

	//get the value from rs and sign extend it
	int32_t rs_val = CURRENT_STATE.REGS[rs];

	//add the immediate and value from rs to generate a virtual address
	int32_t address = rs_val + immed;

	//rt register gets 2 bytes (halfword) from address, bit masked by 0xFFFF
	NEXT_STATE.REGS[rt] = 0xFFFF & mem_read_32( address );

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//load word
void instr_handler_LW()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	int16_t immed = GET_IMMED( instr );

	//get the value from rs and sign extend it
	int32_t rs_val = CURRENT_STATE.REGS[rs];

	//add the immediate and value from rs to generate a virtual address
	int32_t address = rs_val + immed;

	//rt register gets 4 bytes ( 1 word) from the address
	NEXT_STATE.REGS[rt] = mem_read_32( address );

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//store byte
void instr_handler_SB()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	int16_t immed = GET_IMMED( instr );

	//get the value from rs,rt and sign extend it
	int32_t rs_val = CURRENT_STATE.REGS[rs];
	uint32_t rt_val = CURRENT_STATE.REGS[rt];

	//add the immediate and value from rs to generate a virtual address
	int32_t address = rs_val + immed;

	//get the least significant byte from rt
	uint8_t value = 0xFF & rt_val;

	//store the value to the calculated address
	mem_write_32( address, value );

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//store halfword
void instr_handler_SH()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	int16_t immed = GET_IMMED( instr );

	//get the value from rs,rt and sign extend it
	int32_t rs_val = CURRENT_STATE.REGS[rs];
	uint32_t rt_val = CURRENT_STATE.REGS[rt];

	//add the immediate and value from rs to generate a virtual address
	int32_t address = rs_val + immed;

	//get the least significant halfword from rt
	uint8_t value = 0xFFFF & rt_val;

	//store the value to the calculated address
	mem_write_32( address, value );

	//bump the program counter
	NEXT_STATE.PC += 4;
}


//store word
void instr_handler_SW()
{
	//get the instruction
	uint32_t instr = mem_read_32( CURRENT_STATE.PC );

	//get registers and immediate from instr
	uint8_t rs = GET_RS( instr );
	uint8_t rt = GET_RT( instr );
	int16_t immed = GET_IMMED( instr );

	//get the value from rs,rt and sign extend it
	int32_t rs_val = CURRENT_STATE.REGS[rs];
	uint32_t rt_val = CURRENT_STATE.REGS[rt];

	//add the immediate and value from rs to generate a virtual address
	int32_t address = rs_val + immed;

	//store the value to the calculated address
	mem_write_32( address, rt_val );

	//bump the program counter
	NEXT_STATE.PC += 4;
}
