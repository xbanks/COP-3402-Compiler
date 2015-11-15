/*
   Assignment 3
   11/16/15
   Kody Denues
   Emre Kutsal
   Xavier Banks
*/

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

#define __TOKEN_LIST__ "./tokenlist.txt"
#define __SYMBOL_TABLE__ "./symboltable.txt"
#define IDENT_LIMIT 11
#define NUM_LIMIT 5

void getToken();
char * getIdent();
void enter(symbol s);
void block();
void constDeclaration();
void varDeclaration();
void procDelcaration();
void statement();
void condition();
void expression();
void term();
void factor();
void error(int num);

char TOKEN;
int LEVEL;
FILE * tokenFP;
FILE * symboltableFP;

int main(){
    tokenFP = (FILE *) fopen(__TOKEN_LIST__, "r");
    symboltableFP = (FILE *) fopen(__SYMBOL_TABLE__, "w+");

    if(tokenFP == NULL){
        printf("%s\n", "Token list not found");
        return 0;
    }

    if(symboltableFP == NULL){
        printf("%s\n", "symboltableFP could not be created");
        return 0;
    }

    getToken();
    block();
    if(TOKEN != periodsym)
    {
       error(TOKEN);
    }


    fclose(tokenFP);
    fclose(symboltableFP);
    return 0;
}

void getToken()
{
    //pull next token from input file tokenlist.
    if(fscanf(tokenFP, "%s", TOKEN)){
        printf("TOKEN: %s\n", TOKEN);
    }
}

char * getIdent()
{
    char ident[IDENT_LIMIT];

    if( (fscanf(tokenFP, "%s", ident)) ){
        printf("IDENT: %s\n", TOKEN);
    }
}

void enter(symbol s)
{

}

void block()
{
    if(TOKEN == constsym)
        constDeclaration();
    if(TOKEN == varsym)
        varDeclaration();
    if(TOKEN == procsym)
        procDelcaration();
    statement();
}

void constDeclaration()
{
    do{
        getToken();
        if(TOKEN != identsym)
            error(0);
        getToken();
        if(TOKEN != eqsym)
            error(26);
        getToken();
        if(TOKEN != numbersym)
            error(2);
        // enter(); //need variables inserted  (constant, ident, number)
        getToken();
    }while(TOKEN != commasym);
    if(TOKEN == semicolonsym)
        error(17);
    getToken();
}

void varDeclaration()
{
    do{
        getToken();
        if(TOKEN != identsym)
            error(0);
        getToken();
        // enter(); //need variables inserted (variable, ident, level)
    }while(TOKEN != commasym);
    if(TOKEN == semicolonsym)
        error(17);
    getToken();
}

void procDelcaration()
{
    while(TOKEN == procsym)
    {
        getToken();
        if(TOKEN != identsym)
            error(0);
        // enter(); //needs variables inserted (proocedure, ident)
        getToken();
        if(TOKEN != semicolonsym)
            error(17);
        getToken();
        block(LEVEL+1);
        if(TOKEN != semicolonsym)
            error(17);
        getToken();
    }
}

void statement()
{
    if(TOKEN == identsym)
    {
        getToken();
        if(TOKEN != becomessym)
            error(3);
        getToken();
        expression();
    }
    else if(TOKEN == callsym)
    {
        getToken();
        if(TOKEN != identsym)
            error(14);
        getToken();
    }
    else if(TOKEN == beginsym)
    {
        getToken();
        statement();
        while(TOKEN == semicolonsym)
        {
            getToken();
            statement();
        }
        if(TOKEN != endsym)
            error(27);
        getToken();
    }
    else if(TOKEN == ifsym)
    {
        getToken();
        condition();
        if(TOKEN != thensym)
            error(16);
        getToken();
        statement();
    }
    else if(TOKEN == whilesym)
    {
        getToken();
        condition;
        if(TOKEN != dosym)
            error(18);
        getToken();
        statement();
    }
}

void condition()
{
    if(TOKEN == oddsym)
    {
        getToken();
        expression();
    }
    else
    {
        expression();
        if(relOP() == 0)
            error(20);
        getToken();
        expression();
    }
}

int relOP()
{
    if(TOKEN ==  eqsym || TOKEN == neqsym || TOKEN == lessym || TOKEN == leqsym || TOKEN == gtrsym || geqsym)
        return 1;
    return 0;
}

void expression()
{
    if(TOKEN == plussym)
        getToken();
    term();
    while(TOKEN == plussym)
    {
        getToken();
        term();
    }
}

void term()
{
    factor();
    while(TOKEN == multsym)
    {
        getToken();
        factor();
    }
}

void factor()
{
    if(TOKEN == identsym)
        getToken();
    else if(TOKEN ==numbersym)
        getToken();
    else if(TOKEN == lparentsym)
    {
        getToken();
        expression();
        if(TOKEN != rparentsym)
            error(22);
        getToken();
    }
    else
        error(28);
}

void error(int num)
{
    switch(num)
    {
        case 0:
            printf("ERROR: Missing procedure declaration.\n");
            break;
        case 1:
            printf("ERROR: Use = instead of :=.\n");
            break;
        case 2:
            printf("ERROR: = must be followed by a number.\n");
            break;
        case 3:
            printf("ERROR: Identifier must be followed by :=.\n");
            break;
        case 4:
            printf("ERROR: const, var, procedure must be followed by identifier.\n");
            break;
        case 5:
            printf("ERROR: Semicolon or comma missing.\n");
            break;
        case 6:
            printf("ERROR: Incorrect symbol after procedure declaration.\n");
            break;
        case 7:
            printf("ERROR: Statement expected.\n");
            break;
        case 8:
            printf("ERROR: Incorrect symbol after statement part in block.\n");
            break;
        case 9:
            printf("ERROR: Period expected.\n");
            break;
        case 10:
            printf("ERROR: Semicolon between statements missing.\n");
            break;
        case 11:
            printf("ERROR: Undeclared identifier.\n");
            break;
        case 12:
            printf("ERROR: Assignment to constant or procedure is not allowed.\n");
            break;
        case 13:
            printf("ERROR: Assignment operator expected.\n");
            break;
        case 14:
            printf("ERROR: call must be followed by an identifier.\n");
            break;
        case 15:
            printf("ERROR: Call of a constant or variable is meaningless.\n");
            break;
        case 16:
            printf("ERROR: then expected.\n");
            break;
        case 17:
            printf("ERROR: Semicolon or } expected.\n");
            break;
        case 18:
            printf("ERROR: do expected.\n");
            break;
        case 19:
            printf("ERROR: Incorrect symbol following statement.\n");
            break;
        case 20:
            printf("ERROR: Relational operator expected.\n");
            break;
        case 21:
            printf("ERROR: Expression must not contain a procedure identifier.\n");
            break;
        case 22:
            printf("ERROR: Right parenthesis missing.\n");
            break;
        case 23:
            printf("ERROR: The preceding factor cannot begin with this symbol.\n");
            break;
        case 24:
            printf("ERROR: An expression cannot begin with this symbol.\n");
            break;
        case 25:
            printf("ERROR: This number is too large.\n");
            break;
        case 26:
            printf("ERROR: Identifier should be followed by number.\n");
            break;
        case 27:
            printf("ERROR: begin must be closed with end.\n");
            break;
        case 28:
            printf("ERROR: identifier, (, or number expected.\n");
            break;
    }
    exit(0);
}
