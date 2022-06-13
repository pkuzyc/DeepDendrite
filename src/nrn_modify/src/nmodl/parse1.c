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

/* /local/src/master/nrn/src/nmodl/parse1.y,v 4.11 1999/03/24 18:34:08 hines Exp */

#include <../../nmodlconf.h>
#include "modl.h"
#include <stdlib.h>

#if defined(__STDC__)
#define sdebug(arg1,arg2) {}
#define qdebug(arg1,arg2) {}
#else
#define sdebug(arg1,arg2) {Fprintf(stderr,"arg1:%s\n", arg2); Fflush(stderr);}
#define qdebug(arg1,arg2) {Item *q; Fprintf(stderr,"arg1:");Fflush(stderr);\
	for (q=arg2; q->type != 0; q=q->next){\
		if (q->type == SYMBOL)\
			Fprintf(stderr,"%s\n", SYM(q)->name);\
		else if (q->type == STRING)\
			Fprintf(stderr,"%s\n", STR(q);\
		else\
			Fprintf(stderr,"Illegal Item type\n");\
		Fflush(stderr);}\
		Fprintf(stderr,"\n");Fflush(stderr);}
#endif

#define ldebug(arg1, arg2) qdebug(arg1, arg2->next)

extern int yylex(), yyparse();
static void yyerror();

#if YYBISON
#define myerr(arg) static int ierr=0;\
if (!(ierr++))yyerror(arg); --yyssp; --yyvsp; YYERROR
#else
#define myerr(arg) static int ierr=0;\
if (!(ierr++))yyerror(arg); --yyps; --yypv; YYERROR
#endif

int brkpnt_exists;
int assert_threadsafe;
int usederivstatearray;
extern int protect_;
extern int vectorize;
extern int in_comment_; /* allow non-ascii in a COMMENT */
extern char *modelline;
extern Item* protect_astmt(Item*, Item*);
extern List* toplocal_;
static List* toplocal1_;
extern List *firstlist; /* NAME symbols in order that they appear in file */
extern int lexcontext; /* ':' can return 3 different tokens */
extern List *solveforlist; /* List of symbols that are actually to be solved
				for in a block. See in_solvefor() */
static int stateblock; /* 0 if dependent, 1 if state */
static int blocktype;
static int saw_verbatim_; /* only print the notice once */
static int inequation; /* inside an equation?*/
static int nstate;	/* number of states seen in an expression */
static int leftside;	/* inside left hand side of equation? */
static int pstate;	/* number of state in a primary expression */
static int tstate;	/* number of states in a term */
static Item *lastok;	/* last token accepted by expr */
static int sensused;	/* a SENS statement occurred in this block */
static Symbol *matchindex; /* local symbol for implied MATCH loop */
static int model_level = 0; /* the model level prepended to declaration
				blocks by merge */
static int scopindep = 0;/* SCoP independent explicitly declared if 1 */
static int extdef2 = 0; /* flag that says we are in an EXTDEF2 function */
static List *table_list = LIST0; /* table information for TABLE statement */
static int forallindex = 0;	/* 0 not in FORALL, -1 just starting, 
					>0 index of arrays used (must all
					be the same */
static Item* astmt_end_;	/* see kinetic.c vectorizing */
static int nr_argcnt_, argcnt_; /* for matching number of args in NET_RECEIVE
				 and FOR_NETCONS */

#line 141 "parse1.c" /* yacc.c:339  */

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
#line 76 "parse1.y" /* yacc.c:355  */

	Item	*qp;
	char	*str;
	List	*lp;
	int	i;

#line 414 "parse1.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 431 "parse1.c" /* yacc.c:358  */

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
#define YYLAST   1355

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  131
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  167
/* YYNRULES -- Number of rules.  */
#define YYNRULES  395
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  776

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   368

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   129,     2,     2,
      57,    58,    63,    62,    69,    64,     2,    65,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    68,     2,
       2,    66,     2,     2,    61,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    59,     2,    60,    67,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    55,     2,    56,    70,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   130
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   133,   133,   134,   136,   137,   138,   151,   152,   153,
     153,   156,   155,   161,   170,   177,   178,   181,   185,   187,
     196,   198,   204,   204,   204,   204,   204,   205,   205,   205,
     205,   207,   209,   210,   214,   218,   220,   225,   227,   228,
     230,   230,   234,   235,   238,   239,   245,   247,   248,   251,
     255,   256,   258,   259,   261,   262,   269,   269,   271,   272,
     274,   275,   281,   283,   284,   286,   286,   291,   296,   298,
     300,   300,   302,   303,   306,   310,   316,   320,   326,   330,
     331,   334,   335,   340,   340,   342,   347,   349,   357,   361,
     362,   365,   365,   367,   367,   368,   368,   369,   369,   370,
     370,   371,   371,   372,   372,   373,   373,   374,   374,   375,
     375,   376,   376,   377,   377,   378,   378,   379,   379,   380,
     380,   381,   381,   382,   382,   384,   387,   390,   393,   393,
     394,   397,   399,   403,   402,   412,   414,   429,   444,   445,
     448,   449,   449,   451,   453,   454,   455,   456,   457,   458,
     459,   460,   470,   472,   473,   474,   475,   476,   477,   478,
     480,   481,   482,   486,   487,   488,   489,   490,   491,   492,
     495,   498,   498,   502,   502,   505,   509,   533,   542,   549,
     564,   587,   590,   594,   602,   603,   604,   605,   606,   607,
     608,   610,   611,   612,   613,   614,   615,   616,   617,   618,
     621,   622,   623,   624,   625,   626,   627,   628,   629,   630,
     631,   633,   635,   640,   642,   645,   648,   651,   655,   661,
     667,   673,   674,   675,   677,   680,   685,   684,   702,   703,
     704,   705,   706,   708,   708,   731,   734,   735,   737,   737,
     756,   759,   761,   763,   764,   771,   772,   779,   783,   783,
     789,   795,   799,   804,   808,   810,   812,   815,   815,   817,
     845,   844,   873,   874,   874,   876,   880,   885,   904,   903,
     921,   923,   931,   934,   936,   939,   940,   944,   948,   950,
     954,   956,   958,   962,   965,   967,   969,   971,   973,   975,
     977,   979,   981,   984,   985,   987,   987,   997,   999,  1000,
    1001,  1002,  1003,  1004,  1005,  1006,  1007,  1010,  1011,  1014,
    1019,  1021,  1024,  1028,  1028,  1032,  1035,  1034,  1038,  1042,
    1041,  1045,  1048,  1049,  1051,  1058,  1058,  1060,  1062,  1064,
    1066,  1067,  1068,  1069,  1071,  1073,  1075,  1076,  1078,  1078,
    1081,  1082,  1084,  1086,  1093,  1097,  1099,  1104,  1108,  1109,
    1110,  1112,  1114,  1116,  1122,  1136,  1145,  1151,  1153,  1154,
    1161,  1176,  1179,  1180,  1182,  1184,  1188,  1189,  1192,  1195,
    1196,  1198,  1199,  1200,  1202,  1204,  1206,  1208,  1210,  1212,
    1214,  1216,  1219,  1221,  1223,  1225,  1228,  1229,  1231,  1232,
    1234,  1238,  1239,  1242,  1243,  1245
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "VERBATIM", "COMMENT", "MODEL",
  "CONSTANT", "INDEPENDENT", "DEPENDENT", "STATE", "INITIAL1",
  "DERIVATIVE", "SOLVE", "USING", "WITH", "STEPPED", "DISCRETE", "FROM",
  "FORALL1", "TO", "BY", "WHILE", "IF", "ELSE", "START1", "STEP", "SENS",
  "SOLVEFOR", "PROCEDURE", "PARTIAL", "DEL", "DEL2", "DEFINE1", "IFERROR",
  "PARAMETER", "DERFUNC", "EQUATION", "TERMINAL", "LINEAR", "NONLINEAR",
  "FUNCTION1", "LOCAL", "METHOD", "LIN1", "NONLIN1", "PUTQ", "GETQ",
  "TABLE", "DEPEND", "BREAKPOINT", "INCLUDE1", "FUNCTION_TABLE", "PROTECT",
  "NRNMUTEXLOCK", "NRNMUTEXUNLOCK", "'{'", "'}'", "'('", "')'", "'['",
  "']'", "'@'", "'+'", "'*'", "'-'", "'/'", "'='", "'^'", "':'", "','",
  "'~'", "OR", "AND", "GT", "LT", "LE", "EQ", "NE", "NOT", "NAME", "PRIME",
  "REAL", "INTEGER", "DEFINEDVAR", "ONTOLOGY_ID", "STRING", "PLOT", "VS",
  "LAG", "RESET", "MATCH", "MODEL_LEVEL", "SWEEP", "FIRST", "LAST",
  "KINETIC", "CONSERVE", "REACTION", "REACT1", "COMPARTMENT", "UNITS",
  "UNITSON", "UNITSOFF", "LONGDIFUS", "NEURON", "SUFFIX", "NONSPECIFIC",
  "READ", "WRITE", "USEION", "VALENCE", "THREADSAFE", "REPRESENTS",
  "GLOBAL", "SECTION", "RANGE", "POINTER", "BBCOREPOINTER", "EXTERNAL",
  "BEFORE", "AFTER", "WATCH", "ELECTRODE_CURRENT", "CONSTRUCTOR",
  "DESTRUCTOR", "NETRECEIVE", "FOR_NETCONS", "CONDUCTANCE", "GE", "'%'",
  "UNARYMINUS", "$accept", "top", "all", "$@1", "$@2", "model", "line",
  "define1", "Name", "declare", "parmblk", "parmbody", "parmasgn", "units",
  "unit", "@3", "uniton", "limits", "stepblk", "stepbdy", "stepped",
  "numlist", "name", "number", "NUMBER", "integer", "real", "indepblk",
  "indepbody", "$@4", "indepdef", "withby", "depblk", "$@5", "depbody",
  "depdef", "opstart", "abstol", "stateblk", "$@6", "plotdecl", "pvlist",
  "optindex", "proc", "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13",
  "$@14", "$@15", "$@16", "$@17", "$@18", "$@19", "$@20", "$@21", "$@22",
  "$@23", "initblk", "constructblk", "destructblk", "stmtlist", "$@24",
  "conducthint", "locallist", "$@25", "locallist1", "locoptarray",
  "stmtlist1", "$@26", "ostmt", "astmt", "$@27", "$@28", "asgn", "varname",
  "intexpr", "expr", "nonlineqn", "lineqn", "leftlinexpr", "linexpr",
  "primary", "term", "funccall", "$@29", "exprlist", "fromstmt", "$@30",
  "opinc", "forallstmt", "$@31", "whilestmt", "ifstmt", "optelseif",
  "optelse", "derivblk", "linblk", "$@32", "nonlinblk", "discretblk",
  "partialblk", "pareqn", "firstlast", "functableblk", "funcblk", "$@33",
  "arglist", "$@34", "arglist1", "procedblk", "netrecblk", "$@35",
  "initstmt", "solveblk", "ifsolerr", "solvefor", "solvefor1", "brkptblk",
  "terminalblk", "bablk", "watchstmt", "watch1", "watchdir", "fornetcon",
  "$@36", "aexpr", "sens", "senslist", "conserve", "$@37", "compart",
  "$@38", "ldifus", "$@39", "namelist", "kineticblk", "reaction", "$@40",
  "react", "lagstmt", "queuestmt", "matchblk", "$@41", "matchlist",
  "match", "matchname", "unitblk", "unitbody", "unitdef", "factordef",
  "constblk", "conststmt", "tablestmt", "tablst", "tablst1", "dependlst",
  "neuronblk", "nrnstmt", "nrnuse", "optontology", "nrnlist", "optnrnlist",
  "valence", YY_NULLPTR
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
     305,   306,   307,   308,   309,   123,   125,    40,    41,    91,
      93,    64,    43,    42,    45,    47,    61,    94,    58,    44,
     126,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,    37,
     368
};
# endif

#define YYPACT_NINF -569

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-569)))

#define YYTABLE_NINF -392

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     688,  -569,    52,   840,  -569,  -569,  -569,  -569,   -22,    63,
    -569,  -569,   102,    44,   130,    56,    59,    28,    31,   138,
    -569,  -569,   155,   703,  -569,  -569,  -569,  -569,  -569,  -569,
    -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,
    -569,   212,   234,  -569,  -569,   182,  -569,  -569,   246,  -569,
    -569,  -569,  -569,  -569,   274,   191,  -569,  -569,  -569,  -569,
     351,   341,   318,   343,   370,   409,   446,   353,   448,   477,
     480,   411,   410,   388,   478,   427,   415,   127,    33,  -569,
    -569,   221,  -569,   175,   508,   502,   491,  -569,   322,   322,
     209,    95,  1166,   529,  -569,   512,  -569,   529,  -569,   513,
    -569,   514,  -569,   517,  -569,   530,  -569,    10,  -569,   529,
    -569,   533,  -569,    58,  -569,   541,  -569,   529,  -569,   529,
    -569,   543,  -569,   296,   296,  -569,   557,  -569,  -569,   615,
    -569,  -569,   165,   170,  -569,   567,  -569,  -569,  -569,   473,
    -569,   216,  -569,   574,   600,   274,   274,  -569,  -569,  -569,
    -569,   608,    76,  -569,  -569,  -569,   593,    61,    69,   260,
     490,    61,    61,    61,    61,    61,    61,    61,  -569,   634,
     622,   529,   623,   653,   653,   633,   645,  -569,   647,   650,
     529,  -569,   529,   653,   651,   652,   654,  -569,   529,   529,
     529,   529,  -569,  -569,   348,   348,    75,  -569,  -569,   295,
    -569,  -569,   348,   216,   348,   635,  -569,  -569,  -569,   661,
     508,  -569,  -569,  -569,   655,   323,  -569,   676,  -569,  -569,
    -569,   641,  -569,   330,   641,  -569,  -569,   641,   641,   641,
     641,   641,   641,   641,  -569,  -569,  -569,   656,  -569,    94,
    -569,   665,   529,   647,   647,   677,   322,  -569,   680,   694,
     529,  -569,  -569,   647,   697,   705,   706,   707,   315,  -569,
     676,  -569,  -569,   756,  -569,   348,   348,   216,   676,   -11,
     708,   721,   676,   472,  -569,  -569,  -569,  -569,  -569,   -10,
    -569,   676,  -569,   712,    61,    61,   383,   621,  -569,  -569,
    -569,   529,   720,   726,   727,   728,  -569,   676,   715,  -569,
    -569,   744,   751,  -569,  -569,  -569,  -569,  -569,  -569,   348,
     791,  -569,   755,   743,   348,  -569,   348,   676,   635,   749,
     746,  -569,  -569,  -569,   219,   -41,  -569,  -569,  -569,    25,
     761,   560,  -569,   732,  -569,   759,   546,   666,  -569,   774,
    -569,  -569,   676,   676,   529,  -569,   322,  -569,   676,   817,
     348,   290,   472,  -569,  -569,  -569,   635,  -569,   472,   676,
      61,   377,   722,   722,   340,  -569,   216,    67,  -569,  -569,
    -569,   529,   108,   109,   194,   778,   779,    50,   322,   322,
     231,  -569,  -569,   133,    80,  -569,  -569,    99,   723,   823,
      14,    18,   758,  -569,   782,  -569,  -569,  -569,  -569,  -569,
    -569,  -569,  -569,  -569,   773,  -569,  -569,  -569,  -569,  -569,
    -569,  -569,  -569,  -569,   140,  -569,   546,  -569,   546,   546,
     546,   676,  -569,  1124,  -569,  -569,   546,   189,  -569,  -569,
     784,   291,   263,  -569,  -569,  -569,  -569,   529,   796,   676,
    -569,  -569,   216,   829,   348,   676,   781,  -569,   785,  -569,
     -41,   472,  -569,   775,  -569,  -569,   799,  -569,   813,   798,
    -569,    67,  -569,  -569,   525,   805,  -569,    17,  -569,  -569,
    -569,  -569,   546,   546,  -569,  -569,   793,  -569,  -569,  -569,
    -569,   815,   795,   786,   807,   801,  -569,   848,   814,  -569,
     575,   254,   845,   342,   883,  -569,   714,  -569,  -569,   647,
     764,  -569,   818,  -569,   416,  -569,   -37,  1167,   693,   811,
     811,  -569,   546,   546,   546,   546,   546,   546,   546,   546,
     546,   546,   546,   546,   546,   546,   950,   263,   666,   189,
     189,   189,   189,   529,   827,  -569,  -569,   829,   676,   865,
     743,  -569,  -569,   722,  -569,  -569,  -569,  1167,    64,    67,
     487,  -569,    67,    67,    67,    67,  -569,   843,   529,  -569,
     820,   529,   974,   997,   416,   758,   871,   758,   838,   406,
     832,   834,    89,   163,  -569,   835,  -569,   835,  -569,   714,
     714,   676,  -569,  -569,   495,   841,   836,  -569,  -569,   575,
     850,   851,  -569,  -569,   408,   811,   408,   811,  1167,   811,
    1183,  1227,   554,   554,   554,   554,   554,   554,  -569,   291,
     263,   263,  -569,  -569,   868,  -569,   676,   743,   348,  -569,
    -569,  -569,   519,   606,  -569,   369,  -569,   369,  -569,   894,
     872,    67,   873,   529,   529,  -569,   795,   546,  -569,   406,
    -569,  -569,   870,   869,  -569,  -569,   389,   206,  -569,   876,
     546,   546,  -569,   264,   546,   308,   419,   867,  -569,   714,
     714,   714,   714,   714,  -569,  -569,   714,  -569,  -569,   416,
    -569,   878,   879,   575,  -569,  -569,  -569,   829,  -569,  1167,
    -569,  -569,  -569,   285,  -569,   881,   887,   451,   889,   885,
     908,   874,  -569,  -569,   546,  1167,   907,  -569,  -569,   930,
    -569,  -569,   454,   867,   454,   867,   867,   440,   529,  -569,
     546,   546,   147,   676,    67,  -569,  -569,   546,   886,   546,
     904,   903,  1017,   835,   835,   472,   909,  1041,  1064,   546,
     743,   326,   941,   380,   546,  1167,   888,  -569,   900,   314,
     339,  -569,  -569,  -569,  -569,  1142,  -569,    67,   529,   181,
    -569,   893,  1167,   910,   546,  -569,  -569,   546,   620,   920,
     929,   921,  -569,   925,  1167,  1084,  -569,   546,  -569,   911,
    -569,  1108,  -569,   529,   932,  -569
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     3,     0,    11,     1,    13,    14,    18,     0,     0,
      70,    83,     0,     0,     0,     0,     0,     0,     0,     0,
      42,    43,     0,    91,     5,     7,     8,    22,    15,    26,
      23,    24,    25,    27,     6,    29,    30,    28,    17,   358,
      63,     0,     0,    47,    20,     0,    32,   135,     0,    16,
      86,    21,    53,    52,    89,     0,     9,   348,   369,    12,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    72,
      72,     0,    19,     0,   138,   134,     0,    87,     0,     0,
       0,     0,     0,     0,    92,     0,    94,     0,    96,     0,
      98,     0,   100,     0,   102,     0,   104,     0,   106,     0,
     108,     0,   110,     0,   112,     0,   114,     0,   116,     0,
     118,     0,   120,     0,     0,   357,     0,    68,    62,     0,
      65,    64,     0,     0,    46,     0,    48,    37,    31,    38,
      33,     0,   136,     0,     0,    89,    89,    10,   356,   347,
      40,     0,     0,   349,   350,   368,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   371,   128,
       0,     0,     0,   277,   277,     0,     0,   270,   263,     0,
       0,   253,     0,   277,     0,     0,     0,   288,     0,     0,
       0,     0,   122,   124,     0,     0,     0,    78,    71,    79,
      73,    84,     0,     0,     0,    44,    39,    58,    59,     0,
     138,    90,    88,    85,     0,     0,   352,     0,   370,   390,
     388,   373,   385,     0,   392,   381,   372,   377,   375,   376,
     378,   379,   380,   374,   140,   140,   125,     0,   282,     0,
     248,   278,     0,   263,   263,     0,     0,   283,     0,     0,
       0,   126,   127,   263,     0,     0,     0,     0,     0,    57,
      38,    54,    56,     0,    66,     0,     0,     0,    38,    38,
       0,     0,    38,     0,    35,   139,   137,    41,    60,     0,
      61,     0,   351,     0,     0,     0,   141,   141,   247,   281,
     279,     0,     0,     0,     0,     0,   268,    38,   264,   251,
     252,     0,     0,   286,   285,   287,   284,    55,   359,     0,
       0,    80,     0,    81,     0,    49,     0,    38,    44,     0,
       0,   354,   353,   389,   393,   393,   212,   211,   171,    21,
     179,     0,   143,     0,   170,     0,     0,     0,   175,     0,
     280,   250,    38,    38,     0,   265,     0,   324,    38,     0,
       0,    79,     0,    74,    51,    50,    44,    34,     0,     0,
       0,     0,   386,   386,     0,   226,     0,     0,   169,   151,
     152,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,   168,     0,     0,   159,   338,     0,     0,     0,
       0,     0,     0,   163,     0,   150,   142,   144,   145,   146,
     147,   161,   164,   149,   165,   166,   153,   156,   154,   155,
     157,   158,   160,   162,     0,   174,     0,   210,     0,     0,
       0,    38,   191,     0,   193,   225,     0,     0,   222,   221,
       0,   213,   214,   218,   223,   249,   260,     0,     0,    38,
     259,    69,     0,    79,     0,    38,     0,    36,     0,   355,
     393,     0,   394,     0,   382,   383,    21,   172,     0,   181,
     190,     0,   183,   184,     0,     0,   274,   275,   235,   233,
     240,   238,     0,     0,   310,   311,   309,   336,   337,   361,
     364,   366,   363,     0,     0,     0,   335,     0,     0,   315,
       0,    21,     0,    21,     0,   291,     0,   289,   297,   263,
     131,   148,     0,   329,     0,   330,     0,   176,     0,   209,
     208,   192,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   215,     0,     0,
       0,     0,     0,     0,     0,   269,   266,    79,    38,     0,
      81,    82,    45,   386,   395,   387,   230,   229,     0,     0,
       0,   180,     0,     0,     0,     0,   271,     0,     0,   272,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   316,     0,   319,     0,   308,     0,
       0,    38,   298,   300,     0,     0,     0,   290,   331,     0,
       0,     0,   325,   194,   195,   197,   196,   198,   177,   199,
     200,   201,   202,   203,   205,   206,   207,   204,   224,   178,
     216,   217,   219,   220,     0,   267,    38,    81,     0,    75,
     384,   227,     0,     0,   185,   186,   188,   187,   189,   275,
       0,     0,     0,     0,     0,   312,   367,     0,   365,     0,
     257,   258,     0,     0,   334,   344,   342,     0,   340,     0,
       0,     0,   322,     0,     0,     0,     0,   307,   299,     0,
       0,     0,     0,     0,   293,   294,     0,   295,   132,     0,
     332,     0,     0,     0,   261,    67,    76,    79,   232,   231,
     182,   273,   276,     0,   239,     0,     0,     0,     0,     0,
       0,     0,   339,   341,     0,   314,     0,   318,   323,     0,
     321,   301,   302,   304,   303,   305,   306,     0,     0,   333,
       0,     0,     0,    38,     0,   241,   243,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      81,   236,   245,     0,     0,   256,     0,   346,     0,     0,
       0,   292,   296,   328,   327,     0,    77,     0,     0,     0,
     242,     0,   255,     0,     0,   317,   320,     0,   237,     0,
       0,     0,   360,     0,   343,     0,   234,     0,   246,     0,
     326,     0,   254,     0,     0,   244
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,  -314,   899,
    -569,  -569,  -569,  -237,   -83,  -569,   660,  -104,  -569,  -569,
    -569,  -569,   -16,  -178,   738,  -139,  -194,  -569,  -569,  -569,
     802,  -569,  -569,  -569,   919,  -569,  -346,  -533,  -569,  -569,
    -569,  -569,   398,  -569,  -569,  -569,  -569,  -569,  -569,  -569,
    -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,
    -569,  -569,  -569,  -569,   -97,  -569,  -569,   831,  -569,  -569,
     800,   783,  -569,  -569,  -569,  -569,  -569,   664,  -172,  -451,
     122,  -569,  -569,  -569,   481,  -409,    34,  -238,  -569,  -569,
    -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,
    -569,  -569,  -569,  -569,  -569,  -569,   381,  -569,  -569,  -569,
    -240,  -569,  -569,  -569,  -569,  -569,  -569,  -569,   387,    82,
    -569,  -569,  -569,   895,  -569,   527,  -569,  -569,  -569,  -379,
    -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,  -568,  -569,
    -569,  -569,  -474,  -569,  -569,  -569,  -569,  -569,   384,  -569,
    -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,   465,  -569,
    -569,  -569,  -569,  -357,  -123,  -569,  -311
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    90,    23,    24,    38,    25,    53,    26,
      27,    83,   140,   205,   206,   214,    28,   274,    29,    81,
     136,   269,   330,   260,   261,   280,   421,    30,    78,   196,
     131,   442,    31,    41,   132,   200,   268,   353,    32,    42,
      33,    55,    87,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    94,   118,   120,   170,   234,   395,    34,    48,    85,
     142,   286,   331,   396,   332,   364,   333,   334,   422,   464,
     423,   336,   337,   430,   431,   432,   433,   424,   458,   548,
     397,   560,   748,   398,   561,   399,   400,   732,   750,    96,
     100,   291,   102,   112,   114,   401,   642,   122,   104,   533,
     245,   246,   298,   106,   108,   344,   402,   403,   559,   240,
     241,    98,   110,   192,   404,   497,   666,   405,   708,   584,
     406,   476,   407,   490,   408,   651,   409,   654,   653,   116,
     415,   673,   506,   410,   411,   412,   488,   647,   648,   649,
      35,    91,   153,   154,    36,    77,   413,   481,   482,   566,
      37,    92,   168,   454,   221,   225,   362
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     172,    54,   209,   294,   295,   445,   455,   619,   152,   655,
     550,   177,   179,   302,   363,   495,   573,   263,   527,   498,
     184,   281,   185,   308,   270,   589,   272,   590,   283,    50,
     557,   313,   315,    39,   127,   318,   224,   591,   227,   228,
     229,   230,   231,   232,   233,    44,   150,   150,   338,   338,
     558,   474,     4,   462,   320,   262,   262,    47,   314,   181,
     345,   592,   219,   262,   271,   262,   480,   178,   460,   361,
     222,   496,   145,   146,   237,   499,   127,   216,   500,   319,
     356,   486,   365,   248,   676,   249,   366,   310,   311,   128,
     645,   254,   255,   256,   257,   289,   148,   538,   623,   434,
     489,   625,   626,   627,   628,   436,   437,    51,    52,   466,
     468,   440,   129,    56,   335,   335,   199,   199,    40,   262,
     610,   611,   621,    45,   461,   130,   262,   262,   312,   456,
      52,   349,   279,   622,   282,  -133,   354,   182,   355,   543,
     220,   503,   217,   428,    49,   293,    51,   462,   223,   207,
     208,   149,   150,   301,   129,   739,   740,    43,   446,    51,
      52,   324,   325,   483,   448,   429,   197,   452,    51,    52,
     262,   197,   443,   290,   151,   262,   137,   262,  -313,  -313,
     683,  -313,  -313,   125,   511,    46,   620,   467,   469,   434,
     425,   616,   335,    57,   339,   470,   321,   746,   322,   712,
     656,   657,   536,   760,   729,   475,   126,   645,   540,   589,
      58,   262,   484,   485,   357,     8,     9,    10,    11,   456,
      52,   198,   207,   208,    12,   589,   201,   459,   463,   650,
     297,   138,   479,   428,   394,   462,   169,   450,   462,   462,
     462,   462,   505,    14,    51,    52,   426,   438,  -362,    51,
      52,   480,   447,   638,   139,   429,   242,   544,   583,   585,
      88,   219,   692,   731,    82,   250,   539,    79,   329,    52,
     278,   207,   208,   471,   465,   504,   449,   134,    89,  -362,
     702,   703,   704,   705,   706,    51,    52,   707,   283,    80,
     434,   434,   434,   434,   434,    17,   758,   187,   207,   208,
     135,   617,   581,   537,   714,   262,   188,   444,   189,    19,
      51,   365,   265,    22,   266,   366,  -391,   462,   505,   266,
     697,   190,   463,   574,   582,    84,   531,   360,   532,   361,
     439,   713,   588,    86,   428,   428,   428,   428,   428,   220,
     534,   583,   583,   698,   658,   191,   747,   552,   553,   554,
     555,   504,    95,   529,   267,   530,   429,   429,   429,   429,
     429,    93,   477,   478,   700,  -391,  -391,    97,   487,  -391,
     755,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,   675,
     150,    99,  -391,   326,   327,   581,   581,   698,   552,   553,
     554,   555,   635,   698,   751,   756,   259,   207,   208,   365,
     462,    51,    52,   366,   278,   207,   208,   582,   582,   101,
     463,   576,   258,   463,   463,   463,   463,   670,   698,   456,
      52,   583,   583,   583,   583,   583,   326,   327,   583,   259,
     207,   208,   553,   462,   555,   328,   614,   284,   285,  -129,
     677,   451,   512,   513,   514,   515,  -345,   517,   691,   103,
     669,   518,   519,   520,   521,   522,   523,   524,   278,   207,
     208,   630,   329,    52,   632,   581,   581,   581,   581,   581,
     717,   513,   581,   515,   105,   517,   730,   701,   107,   262,
    -173,   659,   660,   661,   662,   109,   663,   582,   582,   582,
     582,   582,   463,   111,   582,   456,    52,   709,   725,   640,
     641,   505,   659,   660,   661,   662,   115,   663,   525,   113,
     492,   494,   119,   512,   513,   514,   515,   660,   517,   662,
     417,   663,   518,   519,   520,   521,   522,   523,   524,   121,
     150,   741,   203,   117,   504,   124,   685,   686,   507,   204,
     508,   509,   510,   212,   213,   624,   123,   417,   526,   552,
     553,   554,   555,   278,   207,   208,   646,   659,   660,   661,
     662,   368,   663,   369,   370,   612,   613,   141,   664,   665,
     371,   143,   372,   144,   226,   463,   418,   373,   374,   525,
     547,   375,   376,   419,   169,   551,   377,   552,   553,   554,
     555,   171,   173,   174,   562,   563,   175,   420,   329,    52,
     278,   207,   208,   418,   678,   378,   379,   380,   463,   176,
     419,   726,   180,   381,   382,   169,   512,   513,   514,   515,
     183,   517,   186,   194,   420,   329,    52,   278,   207,   208,
     383,   646,   195,   202,   594,   595,   596,   597,   598,   599,
     600,   601,   602,   603,   604,   605,   606,   607,   384,   385,
     386,   759,   761,   210,   456,    52,   387,   207,   208,   388,
     211,    20,    21,   389,   326,   327,   680,   425,   552,   553,
     554,   555,   218,   328,   215,    15,   774,  -130,   236,   238,
     239,   390,   552,   553,   554,   555,   391,   392,    -4,     1,
     243,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
     329,    52,   244,    -4,    -4,  -262,   247,   251,   252,   273,
     283,   253,   288,   277,   -93,   578,    -4,    -4,  -173,  -109,
      -4,   275,    -4,   426,   417,    -4,    -4,    -4,    -4,    -4,
     427,  -103,  -111,   150,   292,   296,   299,    -4,    -4,    -4,
    -107,   -97,   -99,  -101,   679,   329,    52,   278,   207,   208,
     300,   593,   -95,   303,  -119,   512,   513,   514,   515,   687,
     517,   304,   305,   306,   518,   519,   520,   521,   522,   523,
     524,   579,   695,   696,    -4,   309,   699,   316,   580,    -4,
     418,   317,   341,    -4,   346,   342,   343,   419,    -4,    -4,
      -4,   323,    -4,   329,    52,   278,   207,   208,  -113,   340,
     347,   420,   491,    52,   278,   207,   208,    -4,    -4,   348,
     350,    -4,    -4,    -4,   417,   351,   722,   352,   358,   359,
     367,   525,  -121,  -123,   417,   416,  -115,  -117,  -105,   414,
     435,   441,   727,   728,   453,   472,   473,    51,   501,   733,
      -2,   735,   502,     5,     6,     7,     8,     9,    10,    11,
     528,   745,   535,   266,   541,    12,   752,   549,   542,   545,
     366,   556,   564,   565,   567,   568,   569,   570,   571,   572,
     418,  -228,    13,   586,    14,   496,   764,   419,   517,   765,
     418,    15,  -228,   615,   618,   629,   631,   419,   637,   771,
      16,   420,   329,    52,   278,   207,   208,   639,   546,   667,
     575,   420,   493,    52,   278,   207,   208,   512,   513,   514,
     515,   643,   517,   644,   652,   668,   518,   519,   520,   521,
     522,   523,   524,   671,   674,   672,    17,   558,   682,   684,
     689,    18,   690,   694,   663,   710,   711,   715,   577,   720,
      19,    20,    21,   716,    22,   512,   513,   514,   515,   718,
     517,   719,   734,   721,   518,   519,   520,   521,   522,   523,
     524,   736,   723,   737,   749,   742,   754,   753,   763,   512,
     513,   514,   515,   525,   517,   762,   766,   768,   518,   519,
     520,   521,   522,   523,   524,   724,   767,   769,   775,   147,
     772,   393,   512,   513,   514,   515,   307,   517,   264,   133,
     235,   518,   519,   520,   521,   522,   523,   524,   608,   609,
     276,   525,   512,   513,   514,   515,   681,   517,   287,   193,
     688,   518,   519,   520,   521,   522,   523,   524,   457,   587,
     636,   693,   633,     0,     0,   525,   512,   513,   514,   515,
       0,   517,     0,     0,     0,   518,   519,   520,   521,   522,
     523,   524,     0,     0,     0,   634,     0,     0,   525,   512,
     513,   514,   515,     0,   517,     0,     0,     0,   518,   519,
     520,   521,   522,   523,   524,   738,     0,     0,   525,   512,
     513,   514,   515,     0,   517,     0,     0,     0,   518,   519,
     520,   521,   522,   523,   524,     0,     0,     0,     0,   743,
       0,     0,   525,   512,   513,   514,   515,     0,   517,     0,
       0,     0,   518,   519,   520,   521,   522,   523,   524,     0,
       0,     0,   744,     0,     0,   525,   512,   513,   514,   515,
       0,   517,     0,     0,     0,   518,   519,   520,   521,   522,
     523,   524,   770,     0,     0,   525,   512,   513,   514,   515,
       0,   517,     0,     0,     0,   518,   519,   520,   521,   522,
     523,   524,     0,     0,     0,     0,   773,     0,     0,   525,
     512,   513,   514,   515,     0,   517,     0,     0,     0,   518,
     519,   520,   521,   522,   523,   524,   512,   513,   514,   515,
     516,   517,   525,     0,     0,   518,   519,   520,   521,   522,
     523,   524,     0,     0,   512,   513,   514,   515,     0,   517,
       0,   757,   525,   518,   519,   520,   521,   522,   523,   524,
       0,     0,   155,     0,     0,     0,     0,     0,     0,   512,
     513,   514,   515,     0,   517,     0,   525,     0,   518,   519,
     520,   521,   522,   523,   524,   512,   513,   514,   515,     0,
     517,     0,   525,     0,     0,   519,   520,   521,   522,   523,
     524,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,   156,   157,     0,     0,   158,     0,   159,   160,   161,
     162,   163,   164,   165,   166,     0,     0,     0,   167,   512,
     513,   514,   515,     0,   517,   525,     0,     0,     0,     0,
     520,   521,   522,   523,   524,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525
};

static const yytype_int16 yycheck[] =
{
      97,    17,   141,   243,   244,   351,   363,   540,    91,   577,
     461,     1,   109,   253,   325,     1,   490,   195,   427,     1,
     117,   215,   119,   260,   202,    62,   204,    64,    69,     1,
      13,   268,   269,    55,     1,   272,   159,    74,   161,   162,
     163,   164,   165,   166,   167,     1,    57,    57,   286,   287,
      33,     1,     0,   367,    64,   194,   195,     1,    69,     1,
     297,    98,     1,   202,   203,   204,   380,    57,     1,   110,
       1,    57,    88,    89,   171,    57,     1,     1,   392,   273,
     317,     1,    57,   180,   617,   182,    61,   265,   266,    56,
       1,   188,   189,   190,   191,     1,     1,   443,   549,   337,
       1,   552,   553,   554,   555,   342,   343,    79,    80,     1,
       1,   348,    79,    82,   286,   287,   132,   133,    55,   258,
     529,   530,    58,    79,    57,    92,   265,   266,   267,    79,
      80,   309,   215,    69,   217,    79,   314,    79,   316,   450,
      79,     1,    66,   337,    85,   242,    79,   461,    79,    82,
      83,    56,    57,   250,    79,   723,   724,    55,   352,    79,
      80,   284,   285,    30,   358,   337,     1,   361,    79,    80,
     309,     1,   350,    79,    79,   314,     1,   316,    79,    80,
     631,    82,    83,    56,   421,    55,   543,    79,    79,   427,
       1,   537,   364,    55,   291,     1,   279,   730,   281,   673,
     579,   580,   439,    22,    57,   377,    79,     1,   445,    62,
      55,   350,    79,    80,   318,     6,     7,     8,     9,    79,
      80,    56,    82,    83,    15,    62,    56,   366,   367,    66,
     246,    56,     1,   427,   331,   549,    55,   360,   552,   553,
     554,   555,   414,    34,    79,    80,    57,   344,    17,    79,
      80,   565,   356,   567,    79,   427,   174,   451,   496,   499,
      69,     1,    56,   714,    82,   183,   444,    55,    79,    80,
      81,    82,    83,    79,   371,   414,   359,    56,    87,    48,
     659,   660,   661,   662,   663,    79,    80,   666,    69,    55,
     528,   529,   530,   531,   532,    86,   747,     1,    82,    83,
      79,   538,   496,   442,    19,   444,    10,    17,    12,   100,
      79,    57,    17,   104,    24,    61,    56,   631,   490,    24,
      56,    25,   461,    69,   496,    79,    63,   108,    65,   110,
     346,   677,   504,    59,   528,   529,   530,   531,   532,    79,
     437,   579,   580,    79,   581,    49,    20,    62,    63,    64,
      65,   490,    11,    62,    59,    64,   528,   529,   530,   531,
     532,    10,   378,   379,    56,   105,   106,    49,   384,   109,
      56,   111,   112,   113,   114,   115,   116,   117,   118,   616,
      57,    38,   122,    43,    44,   579,   580,    79,    62,    63,
      64,    65,   564,    79,    14,    56,    81,    82,    83,    57,
     714,    79,    80,    61,    81,    82,    83,   579,   580,    39,
     549,    69,    64,   552,   553,   554,   555,   589,    79,    79,
      80,   659,   660,   661,   662,   663,    43,    44,   666,    81,
      82,    83,    63,   747,    65,    52,   533,   107,   108,    56,
     618,    64,    62,    63,    64,    65,    57,    67,    59,    40,
     589,    71,    72,    73,    74,    75,    76,    77,    81,    82,
      83,   558,    79,    80,   561,   659,   660,   661,   662,   663,
      19,    63,   666,    65,    28,    67,   713,    58,   125,   618,
      97,    62,    63,    64,    65,    37,    67,   659,   660,   661,
     662,   663,   631,    16,   666,    79,    80,   669,    58,    93,
      94,   673,    62,    63,    64,    65,    95,    67,   128,    29,
     388,   389,   124,    62,    63,    64,    65,    63,    67,    65,
       1,    67,    71,    72,    73,    74,    75,    76,    77,    51,
      57,   725,    59,   123,   673,   120,   633,   634,   416,    66,
     418,   419,   420,   145,   146,    58,   119,     1,   426,    62,
      63,    64,    65,    81,    82,    83,   572,    62,    63,    64,
      65,     1,    67,     3,     4,   531,   532,    59,    73,    74,
      10,    69,    12,    82,    84,   714,    57,    17,    18,   128,
     458,    21,    22,    64,    55,    60,    26,    62,    63,    64,
      65,    79,    79,    79,   472,   473,    79,    78,    79,    80,
      81,    82,    83,    57,    85,    45,    46,    47,   747,    79,
      64,   708,    79,    53,    54,    55,    62,    63,    64,    65,
      79,    67,    79,    66,    78,    79,    80,    81,    82,    83,
      70,   647,    17,    66,   512,   513,   514,   515,   516,   517,
     518,   519,   520,   521,   522,   523,   524,   525,    88,    89,
      90,   748,   749,    79,    79,    80,    96,    82,    83,    99,
      60,   101,   102,   103,    43,    44,    60,     1,    62,    63,
      64,    65,    79,    52,    66,    41,   773,    56,    56,    56,
      27,   121,    62,    63,    64,    65,   126,   127,     0,     1,
      57,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      79,    80,    57,    15,    16,    58,    56,    56,    56,    74,
      69,    57,    56,    58,    11,     1,    28,    29,    97,    16,
      32,    60,    34,    57,     1,    37,    38,    39,    40,    41,
      64,    28,    29,    57,    69,    58,    56,    49,    50,    51,
      37,    38,    39,    40,   622,    79,    80,    81,    82,    83,
      56,    58,    49,    56,    51,    62,    63,    64,    65,   637,
      67,    56,    56,    56,    71,    72,    73,    74,    75,    76,
      77,    57,   650,   651,    86,    19,   654,    69,    64,    91,
      57,    60,    56,    95,    69,    58,    58,    64,   100,   101,
     102,    79,   104,    79,    80,    81,    82,    83,    95,    79,
      56,    78,    79,    80,    81,    82,    83,   119,   120,    58,
      19,   123,   124,   125,     1,    60,   694,    74,    69,    73,
      59,   128,   119,   120,     1,    66,   123,   124,   125,    97,
      56,    14,   710,   711,   112,    57,    57,    79,    56,   717,
       0,   719,    69,     3,     4,     5,     6,     7,     8,     9,
      66,   729,    56,    24,    73,    15,   734,    59,    73,    84,
      61,    56,    69,    48,    69,    79,    59,    66,    20,    55,
      57,    58,    32,   109,    34,    57,   754,    64,    67,   757,
      57,    41,    69,    56,    19,    42,    66,    64,    17,   767,
      50,    78,    79,    80,    81,    82,    83,    59,    85,    58,
      55,    78,    79,    80,    81,    82,    83,    62,    63,    64,
      65,    79,    67,    79,    79,    79,    71,    72,    73,    74,
      75,    76,    77,    73,    56,    74,    86,    33,    56,    56,
      60,    91,    63,    57,    67,    57,    57,    56,    55,    31,
     100,   101,   102,    56,   104,    62,    63,    64,    65,    60,
      67,    66,    66,    79,    71,    72,    73,    74,    75,    76,
      77,    57,    55,    60,    23,    56,    66,    79,    58,    62,
      63,    64,    65,   128,    67,    82,    56,    56,    71,    72,
      73,    74,    75,    76,    77,    55,    57,    62,    56,    90,
      79,   331,    62,    63,    64,    65,   258,    67,   196,    80,
     169,    71,    72,    73,    74,    75,    76,    77,    58,   528,
     210,   128,    62,    63,    64,    65,   629,    67,   235,   124,
     639,    71,    72,    73,    74,    75,    76,    77,   364,   502,
     565,   647,    58,    -1,    -1,   128,    62,    63,    64,    65,
      -1,    67,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    -1,    -1,    -1,    58,    -1,    -1,   128,    62,
      63,    64,    65,    -1,    67,    -1,    -1,    -1,    71,    72,
      73,    74,    75,    76,    77,    58,    -1,    -1,   128,    62,
      63,    64,    65,    -1,    67,    -1,    -1,    -1,    71,    72,
      73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,    58,
      -1,    -1,   128,    62,    63,    64,    65,    -1,    67,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    -1,
      -1,    -1,    58,    -1,    -1,   128,    62,    63,    64,    65,
      -1,    67,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    58,    -1,    -1,   128,    62,    63,    64,    65,
      -1,    67,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    -1,    -1,    -1,    -1,    58,    -1,    -1,   128,
      62,    63,    64,    65,    -1,    67,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    62,    63,    64,    65,
      66,    67,   128,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    -1,    -1,    62,    63,    64,    65,    -1,    67,
      -1,    69,   128,    71,    72,    73,    74,    75,    76,    77,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    67,    -1,   128,    -1,    71,    72,
      73,    74,    75,    76,    77,    62,    63,    64,    65,    -1,
      67,    -1,   128,    -1,    -1,    72,    73,    74,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,   105,   106,    -1,    -1,   109,    -1,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,    -1,   122,    62,
      63,    64,    65,    -1,    67,   128,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,   132,   133,     0,     3,     4,     5,     6,     7,
       8,     9,    15,    32,    34,    41,    50,    86,    91,   100,
     101,   102,   104,   135,   136,   138,   140,   141,   147,   149,
     158,   163,   169,   171,   198,   281,   285,   291,   137,    55,
      55,   164,   170,    55,     1,    79,    55,     1,   199,    85,
       1,    79,    80,   139,   153,   172,    82,    55,    55,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   286,   159,    55,
      55,   150,    82,   142,    79,   200,    59,   173,    69,    87,
     134,   282,   292,    10,   192,    11,   230,    49,   252,    38,
     231,    39,   233,    40,   239,    28,   244,   125,   245,    37,
     253,    16,   234,    29,   235,    95,   270,   123,   193,   124,
     194,    51,   238,   119,   120,    56,    79,     1,    56,    79,
      92,   161,   165,   165,    56,    79,   151,     1,    56,    79,
     143,    59,   201,    69,    82,   153,   153,   140,     1,    56,
      57,    79,   145,   283,   284,    56,   105,   106,   109,   111,
     112,   113,   114,   115,   116,   117,   118,   122,   293,    55,
     195,    79,   195,    79,    79,    79,    79,     1,    57,   195,
      79,     1,    79,    79,   195,   195,    79,     1,    10,    12,
      25,    49,   254,   254,    66,    17,   160,     1,    56,   153,
     166,    56,    66,    59,    66,   144,   145,    82,    83,   156,
      79,    60,   173,   173,   146,    66,     1,    66,    79,     1,
      79,   295,     1,    79,   295,   296,    84,   295,   295,   295,
     295,   295,   295,   295,   196,   198,    56,   195,    56,    27,
     250,   251,   250,    57,    57,   241,   242,    56,   195,   195,
     250,    56,    56,    57,   195,   195,   195,   195,    64,    81,
     154,   155,   156,   154,   161,    17,    24,    59,   167,   152,
     154,   156,   154,    74,   148,    60,   201,    58,    81,   145,
     156,   157,   145,    69,   107,   108,   202,   202,    56,     1,
      79,   232,    69,   195,   241,   241,    58,   153,   243,    56,
      56,   195,   241,    56,    56,    56,    56,   155,   144,    19,
     154,   154,   156,   144,    69,   144,    69,    60,   144,   157,
      64,   145,   145,    79,   295,   295,    43,    44,    52,    79,
     153,   203,   205,   207,   208,   209,   212,   213,   218,   195,
      79,    56,    58,    58,   246,   144,    69,    56,    58,   154,
      19,    60,    74,   168,   154,   154,   144,   148,    69,    73,
     108,   110,   297,   297,   206,    57,    61,    59,     1,     3,
       4,    10,    12,    17,    18,    21,    22,    26,    45,    46,
      47,    53,    54,    70,    88,    89,    90,    96,    99,   103,
     121,   126,   127,   147,   195,   197,   204,   221,   224,   226,
     227,   236,   247,   248,   255,   258,   261,   263,   265,   267,
     274,   275,   276,   287,    97,   271,    66,     1,    57,    64,
      78,   157,   209,   211,   218,     1,    57,    64,   157,   209,
     214,   215,   216,   217,   218,    56,   144,   144,   195,   153,
     144,    14,   162,   154,    17,   167,   157,   148,   157,   145,
     295,    64,   157,   112,   294,   294,    79,   208,   219,   156,
       1,    57,   139,   156,   210,   195,     1,    79,     1,    79,
       1,    79,    57,    57,     1,   209,   262,   153,   153,     1,
     139,   288,   289,    30,    79,    80,     1,   153,   277,     1,
     264,    79,   211,    79,   211,     1,    57,   256,     1,    57,
     139,    56,    69,     1,   156,   209,   273,   211,   211,   211,
     211,   144,    62,    63,    64,    65,    66,    67,    71,    72,
      73,    74,    75,    76,    77,   128,   211,   216,    66,    62,
      64,    63,    65,   240,   195,    56,   144,   156,   167,   154,
     144,    73,    73,   297,   157,    84,    85,   211,   220,    59,
     210,    60,    62,    63,    64,    65,    56,    13,    33,   249,
     222,   225,   211,   211,    69,    48,   290,    69,    79,    59,
      66,    20,    55,   273,    69,    55,    69,    55,     1,    57,
      64,   157,   209,   218,   260,   241,   109,   256,   209,    62,
      64,    74,    98,    58,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,    58,   215,
     216,   216,   217,   217,   195,    56,   167,   144,    19,   168,
     294,    58,    69,   210,    58,   210,   210,   210,   210,    42,
     195,    66,   195,    58,    58,   209,   289,    17,   139,    59,
      93,    94,   237,    79,    79,     1,   153,   278,   279,   280,
      66,   266,    79,   269,   268,   269,   260,   260,   144,    62,
      63,    64,    65,    67,    73,    74,   257,    58,    79,   156,
     209,    73,    74,   272,    56,   144,   168,   154,    85,   211,
      60,   249,    56,   210,    56,   195,   195,   211,   237,    60,
      63,    59,    56,   279,    57,   211,   211,    56,    79,   211,
      56,    58,   260,   260,   260,   260,   260,   260,   259,   209,
      57,    57,   273,   167,    19,    56,    56,    19,    60,    66,
      31,    79,   211,    55,    55,    58,   195,   211,   211,    57,
     144,   210,   228,   211,    66,   211,    57,    60,    58,   269,
     269,   157,    56,    58,    58,   211,   168,    20,   223,    23,
     229,    14,   211,    79,    66,    56,    56,    69,   210,   195,
      22,   195,    82,    58,   211,   211,    56,    57,    56,    62,
      58,   211,    79,    58,   195,    56
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   131,   132,   132,   133,   133,   133,   133,   133,   134,
     133,   135,   133,   133,   133,   133,   133,   136,   137,   138,
     138,   139,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   141,   142,   142,   143,   143,   143,   143,   144,   144,
     146,   145,   147,   147,   148,   148,   149,   150,   150,   151,
     152,   152,   153,   153,   154,   154,   155,   155,   156,   156,
     157,   157,   158,   159,   159,   160,   159,   161,   161,   162,
     164,   163,   165,   165,   166,   166,   166,   166,   166,   167,
     167,   168,   168,   170,   169,   171,   171,   172,   172,   173,
     173,   175,   174,   176,   174,   177,   174,   178,   174,   179,
     174,   180,   174,   181,   174,   182,   174,   183,   174,   184,
     174,   185,   174,   186,   174,   187,   174,   188,   174,   189,
     174,   190,   174,   191,   174,   192,   193,   194,   196,   195,
     195,   197,   197,   199,   198,   198,   200,   200,   201,   201,
     202,   203,   202,   202,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     205,   206,   205,   207,   205,   205,   208,   208,   208,   209,
     209,   209,   209,   210,   210,   210,   210,   210,   210,   210,
     210,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   212,   213,   214,   215,   215,   215,   215,   216,   216,
     216,   217,   217,   217,   217,   217,   219,   218,   220,   220,
     220,   220,   220,   222,   221,   221,   223,   223,   225,   224,
     224,   226,   227,   228,   228,   229,   229,   230,   232,   231,
     233,   234,   235,   235,   236,   236,   236,   237,   237,   238,
     240,   239,   241,   242,   241,   243,   243,   244,   246,   245,
     245,   247,   248,   248,   248,   249,   249,   250,   250,   251,
     251,   251,   252,   253,   254,   254,   254,   254,   254,   255,
     255,   255,   256,   257,   257,   259,   258,   258,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   261,
     261,   262,   262,   264,   263,   263,   266,   265,   265,   268,
     267,   267,   269,   269,   270,   272,   271,   271,   271,   271,
     273,   273,   273,   273,   274,   274,   275,   275,   277,   276,
     278,   278,   279,   279,   279,   280,   280,   281,   282,   282,
     282,   283,   283,   284,   284,   284,   284,   285,   286,   286,
     287,   287,   288,   288,   289,   289,   290,   290,   291,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   293,   293,   293,   293,   294,   294,   295,   295,
     295,   296,   296,   297,   297,   297
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     0,     2,     2,     2,     2,     0,
       5,     0,     3,     2,     2,     2,     3,     2,     0,     3,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     0,     2,     5,     3,     6,     1,     0,     1,
       0,     3,     1,     1,     0,     5,     4,     0,     2,     4,
       3,     3,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     4,     0,     2,     0,     4,     9,     1,     1,
       0,     5,     0,     2,     4,     7,     8,    11,     1,     0,
       2,     0,     3,     0,     5,     5,     2,     2,     4,     0,
       3,     0,     2,     0,     2,     0,     2,     0,     2,     0,
       2,     0,     2,     0,     2,     0,     2,     0,     2,     0,
       2,     0,     2,     0,     2,     0,     2,     0,     2,     0,
       2,     0,     3,     0,     3,     3,     3,     3,     0,     3,
       3,     2,     4,     0,     3,     2,     2,     4,     0,     3,
       0,     0,     3,     2,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     0,     2,     1,     3,     4,     4,     1,
       4,     3,     6,     1,     1,     3,     3,     3,     3,     3,
       1,     1,     2,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       1,     1,     1,     1,     1,     2,     3,     3,     1,     3,
       3,     1,     1,     1,     3,     1,     0,     5,     0,     1,
       1,     3,     3,     0,    10,     2,     0,     2,     0,     5,
       2,     6,     8,     0,     8,     0,     3,     4,     0,     6,
       5,     4,     4,     2,    11,     8,     7,     1,     1,     6,
       0,     9,     0,     0,     2,     2,     4,     8,     0,     7,
       2,     3,     3,     5,     2,     0,     3,     0,     1,     2,
       3,     2,     3,     3,     3,     3,     3,     3,     1,     2,
       3,     2,     6,     1,     1,     0,     7,     2,     1,     2,
       1,     3,     3,     3,     3,     3,     3,     2,     1,     2,
       2,     1,     3,     0,     5,     2,     0,     8,     5,     0,
       8,     5,     1,     2,     5,     0,    10,     7,     7,     2,
       1,     2,     3,     4,     4,     2,     2,     2,     0,     5,
       1,     2,     1,     6,     1,     1,     4,     4,     0,     2,
       2,     3,     2,     4,     4,     6,     1,     4,     0,     5,
       9,     2,     0,     1,     1,     3,     0,     2,     4,     0,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     6,     8,     2,     0,     2,     1,     3,
       1,     0,     1,     0,     2,     3
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
        case 2:
#line 133 "parse1.y" /* yacc.c:1646  */
    {/*ldebug(top, intoken)*/;}
#line 2213 "parse1.c" /* yacc.c:1646  */
    break;

  case 3:
#line 134 "parse1.y" /* yacc.c:1646  */
    {diag("Illegal block", (char *)0);}
#line 2219 "parse1.c" /* yacc.c:1646  */
    break;

  case 6:
#line 140 "parse1.y" /* yacc.c:1646  */
    {Item* q; replacstr((yyvsp[0].qp), "static double");
		vectorize_substitute((yyvsp[0].qp), "/*Top LOCAL");
		vectorize_substitute(lastok->next, "*/\n");
		movelist((yyvsp[0].qp), lastok->next, firstlist);
		if (!toplocal_) {toplocal_ = newlist();}
		ITERATE(q, toplocal1_) {
			assert(SYM(q)->name[0] == '_' && SYM(q)->name[1] == 'l');
			SYM(q)->name[1] = 'z';
		}
		movelist(toplocal1_->next, toplocal1_->prev, toplocal_);
		}
#line 2235 "parse1.c" /* yacc.c:1646  */
    break;

  case 9:
#line 153 "parse1.y" /* yacc.c:1646  */
    {model_level = atoi(STR((yyvsp[0].qp)));}
#line 2241 "parse1.c" /* yacc.c:1646  */
    break;

  case 10:
#line 154 "parse1.y" /* yacc.c:1646  */
    {model_level = 0;}
#line 2247 "parse1.c" /* yacc.c:1646  */
    break;

  case 11:
#line 156 "parse1.y" /* yacc.c:1646  */
    {if (sensused)
			diag("sensitivity analysis not implemented for",
				" this block type");
		}
#line 2256 "parse1.c" /* yacc.c:1646  */
    break;

  case 13:
#line 163 "parse1.y" /* yacc.c:1646  */
    {inblock(SYM((yyvsp[0].qp))->name); replacstr((yyvsp[0].qp), "\n/*VERBATIM*/\n");
		if (!assert_threadsafe && !saw_verbatim_) {
 		 fprintf(stderr, "Notice: VERBATIM blocks are not thread safe\n");
		 saw_verbatim_ = 1;
		 vectorize = 0;
		}
		movelist((yyvsp[0].qp),intoken->prev, procfunc);}
#line 2268 "parse1.c" /* yacc.c:1646  */
    break;

  case 14:
#line 172 "parse1.y" /* yacc.c:1646  */
    {
		in_comment_ = 1;
		inblock(SYM((yyvsp[0].qp))->name); deltokens((yyvsp[0].qp), intoken->prev);
		in_comment_ = 0;
		}
#line 2278 "parse1.c" /* yacc.c:1646  */
    break;

  case 16:
#line 179 "parse1.y" /* yacc.c:1646  */
    {include_file((yyvsp[0].qp));}
#line 2284 "parse1.c" /* yacc.c:1646  */
    break;

  case 17:
#line 182 "parse1.y" /* yacc.c:1646  */
    {if (modelline == NULL) modelline = (yyvsp[0].str);
			deltokens((yyvsp[-1].qp), intoken->prev);}
#line 2291 "parse1.c" /* yacc.c:1646  */
    break;

  case 18:
#line 185 "parse1.y" /* yacc.c:1646  */
    {(yyval.str) = inputline();}
#line 2297 "parse1.c" /* yacc.c:1646  */
    break;

  case 19:
#line 190 "parse1.y" /* yacc.c:1646  */
    { Symbol *sp = SYM((yyvsp[-1].qp));
	 if (sp->subtype)
		diag(sp->name, " used before DEFINEed");
	 sp->u.str = STR((yyvsp[0].qp));
	 sp->type = DEFINEDVAR;
	 deltokens((yyvsp[-2].qp), (yyvsp[0].qp));}
#line 2308 "parse1.c" /* yacc.c:1646  */
    break;

  case 20:
#line 196 "parse1.y" /* yacc.c:1646  */
    {myerr("syntax: DEFINE name integer");}
#line 2314 "parse1.c" /* yacc.c:1646  */
    break;

  case 21:
#line 199 "parse1.y" /* yacc.c:1646  */
    { Symbol *checklocal();
		  SYM((yyvsp[0].qp)) = checklocal(SYM((yyvsp[0].qp)));  /* it was a bug
			when this was done to the lookahead token in lex */
		}
#line 2323 "parse1.c" /* yacc.c:1646  */
    break;

  case 31:
#line 207 "parse1.y" /* yacc.c:1646  */
    {deltokens((yyvsp[-3].qp), (yyvsp[0].qp));}
#line 2329 "parse1.c" /* yacc.c:1646  */
    break;

  case 33:
#line 211 "parse1.y" /* yacc.c:1646  */
    { explicit_decl(model_level, (yyvsp[0].qp));}
#line 2335 "parse1.c" /* yacc.c:1646  */
    break;

  case 34:
#line 217 "parse1.y" /* yacc.c:1646  */
    {parminstall(SYM((yyvsp[-4].qp)), STR((yyvsp[-2].qp)), (yyvsp[-1].str), (yyvsp[0].str));}
#line 2341 "parse1.c" /* yacc.c:1646  */
    break;

  case 35:
#line 219 "parse1.y" /* yacc.c:1646  */
    {parminstall(SYM((yyvsp[-2].qp)), "0", (yyvsp[-1].str), (yyvsp[0].str));}
#line 2347 "parse1.c" /* yacc.c:1646  */
    break;

  case 36:
#line 221 "parse1.y" /* yacc.c:1646  */
    { int i = atoi(STR((yyvsp[-3].qp)));
		  if (i < 1) diag("Array index must be > 0", (char*)0);
		  parm_array_install(SYM((yyvsp[-5].qp)), "0", (yyvsp[-1].str), (yyvsp[0].str), i);
		}
#line 2356 "parse1.c" /* yacc.c:1646  */
    break;

  case 37:
#line 225 "parse1.y" /* yacc.c:1646  */
    {diag("name = number", (char *)0);}
#line 2362 "parse1.c" /* yacc.c:1646  */
    break;

  case 38:
#line 227 "parse1.y" /* yacc.c:1646  */
    {(yyval.str) = stralloc("", (char *)0);}
#line 2368 "parse1.c" /* yacc.c:1646  */
    break;

  case 40:
#line 230 "parse1.y" /* yacc.c:1646  */
    {(yyval.str) = inputtopar();}
#line 2374 "parse1.c" /* yacc.c:1646  */
    break;

  case 41:
#line 232 "parse1.y" /* yacc.c:1646  */
    {(yyval.str) = (yyvsp[-1].str); delete((yyvsp[-2].qp)); delete((yyvsp[0].qp));}
#line 2380 "parse1.c" /* yacc.c:1646  */
    break;

  case 42:
#line 234 "parse1.y" /* yacc.c:1646  */
    {replacstr((yyvsp[0].qp), "");}
#line 2386 "parse1.c" /* yacc.c:1646  */
    break;

  case 43:
#line 235 "parse1.y" /* yacc.c:1646  */
    {replacstr((yyvsp[0].qp), "");}
#line 2392 "parse1.c" /* yacc.c:1646  */
    break;

  case 44:
#line 238 "parse1.y" /* yacc.c:1646  */
    {(yyval.str) = stralloc("", (char*)0);}
#line 2398 "parse1.c" /* yacc.c:1646  */
    break;

  case 45:
#line 240 "parse1.y" /* yacc.c:1646  */
    {
		 Sprintf(buf, "%s %s", STR((yyvsp[-3].qp)), STR((yyvsp[-1].qp)));
		 (yyval.str) = stralloc(buf, (char*)0);
		}
#line 2407 "parse1.c" /* yacc.c:1646  */
    break;

  case 46:
#line 245 "parse1.y" /* yacc.c:1646  */
    {deltokens((yyvsp[-3].qp), (yyvsp[0].qp));}
#line 2413 "parse1.c" /* yacc.c:1646  */
    break;

  case 48:
#line 249 "parse1.y" /* yacc.c:1646  */
    { explicit_decl(model_level, (yyvsp[0].qp)); }
#line 2419 "parse1.c" /* yacc.c:1646  */
    break;

  case 49:
#line 253 "parse1.y" /* yacc.c:1646  */
    {steppedinstall(SYM((yyvsp[-3].qp)), (yyvsp[-1].qp), lastok, (yyvsp[0].str));}
#line 2425 "parse1.c" /* yacc.c:1646  */
    break;

  case 54:
#line 261 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);}
#line 2431 "parse1.c" /* yacc.c:1646  */
    break;

  case 55:
#line 264 "parse1.y" /* yacc.c:1646  */
    { Sprintf(buf, "-%s", STR((yyvsp[0].qp)));
		 STR((yyvsp[0].qp)) = stralloc(buf, STR((yyvsp[0].qp))); (yyval.qp) = (yyvsp[0].qp);
		delete((yyvsp[-1].qp)); lastok = (yyvsp[0].qp);
		}
#line 2440 "parse1.c" /* yacc.c:1646  */
    break;

  case 59:
#line 272 "parse1.y" /* yacc.c:1646  */
    {replacstr((yyvsp[0].qp), SYM((yyvsp[0].qp))->u.str);}
#line 2446 "parse1.c" /* yacc.c:1646  */
    break;

  case 60:
#line 274 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);}
#line 2452 "parse1.c" /* yacc.c:1646  */
    break;

  case 61:
#line 276 "parse1.y" /* yacc.c:1646  */
    {Sprintf(buf, "%s.0", STR((yyvsp[0].qp)));
		STR((yyvsp[0].qp)) = stralloc(buf, STR((yyvsp[0].qp)));
		lastok = (yyvsp[0].qp);
		}
#line 2461 "parse1.c" /* yacc.c:1646  */
    break;

  case 62:
#line 281 "parse1.y" /* yacc.c:1646  */
    {deltokens((yyvsp[-3].qp), (yyvsp[0].qp));}
#line 2467 "parse1.c" /* yacc.c:1646  */
    break;

  case 64:
#line 285 "parse1.y" /* yacc.c:1646  */
    { explicit_decl(model_level, (yyvsp[0].qp)); }
#line 2473 "parse1.c" /* yacc.c:1646  */
    break;

  case 65:
#line 286 "parse1.y" /* yacc.c:1646  */
    {scopindep = 1;}
#line 2479 "parse1.c" /* yacc.c:1646  */
    break;

  case 66:
#line 287 "parse1.y" /* yacc.c:1646  */
    { explicit_decl(model_level, (yyvsp[0].qp));
		  scopindep = 0;
		}
#line 2487 "parse1.c" /* yacc.c:1646  */
    break;

  case 67:
#line 293 "parse1.y" /* yacc.c:1646  */
    {indepinstall(SYM((yyvsp[-8].qp)), STR((yyvsp[-6].qp)),
		 STR((yyvsp[-4].qp)), STR((yyvsp[-2].qp)), (yyvsp[-1].qp), (yyvsp[0].str), scopindep);
		}
#line 2495 "parse1.c" /* yacc.c:1646  */
    break;

  case 68:
#line 296 "parse1.y" /* yacc.c:1646  */
    {diag("name FROM number TO number WITH integer\n", (char *)0);}
#line 2501 "parse1.c" /* yacc.c:1646  */
    break;

  case 70:
#line 300 "parse1.y" /* yacc.c:1646  */
    {stateblock = 0;}
#line 2507 "parse1.c" /* yacc.c:1646  */
    break;

  case 71:
#line 300 "parse1.y" /* yacc.c:1646  */
    {deltokens((yyvsp[-4].qp), (yyvsp[0].qp));}
#line 2513 "parse1.c" /* yacc.c:1646  */
    break;

  case 73:
#line 304 "parse1.y" /* yacc.c:1646  */
    { explicit_decl(model_level, (yyvsp[0].qp));}
#line 2519 "parse1.c" /* yacc.c:1646  */
    break;

  case 74:
#line 308 "parse1.y" /* yacc.c:1646  */
    {depinstall(stateblock, SYM((yyvsp[-3].qp)), 0, "0", "1", (yyvsp[-1].str), (yyvsp[-2].qp), 1, (yyvsp[0].str));
		 }
#line 2526 "parse1.c" /* yacc.c:1646  */
    break;

  case 75:
#line 311 "parse1.y" /* yacc.c:1646  */
    {int i = atoi(STR((yyvsp[-4].qp)));
		 if (i < 1) diag("Array index must be > 0", (char *)0);
		 depinstall(stateblock, SYM((yyvsp[-6].qp)), i,
			 "0", "1", (yyvsp[-1].str), (yyvsp[-2].qp), 1, (yyvsp[0].str));
		}
#line 2536 "parse1.c" /* yacc.c:1646  */
    break;

  case 76:
#line 317 "parse1.y" /* yacc.c:1646  */
    {depinstall(stateblock, SYM((yyvsp[-7].qp)), 0, STR((yyvsp[-5].qp)),
		 STR((yyvsp[-3].qp)), (yyvsp[-1].str), (yyvsp[-2].qp), 1, (yyvsp[0].str));
		}
#line 2544 "parse1.c" /* yacc.c:1646  */
    break;

  case 77:
#line 321 "parse1.y" /* yacc.c:1646  */
    {int i = atoi(STR((yyvsp[-8].qp)));
		 if (i < 1) diag("Array index must be > 0", (char *)0);
		 depinstall(stateblock, SYM((yyvsp[-10].qp)), i,
		 STR((yyvsp[-5].qp)), STR((yyvsp[-3].qp)), (yyvsp[-1].str), (yyvsp[-2].qp), 1, (yyvsp[0].str));
		}
#line 2554 "parse1.c" /* yacc.c:1646  */
    break;

  case 78:
#line 326 "parse1.y" /* yacc.c:1646  */
    {
diag("name FROM number TO number START number\n",
"FROM...TO and START are optional, name can be name[integer]\n");}
#line 2562 "parse1.c" /* yacc.c:1646  */
    break;

  case 79:
#line 330 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 2568 "parse1.c" /* yacc.c:1646  */
    break;

  case 80:
#line 331 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 2574 "parse1.c" /* yacc.c:1646  */
    break;

  case 81:
#line 334 "parse1.y" /* yacc.c:1646  */
    { (yyval.str) = stralloc("", (char*)0);}
#line 2580 "parse1.c" /* yacc.c:1646  */
    break;

  case 82:
#line 336 "parse1.y" /* yacc.c:1646  */
    {
			(yyval.str) = stralloc(STR((yyvsp[-1].qp)), (char*)0);
		}
#line 2588 "parse1.c" /* yacc.c:1646  */
    break;

  case 83:
#line 340 "parse1.y" /* yacc.c:1646  */
    {stateblock = 1;}
#line 2594 "parse1.c" /* yacc.c:1646  */
    break;

  case 84:
#line 340 "parse1.y" /* yacc.c:1646  */
    {deltokens((yyvsp[-4].qp), (yyvsp[0].qp));}
#line 2600 "parse1.c" /* yacc.c:1646  */
    break;

  case 85:
#line 344 "parse1.y" /* yacc.c:1646  */
    { Item *q;
		 q = linsertsym(plotlist, SYM((yyvsp[-1].qp))); Insertstr(q->next, (yyvsp[0].str)); }
#line 2607 "parse1.c" /* yacc.c:1646  */
    break;

  case 86:
#line 347 "parse1.y" /* yacc.c:1646  */
    { diag("PLOT namelist VS name", (char *)0);}
#line 2613 "parse1.c" /* yacc.c:1646  */
    break;

  case 87:
#line 350 "parse1.y" /* yacc.c:1646  */
    { if (plotlist->next == plotlist) {
			Lappendsym(plotlist, SYM((yyvsp[-1].qp)));
			Lappendstr(plotlist, (yyvsp[0].str));
		  }else{
			diag("Only one PLOT declaration allowed", (char *)0);
		  }
		}
#line 2625 "parse1.c" /* yacc.c:1646  */
    break;

  case 88:
#line 358 "parse1.y" /* yacc.c:1646  */
    { Lappendsym(plotlist, SYM((yyvsp[-1].qp))); Lappendstr(plotlist, (yyvsp[0].str));}
#line 2631 "parse1.c" /* yacc.c:1646  */
    break;

  case 89:
#line 361 "parse1.y" /* yacc.c:1646  */
    { (yyval.str) = "-1";}
#line 2637 "parse1.c" /* yacc.c:1646  */
    break;

  case 90:
#line 363 "parse1.y" /* yacc.c:1646  */
    { (yyval.str) = STR((yyvsp[-1].qp));}
#line 2643 "parse1.c" /* yacc.c:1646  */
    break;

  case 91:
#line 365 "parse1.y" /* yacc.c:1646  */
    {blocktype = INITIAL1;}
#line 2649 "parse1.c" /* yacc.c:1646  */
    break;

  case 93:
#line 367 "parse1.y" /* yacc.c:1646  */
    {lexcontext = NONLINEAR; blocktype = DERIVATIVE;}
#line 2655 "parse1.c" /* yacc.c:1646  */
    break;

  case 95:
#line 368 "parse1.y" /* yacc.c:1646  */
    {blocktype = BREAKPOINT;}
#line 2661 "parse1.c" /* yacc.c:1646  */
    break;

  case 97:
#line 369 "parse1.y" /* yacc.c:1646  */
    {lexcontext = blocktype = LINEAR;}
#line 2667 "parse1.c" /* yacc.c:1646  */
    break;

  case 99:
#line 370 "parse1.y" /* yacc.c:1646  */
    {lexcontext = blocktype = NONLINEAR;}
#line 2673 "parse1.c" /* yacc.c:1646  */
    break;

  case 101:
#line 371 "parse1.y" /* yacc.c:1646  */
    {blocktype = FUNCTION1;}
#line 2679 "parse1.c" /* yacc.c:1646  */
    break;

  case 103:
#line 372 "parse1.y" /* yacc.c:1646  */
    {blocktype = PROCEDURE;}
#line 2685 "parse1.c" /* yacc.c:1646  */
    break;

  case 105:
#line 373 "parse1.y" /* yacc.c:1646  */
    {blocktype = NETRECEIVE;}
#line 2691 "parse1.c" /* yacc.c:1646  */
    break;

  case 107:
#line 374 "parse1.y" /* yacc.c:1646  */
    {blocktype = TERMINAL;}
#line 2697 "parse1.c" /* yacc.c:1646  */
    break;

  case 109:
#line 375 "parse1.y" /* yacc.c:1646  */
    {blocktype = DISCRETE;}
#line 2703 "parse1.c" /* yacc.c:1646  */
    break;

  case 111:
#line 376 "parse1.y" /* yacc.c:1646  */
    {lexcontext = blocktype = PARTIAL;}
#line 2709 "parse1.c" /* yacc.c:1646  */
    break;

  case 113:
#line 377 "parse1.y" /* yacc.c:1646  */
    {lexcontext = blocktype = KINETIC;ostmt_start();}
#line 2715 "parse1.c" /* yacc.c:1646  */
    break;

  case 114:
#line 377 "parse1.y" /* yacc.c:1646  */
    {see_ostmt();}
#line 2721 "parse1.c" /* yacc.c:1646  */
    break;

  case 115:
#line 378 "parse1.y" /* yacc.c:1646  */
    {blocktype = CONSTRUCTOR;}
#line 2727 "parse1.c" /* yacc.c:1646  */
    break;

  case 117:
#line 379 "parse1.y" /* yacc.c:1646  */
    {blocktype = DESTRUCTOR;}
#line 2733 "parse1.c" /* yacc.c:1646  */
    break;

  case 119:
#line 380 "parse1.y" /* yacc.c:1646  */
    {blocktype = FUNCTION_TABLE;}
#line 2739 "parse1.c" /* yacc.c:1646  */
    break;

  case 121:
#line 381 "parse1.y" /* yacc.c:1646  */
    {blocktype = BEFORE;}
#line 2745 "parse1.c" /* yacc.c:1646  */
    break;

  case 123:
#line 382 "parse1.y" /* yacc.c:1646  */
    {blocktype = AFTER;}
#line 2751 "parse1.c" /* yacc.c:1646  */
    break;

  case 125:
#line 385 "parse1.y" /* yacc.c:1646  */
    {movelist((yyvsp[-1].qp), (yyvsp[0].qp), initfunc);}
#line 2757 "parse1.c" /* yacc.c:1646  */
    break;

  case 126:
#line 388 "parse1.y" /* yacc.c:1646  */
    {movelist((yyvsp[-1].qp), (yyvsp[0].qp), constructorfunc);}
#line 2763 "parse1.c" /* yacc.c:1646  */
    break;

  case 127:
#line 391 "parse1.y" /* yacc.c:1646  */
    {movelist((yyvsp[-1].qp), (yyvsp[0].qp), destructorfunc);}
#line 2769 "parse1.c" /* yacc.c:1646  */
    break;

  case 128:
#line 393 "parse1.y" /* yacc.c:1646  */
    {pushlocal();}
#line 2775 "parse1.c" /* yacc.c:1646  */
    break;

  case 129:
#line 393 "parse1.y" /* yacc.c:1646  */
    {poplocal();}
#line 2781 "parse1.c" /* yacc.c:1646  */
    break;

  case 130:
#line 395 "parse1.y" /* yacc.c:1646  */
    {poplocal();}
#line 2787 "parse1.c" /* yacc.c:1646  */
    break;

  case 131:
#line 398 "parse1.y" /* yacc.c:1646  */
    {conductance_hint(blocktype, (yyvsp[-1].qp), (yyvsp[0].qp));}
#line 2793 "parse1.c" /* yacc.c:1646  */
    break;

  case 132:
#line 400 "parse1.y" /* yacc.c:1646  */
    {conductance_hint(blocktype, (yyvsp[-3].qp), (yyvsp[0].qp));}
#line 2799 "parse1.c" /* yacc.c:1646  */
    break;

  case 133:
#line 403 "parse1.y" /* yacc.c:1646  */
    {
		  if (toplocal1_) {freelist(&toplocal1_);}
		  toplocal1_ = newlist();
		}
#line 2808 "parse1.c" /* yacc.c:1646  */
    break;

  case 134:
#line 408 "parse1.y" /* yacc.c:1646  */
    { replacstr((yyvsp[-2].qp), "double");
		  Insertstr(lastok->next, ";\n");
		  possible_local_current(blocktype, toplocal1_);
		}
#line 2817 "parse1.c" /* yacc.c:1646  */
    break;

  case 135:
#line 412 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal LOCAL declaration");}
#line 2823 "parse1.c" /* yacc.c:1646  */
    break;

  case 136:
#line 417 "parse1.y" /* yacc.c:1646  */
    {int a2; pushlocal();
		 a2 = SYM((yyvsp[-1].qp))->assigned_to_; /* in case marked threadsafe */
		 SYM((yyvsp[-1].qp)) = copylocal(SYM((yyvsp[-1].qp)));
		 SYM((yyvsp[-1].qp))->assigned_to_ = a2;
		 lappendsym(toplocal1_, SYM((yyvsp[-1].qp)));
		 if ((yyvsp[0].i)) {
			SYM((yyvsp[-1].qp))->araydim = (yyvsp[0].i);
			SYM((yyvsp[-1].qp))->subtype |= ARRAY;
		 }else{
			lastok = (yyvsp[-1].qp);
		 }
		}
#line 2840 "parse1.c" /* yacc.c:1646  */
    break;

  case 137:
#line 430 "parse1.y" /* yacc.c:1646  */
    {
		 int a2 = SYM((yyvsp[-1].qp))->assigned_to_; /* in case marked threadsafe */
		 SYM((yyvsp[-1].qp)) = copylocal(SYM((yyvsp[-1].qp)));
		 SYM((yyvsp[-1].qp))->assigned_to_ = a2;
		 lappendsym(toplocal1_, SYM((yyvsp[-1].qp)));
		 if ((yyvsp[0].i)) {
			SYM((yyvsp[-1].qp))->araydim = (yyvsp[0].i);
			SYM((yyvsp[-1].qp))->subtype |= ARRAY;
		 }else{
			lastok = (yyvsp[-1].qp);
		 }
		}
#line 2857 "parse1.c" /* yacc.c:1646  */
    break;

  case 138:
#line 444 "parse1.y" /* yacc.c:1646  */
    {(yyval.i) = 0;}
#line 2863 "parse1.c" /* yacc.c:1646  */
    break;

  case 139:
#line 446 "parse1.y" /* yacc.c:1646  */
    {(yyval.i) = atoi(STR((yyvsp[-1].qp))); lastok = (yyvsp[0].qp);}
#line 2869 "parse1.c" /* yacc.c:1646  */
    break;

  case 141:
#line 449 "parse1.y" /* yacc.c:1646  */
    {if (blocktype == KINETIC) see_ostmt();}
#line 2875 "parse1.c" /* yacc.c:1646  */
    break;

  case 142:
#line 450 "parse1.y" /* yacc.c:1646  */
    {if (blocktype == KINETIC) see_ostmt();}
#line 2881 "parse1.c" /* yacc.c:1646  */
    break;

  case 143:
#line 451 "parse1.y" /* yacc.c:1646  */
    {if (blocktype == KINETIC) { see_astmt((yyvsp[0].qp), astmt_end_); }}
#line 2887 "parse1.c" /* yacc.c:1646  */
    break;

  case 151:
#line 461 "parse1.y" /* yacc.c:1646  */
    {inblock(SYM((yyvsp[0].qp))->name);
		replacstr((yyvsp[0].qp), "\n/*VERBATIM*/\n");
		if (!assert_threadsafe && !saw_verbatim_) {
 		 fprintf(stderr, "Notice: VERBATIM blocks are not thread safe\n");
		 saw_verbatim_ = 1;
		 vectorize = 0;
		}
		}
#line 2900 "parse1.c" /* yacc.c:1646  */
    break;

  case 152:
#line 471 "parse1.y" /* yacc.c:1646  */
    {inblock(SYM((yyvsp[0].qp))->name); deltokens((yyvsp[0].qp), intoken->prev);}
#line 2906 "parse1.c" /* yacc.c:1646  */
    break;

  case 154:
#line 473 "parse1.y" /* yacc.c:1646  */
    {check_block(KINETIC, blocktype, "COMPARTMENT");}
#line 2912 "parse1.c" /* yacc.c:1646  */
    break;

  case 155:
#line 474 "parse1.y" /* yacc.c:1646  */
    {check_block(KINETIC, blocktype, "LONGDIFUS");}
#line 2918 "parse1.c" /* yacc.c:1646  */
    break;

  case 156:
#line 475 "parse1.y" /* yacc.c:1646  */
    {check_block(KINETIC, blocktype, "CONSERVE");}
#line 2924 "parse1.c" /* yacc.c:1646  */
    break;

  case 159:
#line 479 "parse1.y" /* yacc.c:1646  */
    { replacstr((yyvsp[0].qp), " _reset = 1;\n"); }
#line 2930 "parse1.c" /* yacc.c:1646  */
    break;

  case 162:
#line 483 "parse1.y" /* yacc.c:1646  */
    {if (blocktype !=FUNCTION1 && blocktype != PROCEDURE) {
			diag("TABLE valid only for FUNCTION or PROCEDURE", (char *)0);
		}}
#line 2938 "parse1.c" /* yacc.c:1646  */
    break;

  case 167:
#line 490 "parse1.y" /* yacc.c:1646  */
    { nrnmutex(1,(yyvsp[0].qp)); }
#line 2944 "parse1.c" /* yacc.c:1646  */
    break;

  case 168:
#line 491 "parse1.y" /* yacc.c:1646  */
    { nrnmutex(0,(yyvsp[0].qp)); }
#line 2950 "parse1.c" /* yacc.c:1646  */
    break;

  case 169:
#line 493 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal statement");}
#line 2956 "parse1.c" /* yacc.c:1646  */
    break;

  case 170:
#line 497 "parse1.y" /* yacc.c:1646  */
    {astmt_end_ = insertsym(lastok->next, semi);}
#line 2962 "parse1.c" /* yacc.c:1646  */
    break;

  case 171:
#line 498 "parse1.y" /* yacc.c:1646  */
    {protect_ = 1;}
#line 2968 "parse1.c" /* yacc.c:1646  */
    break;

  case 172:
#line 499 "parse1.y" /* yacc.c:1646  */
    {protect_ = 0; astmt_end_ = insertsym(lastok->next, semi);
			astmt_end_ = protect_astmt((yyvsp[-2].qp), astmt_end_);
		}
#line 2976 "parse1.c" /* yacc.c:1646  */
    break;

  case 173:
#line 502 "parse1.y" /* yacc.c:1646  */
    {inequation = 1;}
#line 2982 "parse1.c" /* yacc.c:1646  */
    break;

  case 174:
#line 502 "parse1.y" /* yacc.c:1646  */
    {
		(yyval.qp) = (yyvsp[0].qp); inequation = 0;
		astmt_end_ = insertstr(lastok->next->next->next, "");}
#line 2990 "parse1.c" /* yacc.c:1646  */
    break;

  case 175:
#line 506 "parse1.y" /* yacc.c:1646  */
    {astmt_end_ = insertsym(lastok->next, semi);}
#line 2996 "parse1.c" /* yacc.c:1646  */
    break;

  case 176:
#line 511 "parse1.y" /* yacc.c:1646  */
    { if (blocktype == DERIVATIVE && SYM((yyvsp[-2].qp))->type == PRIME) {
			/* put Dvar in a derivative used list */
			deriv_used(SYM((yyvsp[-2].qp)), (yyvsp[0].qp), lastok);
			}
		  if (blocktype == DERIVATIVE && (SYM((yyvsp[-2].qp))->subtype & STAT)) {
			Fprintf(stderr,
"WARNING: %s (a STATE) is assigned a value\
 in a DERIVATIVE block.\n Multistep integrators (such as Runge) may not\
 work correctly.\n", SYM((yyvsp[-2].qp))->name);
		  }
		  if (blocktype == DISCRETE && SYM((yyvsp[-2].qp))->type == NAME
			&& (SYM((yyvsp[-2].qp))->subtype & STAT)) {
			SYM((yyvsp[-2].qp))->used++;
			}
		  if (blocktype == NETRECEIVE) {
			/* STATE discontinuity adjustment */
			netrec_asgn((yyvsp[-2].qp), (yyvsp[-1].qp), (yyvsp[0].qp), lastok);
		  }
#if NOCMODL
		  nrn_var_assigned(SYM((yyvsp[-2].qp)));
#endif
		}
#line 3023 "parse1.c" /* yacc.c:1646  */
    break;

  case 177:
#line 535 "parse1.y" /* yacc.c:1646  */
    { inequation = 0;
		Insertstr((yyvsp[-2].qp), " -(");
		replacstr((yyvsp[-1].qp), ") + ");
		if (nstate == 0)
{yyerror("previous equation contains no state variables"); YYERROR;}
		 eqnqueue((yyvsp[-3].qp));
		}
#line 3035 "parse1.c" /* yacc.c:1646  */
    break;

  case 178:
#line 543 "parse1.y" /* yacc.c:1646  */
    { inequation = 0;
		delete((yyvsp[-1].qp));
		if (nstate == 0)
{yyerror("previous equation contains no state variables"); YYERROR;}
		}
#line 3045 "parse1.c" /* yacc.c:1646  */
    break;

  case 179:
#line 551 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);
		if (!extdef2){SYM((yyvsp[0].qp))->usage |= DEP;}
		if (SYM((yyvsp[0].qp))->subtype & ARRAY && !extdef2)
			{myerr("variable needs an index");}
		if (inequation && (SYM((yyvsp[0].qp))->subtype & STAT) && in_solvefor(SYM((yyvsp[0].qp)))) {
			SYM((yyvsp[0].qp))->used++;
			nstate++; pstate++; tstate++;
		}
		if (SYM((yyvsp[0].qp))->subtype & INTGER) {
			lastok = insertstr((yyvsp[0].qp)->next, ")");
			(yyvsp[0].qp) = insertstr((yyvsp[0].qp), "((double)");
		}
		}
#line 3063 "parse1.c" /* yacc.c:1646  */
    break;

  case 180:
#line 565 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);
		if (SYM((yyvsp[-3].qp))->type == PRIME) {
			usederivstatearray = 1;
		}
		SYM((yyvsp[-3].qp))->usage |= DEP;
		if ((SYM((yyvsp[-3].qp))->subtype & ARRAY) == 0)
			{myerr("variable is not an array");}
		if (inequation && (SYM((yyvsp[-3].qp))->subtype & STAT) && in_solvefor(SYM((yyvsp[-3].qp)))) {
			SYM((yyvsp[-3].qp))->used++;
			nstate++; pstate++; tstate++;
		}
		  if (forallindex) {
			if (forallindex == -1) {
				forallindex = SYM((yyvsp[-3].qp))->araydim;
			}
			if (forallindex != SYM((yyvsp[-3].qp))->araydim) {
Sprintf(buf, "%s dimension not same as other dimensions used in FORALL statement",
SYM((yyvsp[-3].qp))->name);
				diag(buf, (char *)0);
			}
		  }
		}
#line 3090 "parse1.c" /* yacc.c:1646  */
    break;

  case 181:
#line 588 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);
		SYM((yyvsp[-2].qp))->usage |= DEP; disc_var_seen((yyvsp[-2].qp), (yyvsp[-1].qp), (yyvsp[0].qp), 0);}
#line 3097 "parse1.c" /* yacc.c:1646  */
    break;

  case 182:
#line 591 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);
		SYM((yyvsp[-5].qp))->usage |= DEP; disc_var_seen((yyvsp[-5].qp), (yyvsp[-4].qp), (yyvsp[-3].qp), ARRAY);}
#line 3104 "parse1.c" /* yacc.c:1646  */
    break;

  case 183:
#line 595 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);
		 SYM((yyvsp[0].qp))->usage |= DEP;
		 if (!(SYM((yyvsp[0].qp))->subtype & INTGER)) {
		 	lastok = insertstr((yyvsp[0].qp)->next, ")");
			(yyvsp[0].qp) = insertstr((yyvsp[0].qp), "((int)");
		 }
		}
#line 3116 "parse1.c" /* yacc.c:1646  */
    break;

  case 184:
#line 602 "parse1.y" /* yacc.c:1646  */
    { lastok = (yyvsp[0].qp);}
#line 3122 "parse1.c" /* yacc.c:1646  */
    break;

  case 185:
#line 603 "parse1.y" /* yacc.c:1646  */
    { lastok = (yyvsp[0].qp);}
#line 3128 "parse1.c" /* yacc.c:1646  */
    break;

  case 190:
#line 608 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal integer expression");}
#line 3134 "parse1.c" /* yacc.c:1646  */
    break;

  case 194:
#line 613 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);}
#line 3140 "parse1.c" /* yacc.c:1646  */
    break;

  case 199:
#line 619 "parse1.y" /* yacc.c:1646  */
    { (yyval.qp) = insertstr((yyvsp[-2].qp), "pow("); replacstr((yyvsp[-1].qp), ",");
			lastok = insertstr(lastok->next, ")"); }
#line 3147 "parse1.c" /* yacc.c:1646  */
    break;

  case 200:
#line 621 "parse1.y" /* yacc.c:1646  */
    {replacstr((yyvsp[-1].qp), " ||");}
#line 3153 "parse1.c" /* yacc.c:1646  */
    break;

  case 201:
#line 622 "parse1.y" /* yacc.c:1646  */
    {replacstr((yyvsp[-1].qp), " &&");}
#line 3159 "parse1.c" /* yacc.c:1646  */
    break;

  case 206:
#line 627 "parse1.y" /* yacc.c:1646  */
    {replacstr((yyvsp[-1].qp), " ==");}
#line 3165 "parse1.c" /* yacc.c:1646  */
    break;

  case 207:
#line 628 "parse1.y" /* yacc.c:1646  */
    {replacstr((yyvsp[-1].qp), " !=");}
#line 3171 "parse1.c" /* yacc.c:1646  */
    break;

  case 208:
#line 629 "parse1.y" /* yacc.c:1646  */
    {replacstr((yyvsp[-1].qp), " !");}
#line 3177 "parse1.c" /* yacc.c:1646  */
    break;

  case 210:
#line 631 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal expression");}
#line 3183 "parse1.c" /* yacc.c:1646  */
    break;

  case 211:
#line 633 "parse1.y" /* yacc.c:1646  */
    {inequation = 1; nstate = 0;}
#line 3189 "parse1.c" /* yacc.c:1646  */
    break;

  case 212:
#line 637 "parse1.y" /* yacc.c:1646  */
    {inequation = 1; nstate = 0;
		pstate = 0; tstate = 0; init_lineq((yyvsp[0].qp)); leftside = -1;}
#line 3196 "parse1.c" /* yacc.c:1646  */
    break;

  case 213:
#line 640 "parse1.y" /* yacc.c:1646  */
    {leftside = 1;}
#line 3202 "parse1.c" /* yacc.c:1646  */
    break;

  case 214:
#line 644 "parse1.y" /* yacc.c:1646  */
    {linterm((yyvsp[0].qp), lastok, pstate, leftside); pstate = 0;}
#line 3208 "parse1.c" /* yacc.c:1646  */
    break;

  case 215:
#line 646 "parse1.y" /* yacc.c:1646  */
    {delete((yyvsp[-1].qp));
		linterm((yyvsp[0].qp), lastok, pstate, -leftside); pstate = 0;}
#line 3215 "parse1.c" /* yacc.c:1646  */
    break;

  case 216:
#line 649 "parse1.y" /* yacc.c:1646  */
    {delete((yyvsp[-1].qp));
		linterm((yyvsp[0].qp), lastok, pstate, leftside); pstate = 0;}
#line 3222 "parse1.c" /* yacc.c:1646  */
    break;

  case 217:
#line 652 "parse1.y" /* yacc.c:1646  */
    {delete((yyvsp[-1].qp));
		linterm((yyvsp[0].qp), lastok, pstate, -leftside); pstate = 0;}
#line 3229 "parse1.c" /* yacc.c:1646  */
    break;

  case 218:
#line 656 "parse1.y" /* yacc.c:1646  */
    { if (tstate == 1) {
			lin_state_term((yyvsp[0].qp), lastok);
		  }
		  tstate = 0;
		}
#line 3239 "parse1.c" /* yacc.c:1646  */
    break;

  case 219:
#line 662 "parse1.y" /* yacc.c:1646  */
    { if (tstate == 1) {
			lin_state_term((yyvsp[0].qp), lastok);
		  }
		  tstate = 0;
		}
#line 3249 "parse1.c" /* yacc.c:1646  */
    break;

  case 220:
#line 668 "parse1.y" /* yacc.c:1646  */
    { if (tstate) {
			diag("state ocurs in denominator", (char *)0);
		  }
		}
#line 3258 "parse1.c" /* yacc.c:1646  */
    break;

  case 223:
#line 675 "parse1.y" /* yacc.c:1646  */
    {if (tstate) diag("states not permitted in function calls",
			(char *)0);}
#line 3265 "parse1.c" /* yacc.c:1646  */
    break;

  case 224:
#line 677 "parse1.y" /* yacc.c:1646  */
    { lastok = (yyvsp[0].qp);
			if (tstate) diag("states not permitted between ",
				"parentheses");}
#line 3273 "parse1.c" /* yacc.c:1646  */
    break;

  case 225:
#line 681 "parse1.y" /* yacc.c:1646  */
    {diag("Some operators are not permitted in linear\n",
"expressions unless the terms containing them are\nenclosed in parentheses");}
#line 3280 "parse1.c" /* yacc.c:1646  */
    break;

  case 226:
#line 685 "parse1.y" /* yacc.c:1646  */
    { if (SYM((yyvsp[-1].qp))->subtype & EXTDEF2) { extdef2 = 1;}}
#line 3286 "parse1.c" /* yacc.c:1646  */
    break;

  case 227:
#line 687 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp); SYM((yyvsp[-4].qp))->usage |= FUNCT;
		 if (SYM((yyvsp[-4].qp))->subtype & EXTDEF2) { extdef2 = 0;}
		 if (SYM((yyvsp[-4].qp))->subtype & EXTDEF3) { add_reset_args((yyvsp[-3].qp));}
		 if (SYM((yyvsp[-4].qp))->subtype & EXTDEF4) { add_nrnthread_arg((yyvsp[-3].qp));}
		 if (SYM((yyvsp[-4].qp))->subtype & EXTDEF5) {
			if (!assert_threadsafe) {
fprintf(stderr, "Notice: %s is not thread safe\n", SYM((yyvsp[-4].qp))->name);
				vectorize = 0;
			}
		 }
#if VECTORIZE
		 vectorize_use_func((yyvsp[-4].qp),(yyvsp[-3].qp),(yyvsp[-1].qp),(yyvsp[0].qp),blocktype);
#endif
		}
#line 3305 "parse1.c" /* yacc.c:1646  */
    break;

  case 228:
#line 702 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 3311 "parse1.c" /* yacc.c:1646  */
    break;

  case 233:
#line 708 "parse1.y" /* yacc.c:1646  */
    {pushlocal(); SYM((yyvsp[0].qp)) = copylocal(SYM((yyvsp[0].qp)));
			SYM((yyvsp[0].qp))->subtype |= INTGER;}
#line 3318 "parse1.c" /* yacc.c:1646  */
    break;

  case 234:
#line 712 "parse1.y" /* yacc.c:1646  */
    { replacstr((yyvsp[-9].qp), "{int ");
		poplocal();
		Insertstr((yyvsp[-6].qp), ";for (");
		Insertstr((yyvsp[-6].qp), SYM((yyvsp[-8].qp))->name);
		Insertstr((yyvsp[-4].qp), ";");
		Insertstr((yyvsp[-4].qp), SYM((yyvsp[-8].qp))->name);
		replacstr((yyvsp[-4].qp), "<=");
		if ((yyvsp[-2].qp)) {
			Insertstr((yyvsp[-2].qp), ";");
			Insertstr((yyvsp[-2].qp), SYM((yyvsp[-8].qp))->name);
			replacstr((yyvsp[-2].qp), "+=");
		} else {
			Insertstr((yyvsp[-1].qp), ";");
			Insertstr((yyvsp[-1].qp), SYM((yyvsp[-8].qp))->name);
			Insertstr((yyvsp[-1].qp), "++");
		}
		Insertstr((yyvsp[-1].qp), ")");
		Insertstr((yyvsp[0].qp), "}");
		}
#line 3342 "parse1.c" /* yacc.c:1646  */
    break;

  case 235:
#line 731 "parse1.y" /* yacc.c:1646  */
    {
myerr("FROM intvar = intexpr TO intexpr BY intexpr { statements }");}
#line 3349 "parse1.c" /* yacc.c:1646  */
    break;

  case 236:
#line 734 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 3355 "parse1.c" /* yacc.c:1646  */
    break;

  case 238:
#line 737 "parse1.y" /* yacc.c:1646  */
    {pushlocal(); SYM((yyvsp[0].qp)) = copylocal(SYM((yyvsp[0].qp)));
			  SYM((yyvsp[0].qp))->subtype |= INTGER;
			  if (forallindex) {
diag("Nested FORALL statements not allowed", (char *)0);
			  }
			  forallindex = -1;
			}
#line 3367 "parse1.c" /* yacc.c:1646  */
    break;

  case 239:
#line 746 "parse1.y" /* yacc.c:1646  */
    { replacstr((yyvsp[-4].qp), "{int ");
		poplocal();
		if (forallindex == -1) {
diag("FORALL range is undefined since no arrays used", " within the statement");
		}
		Sprintf(buf, "; for (%s=0; %s<%d; %s++)", SYM((yyvsp[-3].qp))->name,
			SYM((yyvsp[-3].qp))->name, forallindex, SYM((yyvsp[-3].qp))->name);
		Insertstr((yyvsp[-1].qp), buf);
		Insertstr((yyvsp[0].qp), "}");
		}
#line 3382 "parse1.c" /* yacc.c:1646  */
    break;

  case 240:
#line 756 "parse1.y" /* yacc.c:1646  */
    {
myerr("FORALL intvar { statements }");}
#line 3389 "parse1.c" /* yacc.c:1646  */
    break;

  case 244:
#line 765 "parse1.y" /* yacc.c:1646  */
    {
#if VECTORIZE
			vectorize_if_else_stmt(blocktype);
#endif
		}
#line 3399 "parse1.c" /* yacc.c:1646  */
    break;

  case 246:
#line 773 "parse1.y" /* yacc.c:1646  */
    {
#if VECTORIZE
			vectorize_if_else_stmt(blocktype);
#endif
		}
#line 3409 "parse1.c" /* yacc.c:1646  */
    break;

  case 247:
#line 781 "parse1.y" /* yacc.c:1646  */
    {massagederiv((yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[-1].qp), (yyvsp[0].qp), sensused); sensused = 0;}
#line 3415 "parse1.c" /* yacc.c:1646  */
    break;

  case 248:
#line 783 "parse1.y" /* yacc.c:1646  */
    {init_linblk((yyvsp[-1].qp));}
#line 3421 "parse1.c" /* yacc.c:1646  */
    break;

  case 249:
#line 785 "parse1.y" /* yacc.c:1646  */
    {massage_linblk((yyvsp[-5].qp), (yyvsp[-4].qp), (yyvsp[-1].qp), (yyvsp[0].qp), sensused);
		lexcontext = 0; sensused = 0;
		}
#line 3429 "parse1.c" /* yacc.c:1646  */
    break;

  case 250:
#line 791 "parse1.y" /* yacc.c:1646  */
    {massagenonlin((yyvsp[-4].qp), (yyvsp[-3].qp), (yyvsp[-1].qp), (yyvsp[0].qp), sensused);
		lexcontext = 0; sensused = 0;
		}
#line 3437 "parse1.c" /* yacc.c:1646  */
    break;

  case 251:
#line 797 "parse1.y" /* yacc.c:1646  */
    {massagediscblk((yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[-1].qp), (yyvsp[0].qp));}
#line 3443 "parse1.c" /* yacc.c:1646  */
    break;

  case 252:
#line 801 "parse1.y" /* yacc.c:1646  */
    {massagepartial((yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[-1].qp), (yyvsp[0].qp));
		lexcontext = 0;
		}
#line 3451 "parse1.c" /* yacc.c:1646  */
    break;

  case 253:
#line 804 "parse1.y" /* yacc.c:1646  */
    {
diag("within the PARTIAL block must occur an equation with the syntax ---\n",
"~ V' = F*DEL2(V) + G\n"); }
#line 3459 "parse1.c" /* yacc.c:1646  */
    break;

  case 254:
#line 809 "parse1.y" /* yacc.c:1646  */
    {partial_eqn((yyvsp[-9].qp), (yyvsp[-7].qp), (yyvsp[-3].qp), (yyvsp[0].qp));}
#line 3465 "parse1.c" /* yacc.c:1646  */
    break;

  case 255:
#line 811 "parse1.y" /* yacc.c:1646  */
    {partial_bndry(0, (yyvsp[-5].qp), (yyvsp[-3].qp), (yyvsp[0].qp), lastok);}
#line 3471 "parse1.c" /* yacc.c:1646  */
    break;

  case 256:
#line 813 "parse1.y" /* yacc.c:1646  */
    {partial_bndry(2, (yyvsp[-5].qp), (yyvsp[-3].qp), (yyvsp[0].qp), lastok);}
#line 3477 "parse1.c" /* yacc.c:1646  */
    break;

  case 259:
#line 818 "parse1.y" /* yacc.c:1646  */
    {Item *b1, *b2;
		Symbol* s = SYM((yyvsp[-4].qp));
		s->varnum = argcnt_;
		b1 = insertstr((yyvsp[-1].qp)->next, "{\n");
		b2 = insertstr(b1->next, "}\n");

#define GLOBFUNCT 1
#if GLOBFUNCT && NMODL
		replacstr((yyvsp[-5].qp), "\ndouble");
#else
		replacstr((yyvsp[-5].qp), "\nstatic double");
#endif
		defarg((yyvsp[-3].qp), (yyvsp[-1].qp));
		movelist((yyvsp[-5].qp), b2, procfunc);
		if (SYM((yyvsp[-4].qp))->subtype & FUNCT) {
			diag(SYM((yyvsp[-4].qp))->name, " declared as FUNCTION twice");
		}
		SYM((yyvsp[-4].qp))->subtype |= FUNCT;
		SYM((yyvsp[-4].qp))->usage |= FUNCT;
#if HMODL || NMODL
		hocfunc(s, (yyvsp[-3].qp), (yyvsp[-1].qp));
#endif
		function_table(s, (yyvsp[-3].qp), (yyvsp[-1].qp), b1, b2);
		poplocal();
		}
#line 3507 "parse1.c" /* yacc.c:1646  */
    break;

  case 260:
#line 845 "parse1.y" /* yacc.c:1646  */
    {IGNORE(copylocal(SYM((yyvsp[-4].qp))));}
#line 3513 "parse1.c" /* yacc.c:1646  */
    break;

  case 261:
#line 849 "parse1.y" /* yacc.c:1646  */
    { Symbol *s = SYM((yyvsp[-7].qp));
		s->varnum = argcnt_;
		table_massage(table_list, (yyvsp[-8].qp), (yyvsp[-7].qp), (yyvsp[-5].lp)); freelist(&table_list);
#if GLOBFUNCT && NMODL
		replacstr((yyvsp[-8].qp), "\ndouble");
#else
		replacstr((yyvsp[-8].qp), "\nstatic double");
#endif
		defarg((yyvsp[-6].qp), (yyvsp[-4].qp));
		Sprintf(buf, "double _l%s;\n", s->name);
		Insertstr((yyvsp[-1].qp)->next, buf);
		Sprintf(buf, "\nreturn _l%s;\n", s->name);
		Insertstr((yyvsp[0].qp), buf);
		movelist((yyvsp[-8].qp), (yyvsp[0].qp), procfunc);
		if (SYM((yyvsp[-7].qp))->subtype & FUNCT) {
			diag(SYM((yyvsp[-7].qp))->name, " declared as FUNCTION twice");
		}
		SYM((yyvsp[-7].qp))->subtype |= FUNCT;
		SYM((yyvsp[-7].qp))->usage |= FUNCT;
#if HMODL || NMODL
		hocfunc(s, (yyvsp[-6].qp), (yyvsp[-4].qp));
#endif
		poplocal(); freelist(&(yyvsp[-5].lp));}
#line 3541 "parse1.c" /* yacc.c:1646  */
    break;

  case 262:
#line 873 "parse1.y" /* yacc.c:1646  */
    {pushlocal(); (yyval.lp) = LIST0; argcnt_ = 0;}
#line 3547 "parse1.c" /* yacc.c:1646  */
    break;

  case 263:
#line 874 "parse1.y" /* yacc.c:1646  */
    {pushlocal();}
#line 3553 "parse1.c" /* yacc.c:1646  */
    break;

  case 264:
#line 874 "parse1.y" /* yacc.c:1646  */
    {(yyval.lp) = (yyvsp[0].lp);}
#line 3559 "parse1.c" /* yacc.c:1646  */
    break;

  case 265:
#line 877 "parse1.y" /* yacc.c:1646  */
    {SYM((yyvsp[-1].qp)) = copylocal(SYM((yyvsp[-1].qp))); argcnt_ = 1;
		 (yyval.lp) = newlist(); Lappendsym((yyval.lp), SYM((yyvsp[-1].qp)));
		}
#line 3567 "parse1.c" /* yacc.c:1646  */
    break;

  case 266:
#line 881 "parse1.y" /* yacc.c:1646  */
    {SYM((yyvsp[-1].qp)) = copylocal(SYM((yyvsp[-1].qp))); Lappendsym((yyval.lp), SYM((yyvsp[-1].qp)));
		 ++argcnt_;
		}
#line 3575 "parse1.c" /* yacc.c:1646  */
    break;

  case 267:
#line 886 "parse1.y" /* yacc.c:1646  */
    {Symbol *s = SYM((yyvsp[-6].qp));
		s->u.i = 0; 	/* avoid objectcenter warning if solved */
		s->varnum = argcnt_; /* allow proper number of "double" in prototype */
		table_massage(table_list, (yyvsp[-7].qp), (yyvsp[-6].qp), (yyvsp[-4].lp)); freelist(&table_list);
		replacstr((yyvsp[-7].qp), "\nstatic int "); defarg((yyvsp[-5].qp), (yyvsp[-3].qp));
		Insertstr((yyvsp[0].qp), " return 0;");
		movelist((yyvsp[-7].qp), (yyvsp[0].qp), procfunc);
		if (SYM((yyvsp[-6].qp))->subtype & PROCED) {
			diag(SYM((yyvsp[-6].qp))->name, " declared as PROCEDURE twice");
		}
		SYM((yyvsp[-6].qp))->subtype |= PROCED;
		SYM((yyvsp[-6].qp))->usage |= FUNCT;
#if HMODL || NMODL
		hocfunc(s, (yyvsp[-5].qp), (yyvsp[-3].qp));
#endif
		poplocal(); freelist(&(yyvsp[-4].lp));}
#line 3596 "parse1.c" /* yacc.c:1646  */
    break;

  case 268:
#line 904 "parse1.y" /* yacc.c:1646  */
    {
			nr_argcnt_ = argcnt_;
			/* add flag arg */
			if ((yyvsp[-1].lp) == LIST0) {
diag("NET_RECEIVE block"," must have at least one argument");
			}
			Lappendsym((yyvsp[-1].lp), copylocal(install("flag", NAME)));
		}
#line 3609 "parse1.c" /* yacc.c:1646  */
    break;

  case 269:
#line 913 "parse1.y" /* yacc.c:1646  */
    {
		replacstr((yyvsp[-6].qp), "\nstatic void _net_receive");
		movelist((yyvsp[-6].qp), (yyvsp[0].qp), procfunc);
#if NMODL
		net_receive((yyvsp[-4].lp), (yyvsp[-5].qp), (yyvsp[-3].qp), (yyvsp[-1].qp), (yyvsp[0].qp));
#endif
		poplocal(); freelist(&(yyvsp[-4].lp));
		}
#line 3622 "parse1.c" /* yacc.c:1646  */
    break;

  case 270:
#line 921 "parse1.y" /* yacc.c:1646  */
    { myerr("syntax: NET_RECEIVE ( weight ) {stmtlist}");}
#line 3628 "parse1.c" /* yacc.c:1646  */
    break;

  case 271:
#line 924 "parse1.y" /* yacc.c:1646  */
    {
			check_block(NETRECEIVE, blocktype, "INITIAL");
#if NMODL
			net_init((yyvsp[-2].qp), (yyvsp[0].qp));
#endif
		}
#line 3639 "parse1.c" /* yacc.c:1646  */
    break;

  case 272:
#line 933 "parse1.y" /* yacc.c:1646  */
    { solvequeue((yyvsp[-1].qp), ITEM0, blocktype, (yyvsp[0].qp)); }
#line 3645 "parse1.c" /* yacc.c:1646  */
    break;

  case 273:
#line 935 "parse1.y" /* yacc.c:1646  */
    { solvequeue((yyvsp[-3].qp), (yyvsp[-1].qp), blocktype, (yyvsp[0].qp)); }
#line 3651 "parse1.c" /* yacc.c:1646  */
    break;

  case 274:
#line 936 "parse1.y" /* yacc.c:1646  */
    { myerr("Illegal SOLVE statement");}
#line 3657 "parse1.c" /* yacc.c:1646  */
    break;

  case 275:
#line 939 "parse1.y" /* yacc.c:1646  */
    { (yyval.qp) = ITEM0; }
#line 3663 "parse1.c" /* yacc.c:1646  */
    break;

  case 276:
#line 941 "parse1.y" /* yacc.c:1646  */
    { (yyval.qp) = (yyvsp[0].qp); }
#line 3669 "parse1.c" /* yacc.c:1646  */
    break;

  case 277:
#line 944 "parse1.y" /* yacc.c:1646  */
    { if (solveforlist) {
			freelist(&solveforlist);
		  }
		}
#line 3678 "parse1.c" /* yacc.c:1646  */
    break;

  case 279:
#line 951 "parse1.y" /* yacc.c:1646  */
    { solveforlist = newlist(); Lappendsym(solveforlist, SYM((yyvsp[0].qp)));
		  delete((yyvsp[-1].qp)); delete((yyvsp[0].qp));
		}
#line 3686 "parse1.c" /* yacc.c:1646  */
    break;

  case 280:
#line 955 "parse1.y" /* yacc.c:1646  */
    { Lappendsym(solveforlist, SYM((yyvsp[0].qp))); delete((yyvsp[-1].qp)); delete((yyvsp[0].qp));}
#line 3692 "parse1.c" /* yacc.c:1646  */
    break;

  case 281:
#line 956 "parse1.y" /* yacc.c:1646  */
    { myerr("Syntax: SOLVEFOR name, name, ...");}
#line 3698 "parse1.c" /* yacc.c:1646  */
    break;

  case 282:
#line 960 "parse1.y" /* yacc.c:1646  */
    {brkpnt_exists = 1; movelist((yyvsp[-1].qp), (yyvsp[0].qp), modelfunc);}
#line 3704 "parse1.c" /* yacc.c:1646  */
    break;

  case 283:
#line 963 "parse1.y" /* yacc.c:1646  */
    {movelist((yyvsp[-1].qp), (yyvsp[0].qp), termfunc);}
#line 3710 "parse1.c" /* yacc.c:1646  */
    break;

  case 284:
#line 966 "parse1.y" /* yacc.c:1646  */
    {bablk(blocktype, BREAKPOINT, (yyvsp[-1].qp), (yyvsp[0].qp));}
#line 3716 "parse1.c" /* yacc.c:1646  */
    break;

  case 285:
#line 968 "parse1.y" /* yacc.c:1646  */
    {bablk(blocktype, SOLVE, (yyvsp[-1].qp), (yyvsp[0].qp));}
#line 3722 "parse1.c" /* yacc.c:1646  */
    break;

  case 286:
#line 970 "parse1.y" /* yacc.c:1646  */
    {bablk(blocktype, INITIAL1, (yyvsp[-1].qp), (yyvsp[0].qp));}
#line 3728 "parse1.c" /* yacc.c:1646  */
    break;

  case 287:
#line 972 "parse1.y" /* yacc.c:1646  */
    {bablk(blocktype, STEP, (yyvsp[-1].qp), (yyvsp[0].qp));}
#line 3734 "parse1.c" /* yacc.c:1646  */
    break;

  case 288:
#line 973 "parse1.y" /* yacc.c:1646  */
    {myerr("[BEFORE AFTER] [BREAKPOINT SOLVE INITIAL STEP] { stmt }");}
#line 3740 "parse1.c" /* yacc.c:1646  */
    break;

  case 289:
#line 976 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp); delete((yyvsp[-1].qp));}
#line 3746 "parse1.c" /* yacc.c:1646  */
    break;

  case 290:
#line 978 "parse1.y" /* yacc.c:1646  */
    {delete((yyvsp[-1].qp));}
#line 3752 "parse1.c" /* yacc.c:1646  */
    break;

  case 291:
#line 979 "parse1.y" /* yacc.c:1646  */
    {myerr("WATCH (expr > expr) flag");}
#line 3758 "parse1.c" /* yacc.c:1646  */
    break;

  case 292:
#line 982 "parse1.y" /* yacc.c:1646  */
    {watchstmt((yyvsp[-5].qp), (yyvsp[-3].qp), (yyvsp[-1].qp), (yyvsp[0].qp), blocktype); (yyval.qp)=(yyvsp[0].qp);}
#line 3764 "parse1.c" /* yacc.c:1646  */
    break;

  case 295:
#line 987 "parse1.y" /* yacc.c:1646  */
    {
			if (blocktype != NETRECEIVE) {
				diag("\"FOR_NETCONS\" statement only allowed in NET_RECEIVE block", (char*)0);
			}
			if (argcnt_ != nr_argcnt_) {
				diag("FOR_NETCONS and NET_RECEIVE do not have same number of arguments", (char*)0);
			}
		}
#line 3777 "parse1.c" /* yacc.c:1646  */
    break;

  case 296:
#line 996 "parse1.y" /* yacc.c:1646  */
    {fornetcon((yyvsp[-6].qp), (yyvsp[-5].qp), (yyvsp[-4].lp), (yyvsp[-3].qp), (yyvsp[-1].qp), (yyvsp[0].qp)); (yyval.qp) = (yyvsp[0].qp); }
#line 3783 "parse1.c" /* yacc.c:1646  */
    break;

  case 297:
#line 997 "parse1.y" /* yacc.c:1646  */
    { myerr("syntax: FOR_NETCONS(args,like,netreceive) { stmtlist}");}
#line 3789 "parse1.c" /* yacc.c:1646  */
    break;

  case 301:
#line 1002 "parse1.y" /* yacc.c:1646  */
    {lastok = (yyvsp[0].qp);}
#line 3795 "parse1.c" /* yacc.c:1646  */
    break;

  case 306:
#line 1008 "parse1.y" /* yacc.c:1646  */
    { (yyval.qp) = insertstr((yyvsp[-2].qp), "pow("); replacstr((yyvsp[-1].qp), ",");
			lastok = insertstr(lastok->next, ")"); }
#line 3802 "parse1.c" /* yacc.c:1646  */
    break;

  case 308:
#line 1011 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal expression");}
#line 3808 "parse1.c" /* yacc.c:1646  */
    break;

  case 309:
#line 1016 "parse1.y" /* yacc.c:1646  */
    { sensused = 1;
		  delete((yyvsp[-1].qp));
		}
#line 3816 "parse1.c" /* yacc.c:1646  */
    break;

  case 310:
#line 1019 "parse1.y" /* yacc.c:1646  */
    {myerr("syntax is SENS var1, var2, var3, etc");}
#line 3822 "parse1.c" /* yacc.c:1646  */
    break;

  case 311:
#line 1023 "parse1.y" /* yacc.c:1646  */
    { sensparm((yyvsp[0].qp)); delete((yyvsp[0].qp));}
#line 3828 "parse1.c" /* yacc.c:1646  */
    break;

  case 312:
#line 1025 "parse1.y" /* yacc.c:1646  */
    { sensparm((yyvsp[0].qp)); deltokens((yyvsp[-1].qp), (yyvsp[0].qp));}
#line 3834 "parse1.c" /* yacc.c:1646  */
    break;

  case 313:
#line 1028 "parse1.y" /* yacc.c:1646  */
    {extdef2 = 0; }
#line 3840 "parse1.c" /* yacc.c:1646  */
    break;

  case 314:
#line 1029 "parse1.y" /* yacc.c:1646  */
    {/* react originally designed for reactions and is unchanged*/
		extdef2 = 0;
		massageconserve((yyvsp[-4].qp), (yyvsp[-1].qp), lastok);}
#line 3848 "parse1.c" /* yacc.c:1646  */
    break;

  case 315:
#line 1032 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal CONSERVE syntax");}
#line 3854 "parse1.c" /* yacc.c:1646  */
    break;

  case 316:
#line 1035 "parse1.y" /* yacc.c:1646  */
    {pushlocal(); SYM((yyvsp[-1].qp)) = copylocal(SYM((yyvsp[-1].qp)));}
#line 3860 "parse1.c" /* yacc.c:1646  */
    break;

  case 317:
#line 1037 "parse1.y" /* yacc.c:1646  */
    {massagecompart((yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[0].qp), SYM((yyvsp[-6].qp))); poplocal();}
#line 3866 "parse1.c" /* yacc.c:1646  */
    break;

  case 318:
#line 1039 "parse1.y" /* yacc.c:1646  */
    {massagecompart((yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[0].qp), SYM0);}
#line 3872 "parse1.c" /* yacc.c:1646  */
    break;

  case 319:
#line 1042 "parse1.y" /* yacc.c:1646  */
    {pushlocal(); SYM((yyvsp[-1].qp)) = copylocal(SYM((yyvsp[-1].qp)));}
#line 3878 "parse1.c" /* yacc.c:1646  */
    break;

  case 320:
#line 1044 "parse1.y" /* yacc.c:1646  */
    {massageldifus((yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[0].qp), SYM((yyvsp[-6].qp))); poplocal();}
#line 3884 "parse1.c" /* yacc.c:1646  */
    break;

  case 321:
#line 1046 "parse1.y" /* yacc.c:1646  */
    {massageldifus((yyvsp[-3].qp), (yyvsp[-2].qp), (yyvsp[0].qp), SYM0);}
#line 3890 "parse1.c" /* yacc.c:1646  */
    break;

  case 324:
#line 1054 "parse1.y" /* yacc.c:1646  */
    {massagekinetic((yyvsp[-4].qp), (yyvsp[-3].qp), (yyvsp[-1].qp), (yyvsp[0].qp), sensused);
		lexcontext = 0; sensused = 0;
		}
#line 3898 "parse1.c" /* yacc.c:1646  */
    break;

  case 325:
#line 1058 "parse1.y" /* yacc.c:1646  */
    {leftreact();}
#line 3904 "parse1.c" /* yacc.c:1646  */
    break;

  case 326:
#line 1059 "parse1.y" /* yacc.c:1646  */
    {massagereaction((yyvsp[-9].qp),(yyvsp[-7].qp),(yyvsp[-4].qp),(yyvsp[-2].qp),(yyvsp[0].qp));}
#line 3910 "parse1.c" /* yacc.c:1646  */
    break;

  case 327:
#line 1061 "parse1.y" /* yacc.c:1646  */
    {flux((yyvsp[-6].qp), (yyvsp[-4].qp), (yyvsp[0].qp));}
#line 3916 "parse1.c" /* yacc.c:1646  */
    break;

  case 328:
#line 1063 "parse1.y" /* yacc.c:1646  */
    {flux((yyvsp[-6].qp), (yyvsp[-4].qp), (yyvsp[0].qp));}
#line 3922 "parse1.c" /* yacc.c:1646  */
    break;

  case 329:
#line 1064 "parse1.y" /* yacc.c:1646  */
    {myerr("Illegal reaction syntax");}
#line 3928 "parse1.c" /* yacc.c:1646  */
    break;

  case 330:
#line 1066 "parse1.y" /* yacc.c:1646  */
    {reactname((yyvsp[0].qp), lastok, ITEM0);}
#line 3934 "parse1.c" /* yacc.c:1646  */
    break;

  case 331:
#line 1067 "parse1.y" /* yacc.c:1646  */
    {reactname((yyvsp[0].qp), lastok, (yyvsp[-1].qp));}
#line 3940 "parse1.c" /* yacc.c:1646  */
    break;

  case 332:
#line 1068 "parse1.y" /* yacc.c:1646  */
    {reactname((yyvsp[0].qp), lastok, ITEM0);}
#line 3946 "parse1.c" /* yacc.c:1646  */
    break;

  case 333:
#line 1069 "parse1.y" /* yacc.c:1646  */
    {reactname((yyvsp[0].qp), lastok, (yyvsp[-1].qp));}
#line 3952 "parse1.c" /* yacc.c:1646  */
    break;

  case 334:
#line 1072 "parse1.y" /* yacc.c:1646  */
    {lag_stmt((yyvsp[-3].qp), blocktype);}
#line 3958 "parse1.c" /* yacc.c:1646  */
    break;

  case 335:
#line 1073 "parse1.y" /* yacc.c:1646  */
    {myerr("Lag syntax is: LAG name BY const");}
#line 3964 "parse1.c" /* yacc.c:1646  */
    break;

  case 336:
#line 1075 "parse1.y" /* yacc.c:1646  */
    {queue_stmt((yyvsp[-1].qp), (yyvsp[0].qp));}
#line 3970 "parse1.c" /* yacc.c:1646  */
    break;

  case 337:
#line 1076 "parse1.y" /* yacc.c:1646  */
    {queue_stmt((yyvsp[-1].qp), (yyvsp[0].qp));}
#line 3976 "parse1.c" /* yacc.c:1646  */
    break;

  case 338:
#line 1078 "parse1.y" /* yacc.c:1646  */
    {checkmatch(blocktype);}
#line 3982 "parse1.c" /* yacc.c:1646  */
    break;

  case 339:
#line 1079 "parse1.y" /* yacc.c:1646  */
    {deltokens((yyvsp[-4].qp), (yyvsp[0].qp));}
#line 3988 "parse1.c" /* yacc.c:1646  */
    break;

  case 342:
#line 1085 "parse1.y" /* yacc.c:1646  */
    {matchinitial((yyvsp[0].qp));}
#line 3994 "parse1.c" /* yacc.c:1646  */
    break;

  case 343:
#line 1087 "parse1.y" /* yacc.c:1646  */
    { matchbound((yyvsp[-5].qp), (yyvsp[-4].qp), (yyvsp[-2].qp), (yyvsp[0].qp), lastok, matchindex);
		  if (matchindex) {
			poplocal();
			matchindex = SYM0;
		  }
		}
#line 4005 "parse1.c" /* yacc.c:1646  */
    break;

  case 344:
#line 1094 "parse1.y" /* yacc.c:1646  */
    {myerr("MATCH syntax is state0 or state(expr)=expr or\
state[i](expr(i)) = expr(i)");}
#line 4012 "parse1.c" /* yacc.c:1646  */
    break;

  case 345:
#line 1098 "parse1.y" /* yacc.c:1646  */
    {matchindex = SYM0;}
#line 4018 "parse1.c" /* yacc.c:1646  */
    break;

  case 346:
#line 1100 "parse1.y" /* yacc.c:1646  */
    { pushlocal();
		  matchindex = copylocal(SYM((yyvsp[-1].qp)));
		}
#line 4026 "parse1.c" /* yacc.c:1646  */
    break;

  case 347:
#line 1105 "parse1.y" /* yacc.c:1646  */
    { deltokens((yyvsp[-3].qp),(yyvsp[0].qp));}
#line 4032 "parse1.c" /* yacc.c:1646  */
    break;

  case 348:
#line 1108 "parse1.y" /* yacc.c:1646  */
    {modl_units();}
#line 4038 "parse1.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1113 "parse1.y" /* yacc.c:1646  */
    {install_units((yyvsp[-2].str), (yyvsp[0].str));}
#line 4044 "parse1.c" /* yacc.c:1646  */
    break;

  case 352:
#line 1114 "parse1.y" /* yacc.c:1646  */
    {myerr("Unit definition syntax: (units) = (units)");}
#line 4050 "parse1.c" /* yacc.c:1646  */
    break;

  case 353:
#line 1117 "parse1.y" /* yacc.c:1646  */
    { SYM((yyvsp[-3].qp))->subtype |= nmodlCONST;
		  Sprintf(buf, "static double %s = %s;\n", SYM((yyvsp[-3].qp))->name,
			STR((yyvsp[-1].qp)));
		  Lappendstr(firstlist, buf);
		}
#line 4060 "parse1.c" /* yacc.c:1646  */
    break;

  case 354:
#line 1123 "parse1.y" /* yacc.c:1646  */
    {Item *q; double d, unit_mag();
		    Unit_push((yyvsp[-1].str));
			Unit_push((yyvsp[0].str)); unit_div();
		    SYM((yyvsp[-3].qp))->subtype |= nmodlCONST;
#if defined(LegacyFR) && LegacyFR == 1
		    Sprintf(buf, "static double %s = %g;\n", SYM((yyvsp[-3].qp))->name,
#else
		    Sprintf(buf, "static double %s = %.12g;\n", SYM((yyvsp[-3].qp))->name,
#endif
			unit_mag());
		    Lappendstr(firstlist, buf);
		    unit_pop();
		}
#line 4078 "parse1.c" /* yacc.c:1646  */
    break;

  case 355:
#line 1137 "parse1.y" /* yacc.c:1646  */
    { double unit_mag();
		    Unit_push((yyvsp[-3].str)); Unit_push((yyvsp[0].str)); unit_div();
		    SYM((yyvsp[-5].qp))->subtype |= nmodlCONST;
		    Sprintf(buf, "static double %s = %g;\n", SYM((yyvsp[-5].qp))->name,
			unit_mag());
		    Lappendstr(firstlist, buf);
		    unit_pop();
		}
#line 4091 "parse1.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1145 "parse1.y" /* yacc.c:1646  */
    {myerr("Unit factor syntax: examples:\n\
foot2inch = (foot) -> (inch)\n\
F = 96520 (coulomb)\n\
R = (k-mole) (joule/degC)");
		}
#line 4101 "parse1.c" /* yacc.c:1646  */
    break;

  case 359:
#line 1155 "parse1.y" /* yacc.c:1646  */
    { SYM((yyvsp[-3].qp))->subtype |= nmodlCONST;
		  Sprintf(buf, "static double %s = %s;\n", SYM((yyvsp[-3].qp))->name,
			STR((yyvsp[-1].qp)));
		  Lappendstr(firstlist, buf);
		}
#line 4111 "parse1.c" /* yacc.c:1646  */
    break;

  case 360:
#line 1162 "parse1.y" /* yacc.c:1646  */
    { Item *q;
		  if (table_list) {
			diag("only one TABLE per function", (char *)0);
		  }
		  table_list = newlist();
		  Lappendlst(table_list, (yyvsp[-7].lp));
		  q = lappendlst(table_list, newlist());
		  movelist((yyvsp[-5].qp)->next, (yyvsp[-3].qp)->prev, LST(q));
		  q = lappendlst(table_list, newlist());
		  movelist((yyvsp[-3].qp)->next, (yyvsp[-1].qp)->prev, LST(q));
		  Lappendstr(table_list, STR((yyvsp[0].qp)));
		  Lappendlst(table_list, (yyvsp[-6].lp));
		  deltokens((yyvsp[-8].qp), (yyvsp[0].qp));
		}
#line 4130 "parse1.c" /* yacc.c:1646  */
    break;

  case 361:
#line 1176 "parse1.y" /* yacc.c:1646  */
    { myerr("syntax: TABLE [list] [DEPEND list] FROM expr TO expr WITH integer");}
#line 4136 "parse1.c" /* yacc.c:1646  */
    break;

  case 362:
#line 1179 "parse1.y" /* yacc.c:1646  */
    {(yyval.lp) = LIST0;}
#line 4142 "parse1.c" /* yacc.c:1646  */
    break;

  case 364:
#line 1183 "parse1.y" /* yacc.c:1646  */
    {(yyval.lp) = newlist(); Lappendsym((yyval.lp), SYM((yyvsp[0].qp)));}
#line 4148 "parse1.c" /* yacc.c:1646  */
    break;

  case 365:
#line 1185 "parse1.y" /* yacc.c:1646  */
    { Lappendsym((yyvsp[-2].lp), SYM((yyvsp[0].qp)));}
#line 4154 "parse1.c" /* yacc.c:1646  */
    break;

  case 366:
#line 1188 "parse1.y" /* yacc.c:1646  */
    {(yyval.lp) = LIST0;}
#line 4160 "parse1.c" /* yacc.c:1646  */
    break;

  case 367:
#line 1190 "parse1.y" /* yacc.c:1646  */
    {(yyval.lp) = (yyvsp[0].lp);}
#line 4166 "parse1.c" /* yacc.c:1646  */
    break;

  case 368:
#line 1193 "parse1.y" /* yacc.c:1646  */
    { deltokens((yyvsp[-3].qp),(yyvsp[0].qp));}
#line 4172 "parse1.c" /* yacc.c:1646  */
    break;

  case 370:
#line 1197 "parse1.y" /* yacc.c:1646  */
    { nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}
#line 4178 "parse1.c" /* yacc.c:1646  */
    break;

  case 373:
#line 1201 "parse1.y" /* yacc.c:1646  */
    { nrn_list((yyvsp[-1].qp),(yyvsp[0].qp));}
#line 4184 "parse1.c" /* yacc.c:1646  */
    break;

  case 374:
#line 1203 "parse1.y" /* yacc.c:1646  */
    { nrn_list((yyvsp[-1].qp),(yyvsp[0].qp));}
#line 4190 "parse1.c" /* yacc.c:1646  */
    break;

  case 375:
#line 1205 "parse1.y" /* yacc.c:1646  */
    { nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}
#line 4196 "parse1.c" /* yacc.c:1646  */
    break;

  case 376:
#line 1207 "parse1.y" /* yacc.c:1646  */
    { nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}
#line 4202 "parse1.c" /* yacc.c:1646  */
    break;

  case 377:
#line 1209 "parse1.y" /* yacc.c:1646  */
    { nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}
#line 4208 "parse1.c" /* yacc.c:1646  */
    break;

  case 378:
#line 1211 "parse1.y" /* yacc.c:1646  */
    { nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}
#line 4214 "parse1.c" /* yacc.c:1646  */
    break;

  case 379:
#line 1213 "parse1.y" /* yacc.c:1646  */
    { nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}
#line 4220 "parse1.c" /* yacc.c:1646  */
    break;

  case 380:
#line 1215 "parse1.y" /* yacc.c:1646  */
    { nrn_list((yyvsp[-1].qp), (yyvsp[0].qp));}
#line 4226 "parse1.c" /* yacc.c:1646  */
    break;

  case 381:
#line 1217 "parse1.y" /* yacc.c:1646  */
    { threadsafe_seen((yyvsp[-1].qp), (yyvsp[0].qp)); }
#line 4232 "parse1.c" /* yacc.c:1646  */
    break;

  case 382:
#line 1220 "parse1.y" /* yacc.c:1646  */
    {nrn_use((yyvsp[-4].qp), (yyvsp[-2].qp), ITEM0, (yyvsp[-1].qp));}
#line 4238 "parse1.c" /* yacc.c:1646  */
    break;

  case 383:
#line 1222 "parse1.y" /* yacc.c:1646  */
    {nrn_use((yyvsp[-4].qp), ITEM0, (yyvsp[-2].qp), (yyvsp[-1].qp));}
#line 4244 "parse1.c" /* yacc.c:1646  */
    break;

  case 384:
#line 1224 "parse1.y" /* yacc.c:1646  */
    {nrn_use((yyvsp[-6].qp), (yyvsp[-4].qp), (yyvsp[-2].qp), (yyvsp[-1].qp));}
#line 4250 "parse1.c" /* yacc.c:1646  */
    break;

  case 385:
#line 1226 "parse1.y" /* yacc.c:1646  */
    {myerr("syntax is: USEION ion READ list WRITE list REPRESENTS curie");}
#line 4256 "parse1.c" /* yacc.c:1646  */
    break;

  case 386:
#line 1228 "parse1.y" /* yacc.c:1646  */
    { (yyval.qp) = NULL; }
#line 4262 "parse1.c" /* yacc.c:1646  */
    break;

  case 387:
#line 1230 "parse1.y" /* yacc.c:1646  */
    { (yyval.qp) = (yyvsp[0].qp); }
#line 4268 "parse1.c" /* yacc.c:1646  */
    break;

  case 389:
#line 1233 "parse1.y" /* yacc.c:1646  */
    { delete((yyvsp[-1].qp)); (yyval.qp) = (yyvsp[0].qp);}
#line 4274 "parse1.c" /* yacc.c:1646  */
    break;

  case 390:
#line 1235 "parse1.y" /* yacc.c:1646  */
    {myerr("syntax is: keyword name , name, ..., name");}
#line 4280 "parse1.c" /* yacc.c:1646  */
    break;

  case 391:
#line 1238 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = NULL;}
#line 4286 "parse1.c" /* yacc.c:1646  */
    break;

  case 393:
#line 1242 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = ITEM0;}
#line 4292 "parse1.c" /* yacc.c:1646  */
    break;

  case 394:
#line 1244 "parse1.y" /* yacc.c:1646  */
    {(yyval.qp) = (yyvsp[0].qp);}
#line 4298 "parse1.c" /* yacc.c:1646  */
    break;

  case 395:
#line 1247 "parse1.y" /* yacc.c:1646  */
    { Sprintf(buf, "-%s", STR((yyvsp[0].qp)));
		 STR((yyvsp[0].qp)) = stralloc(buf, STR((yyvsp[0].qp))); (yyval.qp) = (yyvsp[0].qp);
		delete((yyvsp[-1].qp)); lastok = (yyvsp[0].qp);
		}
#line 4307 "parse1.c" /* yacc.c:1646  */
    break;


#line 4311 "parse1.c" /* yacc.c:1646  */
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
#line 1252 "parse1.y" /* yacc.c:1906  */

void yyerror(s)	/* called for yacc syntax error */
	char *s;
{
	Fprintf(stderr, "%s:\n ", s);
}


#if !NMODL
void nrn_list(q1, q2)
	Item *q1, *q2;
{
	/*ARGSUSED*/
}
void nrn_use(q1, q2, q3, q4)
	Item *q1, *q2, *q3, *q4;
{
	/*ARGSUSED*/
}
#endif
