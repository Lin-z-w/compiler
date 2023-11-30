#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct SymbolTable_* SymbolTable;
struct inode;

enum TypeKind{ BASIC, ARRAY, STRUCTURE, TBD };
enum SymbolKind { VARSYM, STRUCTSYM, FUNSYM };

struct Type_
{
    enum TypeKind kind;
    union
    {
        // 基本类型 0 int, 1 float
        int basic;
        // 数组类型信息包括元素类型与数组大小构成
        struct { Type elem; int size; } array;
        // 结构体类型信息是一个链表
        FieldList structure;
    } u;
};

struct FieldList_
{
    char* name; // 域的名字
    Type type; // 域的类型. 结构体的链表表示示例。
    FieldList tail; // 下一个域
};

struct SymbolTable_
{
    int location;
    int hasInit;
    int size;
    char* name; // 符号名称 e.g. 变量名、函数名
    Type type; 
    Type specifier;
    SymbolTable args;
    SymbolTable next;
    struct inode* initExp;
    enum SymbolKind kind;
} ;

// find symbol in table
SymbolTable findSymbol(SymbolTable s, char* name);
Type findStructField(Type t, char* name);

// modify table
void insertTable(SymbolTable s, char* name, Type t, SymbolTable args, enum SymbolKind k);
// void insertTableFun(SymbolTable s, char* name, Type t, SymbolTable args);
void insertDefList(SymbolTable s, char* name, Type t, int location, int init, struct inode* initExp, Type sp);
void insertField(FieldList f, char* name, Type t);

// construct
SymbolTable symbolTable(char* name, Type t);
SymbolTable headSymbol();
FieldList fieldList(char* name, Type t);
FieldList headField();
void initStaticType();

// judge type
int isBasicType(Type t);
int isStruct(Type t);
int isInt(Type t);
int isTBD(Type t);
int isArray(Type t);
int isTBDorTBDArray(Type t);
int typeEqual(Type t1, Type t2);
int dimensionEqual(Type t1, Type t2);

int sizeofType(Type t);

extern Type intType, floatType, tbdType;