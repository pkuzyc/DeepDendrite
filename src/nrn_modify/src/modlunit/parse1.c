/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parse1.y" /* yacc.c:339  */

/* /local/src/master/nrn/src/modlunit/parse1.y,v 1.11 1999/02/27 21:13:50 hines Exp */

#include <../../nmodlconf.h>
#include <stdlib.h>
#include "model.h"

/* Constructs a parse tree. No translation is done, ie. on exit printing
the intoken list will make an exact copy of the input file.
All tokens and productions are of Item type and consist of STRING, SYMBOL,
and LIST.
SPACE and NEWLINE are in the intoken list but are not yacc tokens.

All explicitly declared names are given their subtype
and a pointer to their declaration. All "used" names are marked with
their usage for later error checking.
*/

extern Symbol *checklocal();
extern int next_intoken();
extern Item *title;
extern int declare_level;
extern int parse_pass, restart_pass;
extern List *solvelist;

extern int conductance_seen_;
extern int breakpoint_local_seen_;

#define IFP(n)	if (parse_pass == n)
#define IFR(n)	if (restart_pass == n)
#define P1	IFP(1)
#define P2	IFP(2)
#define P3	IFP(3)
#define R0	IFR(0)
#define R1	IFR(1)

static int yylex();
static void yyerror();

#if YYBISON 
#define myerr(arg) static int ierr=0;\
if (!(ierr++))yyerror(arg); --yyssp; --yyvsp; YYERROR
#else
#define myerr(arg) static int ierr=0;\
if (!(ierr++))yyerror(arg); --yyps; --yypv; YYERROR
#endif

extern Item *lastok;	/* last token accepted by expr */
static int blocktype;
static int unitflagsave; /*must turn off units in restartpass0 in kinetic block */
static List* netreceive_arglist;
static List* args;
extern int lexcontext;

#line 121 "parse1.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
#line 56 "parse1.y" /* yacc.c:355  */

	Item	*qp;

#line 395 "parse1.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 412 "parse1.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1256

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  133
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  147
/* YYNRULES -- Number of rules.  */
#define YYNRULES  348
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  677

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   370

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   131,     2,     2,
      55,    56,    61,    60,    67,    62,     2,    63,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    66,     2,
       2,    64,     2,     2,    59,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    57,     2,    58,    65,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    53,     2,    54,    68,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     132
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   119,   119,   120,   123,   124,   126,   126,   126,   126,
     126,   126,   126,   127,   128,   130,   133,   135,   137,   139,
     142,   145,   147,   149,   152,   153,   153,   156,   156,   156,
     156,   156,   157,   157,   157,   159,   160,   163,   164,   166,
     171,   173,   175,   178,   179,   181,   181,   185,   186,   189,
     190,   192,   195,   196,   198,   201,   202,   204,   205,   207,
     209,   212,   212,   214,   215,   217,   219,   222,   225,   226,
     228,   232,   234,   236,   238,   241,   242,   245,   247,   249,
     251,   253,   257,   258,   260,   263,   264,   267,   268,   270,
     271,   274,   275,   278,   281,   282,   283,   285,   287,   289,
     291,   302,   313,   319,   319,   321,   321,   323,   323,   325,
     325,   327,   327,   329,   329,   331,   331,   333,   333,   335,
     335,   337,   337,   339,   339,   341,   341,   343,   343,   345,
     345,   347,   347,   352,   352,   353,   353,   355,   357,   359,
     361,   362,   363,   364,   366,   368,   371,   373,   376,   377,
     379,   381,   385,   386,   390,   391,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   422,   434,   441,   448,   455,   463,   471,
     473,   474,   475,   476,   477,   478,   479,   481,   482,   488,
     489,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   509,   509,   515,
     516,   518,   519,   520,   521,   523,   523,   525,   528,   529,
     531,   533,   536,   537,   540,   541,   543,   545,   548,   551,
     554,   557,   560,   569,   575,   579,   581,   581,   584,   583,
     592,   601,   605,   607,   621,   633,   641,   640,   654,   657,
     658,   659,   661,   664,   663,   692,   695,   697,   699,   702,
     703,   707,   708,   710,   715,   720,   722,   724,   726,   727,
     730,   732,   734,   735,   736,   737,   741,   741,   747,   753,
     755,   758,   758,   764,   770,   772,   775,   776,   778,   781,
     783,   785,   787,   789,   790,   791,   792,   796,   797,   799,
     803,   804,   807,   808,   810,   811,   813,   815,   816,   818,
     819,   820,   824,   825,   829,   828,   838,   839,   841,   842,
     844,   846,   848,   850,   852,   854,   856,   859,   860,   862,
     864,   866,   868,   871,   873,   875,   879,   880,   882
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "VERBATIM", "COMMENT", "TITLE",
  "CONSTANT", "INDEPENDENT", "ASSIGNED", "STATE", "END_VERBATIM",
  "END_COMMENT", "UNITS", "BREAKPOINT", "PARAMETER", "INITIAL1",
  "DERIVATIVE", "SOLVE", "USING", "WITH", "STEPPED", "DISCRETE", "FROM",
  "TO", "BY", "WHILE", "IF", "ELSE", "START1", "STEP", "SENS", "SOLVEFOR",
  "PROCEDURE", "PARTIAL", "DEL", "DEL2", "DEFINE1", "IFERROR", "DERFUNC",
  "EQUATION", "TERMINAL", "LINEAR", "NONLINEAR", "FUNCTION1", "LOCAL",
  "METHOD", "LIN1", "NONLIN1", "PUTQ", "GETQ", "FUNCTION_TABLE",
  "INCLUDE1", "PROTECT", "'{'", "'}'", "'('", "')'", "'['", "']'", "'@'",
  "'+'", "'*'", "'-'", "'/'", "'='", "'^'", "':'", "','", "'~'", "OR",
  "AND", "GT", "GE", "LT", "LE", "EQ", "NE", "NOT", "NAME", "PRIME",
  "REAL", "INTEGER", "DEFINEDVAR", "KINETIC", "CONSERVE", "REACTION",
  "REACT1", "COMPARTMENT", "LONGDIFUS", "PARTEQN", "STRING", "PLOT", "VS",
  "LAG", "RESET", "MATCH", "MODEL_LEVEL", "SWEEP", "FIRST", "LAST",
  "SPACE", "NEWLINE", "TO_EOL", "STUFF", "UNITBLK", "UNITSON", "UNITSOFF",
  "TABLE", "DEPEND", "NEURON", "SUFFIX", "NONSPECIFIC", "READ", "WRITE",
  "USEION", "VALENCE", "GLOBAL", "SECTION", "RANGE", "POINTER", "EXTERNAL",
  "BEFORE", "AFTER", "ELECTRODE_CURRENT", "CONSTRUCTOR", "DESTRUCTOR",
  "NETRECEIVE", "FOR_NETCONS", "WATCH", "THREADSAFE", "CONDUCTANCE", "'%'",
  "UNARYMINUS", "$accept", "top", "all", "all1", "title", "verbatim",
  "comment", "unitflag", "include1", "define1", "Name", "declare", "$@1",
  "declare1", "constblk", "constbody", "constasgn", "units", "Units",
  "$@2", "limits", "tolerance", "stepblk", "stepbdy", "stepped", "numlist",
  "name", "number", "NUMBER", "integer", "real", "indepblk", "indepbody",
  "indepdef", "withby", "depblk", "depbody", "depdef", "opstart",
  "stateblk", "statbody", "plotdecl", "pvlist", "optindex", "unitblk",
  "unitbody", "unitdef", "factordef", "proc", "$@3", "$@4", "$@5", "$@6",
  "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14", "$@15",
  "$@16", "$@17", "$@18", "$@19", "initblk", "constructblk", "destructblk",
  "bablk", "stmtlist", "conducthint", "locallist", "locallist1",
  "locoptarray", "stmtlist1", "stmt", "asgn", "varname", "intexpr", "expr",
  "funccall", "$@20", "exprlist", "exprlist1", "fromstmt", "$@21", "opinc",
  "whilestmt", "ifstmt", "optelseif", "optelse", "initstmt", "derivblk",
  "linblk", "nonlinblk", "discretblk", "partialblk", "pareqn", "firstlast",
  "funcblk", "$@22", "functbl", "arglist", "arglist1", "procedblk",
  "netrecblk", "$@23", "watchstmt", "watch1", "fornetcon", "$@24",
  "solveblk", "ifsolerr", "solvefor", "solvefor1", "eqnblk", "terminalblk",
  "sens", "conserve", "consreact", "compart", "$@25", "compartlist",
  "longdifus", "$@26", "ldifuslist", "namelist", "kineticblk", "reaction",
  "react", "lagstmt", "tablestmt", "tablst", "dependlst", "queuestmt",
  "matchblk", "matchlist", "match", "matchname", "neuronblk", "$@27",
  "nrnstmt", "optnamelist", "nrnuse", "nrnlist", "valence", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   123,   125,    40,    41,    91,    93,    64,
      43,    42,    45,    47,    61,    94,    58,    44,   126,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,    37,   370
};
# endif

#define YYPACT_NINF -547

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-547)))

#define YYTABLE_NINF -323

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     831,  -547,    39,   888,  -547,    43,    90,   -15,    93,   121,
     134,   235,   251,   254,    59,    76,   112,    85,   124,   284,
    -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,
    -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,
     238,   250,   283,   272,   308,   310,   317,   370,   296,   392,
     434,   334,   338,   444,   398,   367,   376,  -547,  -547,  -547,
    -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,   152,
    -547,   449,   442,  -547,  -547,  -547,  -547,  -547,   453,   -22,
    -547,  -547,   459,   472,  -547,   448,  -547,   472,  -547,   450,
    -547,   457,  -547,   466,  -547,   475,  -547,   478,  -547,    28,
    -547,   472,  -547,   480,  -547,   472,  -547,   472,  -547,   481,
    -547,   483,  -547,   311,   311,   181,    13,    27,   135,   192,
      22,  -547,  -547,  -547,   152,  -547,   484,   152,  -547,   300,
     300,   243,    74,  -547,   510,  -547,   472,  -547,   496,   496,
     508,   509,   516,  -547,   300,  -547,   472,  -547,  -547,   472,
     496,   472,   472,   472,   472,  -547,  -547,  -547,  -547,     4,
    -547,  -547,  -547,   550,    98,  -547,  -547,  -547,   197,  -547,
    -547,  -547,  -547,  -547,   518,  -547,   515,   449,   525,   453,
     453,  -547,  -547,  -547,  -547,   520,    14,  -547,  -547,    61,
    -547,   605,  -547,   117,   472,   507,   472,   300,   300,   300,
     530,   531,   521,  -547,  -547,   472,  -547,  -547,  -547,  -547,
     152,   289,   523,  -547,   289,  -547,   289,   289,   152,   530,
     289,  -547,  -547,  -547,  -547,  -547,   577,   203,  -547,   530,
    -547,  -547,   519,   149,   522,   149,   149,   149,   149,   149,
     149,   532,  -547,   700,  -547,   472,   196,   211,   538,   544,
     214,   300,   300,   223,  -547,    78,   287,    17,    53,   335,
     452,    10,   142,  -547,   548,   532,    62,    89,   532,  -547,
    -547,  -547,   562,  -547,  -547,  -547,  -547,   557,  -547,  -547,
    -547,  -547,  -547,  -547,   556,  -547,  -547,  -547,  -547,  -547,
    -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,   546,
    -547,   569,   570,   572,  -547,  -547,   300,  -547,   571,   355,
    -547,   530,  -547,  -547,   289,  -547,   610,   611,  -547,   579,
     565,   118,   573,   583,  -547,    47,  -547,   530,  -547,  -547,
    -547,  -547,   574,   260,   574,   574,   574,   574,   574,   574,
    -547,   575,  -547,  -547,  -547,  -547,     1,  -547,   580,    78,
      78,  -547,   575,  -547,  -547,   584,  -547,  -547,    78,    78,
      78,   530,  -547,  1146,  -547,  -547,   152,  -547,   401,  -547,
     302,  -547,   401,  -547,   213,   268,   739,   332,   955,  -547,
     567,   589,   586,  -547,   623,   160,   575,   540,  -547,   300,
    -547,    78,  -547,   537,    42,    78,   601,  -547,   530,   530,
     472,   472,   530,   530,  -547,   523,   593,   289,   289,   215,
     289,  -547,   289,  -547,   289,  -547,   590,  -547,  -547,   585,
     149,   149,   532,   619,   472,  -547,    42,   542,   606,   701,
     600,   600,  -547,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,   200,   613,  -547,
     307,    78,  -547,   314,   603,   612,   330,   620,   608,   621,
     617,   634,   388,   618,   624,  -547,   327,   190,  -547,   642,
     532,   683,   650,  1024,   629,  -547,    42,  -547,  -547,   343,
    1180,  -547,  -547,  -547,  -547,  -547,  -547,   523,  -547,   289,
     689,   681,   289,   530,   643,  -547,  -547,   530,  -547,   -33,
     -50,  -547,   676,  -547,   249,   472,   472,  -547,   253,   600,
     253,   600,  1180,   600,   902,   505,   368,   368,   368,   368,
     368,   368,  -547,  1180,   -25,  -547,    42,   401,  -547,  1180,
     401,  -547,   661,   663,   128,    78,  -547,   138,    78,  -547,
     163,   388,  -547,  -547,   662,   658,  -547,   645,  -547,  -547,
      78,   575,    78,  -547,   385,  -547,   455,  -547,    42,    42,
      42,    42,   472,  -547,   653,  -547,   152,   530,   698,   565,
    -547,  -547,   149,   294,  -547,  -547,  -547,    42,  -547,  -547,
    -547,   423,   461,  -547,  -547,    78,    78,    78,   979,  -547,
    -547,  1003,  -547,  -547,   669,   664,   694,   673,  1045,   476,
     472,  -547,  -547,   358,  -547,   358,  -547,  -547,  -547,   681,
     565,   289,  -547,   -50,   385,  -547,   298,   707,  -547,  1180,
    -547,  1066,  1087,  1163,   608,   617,   672,    78,   682,  -547,
     674,    78,  -547,   530,  -547,   681,  -547,  -547,    42,  -547,
      31,  -547,  -547,  -547,    78,   239,   241,    78,  1180,   665,
      78,   399,  -547,   530,   433,   472,   684,  -547,  1108,  -547,
    -547,  1180,   685,  1180,   152,   565,  -547,    78,  -547,   680,
    -547,  -547,  1129,   666,   472,  -547,  -547
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     3,     0,     2,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,    19,   324,     5,     6,    11,    12,    13,    14,     8,
       9,    24,    27,    31,    28,    29,    30,    32,    33,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,    34,    16,
      17,    15,    37,    68,    75,    85,    37,    52,    22,     0,
     149,   152,   148,    20,    88,    23,    58,    57,    91,     0,
      25,    94,     0,     0,   104,     0,   106,     0,   108,     0,
     110,     0,   112,     0,   114,     0,   116,     0,   118,     0,
     120,     0,   122,     0,   124,     0,   126,     0,   128,     0,
     130,     0,   132,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    21,     0,   150,     0,     0,    89,     0,
       0,     0,     0,   326,   154,   137,     0,   276,   271,   271,
       0,     0,     0,   258,   251,   277,     0,   138,   139,     0,
     271,     0,     0,     0,     0,   134,   136,    42,    35,    43,
      38,    72,    67,     0,     0,    69,    81,    74,    82,    76,
      84,    86,    36,    51,     0,    53,     0,   152,     0,    91,
      91,    26,   102,    93,    45,     0,     0,    95,    96,     0,
     154,     0,   237,     0,     0,   272,     0,   251,   251,   251,
      43,     0,   252,   240,   241,     0,   140,   142,   141,   143,
       0,     0,    47,    44,     0,    70,     0,     0,     0,    43,
       0,   153,   151,    92,    90,    87,     0,     0,    98,     0,
     342,   325,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   337,   328,     0,   182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   144,     0,    23,     0,     0,     0,
       0,     0,     0,   176,     0,   310,     0,     0,     0,   165,
     166,   178,   185,   161,   163,   155,   156,     0,   162,   158,
     159,   160,   179,   174,   180,   181,   164,   167,   169,   170,
     171,   168,   172,   173,   175,   177,   275,   273,   238,     0,
     239,     0,     0,     0,   253,   256,     0,   298,     0,     0,
      62,    43,    59,    61,     0,    40,     0,     0,    83,     0,
      49,    43,     0,     0,    65,     0,    66,     0,    97,   327,
     345,   343,   329,     0,   333,   332,   331,   334,   335,   330,
     296,   338,   336,   145,   236,   268,   269,   227,     0,     0,
       0,   279,   278,   314,   315,    23,   157,   216,     0,     0,
       0,    43,   197,     0,   199,   217,     0,   281,     0,   282,
       0,   302,     0,   303,     0,    23,     0,    23,     0,   245,
       0,     0,     0,   308,     0,     0,   311,   312,   265,   251,
     261,     0,   259,   146,     0,     0,     0,   274,    43,    43,
       0,     0,    43,    43,    60,    47,     0,     0,     0,    82,
       0,    77,     0,    54,     0,    46,     0,   100,    99,     0,
       0,     0,     0,     0,     0,   266,     0,     0,     0,     0,
     215,   214,   198,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,   283,
       0,     0,   304,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   321,   319,     0,   317,     0,
       0,     0,     0,     0,     0,   196,     0,   189,   190,     0,
     183,   260,   248,   250,   255,   257,   254,    47,    39,     0,
       0,    82,     0,    43,     0,    56,    55,     0,   344,   346,
     346,   297,   269,   270,     0,     0,     0,   200,   201,   203,
     202,   204,   184,   205,   206,   207,   208,   210,   209,   211,
     212,   213,   222,   221,     0,   220,     0,     0,   284,   280,
       0,   305,     0,     0,     0,     0,   289,     0,     0,   294,
       0,     0,   246,   247,     0,     0,   307,     0,   316,   318,
       0,   313,     0,   263,     0,   147,     0,   186,     0,     0,
       0,     0,     0,    41,     0,    73,     0,    43,     0,    49,
      50,   101,     0,     0,   339,   340,   267,     0,   230,   232,
     218,     0,     0,   285,   306,     0,     0,     0,     0,   288,
     290,     0,   293,   295,     0,     0,     0,     0,     0,     0,
       0,   262,   191,   192,   194,   193,   195,   249,    48,    82,
      49,     0,    78,   346,     0,   347,   228,   234,   224,   223,
     188,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,     0,   264,    43,    79,    82,   341,   348,     0,   225,
       0,   231,   301,   300,     0,     0,     0,     0,   244,     0,
       0,     0,    71,    43,   229,     0,     0,   235,     0,   287,
     292,   243,     0,   320,     0,    49,   226,     0,   299,     0,
     309,    80,     0,     0,     0,   242,   233
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -547,  -547,  -547,  -547,  -547,   742,   743,   744,  -547,  -547,
    -228,  -547,  -547,   625,  -547,   692,  -547,  -199,  -116,  -547,
    -393,  -546,  -547,  -547,  -547,  -547,     9,  -184,   441,   -69,
    -225,  -547,  -547,   578,  -547,  -547,  -547,   633,  -399,  -547,
    -547,  -547,  -547,   304,  -547,  -547,  -547,  -547,  -547,  -547,
    -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,
    -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,   641,
     -80,  -547,   626,  -547,   582,   591,  -547,   512,  -150,  -391,
       5,  -139,  -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,
    -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,  -547,   226,
    -547,  -547,  -547,  -193,  -547,  -547,  -547,  -547,  -547,   373,
    -547,  -547,  -547,   278,   137,  -547,  -547,  -547,  -547,  -547,
    -547,  -547,  -547,   158,  -547,  -547,   161,  -241,  -547,  -547,
     340,  -547,  -547,  -547,  -547,  -547,  -547,  -547,   316,  -547,
    -547,  -547,  -547,  -547,  -547,  -189,  -492
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    23,    24,   269,   270,   271,    28,    29,
      77,    30,   131,    31,    32,   115,   160,   212,   213,   226,
     315,   411,    33,   120,   175,   321,   272,   311,   312,   326,
     361,    34,   116,   165,   566,    35,   117,   169,   219,    36,
     118,    37,    79,   128,    38,   132,   187,   188,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    84,   106,   108,   155,
     273,   274,    57,    72,   125,   191,   275,   276,   362,   479,
     363,   364,   447,   524,   525,   279,   655,   639,   280,   281,
     617,   641,   282,    86,    90,    92,   104,   110,   283,   544,
      94,   562,    96,   201,   202,    98,   100,   401,   284,   392,
     285,   600,   286,   425,   194,   195,    88,   102,   287,   288,
     370,   289,   457,   537,   290,   459,   540,   341,   112,   291,
     374,   292,   293,   387,   471,   294,   295,   467,   468,   469,
      58,    82,   189,   342,   242,   332,   574
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     123,   304,   327,   135,   301,   302,   303,   137,   575,   352,
     493,   379,   488,   340,   161,   228,   186,   419,   367,   423,
     320,   145,   340,   612,   386,   147,    78,   148,   166,   143,
     316,   580,   317,   318,   419,   504,   322,   340,   424,     4,
     393,   277,   581,   475,   380,   129,   334,   335,   336,   337,
     338,   339,   278,    59,   371,   176,   192,   656,   178,   184,
      68,   210,   230,   388,   634,   573,   203,   162,   211,   204,
     130,   206,   207,   208,   209,   182,   173,    70,   229,   357,
     572,   167,   573,   144,   134,   556,    74,    61,   381,   382,
     390,   163,   567,   277,   563,   355,    76,   476,   368,   161,
     174,    60,   184,   277,   278,    75,    76,   369,   373,   416,
     164,   325,   405,   328,   298,   231,   300,   389,   296,   671,
      75,   636,   413,   121,   122,   307,   168,   168,   183,   184,
     406,   355,    76,   358,   372,   582,   166,    69,   179,   180,
     359,   308,   313,   383,   391,   313,    62,   313,   313,   319,
     330,   313,   185,   200,    71,   360,   256,    76,   324,   121,
     122,   465,   432,    75,    76,   344,   477,   603,   604,   605,
     606,   232,   233,   184,    63,   234,   163,   235,   236,   237,
     238,   239,   157,   587,   240,   412,   616,    64,   453,   170,
     241,   465,   589,   157,   501,   297,   472,   345,   477,   482,
     483,   357,    73,   486,   487,    80,   200,   200,   200,   417,
     633,   418,   347,    75,    76,   351,   590,   592,   449,   216,
      75,    76,   452,   490,   491,   217,   494,   331,   495,   551,
     496,   499,   500,   121,   122,   158,   653,   492,    75,    76,
     313,   593,   340,   217,   548,   313,   172,   654,   477,     8,
       9,    10,    11,    83,   218,   358,  -219,    12,   184,   159,
     353,   354,   359,    13,   376,   378,    85,  -219,    75,    76,
     159,   384,   577,   453,   346,   454,   196,   360,   256,    76,
     324,   121,   122,   324,   121,   122,   455,   205,    65,   348,
     522,   255,    75,   659,   569,   660,    87,   448,   477,   456,
     528,   355,    76,   531,    66,   564,   373,    67,   568,   558,
     559,   560,   561,    89,   434,   402,   436,   590,   438,   593,
     484,   485,   638,   365,   151,   478,   152,   366,   153,   601,
     477,   477,   477,   477,    17,  -286,   357,    81,   313,   313,
     154,   313,   365,   313,   503,   313,   366,    19,   615,   477,
      91,   309,    22,    93,   427,   428,   614,   478,   558,   559,
     560,   561,   450,   429,   430,   431,   451,    95,   610,   310,
     121,   122,   420,   421,   324,   121,   122,   583,    75,    76,
     584,   571,  -322,   613,   547,   355,    76,   365,   527,   637,
     358,   366,   355,    76,   466,   530,   473,   359,   200,  -291,
     480,   557,    97,   558,   559,   560,   561,   478,   355,    76,
     477,   372,   360,   375,    76,   324,   121,   122,   664,   559,
     313,   561,    99,   313,   357,   578,   579,   635,   433,   434,
     435,   436,   101,   438,   652,   310,   121,   122,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     520,   521,   523,   357,   665,   103,   529,   478,   105,   433,
     434,   435,   436,   107,   438,   324,   121,   122,   439,   440,
     441,   442,   443,   444,   445,   446,   466,   109,   358,   355,
      76,   111,   607,   224,   225,   359,   542,   543,   113,   478,
     478,   478,   478,   558,   559,   560,   561,   609,   114,   631,
     360,   256,    76,   324,   121,   122,   124,   358,   478,   126,
     127,   602,   133,   618,   359,   558,   559,   560,   561,   620,
     632,   558,   559,   560,   561,   134,   136,   193,   138,   360,
     377,    76,   324,   121,   122,   139,   433,   434,   435,   436,
     588,   438,   313,   591,   140,   439,   440,   441,   442,   443,
     444,   445,   446,   141,    15,   598,   142,   599,   146,   149,
     657,   150,   177,   197,   198,   433,   434,   435,   436,   478,
     438,   199,   214,   221,   299,   666,   441,   442,   443,   444,
     445,   446,   220,   223,   227,   184,   619,   305,   306,   323,
     621,   622,   623,   349,   676,   670,   314,   329,   505,   350,
     333,   385,   433,   434,   435,   436,   244,   438,     5,     6,
      75,   439,   440,   441,   442,   443,   444,   445,   446,   394,
     245,   395,   246,   396,   397,   398,   399,   247,   400,   403,
     248,   249,   648,   407,   408,   250,   651,   409,   410,   415,
     414,   419,   422,   366,   426,   461,   462,   464,   470,   658,
     463,   474,   661,   251,   252,   663,   391,   253,   134,   254,
     489,   497,   506,   498,   502,   438,   433,   434,   435,   436,
     526,   438,   672,   255,   532,   439,   440,   441,   442,   443,
     444,   445,   446,   256,    76,   533,   536,   535,   538,   257,
     258,   541,   259,   260,   261,   539,   545,   550,   262,   263,
     264,   244,   546,     5,     6,   552,   553,   555,   565,   217,
      20,    21,   265,   424,   570,   245,   585,   246,   586,   596,
     595,   611,   247,   597,   608,   248,   249,   626,   627,   628,
     250,   629,   266,   267,   640,   268,   647,   649,   650,   667,
     673,   669,   215,   662,   675,    25,    26,    27,   251,   252,
     404,   171,   253,   134,   343,   156,   181,   507,   119,   222,
     190,   433,   434,   435,   436,   356,   438,   594,   255,   481,
     439,   440,   441,   442,   443,   444,   445,   446,   256,    76,
     576,   243,   645,   549,   257,   258,   646,   259,   260,   261,
       0,     0,   458,   262,   263,   264,   534,     0,     0,   433,
     434,   435,   436,     0,   438,    20,    21,   265,   439,   440,
     441,   442,   443,   444,   445,   446,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   266,   267,     0,
     268,    -4,     1,     0,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,     0,     0,     0,    -4,    -4,    -4,    -4,     0,     0,
       0,    -4,    -4,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    -4,    -4,     0,     0,    -4,     0,     0,
       0,    -4,    -4,    -4,    -4,    -4,     0,     0,     0,     0,
       0,    -4,    -4,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,  -107,    12,  -103,  -105,     0,     0,     0,    13,  -123,
       0,     0,     0,     0,    -4,     0,     0,     0,     0,     0,
    -117,  -129,    -4,     0,    14,     0,     0,    -4,  -121,  -109,
    -111,  -113,    15,     0,     0,    -4,    -4,    -4,  -115,    16,
      -4,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    -4,    -4,     0,    -4,    -4,    -4,     0,     0,
       0,     0,   433,   434,   435,   436,     0,   438,     0,     0,
       0,  -131,   440,   441,   442,   443,   444,   445,   446,    17,
       0,     0,     0,     0,    18,     0,     0,     0,     0,     0,
       0,     0,    19,    20,    21,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   460,  -133,
    -135,     0,  -125,  -127,  -119,   433,   434,   435,   436,     0,
     438,     0,     0,     0,   439,   440,   441,   442,   443,   444,
     445,   446,   624,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,     0,   438,     0,     0,     0,   439,   440,
     441,   442,   443,   444,   445,   446,   625,     0,     0,     0,
       0,     0,     0,   433,   434,   435,   436,     0,   438,     0,
       0,     0,   439,   440,   441,   442,   443,   444,   445,   446,
     554,     0,     0,     0,   433,   434,   435,   436,     0,   438,
       0,     0,     0,   439,   440,   441,   442,   443,   444,   445,
     446,   630,     0,     0,     0,   433,   434,   435,   436,     0,
     438,     0,     0,     0,   439,   440,   441,   442,   443,   444,
     445,   446,   642,     0,     0,     0,   433,   434,   435,   436,
       0,   438,     0,     0,     0,   439,   440,   441,   442,   443,
     444,   445,   446,   643,     0,     0,     0,   433,   434,   435,
     436,     0,   438,     0,     0,     0,   439,   440,   441,   442,
     443,   444,   445,   446,   668,     0,     0,     0,   433,   434,
     435,   436,     0,   438,     0,     0,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   674,     0,     0,     0,   433,
     434,   435,   436,     0,   438,     0,     0,     0,   439,   440,
     441,   442,   443,   444,   445,   446,   433,   434,   435,   436,
     437,   438,     0,     0,     0,   439,   440,   441,   442,   443,
     444,   445,   446,   433,   434,   435,   436,     0,   438,     0,
     644,     0,   439,   440,   441,   442,   443,   444,   445,   446,
     433,   434,   435,   436,     0,   438,     0,     0,     0,   439,
     440,   441,   442,   443,   444,   445,   446
};

static const yytype_int16 yycheck[] =
{
      69,   200,   227,    83,   197,   198,   199,    87,   500,   250,
     409,     1,   405,   241,     1,     1,   132,    67,     1,    18,
     219,   101,   250,   569,   265,   105,    17,   107,     1,     1,
     214,    56,   216,   217,    67,   426,   220,   265,    37,     0,
     268,   191,    67,     1,    34,    67,   235,   236,   237,   238,
     239,   240,   191,    10,     1,   124,   136,    26,   127,    55,
       1,    57,     1,     1,   610,   115,   146,    54,    64,   149,
      92,   151,   152,   153,   154,     1,    54,     1,    64,     1,
     113,    54,   115,    55,    53,   476,     1,   102,    78,    79,
       1,    78,   491,   243,   487,    78,    79,    55,    81,     1,
      78,    11,    55,   253,   243,    78,    79,   257,   258,    62,
      97,   227,   311,   229,   194,    54,   196,    55,     1,   665,
      78,   613,   321,    81,    82,   205,   117,   118,    54,    55,
     314,    78,    79,    55,    81,   526,     1,    78,   129,   130,
      62,   210,   211,     1,    55,   214,    53,   216,   217,   218,
       1,   220,    78,   144,    78,    77,    78,    79,    80,    81,
      82,     1,   361,    78,    79,   245,   394,   558,   559,   560,
     561,   110,   111,    55,    53,   114,    78,   116,   117,   118,
     119,   120,     1,    55,   123,    67,   577,    53,    60,    54,
     129,     1,    54,     1,   422,    78,   389,     1,   426,   398,
     399,     1,    90,   402,   403,    81,   197,   198,   199,   325,
     609,   327,     1,    78,    79,     1,    78,    54,   368,    22,
      78,    79,   372,   407,   408,    28,   410,    78,   412,   470,
     414,   420,   421,    81,    82,    54,   635,    22,    78,    79,
     309,    78,   470,    28,    54,   314,    54,   638,   476,     6,
       7,     8,     9,    15,    57,    55,    56,    14,    55,    78,
     251,   252,    62,    20,   259,   260,    16,    67,    78,    79,
      78,   262,    23,    60,    78,    62,   139,    77,    78,    79,
      80,    81,    82,    80,    81,    82,    73,   150,    53,    78,
      90,    68,    78,    54,   493,    54,    13,   366,   526,    86,
     450,    78,    79,   453,    53,   489,   456,    53,   492,    60,
      61,    62,    63,    41,    61,   306,    63,    78,    65,    78,
     400,   401,    24,    55,    13,   394,    15,    59,    17,   554,
     558,   559,   560,   561,    91,    67,     1,    53,   407,   408,
      29,   410,    55,   412,   424,   414,    59,   104,   573,   577,
      42,    62,   109,    43,   349,   350,    62,   426,    60,    61,
      62,    63,    60,   358,   359,   360,    64,    50,   567,    80,
      81,    82,   112,   113,    80,    81,    82,   527,    78,    79,
     530,   497,    55,   572,    57,    78,    79,    55,    81,   614,
      55,    59,    78,    79,   385,    81,   391,    62,   389,    67,
     395,    58,    32,    60,    61,    62,    63,   476,    78,    79,
     638,    81,    77,    78,    79,    80,    81,    82,    19,    61,
     489,    63,   126,   492,     1,   505,   506,   611,    60,    61,
      62,    63,    40,    65,   633,    80,    81,    82,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,     1,   653,    21,   451,   526,   124,    60,
      61,    62,    63,   125,    65,    80,    81,    82,    69,    70,
      71,    72,    73,    74,    75,    76,   467,    33,    55,    78,
      79,    83,   562,   179,   180,    62,    98,    99,   121,   558,
     559,   560,   561,    60,    61,    62,    63,   566,   122,    23,
      77,    78,    79,    80,    81,    82,    57,    55,   577,    67,
      57,    56,    53,    90,    62,    60,    61,    62,    63,    58,
     600,    60,    61,    62,    63,    53,    78,    31,    78,    77,
      78,    79,    80,    81,    82,    78,    60,    61,    62,    63,
     535,    65,   611,   538,    78,    69,    70,    71,    72,    73,
      74,    75,    76,    78,    44,   550,    78,   552,    78,    78,
     640,    78,    78,    55,    55,    60,    61,    62,    63,   638,
      65,    55,    22,    58,    67,   655,    71,    72,    73,    74,
      75,    76,    64,    58,    64,    55,   581,    56,    67,    12,
     585,   586,   587,    55,   674,   664,    73,    78,    56,    55,
      78,    53,    60,    61,    62,    63,     1,    65,     3,     4,
      78,    69,    70,    71,    72,    73,    74,    75,    76,    57,
      15,    64,    17,    67,    78,    56,    56,    22,    56,    58,
      25,    26,   627,    23,    23,    30,   631,    58,    73,    56,
      67,    67,    67,    59,    64,    78,    57,    24,   108,   644,
      64,   114,   647,    48,    49,   650,    55,    52,    53,    54,
      67,    71,    56,    78,    45,    65,    60,    61,    62,    63,
      57,    65,   667,    68,    71,    69,    70,    71,    72,    73,
      74,    75,    76,    78,    79,    73,    78,    67,    67,    84,
      85,    57,    87,    88,    89,    78,    78,    55,    93,    94,
      95,     1,    78,     3,     4,    22,    56,    78,    19,    28,
     105,   106,   107,    37,    71,    15,    55,    17,    55,    61,
      58,    23,    22,    78,    71,    25,    26,    58,    64,    35,
      30,    58,   127,   128,    27,   130,    64,    55,    64,    55,
      60,    56,   164,    78,    78,     3,     3,     3,    48,    49,
     309,   118,    52,    53,    54,   114,   131,    56,    66,   177,
     134,    60,    61,    62,    63,   253,    65,   541,    68,   396,
      69,    70,    71,    72,    73,    74,    75,    76,    78,    79,
     502,   190,   624,   467,    84,    85,   625,    87,    88,    89,
      -1,    -1,    53,    93,    94,    95,   456,    -1,    -1,    60,
      61,    62,    63,    -1,    65,   105,   106,   107,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,    -1,
     130,     0,     1,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    13,    14,    15,    16,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      32,    33,    91,    -1,    36,    -1,    -1,    96,    40,    41,
      42,    43,    44,    -1,    -1,   104,   105,   106,    50,    51,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,   122,    -1,   124,   125,   126,    -1,    -1,
      -1,    -1,    60,    61,    62,    63,    -1,    65,    -1,    -1,
      -1,    83,    70,    71,    72,    73,    74,    75,    76,    91,
      -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,   121,
     122,    -1,   124,   125,   126,    60,    61,    62,    63,    -1,
      65,    -1,    -1,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    53,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    65,    -1,    -1,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      56,    -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,
      -1,    -1,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    56,    -1,    -1,    -1,    60,    61,    62,    63,    -1,
      65,    -1,    -1,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    -1,    -1,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    56,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    65,    -1,    -1,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    56,    -1,    -1,    -1,    60,    61,
      62,    63,    -1,    65,    -1,    -1,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    56,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    65,    -1,    -1,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    60,    61,    62,    63,
      64,    65,    -1,    -1,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    60,    61,    62,    63,    -1,    65,    -1,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      60,    61,    62,    63,    -1,    65,    -1,    -1,    -1,    69,
      70,    71,    72,    73,    74,    75,    76
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,   134,   135,     0,     3,     4,     5,     6,     7,
       8,     9,    14,    20,    36,    44,    51,    91,    96,   104,
     105,   106,   109,   136,   137,   138,   139,   140,   141,   142,
     144,   146,   147,   155,   164,   168,   172,   174,   177,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   205,   273,    10,
      11,   102,    53,    53,    53,    53,    53,    53,     1,    78,
       1,    78,   206,    90,     1,    78,    79,   143,   159,   175,
      81,    53,   274,    15,   199,    16,   226,    13,   249,    41,
     227,    42,   228,    43,   233,    50,   235,    32,   238,   126,
     239,    40,   250,    21,   229,   124,   200,   125,   201,    33,
     230,    83,   261,   121,   122,   148,   165,   169,   173,   148,
     156,    81,    82,   162,    57,   207,    67,    57,   176,    67,
      92,   145,   178,    53,    53,   203,    78,   203,    78,    78,
      78,    78,    78,     1,    55,   203,    78,   203,   203,    78,
      78,    13,    15,    17,    29,   202,   202,     1,    54,    78,
     149,     1,    54,    78,    97,   166,     1,    54,   159,   170,
      54,   170,    54,    54,    78,   157,   162,    78,   162,   159,
     159,   146,     1,    54,    55,    78,   151,   179,   180,   275,
     205,   208,   203,    31,   247,   248,   247,    55,    55,    55,
     159,   236,   237,   203,   203,   247,   203,   203,   203,   203,
      57,    64,   150,   151,    22,   166,    22,    28,    57,   171,
      64,    58,   207,    58,   176,   176,   152,    64,     1,    64,
       1,    54,   110,   111,   114,   116,   117,   118,   119,   120,
     123,   129,   277,   208,     1,    15,    17,    22,    25,    26,
      30,    48,    49,    52,    54,    68,    78,    84,    85,    87,
      88,    89,    93,    94,    95,   107,   127,   128,   130,   138,
     139,   140,   159,   203,   204,   209,   210,   211,   214,   218,
     221,   222,   225,   231,   241,   243,   245,   251,   252,   254,
     257,   262,   264,   265,   268,   269,     1,    78,   203,    67,
     203,   236,   236,   236,   150,    56,    67,   203,   162,    62,
      80,   160,   161,   162,    73,   153,   160,   160,   160,   162,
     150,   158,   160,    12,    80,   151,   162,   163,   151,    78,
       1,    78,   278,    78,   278,   278,   278,   278,   278,   278,
     143,   260,   276,    54,   203,     1,    78,     1,    78,    55,
      55,     1,   260,   159,   159,    78,   210,     1,    55,    62,
      77,   163,   211,   213,   214,    55,    59,     1,    81,   211,
     253,     1,    81,   211,   263,    78,   213,    78,   213,     1,
      34,    78,    79,     1,   159,    53,   260,   266,     1,    55,
       1,    55,   242,   143,    57,    64,    67,    78,    56,    56,
      56,   240,   159,    58,   161,   150,   160,    23,    23,    58,
      73,   154,    67,   150,    67,    56,    62,   151,   151,    67,
     112,   113,    67,    18,    37,   246,    64,   213,   213,   213,
     213,   213,   150,    60,    61,    62,    63,    64,    65,    69,
      70,    71,    72,    73,    74,    75,    76,   215,   162,   211,
      60,    64,   211,    60,    62,    73,    86,   255,    53,   258,
      53,    78,    57,    64,    24,     1,   159,   270,   271,   272,
     108,   267,   236,   213,   114,     1,    55,   143,   162,   212,
     213,   242,   150,   150,   203,   203,   150,   150,   153,    67,
     160,   160,    22,   171,   160,   160,   160,    71,    78,   278,
     278,   143,    45,   203,   212,    56,    56,    56,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,    90,   213,   216,   217,    57,    81,   211,   213,
      81,   211,    71,    73,   263,    67,    78,   256,    67,    78,
     259,    57,    98,    99,   232,    78,    78,    57,    54,   271,
      55,   260,    22,    56,    56,    78,   212,    58,    60,    61,
      62,    63,   234,   153,   160,    19,   167,   171,   160,   150,
      71,   151,   113,   115,   279,   279,   246,    23,   203,   203,
      56,    67,   212,   211,   211,    55,    55,    55,   213,    54,
      78,   213,    54,    78,   232,    58,    61,    78,   213,   213,
     244,   163,    56,   212,   212,   212,   212,   203,    71,   162,
     150,    23,   154,   278,    62,   163,   212,   223,    90,   213,
      58,   213,   213,   213,    53,    53,    58,    64,    35,    58,
      56,    23,   203,   171,   154,   160,   279,   163,    24,   220,
      27,   224,    56,    56,    67,   256,   259,    64,   213,    55,
      64,   213,   150,   171,   212,   219,    26,   203,   213,    54,
      54,   213,    78,   213,    19,   150,   203,    55,    56,    56,
     162,   154,   213,    60,    56,    78,   203
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   133,   134,   134,   135,   135,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   137,   138,   139,   140,   140,
     141,   142,   142,   143,   144,   145,   144,   146,   146,   146,
     146,   146,   146,   146,   146,   147,   147,   148,   148,   149,
     149,   149,   149,   150,   150,   152,   151,   153,   153,   154,
     154,   155,   156,   156,   157,   158,   158,   159,   159,   160,
     160,   161,   161,   162,   162,   163,   163,   164,   165,   165,
     165,   166,   166,   167,   168,   169,   169,   170,   170,   170,
     170,   170,   171,   171,   172,   173,   173,   174,   174,   175,
     175,   176,   176,   177,   178,   178,   178,   179,   179,   180,
     180,   180,   180,   182,   181,   183,   181,   184,   181,   185,
     181,   186,   181,   187,   181,   188,   181,   189,   181,   190,
     181,   191,   181,   192,   181,   193,   181,   194,   181,   195,
     181,   196,   181,   197,   181,   198,   181,   199,   200,   201,
     202,   202,   202,   202,   203,   203,   204,   204,   205,   205,
     206,   206,   207,   207,   208,   208,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   210,   210,   211,   211,   211,   211,   212,
     212,   212,   212,   212,   212,   212,   212,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   215,   214,   216,
     216,   217,   217,   217,   217,   219,   218,   218,   220,   220,
     221,   222,   223,   223,   224,   224,   225,   226,   227,   228,
     229,   230,   231,   231,   231,   231,   232,   232,   234,   233,
     235,   236,   236,   237,   237,   238,   240,   239,   239,   241,
     241,   241,   242,   244,   243,   243,   245,   245,   245,   246,
     246,   247,   247,   248,   248,   248,   249,   250,   251,   251,
     252,   252,   253,   253,   253,   253,   255,   254,   254,   256,
     256,   258,   257,   257,   259,   259,   260,   260,   261,   262,
     262,   262,   262,   263,   263,   263,   263,   264,   264,   265,
     266,   266,   267,   267,   268,   268,   269,   270,   270,   271,
     271,   271,   272,   272,   274,   273,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   276,   276,   277,
     277,   277,   277,   278,   278,   278,   279,   279,   279
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     1,     1,
       2,     3,     2,     1,     1,     0,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     0,     2,     5,
       3,     6,     1,     0,     1,     0,     4,     0,     5,     0,
       3,     4,     0,     2,     4,     3,     3,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     4,     0,     2,
       3,     9,     1,     1,     4,     0,     2,     4,     7,     8,
      11,     1,     0,     2,     4,     0,     2,     5,     2,     2,
       4,     0,     3,     4,     0,     2,     2,     3,     2,     4,
       4,     6,     1,     0,     2,     0,     2,     0,     2,     0,
       2,     0,     2,     0,     2,     0,     2,     0,     2,     0,
       2,     0,     2,     0,     2,     0,     2,     0,     2,     0,
       2,     0,     2,     0,     3,     0,     3,     2,     2,     2,
       2,     2,     2,     2,     3,     4,     2,     4,     2,     2,
       2,     4,     0,     3,     0,     2,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     1,     4,     3,     6,     1,
       1,     3,     3,     3,     3,     3,     1,     1,     2,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     1,     0,     5,     0,
       1,     1,     1,     3,     3,     0,     9,     2,     0,     2,
       5,     7,     0,     7,     0,     2,     2,     3,     4,     4,
       3,     3,    11,     8,     7,     2,     1,     1,     0,     8,
       6,     0,     1,     2,     4,     6,     0,     6,     2,     2,
       3,     2,     4,     0,     6,     2,     3,     5,     2,     0,
       2,     0,     1,     2,     3,     2,     2,     2,     2,     2,
       4,     2,     1,     2,     3,     4,     0,     8,     5,     1,
       2,     0,     8,     5,     1,     2,     1,     3,     4,     9,
       7,     7,     2,     1,     2,     3,     4,     4,     2,     9,
       0,     1,     0,     2,     2,     2,     4,     1,     2,     1,
       6,     1,     1,     4,     0,     5,     0,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     0,     1,     5,
       5,     7,     1,     1,     3,     1,     0,     2,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 120 "parse1.y" /* yacc.c:1646  */
    {diag("Illegal block", (char *)0);}
#line 2123 "parse1.c" /* yacc.c:1646  */
    break;

  case 4:
#line 123 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2129 "parse1.c" /* yacc.c:1646  */
    break;

  case 15:
#line 131 "parse1.y" /* yacc.c:1646  */
    {P1{if (!title) title = (yyvsp[0].qp);}}
#line 2135 "parse1.c" /* yacc.c:1646  */
    break;

  case 18:
#line 138 "parse1.y" /* yacc.c:1646  */
    { unitonflag = unitflagsave = 1;}
#line 2141 "parse1.c" /* yacc.c:1646  */
    break;

  case 19:
#line 140 "parse1.y" /* yacc.c:1646  */
    {unitonflag = unitflagsave = 0;}
#line 2147 "parse1.c" /* yacc.c:1646  */
    break;

  case 20:
#line 143 "parse1.y" /* yacc.c:1646  */
    {P1{include_file((yyvsp[0].qp));}}
#line 2153 "parse1.c" /* yacc.c:1646  */
    break;

  case 21:
#line 146 "parse1.y" /* yacc.c:1646  */
    {P1{define_value((yyvsp[-1].qp), (yyvsp[0].qp));}}
#line 2159 "parse1.c" /* yacc.c:1646  */
    break;

  case 22:
#line 147 "parse1.y" /* yacc.c:1646  */
    {myerr("syntax: DEFINE name integer");}
#line 2165 "parse1.c" /* yacc.c:1646  */
    break;

  case 23:
#line 150 "parse1.y" /* yacc.c:1646  */
    {P1{(yyvsp[0].qp)->element = (void *)checklocal(SYM((yyvsp[0].qp)));}}
#line 2171 "parse1.c" /* yacc.c:1646  */
    break;

  case 25:
#line 153 "parse1.y" /* yacc.c:1646  */
    {declare_level = atoi(STR((yyvsp[0].qp)));}
#line 2177 "parse1.c" /* yacc.c:1646  */
    break;

  case 26:
#line 154 "parse1.y" /* yacc.c:1646  */
    {declare_level = 0;}
#line 2183 "parse1.c" /* yacc.c:1646  */
    break;

  case 37:
#line 163 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2189 "parse1.c" /* yacc.c:1646  */
    break;

  case 39:
#line 170 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(7, (yyvsp[-4].qp), (yyvsp[-1].qp), ITEM0, (yyvsp[-2].qp),ITEM0,ITEM0,ITEM0); declare(modlunitCONST, (yyvsp[-4].qp), (yyval.qp));}}
#line 2195 "parse1.c" /* yacc.c:1646  */
    break;

  case 40:
#line 172 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(7, (yyvsp[-2].qp), (yyvsp[-1].qp), ITEM0,ITEM0,ITEM0,ITEM0,ITEM0); declare(modlunitCONST, (yyvsp[-2].qp), (yyval.qp));}}
#line 2201 "parse1.c" /* yacc.c:1646  */
    break;

  case 41:
#line 174 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(7, (yyvsp[-5].qp), (yyvsp[-1].qp), (yyvsp[-3].qp), ITEM0, ITEM0,ITEM0,ITEM0); declare(modlunitCONST, (yyvsp[-5].qp), (yyval.qp));}}
#line 2207 "parse1.c" /* yacc.c:1646  */
    break;

  case 42:
#line 175 "parse1.y" /* yacc.c:1646  */
    {myerr("name = number");}
#line 2213 "parse1.c" /* yacc.c:1646  */
    break;

  case 43:
#line 178 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2219 "parse1.c" /* yacc.c:1646  */
    break;

  case 45:
#line 181 "parse1.y" /* yacc.c:1646  */
    {P1{lex_units();}}
#line 2225 "parse1.c" /* yacc.c:1646  */
    break;

  case 46:
#line 182 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[-1].qp); lastok = (yyvsp[0].qp); P2{unitcheck(STR((yyvsp[-1].qp)));}}
#line 2231 "parse1.c" /* yacc.c:1646  */
    break;

  case 47:
#line 185 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2237 "parse1.c" /* yacc.c:1646  */
    break;

  case 49:
#line 189 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2243 "parse1.c" /* yacc.c:1646  */
    break;

  case 52:
#line 195 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2249 "parse1.c" /* yacc.c:1646  */
    break;

  case 54:
#line 199 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(3,(yyvsp[-3].qp), (yyvsp[0].qp), (yyvsp[-1].qp)); declare(STEP1, (yyvsp[-3].qp), (yyval.qp));}}
#line 2255 "parse1.c" /* yacc.c:1646  */
    break;

  case 57:
#line 204 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);}
#line 2261 "parse1.c" /* yacc.c:1646  */
    break;

  case 58:
#line 205 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);}
#line 2267 "parse1.c" /* yacc.c:1646  */
    break;

  case 59:
#line 208 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);}
#line 2273 "parse1.c" /* yacc.c:1646  */
    break;

  case 60:
#line 210 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);}
#line 2279 "parse1.c" /* yacc.c:1646  */
    break;

  case 65:
#line 218 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);}
#line 2285 "parse1.c" /* yacc.c:1646  */
    break;

  case 66:
#line 220 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);}
#line 2291 "parse1.c" /* yacc.c:1646  */
    break;

  case 68:
#line 225 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2297 "parse1.c" /* yacc.c:1646  */
    break;

  case 69:
#line 227 "parse1.y" /* yacc.c:1646  */
    {P1{declare(INDEP, ITMA((yyvsp[0].qp))[0], (yyvsp[0].qp));}}
#line 2303 "parse1.c" /* yacc.c:1646  */
    break;

  case 70:
#line 229 "parse1.y" /* yacc.c:1646  */
    {P1{ITMA((yyvsp[0].qp))[7] = (yyvsp[-1].qp); declare(INDEP, ITMA((yyvsp[0].qp))[0], (yyvsp[0].qp));}}
#line 2309 "parse1.c" /* yacc.c:1646  */
    break;

  case 71:
#line 233 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(8, (yyvsp[-8].qp), (yyvsp[0].qp), (yyvsp[-6].qp), (yyvsp[-4].qp), (yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[-1].qp), ITEM0);}}
#line 2315 "parse1.c" /* yacc.c:1646  */
    break;

  case 72:
#line 234 "parse1.y" /* yacc.c:1646  */
    {myerr("name FROM number TO number WITH integer");}
#line 2321 "parse1.c" /* yacc.c:1646  */
    break;

  case 75:
#line 241 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2327 "parse1.c" /* yacc.c:1646  */
    break;

  case 76:
#line 243 "parse1.y" /* yacc.c:1646  */
    {P1{declare(DEP, ITMA((yyvsp[0].qp))[0], (yyvsp[0].qp));}}
#line 2333 "parse1.c" /* yacc.c:1646  */
    break;

  case 77:
#line 246 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(7, (yyvsp[-3].qp), (yyvsp[-1].qp), ITEM0, ITEM0, ITEM0, (yyvsp[-2].qp), ITEM0);}}
#line 2339 "parse1.c" /* yacc.c:1646  */
    break;

  case 78:
#line 248 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(7, (yyvsp[-6].qp), (yyvsp[-1].qp), (yyvsp[-4].qp), ITEM0, ITEM0, (yyvsp[-2].qp), ITEM0);}}
#line 2345 "parse1.c" /* yacc.c:1646  */
    break;

  case 79:
#line 250 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(7, (yyvsp[-7].qp), (yyvsp[-1].qp), ITEM0, (yyvsp[-5].qp), (yyvsp[-3].qp), (yyvsp[-2].qp), ITEM0);}}
#line 2351 "parse1.c" /* yacc.c:1646  */
    break;

  case 80:
#line 252 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(7, (yyvsp[-10].qp), (yyvsp[-1].qp), (yyvsp[-8].qp), (yyvsp[-5].qp), (yyvsp[-3].qp), (yyvsp[-2].qp), ITEM0);}}
#line 2357 "parse1.c" /* yacc.c:1646  */
    break;

  case 81:
#line 253 "parse1.y" /* yacc.c:1646  */
    {
diag("name FROM number TO number START number\n",
"FROM...TO and START are optional, name can be name[integer]\n");}
#line 2365 "parse1.c" /* yacc.c:1646  */
    break;

  case 82:
#line 257 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2371 "parse1.c" /* yacc.c:1646  */
    break;

  case 83:
#line 258 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2377 "parse1.c" /* yacc.c:1646  */
    break;

  case 85:
#line 263 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2383 "parse1.c" /* yacc.c:1646  */
    break;

  case 86:
#line 265 "parse1.y" /* yacc.c:1646  */
    {P1{declare(STAT, ITMA((yyvsp[0].qp))[0], (yyvsp[0].qp));}}
#line 2389 "parse1.c" /* yacc.c:1646  */
    break;

  case 88:
#line 268 "parse1.y" /* yacc.c:1646  */
    { myerr("PLOT namelist VS name");}
#line 2395 "parse1.c" /* yacc.c:1646  */
    break;

  case 91:
#line 274 "parse1.y" /* yacc.c:1646  */
    { (yyval.qp) = ITEM0;}
#line 2401 "parse1.c" /* yacc.c:1646  */
    break;

  case 92:
#line 276 "parse1.y" /* yacc.c:1646  */
    { (yyval.qp) = (yyvsp[-1].qp);}
#line 2407 "parse1.c" /* yacc.c:1646  */
    break;

  case 94:
#line 281 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2413 "parse1.c" /* yacc.c:1646  */
    break;

  case 97:
#line 286 "parse1.y" /* yacc.c:1646  */
    {P1{install_units(STR((yyvsp[-2].qp)), STR((yyvsp[0].qp)));}}
#line 2419 "parse1.c" /* yacc.c:1646  */
    break;

  case 98:
#line 287 "parse1.y" /* yacc.c:1646  */
    {myerr("Unit definition syntax: (units) = (units)");}
#line 2425 "parse1.c" /* yacc.c:1646  */
    break;

  case 99:
#line 290 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(3, (yyvsp[-3].qp), (yyvsp[0].qp), (yyvsp[-1].qp)); declare(UFACTOR, (yyvsp[-3].qp), (yyval.qp));}}
#line 2431 "parse1.c" /* yacc.c:1646  */
    break;

  case 100:
#line 292 "parse1.y" /* yacc.c:1646  */
    {P1{Item *q; double d, unit_mag();
		    Unit_push(STR((yyvsp[-1].qp)));
			Unit_push(STR((yyvsp[0].qp))); unit_div();
			dimensionless();
			Sprintf(buf, "%g", unit_mag());
			(yyval.qp) = itemarray(3, (yyvsp[-3].qp), (yyvsp[0].qp), lappendstr(misc, buf));
/*printf("%s has value %s and units (%s)\n", SYM($1)->name, buf, STR($5));*/
		    unit_pop();
		    declare(UFACTOR, (yyvsp[-3].qp), (yyval.qp));
		}}
#line 2446 "parse1.c" /* yacc.c:1646  */
    break;

  case 101:
#line 303 "parse1.y" /* yacc.c:1646  */
    {P1{ Item *q; double unit_mag();
		    Unit_push(STR((yyvsp[-3].qp))); Unit_push(STR((yyvsp[0].qp))); unit_div();
		    q = lappendstr(misc, unit_str());
		    dimensionless();
		    Sprintf(buf, "%g", 1./unit_mag());
		    (yyval.qp) = itemarray(3, (yyvsp[-5].qp), lappendstr(misc, buf), q),
/*printf("%s has value %s and units (%s)\n", SYM($1)->name, STR(q), buf );*/
		    unit_pop();
		    declare(UFACTOR, (yyvsp[-5].qp), (yyval.qp));
		}}
#line 2461 "parse1.c" /* yacc.c:1646  */
    break;

  case 102:
#line 313 "parse1.y" /* yacc.c:1646  */
    {myerr("Unit factor syntax: examples:\n\
foot2inch = (foot) -> (inch)\n\
F = 96520 (coulombs)\n\
R = (k-mole) (joule/degC)");
		}
#line 2471 "parse1.c" /* yacc.c:1646  */
    break;

  case 103:
#line 319 "parse1.y" /* yacc.c:1646  */
    {blocktype = INITIAL1;}
#line 2477 "parse1.c" /* yacc.c:1646  */
    break;

  case 104:
#line 320 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2483 "parse1.c" /* yacc.c:1646  */
    break;

  case 105:
#line 321 "parse1.y" /* yacc.c:1646  */
    {lexcontext = NONLINEAR; blocktype = DERIVATIVE;}
#line 2489 "parse1.c" /* yacc.c:1646  */
    break;

  case 106:
#line 322 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2495 "parse1.c" /* yacc.c:1646  */
    break;

  case 107:
#line 323 "parse1.y" /* yacc.c:1646  */
    {blocktype = BREAKPOINT;}
#line 2501 "parse1.c" /* yacc.c:1646  */
    break;

  case 108:
#line 324 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2507 "parse1.c" /* yacc.c:1646  */
    break;

  case 109:
#line 325 "parse1.y" /* yacc.c:1646  */
    {lexcontext = blocktype = LINEAR;}
#line 2513 "parse1.c" /* yacc.c:1646  */
    break;

  case 110:
#line 326 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2519 "parse1.c" /* yacc.c:1646  */
    break;

  case 111:
#line 327 "parse1.y" /* yacc.c:1646  */
    {lexcontext = blocktype = NONLINEAR;}
#line 2525 "parse1.c" /* yacc.c:1646  */
    break;

  case 112:
#line 328 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2531 "parse1.c" /* yacc.c:1646  */
    break;

  case 113:
#line 329 "parse1.y" /* yacc.c:1646  */
    {blocktype = FUNCTION1;}
#line 2537 "parse1.c" /* yacc.c:1646  */
    break;

  case 114:
#line 330 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2543 "parse1.c" /* yacc.c:1646  */
    break;

  case 115:
#line 331 "parse1.y" /* yacc.c:1646  */
    {blocktype = FUNCTION_TABLE;}
#line 2549 "parse1.c" /* yacc.c:1646  */
    break;

  case 116:
#line 332 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2555 "parse1.c" /* yacc.c:1646  */
    break;

  case 117:
#line 333 "parse1.y" /* yacc.c:1646  */
    {blocktype = PROCEDURE;}
#line 2561 "parse1.c" /* yacc.c:1646  */
    break;

  case 118:
#line 334 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2567 "parse1.c" /* yacc.c:1646  */
    break;

  case 119:
#line 335 "parse1.y" /* yacc.c:1646  */
    {blocktype = PROCEDURE;}
#line 2573 "parse1.c" /* yacc.c:1646  */
    break;

  case 120:
#line 336 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2579 "parse1.c" /* yacc.c:1646  */
    break;

  case 121:
#line 337 "parse1.y" /* yacc.c:1646  */
    {blocktype = TERMINAL;}
#line 2585 "parse1.c" /* yacc.c:1646  */
    break;

  case 122:
#line 338 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2591 "parse1.c" /* yacc.c:1646  */
    break;

  case 123:
#line 339 "parse1.y" /* yacc.c:1646  */
    {blocktype = DISCRETE;}
#line 2597 "parse1.c" /* yacc.c:1646  */
    break;

  case 124:
#line 340 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2603 "parse1.c" /* yacc.c:1646  */
    break;

  case 125:
#line 341 "parse1.y" /* yacc.c:1646  */
    {blocktype = CONSTRUCTOR;}
#line 2609 "parse1.c" /* yacc.c:1646  */
    break;

  case 126:
#line 342 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2615 "parse1.c" /* yacc.c:1646  */
    break;

  case 127:
#line 343 "parse1.y" /* yacc.c:1646  */
    {blocktype = DESTRUCTOR;}
#line 2621 "parse1.c" /* yacc.c:1646  */
    break;

  case 128:
#line 344 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2627 "parse1.c" /* yacc.c:1646  */
    break;

  case 129:
#line 345 "parse1.y" /* yacc.c:1646  */
    {lexcontext = blocktype = PARTIAL;}
#line 2633 "parse1.c" /* yacc.c:1646  */
    break;

  case 130:
#line 346 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2639 "parse1.c" /* yacc.c:1646  */
    break;

  case 131:
#line 347 "parse1.y" /* yacc.c:1646  */
    {lexcontext = blocktype = KINETIC;
		P3 R0{unitflagsave=unitonflag; unitonflag=0;}}
#line 2646 "parse1.c" /* yacc.c:1646  */
    break;

  case 132:
#line 350 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp); P3{ R0{unitonflag=unitflagsave;}
		R1{clear_compartlist();} parse_restart((yyval.qp), 1);}}
#line 2653 "parse1.c" /* yacc.c:1646  */
    break;

  case 133:
#line 352 "parse1.y" /* yacc.c:1646  */
    {blocktype = BEFORE;}
#line 2659 "parse1.c" /* yacc.c:1646  */
    break;

  case 134:
#line 352 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[-1].qp);}
#line 2665 "parse1.c" /* yacc.c:1646  */
    break;

  case 135:
#line 353 "parse1.y" /* yacc.c:1646  */
    {blocktype = AFTER;}
#line 2671 "parse1.c" /* yacc.c:1646  */
    break;

  case 136:
#line 353 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[-1].qp);}
#line 2677 "parse1.c" /* yacc.c:1646  */
    break;

  case 144:
#line 367 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[-1].qp);}
#line 2683 "parse1.c" /* yacc.c:1646  */
    break;

  case 145:
#line 369 "parse1.y" /* yacc.c:1646  */
    {P1{poplocal();}}
#line 2689 "parse1.c" /* yacc.c:1646  */
    break;

  case 146:
#line 372 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0; conductance_seen_ = 1;}
#line 2695 "parse1.c" /* yacc.c:1646  */
    break;

  case 147:
#line 374 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0; conductance_seen_ = 1;}
#line 2701 "parse1.c" /* yacc.c:1646  */
    break;

  case 148:
#line 376 "parse1.y" /* yacc.c:1646  */
    { if (blocktype == BREAKPOINT) breakpoint_local_seen_ = 1; }
#line 2707 "parse1.c" /* yacc.c:1646  */
    break;

  case 149:
#line 377 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal LOCAL declaration");}
#line 2713 "parse1.c" /* yacc.c:1646  */
    break;

  case 150:
#line 380 "parse1.y" /* yacc.c:1646  */
    {P1{pushlocal((yyvsp[-1].qp), (yyvsp[0].qp));}}
#line 2719 "parse1.c" /* yacc.c:1646  */
    break;

  case 151:
#line 382 "parse1.y" /* yacc.c:1646  */
    {P1{install_local((yyvsp[-1].qp), (yyvsp[0].qp));}}
#line 2725 "parse1.c" /* yacc.c:1646  */
    break;

  case 152:
#line 385 "parse1.y" /* yacc.c:1646  */
    { (yyval.qp) = ITEM0;}
#line 2731 "parse1.c" /* yacc.c:1646  */
    break;

  case 153:
#line 387 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[-1].qp);}
#line 2737 "parse1.c" /* yacc.c:1646  */
    break;

  case 154:
#line 390 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2743 "parse1.c" /* yacc.c:1646  */
    break;

  case 182:
#line 420 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal statement");}
#line 2749 "parse1.c" /* yacc.c:1646  */
    break;

  case 183:
#line 423 "parse1.y" /* yacc.c:1646  */
    {
		  P3 {
			if (SYM((yyvsp[-2].qp))->subtype & LOCL) {
				SYM((yyvsp[-2].qp))->u.str = stralloc(unit_str(), (char *)0);
				unit_pop();
			}else{
			   unit_push((yyvsp[-2].qp)); unit_swap();
			   unit_cmp((yyvsp[-2].qp), (yyvsp[-1].qp), lastok); unit_pop();
			}
		  }
		}
#line 2765 "parse1.c" /* yacc.c:1646  */
    break;

  case 184:
#line 435 "parse1.y" /* yacc.c:1646  */
    {
		  P3 {
			unit_cmp((yyvsp[-2].qp), (yyvsp[-1].qp), lastok); unit_pop();
		  }
		}
#line 2775 "parse1.c" /* yacc.c:1646  */
    break;

  case 185:
#line 442 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);
		  P1{SYM((yyvsp[0].qp))->usage |= DEP;}
		  P2{ if (SYM((yyvsp[0].qp))->subtype & ARRAY) {
			myerr("variable needs an index");}
		  }
		}
#line 2786 "parse1.c" /* yacc.c:1646  */
    break;

  case 186:
#line 449 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp); 
		  P1{SYM((yyvsp[-3].qp))->usage |= DEP;}
		  P2{ if ((SYM((yyvsp[-3].qp))->subtype & ARRAY) == 0)
			{myerr("variable is not an array");}
		  }
		}
#line 2797 "parse1.c" /* yacc.c:1646  */
    break;

  case 187:
#line 456 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);
		  P1{SYM((yyvsp[-2].qp))->usage |= DEP;}
		  P2{ if (SYM((yyvsp[-2].qp))->subtype & ARRAY) {
			myerr("variable needs an index");}
		  }
		}
#line 2808 "parse1.c" /* yacc.c:1646  */
    break;

  case 188:
#line 464 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);
		  P1{SYM((yyvsp[-5].qp))->usage |= DEP;}
		  P2{ if ((SYM((yyvsp[-5].qp))->subtype & ARRAY) == 0)
			{myerr("variable is not an array");}
		  }
		}
#line 2819 "parse1.c" /* yacc.c:1646  */
    break;

  case 189:
#line 472 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp); P1{SYM((yyvsp[0].qp))->usage |= DEP;}}
#line 2825 "parse1.c" /* yacc.c:1646  */
    break;

  case 190:
#line 473 "parse1.y" /* yacc.c:1646  */
    { lastok = (yyvsp[0].qp);}
#line 2831 "parse1.c" /* yacc.c:1646  */
    break;

  case 191:
#line 474 "parse1.y" /* yacc.c:1646  */
    { lastok = (yyvsp[0].qp);}
#line 2837 "parse1.c" /* yacc.c:1646  */
    break;

  case 196:
#line 479 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal integer expression");}
#line 2843 "parse1.c" /* yacc.c:1646  */
    break;

  case 197:
#line 481 "parse1.y" /* yacc.c:1646  */
    {P3{unit_push((yyvsp[0].qp));}}
#line 2849 "parse1.c" /* yacc.c:1646  */
    break;

  case 198:
#line 482 "parse1.y" /* yacc.c:1646  */
    {P3{if ((yyvsp[0].qp)) {
				Unit_push(STR((yyvsp[0].qp)));
			     }else{		
				Unit_push((char *)0);
			     }
			}}
#line 2860 "parse1.c" /* yacc.c:1646  */
    break;

  case 199:
#line 488 "parse1.y" /* yacc.c:1646  */
    {P3{unit_push((yyvsp[0].qp));}}
#line 2866 "parse1.c" /* yacc.c:1646  */
    break;

  case 200:
#line 489 "parse1.y" /* yacc.c:1646  */
    { lastok = (yyvsp[0].qp);
			  P3{ifcnvfac((yyvsp[0].qp));}
			}
#line 2874 "parse1.c" /* yacc.c:1646  */
    break;

  case 201:
#line 492 "parse1.y" /* yacc.c:1646  */
    {P3{unit_cmp((yyvsp[-2].qp), (yyvsp[-1].qp), lastok);}}
#line 2880 "parse1.c" /* yacc.c:1646  */
    break;

  case 202:
#line 493 "parse1.y" /* yacc.c:1646  */
    {P3{unit_cmp((yyvsp[-2].qp), (yyvsp[-1].qp), lastok);}}
#line 2886 "parse1.c" /* yacc.c:1646  */
    break;

  case 203:
#line 494 "parse1.y" /* yacc.c:1646  */
    {P3{unit_mul();}}
#line 2892 "parse1.c" /* yacc.c:1646  */
    break;

  case 204:
#line 495 "parse1.y" /* yacc.c:1646  */
    {P3{unit_div();}}
#line 2898 "parse1.c" /* yacc.c:1646  */
    break;

  case 205:
#line 496 "parse1.y" /* yacc.c:1646  */
    {P3{unit_exponent((yyvsp[0].qp), lastok);}}
#line 2904 "parse1.c" /* yacc.c:1646  */
    break;

  case 206:
#line 497 "parse1.y" /* yacc.c:1646  */
    {P3{unit_logic(1, (yyvsp[-2].qp), (yyvsp[-1].qp), lastok);}}
#line 2910 "parse1.c" /* yacc.c:1646  */
    break;

  case 207:
#line 498 "parse1.y" /* yacc.c:1646  */
    {P3{unit_logic(1, (yyvsp[-2].qp), (yyvsp[-1].qp), lastok);}}
#line 2916 "parse1.c" /* yacc.c:1646  */
    break;

  case 208:
#line 499 "parse1.y" /* yacc.c:1646  */
    {P3{unit_logic(2, (yyvsp[-2].qp), (yyvsp[-1].qp), lastok);}}
#line 2922 "parse1.c" /* yacc.c:1646  */
    break;

  case 209:
#line 500 "parse1.y" /* yacc.c:1646  */
    {P3{unit_logic(2, (yyvsp[-2].qp), (yyvsp[-1].qp), lastok);}}
#line 2928 "parse1.c" /* yacc.c:1646  */
    break;

  case 210:
#line 501 "parse1.y" /* yacc.c:1646  */
    {P3{unit_logic(2, (yyvsp[-2].qp), (yyvsp[-1].qp), lastok);}}
#line 2934 "parse1.c" /* yacc.c:1646  */
    break;

  case 211:
#line 502 "parse1.y" /* yacc.c:1646  */
    {P3{unit_logic(2, (yyvsp[-2].qp), (yyvsp[-1].qp), lastok);}}
#line 2940 "parse1.c" /* yacc.c:1646  */
    break;

  case 212:
#line 503 "parse1.y" /* yacc.c:1646  */
    {P3{unit_logic(2, (yyvsp[-2].qp), (yyvsp[-1].qp), lastok);}}
#line 2946 "parse1.c" /* yacc.c:1646  */
    break;

  case 213:
#line 504 "parse1.y" /* yacc.c:1646  */
    {P3{unit_logic(2, (yyvsp[-2].qp), (yyvsp[-1].qp), lastok);}}
#line 2952 "parse1.c" /* yacc.c:1646  */
    break;

  case 214:
#line 505 "parse1.y" /* yacc.c:1646  */
    {P3{unit_pop(); Unit_push("");}}
#line 2958 "parse1.c" /* yacc.c:1646  */
    break;

  case 216:
#line 507 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal expression");}
#line 2964 "parse1.c" /* yacc.c:1646  */
    break;

  case 217:
#line 509 "parse1.y" /* yacc.c:1646  */
    {P3{unit_push_args((yyvsp[-1].qp));}}
#line 2970 "parse1.c" /* yacc.c:1646  */
    break;

  case 218:
#line 510 "parse1.y" /* yacc.c:1646  */
    { lastok = (yyvsp[0].qp); P1{SYM((yyvsp[-4].qp))->usage |= FUNCT;}
		  P3{unit_done_args();}
		}
#line 2978 "parse1.c" /* yacc.c:1646  */
    break;

  case 219:
#line 515 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0; }
#line 2984 "parse1.c" /* yacc.c:1646  */
    break;

  case 221:
#line 518 "parse1.y" /* yacc.c:1646  */
    {P3{unit_chk_arg((yyvsp[0].qp), lastok);}}
#line 2990 "parse1.c" /* yacc.c:1646  */
    break;

  case 223:
#line 520 "parse1.y" /* yacc.c:1646  */
    {P3{unit_chk_arg((yyvsp[0].qp), lastok);}}
#line 2996 "parse1.c" /* yacc.c:1646  */
    break;

  case 225:
#line 523 "parse1.y" /* yacc.c:1646  */
    {P1{pushlocal((yyvsp[-5].qp), ITEM0);}}
#line 3002 "parse1.c" /* yacc.c:1646  */
    break;

  case 226:
#line 524 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(6, (yyvsp[-8].qp), (yyvsp[-7].qp), (yyvsp[-5].qp), (yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[0].qp)); poplocal();}}
#line 3008 "parse1.c" /* yacc.c:1646  */
    break;

  case 227:
#line 525 "parse1.y" /* yacc.c:1646  */
    {
myerr("FROM intvar = intexpr TO intexpr BY intexpr { statements }");}
#line 3015 "parse1.c" /* yacc.c:1646  */
    break;

  case 228:
#line 528 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 3021 "parse1.c" /* yacc.c:1646  */
    break;

  case 230:
#line 531 "parse1.y" /* yacc.c:1646  */
    {P3{unit_pop();}}
#line 3027 "parse1.c" /* yacc.c:1646  */
    break;

  case 231:
#line 533 "parse1.y" /* yacc.c:1646  */
    {P3{unit_pop();}}
#line 3033 "parse1.c" /* yacc.c:1646  */
    break;

  case 232:
#line 536 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 3039 "parse1.c" /* yacc.c:1646  */
    break;

  case 233:
#line 537 "parse1.y" /* yacc.c:1646  */
    {P3{unit_pop();}}
#line 3045 "parse1.c" /* yacc.c:1646  */
    break;

  case 234:
#line 540 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 3051 "parse1.c" /* yacc.c:1646  */
    break;

  case 237:
#line 546 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(3, (yyvsp[-2].qp), (yyvsp[-1].qp), (yyvsp[0].qp)); declare(DERF, (yyvsp[-1].qp), (yyval.qp));}}
#line 3057 "parse1.c" /* yacc.c:1646  */
    break;

  case 238:
#line 549 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(4, (yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[-1].qp), (yyvsp[0].qp)); declare(LINF, (yyvsp[-2].qp), (yyval.qp));}}
#line 3063 "parse1.c" /* yacc.c:1646  */
    break;

  case 239:
#line 552 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(4, (yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[-1].qp), (yyvsp[0].qp)); declare(NLINF, (yyvsp[-2].qp), (yyval.qp));}}
#line 3069 "parse1.c" /* yacc.c:1646  */
    break;

  case 240:
#line 555 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(3, (yyvsp[-2].qp), (yyvsp[-1].qp), (yyvsp[0].qp)); declare(DISCF, (yyvsp[-1].qp), (yyval.qp));}}
#line 3075 "parse1.c" /* yacc.c:1646  */
    break;

  case 241:
#line 558 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(3, (yyvsp[-2].qp), (yyvsp[-1].qp), (yyvsp[0].qp)); declare(PARF, (yyvsp[-1].qp), (yyval.qp));}}
#line 3081 "parse1.c" /* yacc.c:1646  */
    break;

  case 242:
#line 561 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);
		 P3{
			unit_push((yyvsp[-7].qp)); unit_del(2); unit_mul();
			unit_push((yyvsp[-3].qp)); unit_mul();
			unit_push((yyvsp[0].qp)); unit_cmp((yyvsp[-7].qp), (yyvsp[-1].qp), (yyvsp[0].qp));
			unit_push((yyvsp[-9].qp)); unit_swap(); unit_cmp((yyvsp[-9].qp), (yyvsp[-8].qp), (yyvsp[0].qp));
			unit_pop();
		}}
#line 3094 "parse1.c" /* yacc.c:1646  */
    break;

  case 243:
#line 570 "parse1.y" /* yacc.c:1646  */
    {P3{
			unit_del(1);
			unit_push((yyvsp[-5].qp)); unit_mul(); unit_swap();
			unit_cmp((yyvsp[-6].qp),(yyvsp[-1].qp),lastok); unit_pop();
		}}
#line 3104 "parse1.c" /* yacc.c:1646  */
    break;

  case 244:
#line 576 "parse1.y" /* yacc.c:1646  */
    {P3{unit_push((yyvsp[-5].qp)); unit_swap();
		    unit_cmp((yyvsp[-5].qp),(yyvsp[-1].qp),lastok); unit_pop();
		}}
#line 3112 "parse1.c" /* yacc.c:1646  */
    break;

  case 245:
#line 579 "parse1.y" /* yacc.c:1646  */
    {myerr("Illeqal partial diffeq");}
#line 3118 "parse1.c" /* yacc.c:1646  */
    break;

  case 248:
#line 584 "parse1.y" /* yacc.c:1646  */
    {P1{pushlocal((yyvsp[-4].qp), ITEM0); func_unit((yyvsp[-4].qp), (yyvsp[0].qp));}}
#line 3124 "parse1.c" /* yacc.c:1646  */
    break;

  case 249:
#line 586 "parse1.y" /* yacc.c:1646  */
    {P1{
		    declare(FUNCT, (yyvsp[-6].qp), itemarray(3, (yyvsp[-6].qp), (yyvsp[-2].qp), (yyvsp[-4].qp)));
		    poplocal(); poplocal(); SYM((yyvsp[-6].qp))->usage |= FUNCT;
		   }
		}
#line 3134 "parse1.c" /* yacc.c:1646  */
    break;

  case 250:
#line 593 "parse1.y" /* yacc.c:1646  */
    {P1{pushlocal((yyvsp[-4].qp), ITEM0); func_unit((yyvsp[-4].qp), (yyvsp[0].qp));}
		 P1{
		    declare(FUNCT, (yyvsp[-4].qp), itemarray(3, (yyvsp[-4].qp), (yyvsp[0].qp), (yyvsp[-2].qp)));
		    poplocal(); poplocal(); SYM((yyvsp[-4].qp))->usage |= FUNCT;
		   }
		}
#line 3145 "parse1.c" /* yacc.c:1646  */
    break;

  case 251:
#line 601 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0; P1{pushlocal(ITEM0, ITEM0);}
			if (args) { freelist(&args); }
			args = newlist();
		}
#line 3154 "parse1.c" /* yacc.c:1646  */
    break;

  case 253:
#line 608 "parse1.y" /* yacc.c:1646  */
    {P1{
		    if (args) { freelist(&args); }
		    args = newlist();
		    (yyval.qp) = (Item *)newlist();
		    pushlocal((yyvsp[-1].qp), ITEM0);
		    Lappendsym(args, checklocal(SYM((yyvsp[-1].qp))));
		    if ((yyvsp[0].qp)) {
			checklocal(SYM((yyvsp[-1].qp)))->u.str = STR((yyvsp[0].qp));
			Lappendstr((List *)(yyval.qp), STR((yyvsp[0].qp)));
		    }else{
			Lappendstr((List *)(yyval.qp), "");
		    }
		}}
#line 3172 "parse1.c" /* yacc.c:1646  */
    break;

  case 254:
#line 622 "parse1.y" /* yacc.c:1646  */
    {P1{
		    pushlocal((yyvsp[-1].qp), ITEM0);
		    Lappendsym(args, checklocal(SYM((yyvsp[-1].qp))));
		    if ((yyvsp[0].qp)) {
			checklocal(SYM((yyvsp[-1].qp)))->u.str = STR((yyvsp[0].qp));
			Lappendstr((List *)(yyvsp[-3].qp), STR((yyvsp[0].qp)));
		    }else{
			Lappendstr((List *)(yyvsp[-3].qp), "");
		    }
		}}
#line 3187 "parse1.c" /* yacc.c:1646  */
    break;

  case 255:
#line 634 "parse1.y" /* yacc.c:1646  */
    {P1{
		    declare(PROCED, (yyvsp[-4].qp), itemarray(3, (yyvsp[-4].qp), ITEM0, (yyvsp[-2].qp)));
		    poplocal(); SYM((yyvsp[-4].qp))->usage |= FUNCT;
		   }
		}
#line 3197 "parse1.c" /* yacc.c:1646  */
    break;

  case 256:
#line 641 "parse1.y" /* yacc.c:1646  */
    {P1{
			List* l; Item* q;
			if ((yyvsp[-1].qp) == ITEM0) {
				diag("NET_RECEIVE must have at least one argument", (char*)0);
			}
			l = newlist();
			q = lappendsym(l, install("flag", NAME));
			pushlocal(q, ITEM0);
			Lappendstr((List *)(yyvsp[-1].qp), "");
			netreceive_arglist = args; args = (List*)0;
		}}
#line 3213 "parse1.c" /* yacc.c:1646  */
    break;

  case 257:
#line 653 "parse1.y" /* yacc.c:1646  */
    { P1{poplocal();}}
#line 3219 "parse1.c" /* yacc.c:1646  */
    break;

  case 258:
#line 654 "parse1.y" /* yacc.c:1646  */
    { myerr("Illegal NETRECEIVE block");}
#line 3225 "parse1.c" /* yacc.c:1646  */
    break;

  case 261:
#line 659 "parse1.y" /* yacc.c:1646  */
    { myerr("Illegal WATCH statement");}
#line 3231 "parse1.c" /* yacc.c:1646  */
    break;

  case 263:
#line 664 "parse1.y" /* yacc.c:1646  */
    {P1{ Item* q1, *q2;
		  q1 = netreceive_arglist->next;
		  q2 = args->next;
		  while (q1 != netreceive_arglist && q2 != args) {
			Symbol* s1 = SYM(q1);
			Symbol* s2 = SYM(q2);
			if (s1->u.str) { /* s2 must be nil or same */
				if (s2->u.str) {
					if (strcmp(s1->u.str, s2->u.str) != 0) {
						diag(s1->name, " in FOR_NETCONS arglist does not have same units as corresponding arg in NET_RECEIVE arglist");
					}
				}else{
					s2->u.str = s1->u.str;
				}
			}else{ /* s2 must be nil */
				if (s2->u.str) {
					diag(s1->name, " in FOR_NETCONS arglist does not have same units as corresponding arg in NET_RECEIVE arglist");
				}
			}
/*printf("|%s|%s|  |%s|%s|\n", s1->name, s1->u.str, s2->name, s2->u.str);*/
			q1 = q1->next;
			q2 = q2->next;
		  }
		  if (q1 != netreceive_arglist || q2 != args) {
			diag("NET_RECEIVE and FOR_NETCONS do not have same number of arguments", (char*)0);
		  }
		}}
#line 3263 "parse1.c" /* yacc.c:1646  */
    break;

  case 265:
#line 692 "parse1.y" /* yacc.c:1646  */
    { myerr("Illegal FOR_NETCONS statement");}
#line 3269 "parse1.c" /* yacc.c:1646  */
    break;

  case 266:
#line 696 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(4, (yyvsp[-2].qp), (yyvsp[-1].qp), ITEM0, (yyvsp[0].qp)); lappenditem(solvelist, (yyval.qp));}}
#line 3275 "parse1.c" /* yacc.c:1646  */
    break;

  case 267:
#line 698 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = itemarray(4, (yyvsp[-4].qp), (yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[-1].qp)); lappenditem(solvelist, (yyval.qp));}}
#line 3281 "parse1.c" /* yacc.c:1646  */
    break;

  case 268:
#line 699 "parse1.y" /* yacc.c:1646  */
    { myerr("Illegal SOLVE statement");}
#line 3287 "parse1.c" /* yacc.c:1646  */
    break;

  case 269:
#line 702 "parse1.y" /* yacc.c:1646  */
    { (yyval.qp) = ITEM0; }
#line 3293 "parse1.c" /* yacc.c:1646  */
    break;

  case 270:
#line 704 "parse1.y" /* yacc.c:1646  */
    { (yyval.qp) = (yyvsp[0].qp); }
#line 3299 "parse1.c" /* yacc.c:1646  */
    break;

  case 271:
#line 707 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 3305 "parse1.c" /* yacc.c:1646  */
    break;

  case 273:
#line 711 "parse1.y" /* yacc.c:1646  */
    { P2{if(!(SYM((yyvsp[0].qp))->subtype&STAT)){
			myerr("Not a STATE");}
		  }
		}
#line 3314 "parse1.c" /* yacc.c:1646  */
    break;

  case 274:
#line 716 "parse1.y" /* yacc.c:1646  */
    { P2{if(!(SYM((yyvsp[-1].qp))->subtype&STAT)){
			myerr("Not a STATE");}
		  }
		}
#line 3323 "parse1.c" /* yacc.c:1646  */
    break;

  case 275:
#line 720 "parse1.y" /* yacc.c:1646  */
    {myerr("Syntax: SOLVEFOR name, name, ...");}
#line 3329 "parse1.c" /* yacc.c:1646  */
    break;

  case 279:
#line 727 "parse1.y" /* yacc.c:1646  */
    {myerr("syntax is SENS var1, var2, var3, etc");}
#line 3335 "parse1.c" /* yacc.c:1646  */
    break;

  case 280:
#line 731 "parse1.y" /* yacc.c:1646  */
    {P3{unit_cmp((yyvsp[-2].qp), (yyvsp[-1].qp), lastok);}}
#line 3341 "parse1.c" /* yacc.c:1646  */
    break;

  case 281:
#line 732 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal CONSERVE syntax");}
#line 3347 "parse1.c" /* yacc.c:1646  */
    break;

  case 282:
#line 734 "parse1.y" /* yacc.c:1646  */
    {P3{consreact_push((yyvsp[0].qp));}}
#line 3353 "parse1.c" /* yacc.c:1646  */
    break;

  case 283:
#line 735 "parse1.y" /* yacc.c:1646  */
    {P3{consreact_push((yyvsp[0].qp));}}
#line 3359 "parse1.c" /* yacc.c:1646  */
    break;

  case 284:
#line 736 "parse1.y" /* yacc.c:1646  */
    {P3{consreact_push((yyvsp[0].qp)); unit_cmp((yyvsp[-2].qp),(yyvsp[-1].qp),lastok);}}
#line 3365 "parse1.c" /* yacc.c:1646  */
    break;

  case 285:
#line 737 "parse1.y" /* yacc.c:1646  */
    {
		  P3{consreact_push((yyvsp[0].qp)); unit_cmp((yyvsp[-3].qp),(yyvsp[-2].qp),lastok);}
		}
#line 3373 "parse1.c" /* yacc.c:1646  */
    break;

  case 286:
#line 741 "parse1.y" /* yacc.c:1646  */
    {P1{pushlocal((yyvsp[0].qp), ITEM0);}}
#line 3379 "parse1.c" /* yacc.c:1646  */
    break;

  case 287:
#line 742 "parse1.y" /* yacc.c:1646  */
    {P1{poplocal();}
		  P3{
		    unit_pop();
		  }
		}
#line 3389 "parse1.c" /* yacc.c:1646  */
    break;

  case 288:
#line 748 "parse1.y" /* yacc.c:1646  */
    { P3{
		    unit_pop();
		  }
		}
#line 3398 "parse1.c" /* yacc.c:1646  */
    break;

  case 289:
#line 754 "parse1.y" /* yacc.c:1646  */
    {P3 R0{unit_compartlist((yyvsp[0].qp));}}
#line 3404 "parse1.c" /* yacc.c:1646  */
    break;

  case 290:
#line 756 "parse1.y" /* yacc.c:1646  */
    {P3 R0{unit_compartlist((yyvsp[0].qp));}}
#line 3410 "parse1.c" /* yacc.c:1646  */
    break;

  case 291:
#line 758 "parse1.y" /* yacc.c:1646  */
    {P1{pushlocal((yyvsp[0].qp), ITEM0);}}
#line 3416 "parse1.c" /* yacc.c:1646  */
    break;

  case 292:
#line 759 "parse1.y" /* yacc.c:1646  */
    {P1{poplocal();}
		  P3{
		    unit_pop();
		  }
		}
#line 3426 "parse1.c" /* yacc.c:1646  */
    break;

  case 293:
#line 765 "parse1.y" /* yacc.c:1646  */
    { P3{
		    unit_pop();
		  }
		}
#line 3435 "parse1.c" /* yacc.c:1646  */
    break;

  case 294:
#line 771 "parse1.y" /* yacc.c:1646  */
    {P3 R0{unit_ldifuslist((yyvsp[0].qp), unitflagsave);}}
#line 3441 "parse1.c" /* yacc.c:1646  */
    break;

  case 295:
#line 773 "parse1.y" /* yacc.c:1646  */
    {P3 R0{unit_ldifuslist((yyvsp[0].qp), unitflagsave);}}
#line 3447 "parse1.c" /* yacc.c:1646  */
    break;

  case 298:
#line 779 "parse1.y" /* yacc.c:1646  */
    {P1{declare(KINF, (yyvsp[-2].qp), ITEM0);}}
#line 3453 "parse1.c" /* yacc.c:1646  */
    break;

  case 299:
#line 782 "parse1.y" /* yacc.c:1646  */
    {P3{kinunits((yyvsp[-6].qp), restart_pass);}}
#line 3459 "parse1.c" /* yacc.c:1646  */
    break;

  case 300:
#line 784 "parse1.y" /* yacc.c:1646  */
    {P3{kinunits((yyvsp[-4].qp), restart_pass);}}
#line 3465 "parse1.c" /* yacc.c:1646  */
    break;

  case 301:
#line 786 "parse1.y" /* yacc.c:1646  */
    {P3{kinunits((yyvsp[-4].qp), restart_pass);}}
#line 3471 "parse1.c" /* yacc.c:1646  */
    break;

  case 302:
#line 787 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal reaction syntax");}
#line 3477 "parse1.c" /* yacc.c:1646  */
    break;

  case 303:
#line 789 "parse1.y" /* yacc.c:1646  */
    {P3{R1{ureactadd((yyvsp[0].qp));} unit_push((yyvsp[0].qp));}}
#line 3483 "parse1.c" /* yacc.c:1646  */
    break;

  case 304:
#line 790 "parse1.y" /* yacc.c:1646  */
    {P3{R1{ureactadd((yyvsp[0].qp));} unit_push((yyvsp[0].qp)); Unit_push((char*)0); unit_exponent((yyvsp[-1].qp),(yyvsp[-1].qp));}}
#line 3489 "parse1.c" /* yacc.c:1646  */
    break;

  case 305:
#line 791 "parse1.y" /* yacc.c:1646  */
    {P3{R1{ureactadd((yyvsp[0].qp));}unit_push((yyvsp[0].qp)); unit_mul();}}
#line 3495 "parse1.c" /* yacc.c:1646  */
    break;

  case 306:
#line 792 "parse1.y" /* yacc.c:1646  */
    {
		  P3{R1{ureactadd((yyvsp[0].qp));}unit_push((yyvsp[0].qp)); Unit_push((char*)0); unit_exponent((yyvsp[-1].qp),(yyvsp[-1].qp)); unit_mul();}
		}
#line 3503 "parse1.c" /* yacc.c:1646  */
    break;

  case 308:
#line 797 "parse1.y" /* yacc.c:1646  */
    {myerr("Lag syntax is: LAG name BY const");}
#line 3509 "parse1.c" /* yacc.c:1646  */
    break;

  case 309:
#line 800 "parse1.y" /* yacc.c:1646  */
    {P3{unit_pop(); unit_pop();}}
#line 3515 "parse1.c" /* yacc.c:1646  */
    break;

  case 310:
#line 803 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 3521 "parse1.c" /* yacc.c:1646  */
    break;

  case 312:
#line 807 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 3527 "parse1.c" /* yacc.c:1646  */
    break;

  case 321:
#line 821 "parse1.y" /* yacc.c:1646  */
    {myerr("MATCH syntax is state0 or state(expr)=expr or\
state[i](expr(i)) = expr(i)");}
#line 3534 "parse1.c" /* yacc.c:1646  */
    break;

  case 324:
#line 829 "parse1.y" /* yacc.c:1646  */
    {
			  lastok = (yyvsp[0].qp);
#if NRNUNIT
			  P2{nrn_unit_chk();}
#endif
			}
#line 3545 "parse1.c" /* yacc.c:1646  */
    break;

  case 325:
#line 836 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[-2].qp);}
#line 3551 "parse1.c" /* yacc.c:1646  */
    break;

  case 327:
#line 840 "parse1.y" /* yacc.c:1646  */
    { P1{nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}}
#line 3557 "parse1.c" /* yacc.c:1646  */
    break;

  case 329:
#line 843 "parse1.y" /* yacc.c:1646  */
    { P1{nrn_list((yyvsp[-1].qp),(yyvsp[0].qp));}}
#line 3563 "parse1.c" /* yacc.c:1646  */
    break;

  case 330:
#line 845 "parse1.y" /* yacc.c:1646  */
    { P1{nrn_list((yyvsp[-1].qp),(yyvsp[0].qp));}}
#line 3569 "parse1.c" /* yacc.c:1646  */
    break;

  case 331:
#line 847 "parse1.y" /* yacc.c:1646  */
    { P1{nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}}
#line 3575 "parse1.c" /* yacc.c:1646  */
    break;

  case 332:
#line 849 "parse1.y" /* yacc.c:1646  */
    { P1{nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}}
#line 3581 "parse1.c" /* yacc.c:1646  */
    break;

  case 333:
#line 851 "parse1.y" /* yacc.c:1646  */
    { P1{nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}}
#line 3587 "parse1.c" /* yacc.c:1646  */
    break;

  case 334:
#line 853 "parse1.y" /* yacc.c:1646  */
    { P1{nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}}
#line 3593 "parse1.c" /* yacc.c:1646  */
    break;

  case 335:
#line 855 "parse1.y" /* yacc.c:1646  */
    { P1{nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}}
#line 3599 "parse1.c" /* yacc.c:1646  */
    break;

  case 337:
#line 859 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = NULL;}
#line 3605 "parse1.c" /* yacc.c:1646  */
    break;

  case 339:
#line 863 "parse1.y" /* yacc.c:1646  */
    {P1{nrn_use((yyvsp[-3].qp), (yyvsp[-1].qp), ITEM0);}}
#line 3611 "parse1.c" /* yacc.c:1646  */
    break;

  case 340:
#line 865 "parse1.y" /* yacc.c:1646  */
    {P1{nrn_use((yyvsp[-3].qp), ITEM0, (yyvsp[-1].qp));}}
#line 3617 "parse1.c" /* yacc.c:1646  */
    break;

  case 341:
#line 867 "parse1.y" /* yacc.c:1646  */
    {P1{nrn_use((yyvsp[-5].qp), (yyvsp[-3].qp), (yyvsp[-1].qp));}}
#line 3623 "parse1.c" /* yacc.c:1646  */
    break;

  case 342:
#line 869 "parse1.y" /* yacc.c:1646  */
    {myerr("syntax is: USEION ion READ list WRITE list");}
#line 3629 "parse1.c" /* yacc.c:1646  */
    break;

  case 343:
#line 872 "parse1.y" /* yacc.c:1646  */
    {P1{(yyval.qp) = (Item *)newlist(); Lappendsym((List *)(yyval.qp), SYM((yyvsp[0].qp)));}}
#line 3635 "parse1.c" /* yacc.c:1646  */
    break;

  case 344:
#line 874 "parse1.y" /* yacc.c:1646  */
    {P1{ Lappendsym((List *)(yyvsp[-2].qp), SYM((yyvsp[0].qp)));}}
#line 3641 "parse1.c" /* yacc.c:1646  */
    break;

  case 345:
#line 876 "parse1.y" /* yacc.c:1646  */
    {myerr("syntax is: keyword name , name, ..., name");}
#line 3647 "parse1.c" /* yacc.c:1646  */
    break;

  case 346:
#line 879 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 3653 "parse1.c" /* yacc.c:1646  */
    break;

  case 347:
#line 881 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 3659 "parse1.c" /* yacc.c:1646  */
    break;

  case 348:
#line 883 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 3665 "parse1.c" /* yacc.c:1646  */
    break;


#line 3669 "parse1.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 885 "parse1.y" /* yacc.c:1906  */

	/* end of grammar */

static void yyerror(s)	/* called for yacc syntax error */
	char *s;
{
	Fprintf(stderr, "%s:\n ", s);
}

static int yylex() {return next_intoken(&(yylval.qp));}

#if !NRNUNIT
void nrn_list(q1, q2)
	Item *q1, *q2;
{
	/*ARGSUSED*/
}
void nrn_use(q1, q2, q3)
	Item *q1, *q2, *q3;
{
	/*ARGSUSED*/
}
#endif

