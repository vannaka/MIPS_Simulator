#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "assembler.h"
#include "Instructions.h"

/***************************************************************/
/* main                                                        */
/***************************************************************/
int main( int argc, char *argv[] ) 
{      
    FILE* fp;
    char* instr_str = NULL;
    size_t instr_str_size = 0;
    char delim[] = {' ', '$', '\n', '\0'};
    char* token;
	
	if( argc < 2 ) 
	{
		printf( "Error: You should provide input file.\nUsage: %s <input program> \n\n",  argv[0] );
		exit(1);
	}

	/* Open program file. */
	fp = fopen( argv[1], "r" );
	if( fp == NULL )
	{
		printf( "Error: Can't open code file %s\n", argv[1] );
		exit(-1);
	}
	
	while( getline( &instr_str, &instr_str_size, fp ) != EOF )
    {        
        token = strtok( instr_str, delim );
    
        printf("\nOPCODE: %s", token);
    
    
        token = strtok( NULL, delim );
        while( token != NULL )
        {
            printf("\nOPERAND: %s", token);
            token = strtok( NULL, delim );
        }
    }
    
    printf("\n");	
    
    
	return 0;
}


void assem_operand_decode( inst_op_type_t op_type, char* szRegisterName, uint32_t* pHexInstruction ) {

	switch( op_type )
	{
	case RS:	*pHexInstruction |= ( GetRegister( szRegisterName ) << 21 );	break;
	case RT:	*pHexInstruction |= ( GetRegister( szRegisterName ) << 16 );	break;
	case RD:	*pHexInstruction |= ( GetRegister( szRegisterName ) << 11 );	break;
	case SA:	*pHexInstruction |= ( (int)strtol( szRegisterName, NULL, 16 ) << 6 );	break;
	case IMMED:	// uses same step below
	case ADDRESS:	*pHexInstruction |= ( (int)strtol( szRegisterName, NULL, 16 ) );		break;
	default:	break;
	}
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
