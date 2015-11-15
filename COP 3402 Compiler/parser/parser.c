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

char TOKEN;
int LEVEL;

int void main(){
    getToken();
    block();
    if(TOKEN != periodsym)
    {
       error(TOKEN);
    }

    return 0;
}

void getToken()
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

void constDeclaration() //needs work in if statements
{
    do{
        getToken();
        if(TOKEN != identsym)
            error(0);
        getToken();
        if(TOKEN != eqsym)
            error(3);
        getToken();
        if(TOKEN != numbersym)
            error(2);
        enter(); //need variables inserted
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
        enter(); //need variables inserted
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
        enter(); //needs variables
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

}

void condition()
{

}

void relOP()
{

}

void expression()
{

}

void term()
{

}

int factor()
{
    int TF;

    return TF;
}

int number()
{
    int TF;

    return TF;
}

int ident()
    int TF;

    return TF;
}

int digit()
    int TF;

    return TF;
}

int letter()
    int TF;

    return TF;
}

void error(int num)
{
    switch(num)
    {
        case 0:
            printf("ERROR: Missing Identifier.\n");
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
}
