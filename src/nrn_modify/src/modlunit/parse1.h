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
    TITLE = 260,
    CONSTANT = 261,
    INDEPENDENT = 262,
    ASSIGNED = 263,
    STATE = 264,
    END_VERBATIM = 265,
    END_COMMENT = 266,
    UNITS = 267,
    BREAKPOINT = 268,
    PARAMETER = 269,
    INITIAL1 = 270,
    DERIVATIVE = 271,
    SOLVE = 272,
    USING = 273,
    WITH = 274,
    STEPPED = 275,
    DISCRETE = 276,
    FROM = 277,
    TO = 278,
    BY = 279,
    WHILE = 280,
    IF = 281,
    ELSE = 282,
    START1 = 283,
    STEP = 284,
    SENS = 285,
    SOLVEFOR = 286,
    PROCEDURE = 287,
    PARTIAL = 288,
    DEL = 289,
    DEL2 = 290,
    DEFINE1 = 291,
    IFERROR = 292,
    DERFUNC = 293,
    EQUATION = 294,
    TERMINAL = 295,
    LINEAR = 296,
    NONLINEAR = 297,
    FUNCTION1 = 298,
    LOCAL = 299,
    METHOD = 300,
    LIN1 = 301,
    NONLIN1 = 302,
    PUTQ = 303,
    GETQ = 304,
    FUNCTION_TABLE = 305,
    INCLUDE1 = 306,
    PROTECT = 307,
    OR = 308,
    AND = 309,
    GT = 310,
    GE = 311,
    LT = 312,
    LE = 313,
    EQ = 314,
    NE = 315,
    NOT = 316,
    NAME = 317,
    PRIME = 318,
    REAL = 319,
    INTEGER = 320,
    DEFINEDVAR = 321,
    KINETIC = 322,
    CONSERVE = 323,
    REACTION = 324,
    REACT1 = 325,
    COMPARTMENT = 326,
    LONGDIFUS = 327,
    PARTEQN = 328,
    STRING = 329,
    PLOT = 330,
    VS = 331,
    LAG = 332,
    RESET = 333,
    MATCH = 334,
    MODEL_LEVEL = 335,
    SWEEP = 336,
    FIRST = 337,
    LAST = 338,
    SPACE = 339,
    NEWLINE = 340,
    TO_EOL = 341,
    STUFF = 342,
    UNITBLK = 343,
    UNITSON = 344,
    UNITSOFF = 345,
    TABLE = 346,
    DEPEND = 347,
    NEURON = 348,
    SUFFIX = 349,
    NONSPECIFIC = 350,
    READ = 351,
    WRITE = 352,
    USEION = 353,
    VALENCE = 354,
    GLOBAL = 355,
    SECTION = 356,
    RANGE = 357,
    POINTER = 358,
    EXTERNAL = 359,
    BEFORE = 360,
    AFTER = 361,
    ELECTRODE_CURRENT = 362,
    CONSTRUCTOR = 363,
    DESTRUCTOR = 364,
    NETRECEIVE = 365,
    FOR_NETCONS = 366,
    WATCH = 367,
    THREADSAFE = 368,
    CONDUCTANCE = 369,
    UNARYMINUS = 370
  };
#endif
/* Tokens.  */
#define VERBATIM 258
#define COMMENT 259
#define TITLE 260
#define CONSTANT 261
#define INDEPENDENT 262
#define ASSIGNED 263
#define STATE 264
#define END_VERBATIM 265
#define END_COMMENT 266
#define UNITS 267
#define BREAKPOINT 268
#define PARAMETER 269
#define INITIAL1 270
#define DERIVATIVE 271
#define SOLVE 272
#define USING 273
#define WITH 274
#define STEPPED 275
#define DISCRETE 276
#define FROM 277
#define TO 278
#define BY 279
#define WHILE 280
#define IF 281
#define ELSE 282
#define START1 283
#define STEP 284
#define SENS 285
#define SOLVEFOR 286
#define PROCEDURE 287
#define PARTIAL 288
#define DEL 289
#define DEL2 290
#define DEFINE1 291
#define IFERROR 292
#define DERFUNC 293
#define EQUATION 294
#define TERMINAL 295
#define LINEAR 296
#define NONLINEAR 297
#define FUNCTION1 298
#define LOCAL 299
#define METHOD 300
#define LIN1 301
#define NONLIN1 302
#define PUTQ 303
#define GETQ 304
#define FUNCTION_TABLE 305
#define INCLUDE1 306
#define PROTECT 307
#define OR 308
#define AND 309
#define GT 310
#define GE 311
#define LT 312
#define LE 313
#define EQ 314
#define NE 315
#define NOT 316
#define NAME 317
#define PRIME 318
#define REAL 319
#define INTEGER 320
#define DEFINEDVAR 321
#define KINETIC 322
#define CONSERVE 323
#define REACTION 324
#define REACT1 325
#define COMPARTMENT 326
#define LONGDIFUS 327
#define PARTEQN 328
#define STRING 329
#define PLOT 330
#define VS 331
#define LAG 332
#define RESET 333
#define MATCH 334
#define MODEL_LEVEL 335
#define SWEEP 336
#define FIRST 337
#define LAST 338
#define SPACE 339
#define NEWLINE 340
#define TO_EOL 341
#define STUFF 342
#define UNITBLK 343
#define UNITSON 344
#define UNITSOFF 345
#define TABLE 346
#define DEPEND 347
#define NEURON 348
#define SUFFIX 349
#define NONSPECIFIC 350
#define READ 351
#define WRITE 352
#define USEION 353
#define VALENCE 354
#define GLOBAL 355
#define SECTION 356
#define RANGE 357
#define POINTER 358
#define EXTERNAL 359
#define BEFORE 360
#define AFTER 361
#define ELECTRODE_CURRENT 362
#define CONSTRUCTOR 363
#define DESTRUCTOR 364
#define NETRECEIVE 365
#define FOR_NETCONS 366
#define WATCH 367
#define THREADSAFE 368
#define CONDUCTANCE 369
#define UNARYMINUS 370

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 56 "parse1.y" /* yacc.c:1909  */

	Item	*qp;

#line 288 "parse1.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */
