#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vm_interpreter.h"

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define __INFILE__ "./mcode.txt"
#define __OUTFILE__ "./stacktrace.txt"

// Function Prototypes

void FileToCodeStore();
void PrintCodeStore(Instruction codeStore[]);
void parseCodeStore(Instruction codeStore[]);
bool isHalt(Instruction instruction);
void Fetch();
void Execute();
void ReturnFromCall();
void printPointers();
void printStack();
void ALU(Instruction IR);
int base(int l, int base);
void OpenOutput();
void CloseOutput();

static int pc = 0;
static int bp = 1;
static int sp = 0;

static int code_length = 0;
static int current_level = 0; // DO YOU NEED THIS? ...Maybe
static bool HALT = false;

static int pipes[MAX_LEXI_LEVELS];
int stack[MAX_STACK_HEIGHT];

Instruction IR;
Instruction codeStore[MAX_CODE_LENGTH];
FILE * stacktrace_file = NULL;
/*
int main()
{
    OpenOutput();
    if (HALT)
    {
        return 0;
    }
    FileToCodeStore();
    PrintCodeStore(codeStore);
    parseCodeStore(codeStore);
    CloseOutput();
    return 0;
}
*/
// Inputs the code from a file into the code store
void FileToCodeStore()
{
    FILE *fp;
    fp = (FILE*) fopen(__INFILE__, "r");
    if(fp == NULL)
    {
        printf("File not found: %s\n", __INFILE__);
        exit(0);
    }
    int i;

    for(i = 0; fscanf(fp, "%d %d %d", &codeStore[i].OP, &codeStore[i].L, &codeStore[i].M) != EOF; i++){
        codeStore[i].line = i;
    }
    code_length = i;
    fclose(fp);
    //free(code);
}

void PrintCodeStore(Instruction codeStore[])
{
    fprintf(stacktrace_file, "%s\t%s\t%s\t%s\n", "Line", "OP", "L", "M");
    int i;
    for(i = 0; i < code_length; i++)
    {
        fprintf(stacktrace_file, "%2d\t%s\t%d\t%d\n", i, OPS[codeStore[i].OP], codeStore[i].L, codeStore[i].M);
    }
}

void parseCodeStore(Instruction codeStore[])
{
    fprintf(stacktrace_file, "\t\t\t\tpc\tbp\tsp\tstack\n");
    fprintf(stacktrace_file, "Initial values\t\t\t\t\t%d\t%d\t%d\n", sp, bp, sp);
    while(true)
    {
        Fetch();
        Execute();
        if(isHalt(IR) || HALT)
            break;
    }
}

// Fetch Cycle
void Fetch()
{
    IR = codeStore[pc];
    pc++;
}

// Execute Cycle
void Execute()
{
    fprintf(stacktrace_file, "%2d\t%s\t%d\t%d", IR.line, OPS[IR.OP], IR.L, IR.M);
    switch(IR.OP)
    {
        case LIT:
            sp++;
            stack[sp] = IR.M;
            break;
        case OPR:
            ALU(IR);
            break;
        case LOD:
            sp++;
            stack[sp] = stack[base(IR.L, bp) + IR.M];
            break;
        case STO:
            stack[base(IR.L, bp) + IR.M] = stack[sp];
            sp--;
            break;
        case CAL:
            current_level++;
            pipes[current_level] = sp + 1;
            stack[sp + 1] = 0;
            stack[sp + 2] = base(IR.L, bp);
            stack[sp + 3] = bp;
            stack[sp + 4] = pc;
            bp = sp + 1;
            pc = IR.M;
            break;
        case INC:
            sp = sp + IR.M;
            break;
        case JMP:
            pc = IR.M;
            break;
        case JPC:
            if( stack[sp] == 0 )
            {
                pc = IR.M;
            }
            sp--;
            break;
        case SIO1:
            fprintf(stacktrace_file, "%d\n", stack[sp]);
            sp--;
            break;
        case SIO2:
            sp++;
            scanf("%d", &stack[sp]);
            break;
        case SIO3:
            pc = 0;
            bp = 0;
            sp = 0;
            break;
        default:
            printf("Invalid Operation\n");
            fprintf(stacktrace_file, "Invalid Operation\n");
            HALT = true;
    }

    if(!HALT)
    {
        printPointers();
        printStack();
    }
}

void ALU(Instruction IR)
{
    switch(IR.M)
    {
        case RETURN:
            if(IR.L == 0)
            {
                sp = bp - 1;
                pc = stack[sp + 4];
                bp = stack[sp + 3];
                pipes[current_level] = 0;
                current_level--;
            }
            break;
        case NEG:
            stack[sp] = -stack[sp];
            break;
        case ADD:
            sp--;
            stack[sp] = stack[sp] + stack[sp + 1];
            break;
        case SUB:
            sp--;
            stack[sp] = stack[sp] - stack[sp + 1];
            break;
        case MUL:
            sp--;
            stack[sp] = stack[sp] * stack[sp + 1];
            break;
        case DIV:
            sp--;
            stack[sp] = stack[sp] / stack[sp + 1];
            break;
        case ODD:
            stack[sp] = stack[sp] % 2;
            break;
        case MOD:
            sp--;
            stack[sp] = (stack[sp] % stack[sp + 1]);
            break;
        case EQL:
            sp--;
            stack[sp] = (stack[sp] == stack[sp + 1]);
            break;
        case NEQ:
            sp--;
            stack[sp] = (stack[sp] != stack[sp + 1]);
            break;
        case LSS:
            sp--;
            stack[sp] = (stack[sp] < stack[sp + 1]);
            break;
        case LEQ:
            sp--;
            stack[sp] = (stack[sp] <= stack[sp + 1]);
            break;
        case GTR:
            sp--;
            stack[sp] = (stack[sp] > stack[sp + 1]);
            break;
        case GEQ:
            sp--;
            stack[sp] = (stack[sp] >= stack[sp + 1]);
            break;
        default:
            printf("Invalid ALU Operation...\n");
            fprintf(stacktrace_file, "Invalid ALU Operation...\n");
            HALT = true;
    }
}

void printPointers()
{
    fprintf(stacktrace_file, "\t%d\t%d\t%d", pc, bp, sp);
}

void printStack()
{
    // fprintf(stacktrace_file, "PIPES: %d %d %d", pipes[0], pipes[1], pipes[2]);
    fprintf(stacktrace_file, "\t");
    int i = 1;
    int j = 1;
    while(i <= sp)
    {
        if(pipes[j] == i && pipes[j] > 0)
        {
            fprintf(stacktrace_file, "| ");
            j++;
        }
        fprintf(stacktrace_file, "%d ", stack[i]);
        i++;
    }
    fprintf(stacktrace_file, "\n");
}

bool isHalt(Instruction instruction)
{
    if(IR.OP == SIO3 && IR.L == 0 && IR.M == 3 )
        return true;

    return false;
}

int base(int l, int base)
{
    int b1 = base; // find base L levels down
    while (l > 0)
    {
        b1 = stack[b1 + 1];
        l--;
    }
    return b1;
}

void OpenOutput()
{
    if(stacktrace_file == NULL)
    {
        stacktrace_file = fopen(__OUTFILE__, "w");
    }
    else
    {
        fprintf(stacktrace_file, "%s is already open.\n", __OUTFILE__);
    }

    if(stacktrace_file == NULL)
    {
        printf("Could not open file: %s\n", __OUTFILE__);
        HALT = true;
    }
}

void CloseOutput()
{
    fclose(stacktrace_file);
    stacktrace_file = NULL;
    printf("Output: %s\n", __OUTFILE__);
}
