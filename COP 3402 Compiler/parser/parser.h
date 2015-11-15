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
