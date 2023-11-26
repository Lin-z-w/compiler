#include "translate.h"

const int acceptFloat = 0, acceptStruct = 0;
SymbolTable topTable;

InterCodes translate_Exp(SyntaxTree exp);
InterCodes translate_Cond(SyntaxTree exp, Operand lableT, Operand lableF);
InterCodes translate_Args(SyntaxTree args);
InterCodes translate_CompSt(SyntaxTree compSt);
InterCodes translate_Stmt(SyntaxTree stmt);

InterCodes translate_Cond(SyntaxTree exp, Operand lableT, Operand lableF) {
    SyntaxTree exp1, exp2;
    InterCodes result;
    InterCode code;
    Operand op1, op2;
    int kind;
    char* relop;
    switch (exp->syntaxNum)
    {
    // NOT Exp
    case 11:
        exp1 = exp->sons->next;
        result = translate_Cond(exp1, lableF, lableT);
        break;
    // Exp AND Exp 
    case 2:
        exp1 = exp->sons;
        exp2 = exp1->next->next;
        op1 = lableOperand();
        // code1
        result = translate_Cond(exp1, op1, lableF);
        // LABLE op1
        code = sinopCode(op1, LABLECODE);
        insertInterCode(result, code);
        // code2
        insertInterCodes(result, translate_Cond(exp2, lableT, lableF));
        break;
    // Exp OR Exp
    case 3:
        exp1 = exp->sons;
        exp2 = exp1->next->next;
        op1 = lableOperand();
        // code1
        result = translate_Cond(exp1, lableT, op1);
        // LABLE op1
        code = sinopCode(op1, LABLECODE);
        insertInterCode(result, code);
        // code2
        insertInterCodes(result, translate_Cond(exp2, lableT, lableF));
        break;
    // Exp RELOP Exp
    case 4:
        exp1 = exp->sons;
        relop = exp1->next->code;
        exp2 = exp1->next->next;
        // code1
        result = translate_Exp(exp1);
        // code2
        insertInterCodes(result, translate_Exp(exp2));
        if (strcmp(relop, ">") == 0) {
            kind = IFGCODE;
        }
        else if (strcmp(relop, ">=") == 0) {
            kind = IFGECODE;
        }
        else if (strcmp(relop, "<") == 0) {
            kind = IFLCODE;
        }
        else if (strcmp(relop, "<=") == 0) {
            kind = IFLECODE;
        }
        else if (strcmp(relop, "==") == 0) {
            kind = IFECODE;
        }
        else if (strcmp(relop, "!=") == 0) {
            kind = IFNECODE;
        }
        // code3
        code = ifCode(exp1->place, exp2->place, lableT, kind);
        insertInterCode(result, code);
        // GOTO false lable
        code = sinopCode(lableF, GOTOCODE);
        insertInterCode(result, code);
        break;
    default:
        // code1
        result = translate_Exp(exp);
        // code2
        op1 = constOperand(0);
        code = ifCode(exp->place, op1, lableT, IFNECODE);
        insertInterCode(result, code);
        // GOTO false lable
        code = sinopCode(lableF, GOTOCODE);
        insertInterCode(result, code);
        break;
    }
    return NULL;
}

InterCodes translate_Args(SyntaxTree args) {
    SyntaxTree exp, args1;
    InterCodes result;
    ArgList arlst;
    switch (args->syntaxNum)
    {
    // Exp COMMA Args
    case 1:
        exp = args->sons;
        args1 = exp->next->next;
        // code1
        result = translate_Exp(exp);
        args->argList = argList(exp->place);
        // code2
        insertInterCodes(result, translate_Args(args1));
        insertArgList(args1->argList, args->argList);
        args->argList = args1->argList;
        break;
    // Exp
    case 2:
        exp = args->sons;
        // code1
        result = translate_Exp(exp);
        args->argList = argList(exp->place);
        break;
    default:
        break;
    }
    return result;
}

InterCodes translate_Exp(SyntaxTree exp) {
    InterCode code;
    InterCodes result;
    SyntaxTree exp1, exp2, id, args;
    Operand op1, op2, op3;
    ArgList arlst;
    result->next = NULL;
    result->prev = NULL;

    // init place
    exp->place = tmpOperand();

    switch (exp->syntaxNum)
    {
    // Exp1 ASSIGNOP Exp2
    case 1:
        exp1 = exp->sons;
        exp2 = exp->sons->next->next;
        // Exp1 must be ID
        assert(exp1->mytype == MID);
        // translate exp2
        result = translate_Exp(exp2);
        // assign exp1
        op1 = varOperand(exp1->code);
        code = assignCode(op1, exp2->place);
        insertInterCode(result, code);
        // assign exp->place
        code = assignCode(exp->place, op1);
        insertInterCode(result, code);
        break;
    // Exp AND Exp 
    case 2:
    // Exp OR Exp
    case 3:
    // Exp RELOP Exp
    case 4:
        op1 = lableOperand();
        op2 = lableOperand();
        op3 = constOperand(0);
        code = assignCode(exp->place, op3);
        // init place = 0;
        result = interCodes(code);
        // translate cond
        insertInterCodes(result, translate_Cond(exp, op1, op2));
        // if true place = 1
        code = sinopCode(op1, LABLECODE);
        insertInterCode(result, code);
        op3 = constOperand(1);
        code = assignCode(exp->place, op3);
        insertInterCode(result, code);
        // if false jump to l2
        code = sinopCode(op2, LABLECODE);
        insertInterCode(result, code);
        break;
    // Exp PLUS Exp
    case 5:
        exp1 = exp->sons;
        exp2 = exp->sons->next->next;
        result = translate_Exp(exp1);
        insertInterCodes(result, translate_Exp(exp2));
        code = binopCode(exp->place, exp1->place, exp2->place, ADDCODE);
        insertInterCode(result, code);
        break;
    // Exp MINUS Exp %prec LOWER_THAN_MINUS 
    case 6:
        exp1 = exp->sons;
        exp2 = exp->sons->next->next;
        result = translate_Exp(exp1);
        insertInterCodes(result, translate_Exp(exp2));
        code = binopCode(exp->place, exp1->place, exp2->place, SUBCODE);
        insertInterCode(result, code);
        break;
    // Exp STAR Exp 
    case 7:
        exp1 = exp->sons;
        exp2 = exp->sons->next->next;
        result = translate_Exp(exp1);
        insertInterCodes(result, translate_Exp(exp2));
        code = binopCode(exp->place, exp1->place, exp2->place, MULCODE);
        insertInterCode(result, code);
        break;
    // Exp DIV Exp
    case 8:
        exp1 = exp->sons;
        exp2 = exp->sons->next->next;
        result = translate_Exp(exp1);
        insertInterCodes(result, translate_Exp(exp2));
        code = binopCode(exp->place, exp1->place, exp2->place, DIVCODE);
        insertInterCode(result, code);
        break;
    // LP Exp RP
    case 9:
        exp1 = exp->sons->next;
        result = translate_Exp(exp1);
        code = assignCode(exp->place, exp1->place);
        insertInterCode(result, code);
        break;
    // MINUS Exp
    case 10:
        exp1 = exp->sons->next;
        result = translate_Exp(exp1);
        op1 = constOperand(0);
        code = binopCode(exp->place, op1, exp1->place, SUBCODE);
        insertInterCode(result, code);
        break;
    // NOT Exp
    case 11:
        op1 = lableOperand();
        op2 = lableOperand();
        op3 = constOperand(0);
        code = assignCode(exp->place, op3);
        // init place = 0;
        result = interCodes(code);
        // translate cond
        insertInterCodes(result, translate_Cond(exp, op1, op2));
        // if true place = 1
        code = sinopCode(op1, LABLECODE);
        insertInterCode(result, code);
        op3 = constOperand(1);
        code = assignCode(exp->place, op3);
        insertInterCode(result, code);
        // if false jump to l2
        code = sinopCode(op2, LABLECODE);
        insertInterCode(result, code);
        break;
    // ID LP Args RP
    case 12:
        id = exp->sons;
        args = id->next->next;
        // code1
        result = translate_Args(args);
        arlst = args->argList;
        if (strcmp(id->code, "write") == 0) {
            // WRITE arlst[0]
            code = sinopCode(arlst->arg, WRITECODE);
            insertInterCode(result, code);
            // return 0
            op1 = constOperand(0);
            code = assignCode(exp->place, op1);
            insertInterCode(result, code);
        }
        else {
            // for i = 1 to length(arg_list) code2 = code2 + [ARG arg_list[i]]
            while (arlst != NULL) {
                code = sinopCode(arlst->arg, ARGCODE);
                insertInterCode(result, code);
                arlst = arlst->next;
            }
            op1 = varOperand(id->code);
            code = callCode(exp->place, op1);
            insertInterCode(result, code);
        }
        break;
    // ID LP RP
    case 13:       
        id = exp->sons;
        if (strcmp(id->code, "read") == 0) {
            code = sinopCode(exp->place, READCODE);
            result = interCodes(code);
        }
        else {
            op1 = varOperand(id->code);
            code = callCode(exp->place, op1);
            result = interCodes(code);
        }
        break;
    // Exp LB Exp RB
    case 14:

        break;
    // Exp DOT ID
    case 15:
        // TODO
        assert(acceptStruct == 0);
        break;
    // ID
    case 16:
        op1 = varOperand(exp->sons->code);
        code = assignCode(exp->place, op1);
        result = interCodes(code);
        break;
    // INT
    case 17:
        op1 = constOperand(atoi(exp->sons->code));
        code = assignCode(exp->place, op1);
        result = interCodes(code);
        break;
    // FLOAT
    case 18:
        assert(acceptFloat == 0);
    default:
        break;
    }
    return result;
}

InterCodes translate_CompSt(SyntaxTree compSt) {
    return NULL;
}

InterCodes translate_Stmt(SyntaxTree stmt) {
    InterCode code;
    InterCodes result;
    Operand op1, op2, op3;
    SyntaxTree exp, stmt1, stmt2;
    switch (stmt->syntaxNum)
    {
    // Exp SEMI
    case 1:
        exp = stmt->sons;
        result = translate_Exp(exp);
        break;
    // CompSt
    case 2:
        result = translate_CompSt(stmt->sons);
        break;
    // RETURN Exp SEMI
    case 3:
        exp = stmt->sons->next;
        result = translate_Exp(exp);
        code = sinopCode(exp->place, RETURNCODE);
        insertInterCode(result, code);
        break;
    // IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
    case 4:
        exp = stmt->sons->next->next;
        stmt1 = exp->next->next;
        op1 = lableOperand();
        op2 = lableOperand();
        // code1
        result = translate_Cond(exp, op1, op2);
        // LABLE op1
        code = sinopCode(op1, LABLECODE);
        insertInterCode(result, code);
        // code2
        insertInterCodes(result, translate_Stmt(stmt1));
        // LABLE op2
        code = sinopCode(op2, LABLECODE);
        insertInterCode(result, code);
        break;
    // IF LP Exp RP Stmt ELSE Stmt
    case 5:
        exp = stmt->sons->next->next;
        stmt1 = exp->next->next;
        stmt2 = stmt1->next->next;
        op1 = lableOperand();
        op2 = lableOperand();
        op3 = lableOperand();
        // code1
        result = translate_Cond(exp, op1, op2);
        // LABLE op1
        code = sinopCode(op1, LABLECODE);
        insertInterCode(result, code);
        // code2
        insertInterCodes(result, translate_Stmt(stmt1));
        // GOTO op3
        code = sinopCode(op3, GOTOCODE);
        insertInterCode(result, code);
        // LABLE op2
        code = sinopCode(op2, LABLECODE);
        insertInterCode(result, code);
        // code3
        insertInterCodes(result, translate_Stmt(stmt2));
        // LABLE op3
        code = sinopCode(op3, LABLECODE);
        insertInterCode(result, code);
        break;
    // WHILE LP Exp RP Stmt
    case 6:
        op1 = lableOperand();
        op2 = lableOperand();
        op3 = lableOperand();
        exp = stmt->sons->next->next;
        stmt1 = exp->next->next;
        // LABLE op1
        code = sinopCode(op1, LABLECODE);
        result = interCodes(code);
        // code1
        insertInterCodes(result, translate_Cond(exp, op2, op3));
        // LABLE op2
        code = sinopCode(op2, LABLECODE);
        insertInterCode(result, code);
        // code2
        insertInterCodes(result, translate_Stmt(stmt1));
        // GOTO op1
        code = sinopCode(op1, GOTOCODE);
        insertInterCode(result, code);
        // LABLE op3
        code = sinopCode(op3, LABLE);
        insertInterCode(result, code);
        break;
    default:
        break;
    }
    return result;
}









void translateCode(SyntaxTree t) {
    topTable = symTable[0];

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
        translate_Stmt(t);
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
        translate_Exp(t);
        break;
    case MArgs:
        break;
    default:
        break;
    }
}

void genInterCode() {
    translateCode(tree);
    displayInterCodes(tree->intercodes);
}