#ifndef  INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include <stdbool.h>

#define GET_OPCODE(x)		( ( x >> 26 ) & 0x3F )
#define GET_RS(x)			( ( x >> 21 ) & 0x1F )
#define GET_RT(x)			( ( x >> 16 ) & 0x1F )
#define GET_RD(x)			( ( x >> 11 ) & 0x1F )
#define GET_SA(x)			( ( x >> 6  ) & 0x1F )
#define GET_FUNCTCODE(x)	( x & 0x3F )
#define GET_IMMED(x)		( x & 0xFFFF )
#define GET_ADDRESS(x)		( x & 0x3FFFFFF )

#define intr_lookup_limit	0x2C
#define opcode_0x01_limit   0x02
#define opcode_0x00_limit   0x2B

// project types
typedef enum 
{
	INV_TYPE = 0,
	R_TYPE, 
	I_TYPE, 
	J_TYPE,
	M_TYPE
} inst_type_t;

typedef enum
{
	INV = 0,
	RS,
	RT,
	RD,
	SA,
	IMMED,
	ADDRESS
} inst_op_type_t;

typedef struct mips_instr
{
	char name[10];
	inst_type_t type;
	uint8_t opcode;
	uint8_t funct_code;
	inst_op_type_t skeleton[3];
	void(*funct)();
	struct mips_instr *subtable;
} mips_instr_t;

// Declare lookup tables
extern mips_instr_t opcode_0x00_table[];
extern mips_instr_t opcode_0x01_table[];
extern mips_instr_t mips_instr_lookup[];
extern char mips_reg_names[][5];


// Function Declarations
mips_instr_t mips_instr_decode( uint32_t instr );

// Declare instruction handlers
void instr_handler_SLL();
void instr_handler_SRL();
void instr_handler_SRA();
void instr_handler_JR();
void instr_handler_JALR();
void instr_handler_SYSCALL();
void instr_handler_MFHI();
void instr_handler_MTHI();
void instr_handler_MFLO();
void instr_handler_MTLO();
void instr_handler_MULT();
void instr_handler_MULTU();
void instr_handler_DIV();
void instr_handler_DIVU();
void instr_handler_ADD();

void instr_handler_ADDU();
void instr_handler_SUB();
void instr_handler_SUBU();
void instr_handler_AND();
void instr_handler_OR();
void instr_handler_XOR();
void instr_handler_NOR();
void instr_handler_SLT();
void instr_handler_BLTZ();
void instr_handler_BGEZ();
void instr_handler_J();
void instr_handler_JAL();

void instr_handler_BEQ();
void instr_handler_BNE();
void instr_handler_BLEZ();
void instr_handler_BGTZ();
void instr_handler_ADDI();
void instr_handler_ADDIU();
void instr_handler_SLTI();
void instr_handler_ANDI();
void instr_handler_ORI();
void instr_handler_XORI();
void instr_handler_LUI();
void instr_handler_LB();
void instr_handler_LH();
void instr_handler_LW();
void instr_handler_SB();
void instr_handler_SH();
void instr_handler_SW();

#endif // ! INSTRUCTIONS_H
