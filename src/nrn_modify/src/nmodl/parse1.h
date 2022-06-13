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
    VERBATIM = 258,
    COMMENT = 259,
    MODEL = 260,
    CONSTANT = 261,
    INDEPENDENT = 262,
    DEPENDENT = 263,
    STATE = 264,
    INITIAL1 = 265,
    DERIVATIVE = 266,
    SOLVE = 267,
    USING = 268,
    WITH = 269,
    STEPPED = 270,
    DISCRETE = 271,
    FROM = 272,
    FORALL1 = 273,
    TO = 274,
    BY = 275,
    WHILE = 276,
    IF = 277,
    ELSE = 278,
    START1 = 279,
    STEP = 280,
    SENS = 281,
    SOLVEFOR = 282,
    PROCEDURE = 283,
    PARTIAL = 284,
    DEL = 285,
    DEL2 = 286,
    DEFINE1 = 287,
    IFERROR = 288,
    PARAMETER = 289,
    DERFUNC = 290,
    EQUATION = 291,
    TERMINAL = 292,
    LINEAR = 293,
    NONLINEAR = 294,
    FUNCTION1 = 295,
    LOCAL = 296,
    METHOD = 297,
    LIN1 = 298,
    NONLIN1 = 299,
    PUTQ = 300,
    GETQ = 301,
    TABLE = 302,
    DEPEND = 303,
    BREAKPOINT = 304,
    INCLUDE1 = 305,
    FUNCTION_TABLE = 306,
    PROTECT = 307,
    NRNMUTEXLOCK = 308,
    NRNMUTEXUNLOCK = 309,
    OR = 310,
    AND = 311,
    GT = 312,
    LT = 313,
    LE = 314,
    EQ = 315,
    NE = 316,
    NOT = 317,
    NAME = 318,
    PRIME = 319,
    REAL = 320,
    INTEGER = 321,
    DEFINEDVAR = 322,
    ONTOLOGY_ID = 323,
    STRING = 324,
    PLOT = 325,
    VS = 326,
    LAG = 327,
    RESET = 328,
    MATCH = 329,
    MODEL_LEVEL = 330,
    SWEEP = 331,
    FIRST = 332,
    LAST = 333,
    KINETIC = 334,
    CONSERVE = 335,
    REACTION = 336,
    REACT1 = 337,
    COMPARTMENT = 338,
    UNITS = 339,
    UNITSON = 340,
    UNITSOFF = 341,
    LONGDIFUS = 342,
    NEURON = 343,
    SUFFIX = 344,
    NONSPECIFIC = 345,
    READ = 346,
    WRITE = 347,
    USEION = 348,
    VALENCE = 349,
    THREADSAFE = 350,
    REPRESENTS = 351,
    GLOBAL = 352,
    SECTION = 353,
    RANGE = 354,
    POINTER = 355,
    BBCOREPOINTER = 356,
    EXTERNAL = 357,
    BEFORE = 358,
    AFTER = 359,
    WATCH = 360,
    ELECTRODE_CURRENT = 361,
    CONSTRUCTOR = 362,
    DESTRUCTOR = 363,
    NETRECEIVE = 364,
    FOR_NETCONS = 365,
    CONDUCTANCE = 366,
    GE = 367,
    UNARYMINUS = 368
  };
#endif
/* Tokens.  */
#define VERBATIM 258
#define COMMENT 259
#define MODEL 260
#define CONSTANT 261
#define INDEPENDENT 262
#define DEPENDENT 263
#define STATE 264
#define INITIAL1 265
#define DERIVATIVE 266
#define SOLVE 267
#define USING 268
#define WITH 269
#define STEPPED 270
#define DISCRETE 271
#define FROM 272
#define FORALL1 273
#define TO 274
#define BY 275
#define WHILE 276
#define IF 277
#define ELSE 278
#define START1 279
#define STEP 280
#define SENS 281
#define SOLVEFOR 282
#define PROCEDURE 283
#define PARTIAL 284
#define DEL 285
#define DEL2 286
#define DEFINE1 287
#define IFERROR 288
#define PARAMETER 289
#define DERFUNC 290
#define EQUATION 291
#define TERMINAL 292
#define LINEAR 293
#define NONLINEAR 294
#define FUNCTION1 295
#define LOCAL 296
#define METHOD 297
#define LIN1 298
#define NONLIN1 299
#define PUTQ 300
#define GETQ 301
#define TABLE 302
#define DEPEND 303
#define BREAKPOINT 304
#define INCLUDE1 305
#define FUNCTION_TABLE 306
#define PROTECT 307
#define NRNMUTEXLOCK 308
#define NRNMUTEXUNLOCK 309
#define OR 310
#define AND 311
#define GT 312
#define LT 313
#define LE 314
#define EQ 315
#define NE 316
#define NOT 317
#define NAME 318
#define PRIME 319
#define REAL 320
#define INTEGER 321
#define DEFINEDVAR 322
#define ONTOLOGY_ID 323
#define STRING 324
#define PLOT 325
#define VS 326
#define LAG 327
#define RESET 328
#define MATCH 329
#define MODEL_LEVEL 330
#define SWEEP 331
#define FIRST 332
#define LAST 333
#define KINETIC 334
#define CONSERVE 335
#define REACTION 336
#define REACT1 337
#define COMPARTMENT 338
#define UNITS 339
#define UNITSON 340
#define UNITSOFF 341
#define LONGDIFUS 342
#define NEURON 343
#define SUFFIX 344
#define NONSPECIFIC 345
#define READ 346
#define WRITE 347
#define USEION 348
#define VALENCE 349
#define THREADSAFE 350
#define REPRESENTS 351
#define GLOBAL 352
#define SECTION 353
#define RANGE 354
#define POINTER 355
#define BBCOREPOINTER 356
#define EXTERNAL 357
#define BEFORE 358
#define AFTER 359
#define WATCH 360
#define ELECTRODE_CURRENT 361
#define CONSTRUCTOR 362
#define DESTRUCTOR 363
#define NETRECEIVE 364
#define FOR_NETCONS 365
#define CONDUCTANCE 366
#define GE 367
#define UNARYMINUS 368

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 76 "parse1.y" /* yacc.c:1909  */

	Item	*qp;
	char	*str;
	List	*lp;
	int	i;

#line 287 "parse1.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */
