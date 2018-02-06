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
