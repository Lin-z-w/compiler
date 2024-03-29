/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_IR_YY_BUILD_GENERATE_IR_PARSE_IR_SYNTAX_TAB_H_INCLUDED
# define YY_IR_YY_BUILD_GENERATE_IR_PARSE_IR_SYNTAX_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef IR_YYDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define IR_YYDEBUG 1
#  else
#   define IR_YYDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define IR_YYDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined IR_YYDEBUG */
#if IR_YYDEBUG
extern int IR_yydebug;
#endif

/* Token kinds.  */
#ifndef IR_YYTOKENTYPE
# define IR_YYTOKENTYPE
  enum IR_yytokentype
  {
    IR_TOKEN_IR_YYEMPTY = -2,
    IR_TOKEN_YYEOF = 0,            /* "end of file"  */
    IR_TOKEN_IR_YYerror = 256,     /* error  */
    IR_TOKEN_IR_YYUNDEF = 257,     /* "invalid token"  */
    IR_TOKEN_EOL = 258,            /* EOL  */
    IR_TOKEN_COLON = 259,          /* COLON  */
    IR_TOKEN_FUNCTION = 260,       /* FUNCTION  */
    IR_TOKEN_LABEL = 261,          /* LABEL  */
    IR_TOKEN_SHARP = 262,          /* SHARP  */
    IR_TOKEN_INT = 263,            /* INT  */
    IR_TOKEN_ASSIGN = 264,         /* ASSIGN  */
    IR_TOKEN_STAR = 265,           /* STAR  */
    IR_TOKEN_OP = 266,             /* OP  */
    IR_TOKEN_IF = 267,             /* IF  */
    IR_TOKEN_RELOP = 268,          /* RELOP  */
    IR_TOKEN_ADDR_OF = 269,        /* ADDR_OF  */
    IR_TOKEN_GOTO = 270,           /* GOTO  */
    IR_TOKEN_RETURN = 271,         /* RETURN  */
    IR_TOKEN_DEC = 272,            /* DEC  */
    IR_TOKEN_ARG = 273,            /* ARG  */
    IR_TOKEN_CALL = 274,           /* CALL  */
    IR_TOKEN_PARAM = 275,          /* PARAM  */
    IR_TOKEN_READ = 276,           /* READ  */
    IR_TOKEN_WRITE = 277,          /* WRITE  */
    IR_TOKEN_ID = 278              /* ID  */
  };
  typedef enum IR_yytokentype IR_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined IR_YYSTYPE && ! defined IR_YYSTYPE_IS_DECLARED
union IR_YYSTYPE
{
#line 18 "src/IR_parse/IR_syntax.y"

    int INT;
    char *id;
    IR_OP_TYPE IR_op_type;
    IR_RELOP_TYPE IR_relop_type;
    IR_program *IR_program_ptr_node;
    IR_function *IR_function_ptr_node;
    IR_stmt *IR_stmt_ptr_node;
    IR_val IR_val_node;
    IR_label IR_label_node;
    IR_var IR_var_node;

#line 108 "build/generate/IR_parse/IR_syntax.tab.h"

};
typedef union IR_YYSTYPE IR_YYSTYPE;
# define IR_YYSTYPE_IS_TRIVIAL 1
# define IR_YYSTYPE_IS_DECLARED 1
#endif


extern IR_YYSTYPE IR_yylval;


int IR_yyparse (void);


#endif /* !YY_IR_YY_BUILD_GENERATE_IR_PARSE_IR_SYNTAX_TAB_H_INCLUDED  */
