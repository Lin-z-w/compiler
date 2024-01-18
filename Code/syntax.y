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

Program : ExtDefList { setNode(0, "Program", MProgram, @$.first_line, "Program"); $$ = tree; setSons($$, $1); }
    ;
ExtDefList : ExtDef ExtDefList { $$ = creatNode("ExtDefList", MExtDefList, @$.first_line, "ExtDefList"); setSons($$, $1); setSons($$, $2); }
    | /* empty */ { $$ = creatNode("ExtDefList", MEMPTY, @$.first_line, "ExtDefList"); }
    ; 
ExtDef: Specifier ExtDecList SEMI { $$ = creatNode("ExtDef", MExtDef, @$.first_line, "ExtDef"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Specifier SEMI { $$ = creatNode("ExtDef", MExtDef, @$.first_line, "ExtDef"); setSons($$, $1); setSons($$, $2); }
    | Specifier FunDec CompSt { $$ = creatNode("ExtDef", MExtDef, @$.first_line, "ExtDef"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | error SEMI
    ;
ExtDecList : VarDec { $$ = creatNode("ExtDecList", MExtDecList, @$.first_line, "ExtDecList"); setSons($$, $1); }
    | VarDec COMMA ExtDecList { $$ = creatNode("ExtDecList", MExtDecList, @$.first_line, "ExtDecList"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    ;

Specifier : TYPE { $$ = creatNode("Specifier", MSpecifier, @$.first_line, "Specifier"); setSons($$, $1); }
    | StructSpecifier { $$ = creatNode("Specifier", MSpecifier, @$.first_line, "Specifier"); setSons($$, $1); }
    ;
StructSpecifier : STRUCT OptTag LC DefList RC { $$ = creatNode("StructSpecifier", MStructSpecifier, @$.first_line, "StructSpecifier");  setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); setSons($$, $5);}
    | STRUCT Tag { $$ = creatNode("StructSpecifier", MStructSpecifier, @$.first_line, "StructSpecifier");  setSons($$, $1); setSons($$, $2);}
    | STRUCT OptTag LC error RC
    ; 
OptTag : ID { $$ = creatNode("OptTag", MOptTag, @$.first_line, "OptTag");  setSons($$, $1);}
    | /* empty */ { $$ = creatNode("OptTag", MEMPTY, @$.first_line, "OptTag"); }
    ;  
Tag : ID { $$ = creatNode("Tag", MTag, @$.first_line, "Tag");  setSons($$, $1);}
    ;

VarDec : ID { $$ = creatNode("VarDec", MVarDec, @$.first_line, "VarDec");  setSons($$, $1);}
    | VarDec LB INT RB { $$ = creatNode("VarDec", MVarDec, @$.first_line, "VarDec");  setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4);}
    ; 
FunDec : ID LP VarList RP { $$ = creatNode("FunDec", MFunDec, @$.first_line, "FunDec");  setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4);}
    | ID LP RP { $$ = creatNode("FunDec", MFunDec, @$.first_line, "FunDec"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | ID LP error RP
    ; 
VarList : ParamDec COMMA VarList { $$ = creatNode("VarList", MVarList, @$.first_line, "VarList"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | ParamDec { $$ = creatNode("VarList", MVarList, @$.first_line, "VarList"); setSons($$, $1); }
    ; 
ParamDec : Specifier VarDec { $$ = creatNode("ParamDec", MParamDec, @$.first_line, "ParamDec"); setSons($$, $1); setSons($$, $2); }

CompSt : LC DefList StmtList RC { $$ = creatNode("CompSt", MCompSt, @$.first_line, "CompSt"); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); }
    | LC DefList error RC
    ;
StmtList : Stmt StmtList { $$ = creatNode("StmtList", MStmtList, @$.first_line, "StmtList"); setSons($$, $1); setSons($$, $2); }
    | /* empty */ { $$ = creatNode("StmtList", MEMPTY, @$.first_line, "StmtList"); }
    ; 
Stmt : Exp SEMI { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt"); setSons($$, $1); setSons($$, $2); }
    | CompSt { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt"); setSons($$, $1); }
    | RETURN Exp SEMI { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt"); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); setSons($$, $5); }
    | IF LP Exp RP Stmt ELSE Stmt { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt"); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); setSons($$, $5); setSons($$, $6); setSons($$, $7); }
    | WHILE LP Exp RP Stmt { $$ = creatNode("Stmt", MStmt, @$.first_line, "Stmt"); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); setSons($$, $5); }
    | error SEMI 
    ;

DefList : Def DefList { $$ = creatNode("DefList", MDefList, @$.first_line, "DefList"); setSons($$, $1); setSons($$, $2); }
    | /* empty */ { $$ = creatNode("DefList", MEMPTY, @$.first_line, "DefList"); }
    ;  
Def : Specifier DecList SEMI { $$ = creatNode("Def", MDef, @$.first_line, "Def"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Specifier error SEMI
    ;
DecList : Dec { $$ = creatNode("DecList", MDecList, @$.first_line, "DecList"); setSons($$, $1); }
    | Dec COMMA DecList { $$ = creatNode("DecList", MDecList, @$.first_line, "DecList"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    ;
Dec : VarDec { $$ = creatNode("Dec", MDec, @$.first_line, "Dec"); setSons($$, $1); }
    | VarDec ASSIGNOP Exp { $$ = creatNode("Dec", MDec, @$.first_line, "Dec"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    ;

Exp : Exp ASSIGNOP Exp { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp AND Exp  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp OR Exp  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp RELOP Exp { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp PLUS Exp  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp MINUS Exp %prec LOWER_THAN_MINUS { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp STAR Exp  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp DIV Exp  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | LP Exp RP { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | MINUS Exp { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); }
    | NOT Exp { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); }
    | ID LP Args RP { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); }
    | ID LP RP { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp LB Exp RB { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); setSons($$, $4); }
    | Exp DOT ID  { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | ID { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); }
    | INT { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); }
    | FLOAT { $$ = creatNode("Exp", MExp, @$.first_line, "Exp"); setSons($$, $1); }
    | Exp LB error RB
    | error RP
    ;
Args : Exp COMMA Args { $$ = creatNode("Args", MArgs, @$.first_line, "Args"); setSons($$, $1); setSons($$, $2); setSons($$, $3); }
    | Exp { $$ = creatNode("Args", MArgs, @$.first_line, "Args"); setSons($$, $1); }
    ;

%%

void yyerror(char* s) {
    // exit(3);
    if (errorType == 1) {
        printf("Error type B at Line %d: Syntax error\n", yylineno);
    }
    else {
        printf("Error type A at Line %d: Lexical error\n", yylineno); 
    }
    syntaxWrong = 1;
    errorType = 1;
}

