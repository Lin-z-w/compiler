#include "translate.h"

const int acceptFloat = 0, acceptStruct = 0, acceptGlobleVar = 0;
SymbolTable topTable;
AliasTable alTable;
ParamTable prTable = NULL;

InterCodes translate_Exp(SyntaxTree exp);
InterCodes translate_Cond(SyntaxTree exp, Operand lableT, Operand lableF);
InterCodes translate_Args(SyntaxTree args);
InterCodes translate_CompSt(SyntaxTree compSt);
InterCodes translate_Stmt(SyntaxTree stmt);
InterCodes translate_StmtList(SyntaxTree stmtList);
InterCodes translate_FunDec(SyntaxTree funDec);

ParamTable paramTable(char* name) {
    ParamTable newTable = malloc(sizeof(struct ParamTable_));
    newTable->name = malloc(sizeof(name));
    strcpy(newTable->name, name);
    return newTable;
}

int isParam(char* name) {
    if (prTable == NULL) {
        return 0;
    }
    while (prTable != NULL) {
        if (strcmp(name, prTable->name) == 0) {
            return 1;
        }
        prTable = prTable->next;
    }
    return 0;
}

void insertParamTable(char *name) {
    ParamTable tmp = prTable;
    if (prTable == NULL) {
        prTable = paramTable(name);
    } 
    else {
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = paramTable(name);
    }
}

AliasTable aliasTable(char* name, char* alias) {
    AliasTable newTable = malloc(sizeof(struct AliasTable_));
    newTable->alias.name = malloc(sizeof(name));
    strcpy(newTable->alias.name, name);
    newTable->alias.alias = malloc(sizeof(alias));
    strcpy(newTable->alias.alias, alias);
    newTable->next = NULL;
    return newTable;
}

char* findAlias(char* name) {
    AliasTable tmp = alTable;
    while (tmp != NULL)
    {
        if (strcmp(tmp->alias.name, name) == 0) {
            return tmp->alias.alias;
        }
        tmp = tmp->next;
    }
    return name;
}

void insertAliasTable(AliasTable next) {
    AliasTable tmp = alTable;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = next; 
}

InterCodes translate_Cond(SyntaxTree exp, Operand lableT, Operand lableF) {
    SyntaxTree exp1, exp2;
    InterCodes result = NULL;
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
    return result;
}

InterCodes translate_Args(SyntaxTree args) {
    SyntaxTree exp, args1;
    InterCodes result = NULL;
    Operand op;
    ArgList arlst;
    switch (args->syntaxNum)
    {
    // Exp COMMA Args
    case 1:
        exp = args->sons;
        args1 = exp->next->next;
        // code1
        result = translate_Exp(exp);
        if (isStruct(exp->type)) {
            args->argList = argList(exp->address);
        }
        else {
            args->argList = argList(exp->place);
        }
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
        if (isStruct(exp->type)) {
            args->argList = argList(exp->address);
        }
        else {
            args->argList = argList(exp->place);
        }
        break;
    default:
        break;
    }
    return result;
}

InterCodes translate_Exp(SyntaxTree exp) {
    InterCode code;
    InterCodes result = NULL;
    SyntaxTree exp1, exp2, id, args;
    Operand op1, op2, op3;
    ArgList arlst;
    AliasTable newTable, targetTable;
    FieldList fielist;
    char* realname1, *realname2;
    int basicSize = 0;
    // printf("%d\n", exp->syntaxNum);

    // init place
    exp->place = tmpOperand();

    switch (exp->syntaxNum)
    {
    // Exp1 ASSIGNOP Exp2
    case 1:
        exp1 = exp->sons;
        exp2 = exp->sons->next->next;
        // translate exp2
        // Exp1 is ID (a = b)
        if (exp1->sons->mytype == MID) {
            // assign exp1
            if (isArray(exp1->type)) {
                realname1 = findAlias(exp1->sons->code);
                op1 = varOperand(realname1);
                // op2 = addressOperand(exp2->sons->code);
                realname2 = findAlias(exp2->sons->code);
                newTable = aliasTable(realname1, realname2);
                insertAliasTable(newTable);
                code = assignCode(op1, op1);
                result = interCodes(code);
            }
            else {
                result = translate_Exp(exp2);
                realname1 = findAlias(exp1->sons->code);
                op1 = varOperand(realname1);
                code = assignCode(op1, exp2->place);
                insertInterCode(result, code);
                // assign exp->place
                code = assignCode(exp->place, op1);
                insertInterCode(result, code);
            }
        }
        // Exp1 is array elem (a[0] = b) or structure
        else {
            result = translate_Exp(exp2);
            insertInterCodes(result, translate_Exp(exp1));
            assert(exp1->address != NULL);
            // *exp1 = exp2
            op1 = contentOperand(exp1->address->u.var_no);
            code = assignCode(op1, exp2->place);
            insertInterCode(result, code);
            // assign exp->place
            code = assignCode(exp->place, op1);
            insertInterCode(result, code);
        }
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
        exp1 = exp->sons;
        exp2 = exp1->next->next;
        // get exp1
        // t1 = &exp1
        op1 = tmpOperand();
        if (exp1->sons->mytype == MID) {
            id = exp1->sons;
            realname1 = findAlias(id->code);
            op2 = addressOperand(realname1);
            code = assignCode(op1, op2);
            result = interCodes(code);
        }
        else {
            result = translate_Exp(exp1);
            code = assignCode(op1, exp1->address);
            insertInterCode(result, code); 
        }
        // get j
        insertInterCodes(result, translate_Exp(exp2));
        // t2 = j * basicSize
        op2 = tmpOperand();
        basicSize = sizeofType(exp1->type->u.array.elem);
        op3 = constOperand(basicSize);
        code = binopCode(op2, op3, exp2->place, MULCODE);
        insertInterCode(result, code);
        // exp->address = t1 + t2
        exp->address = tmpOperand();
        code = binopCode(exp->address, op1, op2, ADDCODE);
        insertInterCode(result, code);
        // exp->place = *(exp->address)
        op1 = contentOperand(exp->address->u.var_no);
        code = assignCode(exp->place, op1);
        insertInterCode(result, code);
        break;
    // Exp DOT ID
    case 15:
        // TODO
        exp1 = exp->sons;
        id = exp1->next->next;
        assert(isStruct(exp1->type));
        fielist = exp1->type->u.structure;
        // if (funDef) {
        //     // t1 = &exp1
        //     op1 = tmpOperand();
        //     result = translate_Exp(exp1);
        //     if (exp1->sons->mytype == MID) {
        //         op2 = varOperand(exp1->sons->code);
        //     }
        //     else if (isArray(exp1->sons->type)) {
        //         op2 = exp1->address;
        //     }
        //     else {
        //         op2 = exp1->address;
        //     }
        //     code = assignCode(op1, op2);
        //     insertInterCode(result, code);
        //     // bias
        //     while (strcmp(fielist->name, id->code) != 0) {
        //         basicSize += sizeofType(fielist->type);
        //         fielist = fielist->tail;
        //     }
        //     // exp->address = t1 + bias
        //     exp->address = tmpOperand();
        //     op2 = constOperand(basicSize);
        //     code = binopCode(exp->address, op1, op2, ADDCODE);
        //     insertInterCode(result, code);
        //     // exp->place = *(exp->address)
        //     op1 = contentOperand(exp->address->u.var_no);
        //     code = assignCode(exp->place, op1);
        //     insertInterCode(result, code);
        // }
        // else {
            // t1 = &exp1
            op1 = tmpOperand();
            if (exp1->sons->mytype == MID) {
                if (isParam(exp1->sons->code)) {
                    op2 = varOperand(exp1->sons->code);
                }
                else {
                    op2 = addressOperand(exp1->sons->code);
                }
                code = assignCode(op1, op2);
                result = interCodes(code);
            }
            else if (isArray(exp1->sons->type)) {
                result = translate_Exp(exp1);
                code = assignCode(op1, exp1->address);
                insertInterCode(result, code);
            }
            // struct.struct
            else {
                result = translate_Exp(exp1);
                code = assignCode(op1, exp1->address);
                insertInterCode(result, code);
            }
            // bias
            while (strcmp(fielist->name, id->code) != 0) {
                basicSize += sizeofType(fielist->type);
                fielist = fielist->tail;
            }
            // exp->address = t1 + bias
            exp->address = tmpOperand();
            op2 = constOperand(basicSize);
            code = binopCode(exp->address, op1, op2, ADDCODE);
            insertInterCode(result, code);
            // exp->place = *(exp->address)
            op1 = contentOperand(exp->address->u.var_no);
            code = assignCode(exp->place, op1);
            insertInterCode(result, code);
        // }
        break;
    // ID
    case 16:
        realname1 = findAlias(exp->sons->code);
        op1 = varOperand(realname1);
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

InterCodes translate_StmtList(SyntaxTree stmtList) {
    InterCodes result = NULL;
    SyntaxTree stmt, stmtList1;
    // printf("%d\n", stmtList->syntaxNum);
    switch (stmtList->syntaxNum)
    {
    // Stmt StmtList
    case 1:
        stmt = stmtList->sons;
        stmtList1 = stmt->next;
        result = translate_Stmt(stmt);
        if (result == NULL) {
            result = translate_StmtList(stmtList1);
        }
        else {
            insertInterCodes(result, translate_StmtList(stmtList1));
        }
        break;
    // empty
    case 2:
        break;
    default:
        break;
    }
    return result;
}

InterCodes translate_CompSt(SyntaxTree compSt) {
    SyntaxTree stmtList;
    SymbolTable defList;
    InterCodes result = NULL;
    InterCode code;
    Operand op1, op2;
    int basicSize;
    switch (compSt->syntaxNum)
    {
    // LC DefList StmtList RC
    case 1:
        defList = compSt->sons->next->defList;
        stmtList = compSt->sons->next->next;
        // gen init definition
        while (defList != NULL) {
            basicSize = 4;
            // if (defList->specifier->kind == STRUCTURE) {
            // }
            if (isArray(defList->type)) {
                basicSize = sizeofType(defList->specifier);
                op1 = spaceOperand(basicSize * defList->type->u.array.size);
                op2 = varOperand(defList->name);
                code = decCode(op2, op1);
                if (result == NULL) {
                    result = interCodes(code);
                }
                else {
                    insertInterCode(result, code);
                }
            }
            else if (isStruct(defList->specifier)) {
                basicSize = sizeofType(defList->specifier);
                op1 = spaceOperand(basicSize);
                op2 = varOperand(defList->name);
                code = decCode(op2, op1);
                if (result == NULL) {
                    result = interCodes(code);
                }
                else {
                    insertInterCode(result, code);
                }
            }
            
            if (defList->hasInit) {
                if (result == NULL) {
                    result = translate_Exp(defList->initExp);
                }
                else {
                    insertInterCodes(result, translate_Exp(defList->initExp));
                }
                op1 = varOperand(defList->name);
                code = assignCode(op1, defList->initExp->place);
                insertInterCode(result, code);
            }
            defList = defList->next;
        }
        // translate stmtlist
        if (result == NULL) {
            result = translate_StmtList(stmtList);
        }
        else {
            insertInterCodes(result, translate_StmtList(stmtList));
        }
        break;
    default:
        break;
    }
    return result;
}

InterCodes translate_Stmt(SyntaxTree stmt) {
    InterCode code;
    InterCodes result = NULL;
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
        code = sinopCode(op3, LABLECODE);
        insertInterCode(result, code);
        break;
    default:
        break;
    }
    return result;
}

InterCodes translate_FunDec(SyntaxTree funDec) {
    InterCodes result = NULL;
    InterCode code;
    Operand op1;
    SyntaxTree id;
    SymbolTable varList;
    prTable = NULL;
    switch (funDec->syntaxNum)
    {
    // ID LP VarList RP
    case 1:
        id = funDec->sons;
        varList = funDec->sons->next->next->varList;
        op1 = varOperand(id->code);
        code = sinopCode(op1, FUNCTIONCODE);
        result = interCodes(code);  
        while (varList != NULL) {
            op1 = varOperand(varList->name);
            insertParamTable(varList->name);
            code = sinopCode(op1, PARAMCODE);
            insertInterCode(result, code);
            varList = varList->next;
        }
        break;
    // ID LP RP
    case 2:
        id = funDec->sons;
        op1 = varOperand(id->code);
        code = sinopCode(op1, FUNCTIONCODE);
        result = interCodes(code);
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
        translateCode(t->sons);
        t->intercodes = t->sons->intercodes;
        break;
    case MExtDefList:
        // ExtDef ExtDefList
        translateCode(t->sons);
        translateCode(t->sons->next);
        if (t->sons->intercodes == NULL) {
            t->intercodes = t->sons->next->intercodes;
        }
        else {
            t->intercodes = t->sons->intercodes;
            insertInterCodes(t->intercodes, t->sons->next->intercodes);
        }
        break;
    case MExtDef:
        switch (t->syntaxNum)
        {
        // Specifier ExtDecList SEMI
        case 1:
            assert(acceptGlobleVar == 0);
            break;
        // Specifier SEMI
        case 2:
            // translateCode(t->sons);
            // t->intercodes = t->sons->intercodes;
            break;
        // Specifier FunDec CompSt
        case 3:
            translateCode(t->sons->next);
            translateCode(t->sons->next->next);
            t->intercodes = t->sons->next->intercodes;
            insertInterCodes(t->intercodes, t->sons->next->next->intercodes);
            break;
        default:
            break;
        }
        break;
    case MExtDecList:
        assert(acceptGlobleVar == 0);
        break;
    case MSpecifier:
        break;
    case MStructSpecifier:
        break; 
    case MOptTag:
        break;  
    case MTag:
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
        t->intercodes = translate_FunDec(t);
        break;
    case MVarList:
        break;
    case MParamDec:
        break;
    case MCompSt:
        t->intercodes = translate_CompSt(t);
        break;
    case MStmtList:
        t->intercodes = translate_StmtList(t);
        break;
    case MStmt:
        t->intercodes = translate_Stmt(t);
        break;
    // defList 中会包含很多的 NULL Type （作为headSymbol）
    case MDefList:
        break; 
    case MDef:
        // switch (t->syntaxNum)
        // {
        // // Specifier DecList SEMI
        // case 1:
        //     break;
        // default:
        //     break;
        // }
        break;
    case MDecList:
        // switch (t->syntaxNum)
        // {
        // // Dec
        // case 1:
        //     break;
        // // Dec COMMA DecList
        // case 2:
        //     break;
        // default:
        //     break;
        // }
        break;
    case MDec:  
        // switch (t->syntaxNum)
        // {
        // // VarDec
        // case 1:
        //     break;
        // // VarDec ASSIGNOP Exp
        // case 2:
        //     break;
        // default:
        //     break;
        // }
        break;
    case MExp:
        t->intercodes = translate_Exp(t);
        break;
    case MArgs:
        break;
    default:
        break;
    }
}

void genInterCode(FILE* f) {
    alTable = aliasTable("", "");
    translateCode(tree);
    displayInterCodes(tree->intercodes, f);
}