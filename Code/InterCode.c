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

Operand constOperand(int value) {
    Operand op = malloc(sizeof(struct Operand_));
    op->kind = CONSTANT;
    op->u.value = value;
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

Operand addressOperand(int address) {
    Operand op = malloc(sizeof(struct Operand_));
    op->kind = ADDRESS;
    op->u.address = address;
    return op;
}

InterCode assignCode(Operand l, Operand r) {
    InterCode ic;
    ic.kind = ASSIGNCODE;
    ic.u.assign.left = l;
    ic.u.assign.right = r;
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

// InterCode lableCode(Operand lable) {
//     InterCode ic;
//     ic.kind = LABLECODE;
//     ic.u.lable.lable = lable;
//     return ic;
// }

// InterCode functionCode(Operand funcname) {
//     InterCode ic;
//     ic.kind = FUNCTIONCODE;
//     ic.u.function.funcname = funcname;
//     return ic;
// }

// InterCode gotoCode(Operand lable) {
//     InterCode ic;
//     ic.kind = GOTOCODE;
//     ic.u.gotoCode.lable = lable;
//     return ic;
// }

// InterCode ifCode() {

// }

// InterCode returnCode(Operand ret) {
//     InterCode ic;
//     ic.kind = RETURNCODE;
//     ic.u.returnCode.ret = ret;
//     return ic;
// }

// InterCode argCode(Operand op) {
//     InterCode ic;
//     ic.kind = ARGCODE;
//     ic.u.arg.op = op;
//     return ic;
// }

// InterCode paramCode(Operand lable) {
//     InterCode ic;
//     ic.kind = PARAMCODE;
//     ic.u.param.op = lable;
//     return ic;
// }

InterCodes interCodes(InterCode ic) {
    InterCodes ics = malloc(sizeof(struct InterCodes_));
    ics->code = ic;
    ics->next = NULL;
    ics->prev = NULL;
}

void insertInterCodes(InterCodes ics, InterCodes next) {
    assert(ics != NULL);
    while (ics->next != NULL)
    {
        ics = ics->next;
    }
    ics->next = next;
    next->prev = ics;
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
        // TODO
        exit(3);
        break;
    case TEMP:
        printf("t%d", op->u.var_no);
        break;
    case LABLE:
        // TODO
        printf("lable%d", op->u.lableNo);
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
            // printf("LABLE lable%d :", code.u.lable.lable->u.lableNo);
            printf("LABLE ");
            displayOperand(ics->code.u.sinop.op);
            printf(" :");
            break;
        case FUNCTIONCODE:
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
            break;
        case MULCODE:
            break;
        case DIVCODE:
            break;
        case GOTOCODE:
            break; 
        case IFECODE:
            break;
        case IFNECODE:
            break;
        case IFGCODE:
            break;
        case IFGECODE:
            break;
        case IFLCODE:
            break;
        case IFLECODE:
            break;
        case RETURNCODE:
            break;
        case DECCODE:
            break;
        case ARGCODE:
            break;
        case CALLCODE:
            break;
        case PARAMCODE:
            break;
        case READCODE:
            break;
        case WRITECODE:
            break;
        default:
            break;
        }
        printf("\n");
        ics = ics->next;
    }
}