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
    IF_ID.IR = mem_read_32( CURRENT_STATE.PC );
    
    IF_ID.PC = CURRENT_STATE.PC + 4;
}

void ID()
{
    uint8_t rs, rt;
    int16_t immed;
    
    ID_EX.IR = IF_ID.IR;
    ID_EX.instr_data = mips_instr_decode( ID_EX.IR );
    
    rs = GET_RS( ID_EX.IR );
    rt = GET_RT( ID_EX.IR );
    immed = GET_IMMED( ID_EX.IR );
    
    ID_EX.A = rs;
    ID_EX.B = rt;
    ID_EX.IMMED = (int32_t)immed;
}

void EX()
{
    *( ID_EX.instr_data.funct )( &ID_EX, &EX_MEM );
}

void MEM()
{   
        
}