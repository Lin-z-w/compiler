#include "SyntaxTree.h"

SyntaxTree tree;
int nodeIndex = 0;
const int MAX_NODES = 100000;

void initTree() {
    tree = malloc(sizeof(Node) * MAX_NODES);
    creatNode("!", MProgram, 0, "!", 1);
}

void initNode(int t, char* co, enum MYTYPE mtype, int loc, char* na, int synNum) {
    tree[t].parent = NULL;
    tree[t].next = NULL;
    tree[t].sons = NULL;

    
    tree[t].code = malloc(sizeof(char) * strlen(co)); strcpy(tree[t].code, co);
    tree[t].mytype = mtype;
    tree[t].location = loc;
    tree[t].name = malloc(sizeof(char) * strlen(na)); strcpy(tree[t].name, na);
    tree[t].syntaxNum = synNum;
    tree[t].initExp = NULL;
    tree[t].intercodes = NULL;
    tree[t].argList = NULL;
}

SyntaxTree creatNode(char* co, enum MYTYPE mtype, int loc, char* na, int synNum) {
    initNode(nodeIndex, co, mtype, loc, na, synNum);
    nodeIndex++;
    return tree + nodeIndex - 1;
}

void setNode(int t, char* co, enum MYTYPE mtype, int loc, char* na, int synNum) {
    tree[t].parent = NULL;
    tree[t].next = NULL;
    tree[t].sons = NULL;
    
    tree[t].code = malloc(sizeof(char) * strlen(co)); strcpy(tree[t].code, co);
    tree[t].mytype = mtype;
    tree[t].location = loc;
    tree[t].name = malloc(sizeof(char) * strlen(na)); strcpy(tree[t].name, na);
    tree[t].syntaxNum = synNum;
    tree[t].canBeLeftVal = 0;
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

void setType(SyntaxTree s, Type t) {
    s->type = t;
}

void displayNode(Node n, int deep) {
    if (n.mytype == MEMPTY) return;
    SyntaxTree sons = n.sons;
    for (int i = 0; i < deep; i++) {
        printf("  ");
    }
    printf("%s", n.name);
    switch (n.mytype)
    {
    case MID:
        printf(": %s", n.code);
        break;
    case MTYPE:
        printf(": %s", n.code);
        break;
    case MINT:
        printf(": %d", atoi(n.code));
        break;
    case MFLOAT:
        printf(": %f", atof(n.code));
        break;
    default:
        break;
    }
    printf("\n");
    while (sons != NULL) {
        displayNode(*sons, deep+1);
        sons = sons->next;
    }

}

void displayTree() {
    displayNode(tree[0], 0);
}