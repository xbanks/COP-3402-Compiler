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
int isWord(char * lexeme);
int isNumber(char * lexeme);
char getString(char chr, FILE * fp);
char getNumber(char chr, FILE * fp);
char getIdent(char chr, FILE * fp);

// Globals
FILE * lexTable, * tokenList;
int line_number;

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

    // gets the first character in the input file
	char chr = fgetc(fp), prevchr;
	bool ignore = false;
	
	// cleans the comments out of the code using the current and previous characters
	while ((prevchr = chr) != EOF && (chr = fgetc(fp)))
	{   
	    if(prevchr == '/' && chr == '*')
	    {
	        ignore = true;
	    }
	    else if(prevchr == '*' && chr == '/')
	    {
	        ignore = false;
	        chr = fgetc(fp);
	        continue;
	    }
	    
	    if((!ignore && prevchr != '\0') || prevchr == '\n')
	    {
	        fprintf(ofp, "%c", prevchr);
	    }
	}
	
	// close input and output files
	fclose(ofp);
	fclose(fp);
}

void scan()
{
    // open file
	FILE * fp;
    fp = (FILE *) fopen(__CLEAN_INPUT__, "r");
    lexTable = (FILE *) fopen(__LEX_TABLE__, "w");
    tokenList = (FILE *) fopen(__TOKEN_LIST__, "w");
    line_number = 1;

    char chr;
    int length = 0;
    fprintf(lexTable, "lexeme       token type\n");

    // read each char
    chr = fgetc(fp);
    while( chr != EOF )
    {
        // skips spaces
        if(!isspace(chr))
        {
            if(isalpha(chr))
            {
                chr = getString(chr, fp);
            }
            else if(isdigit(chr))
            {
                chr = getNumber(chr, fp);
            }
            else
            {
                chr = getIdent(chr, fp);
            }
        }
        else
        {
            if(chr == '\n') { line_number ++; }
            chr = fgetc(fp);
        }
//        free(lexeme);
    }
    
    fclose(lexTable);
    fclose(fp);
    fclose(tokenList);
}


/*  Gets the next full string until a non alphanumeric value is hit
    Params:
        chr - first character to be placed into the lexeme
        fp - file to read from
    Output:
        chr - the next character in the file
*/
char getString(char chr, FILE * fp)
{
//    printf("Getting String: ");
    char * lexeme = ( char * ) malloc( sizeof(char) * IDENT_LIMIT + 1 );
    lexeme[0] = chr;
    
    int i = 1;
    while((chr = fgetc(fp)) != EOF && (isalpha(chr) || isdigit(chr) || chr == '_'))
    {
        lexeme[i] = chr;
        i++;
    }
    
    checkPrint(lexeme, tokenList, lexTable);
    
    free(lexeme);
    return chr;
}

/*  Gets the full number value. This might not be an actual number, this will catch alpha
    characters and this will be caught and throw an error in checkPrint.
    Params:
        chr - first character to be placed into the lexeme
        fp - file to read from
    Output:
        chr - the next character in the file
*/
char getNumber(char chr, FILE * fp)
{
//    printf("Getting Number: ");
    char * lexeme = ( char * ) malloc( sizeof(char) * IDENT_LIMIT + 1 );
    lexeme[0] = chr;
    
    int i = 1;
    while((chr = fgetc(fp)) != EOF && (isalpha(chr) || isdigit(chr)) )
    {
        lexeme[i] = chr;
        i++;
    }
    
    checkPrint(lexeme, tokenList, lexTable);
    
    free(lexeme);
    return chr;
}

/*  checks if a lexeme is a non alphanumeric reserved word.
    Eg: an arithmetic, comparison, or syntax/assignment operator
    Params:
        lexeme - lexeme to check for in the array
    Output:
        0/1 - whether or not the lexeme was found in the token array
*/
int isNonAlphaNumReserved(char * lexeme)
{
    int i = 0;
    for(i = becomessym; i >= plussym; i--)
    {
        if(strcmp(lexeme, tokenArr[i]) == 0)
        {
            return 1;   
        }
    }
    
    return 0;
}

/*  Gets any non alpha numeric characters, and if they corrispond to a reserved operator
    then it will break. This is done because there dont need to be spaces between these
    types of operators. This combats cases like '((something))', producing invalid symbol
    errors.
    Params:
        chr - first character to be placed into the lexeme
        fp - file to read from
    Output:
        chr - the next character in the file
*/
char getIdent(char chr, FILE * fp)
{
    char * lexeme = ( char * ) malloc( sizeof(char) * IDENT_LIMIT + 1 );
    int i = 0;
    
    do{
        lexeme[i] = chr;
        i++;
        
        // breaks if the current value of the lexeme is a reserved word
        if(isNonAlphaNumReserved(lexeme)) 
        {
            chr = fgetc(fp);
            break;
        }
    }while((chr = fgetc(fp)) != EOF && !(isalpha(chr) || isdigit(chr)));
    
    
    checkPrint(lexeme, tokenList, lexTable);
    
    free(lexeme);
    return chr;
}

/*  Handles any errors in the scanning process, if errors are not found, this
    will print both the lex table, and token list files
    Params:
        lexeme - current lexeme to be printed into the files
        tokenList - pointer to the token list file
        lextable - pointer to the lex table file
*/
void checkPrint(char *lexeme, FILE * tokenList, FILE * lexTable)
{
    if(tokenList == NULL)
    {
        printf("\nError: Can't write to %s\n", __TOKEN_LIST__);
        exit(0);
    }
    
    if(lexTable == NULL)
    {
        printf("\nError: Can't write to %s\n", __LEX_TABLE__);
        exit(0);
    }

	// initialize tokenVal to nulsym
	int x;
	int tokenVal = nulsym;
	
	
	// Checks the lexeme against the token array
	for(x = identsym; x < numtokens; x++)
	{
		if(!strcmp(lexeme, tokenArr[x]))
		{
			tokenVal = x;
			break;
		}
	}
	
	// If no match has been found by the end of the loop, then the lexeme
	// has to either be a number or a string
	if(x == 34)
	{
	    if(isNumber(lexeme))
			tokenVal = numbersym;
		else if(isWord(lexeme))
		    tokenVal = identsym;
	}
	
	if(tokenVal != nulsym)
	{
	    // Prints lexeme and corrisponding token value to lex table file
	    // and just the token value to the token list file
        fprintf(lexTable, "%-11s  %-10d\n", lexeme, tokenVal);
	    fprintf(tokenList, "%d ",tokenVal);	
	    
	    // Print the lexeme to the token list if it is an identifier or a number
	    if(tokenVal == identsym || tokenVal == numbersym)
	    {
	        // Throw error if an identifier starts with a number
	        if(tokenVal == identsym && !isalpha(lexeme[0]))
	        {
	            printf("Line %d Error: \'%s\' is not a  variable\n", line_number, lexeme);
	            exit(0);
	        }
	        fprintf(tokenList, "%s ", lexeme);
	    }	
	}
	else
	{
	    printf("Line %d Error: \'%s\' is not a valid symbol\n", line_number, lexeme);
	    exit(0);
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
        printf("Line %d Error: Length of number \'%s\' too long!\n", line_number, lexeme);
        exit(0);
    }
    
    return 1;
}

int isWord(char * lexeme)
{
    int i = 0;
    for(i = 0; i < strlen(lexeme); i++ )
    {
        if(!isalpha(lexeme[i]) && !isdigit(lexeme[i]) & lexeme[i] != '_')
            return 0;
    }
    
    if(strlen(lexeme) > IDENT_LIMIT)
    {    
        printf("Line %d Error: Length of identifier \'%s\' too long!\n", line_number, lexeme);
        exit(0);
    }
          
    return 1;
}
