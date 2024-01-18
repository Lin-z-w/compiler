#include "SyntaxTree.h"

SyntaxTree tree;
int nodeIndex = 0;
const int MAX_NODES = 100000;

void initTree() {
    tree = malloc(sizeof(Node) * MAX_NODES);
    creatNode("!", MProgram, 0, "!");
}

void initNode(int t, char* v, enum MYTYPE mtype, int loc, char* na) {
    tree[t].val = malloc(sizeof(char) * strlen(v));
    strcpy(tree[t].val, v);
    tree[t].type = mtype;
    tree[t].parent = NULL;
    tree[t].next = NULL;
    tree[t].sons = NULL;
    tree[t].location = loc;
    tree[t].name = malloc(sizeof(char) * strlen(na));
    strcpy(tree[t].name, na);
}

SyntaxTree creatNode(char* v, enum MYTYPE mtype, int loc, char* na) {
    initNode(nodeIndex, v, mtype, loc, na);
    nodeIndex++;
    return tree + nodeIndex - 1;
}

void setNode(int t, char* v, enum MYTYPE mtype, int loc, char* na) {
    tree[t].val = malloc(sizeof(char) * strlen(v));
    strcpy(tree[t].val, v);
    tree[t].type = mtype;
    tree[t].parent = NULL;
    tree[t].next = NULL;
    tree[t].sons = NULL;
    tree[t].location = loc;
    tree[t].name = malloc(sizeof(char) * strlen(na));
    strcpy(tree[t].name, na);
}

void setSons(SyntaxTree p, SyntaxTree s) {
    if (p->sons == NULL) 
        p->sons = s;
    else {
        SyntaxTree sons = p->sons;
        while (sons->next != NULL) {
            sons = sons->next;
        }
        sons->next = s;
        s->parent = p;
    }
}

void displayTree() {
    displayNode(tree[0], 0);
}

void displayNode(Node n, int deep) {
    if (n.type == MEMPTY) return;
    for (int i = 0; i < deep; i++) {
        printf("  ");
    }
    printf("%s", n.name);
    switch (n.type)
    {
    case MID:
        printf(": %s", n.val);
        break;
    case MTYPE:
        printf(": %s", n.val);
        break;
    case MINT:
        printf(": %d", atoi(n.val));
        break;
    case MFLOAT:
        printf(": %f", atof(n.val));
        break;
    case MProgram:
    case MExtDefList:
    case MExtDef:
    case MExtDecList:
    case MSpecifier:
    case MStructSpecifier:
    case MOptTag:
    case MTag:
    case MVarDec:
    case MFunDec:
    case MVarList:
    case MParamDec:
    case MCompSt:
    case MStmtList:
    case MStmt:
    case MDefList:
    case MDef:
    case MDecList:
    case MDec:
    case MExp:
    case MArgs:
        printf(" (%d)", n.location);
    default:
        break;
    }

    printf("\n");

    SyntaxTree sons = n.sons;
    while (sons != NULL) {
        displayNode(*sons, deep+1);
        sons = sons->next;
    }
}