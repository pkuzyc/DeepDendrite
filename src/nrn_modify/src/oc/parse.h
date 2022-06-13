/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_PARSE_H_INCLUDED
# define YY_YY_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    EQNEQ = 258,
    NUMBER = 259,
    STRING = 260,
    PRINT = 261,
    parseDELETE = 262,
    VAR = 263,
    BLTIN = 264,
    UNDEF = 265,
    WHILE = 266,
    IF = 267,
    ELSE = 268,
    FOR = 269,
    FUNCTION = 270,
    PROCEDURE = 271,
    RETURN = 272,
    FUNC = 273,
    parsePROC = 274,
    HOCOBJFUNC = 275,
    READ = 276,
    parseDOUBLE = 277,
    DEBUG = 278,
    EDIT = 279,
    FUN_BLTIN = 280,
    DEPENDENT = 281,
    EQUATION = 282,
    LOCAL = 283,
    HOCOBJFUNCTION = 284,
    BREAK = 285,
    CONTINUE = 286,
    AUTO = 287,
    STRDEF = 288,
    STOPSTMT = 289,
    CSTRING = 290,
    PARALLEL = 291,
    HELP = 292,
    ITERATOR = 293,
    ITERKEYWORD = 294,
    ITERSTMT = 295,
    STRINGFUNC = 296,
    OBJECTFUNC = 297,
    LOCALOBJ = 298,
    AUTOOBJ = 299,
    ARG = 300,
    NUMZERO = 301,
    ARGREF = 302,
    INTERNALSECTIONNAME = 303,
    PYSEC = 304,
    PYSECNAME = 305,
    PYSECOBJ = 306,
    SECTIONKEYWORD = 307,
    SECTION = 308,
    CONNECTKEYWORD = 309,
    ACCESSKEYWORD = 310,
    RANGEVAR = 311,
    MECHANISM = 312,
    INSERTKEYWORD = 313,
    FORALL = 314,
    NRNPNTVAR = 315,
    FORSEC = 316,
    IFSEC = 317,
    UNINSERTKEYWORD = 318,
    SETPOINTERKEYWORD = 319,
    SECTIONREF = 320,
    BEGINTEMPLATE = 321,
    ENDTEMPLATE = 322,
    NEW = 323,
    OBJECTVAR = 324,
    TEMPLATE = 325,
    OBJVARDECL = 326,
    PUBLICDECL = 327,
    EXTERNALDECL = 328,
    OBFUNCTION = 329,
    STRFUNCTION = 330,
    OBJECTARG = 331,
    STRINGARG = 332,
    ROP = 333,
    OR = 334,
    AND = 335,
    GT = 336,
    GE = 337,
    LT = 338,
    LE = 339,
    EQ = 340,
    NE = 341,
    UNARYMINUS = 342,
    NOT = 343
  };
#endif
/* Tokens.  */
#define EQNEQ 258
#define NUMBER 259
#define STRING 260
#define PRINT 261
#define parseDELETE 262
#define VAR 263
#define BLTIN 264
#define UNDEF 265
#define WHILE 266
#define IF 267
#define ELSE 268
#define FOR 269
#define FUNCTION 270
#define PROCEDURE 271
#define RETURN 272
#define FUNC 273
#define parsePROC 274
#define HOCOBJFUNC 275
#define READ 276
#define parseDOUBLE 277
#define DEBUG 278
#define EDIT 279
#define FUN_BLTIN 280
#define DEPENDENT 281
#define EQUATION 282
#define LOCAL 283
#define HOCOBJFUNCTION 284
#define BREAK 285
#define CONTINUE 286
#define AUTO 287
#define STRDEF 288
#define STOPSTMT 289
#define CSTRING 290
#define PARALLEL 291
#define HELP 292
#define ITERATOR 293
#define ITERKEYWORD 294
#define ITERSTMT 295
#define STRINGFUNC 296
#define OBJECTFUNC 297
#define LOCALOBJ 298
#define AUTOOBJ 299
#define ARG 300
#define NUMZERO 301
#define ARGREF 302
#define INTERNALSECTIONNAME 303
#define PYSEC 304
#define PYSECNAME 305
#define PYSECOBJ 306
#define SECTIONKEYWORD 307
#define SECTION 308
#define CONNECTKEYWORD 309
#define ACCESSKEYWORD 310
#define RANGEVAR 311
#define MECHANISM 312
#define INSERTKEYWORD 313
#define FORALL 314
#define NRNPNTVAR 315
#define FORSEC 316
#define IFSEC 317
#define UNINSERTKEYWORD 318
#define SETPOINTERKEYWORD 319
#define SECTIONREF 320
#define BEGINTEMPLATE 321
#define ENDTEMPLATE 322
#define NEW 323
#define OBJECTVAR 324
#define TEMPLATE 325
#define OBJVARDECL 326
#define PUBLICDECL 327
#define EXTERNALDECL 328
#define OBFUNCTION 329
#define STRFUNCTION 330
#define OBJECTARG 331
#define STRINGARG 332
#define ROP 333
#define OR 334
#define AND 335
#define GT 336
#define GE 337
#define LT 338
#define LE 339
#define EQ 340
#define NE 341
#define UNARYMINUS 342
#define NOT 343

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 84 "parse.y" /* yacc.c:1909  */
				/* stack type */
	Symbol	*sym;			/* symbol table pointer */
	Inst	*inst;			/* machine instruction */
	int	narg;			/* number of arguments */
	void*	ptr;

#line 237 "parse.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */
