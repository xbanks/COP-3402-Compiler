/*
   Assignment 3
   11/16/15
   COP 3402

   Kody Denues
   Emre Kutsal
   Xavier Banks
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define __TOKEN_LIST__ "./tokenlist.txt"
#define __SYMBOL_TABLE__ "./symboltable.txt"
#define __MCODE__ "./mcode.txt"
#define IDENT_LIMIT 12
#define NUM_LIMIT 5
#define MAX_CODE_LENGTH 500

void getToken();
char * getIdent();
void enter(int kind, char *name, int val, int level, int addr);
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
void symTablePrint();
void emit(int op, int l, int m);
void codeGenPrint();

int TOKEN;
int LEVEL;
int PROC_ADDRESS;   
int ADDRESS;
FILE * tokenFP;
FILE * symboltableFP;
int symLength;
int codeLength;

Instruction code[MAX_CODE_LENGTH];

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

int main(){
    tokenFP = (FILE *) fopen(__TOKEN_LIST__, "r");
    symboltableFP = (FILE *) fopen(__SYMBOL_TABLE__, "w+");

    LEVEL = 0;
    ADDRESS = 4;
    PROC_ADDRESS = 0;
    symLength = 0;
    codeLength = 0;

    if(tokenFP == NULL){
        printf("%s\n", "Token list not found");
        return 0;
    }

    if(symboltableFP == NULL){
        printf("%s\n", "symboltableFP could not be created");
        return 0;
    }


    // emit(JMP, 0, 0);
    getToken();
    block();
    if(TOKEN != periodsym)
    {
       error(9);
    }

    emit(SIO3, 0, 3);
    symTablePrint();
    codeGenPrint();
    fclose(tokenFP);
    fclose(symboltableFP);
    return 0;
}

void getToken()
{
    //pull next token from input file tokenlist.
    printf("in getToken\n");
    if(fscanf(tokenFP, "%d", &TOKEN) != EOF){
        printf("TOKEN: %d\n", TOKEN);
    }
    else{
        printf("nothing in tokenFP\n");
    }
}

char * getIdent()
{
    char * ident = (char *)malloc(sizeof(char) * IDENT_LIMIT + 1);

    if( (fscanf(tokenFP, "%s", ident)) ){
        printf("IDENT: %s\n", ident);
        return ident;
    }

    printf("in getIdent EOF");
    return NULL;
}

int getNumber(){
    int num;

    if(fscanf(tokenFP, "%d", &num) != EOF){
        printf("NUMBER:%d\n", num);
        return num;
    }

    printf("in getNumber EOF\n");
    return NULL;
}

/* For constants, store kind, name and val
 For variables, store kind, name, L and M
 For procedures, store kind, name, L and M */
void enter(int kind, char * name, int val, int level, int addr)
{
    symLength++;
    symbol s;

    s.kind = kind;
    strcpy(s.name, name);
    s.val = val;
    s.level = level;
    s.addr = addr;

    symbol_table[symLength] = s;
}

void symTablePrint()
{
    int i;

    fprintf(symboltableFP, "Name\tType \tLevel\tValue\n");
    for(i = 1; i <= symLength; i++)
    {
        fprintf(symboltableFP, "%s\t", symbol_table[i].name);
        if(symbol_table[i].kind == 1)
            fprintf(symboltableFP, "const\t%5d\t%5d\n",symbol_table[i].level, symbol_table[i].val);
        else if(symbol_table[i].kind == 2)
            fprintf(symboltableFP, "var\t%5d\t%5d\n",symbol_table[i].level, symbol_table[i].addr);
        else
            fprintf(symboltableFP, "proc\t%5d\t%5d\n",symbol_table[i].level, symbol_table[i].addr);
    }
}

void codeGenPrint(){
    FILE * mcodePtr = fopen(__MCODE__, "w+");

    int i;
    for (i = 0; i < codeLength; ++i)
    {
        fprintf(mcodePtr, "%d %d %d\n", code[i].OP, code[i].L, code[i].M);
    }

    fclose(mcodePtr);
}

void block()
{
    printf("BLOCK\n");
    if(TOKEN == constsym){
        constDeclaration();
    }
    if(TOKEN == varsym){
        varDeclaration();
    }
    if(TOKEN == procsym){
        procDelcaration();
    }
    statement();
}

void constDeclaration()
{
    printf("CONST\n");
    do{
        getToken();
        if(TOKEN != identsym){
            error(0);
        }
        char * identifier = getIdent();
        getToken();
        if(TOKEN != eqsym){
            error(26);
        }
        getToken();
        if(TOKEN != numbersym){
            error(2);
        }
        int number = getNumber();
        enter(1, identifier, number, LEVEL, ADDRESS);

        emit(LIT, 0, number);
        emit(STO, LEVEL, ADDRESS);

        getToken();
    }while(TOKEN == commasym);
    if(TOKEN != semicolonsym){
        error(17);
    }
    getToken();
}

void varDeclaration()
{
    printf("VAR\n");
    do{
        getToken();
        if(TOKEN != identsym){
            error(0);
        }
        char * identifier = getIdent();
        getToken();
        enter(2, identifier, 0, LEVEL, ADDRESS);

        emit(LIT, 0, 0);
        emit(STO, LEVEL, ADDRESS);

        ADDRESS++;
    }while(TOKEN == commasym);
    if(TOKEN != semicolonsym){
        error(17);
    }
    getToken();
}

void procDelcaration()
{
    printf("PROC\n");
    while(TOKEN == procsym)
    {
        getToken();
        if(TOKEN != identsym){
            error(0);
        }
        char * identifier = getIdent();
        PROC_ADDRESS++;
        enter(3, identifier, 0, LEVEL, PROC_ADDRESS);
        getToken();
        if(TOKEN != semicolonsym){
            error(17);
        }
        getToken();
        LEVEL = LEVEL+1;
        ADDRESS = 4;

        int tempCodePosition = codeLength;
        emit(JMP, 0, 0);
        emit(INC, 0, 6);
        printf("tempCodePosition: %d", tempCodePosition);
        block();
        emit(OPR, 0, 0);
        printf("NEW codeLength: %d", codeLength);
        code[tempCodePosition].M = codeLength;
        LEVEL = LEVEL-1;
        if(TOKEN != semicolonsym){
            error(17);
        }
        getToken();
    }
}

int usableSymbol(char * name, int level)
{
    int i = 0;
    printf("ST: %s\n", name);

    for(i = 0; i <= symLength; i++){
        if(strcmp(symbol_table[i].name, name) == 0 && level >= symbol_table[i].level)
        {
            printf("%s %s match\n", name, symbol_table[i].name);
            return 2;
        }
    }

    return -1;
}

int symbolType(char * name, int level){
    int i = 0;

    for(i = 0; i <= symLength; i++){
        if(strcmp(symbol_table[i].name, name) == 0 && level >= symbol_table[i].level)
        {
            printf("%s %s match\n", name, symbol_table[i].name);
            return symbol_table[i].kind;
        }
    }

    return -1;
}

int levelByName(char * name){
    int i;
    for(i=1; i<=symLength; i++){
        if(strcmp(symbol_table[i].name, name) == 0){
            return LEVEL - symbol_table[i].level;
        }
    }
}

int valByName(char * name){
    int i;
    for(i=1; i<=symLength; i++){
        if(strcmp(symbol_table[i].name, name) == 0){
            return symbol_table[i].val;
        }
    }
}

int addressByName(char * name){
    int i;
    for(i=1; i<=symLength; i++){
        if(strcmp(symbol_table[i].name, name) == 0){
            return symbol_table[i].addr;
        }
    }
}

void statement()
{
    printf("STATEMENT\n");
    if(TOKEN == identsym)
    {
        char * ident = getIdent();
        getToken();
        if(TOKEN != becomessym){
            error(3);
        }

        int scoped = usableSymbol(ident, LEVEL);
        if(scoped == -1){
            printf("Undeclared\n"); // Check for scope!
            exit(0);
        }

        if (symbolType(ident, LEVEL) != 2)
        {
            printf("Type cast ERROR\n"); // add these to the master error list
            exit(0);
        }

        getToken();
        expression();

        emit(STO, levelByName(ident), addressByName(ident));
    }
    else if(TOKEN == callsym)
    {
        getToken();
        if(TOKEN != identsym)
            error(14);

        char * ident = getIdent();

        emit(CAL, levelByName(ident), addressByName(ident));
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

        int tempCodePosition = codeLength;
        emit(JMP, 0, 0);
        getToken();
        statement();
        if(TOKEN == elsesym){
            int elseCodePos = codeLength;
            emit(JMP, 0, 0);
            code[tempCodePosition].M = codeLength;
            getToken();
            statement();
            code[elseCodePos].M = codeLength;
        }
    }
    else if(TOKEN == whilesym)
    {
        getToken();
        int conditionCodePos = codeLength;
        condition();
        if(TOKEN != dosym)
            error(18);
        getToken();
        int whileCodePos = codeLength;
        emit(JPC, 0, 0);
        statement();

        emit(JMP, 0, conditionCodePos);
        code[whileCodePos].M = codeLength;
    }
}

void condition()
{
    printf("CONDITION\n");
    if(TOKEN == oddsym)
    {
        getToken();
        expression();
        emit(OPR, 0, ODD);
    }
    else
    {
        expression();
        relOP();
            
        getToken();
        expression();
    }
}

int relOP()
{
    printf("IN RELOP\n");
    switch(TOKEN){
        case eqsym:
            // emit eql op
            emit(OPR, 0, EQL);
            break;
        case neqsym:
            emit(OPR, 0, NEQ);
            break;
        case leqsym:
            emit(OPR, 0, LEQ);
            break;
        case geqsym:
            emit(OPR, 0, GEQ);
            break;
        case gtrsym:
            emit(OPR, 0, GTR);
            break;
        case lessym:
            emit(OPR, 0, LSS);
            break;
        default:
            error(20);
            break;
    }
}

void expression()
{
    printf("EXPRESSION\n");
    int sign = 1;
    if(TOKEN == plussym || TOKEN == minussym){
        int sign = ( ( TOKEN == plussym ) ? 1 : -1);
        getToken();
    }
    term();
    emit(LIT, 0, sign);
    emit(OPR, 0, MUL);
    while(TOKEN == plussym || TOKEN == minussym)
    {
        getToken();
        term();
        if(TOKEN == plussym){
            emit(OPR, 0, ADD);
        }
        else{
            emit(OPR, 0, SUB);
        }
    }
}

void term()
{
    printf("TERM\n");
    factor();
    while(TOKEN == multsym || TOKEN == slashsym)
    {
        getToken();
        factor();

        if(TOKEN == multsym){
            emit(OPR, 0, MUL);
        }
        else{
            emit(OPR, 0, DIV);
        }
    }
}

void factor()
{
    printf("FACTOR\n");
    if(TOKEN == identsym){
        char * ident = getIdent();

        emit(LOD, levelByName(ident), addressByName(ident));
        getToken();
    }
    else if(TOKEN ==numbersym){
        int num = getNumber();

        emit(LIT, 0, num);
        getToken();
    }
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

void emit(int op, int l, int m){
    if(codeLength > MAX_CODE_LENGTH){
        error(29);
    }

    code[codeLength].OP = op;
    code[codeLength].L = l;
    code[codeLength].M = m;
    codeLength++;
    printf("codeLength++\n");
}

void error(int num)
{
    printf("ERROR\n");
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
            printf("ERROR: = expected.\n");
            break;
        case 27:
            printf("ERROR: begin must be closed with end.\n");
            break;
        case 28:
            printf("ERROR: identifier, (, or number expected.\n");
            break;
        case 29:
            printf("ERROR: code length too long\n");
            break;
        default:
            printf("No such error code: %d\n", num);
            break;
    }
    exit(0);
}