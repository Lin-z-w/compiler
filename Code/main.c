#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "semantics.h"
void yyrestart(FILE* f);
int yyparse();
void initTree();
extern int syntaxWrong;
extern int lexicalWrong;
extern int yydebug;

int main(int argc, char** argv)  { 
    if (argc <= 1) return 1; 
    FILE* f = fopen(argv[1], "r"); 
    if (!f) 
    { 
      perror(argv[1]); 
      return 1; 
    } 
    initTree();
    yyrestart(f); 
    yyparse(); 
    // displayTree();
    semanticsAnalysis();
    return 0;
}