#include "translateCode.h"

InterCodes translate_Exp(SyntaxTree exp, int syntaxNum) {
    InterCodes result = malloc(sizeof(struct InterCodes_));
    Operand op = malloc(sizeof(struct Operand_));
    result->next = NULL;
    result->prev = NULL;
    switch (syntaxNum)
    {
    // Exp ASSIGNOP Exp
    case 1:
        assert(exp->sons->sons->mytype == MID);

        break;
    // Exp AND Exp 
    case 2:

        break;
    // Exp OR Exp
    case 3:

        break;
    // Exp RELOP Exp
    case 4:

        break;
    // Exp PLUS Exp
    case 5:

        break;
    // Exp MINUS Exp %prec LOWER_THAN_MINUS 
    case 6:

        break;
    // Exp STAR Exp 
    case 7:

        break;
    // Exp DIV Exp
    case 8:

        break;
    // LP Exp RP
    case 9:

        break;
    // MINUS Exp
    case 10:

        break;
    // NOT Exp
    case 11:

        break;
    // ID LP Args RP
    case 12:

        break;
    // ID LP RP
    case 13:              

        break;
    // Exp LB Exp RB
    case 14:

        break;
    // Exp DOT ID
    case 15:
        // TODO
        exit(1);
        break;
    // ID
    case 16:
        op->kind = VARIABLE;
        op->u.name = malloc(sizeof(strlen(exp->sons->code)));
        strcpy(op->u.name, exp->sons->code);
        result->code.kind = ASSIGNCODE;
        result->code.u.assign.left = exp->place;
        result->code.u.assign.right = op;
        break;
    // INT
    case 17:
        op->kind = CONSTANT;
        op->u.value = atoi(exp->sons->code);
        result->code.kind = ASSIGNCODE;
        result->code.u.assign.left = exp->place;
        result->code.u.assign.right = op;
        break;
    // FLOAT
    case 18:
        assert(0);
    default:
        break;
    }
    return result;
}