#include "genAsm.h"

typedef struct StackTable_* StackTable;

struct StackTable_
{
    int offset;
    Operand op;
    StackTable next;
};

int regCnt = 0, fpOffset = 0;
StackTable sTable;

StackTable stackTable(Operand op) {
    StackTable s = malloc(sizeof(struct StackTable_));
    s->op = op;
    s->next = NULL;
    fpOffset -= 4;
    printf("addi $sp, $sp, -4\n");
    s->offset = fpOffset;
    return s;
}

void insertSTable(Operand op) {
    if (sTable == NULL) {
        sTable = stackTable(op);
    }
    else {
        StackTable tmp = sTable;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = stackTable(op);
    }
}

int findOffset(Operand op) {
    if (sTable == NULL) {
        insertSTable(op);
        return fpOffset;
    }
    else {
        StackTable tmp = sTable;
        while (1)
        {
            if (equalOperand(tmp->op, op)) {
                return tmp->offset;
            }
            if (tmp->next == NULL) {
                tmp->next = stackTable(op);
                return fpOffset;
            }
            tmp = tmp->next;
        }
    }
}

void loadOperand(Operand op, int r) {
    int offset = findOffset(op);
    printf("lw $t%d, %d($fp)\n", r, offset);
}

void storeOperand(Operand op, int r) {
    int offset = findOffset(op);
    printf("sw $t%d, %d($fp)\n", r, offset);
}

void genAndDisplayAsmCode() {
    genInterCode();
    translateInterCode2Asm(tree->intercodes);
}

void preCode() {
    printf(".data\n");
    printf("_prompt: .asciiz \"Enter an integer:\" \n");
    printf("_ret: .asciiz \"\\n\" \n");
    printf(".globl main \n");
    printf(".text \n");
    // read
    printf("read: \n");
    printf("li $v0, 4 \n");
    printf("la $a0, _prompt \n");
    printf("syscall \n");
    printf("li $v0, 5 \n");
    printf("syscall \n");
    printf("jr $ra \n");
    // write
    printf("write: \n");
    printf("li $v0, 1 \n");
    printf("syscall \n");
    printf("li $v0, 4 \n");
    printf("la $a0, _ret \n");
    printf("syscall \n");
    printf("move $v0, $0 \n");
    printf("jr $ra \n");

    // while num test
    // printf("move $fp, $sp\n");
    // printf("li $t0, 0\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -4($fp)\n");
    // printf("lw $t1, -4($fp)\n");
    // printf("move $t0, $t1\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -8($fp)\n");
    // printf("label_0:\n");
    // printf("lw $t1, -8($fp)\n");
    // printf("move $t0, $t1\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -12($fp)\n");
    // printf("li $t0, 1000\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -16($fp)\n");
    // printf("lw $t0, -12($fp)\n");
    // printf("lw $t1, -16($fp)\n");
    // printf("blt $t0, $t1, label_1\n");
    // printf("j label_2\n");
    // printf("label_1:\n");
    // printf("lw $t1, -8($fp)\n");
    // printf("move $t0, $t1\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -20($fp)\n");
    // printf("li $t0, 1\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -24($fp)\n");
    // printf("lw $t1, -20($fp)\n");
    // printf("lw $t2, -24($fp)\n");
    // printf("add $t0, $t1, $t2\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -28($fp)\n");
    // printf("lw $t1, -28($fp)\n");
    // printf("move $t0, $t1\n");
    // printf("sw $t0, -8($fp)\n");
    // printf("lw $t1, -8($fp)\n");
    // printf("move $t0, $t1\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -32($fp)\n");
    // printf("j label_0\n");
    // printf("label_2:\n");
    // printf("li $t0, 0\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -36($fp)\n");
    // printf("move $v0, $0\n");
    // printf("move $sp, $fp\n");
    // printf("jr $ra\n");

    // while test
    // while(1) {}

    // spim while{} test
    // printf("move $fp, $sp\n");
    // printf("label_0:\n");
    // printf("li $t0, 1\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -4($fp)\n");
    // printf("lw $t0, -4($fp)\n");
    // printf("li $t1, 0\n");
    // printf("bne $t0, $t1, label1\n");
    // printf("j label_2\n");
    // printf("label_1:\n");
    // printf("j label_0\n");
    // printf("label_2:\n");
    // printf("li $t0, 0\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -8($fp)\n");
    // printf("move $v0, $t0\n");
    // printf("move $sp, $fp\n");
    // printf("jr $ra\n");

    // add test
    // printf("add:\n");
    // printf("move $fp, $sp\n");
    // printf("lw $t0, 8($fp)\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -4($fp)\n");
    // printf("lw $t0, 12($fp)\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -8($fp)\n");
    // printf("li $t0, 0\n");
    // printf("addi $sp, $sp, -4\n");
    // printf("sw $t0, -12($fp)\n");
    // printf("move $v0, $t0\n");
    // printf("move $sp, $fp\n");
    // printf("jr $ra\n");
}

int reg(Operand op) {
    regCnt++;
    return regCnt-1;
}

void translateInterCode2Asm(InterCodes ics) {
    preCode();
    while (ics != NULL) {
        regCnt = 0;
        InterCode code = ics->code;
        Operand op1, op2, op3;
        int reg1, reg2, reg3, offset, paramCnt;
        switch (code.kind)
        {
        case LABLECODE:
            op1 = code.u.sinop.op;
            printf("label%d:\n", op1->u.labelNo);
            break;
        case FUNCTIONCODE:
            op1 = code.u.sinop.op;
            if (strcmp(op1->u.name, "add") == 0) exit(5);
            printf("%s:\n", op1->u.name);
            // }
            // else {
            //     printf("%sfunc:\n", op1->u.name);
            // }
            printf("move $fp, $sp\n");
            fpOffset = 0;
            paramCnt = 0;
            sTable = NULL;
            // TODO
            break;
        case ASSIGNCODE:
            op1 = code.u.assign.left;
            op2 = code.u.assign.right;
            reg1 = reg(op1);
            reg2 = reg(op2);
            // *x = y
            if (op1->kind == CONTENT) {
                // load y
                loadOperand(op2, reg2);
                // load x
                op3 = malloc(sizeof(struct Operand_));
                op3->kind = TEMP;
                op3->u.var_no = op1->u.var_no;
                loadOperand(op3, reg1);
                printf("sw $t%d, 0($t%d)\n", reg2, reg1);
            }
            // x = *y
            else if (op2->kind == CONTENT) {
                // load y
                op3 = malloc(sizeof(struct Operand_));
                op3->kind = TEMP;
                op3->u.var_no = op2->u.var_no;
                loadOperand(op3, reg2);
                printf("lw $t%d, 0($t%d)\n", reg1, reg2);
                storeOperand(op1, reg1);
            }
            // x = &y
            else if (op2->kind == ADDRESS) {
                op3 = varOperand(op2->u.name);
                offset = findOffset(op3);
                printf("la $t%d, %d($fp)\n", reg1, offset);
                storeOperand(op1, reg1);
            }
            else if (op2->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg1, op2->u.value);
                storeOperand(op1, reg1);
            }
            else {
                loadOperand(op2, reg2);
                printf("move $t%d, $t%d\n", reg1, reg2);
                storeOperand(op1, reg1);
            }
            break;
        case ADDCODE:
            op1 = code.u.binop.result;
            op2 = code.u.binop.op1;
            op3 = code.u.binop.op2;
            reg1 = reg(op1);
            reg2 = reg(op2);
            reg3 = reg(op3);
            if (op2->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg2, op2->u.value);
            }
            else {
                loadOperand(op2, reg2);
            }
            if (op3->kind == CONSTANT) {
                printf("addi $t%d, $t%d, %d\n", reg1, reg2, op3->u.value);
            }
            else {
                loadOperand(op3, reg3);
                printf("add $t%d, $t%d, $t%d\n", reg1, reg2, reg3);
            }
            storeOperand(op1, reg1);
            break;
        case SUBCODE:
            op1 = code.u.binop.result;
            op2 = code.u.binop.op1;
            op3 = code.u.binop.op2;
            reg1 = reg(op1);
            reg2 = reg(op2);
            reg3 = reg(op3);
            if (op2->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg2, op2->u.value);
            }
            else {
                loadOperand(op2, reg2);
            }
            if (op3->kind == CONSTANT) {
                printf("addi $t%d, $t%d, %d\n", reg1, reg2, -op3->u.value);
            }
            else {
                loadOperand(op3, reg3);
                printf("sub $t%d, $t%d, $t%d\n", reg1, reg2, reg3);
            }
            storeOperand(op1, reg1);
            break;
        case MULCODE:
            op1 = code.u.binop.result;
            op2 = code.u.binop.op1;
            op3 = code.u.binop.op2;
            reg1 = reg(op1);
            reg2 = reg(op2);
            reg3 = reg(op3);
            if (op2->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg2, op2->u.value);
            }
            else {
                loadOperand(op2, reg2);
            }
            if (op3->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg3, op3->u.value);
            }
            else {
                loadOperand(op3, reg3);
            }
            printf("mul $t%d, $t%d, $t%d\n", reg1, reg2, reg3);
            storeOperand(op1, reg1);
            break;
        case DIVCODE:
            op1 = code.u.binop.result;
            op2 = code.u.binop.op1;
            op3 = code.u.binop.op2;
            reg1 = reg(op1);
            reg2 = reg(op2);
            reg3 = reg(op3);
            if (op2->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg2, op2->u.value);
            }
            else {
                loadOperand(op2, reg2);
            }
            if (op3->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg3, op3->u.value);
            }
            else {
                loadOperand(op3, reg3);
            }
            printf("div $t%d, $t%d\n", reg2, reg3);
            printf("mflo $t%d\n", reg1);
            storeOperand(op1, reg1);
            break;
        case GOTOCODE:
            op1 = code.u.sinop.op;
            printf("j label%d\n", op1->u.labelNo);
            break; 
        case IFECODE:
            op1 = code.u.ifcode.op1;
            op2 = code.u.ifcode.op2;
            op3 = code.u.ifcode.label;
            reg1 = reg(op1);
            reg2 = reg(op2);
            if (op1->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg1, op1->u.value);
            }
            else {
                loadOperand(op1, reg1);
            }
            if (op2->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg2, op2->u.value);
            }
            else {
                loadOperand(op2, reg2);
            }
            printf("beq $t%d, $t%d, label%d\n", reg1, reg2, op3->u.labelNo);
            break;
        case IFNECODE:
            op1 = code.u.ifcode.op1;
            op2 = code.u.ifcode.op2;
            op3 = code.u.ifcode.label;
            reg1 = reg(op1);
            reg2 = reg(op2);
            if (op1->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg1, op1->u.value);
            }
            else {
                loadOperand(op1, reg1);
            }
            if (op2->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg2, op2->u.value);
            }
            else {
                loadOperand(op2, reg2);
            }
            printf("bne $t%d, $t%d, label%d\n", reg1, reg2, op3->u.labelNo);
            break;
        case IFGCODE:
            op1 = code.u.ifcode.op1;
            op2 = code.u.ifcode.op2;
            op3 = code.u.ifcode.label;
            reg1 = reg(op1);
            reg2 = reg(op2);
            if (op1->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg1, op1->u.value);
            }
            else {
                loadOperand(op1, reg1);
            }
            if (op2->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg2, op2->u.value);
            }
            else {
                loadOperand(op2, reg2);
            }
            printf("bgt $t%d, $t%d, label%d\n", reg1, reg2, op3->u.labelNo);
            break;
        case IFGECODE:
            op1 = code.u.ifcode.op1;
            op2 = code.u.ifcode.op2;
            op3 = code.u.ifcode.label;
            reg1 = reg(op1);
            reg2 = reg(op2);
            if (op1->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg1, op1->u.value);
            }
            else {
                loadOperand(op1, reg1);
            }
            if (op2->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg2, op2->u.value);
            }
            else {
                loadOperand(op2, reg2);
            }
            printf("bge $t%d, $t%d, label%d\n", reg1, reg2, op3->u.labelNo);
            break;
        case IFLCODE:
            op1 = code.u.ifcode.op1;
            op2 = code.u.ifcode.op2;
            op3 = code.u.ifcode.label;
            reg1 = reg(op1);
            reg2 = reg(op2);
            if (op1->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg1, op1->u.value);
            }
            else {
                loadOperand(op1, reg1);
            }
            if (op2->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg2, op2->u.value);
            }
            else {
                loadOperand(op2, reg2);
            }
            printf("blt $t%d, $t%d, label%d\n", reg1, reg2, op3->u.labelNo);
            break;
        case IFLECODE:
            op1 = code.u.ifcode.op1;
            op2 = code.u.ifcode.op2;
            op3 = code.u.ifcode.label;
            reg1 = reg(op1);
            reg2 = reg(op2);
            if (op1->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg1, op1->u.value);
            }
            else {
                loadOperand(op1, reg1);
            }
            if (op2->kind == CONSTANT) {
                printf("li $t%d, %d\n", reg2, op2->u.value);
            }
            else {
                loadOperand(op2, reg2);
            }
            printf("ble $t%d, $t%d, label%d\n", reg1, reg2, op3->u.labelNo);
            break;
        case RETURNCODE:
            op1 = code.u.sinop.op;
            reg1 = reg(op1);
            printf("move $v0, $t%d\n", reg1);
            // make sp = fp
            printf("move $sp, $fp\n");
            printf("jr $ra\n");
            break;
        case DECCODE:
            op1 = code.u.dec.op;
            op2 = code.u.dec.size;
            printf("addi $sp, $sp, %d\n", -op2->u.space);
            fpOffset -= op2->u.space;
            findOffset(op1);
            break;
        case ARGCODE:
            op1 = code.u.sinop.op;
            reg1 = reg(op1);
            loadOperand(op1, reg1);
            fpOffset -= 4;
            printf("addi $sp, $sp, -4\n");
            printf("sw $t%d, 0($sp)\n", reg1);
            break;
        case CALLCODE:
            op1 = code.u.call.ret;
            reg1 = reg(op1);
            op2 = code.u.call.func;
            // save $fp
            printf("addi $sp, $sp, -4\n");
            printf("la $t%d, 0($fp)\n", reg1);
            printf("sw $t%d, 0($sp)\n", reg1);
            // save ret address in ra
            printf("addi $sp, $sp, -4\n");
            printf("sw $ra, 0($sp)\n");
            // printf("jal %sfunc\n", op2->u.name);
            printf("jal %s\n", op2->u.name);
            printf("lw $ra, 0($sp)\n");
            printf("addi $sp, $sp, 4\n");
            // gain origin fp
            printf("lw $fp, 0($sp)\n");
            printf("addi $sp, $sp, 4\n");
            // save ret value
            printf("move $t%d, $v0\n", reg1);
            storeOperand(op1, reg1);
            break;
        case PARAMCODE:
            op1 = code.u.sinop.op;
            reg1 = reg(op1);
            printf("lw $t%d, %d($fp)\n", reg1, paramCnt * 4 + 8);
            paramCnt++;
            storeOperand(op1, reg1);
            break;
        case READCODE:
            op1 = code.u.sinop.op;
            reg1 = reg(op1);
            printf("addi $sp, $sp, -4\n");
            printf("sw $ra, 0($sp)\n");
            printf("jal read\n");
            printf("lw $ra, 0($sp)\n");
            printf("addi $sp, $sp, 4\n");
            printf("move $t%d, $v0\n", reg1);
            storeOperand(op1, reg1);
            break;
        case WRITECODE:
            op1 = code.u.sinop.op;
            reg1 = reg(op1);
            loadOperand(op1, reg1);
            printf("move $a0, $t%d\n", reg1);
            printf("addi $sp, $sp, -4\n");
            printf("sw $ra, 0($sp)\n");
            printf("jal write\n");
            printf("lw $ra, 0($sp)\n");
            printf("addi $sp, $sp, 4\n");
            break;
        default:
            break;
        }
        ics = ics->next;
    }
}