#include <stddef.h>
#include <stdlib.h>

typedef struct Operand_* Operand;
typedef struct InterCode_ InterCode;
typedef struct InterCodes_* InterCodes;

struct Operand_ {
    enum { VARIABLE, CONSTANT, ADDRESS, TEMP} kind;
    union {
        char* name;
        int value;
        int var_no;
    } u;
};

struct InterCode_
{
    enum { LABLECODE, FUNCTIONCODE, ASSIGNCODE, ADDCODE, SUBCODE, MULCODE, DIVCODE, GOTOCODE, 
        IFECODE, IFNECODE, IFGCODE, IFGECODE, IFLCODE, IFLECODE, RETURNCODE, DECCODE, ARGCODE, CALLCODE, PARAMCODE, READCODE, WRITECODE } kind;
    union {
        struct { Operand lineno; } lable;
        struct { Operand funcname; } function;
        struct { Operand right, left; } assign;
        struct { Operand result, op1, op2; } binop;
        struct { Operand lineno; } gotoCode;
        struct { Operand op1, op2, lineno; } ifCode;
        struct { Operand ret; } returnCode;
        struct { Operand op, size; } dec;
        struct { Operand op; } arg;
        struct { Operand ret, func; } call;
        struct { Operand op; } param;
        struct { Operand op; } read;
        struct { Operand op; } write;
    } u;
};

struct InterCodes_ { 
    InterCode code; 
    InterCodes prev, next; 
};

extern InterCodes intercodes;

void insertInterCodes(InterCodes intercodes, InterCode code);