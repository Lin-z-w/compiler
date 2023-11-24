#include <stddef.h>
#include <stdlib.h>
#include "SymbolTable.h"

enum MYTYPE {
    MEMPTY,
    MINT,
    MFLOAT, 
    MID,
    MSEMI, 
    MCOMMA, 
    MASSIGNOP, 
    MRELOP, 
    MPLUS, 
    MMINUS, 
    MSTAR, 
    MDIV, 
    MAND, 
    MOR, 
    MDOT, 
    MNOT, 
    MTYPE, 
    MLP, 
    MRP, 
    MLB, 
    MRB, 
    MLC, 
    MRC, 
    MSTRUCT, 
    MRETURN,
    MIF, 
    MELSE, 
    MWHILE,
    MProgram,
    MExtDefList,
    MExtDef,
    MExtDecList,
    MSpecifier,
    MStructSpecifier, 
    MOptTag,  
    MTag,
    MVarDec,
    MFunDec,
    MVarList,
    MParamDec,
    MCompSt,
    MStmtList,
    MStmt,
    MDefList, 
    MDef,
    MDecList,
    MDec,
    MExp,
    MArgs
} ;

typedef struct inode
{
    char* name;
    enum MYTYPE mytype;
    int location;
    int syntaxNum;
    struct inode *parent, *sons, *next;

    // info get in semantics
    int canBeLeftVal;
    char* code;
    Type type;
    SymbolTable defList, decList, varList, args;
    struct inode *initExp;
} Node;

typedef Node* SyntaxTree;

void initTree();
SyntaxTree creatNode(char* co, enum MYTYPE mtype, int loc, char* na, int synNum);
void initNode(int t, char* co, enum MYTYPE mtype, int loc, char* na, int synNum);
void setNode(int t, char* co, enum MYTYPE mtype, int loc, char* na, int synNum);
void setSons(SyntaxTree p, SyntaxTree s);
void setType(SyntaxTree s, Type t);
void displayTree();
void displayNode(Node n, int deep);

extern SyntaxTree tree;
