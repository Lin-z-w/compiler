#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct Operand_* Operand;
typedef struct InterCode_ InterCode;
typedef struct InterCodes_* InterCodes;
typedef struct ArgList_* ArgList;

struct Operand_ {
    enum { VARIABLE, CONSTANT, ADDRESS, TEMP, LABLE, CONTENT, SPACE} kind;
    union {
        char* name;
        int value;
        int lableNo;
        int var_no;
        int space;
    } u;
};

struct InterCode_
{
    enum { LABLECODE, FUNCTIONCODE, ASSIGNCODE, ADDCODE, SUBCODE, MULCODE, DIVCODE, GOTOCODE, 
        IFECODE, IFNECODE, IFGCODE, IFGECODE, IFLCODE, IFLECODE, RETURNCODE, DECCODE, ARGCODE, CALLCODE, PARAMCODE, READCODE, WRITECODE } kind;
    union {
        struct { Operand right, left; }         assign;
        struct { Operand op1, op2, lable; }     ifcode;
        struct { Operand op, size; }            dec;
        struct { Operand ret, func; }           call;
        struct { Operand result, op1, op2; }    binop;
        struct { Operand op; }                  sinop;
        // struct { Operand funcname; }            function;
        // struct { Operand lable; }               lable;
        // struct { Operand lable; }               gotoCode;
        // struct { Operand ret; }                 returnCode;
        // struct { Operand op; }                  arg;
        // struct { Operand op; }                  param;
        // struct { Operand op; }                  read;
        // struct { Operand op; }                  write;
    } u;
};

struct InterCodes_ { 
    InterCode code; 
    InterCodes prev, next; 
};

struct ArgList_
{
    Operand arg;
    ArgList next;
};


// construct func
Operand varOperand(char *name);
Operand addressOperand(char* name);
Operand contentOperand(int varNo);
Operand constOperand(int value);
Operand spaceOperand(int space);
Operand tmpOperand();
Operand lableOperand();
InterCode assignCode(Operand l, Operand r);
InterCode decCode(Operand op, Operand size);
InterCode ifCode(Operand op1, Operand op2, Operand lable, int kind);
InterCode callCode(Operand ret, Operand func);
InterCode binopCode(Operand reslut, Operand op1, Operand op2, int operator);
InterCode sinopCode(Operand op, int kind);
InterCodes interCodes(InterCode ic);
ArgList argList(Operand arg);

// modify
void insertInterCodes(InterCodes intercodes, InterCodes next);
void insertInterCode(InterCodes ics, InterCode ic);
void insertArgList(ArgList argList1, ArgList argList2);

// dispaly
void displayInterCodes(InterCodes ics);