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

([1-9]{digit}*|0) { yylval = creatNode(yytext, MINT, yylloc.first_line, "INT"); return INT; }
({digit}*\.{digit}+)|({digit}+\.{digit}*) { yylval = creatNode(yytext, MFLOAT, yylloc.first_line, "FLOAT"); return FLOAT; }
";" { yylval = creatNode(yytext, MSEMI, yylloc.first_line, "SEMI"); return SEMI; }
"," { yylval = creatNode(yytext, MCOMMA, yylloc.first_line, "COMMA"); return COMMA; }
"=" { yylval = creatNode(yytext, MASSIGNOP, yylloc.first_line, "ASSIGNOP"); return ASSIGNOP; } 
{relop} { yylval = creatNode(yytext, MRELOP, yylloc.first_line, "RELOP"); return RELOP; }
"+" { yylval = creatNode(yytext, MPLUS, yylloc.first_line, "PLUS"); return PLUS; } 
"-" { yylval = creatNode(yytext, MMINUS, yylloc.first_line, "MINUS"); return MINUS; } 
"*" { yylval = creatNode(yytext, MSTAR, yylloc.first_line, "STAR"); return STAR; } 
"/" { yylval = creatNode(yytext, MDIV, yylloc.first_line, "DIV"); return DIV; } 
"&&" { yylval = creatNode(yytext, MAND, yylloc.first_line, "AND"); return AND; } 
"||" { yylval = creatNode(yytext, MOR, yylloc.first_line, "OR"); return OR; }
"."  { yylval = creatNode(yytext, MDOT, yylloc.first_line, "DOT"); return DOT; }
"!" { yylval = creatNode(yytext, MNOT, yylloc.first_line, "NOT"); return NOT; } 
"int"|"float" { yylval = creatNode(yytext, MTYPE, yylloc.first_line, "TYPE"); return TYPE; } 
"(" { yylval = creatNode(yytext, MLP, yylloc.first_line, "LP"); return LP; } 
")" { yylval = creatNode(yytext, MRP, yylloc.first_line, "RP"); return RP; } 
"[" { yylval = creatNode(yytext, MLB, yylloc.first_line, "LB"); return LB; }
"]" { yylval = creatNode(yytext, MRB, yylloc.first_line, "RB"); return RB; }
"{" { yylval = creatNode(yytext, MLC, yylloc.first_line, "LC"); return LC; }
"}" { yylval = creatNode(yytext, MRC, yylloc.first_line, "RC"); return RC; } 
"struct" { yylval = creatNode(yytext, MSTRUCT, yylloc.first_line, "STRUCT"); return STRUCT; } 
"return" { yylval = creatNode(yytext, MRETURN, yylloc.first_line, "RETURN"); return RETURN; }
"if" { yylval = creatNode(yytext, MIF, yylloc.first_line, "IF"); return IF; }
"else" { yylval = creatNode(yytext, MELSE, yylloc.first_line, "ELSE"); return ELSE; } 
"while" { yylval = creatNode(yytext, MWHILE, yylloc.first_line, "WHILE"); return WHILE; }
{letter}+({letter}|{digit})* { yylval = creatNode(yytext, MID, yylloc.first_line, "ID"); return ID; } 

"\n" { yycolumn = 1; }
" " { }

. {
    lexicalWrong = 1;
    errorType = 0;
    return BAD;
} 

%%


