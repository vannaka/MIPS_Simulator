#include "pipeline.h"
#include "memory.h"
#include "Instructions.h"

void handle_pipeline()
{
    WB();
    MEM();
    EX();
    ID();
    IF();
}

void IF()
{   
	// Write new values in struct 
    IF_ID.IR = mem_read_32( CURRENT_STATE.PC );
    IF_ID.PC = CURRENT_STATE.PC;
	NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void ID()
{
	if(IF_ID.IR != 0) {
		uint8_t rs, rt;
		int16_t immed;
		
		// Pass on values
		ID_EX.PC = IF_ID.PC;
		ID_EX.IR = IF_ID.IR;
		
		// Get new values for struct
		rs = GET_RS( IF_ID.IR );
		rt = GET_RT( IF_ID.IR );
		immed = GET_IMMED( IF_ID.IR );
		
		// Write new values in struct
		ID_EX.instr_data = mips_instr_decode( IF_ID.IR );
		ID_EX.A = CURRENT_STATE.REGS[rs];
		ID_EX.B = CURRENT_STATE.REGS[rt];
		ID_EX.IMMED = (int32_t)immed;
	}
}

void EX()
{
	if(ID_EX.IR != 0) {
		// Pass on values
		EX_MEM.PC = ID_EX.PC;
		EX_MEM.IR = ID_EX.IR;
		EX_MEM.A = ID_EX.A;
		EX_MEM.B = ID_EX.B;
		EX_MEM.instr_data = ID_EX.instr_data;
	
		// Write new value into ALUOutput
		(*( EX_MEM.instr_data.funct ))( &ID_EX, &EX_MEM );
	}
}

void MEM()
{  
	if(EX_MEM.IR != 0) {
		// Pass on values	
		MEM_WB = EX_MEM;
		MEM_WB.IR = EX_MEM.IR;
		MEM_WB.A = EX_MEM.A;
		MEM_WB.B = EX_MEM.B;
		MEM_WB.IMMED = EX_MEM.IMMED;
		MEM_WB.ALUOutput = EX_MEM.ALUOutput;
		MEM_WB.Control = EX_MEM.Control;
		MEM_WB.instr_data = EX_MEM.instr_data;

		// Write new values dependent upon control type
		switch(EX_MEM.Control){
			case LOAD_TYPE: 	
				switch(MEM_WB.num_bytes){					
					case BYTE: 		MEM_WB.LMD = 0x00FF & mem_read_32(EX_MEM.ALUOutput);	break;
					case HALF_WORD: MEM_WB.LMD = 0xFFFF & mem_read_32(EX_MEM.ALUOutput);	break;
					case WORD:		MEM_WB.LMD = mem_read_32(EX_MEM.ALUOutput);				break;
					default: 		/*	Do nothing	*/										break;
				}
				break;
			case STORE_TYPE: 	
				switch(EX_MEM.num_bytes){
					case BYTE:		mem_write_32(EX_MEM.ALUOutput, 0x00FF & EX_MEM.B); 		break;
					case HALF_WORD: mem_write_32(EX_MEM.ALUOutput, 0xFFFF & EX_MEM.B); 		break;			
					case WORD: 		mem_write_32(EX_MEM.ALUOutput, EX_MEM.B); 				break;
					default: 		/*	Do nothing	*/										break;				
				}
				break;
			case REGISTER_TYPE:	/*	Do nothing	*/	break;
			default:			/*	Do nothing	*/	break;
		}
	}
}

void WB()
{
	if(MEM_WB.IR != 0) {
		// Write new values dependent upon control type
		switch(MEM_WB.Control){
			case LOAD_TYPE:			NEXT_STATE.REGS[GET_RT( MEM_WB.IR )] = MEM_WB.LMD;			break;
			case STORE_TYPE: 		/*	Do nothing	*/								 			break;
			case REGISTER_TYPE: 
				switch(MEM_WB.instr_data.type){
					case R_TYPE: 	NEXT_STATE.REGS[GET_RD( MEM_WB.IR )] = MEM_WB.ALUOutput; 	break;
					case I_TYPE: 	NEXT_STATE.REGS[GET_RT( MEM_WB.IR )] = MEM_WB.ALUOutput; 	break;
					default:		/*	Do nothing	*/					 						break;
				}
				break;
			default:				/*	Do nothing	*/	break;
		}
	}
}
