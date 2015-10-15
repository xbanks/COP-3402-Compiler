enum IS
{
    INVALID,
    LIT,
    OPR,
    LOD,
    STO,
    CAL,
    INC,
    JMP,
    JPC,
    SIO1,
    SIO2,
    SIO3
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

typedef struct Instruction
{
    int line;
    int OP;
    int L;
    int M;
} Instruction;

