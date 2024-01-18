%{
    #define YYSTYPE SyntaxTree
    #include <stdlib.h>
    #include "lex.yy.c"
    #include <stdio.h>
    
    int syntaxWrong = 0;
    int errorType = 1;
    extern SyntaxTree tree;
    void yyerror(char* s);
%}

%token INT FLOAT ID
%token SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV 
%token AND OR DOT NOT TYPE LP RP LB RB LC RC 
%token STRUCT RETURN IF ELSE WHILE 
%token BAD

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%nonassoc LOWER_THAN_MINUS
%left PLUS 
%left STAR DIV
%right NOT MINUS
%left LB RB LP RP DOT

%%

Program : ExtDefList { setNode(0, "Program", MProgram, @$.first_line, "Program", 1); $$ = tree; setSons($$, $1); }
    ;
ExtDefList : ExtDef ExtDefList { $$ = creatNode("ExtDefList", MExtDefList, @$.first_line, "ExtDefList", 1); setSons($$, $1); setSons($$, $2); }
    | /* empty */ { $$ = creatNode("ExtDefList", MEMPTY, @$.first_line, "ExtDefList", 2); }
    ; 
ExtDef: Specifier ExtDecList SEMI { $$ = creatNode("ExtDef", MExtDef, @$.first_line, "ExtDef", 1); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Specifier SEMI { $$ = creatNode("ExtDef", MExtDef, @$.first_line, "ExtDef", 2); setSons($$, $1); setSons($$, $2); }
    | Specifier FunDec CompSt { $$ = creatNode("ExtDef", MExtDef, @$.first_line, "ExtDef", 3); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | error SEMI
    ;
ExtDecList : VarDec { $$ = creatNode("ExtDecList", MExtDecList, @$.first_line, "ExtDecList", 1); setSons($$, $1); }
    | VarDec COMMA ExtDecList { $$ = creatNode("ExtDecList", MExtDecList, @$.first_line, "ExtDecList", 2); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    ;

Specifier : TYPE { $$ = creatNode("Specifier", MSpecifier, @$.first_line, "Specifier", 1); setSons($$, $1); }
    | StructSpecifier { $$ = creatNode("Specifier", MSpecifier, @$.first_line, "Specifier", 2); setSons($$, $1); }
    ;
StructSpecifier : STRUCT OptTag LC DefList RC { $$ = creatNode("StructSpecifier", MStructSpecifier, @$.first_line, "StructSpecifier", 1);  setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); setSons($$, $5);}
    | STRUCT Tag { $$ = creatNode("StructSpecifier", MStructSpecifier, @$.first_line, "StructSpecifier", 2);  setSons($$, $1); setSons($$, $2);}
    | STRUCT OptTag LC error RC
    ; 
OptTag : ID { $$ = creatNode("OptTag", MOptTag, @$.first_line, "OptTag", 1);  setSons($$, $1);}
    | /* empty */ { $$ = creatNode("OptTag", MEMPTY, @$.first_line, "OptTag", 2); }
    ;  
Tag : ID { $$ = creatNode("Tag", MTag, @$.first_line, "Tag", 1);  setSons($$, $1);}
    ;

VarDec : ID { $$ = creatNode("VarDec", MVarDec, @$.first_line, "VarDec", 1);  setSons($$, $1);}
    | VarDec LB INT RB { $$ = creatNode("VarDec", MVarDec, @$.first_line, "VarDec", 2);  setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4);}
    ; 
FunDec : ID LP VarList RP { $$ = creatNode("FunDec", MFunDec, @$.first_line, "FunDec", 1);  setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4);}
    | ID LP RP { $$ = creatNode("FunDec", MFunDec, @$.first_line, "FunDec", 2); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | ID LP error RP
    ; 
VarList : ParamDec COMMA VarList { $$ = creatNode("VarList", MVarList, @$.first_line, "VarList", 1); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | ParamDec { $$ = creatNode("VarList", MVarList, @$.first_line, "VarList", 2); setSons($$, $1); }
    ; 
ParamDec : Specifier VarDec { $$ = creatNode("ParamDec", MParamDec, @$.first_line, "ParamDec", 1); setSons($$, $1); setSons($$, $2); }

CompSt : LC DefList StmtList RC { $$ = creatNode("CompSt", MCompSt, @$.first_line, "CompSt", 1); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); }
    | LC DefList error RC
    ;
StmtList : Stmt StmtList { $$ = creatNode("StmtList", MStmtList, @$.first_line, "StmtList", 1); setSons($$, $1); setSons($$, $2); }
    | /* empty */ { $$ = creatNode("StmtList", MEMPTY, @$.first_line, "StmtList", 2); }
    ; 
Stmt : Exp SEMI { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt", 1); setSons($$, $1); setSons($$, $2); }
    | CompSt { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt", 2); setSons($$, $1); }
    | RETURN Exp SEMI { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt", 3); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt", 4); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); setSons($$, $5); }
    | IF LP Exp RP Stmt ELSE Stmt { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt", 5); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); setSons($$, $5); setSons($$, $6); setSons($$, $7); }
    | WHILE LP Exp RP Stmt { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt", 6); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); setSons($$, $5); }
    | error SEMI 
    ;

DefList : Def DefList { $$ = creatNode("DefList", MDefList, @$.first_line, "DefList", 1); setSons($$, $1); setSons($$, $2); }
    | /* empty */ { $$ = creatNode("DefList", MEMPTY, @$.first_line, "DefList", 2); }
    ;  
Def : Specifier DecList SEMI { $$ = creatNode("Def", MDef, @$.first_line, "Def", 1); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Specifier error SEMI
    ;
DecList : Dec { $$ = creatNode("DecList", MDecList, @$.first_line, "DecList", 1); setSons($$, $1); }
    | Dec COMMA DecList { $$ = creatNode("DecList", MDecList, @$.first_line, "DecList", 2); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    ;
Dec : VarDec { $$ = creatNode("Dec", MDec, @$.first_line, "Dec", 1); setSons($$, $1); }
    | VarDec ASSIGNOP Exp { $$ = creatNode("Dec", MDec, @$.first_line, "Dec", 2); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    ;

Exp : Exp ASSIGNOP Exp { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 1); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp AND Exp  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 2); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp OR Exp  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 3); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp RELOP Exp { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 4); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp PLUS Exp  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 5); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp MINUS Exp %prec LOWER_THAN_MINUS { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 6); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp STAR Exp  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 7); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp DIV Exp  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 8); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | LP Exp RP { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 9); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | MINUS Exp { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 10); setSons($$, $1); setSons($$, $2); }
    | NOT Exp { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 11); setSons($$, $1); setSons($$, $2); }
    | ID LP Args RP { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 12); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); }
    | ID LP RP { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 13); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp LB Exp RB { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 14); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); }
    | Exp DOT ID  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 15); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | ID { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 16); setSons($$, $1); }
    | INT { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 17); setSons($$, $1); }
    | FLOAT { $$ = creatNode("Exp", MExp, @$.first_line, "Exp", 18); setSons($$, $1); }
    | Exp LB error RB
    | error RP
    ;
Args : Exp COMMA Args { $$ = creatNode("Args", MArgs, @$.first_line, "Args", 1); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp { $$ = creatNode("Args", MArgs, @$.first_line, "Args", 2); setSons($$, $1); }
    ;

%%

void yyerror(char* s) {
    if (errorType == 1) {
        printf("Error type B at Line %d: Syntax error\n", yylineno);
    }
    else {
        printf("Error type A at Line %d: Lexical error\n", yylineno); 
    }
    syntaxWrong = 1;
    errorType = 1;
}

