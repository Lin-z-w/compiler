#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    char* val;
    char* name;
    enum MYTYPE type;
    int location;
    struct inode *parent, *sons, *next;
} Node;

typedef Node* SyntaxTree;

void initTree();
SyntaxTree creatNode(char* v, enum MYTYPE mtype, int loc, char* na);
void initNode(int t, char* v, enum MYTYPE mtype, int loc, char* na);
void setNode(int t, char* v, enum MYTYPE mtype, int loc, char* na);
void setSons(SyntaxTree p, SyntaxTree s);
void displayTree();
void displayNode(Node n, int deep);

