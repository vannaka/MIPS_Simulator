#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "assembler.h"
#include "Instructions.h"

/***************************************************************/
/* main                                                        */
/***************************************************************/
int main( int argc, char *argv[] ) 
{      
    FILE* fp_in, *fp_out;
    char* instr_str = NULL;
    size_t instr_str_size = 0;
    char delim[] = {' ', ',','$', '\n', '\t', '\0'};
    char* token;
    mips_instr_t instr_info;
    uint32_t hexInstr;
    int iRet = 0, i = 0, line_num = 0;
	
    if( argc < 3 )
    {
        printf( "Error: You should provide input and output files.\nUsage: %s <Input file> <Output file> \n\n",  argv[0] );
        exit(1);
    }

    // Open input file
	fp_in = fopen( argv[1], "r" );
    if( fp_in == NULL )
    {
        printf( "\nError: Can't open input file %s\n", argv[1] );
        exit(-1);
    }

	// Open output file
	fp_out = fopen( argv[2], "w" );
	if( fp_out == NULL )
	{
		printf( "\nError: Can't create output file %s\n", argv[2] );
		exit( -1 );
	}
    
    // grab one line at a time
    while( getline( &instr_str, &instr_str_size, fp_in ) != EOF )
    {     
		line_num++;
        hexInstr = 0;
        i = 0;
        
        // get the opcode
        token = strtok( instr_str, delim );

		// throw out empty lines and comments
		if( token == NULL ) continue;
		else if( strcmp( token, "//" ) == 0 ) continue;

		// parse opcode
        instr_info = assem_decode_opcode( token );
        
        // set the op and funct codes in the instruction
        SET_OPCODE(hexInstr, instr_info.opcode);
        SET_FUNCTCODE(hexInstr, instr_info.funct_code);
        
        printf("\n%s", instr_info.name);
    
        // get the operands
        token = strtok( NULL, delim );
        while( token != NULL && i < 3 && instr_info.skeleton[i] != 0 )
        {
            iRet = assem_operand_decode( instr_info.skeleton[i], token, &hexInstr );
            
            if( iRet == -1 )
            {
                printf("\n[ERROR] Invalid operand %s on line %d\n", token, line_num );
                exit(-1);
            }
            
            printf(" %s", token);
            token = strtok( NULL, delim );
            
            i++;
        }
        
		// save instruction to file
		fprintf( fp_out, "%08x\n", hexInstr );
        printf(" \t[%08x]", hexInstr );
        
    }
    
    printf("\n");

	fclose( fp_in );
	fclose( fp_out );
    
	return 0;
}


mips_instr_t assem_decode_opcode( char* str )
{
	int i = 0x0;
	int ret = -1;
	mips_instr_t instr_info;

	//convert the name to all caps
	char *s = str;
	while( *s )
	{
		*s = toupper( (unsigned char)*s );
		s++;
	}

	//check the lookup table first
	while( i < intr_lookup_limit )
	{
		//compare the given instruction name to the name at entry i
		ret = strcmp( str, mips_instr_lookup[i].name );

		//check if the names match
		if( ret == 0 )
			return ( instr_info = mips_instr_lookup[i] );       //return instr_info structure
		else
			i++;      //bump counter
	}

	//reset counter
	i = 0;

	//check the table for opcodes of 0     
	while( i < opcode_0x00_limit )
	{
		//compare the given instruction name to the name at entry i
		ret = strcmp( str, opcode_0x00_table[i].name );

		//check if the names match
		if( ret == 0 )
			return ( instr_info = opcode_0x00_table[i] );       //return instr_info structure
		else
			i++; //bump counter
	}

	//reset counter
	i = 0;

	//check table for opcodes of 1
	while( i < opcode_0x01_limit )
	{

		//compare the given instruction name to the name at entry i
		ret = strcmp( str, opcode_0x01_table[i].name );

		//check if the names match
		if( ret == 0 )
			return ( instr_info = opcode_0x01_table[i] );       //return instr_info structure
		else
			i++;      //bump counter
	}

	//need a null return case

}



int assem_operand_decode( inst_op_type_t op_type, char* szRegisterName, uint32_t* pHexInstruction ) {

	int iTemp = 0;
	switch( op_type )
	{
	case RS:	if((iTemp = GetRegister( szRegisterName )) < 0) return -1; *pHexInstruction |= ( iTemp << 21 );	break;
	case RT:	if((iTemp = GetRegister( szRegisterName )) < 0) return -1; *pHexInstruction |= ( iTemp << 16 );	break;
	case RD:	if((iTemp = GetRegister( szRegisterName )) < 0) return -1; *pHexInstruction |= ( iTemp << 11 );	break;
	case SA:	*pHexInstruction |= ( 0x1F & ( strtol( szRegisterName, NULL, 16 ) << 6 ) );	break;
	case IMMED:	*pHexInstruction |= ( 0xFFFF & strtol( szRegisterName, NULL, 16 ) );		break;
	case ADDRESS:	*pHexInstruction |= ( 0x3FFFFFF & strtol( szRegisterName, NULL, 16 ) );		break;
	default:	break;
	}

	return 0; //Return 0 upon success
}


int GetRegister( char* szRegisterName ) {

	int iRet = -1;
	switch( szRegisterName[0] )
	{

	// $zero
	case 'z':
	case 'Z':	iRet = 0;	break;

	// $at, $a0, $a1, $a2, $a3
	case 'a':
	case 'A':
		switch( szRegisterName[1] )
		{
		case 't':
		case 'T':	iRet = 1;	break;
		case '0':	iRet = 4;	break;
		case '1':	iRet = 5;	break;
		case '2':	iRet = 6;	break;
		case '3':	iRet = 7;	break;
		default:	iRet = -1;	break;
		}
		break;

	// $v0, $v1
	case 'v':
	case 'V':
		switch( szRegisterName[1] )
		{
		case '0':	iRet = 2;	break;
		case '1':	iRet = 3;	break;
		default:	iRet = -1;	break;
		}
		break;

	// $t0, $t1, ..., $t9
	case 't':
	case 'T':
		switch( szRegisterName[1] )
		{
		case '0':	iRet = 8;	break;
		case '1':	iRet = 9;	break;
		case '2':	iRet = 10;	break;
		case '3':	iRet = 11;	break;
		case '4':	iRet = 12;	break;
		case '5':	iRet = 13;	break;
		case '6':	iRet = 14;	break;
		case '7':	iRet = 15;	break;
		case '8':	iRet = 24;	break;
		case '9':	iRet = 25;	break;
		default:	iRet = -1;	break;
		}
		break;

	// $s0, $s1, ..., $sp
	case 's':
	case 'S':
		switch( szRegisterName[1] )
		{
		case '0':	iRet = 16;	break;
		case '1':	iRet = 17;	break;
		case '2':	iRet = 18;	break;
		case '3':	iRet = 19;	break;
		case '4':	iRet = 20;	break;
		case '5':	iRet = 21;	break;
		case '6':	iRet = 22;	break;
		case '7':	iRet = 23;	break;
		case 'p':
		case 'P':	iRet = 29;	break;
		default:	iRet = -1;	break;
		}
		break;

	// $k0, $k1
	case 'k':
	case 'K':
		switch( szRegisterName[1] )
		{
		case '0':	iRet = 26;	break;
		case '1':	iRet = 27;	break;
		default:	iRet = -1;	break;
		}
		break;

	// $gp
	case 'g':
	case 'G':	iRet = 28;	break;

	// $fp
	case 'f':
	case 'F':	iRet = 30;	break;

	// $ra
	case 'r':
	case 'R':	iRet = 31;	break;

	default:	iRet = -1;	break;
	}

	return iRet;

}
