%{
    #include<stdio.h>
    #include<string.h>
    #include"SyntaxTree.h"
    #include"syntax.tab.h"

    int lexicalWrong = 0;
    int yycolumn = 1; 
    extern int errorType;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
%}

%option yylineno

digit [0-9]
letter [_a-zA-Z]
relop ">"|"<"|">="|"<="|"=="|"!="

%%
"//" { 
    char c = input(); 
    while (c != '\n') c = input(); 
}

"/*" {
    char past = input(), cur;
    do {
        cur = input();
        if (past == '*' && cur == '/') break;
        past = cur;
    } while(1) ;
}

([1-9]{digit}*|0) { yylval = creatNode(yytext, MINT, yylloc.first_line, "INT", 0); return INT; }
({digit}*\.{digit}+)|({digit}+\.{digit}*) { yylval = creatNode(yytext, MFLOAT, yylloc.first_line, "FLOAT", 0); return FLOAT; }
";" { yylval = creatNode(yytext, MSEMI, yylloc.first_line, "SEMI", 0); return SEMI; }
"," { yylval = creatNode(yytext, MCOMMA, yylloc.first_line, "COMMA", 0); return COMMA; }
"=" { yylval = creatNode(yytext, MASSIGNOP, yylloc.first_line, "ASSIGNOP", 0); return ASSIGNOP; } 
{relop} { yylval = creatNode(yytext, MRELOP, yylloc.first_line, "RELOP", 0); return RELOP; }
"+" { yylval = creatNode(yytext, MPLUS, yylloc.first_line, "PLUS", 0); return PLUS; } 
"-" { yylval = creatNode(yytext, MMINUS, yylloc.first_line, "MINUS", 0); return MINUS; } 
"*" { yylval = creatNode(yytext, MSTAR, yylloc.first_line, "STAR", 0); return STAR; } 
"/" { yylval = creatNode(yytext, MDIV, yylloc.first_line, "DIV", 0); return DIV; } 
"&&" { yylval = creatNode(yytext, MAND, yylloc.first_line, "AND", 0); return AND; } 
"||" { yylval = creatNode(yytext, MOR, yylloc.first_line, "OR", 0); return OR; }
"."  { yylval = creatNode(yytext, MDOT, yylloc.first_line, "DOT", 0); return DOT; }
"!" { yylval = creatNode(yytext, MNOT, yylloc.first_line, "NOT", 0); return NOT; } 
"int"|"float" { yylval = creatNode(yytext, MTYPE, yylloc.first_line, "TYPE", 0); return TYPE; } 
"(" { yylval = creatNode(yytext, MLP, yylloc.first_line, "LP", 0); return LP; } 
")" { yylval = creatNode(yytext, MRP, yylloc.first_line, "RP", 0); return RP; } 
"[" { yylval = creatNode(yytext, MLB, yylloc.first_line, "LB", 0); return LB; }
"]" { yylval = creatNode(yytext, MRB, yylloc.first_line, "RB", 0); return RB; }
"{" { yylval = creatNode(yytext, MLC, yylloc.first_line, "LC", 0); return LC; }
"}" { yylval = creatNode(yytext, MRC, yylloc.first_line, "RC", 0); return RC; } 
"struct" { yylval = creatNode(yytext, MSTRUCT, yylloc.first_line, "STRUCT", 0); return STRUCT; } 
"return" { yylval = creatNode(yytext, MRETURN, yylloc.first_line, "RETURN", 0); return RETURN; }
"if" { yylval = creatNode(yytext, MIF, yylloc.first_line, "IF", 0); return IF; }
"else" { yylval = creatNode(yytext, MELSE, yylloc.first_line, "ELSE", 0); return ELSE; } 
"while" { yylval = creatNode(yytext, MWHILE, yylloc.first_line, "WHILE", 0); return WHILE; }
{letter}+({letter}|{digit})* { yylval = creatNode(yytext, MID, yylloc.first_line, "ID", 0); return ID; } 

"\r" { }
"\n" { yycolumn = 1; }
"\t" { }
" " { }

. {
    lexicalWrong = 1;
    errorType = 0;
    return BAD;
} 

%%


