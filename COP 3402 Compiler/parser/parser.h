typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym,
    multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym, elsesym
} token_type;

#define MAX_SYMBOL_TABLE_SIZE 100
/* For constants, store kind, name and val   For variables, store kind, name, L and M   For procedures, store kind, name, L and M */
typedef struct symbol {
    int kind;   // const = 1, var = 2, proc = 3
    char name[12]; // name up to 11 chars
    int val;   // value
    int level;  // L level
    int addr;   // M address
} symbol;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
typedef struct Instruction
{
    int line;
    int OP;
    int L;
    int M;
} Instruction;

enum IS
{
    INVALID,
    LIT = 1,
    OPR = 2,
    LOD = 3,
    STO = 4,
    CAL = 5,
    INC = 6,
    JMP = 7,
    JPC = 8,
    SIO1 = 9,
    SIO2 = 10,
    SIO3 = 11
};

enum AR
{
    RETURN_VAL,
    STATIC_LINK,
    DYNAMIC_LINK,
    RETURN_ADDRESS,
    LOCALS,
    PARAMETERS
};

enum ALUOPS
{
    RETURN,
    NEG,
    ADD,
    SUB,
    MUL,
    DIV,
    ODD,
    MOD,
    EQL,
    NEQ,
    LSS,
    LEQ,
    GTR,
    GEQ
};

const char * OPS[] = {
    "INVALID",
    "lit",
    "opr",
    "lod",
    "sto",
    "cal",
    "inc",
    "jmp",
    "jpc",
    "sio",
    "sio",
    "sio"
};
