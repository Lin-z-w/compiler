#include "InterCode.h"

int tmpVarCnt = 0;
int lableCnt = 0;

Operand varOperand(char *name) {
    Operand op = malloc(sizeof(struct Operand_));
    op->kind = VARIABLE;
    op->u.name = malloc(sizeof(strlen(name)));
    strcpy(op->u.name, name);
    return op;
}

Operand addressOperand(char *name) {
    Operand op = malloc(sizeof(struct Operand_));
    op->kind = ADDRESS;
    op->u.name = malloc(sizeof(strlen(name)));
    strcpy(op->u.name, name);
    return op;
}

Operand contentOperand(int varNo) {
    Operand op = malloc(sizeof(struct Operand_));
    op->kind = CONTENT;
    op->u.var_no = varNo;
    return op;
}

Operand constOperand(int value) {
    Operand op = malloc(sizeof(struct Operand_));
    op->kind = CONSTANT;
    op->u.value = value;
    return op;
}

Operand spaceOperand(int space) {
    Operand op = malloc(sizeof(struct Operand_));
    op->kind = SPACE;
    op->u.space = space;
    return op;
}

Operand tmpOperand() {
    Operand op = malloc(sizeof(struct Operand_));
    op->kind = TEMP;
    op->u.var_no = tmpVarCnt;
    tmpVarCnt++;
    return op;
}

Operand lableOperand() {
    Operand op = malloc(sizeof(struct Operand_));
    op->kind = LABLE;
    op->u.var_no = lableCnt;
    lableCnt++;
    return op;
}

InterCode assignCode(Operand l, Operand r) {
    InterCode ic;
    ic.kind = ASSIGNCODE;
    ic.u.assign.left = l;
    ic.u.assign.right = r;
    return ic;
}

InterCode ifCode(Operand op1, Operand op2, Operand lable, int kind) {
    InterCode ic;
    ic.kind = kind;
    ic.u.ifcode.op1 = op1;
    ic.u.ifcode.op2 = op2;
    ic.u.ifcode.lable = lable;
    return ic;
}

InterCode decCode(Operand op, Operand size) {
    InterCode ic;
    ic.kind = DECCODE;
    ic.u.dec.op = op;
    ic.u.dec.size = size;
    return ic;
}

InterCode callCode(Operand ret, Operand func) {
    InterCode ic;
    ic.kind = CALLCODE;
    ic.u.call.ret = ret;
    ic.u.call.func = func;
    return ic;
}

InterCode binopCode(Operand reslut, Operand op1, Operand op2, int operator) {
    InterCode ic;
    ic.kind = operator;
    ic.u.binop.result = reslut;
    ic.u.binop.op1 = op1;
    ic.u.binop.op2 = op2;
    return ic;
}

InterCode sinopCode(Operand op, int kind) {
    InterCode ic;
    ic.kind = kind;
    ic.u.sinop.op = op;
    return ic;
}

InterCodes interCodes(InterCode ic) {
    InterCodes ics = malloc(sizeof(struct InterCodes_));
    ics->code = ic;
    ics->next = NULL;
    ics->prev = NULL;
}

ArgList argList(Operand arg) {
    ArgList al = malloc(sizeof(struct ArgList_));
    al->arg = arg;
    al->next = NULL;
    return al;
}

void insertInterCodes(InterCodes ics, InterCodes next) {
    assert(ics != NULL);
    while (ics->next != NULL)
    {
        ics = ics->next;
    }
    ics->next = next;
    // if (next != NULL) {
    //     next->prev = ics;
    // }
}

void insertInterCode(InterCodes ics, InterCode ic) {
    assert(ics != NULL);
    while (ics->next != NULL)
    {
        ics = ics->next;
    }
    ics->next = interCodes(ic);
    ics->next->prev = ics;
}

void insertArgList(ArgList argList1, ArgList argList2) {
    assert(argList1 != NULL);
    while (argList1->next != NULL) {
        argList1 = argList1->next;
    }
    argList1->next = argList2;
}

void displayOperand(Operand op, FILE* f) {
    switch (op->kind)
    {
    case VARIABLE:
        fprintf(f, "%s", op->u.name);
        break;
    case CONSTANT:
        fprintf(f, "#%d", op->u.value);
        break;
    case ADDRESS:
        fprintf(f, "&%s", op->u.name);
        break;
    case TEMP:
        fprintf(f, "t%d", op->u.var_no);
        break;
    case LABLE:
        fprintf(f, "label%d", op->u.lableNo);
        break;
    case CONTENT:
        fprintf(f, "*t%d", op->u.var_no);
        break;
    case SPACE:
        fprintf(f, "%d", op->u.space);
        break;
    default:
        break;
    }
}

void displayInterCodes(InterCodes ics, FILE* f) {
    while (ics != NULL) {
        InterCode code = ics->code;
        switch (code.kind)
        {
        case LABLECODE:
            fprintf(f, "LABEL ");
            displayOperand(code.u.sinop.op, f);
            fprintf(f, " :");
            break;
        case FUNCTIONCODE:
            fprintf(f, "FUNCTION ");
            displayOperand(code.u.sinop.op, f);
            fprintf(f, " :");
            break;
        case ASSIGNCODE:
            displayOperand(ics->code.u.assign.left, f);
            fprintf(f, " := ");
            displayOperand(ics->code.u.assign.right, f);
            break;
        case ADDCODE:
            displayOperand(ics->code.u.binop.result, f);
            fprintf(f, " := ");
            displayOperand(ics->code.u.binop.op1, f);
            fprintf(f, " + ");
            displayOperand(ics->code.u.binop.op2, f);
            break;
        case SUBCODE:
            displayOperand(ics->code.u.binop.result, f);
            fprintf(f, " := ");
            displayOperand(ics->code.u.binop.op1, f);
            fprintf(f, " - ");
            displayOperand(ics->code.u.binop.op2, f);
            break;
        case MULCODE:
            displayOperand(ics->code.u.binop.result, f);
            fprintf(f, " := ");
            displayOperand(ics->code.u.binop.op1, f);
            fprintf(f, " * ");
            displayOperand(ics->code.u.binop.op2, f);
            break;
        case DIVCODE:
            displayOperand(ics->code.u.binop.result, f);
            fprintf(f, " := ");
            displayOperand(ics->code.u.binop.op1, f);
            fprintf(f, " / ");
            displayOperand(ics->code.u.binop.op2, f);
            break;
        case GOTOCODE:
            fprintf(f, "GOTO ");
            displayOperand(code.u.sinop.op, f);
            break; 
        case IFECODE:
            fprintf(f, "IF ");
            displayOperand(code.u.ifcode.op1, f);
            fprintf(f, " == ");
            displayOperand(code.u.ifcode.op2, f);
            fprintf(f, " GOTO ");
            displayOperand(code.u.ifcode.lable, f);
            break;
        case IFNECODE:
            fprintf(f, "IF ");
            displayOperand(code.u.ifcode.op1, f);
            fprintf(f, " != ");
            displayOperand(code.u.ifcode.op2, f);
            fprintf(f, " GOTO ");
            displayOperand(code.u.ifcode.lable, f);
            break;
        case IFGCODE:
            fprintf(f, "IF ");
            displayOperand(code.u.ifcode.op1, f);
            fprintf(f, " > ");
            displayOperand(code.u.ifcode.op2, f);
            fprintf(f, " GOTO ");
            displayOperand(code.u.ifcode.lable, f);
            break;
        case IFGECODE:
            fprintf(f, "IF ");
            displayOperand(code.u.ifcode.op1, f);
            fprintf(f, " >= ");
            displayOperand(code.u.ifcode.op2, f);
            fprintf(f, " GOTO ");
            displayOperand(code.u.ifcode.lable, f);
            break;
        case IFLCODE:
            fprintf(f, "IF ");
            displayOperand(code.u.ifcode.op1, f);
            fprintf(f, " < ");
            displayOperand(code.u.ifcode.op2, f);
            fprintf(f, " GOTO ");
            displayOperand(code.u.ifcode.lable, f);
            break;
        case IFLECODE:
            fprintf(f, "IF ");
            displayOperand(code.u.ifcode.op1, f);
            fprintf(f, " <= ");
            displayOperand(code.u.ifcode.op2, f);
            fprintf(f, " GOTO ");
            displayOperand(code.u.ifcode.lable, f);
            break;
        case RETURNCODE:
            fprintf(f, "RETURN ");
            displayOperand(code.u.sinop.op, f);
            break;
        case DECCODE:
            fprintf(f, "DEC ");
            displayOperand(code.u.dec.op, f);
            fprintf(f, " ");
            displayOperand(code.u.dec.size, f);
            break;
        case ARGCODE:
            fprintf(f, "ARG ");
            displayOperand(code.u.sinop.op, f);
            break;
        case CALLCODE:
            displayOperand(code.u.call.ret, f);
            fprintf(f, " := CALL ");
            displayOperand(code.u.call.func, f);
            break;
        case PARAMCODE:
            fprintf(f, "PARAM ");
            displayOperand(code.u.sinop.op, f);
            break;
        case READCODE:
            fprintf(f, "READ ");
            displayOperand(code.u.sinop.op, f);
            break;
        case WRITECODE:
            fprintf(f, "WRITE ");
            displayOperand(code.u.sinop.op, f);
            break;
        default:
            break;
        }
        fprintf(f, "\n");
        ics = ics->next;
    }
}