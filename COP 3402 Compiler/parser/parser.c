/*
   Assignment 3
   11/16/15
   Kody Denues
   Emre Kutsal
   Xavier Banks
*/

#include <stdio.h>
#include <stdlib.h>
#include <file.h>

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
            printf("0. Missing Identifier.\n");
            break;
        case 1:
            printf("1. Use = instead of :=.\n");
            break; 
        case 2:
            printf("2. = must be followed by a number.\n");
            break;
        case 3:
            printf("3. Identifier must be followed by :=.\n");
            break;
        case 4:
            printf("4. const, var, procedure must be followed by identifier.\n");
            break;
        case 5:
            printf("5. Semicolon or comma missing.\n");
            break;
        case 6:
            printf("6. Incorrect symbol after procedure declaration.\n");
            break;
        case 7:
            printf("7. Statement expected.\n");
            break;
        case 8:
            printf("8. Incorrect symbol after statement part in block.\n");
        case 9:
            printf("9. Period expected.\n");
            break;
        case 10:
            printf("10. Semicolon between statements missing.\n");
            break;
        case 11:
            printf("11. Undeclared identifier.\n");
            break;
        case 12:
            printf("12. Assignment to constant or procedure is not allowed.\n");
            break;
        case 13:
            printf("13. Assignment operator expected.\n");
            break;
        case 14:
            printf("14. call must be followed by an identifier.\n");
            break;
        case 15:
            printf("15. Call of a constant or variable is meaningless.\n");
            break;
        case 16:
            printf("16. then expected.\n");
            break;
        case 17:
            printf("17. Semicolon or } expected.\n");
            break;
        case 18:
            printf("18. do expected.\n");
            break;
        case 19:
            printf("19. Incorrect symbol following statement.\n");
            break;
        case 20:
            printf("20. Relational operator expected.\n");
            break;
        case 21:
            printf("21. Expression must not contain a procedure identifier.\n");
            break;
        case 22:
            printf("22. Right parenthesis missing.\n");
            break;
        case 23:
            printf("23. The preceding factor cannot begin with this symbol.\n");
            break;
        case 24:
            printf("24. An expression cannot begin with this symbol.\n");
            break;
        case 25:
            printf("25. This number is too large.\n");
            break;
}