#include <stdio.h>

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
    
    STALL = 0;
}

void IF()
{   
    if( !STALL )
    {
        // Write new values in struct 
        IF_ID.IR = mem_read_32( CURRENT_STATE.PC );
        IF_ID.PC = CURRENT_STATE.PC;
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void ID()
{
	if( IF_ID.IR != 0 ) {
		uint8_t rs, rt;
		int16_t immed;
		
		// Get new values for struct
		rs = GET_RS( IF_ID.IR );
		rt = GET_RT( IF_ID.IR );
		immed = GET_IMMED( IF_ID.IR );
        
		// Write new values in struct
        ID_EX.instr_data = mips_instr_decode( IF_ID.IR );
      ID_EX.IR = 0;  
        // Check for control hazards
        STALL = checkControlHazard();
        
        //Check for data hazards
		if(!ENABLE_FORWARDING && !STALL)
        	STALL = checkDataHazard();
		else if( !STALL )
			STALL = checkForward();

        if( !STALL )
        {
            // Pass on values
            ID_EX.PC = IF_ID.PC;
            ID_EX.IR = IF_ID.IR;
            
			switch(IF_ID.FORWARDA){
            	case 0: 	ID_EX.A = CURRENT_STATE.REGS[rs];	break;
				case 1:		ID_EX.A = EX_MEM.ALUOutput;			break;
				case 2:		ID_EX.A = EX_MEM.ALUOutput2;		break;
				case 3:		ID_EX.A = MEM_WB.ALUOutput;			break;
				case 4:		ID_EX.A = MEM_WB.ALUOutput2;		break;	
				case 5:		ID_EX.A = MEM_WB.LMD;				break;							
				default:	printf("\nERROR FORWARD A");		break;
			}
			switch(IF_ID.FORWARDB){
            	case 0: 	ID_EX.B = CURRENT_STATE.REGS[rt];	break;
				case 1:		ID_EX.B = EX_MEM.ALUOutput;			break;
				case 2:		ID_EX.B = EX_MEM.ALUOutput2;		break;
				case 3:		ID_EX.B = MEM_WB.ALUOutput;			break;
				case 4:		ID_EX.B = MEM_WB.ALUOutput2;		break;	
				case 5:		ID_EX.B = MEM_WB.LMD;				break;							
				default:	printf("\nERROR FORWARD B");		break;
			}
            ID_EX.IMMED = (int32_t)immed;

			printf("\nContents A: %08x, Forward Case: %d,",ID_EX.A, IF_ID.FORWARDA);
			printf("\tContents B: %08x, Forward Case: %d,",ID_EX.B, IF_ID.FORWARDB);
        }
        else
            printf("\nSTALLED");
	}

	IF_ID.FORWARDA = 0;
	IF_ID.FORWARDB = 0;
}

void EX()
{
	if( ID_EX.IR != 0 ) {
		// Pass on values
		EX_MEM.PC = ID_EX.PC;
		EX_MEM.IR = ID_EX.IR;
		EX_MEM.A = ID_EX.A;
		EX_MEM.B = ID_EX.B;
		EX_MEM.instr_data = ID_EX.instr_data;
	
		// Write new value into ALUOutput
		(*( EX_MEM.instr_data.funct ))( &ID_EX, &EX_MEM );
        
        // Flush if branch is taken
        if( EX_MEM.flush )
        {
            memset( &IF_ID, 0, sizeof(CPU_Pipeline_Reg) );
            memset( &ID_EX, 0, sizeof(CPU_Pipeline_Reg) );
            EX_MEM.flush = 0;
        }
	}
	
	ID_EX.IR = 0;
}

void MEM()
{  
	if( EX_MEM.IR != 0 ) {
		// Pass on values	
		MEM_WB = EX_MEM;
		MEM_WB.IR = EX_MEM.IR;
		MEM_WB.A = EX_MEM.A;
		MEM_WB.B = EX_MEM.B;
		MEM_WB.IMMED = EX_MEM.IMMED;
		MEM_WB.ALUOutput = EX_MEM.ALUOutput;
		MEM_WB.ALUOutput2 = EX_MEM.ALUOutput2;
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
				}(*( EX_MEM.instr_data.funct ))( &ID_EX, &EX_MEM );
				break;
			case STORE_TYPE: 	
				switch(EX_MEM.num_bytes){
					case BYTE:		mem_write_32(EX_MEM.ALUOutput, 0x00FF & EX_MEM.B); 		break;
					case HALF_WORD: mem_write_32(EX_MEM.ALUOutput, 0xFFFF & EX_MEM.B); 		break;			
					case WORD: 		mem_write_32(EX_MEM.ALUOutput, EX_MEM.B); 				break;
					default: 		/*	Do nothing	*/										break;				
				}
				break;
			case REGISTER_TYPE:			/*	Do nothing	*/	break;
			case SPECIAL_REGISTER_TYPE:	/*	Do nothing	*/	break;
            case BRANCH_TYPE:           /*  Do nothing  */  break;
			default:					/*	Do nothing	*/	break;
		}
	}
	
	
	EX_MEM.IR = 0;
}

void WB()
{
	if( MEM_WB.IR != 0 ) {
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
			case SPECIAL_REGISTER_TYPE:
				NEXT_STATE.LO = MEM_WB.ALUOutput;
				NEXT_STATE.HI = MEM_WB.ALUOutput2;
				break;
            case BRANCH_TYPE:       /*  Do nothing  */  break;
			default:				/*	Do nothing	*/	break;
		}
	}
	
	MEM_WB.IR = 0;
   
    // Update at end of cycle
    CURRENT_STATE = NEXT_STATE;
}

uint8_t checkDataHazard()
{
    uint8_t dest;
    
    // Check for hazard with execute stage
    if( EX_MEM.instr_data.type == R_TYPE )
    {
        dest = GET_RD( EX_MEM.IR );
    }
    else if( EX_MEM.instr_data.type == I_TYPE )
    {
        dest = GET_RT( EX_MEM.IR );
    }
    else
        dest = 0;
    
    
    if ( ( dest != 0 ) && ( dest == GET_RS( IF_ID.IR ) ) )
        return 1;
    
    if ( ( dest != 0 ) && ( dest == GET_RT( IF_ID.IR ) ) )
        return 1;
    
    
    // Check for hazazrd with Memory stage
    if( MEM_WB.instr_data.type == R_TYPE )
    {
        dest = GET_RD( MEM_WB.IR );
    }
    else if( MEM_WB.instr_data.type == I_TYPE )
    {
        dest = GET_RT( MEM_WB.IR );
    }
    else
        dest = 0;
    
    if ( ( dest != 0 ) && ( dest == GET_RS( IF_ID.IR ) ) )
        return 1;
    
    if ( ( dest != 0 ) && ( dest == GET_RT( IF_ID.IR ) ) )
        return 1;
    
    return 0;
}


uint8_t checkControlHazard()
{
    uint8_t fcode = ID_EX.instr_data.funct_code;
    uint8_t opcode = ID_EX.instr_data.opcode;
    
   
    if( isBranch( opcode, fcode ) )
        return 1;
    
    return 0;
}


uint8_t checkForward()
{
    uint8_t dest;
    
	uint8_t 	bMultDivForward = ((EX_MEM.instr_data.opcode = 0x00) 
								&& (((EX_MEM.instr_data.funct_code >= 0x18) && (EX_MEM.instr_data.funct_code <= 0x1B))
								|| (EX_MEM.instr_data.funct_code == 0x11) || (EX_MEM.instr_data.funct_code == 0x13)));

	// Check for hazard with execute stage
	if(!bMultDivForward) {
		if( EX_MEM.instr_data.type == R_TYPE )
		{
		    dest = GET_RD( EX_MEM.IR );
		}
		else if( EX_MEM.instr_data.type == I_TYPE )
		{
		    dest = GET_RT( EX_MEM.IR );
		}
		else
		    dest = 0;

		// Check if destination register is same as first source		
		if ( ( dest != 0 ) && ( dest == GET_RS( IF_ID.IR ) ) ) {
			if (EX_MEM.Control == LOAD_TYPE){
				return 1;	// Stall
			}
			else{	
				IF_ID.FORWARDA = 1;		// Case of forwarding ALUOutput from execute
			}
		}

		// Check if destination register is same as second source
		if ( ( dest != 0 ) && ( dest == GET_RT( IF_ID.IR ) ) ){
			if (EX_MEM.Control == LOAD_TYPE){
				return 1;	// Stall
			}
			else{	
				IF_ID.FORWARDB = 1;	// Case of forwarding ALUOutput from execute	
			}
		}
	}
	else {
		// If ex_mem has a mult or div instruction running and we need to forward to move from high or low
		if((ID_EX.instr_data.opcode == 0x00) && (ID_EX.instr_data.funct_code == 0x10))
			IF_ID.FORWARDA = 2;	// Case of forwarding ALUOutput2 from execute
		else if((ID_EX.instr_data.opcode == 0x00) && (ID_EX.instr_data.funct_code == 0x12))	
			IF_ID.FORWARDA = 1;	// Case of forwarding ALUOutput from execute
	}
    

	bMultDivForward = ((MEM_WB.instr_data.opcode = 0x00) 
								&& (((MEM_WB.instr_data.funct_code >= 0x18) && (MEM_WB.instr_data.funct_code <= 0x1B))
								|| (MEM_WB.instr_data.funct_code == 0x11) || (MEM_WB.instr_data.funct_code == 0x13)));

    // Check for hazazrd with Memory stage
	if(!bMultDivForward) {
		if( MEM_WB.instr_data.type == R_TYPE )
		{
		    dest = GET_RD( MEM_WB.IR );
		}
		else if( MEM_WB.instr_data.type == I_TYPE )
		{
		    dest = GET_RT( MEM_WB.IR );
		}
		else
		    dest = 0;
		
		// Check if destination register is same as first source
		if ( ( dest != 0 ) && ( dest == GET_RS( IF_ID.IR ) ) ){
			if (EX_MEM.Control == LOAD_TYPE){
				IF_ID.FORWARDA = 5;	// Case of forwarding LMD from memory
			}
			else{	
				IF_ID.FORWARDA = 3;	// Case of forwarding Aluoutput from memory
			}
		}
		
		// Check if destination register is same as second source
		if ( ( dest != 0 ) && ( dest == GET_RT( IF_ID.IR ) ) ){
			if (EX_MEM.Control == LOAD_TYPE){
				IF_ID.FORWARDB = 5;	// Case of forwarding LMD from memory
			}
			else{	
				IF_ID.FORWARDB = 3;	// Case of forwarding Aluoutput from memory
			}
		}
	}
	else {
		// If MEM_WB has a mult or div instruction running and we need to forward to move from high or low
		if((ID_EX.instr_data.opcode == 0x00) && (ID_EX.instr_data.funct_code == 0x10))
			IF_ID.FORWARDA = 4;	// Case of forwarding ALUOutput2 from memory
		else if((ID_EX.instr_data.opcode == 0x00) && (ID_EX.instr_data.funct_code == 0x12))	
			IF_ID.FORWARDA = 3;	// Case of forwarding ALUOutput from memory
	}
    
    return 0;
}

















