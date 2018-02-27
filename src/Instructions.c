#include <stdio.h>
#include <stdint.h>

#include "Instructions.h"
#include "mu-mips.h"
#include "memory.h"
#include "pipeline.h"
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


void instr_handler_SLL( CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM )
{
    uint8_t sa;
    
    sa = ( (*EX_MEM).IMMED >> 6 ) & 0x1F;
    
    // Shift Left Logical Register Contents A by sa bits
	(*EX_MEM).ALUOutput = (*ID_EX).A << sa;
	(*EX_MEM).Control = REGISTER_TYPE;
}

void instr_handler_SRL(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{  
    uint8_t sa;
    
    sa = ( (*EX_MEM).IMMED >> 6 ) & 0x1F;
    
    // Shift Right Logical Register Contents A by sa bits
	(*EX_MEM).ALUOutput = (*ID_EX).A >> sa;
	(*EX_MEM).Control = REGISTER_TYPE;
}


void instr_handler_SRA(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
    uint8_t sa;
    
    sa = ( (*EX_MEM).IMMED >> 6 ) & 0x1F;
    
    // Shift Right Logical Register Contents A by sa bits
	(*EX_MEM).ALUOutput = (int32_t)(*ID_EX).A >> sa;
	(*EX_MEM).Control = REGISTER_TYPE;
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
	switch( v0_val ) {
		case 0x01:	printf("%d\n", a0_val);		break;	// print integer
		case 0x0A:	RUN_FLAG = 0;				break;	// exit program
		default:								return;
	}
}


//move from HI, to rd
void instr_handler_MFHI(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	//set ALUOutput to the HI value
	(*EX_MEM).ALUOutput = CURRENT_STATE.HI;
	//set type as register
	(*EX_MEM).Control = REGISTER_TYPE;
    
	//WB will move ALUOutput to rd
}

//move to HI, from rs
void instr_handler_MTHI(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	//set ALUOutput2 to rs
	(*EX_MEM).ALUOutput2 = (int32_t)((int32_t)(*ID_EX).A);
	//set ALUOutput to current low state to not get overwritten
	(*EX_MEM).ALUOutput = CURRENT_STATE.LO;
	//set type as special register
	(*EX_MEM).Control = SPECIAL_REGISTER_TYPE;
    
	//WB will move ALUOutput2 to HI
	//WB will move ALUOutput  to LO
}

//move from LO, to rd
void instr_handler_MFLO(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	//set ALUOutput to the LO value
	(*EX_MEM).ALUOutput = CURRENT_STATE.LO;
	//set type as register
	(*EX_MEM).Control = REGISTER_TYPE;
    
	//WB will move ALUOutput to rd
}

//move to LO, from rs
void instr_handler_MTLO(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	//set ALUOutput to rs
	(*EX_MEM).ALUOutput = (int32_t)((int32_t)(*ID_EX).A);
	//set ALUOutput2 to current high state to not get overwritten
	(*EX_MEM).ALUOutput2 = CURRENT_STATE.HI;
	//set type as special register
	(*EX_MEM).Control = SPECIAL_REGISTER_TYPE;
	
	//WB will move ALUOutput  to LO
    //WB will move ALUOutput2 to HI
}


void instr_handler_MULT(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{

	// Multiply Register Contents A with Register Contents B - signed
	int64_t Product = (int32_t)(*ID_EX).A * (int32_t)(*ID_EX).B;
	(*EX_MEM).ALUOutput 	= (Product & 0xFFFFFFFF);	// Low state
	(*EX_MEM).ALUOutput2 	= (Product >> 32);			// High state
	(*EX_MEM).Control = SPECIAL_REGISTER_TYPE;
}


void instr_handler_MULTU(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Multiply Register Contents A with Register Contents B - unsigned
	uint64_t Product = (*ID_EX).A * (*ID_EX).B;
	(*EX_MEM).ALUOutput 	= (Product & 0xFFFFFFFF);	// Low state
	(*EX_MEM).ALUOutput2 	= (Product >> 32);			// High state
	(*EX_MEM).Control = SPECIAL_REGISTER_TYPE;
}


void instr_handler_DIV(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Divide Register Contents A by Register Contents B - signed
	(*EX_MEM).ALUOutput 	= (int32_t)((int32_t)(*ID_EX).A / (int32_t)(*ID_EX).B);	// Low state
	(*EX_MEM).ALUOutput2 	= (int32_t)((int32_t)(*ID_EX).A % (int32_t)(*ID_EX).B);	// High state
	(*EX_MEM).Control = SPECIAL_REGISTER_TYPE;
}


void instr_handler_DIVU(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Divide Register Contents A by Register Contents B - unsigned
	(*EX_MEM).ALUOutput 	= (*ID_EX).A / (*ID_EX).B;	// Low state
	(*EX_MEM).ALUOutput2 	= (*ID_EX).A % (*ID_EX).B;	// High state
	(*EX_MEM).Control = SPECIAL_REGISTER_TYPE;
}


void instr_handler_ADD(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Add Register Contents A with Register Contents B - signed
	(*EX_MEM).ALUOutput = (int32_t)((int32_t)(*ID_EX).A + (int32_t)(*ID_EX).B);
	(*EX_MEM).Control = REGISTER_TYPE;
}


void instr_handler_ADDU(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Add Register Contents A with Register Contents B - unsigned
	(*EX_MEM).ALUOutput = (*ID_EX).A + (*ID_EX).B;
	(*EX_MEM).Control = REGISTER_TYPE;
}


void instr_handler_SUB(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Subtract Register Contents A with Register Contents B - signed
	(*EX_MEM).ALUOutput = (int32_t)((int32_t)(*ID_EX).A - (int32_t)(*ID_EX).B);
	(*EX_MEM).Control = REGISTER_TYPE;
}


void instr_handler_SUBU(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Subtract Register Contents A with Register Contents B - unsigned
	(*EX_MEM).ALUOutput = (*ID_EX).A - (*ID_EX).B;
	(*EX_MEM).Control = REGISTER_TYPE;
}


void instr_handler_AND(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// AND Register Contents A with Register Contents B
	(*EX_MEM).ALUOutput = (*ID_EX).A & (*ID_EX).B;
	(*EX_MEM).Control = REGISTER_TYPE;
}


void instr_handler_OR(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// OR Register Contents A with Register Contents B
	(*EX_MEM).ALUOutput = (*ID_EX).A | (*ID_EX).B;
	(*EX_MEM).Control = REGISTER_TYPE;
}


void instr_handler_XOR(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// XOR Register Contents A with Register Contents B
	(*EX_MEM).ALUOutput = (*ID_EX).A ^ (*ID_EX).B;
	(*EX_MEM).Control = REGISTER_TYPE;
}


void instr_handler_NOR(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// NOR Register Contents A with Register Contents B
	(*EX_MEM).ALUOutput = ~((*ID_EX).A | (*ID_EX).B);
	(*EX_MEM).Control = REGISTER_TYPE;
}


void instr_handler_SLT(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Check if Register Contents A is less than Register Contents B
	(*EX_MEM).ALUOutput = ((*ID_EX).A < (*ID_EX).B) ? 1 : 0;
	(*EX_MEM).Control = REGISTER_TYPE;
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
void instr_handler_ADDI(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Add Register Contents A with immediate - signed
	(*EX_MEM).ALUOutput = (int32_t)((int32_t)(*ID_EX).A + (int32_t)(*ID_EX).IMMED);
	(*EX_MEM).Control = REGISTER_TYPE;
}


//add a registers value and an immediate - unsigned
void instr_handler_ADDIU(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Add Register Contents A with immediate - unsigned
	(*EX_MEM).ALUOutput = (*ID_EX).A + (*ID_EX).IMMED;
	(*EX_MEM).Control = REGISTER_TYPE;

}


//set less then immediate - signed
void instr_handler_SLTI(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Set output to 1 if Register Contents A is less than immediate
	(*EX_MEM).ALUOutput = ((*ID_EX).A < (*ID_EX).IMMED) ? 0x1 : 0x0;
	(*EX_MEM).Control = REGISTER_TYPE;
}


//and immediate
void instr_handler_ANDI(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// AND Register Contents A with immediate
	(*EX_MEM).ALUOutput = (*ID_EX).A & (*ID_EX).IMMED;
	(*EX_MEM).Control = REGISTER_TYPE;
}


//or immediate
void instr_handler_ORI(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// OR Register Contents A with immediate
	(*EX_MEM).ALUOutput = (*ID_EX).A | (*ID_EX).IMMED;
	(*EX_MEM).Control = REGISTER_TYPE;
}


//exclusive or immediate
void instr_handler_XORI(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// XOR Register Contents A with immediate
	(*EX_MEM).ALUOutput = (*ID_EX).A ^ (*ID_EX).IMMED;
	(*EX_MEM).Control = REGISTER_TYPE;

}


//load upper immediate
void instr_handler_LUI(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Shift immediate 16 bits
	(*EX_MEM).ALUOutput = (*ID_EX).IMMED << 16;
	(*EX_MEM).Control = REGISTER_TYPE;
	
}


//load byte
void instr_handler_LB(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Set address to Register Contents A + immediate
	(*EX_MEM).ALUOutput = (*ID_EX).A + (*ID_EX).IMMED;
	(*EX_MEM).Control = LOAD_TYPE;
	(*EX_MEM).num_bytes = BYTE;
}


//load halfword
void instr_handler_LH(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Set address to Register Contents A + immediate
	(*EX_MEM).ALUOutput = (*ID_EX).A + (*ID_EX).IMMED;
	(*EX_MEM).Control = LOAD_TYPE;
	(*EX_MEM).num_bytes = HALF_WORD;
}


//load word
void instr_handler_LW(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Set address to Register Contents A + immediate
	(*EX_MEM).ALUOutput = (*ID_EX).A + (*ID_EX).IMMED;
	(*EX_MEM).Control = LOAD_TYPE;
	(*EX_MEM).num_bytes = WORD;
}


//store byte
void instr_handler_SB(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Set address to Register Contents A + immediate
	(*EX_MEM).ALUOutput = (*ID_EX).A + (*ID_EX).IMMED;
	(*EX_MEM).Control = STORE_TYPE;
	(*EX_MEM).num_bytes = BYTE;
}


//store halfword
void instr_handler_SH(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Set address to Register Contents A + immediate
	(*EX_MEM).ALUOutput = (*ID_EX).A + (*ID_EX).IMMED;
	(*EX_MEM).Control = STORE_TYPE;
	(*EX_MEM).num_bytes = HALF_WORD;
}


//store word
void instr_handler_SW(CPU_Pipeline_Reg* ID_EX, CPU_Pipeline_Reg* EX_MEM)
{
	// Set address to Register Contents A + immediate
	(*EX_MEM).ALUOutput = (*ID_EX).A + (*ID_EX).IMMED;
	(*EX_MEM).Control = STORE_TYPE;
	(*EX_MEM).num_bytes = WORD;
}
