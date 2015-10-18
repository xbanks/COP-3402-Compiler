/*
    Assignment 2
    Kody Denues
    Emre Kutsal
    Xavier Banks
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"

#define IDENT_LIMIT 11
#define NUM_LIMIT 5
#define __INPUT_FILE__ "./input.txt"
#define __CLEAN_INPUT__ "./cleaninput.txt"
#define __LEX_TABLE__ "./lexemetable.txt"
#define __TOKEN_LIST__ "./tokenlist.txt"
;
// Prototypes
void cleanInput();
void scan();
void checkPrint(char *lexeme, FILE * tokenList, FILE * lexTable);

int main(void)
{
	cleanInput();
	scan();
	return 0;
}

void cleanInput()
{
	FILE * fp, * ofp;
	fp = (FILE *) fopen(__INPUT_FILE__, "r");
	ofp = (FILE *)fopen(__CLEAN_INPUT__, "w");

	if (fp == NULL || ofp == NULL)
	{
		printf("Could not open: %s", __INPUT_FILE__);
		exit(0);
	}

	char chr, nextchr;
	//fscanf(fp, "%s", &file)
	bool ignore = false;
	while ((chr = fgetc(fp)) != EOF)
	{
		if (chr == '/' && (nextchr = fgetc(fp)) == '*') { ignore = true; }
		else if (chr == '*' && (nextchr = fgetc(fp)) == '/') { ignore = false; continue; }
		if (!ignore) { fputc(chr, ofp); }
	}
	fclose(ofp);
	fclose(fp);
}

void scan()
{
    // open file
	FILE * fp, * lexTable, * tokenList;
    fp = (FILE *) fopen(__CLEAN_INPUT__, "r");
    lexTable = (FILE *) fopen(__LEX_TABLE__, "w");
    tokenList = (FILE *) fopen(__TOKEN_LIST__, "w");

    char chr;
    char * lexeme = (char *)malloc(sizeof(char) * IDENT_LIMIT);
    int length = 0;
    fprintf(lexTable, "lexeme       token type\n");

    // read each char
    while( (chr = fgetc(fp)) != EOF )
    {
        // printf("%c", chr);
        // stop at non characters
        
        if( (chr == '.' || chr == ' ' || chr == ';' || chr == ',' || chr == '\n') 
            && strcmp(lexeme, ""))
        {
            checkPrint(lexeme, tokenList, lexTable);
            length = 0;
            free(lexeme);
            lexeme = (char *) malloc(sizeof(char) * IDENT_LIMIT);
            
            if(chr == '.' || chr == ';' || chr == ',' || chr == '(' || chr == ')')
            {
                lexeme[0] = chr;
                checkPrint(lexeme, tokenList, lexTable);
                free(lexeme);
                lexeme = (char *) malloc(sizeof(char) * IDENT_LIMIT);
            }
        }
        else
        {
            if(chr != ' ' && chr != '\n') 
            {
                lexeme[length] = chr;
                length++;
            }
        }
        
        if(length > 11)
        {
            printf("ERROR, identifier too long.\n");
            exit(0);
        }
    }
    
    fclose(lexTable);
    fclose(fp);
    fclose(tokenList);
}

// needs to have a token array, and a parsed lexeme values
//also assume 2 output files, tokenList, lexTable

void checkPrint(char *lexeme, FILE * tokenList, FILE * lexTable)
{
	int x = 1;
	int tokenVal = 0;
	for(x = 1; x < 34; x++)
	{
		if(!strcmp(lexeme, tokenArr[x]))
		{
			tokenVal = x;
			break;
		}
		
		if(x == 33)
		{
		    if(isNumber(lexeme))
    			tokenVal = 3;
    		else if(isWord(lexeme))
    		    tokenVal = 2;
		}
	}
	
	fprintf(lexTable, "%-11s  %-10d\n", lexeme, tokenVal);
	fprintf(tokenList, "%d ",tokenVal);	
	if(tokenVal == 2 || tokenVal == 3)
	{
	    if(tokenVal == 2 && !isalpha(lexeme[0]))
	    {
	        printf("\nError: \'%s\' is not a variable\n", lexeme);
	        exit(0);
	    }
	    fprintf(tokenList, "%s ", lexeme);
	}
	
}

int isNumber(char * lexeme)
{
    int i = 0;
    for(i = 0; i < strlen(lexeme); i++ )
    {
        if(!isdigit(lexeme[i]))
            return 0;
    }
    
    if(strlen(lexeme) > NUM_LIMIT)
    {    
        printf("Error: Length of number too long!\n");
        exit(0);
    }
    
    return 1;
}

int isWord(char * lexeme)
{
    int i = 0;
    for(i = 0; i < strlen(lexeme); i++ )
    {
        if(!isalpha(lexeme[i]) && !isdigit(lexeme[i]))
            return 0;
    }
    
    if(strlen(lexeme) > IDENT_LIMIT)
    {    
        printf("Error: Length of number too long!\n");
        exit(0);
    }
          
    return 1;
}
