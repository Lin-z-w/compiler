#include "SymbolTable.h"

Type intType, floatType, tbdType;


SymbolTable symbolTable(char* name, Type t) {
    SymbolTable s = malloc(sizeof(struct SymbolTable_));
    s->name = malloc(strlen(name));
    strcpy(s->name, name);
    s->type = t;
    s->next = NULL;
    return s;
}

SymbolTable headSymbol() {
    return symbolTable("", NULL);
}

FieldList fieldList(char* name, Type t) {
    FieldList f = malloc(sizeof(struct FieldList_));
    f->name = malloc(strlen(name));
    strcpy(f->name, name);
    f->type = t;
    f->tail = NULL;
}

FieldList headField() {
    return fieldList("", NULL);
}

void insertTable(SymbolTable s, char* name, Type t, SymbolTable args, enum SymbolKind k) {
    SymbolTable next = symbolTable(name, t);
    next->args = args;
    next->kind = k;
    while (s->next != NULL) {
        s = s->next;
    }
    s->next = next;
}

void insertField(FieldList f, char* name, Type t) {
    FieldList tail = fieldList(name, t);
    while (f->tail != NULL) {
        f = f->tail;
    }
    f->tail = tail;
}

// void insertTableFun(SymbolTable s, char* name, Type t, SymbolTable args) {
//     SymbolTable next = symbolTable(name, t);
//     next->args = args;
//     while (s->next != NULL) {
//         s = s->next;
//     }
//     s->next = next;
// }

void insertDefList(SymbolTable s, char* name, Type t, int location, int hasInit, struct inode* initExp, Type sp) {
    SymbolTable next = symbolTable(name, t);
    next->location = location;
    next->hasInit = hasInit;
    next->initExp = initExp;
    next->specifier = sp;
    while (s->next != NULL) {
        s = s->next;
    }
    s->next = next;
}

SymbolTable findSymbol(SymbolTable s, char* name) {
    while (s != NULL) {
        if (strcmp(s->name, name) == 0) {
            return s;
        }
        s = s->next;
    }
    return NULL;
}

Type findStructField(Type t, char* name) {
    if (t->kind != STRUCTURE) exit(2);
    FieldList fl = t->u.structure;
    while (fl != NULL) {
        if (strcmp(fl->name, name) == 0) {
            return fl->type;
        }
        fl = fl->tail;
    }
    return NULL;
}

void initStaticType() {
    intType = malloc(sizeof(struct Type_));
    // printf("SB");
    intType->kind = BASIC;
    intType->u.basic = 0;

    floatType = malloc(sizeof(struct Type_));
    floatType->kind = BASIC;
    floatType->u.basic = 1;
    
    tbdType = malloc(sizeof(struct Type_));
    tbdType->kind = TBD;
}

int typeEqual(Type t1, Type t2) {
    int result;
    if (t1 == NULL || t2 == NULL) {
        return 0;
    }
    if (t1->kind != t2->kind) {
        return 0;
    }
    switch (t1->kind)
    {
    case BASIC:
        result = t1->u.basic == t2->u.basic;
        break;
    case ARRAY:
        result = typeEqual(t1->u.array.elem, t2->u.array.elem);
        break;
    case STRUCTURE:
        result = (strcmp(t1->u.structure->name, t2->u.structure->name) == 0);
        break;
    default:
        break;
    }
    return result;
}

int dimensionEqual(Type t1, Type t2) {
    if (t1 == NULL || t2 == NULL) return 0;
    if (t1->kind != ARRAY || t2->kind != ARRAY) 
        return 0;
    while (t1->kind == ARRAY && t2->kind == ARRAY)
    {
        t1 = t1->u.array.elem;
        t2 = t2->u.array.elem;
    }
    return t1->kind != ARRAY && t2->kind != ARRAY;
}

int isBasicType(Type t) {
    if (t == NULL) return 0;
    return t->kind == BASIC;
}

int isInt(Type t) {
    if (t == NULL) return 0;
    return isBasicType(t) && t->u.basic == 0;
}

int isStruct(Type t) {
    if (t == NULL) return 0;
    return t->kind == STRUCTURE;
}

int isTBD(Type t) {
    if (t == NULL) return 0;
    return t->kind == TBD;
}

int isArray(Type t) {
    if (t == NULL) return 0;
    return t->kind == ARRAY;
}

int isTBDorTBDArray(Type t) {
    if (t == NULL) return 0;
    if (isTBD(t)) {
        return 1;
    }
    else if (isArray(t)) {
        while (isArray(t)) {
            t = t->u.array.elem;
        }
        return isTBD(t);
    }
    else {
        return 0;
    }
}

int sizeofType(Type t) {
    if (t == NULL) return 0;
    int result = 0;
    FieldList fl;
    switch (t->kind)
    {
    case BASIC:
        result = 4;
        break;
    case ARRAY:
        result = t->u.array.size * sizeofType(t->u.array.elem);
        break;
    case STRUCTURE:
        fl = t->u.structure;
        while (fl != NULL)
        {
            result += sizeofType(fl->type);
            fl = fl->tail;
        }
        break;
    default:
        break;
    }
    return result;
}
