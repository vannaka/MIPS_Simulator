#include <stdio.h>

#include "pipeline.h"
#include "memory.h"
#include "Instructions.h"
#include "mu-cache.h"
#include <string.h>

void handle_pipeline()
{
    WB();
    MEM();
    EX();    
    ID();
    IF();
    
    DATA_STALL = 0;
	CONTROL_STALL = 0;
	JUMPED = 0;
}

void IF()
{   
	if(MEM_STALL <= 0) {
		if( !DATA_STALL && !CONTROL_STALL)
		{
		    // Write new values in struct 
		    IF_ID.IR = mem_read_32( CURRENT_STATE.PC );
		    IF_ID.PC = CURRENT_STATE.PC;
			if(!JUMPED)
				NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		}
	}
}

void ID()
{
	if(MEM_STALL <= 0) {
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

		    //Check for data hazards
			if(!ENABLE_FORWARDING)
		    	DATA_STALL = checkDataHazard();
			else
				DATA_STALL = checkForward();

		    if( !DATA_STALL )
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
					case 6: 	ID_EX.A = CURRENT_STATE.REGS[2];	break;
					case 7:		ID_EX.A = CURRENT_STATE.LO;			break;
					case 8:		ID_EX.A = CURRENT_STATE.HI;			break;		
					default:	printf("\nERROR FORWARD A");		break;
				}
				switch(IF_ID.FORWARDB){
		        	case 0: 	ID_EX.B = CURRENT_STATE.REGS[rt];	break;
					case 1:		ID_EX.B = EX_MEM.ALUOutput;			break;
					case 2:		ID_EX.B = EX_MEM.ALUOutput2;		break;
					case 3:		ID_EX.B = MEM_WB.ALUOutput;			break;
					case 4:		ID_EX.B = MEM_WB.ALUOutput2;		break;	
					case 5:		ID_EX.B = MEM_WB.LMD;				break;
					case 6: 	ID_EX.B = CURRENT_STATE.REGS[4];	break;
					case 7:		ID_EX.B = CURRENT_STATE.LO;			break;
					case 8:		ID_EX.B = CURRENT_STATE.HI;			break;								
					default:	printf("\nERROR FORWARD B");		break;
				}
		        ID_EX.IMMED = (int32_t)immed;

			
				//printf("\nContents A: %08x, Forward Case: %d,",ID_EX.A, IF_ID.FORWARDA);
				//printf("\tContents B: %08x, Forward Case: %d,\n",ID_EX.B, IF_ID.FORWARDB);

	 			// Check for control hazards
		    	CONTROL_STALL = checkControlHazard();
		    }
			 
		   
		}
		else
			printf("---------flush---------\n");

		IF_ID.FORWARDA = 0;
		IF_ID.FORWARDB = 0;
		IF_ID.A = 0;
		IF_ID.B = 0;
	}
}

void EX()
{
	if(MEM_STALL <= 0) {
		if( ID_EX.IR != 0 ) {
			// Pass on values
			EX_MEM.PC = ID_EX.PC;
			EX_MEM.IR = ID_EX.IR;
			EX_MEM.A = ID_EX.A;
			EX_MEM.B = ID_EX.B;
			EX_MEM.IMMED = ID_EX.IMMED;
			EX_MEM.instr_data = ID_EX.instr_data;
	
			// Write new value into ALUOutput
			(*( EX_MEM.instr_data.funct ))( &ID_EX, &EX_MEM );
		    
		    // Flush if branch is taken
		    if( EX_MEM.flush )
		    {
		        memset( &IF_ID, 0, sizeof(CPU_Pipeline_Reg) );
		        memset( &ID_EX, 0, sizeof(CPU_Pipeline_Reg) );
		        EX_MEM.flush = 0;
				CURRENT_STATE = NEXT_STATE;
		    }
		}
	
		ID_EX.IR = 0;
	}
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
				HandleLoadCache(); 	
				break;
			case STORE_TYPE: 
				HandleStoreCache();	
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
			case LOAD_TYPE:			NEXT_STATE.REGS[GET_RT( MEM_WB.IR )] = MEM_WB.LMD;			
					//printf("\nReg: %s with %08x",mips_reg_names[GET_RT( MEM_WB.IR )], MEM_WB.LMD); 								
					//printf("\tA: %08x B: %08x IR:%08x",MEM_WB.A, MEM_WB.B, MEM_WB.IR);
					break;
			case STORE_TYPE: 		/*	Do nothing	*/								 			break;
			case REGISTER_TYPE: 
				switch(MEM_WB.instr_data.type){
					case R_TYPE: 	NEXT_STATE.REGS[GET_RD( MEM_WB.IR )] = MEM_WB.ALUOutput; 	
						//printf("\nReg: %s with %08x",mips_reg_names[GET_RD( MEM_WB.IR )], MEM_WB.ALUOutput);
						break;
					case I_TYPE: 	NEXT_STATE.REGS[GET_RT( MEM_WB.IR )] = MEM_WB.ALUOutput; 	
						//printf("\nReg: %s with %08x",mips_reg_names[GET_RT( MEM_WB.IR )], MEM_WB.ALUOutput);
						break;
					default:		/*	Do nothing	*/					 						break;
				}
				//printf("\tA: %08x B: %08x IR:%08x",MEM_WB.A, MEM_WB.B, MEM_WB.IR);
				break;
			case SPECIAL_REGISTER_TYPE:
				NEXT_STATE.LO = MEM_WB.ALUOutput;
				NEXT_STATE.HI = MEM_WB.ALUOutput2;
				//printf("\nReg: %s with %08x","LO", MEM_WB.ALUOutput);
				//printf("\nReg: %s with %08x","HI", MEM_WB.ALUOutput2);
				//printf("\tA: %08x B: %08x IR:%08x",MEM_WB.A, MEM_WB.B, MEM_WB.IR);
				break;
			case BRANCH_TYPE:
				// JALR			
				if(MEM_WB.instr_data.opcode == 0x00 && MEM_WB.instr_data.funct_code == 0x09){
					NEXT_STATE.REGS[GET_RD( MEM_WB.IR )] = MEM_WB.ALUOutput; 
					//printf("\nReg: %s with %08x",mips_reg_names[GET_RD( MEM_WB.IR )], MEM_WB.ALUOutput);	
					//printf("\tA: %08x B: %08x IR:%08x",MEM_WB.A, MEM_WB.B, MEM_WB.IR);			
				}
				// JAL
				else if(MEM_WB.instr_data.opcode == 0x03){
					NEXT_STATE.REGS[31] = MEM_WB.ALUOutput;
					//printf("\nReg: %s with %08x",mips_reg_names[31], MEM_WB.ALUOutput);
					//printf("\tA: %08x B: %08x IR:%08x",MEM_WB.A, MEM_WB.B, MEM_WB.IR);
				}
			default:				/*	Do nothing	*/	break;
		}
	}
	
	MEM_WB.IR = 0;
   
    // Update at end of cycle
    CURRENT_STATE = NEXT_STATE;
}

uint8_t CheckInCache(uint32_t address){
	return 1;
}

uint32_t GetCacheValue(uint32_t address){

}

uint32_t SetCacheValue(uint32_t address, uint32_t value){

}

void LoadCache(uint32_t address){

}

void HandleLoadCache(){

		if(MEM_STALL <= 0){
			uint8_t bHitOrMiss = CheckInCache(EX_MEM.ALUOutput);
			uint32_t WriteBuffer;
			if(bHitOrMiss){
				MEM_STALL = 0;
				WriteBuffer = GetCacheValue(EX_MEM.ALUOutput);
				switch(MEM_WB.num_bytes){					
					case BYTE: 		MEM_WB.LMD = 0x00FF & WriteBuffer;	break;
					case HALF_WORD: MEM_WB.LMD = 0xFFFF & WriteBuffer;	break;
					case WORD:		MEM_WB.LMD = WriteBuffer;			break;
					default: 		/*	Do nothing	*/										break;
				}
			}
			else{
				LoadCache(EX_MEM.ALUOutput);
				MEM_STALL = 100;
			}
		}
		else
			MEM_STALL--;
}

void HandleStoreCache(){
		if(MEM_STALL <= 0){
			uint8_t bHitOrMiss = CheckInCache(EX_MEM.ALUOutput);
			if(bHitOrMiss){
				MEM_STALL = 0;
				switch(EX_MEM.num_bytes){
					case BYTE:	
						SetCacheValue(EX_MEM.ALUOutput, 0x00FF & EX_MEM.B);	
						mem_write_32(EX_MEM.ALUOutput, 0x00FF & EX_MEM.B); 		
						break;
					case HALF_WORD: 
						SetCacheValue(EX_MEM.ALUOutput, 0xFFFF & EX_MEM.B);
						mem_write_32(EX_MEM.ALUOutput, 0xFFFF & EX_MEM.B); 													
						break;			
					case WORD: 		
						SetCacheValue(EX_MEM.ALUOutput, EX_MEM.B);						
						mem_write_32(EX_MEM.ALUOutput, EX_MEM.B); 				
						break;
					default: 		/*	Do nothing	*/										
						break;				
				}
			}
			else{
				LoadCache(EX_MEM.ALUOutput);
				MEM_STALL = 100;
			}
		}
}

uint8_t isSysCallForward()
{
	return (ID_EX.instr_data.opcode == 0x00) && (ID_EX.instr_data.funct_code == 0x0C);
}

uint8_t checkDataHazard()
{
    uint8_t dest;
	uint8_t 	bMultDivForward = ((EX_MEM.IR != 0) && (EX_MEM.instr_data.opcode == 0x00) 
								&& (((EX_MEM.instr_data.funct_code >= 0x18) && (EX_MEM.instr_data.funct_code <= 0x1B))
								|| (EX_MEM.instr_data.funct_code == 0x11) || (EX_MEM.instr_data.funct_code == 0x13)));
    uint8_t		bSysCallForward = isSysCallForward();
	
	if(bSysCallForward){
		IF_ID.FORWARDA = 6;
		IF_ID.FORWARDB = 6;
	}
	else if(ID_EX.instr_data.funct_code == 0x10){
		IF_ID.FORWARDA = 8;
	}
	else if(ID_EX.instr_data.funct_code == 0x11){
		IF_ID.FORWARDB = 7;
	}
	else if(ID_EX.instr_data.funct_code == 0x12){
		IF_ID.FORWARDA = 7;
	}
	else if(ID_EX.instr_data.funct_code == 0x13){
		IF_ID.FORWARDB = 8;
	}

	if(!bMultDivForward){
		// Check for hazard with execute stage
		if( EX_MEM.instr_data.type == R_TYPE )
		{
		    dest = GET_RD( EX_MEM.IR );
		}
		else if( EX_MEM.instr_data.type == I_TYPE )
		{
		    dest = GET_RT( EX_MEM.IR );
		}
		else if( EX_MEM.instr_data.opcode == 0x03)
			dest = 31;
		else
		    dest = 0;
		
		
		if ( ( dest != 0 ) && ( dest == (bSysCallForward ? 2 : GET_RS( IF_ID.IR ) ) ) )
		    return 1;
		
		if ( ( dest != 0 ) && ( dest == (bSysCallForward ? 4 : GET_RT( IF_ID.IR ) ) ) )
		    return 1;
    }
    else{
		// If ex_mem has a mult or div instruction running and we need to stall for high or low
		if((ID_EX.instr_data.funct_code >= 0x10) && (ID_EX.instr_data.funct_code <= 0x13))
			return 1;
	}

	bMultDivForward = ((MEM_WB.IR!=0) && (MEM_WB.instr_data.opcode == 0x00) 
								&& (((MEM_WB.instr_data.funct_code >= 0x18) && (MEM_WB.instr_data.funct_code <= 0x1B))
								|| (MEM_WB.instr_data.funct_code == 0x11) || (MEM_WB.instr_data.funct_code == 0x13)));

	if(!bMultDivForward){
		// Check for hazazrd with Memory stage
		if( MEM_WB.instr_data.type == R_TYPE )
		{
		    dest = GET_RD( MEM_WB.IR );
		}
		else if( MEM_WB.instr_data.type == I_TYPE )
		{
		    dest = GET_RT( MEM_WB.IR );
		}
		else if( MEM_WB.instr_data.opcode == 0x03)
			dest = 31;
		else
		    dest = 0;
		
		if ( ( dest != 0 ) && ( dest == (bSysCallForward ? 2 : GET_RS( IF_ID.IR ) ) ) )
		    return 1;
		
		if ( ( dest != 0 ) && ( dest == (bSysCallForward ? 4 : GET_RT( IF_ID.IR ) ) ) )
		    return 1;
    }
	else{
		// If MEM_WB has a mult or div instruction running and we need to stall from high or low
		if((ID_EX.instr_data.funct_code >= 0x10) && (ID_EX.instr_data.funct_code <= 0x13))
			return 1;
	}

    return 0;
}


uint8_t checkControlHazard()
{
  	uint8_t fcode = IF_ID.instr_data.funct_code;
    uint8_t opcode = IF_ID.instr_data.opcode;
    
   
    if( isBranch( opcode, fcode ) > 0 )
        return 1;
    
    return 0;
}


uint8_t checkForward()
{
    uint8_t dest;
    
	uint8_t		bSysCallForward = isSysCallForward();

	if(bSysCallForward){
		IF_ID.FORWARDA = 6;
		IF_ID.FORWARDB = 6;
	}
	else if(ID_EX.instr_data.funct_code == 0x10){
		IF_ID.FORWARDA = 8;
	}
	else if(ID_EX.instr_data.funct_code == 0x11){
		IF_ID.FORWARDB = 7;
	}
	else if(ID_EX.instr_data.funct_code == 0x12){
		IF_ID.FORWARDA = 7;
	}
	else if(ID_EX.instr_data.funct_code == 0x13){
		IF_ID.FORWARDB = 8;
	}


	uint8_t bMultDivForward = ((MEM_WB.IR != 0) && (MEM_WB.instr_data.opcode == 0x00) 
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
		else if( MEM_WB.instr_data.opcode == 0x03)
			dest = 31;
		else
		    dest = 0;
		
		// Check if destination register is same as first source
		if ( ( dest != 0 ) && ( dest == ( (bSysCallForward ? 2 : GET_RS( IF_ID.IR ) ) ) ) ) {
			if (MEM_WB.Control == LOAD_TYPE){
				IF_ID.FORWARDA = 5;	// Case of forwarding LMD from memory
			}
			else if(MEM_WB.Control!= STORE_TYPE){	
				IF_ID.FORWARDA = 3;	// Case of forwarding Aluoutput from memory
			}
		}
		
		// Check if destination register is same as second source
		if ( ( dest != 0 ) && ( dest == (bSysCallForward ? 4 : GET_RT( IF_ID.IR ) ) ) ){
			if (MEM_WB.Control == LOAD_TYPE){
				IF_ID.FORWARDB = 5;	// Case of forwarding LMD from memory
			}
			else if(MEM_WB.Control!= STORE_TYPE){	
				IF_ID.FORWARDB = 3;	// Case of forwarding Aluoutput from memory
			}
		}
	}
	else {
		// If MEM_WB has a mult or div instruction running and we need to forward to move from high or low

			// Move from HI
			if(ID_EX.instr_data.funct_code == 0x10){
				IF_ID.FORWARDA = 4; // Case of forwarding ALUOutput2 (HI) from memory
			}
			// Move to HI	
			else if(ID_EX.instr_data.funct_code == 0x11){
				IF_ID.FORWARDB = 3; // Case of forwarding ALUOutput (LO) from memory
			}
			// Move from LO
			else if(ID_EX.instr_data.funct_code == 0x12){
				IF_ID.FORWARDA = 3; // Case of forwarding ALUOutput (LO) from memory
			}
			// Move to LO
			else if(ID_EX.instr_data.funct_code == 0x13){
				IF_ID.FORWARDB = 4; // Case of forwarding ALUOutput2 (HI) from memory
			}

	}

	bMultDivForward = ((EX_MEM.IR != 0) && (EX_MEM.instr_data.opcode == 0x00) 
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
		else if( EX_MEM.instr_data.opcode == 0x03)
			dest = 31;
		else
		    dest = 0;

		// Check if destination register is same as first source		
		if ( ( dest != 0 ) && ( dest == ( (bSysCallForward ? 2 : GET_RS( IF_ID.IR ) ) ) ) ){
			if (EX_MEM.Control == LOAD_TYPE){
				return 1;	// Stall
			}
			else if(EX_MEM.Control!= STORE_TYPE){	
				IF_ID.FORWARDA = 1;		// Case of forwarding ALUOutput from execute
			}
		}

		// Check if destination register is same as second source
		if ( ( dest != 0 ) && ( dest == (bSysCallForward ? 4 : GET_RT( IF_ID.IR ) ) ) ){
			if (EX_MEM.Control == LOAD_TYPE){
				return 1;	// Stall
			}
			else if(EX_MEM.Control!= STORE_TYPE){	
				IF_ID.FORWARDB = 1;	// Case of forwarding ALUOutput from execute	
			}
		}
	}
	else {
		// If ex_mem has a mult or div instruction running and we need to forward to move from high or low
			// Move from HI
			if(ID_EX.instr_data.funct_code == 0x10){
				IF_ID.FORWARDA = 2; // Case of forwarding ALUOutput2 (HI) from execute
			}
			// Move to HI	
			else if(ID_EX.instr_data.funct_code == 0x11){
				IF_ID.FORWARDB = 1; // Case of forwarding ALUOutput (LO) from execute
			}
			// Move from LO
			else if(ID_EX.instr_data.funct_code == 0x12){
				IF_ID.FORWARDA = 1; // Case of forwarding ALUOutput (LO) from execute
			}
			// Move to LO
			else if(ID_EX.instr_data.funct_code == 0x13){
				IF_ID.FORWARDB = 2; // Case of forwarding ALUOutput2 (HI) from execute
			}
	}
    


    
    return 0;
}

















