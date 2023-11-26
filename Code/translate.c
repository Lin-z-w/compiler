#include "translate.h"

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

void translateCode(SyntaxTree t) {
    switch (t->mytype)
    {
    case MProgram:
        // ExtDefList
        break;
    case MExtDefList:
        // ExtDef ExtDefList
        break;
    case MExtDef:
        switch (t->syntaxNum)
        {
        // Specifier ExtDecList SEMI
        // ExtDecList 中全部未赋值
        case 1:
            break;
        // Specifier SEMI
        case 2:
            break;
        // Specifier FunDec CompSt
        case 3:
            break;
        default:
            break;
        }
        break;
    case MExtDecList:
        switch (t->syntaxNum)
        {
        // VarDec
        case 1:
            break;
        // VarDec COMMA ExtDecList
        case 2:
            break;
        default:
            break;
        }
        break;
    case MSpecifier:
        switch (t->syntaxNum)
        {
        // TYPE
        case 1:
            break;
        // StructSpecifier
        case 2:
            break;
        default:
            break;
        }
        break;
    case MStructSpecifier:
        switch (t->syntaxNum)
        {
        // STRUCT OptTag LC DefList RC
        case 1:
            break;
        // STRUCT Tag
        case 2:
            break;
        default:
            break;
        }
        break; 
    case MOptTag:
        switch (t->syntaxNum)
        {
        // ID
        case 1:
            break;
        // empty
        case 2:
            break;
        default:
            break;
        }
        break;  
    case MTag:
        switch (t->syntaxNum)
        {
        // ID
        case 1:
            break;
        default:
            break;
        }
        break;
    case MVarDec:
        switch (t->syntaxNum)
        {
        // ID
        case 1:
            break;
        // VarDec LB INT RB
        case 2:
            break;
        default:
            break;
        }
        break;
    case MFunDec:
        switch (t->syntaxNum)
        {
        // ID LP VarList RP
        case 1:
            break;
        // ID LP RP
        case 2:
            break;
        default:
            break;
        }
        break;
    case MVarList:
        switch (t->syntaxNum)
        {
        // ParamDec COMMA VarList
        case 1:
            break;
        // ParamDec
        case 2:
            break;
        default:
            break;
        }
        break;
    case MParamDec:
        switch (t->syntaxNum)
        {
        // Specifier VarDec
        case 1:
            break;
        default:
            break;
        }
        break;
    case MCompSt:
        switch (t->syntaxNum)
        {
        // LC DefList StmtList RC
        case 1:
            break;
        default:
            break;
        }
        break;
    case MStmtList:
        // switch (t->syntaxNum)
        // {
        // // Stmt StmtList
        // case 1:
        //     break;
        // // empty
        // case 2:
        //     break;
        // default:
        //     break;
        // }
        break;
    case MStmt:
        switch (t->syntaxNum)
        {
        // Exp SEMI
        case 1:
            break;
        // CompSt
        case 2:
            break;
        // RETURN Exp SEMI
        case 3:
            break;
        // IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
        case 4:
            break;
        // IF LP Exp RP Stmt ELSE Stmt
        case 5:
            break;
        // WHILE LP Exp RP Stmt
        case 6:
            break;
        default:
            break;
        }
        break;
    // defList 中会包含很多的 NULL Type （作为headSymbol）
    case MDefList:
        switch (t->syntaxNum)
        {
        // Def DefList
        case 1:
            break;
        // empty
        case 2:
            break;
        default:
            break;
        }
        break; 
    case MDef:
        switch (t->syntaxNum)
        {
        // Specifier DecList SEMI
        case 1:
            break;
        default:
            break;
        }
        break;
    case MDecList:
        switch (t->syntaxNum)
        {
        // Dec
        case 1:
            break;
        // Dec COMMA DecList
        case 2:
            break;
        default:
            break;
        }
        break;
    case MDec:  
        switch (t->syntaxNum)
        {
        // VarDec
        case 1:
            break;
        // VarDec ASSIGNOP Exp
        case 2:
            break;
        default:
            break;
        }
        break;
    case MExp:
        translate_Exp(t, t->syntaxNum);
        break;
    case MArgs:
        switch (t->syntaxNum)
        {
        // Exp COMMA Args
        case 1:
            break;
        // Exp
        case 2:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}