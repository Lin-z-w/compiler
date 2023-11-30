#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "test.h"
void yyrestart(FILE* f);
int yyparse();
void initTree();
extern int syntaxWrong;
extern int lexicalWrong;
extern int yydebug;

int main(int argc, char** argv)  { 
    if (argc <= 1) return 1; 
    FILE* rf = fopen(argv[1], "r"); 
    if (!rf) 
    { 
      perror(argv[1]); 
      return 1; 
    } 
    initTree();
    yyrestart(rf); 
    yyparse(); 
    // displayTree();
    semanticsAnalysis();
    // FILE* wf = fopen(argv[2], "w");
    freopen(argv[2], "w", stdout);
    genInterCode();
    fclose(rf);
    return 0;
}

// test
// int main() {
//   testInterCodes();
// }