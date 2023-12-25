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
    ic.u.ifcode.label = lable;
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

void displayOperand(Operand op) {
    switch (op->kind)
    {
    case VARIABLE:
        printf("%s", op->u.name);
        break;
    case CONSTANT:
        printf("#%d", op->u.value);
        break;
    case ADDRESS:
        printf("&%s", op->u.name);
        break;
    case TEMP:
        printf("t%d", op->u.var_no);
        break;
    case LABLE:
        printf("label%d", op->u.labelNo);
        break;
    case CONTENT:
        printf("*t%d", op->u.var_no);
        break;
    case SPACE:
        printf("%d", op->u.space);
        break;
    default:
        break;
    }
}

void displayInterCodes(InterCodes ics) {
    while (ics != NULL) {
        InterCode code = ics->code;
        switch (code.kind)
        {
        case LABLECODE:
            printf("LABEL ");
            displayOperand(code.u.sinop.op);
            printf(" :");
            break;
        case FUNCTIONCODE:
            printf("FUNCTION ");
            displayOperand(code.u.sinop.op);
            printf(" :");
            break;
        case ASSIGNCODE:
            displayOperand(ics->code.u.assign.left);
            printf(" := ");
            displayOperand(ics->code.u.assign.right);
            break;
        case ADDCODE:
            displayOperand(ics->code.u.binop.result);
            printf(" := ");
            displayOperand(ics->code.u.binop.op1);
            printf(" + ");
            displayOperand(ics->code.u.binop.op2);
            break;
        case SUBCODE:
            displayOperand(ics->code.u.binop.result);
            printf(" := ");
            displayOperand(ics->code.u.binop.op1);
            printf(" - ");
            displayOperand(ics->code.u.binop.op2);
            break;
        case MULCODE:
            displayOperand(ics->code.u.binop.result);
            printf(" := ");
            displayOperand(ics->code.u.binop.op1);
            printf(" * ");
            displayOperand(ics->code.u.binop.op2);
            break;
        case DIVCODE:
            displayOperand(ics->code.u.binop.result);
            printf(" := ");
            displayOperand(ics->code.u.binop.op1);
            printf(" / ");
            displayOperand(ics->code.u.binop.op2);
            break;
        case GOTOCODE:
            printf("GOTO ");
            displayOperand(code.u.sinop.op);
            break; 
        case IFECODE:
            printf("IF ");
            displayOperand(code.u.ifcode.op1);
            printf(" == ");
            displayOperand(code.u.ifcode.op2);
            printf(" GOTO ");
            displayOperand(code.u.ifcode.label);
            break;
        case IFNECODE:
            printf("IF ");
            displayOperand(code.u.ifcode.op1);
            printf(" != ");
            displayOperand(code.u.ifcode.op2);
            printf(" GOTO ");
            displayOperand(code.u.ifcode.label);
            break;
        case IFGCODE:
            printf("IF ");
            displayOperand(code.u.ifcode.op1);
            printf(" > ");
            displayOperand(code.u.ifcode.op2);
            printf(" GOTO ");
            displayOperand(code.u.ifcode.label);
            break;
        case IFGECODE:
            printf("IF ");
            displayOperand(code.u.ifcode.op1);
            printf(" >= ");
            displayOperand(code.u.ifcode.op2);
            printf(" GOTO ");
            displayOperand(code.u.ifcode.label);
            break;
        case IFLCODE:
            printf("IF ");
            displayOperand(code.u.ifcode.op1);
            printf(" < ");
            displayOperand(code.u.ifcode.op2);
            printf(" GOTO ");
            displayOperand(code.u.ifcode.label);
            break;
        case IFLECODE:
            printf("IF ");
            displayOperand(code.u.ifcode.op1);
            printf(" <= ");
            displayOperand(code.u.ifcode.op2);
            printf(" GOTO ");
            displayOperand(code.u.ifcode.label);
            break;
        case RETURNCODE:
            printf("RETURN ");
            displayOperand(code.u.sinop.op);
            break;
        case DECCODE:
            printf("DEC ");
            displayOperand(code.u.dec.op);
            printf(" ");
            displayOperand(code.u.dec.size);
            break;
        case ARGCODE:
            printf("ARG ");
            displayOperand(code.u.sinop.op);
            break;
        case CALLCODE:
            displayOperand(code.u.call.ret);
            printf(" := CALL ");
            displayOperand(code.u.call.func);
            break;
        case PARAMCODE:
            printf("PARAM ");
            displayOperand(code.u.sinop.op);
            break;
        case READCODE:
            printf("READ ");
            displayOperand(code.u.sinop.op);
            break;
        case WRITECODE:
            printf("WRITE ");
            displayOperand(code.u.sinop.op);
            break;
        default:
            break;
        }
        printf("\n");
        ics = ics->next;
    }
}

int equalOperand(Operand op1, Operand op2) {
    int res = 0;
    if (op1->kind == op2->kind) {
        switch (op1->kind)
        {
        case VARIABLE:
        case ADDRESS:
            res = (strcmp(op1->u.name, op2->u.name) == 0);
            break;
        case CONSTANT:
            res = (op1->u.value == op2->u.value);
            break;
        case TEMP:
        case CONTENT:
            res = (op1->u.var_no == op2->u.var_no);
            break;
        case LABLE:
            assert(0);
        case SPACE:
            res = (op1->u.space == op2->u.space);
            break;
        default:
            break;
        }
    }
    return res;
}