#include "semantics.h"

#define tableNum 10

SymbolTable symTable[tableNum];
Type curReturnType; 
int tableTop = 0;

void analysisSons(SyntaxTree sons);
void analysis(SyntaxTree t);
void insertSymTable(char* name, Type t, int location, enum SymbolKind k, SymbolTable args);

void semanticsError(int type, int location, char* msg) {
    printf("Error type %d at Line %d: %s\n", type, location, msg);
}

void initTable() {
    symTable[0] = headSymbol();
    SymbolTable args = symbolTable("x", intType);
    insertSymTable("read", intType, 0, FUNSYM, NULL);
    insertSymTable("write", intType, 0, FUNSYM, args);
}

void increaseTableTop() {
    tableTop++;
    symTable[tableTop] = headSymbol();
}

void decreaseTableTop() {
    symTable[tableTop] = NULL;
    tableTop--;
}

SymbolTable searchCurTable(char* name) {
    return findSymbol(symTable[tableTop], name);
}

SymbolTable searchAllTable(char* name) {
    SymbolTable tmp;
    int curTop = tableTop;
    while (curTop >= 0) {
        tmp = findSymbol(symTable[curTop], name);
        if (tmp != NULL) return tmp;
        curTop--;
    }
    return NULL;
}

void insertSymTable(char* name, Type t, int location, enum SymbolKind k, SymbolTable args) {
    // if (strcmp(name, "") == 0) return ;
    if (searchCurTable(name) == NULL) {
        insertTable(symTable[tableTop], name, t, args, k);
    }
    else {
        switch (k)
        {
        case VARSYM:
            semanticsError(3, location, "line 55");
            break;
        case STRUCTSYM:
            semanticsError(16, location, "line 58");
            break;
        case FUNSYM:
            semanticsError(4, location, "line 61");
            break;
        default:
            break;
        }
    }
}

void analysisSons(SyntaxTree sons) {
    while (sons != NULL) {
        analysis(sons);
        sons = sons->next;
    }
}

void analysis(SyntaxTree t) {
    SyntaxTree sons = t->sons;
    SymbolTable funSymbol, varSymbol, structSymbol, decList, defList, varList, args, funArgs;
    Type newType, expType, fieldType;
    int argsError = 0;
    int structError = 0;

    switch (t->mytype)
    {
    case MProgram:
        analysisSons(sons);
        // ExtDefList
        break;
    case MExtDefList:
        analysisSons(sons);
        // ExtDef ExtDefList
        break;
    case MExtDef:
        switch (t->syntaxNum)
        {
        // Specifier ExtDecList SEMI
        // ExtDecList 中全部未赋值
        case 1:
            analysisSons(sons);
            decList = sons->next->decList; 
            while (decList != NULL) {
                switch (decList->type->kind)
                {
                case TBD:
                    insertSymTable(decList->name, sons->type, decList->location, VARSYM, NULL);
                    break;
                case ARRAY:
                    newType = decList->type;
                    // find the end of array elem
                    while (isArray(newType->u.array.elem)) {
                        newType = newType->u.array.elem;
                    }
                    newType->u.array.elem = sons->type;
                    insertSymTable(decList->name, decList->type, decList->location, VARSYM, NULL);
                    break;
                default:
                    break;
                }
                decList = decList->next;
            }
            break;
        // Specifier SEMI
        case 2:
            analysisSons(sons);
            break;
        // Specifier FunDec CompSt
        case 3:
            analysis(sons);
            analysis(sons->next);
            // insert funSymbol into table
            // insert varList into next table
            varList = sons->next->varList;
            insertSymTable(sons->next->code, sons->type, t->location, FUNSYM, varList);
            increaseTableTop();
            curReturnType = sons->type;
            while (varList != NULL) {
                insertSymTable(varList->name, varList->type, varList->location, VARSYM, NULL);
                varList = varList->next;
            }
            analysis(sons->next->next);
            decreaseTableTop();
            break;
        default:
            break;
        }
        break;
    case MExtDecList:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // VarDec
        case 1:
            t->decList = symbolTable(sons->code, sons->type);
            t->decList->location = sons->location;
            break;
        // VarDec COMMA ExtDecList
        case 2:
            t->decList = symbolTable(sons->code, sons->type);
            t->decList->location = sons->location;
            t->decList->next = sons->next->next->decList;
            break;
        default:
            break;
        }
        break;
    case MSpecifier:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // TYPE
        case 1:
            if (strcmp(sons->code, "int") == 0) {
                t->type = intType;
            }
            else {
                t->type = floatType;
            }
            break;
        // StructSpecifier
        case 2:
            t->type = sons->type;
            break;
        default:
            break;
        }
        break;
    case MStructSpecifier:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // STRUCT OptTag LC DefList RC
        case 1:
            newType = malloc(sizeof(struct Type_));
            newType->kind = STRUCTURE;
            // redefine field or init field
            defList = sons->next->next->next->defList;
            newType->u.structure = headField();
            while (defList != NULL) {
                if (defList->type != NULL) {
                    if (findStructField(newType, defList->name) != NULL) {
                        semanticsError(15, defList->location, "line 209");
                        structError = 1;
                    }
                    if (defList->hasInit == 0) {
                        switch (defList->type->kind)
                        {
                        case TBD:
                            defList->type = defList->specifier;
                            break;
                        case ARRAY:
                            fieldType = defList->type;
                            while (isArray(fieldType->u.array.elem)) {
                                fieldType = fieldType->u.array.elem;
                            }
                            fieldType->u.array.elem = defList->specifier;
                            break;
                        default:
                            break;
                        }
                        insertField(newType->u.structure, defList->name, defList->type);
                    }
                    else {
                        semanticsError(15, defList->location, "line 216");
                        structError = 1;
                    }
                }
                defList = defList->next;
            }
            if (sons->next->code != NULL && !structError) {
                newType->u.structure->name = malloc(strlen(sons->next->code));
                strcpy(newType->u.structure->name, sons->next->code);
                insertSymTable(sons->next->code, newType, t->location, STRUCTSYM, NULL);
            }
            if (structError) {
                t->type = NULL;
            }
            else {
                t->type = newType;
            }
            break;
        // STRUCT Tag
        case 2:
            structSymbol = searchAllTable(sons->next->code);
            if (structSymbol != NULL) {
                t->type = structSymbol->type;
            }
            else {
                t->type = NULL;
                semanticsError(17, t->location, "line 264");
            }
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
            t->code = sons->code;
            break;
        // empty
        case 2:
            t->code = NULL;
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
            t->code = sons->code;
            break;
        default:
            break;
        }
        break;
    case MVarDec:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // ID
        case 1:
            t->code = sons->code;
            t->type = tbdType;
            break;
        // VarDec LB INT RB
        case 2:
            t->code = sons->code;
            t->type = malloc(sizeof(struct Type_));
            t->type->kind = ARRAY;
            t->type->u.array.elem = sons->type;
            t->type->u.array.size = atoi(sons->next->next->code);
            break;
        default:
            break;
        }
        break;
    case MFunDec:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // ID LP VarList RP
        case 1:
            t->code = sons->code;
            t->varList = sons->next->next->varList;
            break;
        // ID LP RP
        case 2:
            t->code = sons->code;
            t->varList = NULL;
            break;
        default:
            break;
        }
        break;
    case MVarList:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // ParamDec COMMA VarList
        case 1:
            t->varList = symbolTable(sons->code, sons->type);
            assert(!isArray(sons->type));
            t->varList->location = sons->location;
            t->varList->next = sons->next->next->varList;
            break;
        // ParamDec
        case 2:
            t->varList = symbolTable(sons->code, sons->type);
            assert(!isArray(sons->type));
            t->varList->location = sons->location;
            break;
        default:
            break;
        }
        break;
    case MParamDec:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // Specifier VarDec
        case 1:
            // param name
            t->code = sons->next->code;
            // param type
            newType = sons->next->type;
            if (isTBD(newType)) {
                t->type = sons->type;
            }
            else {
                while(isArray(newType->u.array.elem)) {
                    newType = newType->u.array.elem;
                }
                newType->u.array.elem = sons->type;
                t->type = sons->next->type;
            }
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
            analysis(sons->next);
            defList = sons->next->defList;
            while (defList != NULL) {
                if (defList->type != NULL) {
                    // init part of definaiton
                    if (defList->hasInit) {
                        analysis(defList->initExp);
                        expType = defList->initExp->type;
                        if (isTBD(defList->type)) {
                            defList->type = expType;
                        }
                        else {
                            if (dimensionEqual(defList->type, expType)) {
                                defList->type = expType;
                            }
                            else {
                                defList->type = NULL;
                                semanticsError(5, defList->location, "line 379");
                            }
                        }
                        if (defList->type == NULL) {
                            defList = defList->next;
                            continue;
                        }
                    }
                    switch (defList->type->kind)
                    {
                    case TBD:
                        insertSymTable(defList->name, defList->specifier, defList->location, VARSYM, NULL);
                        break;
                    case BASIC:
                    case STRUCTURE:
                        if (typeEqual(defList->type, defList->specifier)) {
                            insertSymTable(defList->name, defList->type, defList->location, VARSYM, NULL);
                        }
                        else {
                            semanticsError(5, defList->location, "line 399");
                        }
                        break;
                    case ARRAY:
                        newType = defList->type;
                        // find the type of array elem
                        assert(!isArray(newType->u.array.elem));
                        while (isArray(newType->u.array.elem)) {
                            newType = newType->u.array.elem;
                        }
                        if (isTBD(newType->u.array.elem)) {
                            newType->u.array.elem = defList->specifier;
                            insertSymTable(defList->name, defList->type, defList->location, VARSYM, NULL);
                        }
                        else {
                            if (typeEqual(defList->specifier, newType->u.array.elem)) {
                                insertSymTable(defList->name, defList->type, defList->location, VARSYM, NULL);
                            }
                            else {
                                semanticsError(5, defList->location, "line 502");
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                defList = defList->next;
            }
            analysis(sons->next->next);
            break;
        default:
            break;
        }
        break;
    case MStmtList:
        analysisSons(sons);
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
            analysisSons(sons);
            break;
        // CompSt
        case 2:
            increaseTableTop();
            analysisSons(sons);
            decreaseTableTop();
            break;
        // RETURN Exp SEMI
        case 3:
            analysisSons(sons);
            if (!typeEqual(sons->next->type, curReturnType)) {
                semanticsError(8, t->location, "line 420");
            }
            break;
        // IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
        case 4:
            analysisSons(sons);
            if (!isInt(sons->next->next->type)) {
                semanticsError(7, sons->next->next->location, "line 471");
            }
            break;
        // IF LP Exp RP Stmt ELSE Stmt
        case 5:
            analysisSons(sons);
            if (!isInt(sons->next->next->type)) {
                semanticsError(7, sons->next->next->location, "line 478");
            }
            break;
        // WHILE LP Exp RP Stmt
        case 6:
            analysisSons(sons);
            if (!isInt(sons->next->next->type)) {
                semanticsError(7, sons->next->next->location, "line 485");
            }
            break;
        default:
            break;
        }
        break;
    // defList 中会包含很多的 NULL Type （作为headSymbol）
    case MDefList:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // Def DefList
        case 1:
            t->defList = sons->defList;
            defList = t->defList;
            while (defList->next != NULL) {
                defList = defList->next;
            }
            defList->next = sons->next->defList;
            break;
        // empty
        case 2:
            t->defList = NULL;
            break;
        default:
            break;
        }
        break; 
    case MDef:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // Specifier DecList SEMI
        case 1:
            decList = sons->next->decList;
            t->defList = headSymbol();
            while (decList != NULL) {
                if ((decList->initExp == NULL)) {
                    insertDefList(t->defList, decList->name, decList->type, decList->location, 0, decList->initExp, sons->type);
                }
                else {
                    insertDefList(t->defList, decList->name, decList->type, decList->location, 1, decList->initExp, sons->type);
                }
                decList = decList->next;
            }
            break;
        default:
            break;
        }
        break;
    case MDecList:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // Dec
        case 1:
            t->decList = symbolTable(sons->code, sons->type);
            t->decList->initExp = sons->initExp;
            t->decList->location = sons->location;
            break;
        // Dec COMMA DecList
        case 2:
            t->decList = symbolTable(sons->code, sons->type);
            t->decList->initExp = sons->initExp;
            t->decList->location = sons->location;
            t->decList->next = sons->next->next->decList;
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
            analysis(sons);
            t->code = sons->code;
            t->type = sons->type;
            t->initExp = NULL;
            break;
        // VarDec ASSIGNOP Exp
        case 2:
            analysis(sons);
            t->code = sons->code;
            t->type = sons->type;
            t->initExp = sons->next->next;
            break;
        default:
            break;
        }
        break;
    case MExp:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // Exp ASSIGNOP Exp
        case 1:
            if (sons->canBeLeftVal == 0) {
                semanticsError(6, t->location, "line 576");
            }
            if (!typeEqual(sons->type, sons->next->next->type)) {
                t->type = NULL;
                semanticsError(5, t->location, "line 580");
            }
            else {
                t->type = sons->type;
            }
            break;
        // Exp AND Exp 
        case 2:
            t->type = sons->type;
            if (!isInt(sons->type) || !isInt(sons->next->next->type)) {
                semanticsError(7, t->location, "line 578");
            }
            break;
        // Exp OR Exp
        case 3:
            t->type = sons->type;
            if (!isInt(sons->type) || !isInt(sons->next->next->type)) {
                semanticsError(7, t->location, "line 585");
            }
            break;
        // Exp RELOP Exp
        case 4:
            t->type = sons->type;
            if (!typeEqual(sons->type, sons->next->next->type) || !isBasicType(sons->type) || !isBasicType(sons->next->next->type)) {
                semanticsError(7, t->location, "line 592");
            }
            break;
        // Exp PLUS Exp
        case 5:
            t->type = sons->type;
            if (!typeEqual(sons->type, sons->next->next->type) || !isBasicType(sons->type) || !isBasicType(sons->next->next->type)) {
                semanticsError(7, t->location, "line 599");
            }
            break;
        // Exp MINUS Exp %prec LOWER_THAN_MINUS 
        case 6:
            t->type = sons->type;
            if (!typeEqual(sons->type, sons->next->next->type) || !isBasicType(sons->type) || !isBasicType(sons->next->next->type)) {
                semanticsError(7, t->location, "line 606");
            }
            break;
        // Exp STAR Exp 
        case 7:
            t->type = sons->type;
            if (!typeEqual(sons->type, sons->next->next->type) || !isBasicType(sons->type) || !isBasicType(sons->next->next->type)) {
                semanticsError(7, t->location, "line 613");
            }
            break;
        // Exp DIV Exp
        case 8:
            t->type = sons->type;
            if (!typeEqual(sons->type, sons->next->next->type) || !isBasicType(sons->type) || !isBasicType(sons->next->next->type)) {
                semanticsError(7, t->location, "line 620");
            }
            break;
        // LP Exp RP
        case 9:
            t->type = sons->next->type;
            break;
        // MINUS Exp
        case 10:
            t->type = sons->next->type;
            if (!isBasicType(sons->next->type)) {
                semanticsError(7, t->location, "line 631");
            }
            break;
        // NOT Exp
        case 11:
            t->type = sons->next->type;
            if (!isInt(sons->next->type)) {
                semanticsError(7, t->location, "line 638");
            }
            break;
        // ID LP Args RP
        case 12:
            funSymbol = searchAllTable(sons->code);
            if (funSymbol == NULL) {
                t->type = NULL;
                semanticsError(2, t->location, "line 650");
            }
            else {
                if (funSymbol->kind != FUNSYM) {
                    semanticsError(11, t->location, "line 644");
                }
                else {
                    args = sons->next->next->args;
                    funArgs = funSymbol->args;
                    while (args != NULL && funArgs != NULL) {
                        if (!typeEqual(args->type, funArgs->type)) {
                            argsError = 1;
                            break;
                        }
                        args = args->next;
                        funArgs = funArgs->next;
                    }
                    if (argsError) {
                        t->type = NULL;
                        semanticsError(9, t->location, "Args Type Wrong");
                    }
                    else if (args != NULL || funArgs != NULL){
                        t->type = NULL;
                        semanticsError(9, t->location, "Args Num Wrong");
                    }
                    else {
                        t->type = funSymbol->type;
                    }
                }
            }
            break;
        // ID LP RP
        case 13:              
            funSymbol = searchAllTable(sons->code);
            if (funSymbol == NULL) {
                t->type = NULL;
                semanticsError(2, t->location, "line 680");
            }
            else {
                if (funSymbol->kind != FUNSYM) {
                    semanticsError(11, t->location, "line 674");
                }
                else {
                    t->type = funSymbol->type;
                }
            }
            break;
        // Exp LB Exp RB
        case 14:
            t->canBeLeftVal = 1;
            if (!isInt(sons->next->next->type)) {
                semanticsError(12, t->location, "line 690");
            }
            else {
                if (isArray(sons->type)) {
                    t->type = sons->type->u.array.elem;
                }
                else {
                    semanticsError(10, t->location, "line 697");
                }
            }
            break;
        // Exp DOT ID
        case 15:
            newType = sons->type;
            t->canBeLeftVal = 1;
            if (!isStruct(newType)) {
                semanticsError(13, t->location, "line 705");
            }
            else {
                Type field = findStructField(newType, sons->next->next->code);
                if (field == NULL) {
                    t->type = NULL;
                    semanticsError(14, t->location, "line 711");
                }
                else {
                    t->type = field;
                }
            }
            break;
        // ID
        case 16:
            varSymbol = searchAllTable(sons->code);
            t->canBeLeftVal = 1;
            if (varSymbol == NULL || varSymbol->kind != VARSYM) {
                t->type = NULL;
                semanticsError(1, t->location, "line 723");
            }
            else {
                t->type = varSymbol->type;
            }
            break;
        // INT
        case 17:
            t->type = intType;
            break;
        // FLOAT
        case 18:
            t->type = floatType;
            break;
        default:
            break;
        }
        break;
    case MArgs:
        analysisSons(sons);
        switch (t->syntaxNum)
        {
        // Exp COMMA Args
        case 1:
            t->args = symbolTable("", sons->type);
            t->args->next = sons->next->next->args;
            break;
        // Exp
        case 2:
            t->args = symbolTable("", sons->type);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void semanticsAnalysis() {
    initStaticType();
    initTable();
    analysis(tree);
}