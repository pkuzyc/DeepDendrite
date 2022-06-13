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
#line 2 "parse.y" /* yacc.c:339  */


#include <../../nrnconf.h>
/* changes as of 2-jan-89 */
/*  version 7.2.1 2-jan-89 short form of the for statement */

#if AIX
#pragma alloca
#endif

#include "hoc.h"
#include "ocmisc.h"
#include "hocparse.h"
#include "code.h"
#include "equation.h"
#include "nrnfilewrap.h"

void* nrn_parsing_pysec_;

#if LINT
Inst *inlint;
#define code	inlint = Code
#else
#define code	Code
#endif

#define paction(arg) fprintf(stderr, "%s\n", arg)

/* maintain a list of ierr addresses so we can clear them */
#define HOCERRSIZE 20
static int** hoc_err;
static int hoc_errp;
static int localcnt;

static void clean_err(void) {
	int i;
	for (i=0; i < hoc_errp; ++i) {
		*hoc_err[i] = 0;
	}
	hoc_errp = 0;
}

static void pusherr(int* ip) {
	if (!hoc_err) {
		hoc_err = (int**)ecalloc(HOCERRSIZE, sizeof(int*));
		hoc_errp = 0;
	}
	if (hoc_errp >= HOCERRSIZE) {
		clean_err();
		hoc_execerror("error stack full", (char*)0);
	}
	hoc_err[hoc_errp++] = ip;
}

static void yyerror(const char* s);

#if YYBISON
#define myerr(arg) static int ierr=0;\
if (!(ierr++)){pusherr(&ierr);yyerror(arg);} --yyssp; --yyvsp; YYERROR
#else
#define myerr(arg) static int ierr=0;\
if (!(ierr++)){pusherr(&ierr);yyerror(arg);} YYERROR
#endif

#define code2(c1,c2)	code(c1); codein(c2)
#define code3(c1,c2,c3)	code(c1); codesym(c2); code(c3)
#define relative(ip1,ip2,offset)	((ip1-ip2) - offset)
#define CHECK 1	/* check syntactically the consistency of arrays */
#define NOCHECK 0 /* don't check syntactically. For object components */
#define PN pushi(NUMBER)	/* for type checking. expressions are usually numbers */
#define TPD hoc_ob_check(NUMBER);
#define TPDYNAM hoc_ob_check(0);

static Inst *prog_error;			/* needed for stmtlist loc if error */
static int ntab;			/* auto indentation */

static Inst* argrefcode(Pfrv pfrv, int i, int j);
static Inst* argcode(Pfrv pfrv, int i);
static void hoc_opasgn_invalid(int op);
 

#line 148 "parse.c" /* yacc.c:339  */

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
#line 84 "parse.y" /* yacc.c:355  */
				/* stack type */
	Symbol	*sym;			/* symbol table pointer */
	Inst	*inst;			/* machine instruction */
	int	narg;			/* number of arguments */
	void*	ptr;

#line 371 "parse.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 388 "parse.c" /* yacc.c:358  */

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2859

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  108
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  99
/* YYNRULES -- Number of rules.  */
#define YYNRULES  284
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  553

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   343

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      96,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    92,   107,     2,
      97,    98,    90,    88,   102,    89,   101,    91,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   105,   106,
       2,    79,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    99,     2,   100,    95,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   103,     2,   104,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    80,    81,    82,    83,    84,    85,
      86,    87,    93,    94
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   141,   141,   142,   143,   144,   146,   148,   150,   152,
     154,   158,   164,   164,   165,   175,   179,   181,   184,   188,
     194,   200,   204,   204,   206,   208,   210,   212,   215,   220,
     223,   224,   224,   234,   237,   238,   241,   242,   243,   244,
     245,   247,   250,   252,   254,   256,   259,   269,   271,   274,
     276,   281,   283,   288,   290,   293,   294,   296,   300,   302,
     306,   308,   310,   323,   325,   326,   328,   330,   331,   333,
     335,   337,   346,   350,   356,   358,   360,   362,   364,   366,
     371,   382,   386,   381,   393,   397,   392,   405,   409,   413,
     419,   418,   424,   427,   428,   430,   432,   434,   434,   437,
     439,   441,   451,   453,   455,   458,   462,   467,   466,   475,
     478,   474,   484,   483,   491,   490,   497,   496,   503,   502,
     510,   509,   521,   526,   526,   532,   532,   534,   539,   547,
     551,   552,   556,   557,   557,   567,   569,   571,   573,   576,
     584,   586,   588,   588,   591,   593,   595,   597,   598,   604,
     606,   610,   612,   615,   617,   620,   623,   626,   630,   633,
     636,   637,   648,   649,   654,   656,   658,   660,   662,   665,
     667,   671,   674,   675,   677,   679,   681,   683,   685,   687,
     689,   691,   693,   695,   697,   699,   701,   703,   705,   707,
     709,   711,   713,   715,   717,   720,   721,   722,   723,   725,
     728,   733,   736,   738,   741,   744,   747,   746,   753,   761,
     796,   803,   804,   808,   810,   812,   814,   817,   822,   821,
     828,   827,   834,   833,   840,   839,   846,   852,   853,   854,
     855,   857,   857,   864,   865,   867,   869,   872,   874,   876,
     877,   882,   885,   884,   890,   892,   895,   896,   898,   897,
     902,   902,   907,   907,   912,   913,   915,   922,   934,   935,
     937,   947,   958,   966,   966,   966,   966,   966,   966,   966,
     966,   967,   967,   967,   967,   967,   967,   967,   968,   968,
     968,   968,   968,   968,   969
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "EQNEQ", "NUMBER", "STRING", "PRINT",
  "parseDELETE", "VAR", "BLTIN", "UNDEF", "WHILE", "IF", "ELSE", "FOR",
  "FUNCTION", "PROCEDURE", "RETURN", "FUNC", "parsePROC", "HOCOBJFUNC",
  "READ", "parseDOUBLE", "DEBUG", "EDIT", "FUN_BLTIN", "DEPENDENT",
  "EQUATION", "LOCAL", "HOCOBJFUNCTION", "BREAK", "CONTINUE", "AUTO",
  "STRDEF", "STOPSTMT", "CSTRING", "PARALLEL", "HELP", "ITERATOR",
  "ITERKEYWORD", "ITERSTMT", "STRINGFUNC", "OBJECTFUNC", "LOCALOBJ",
  "AUTOOBJ", "ARG", "NUMZERO", "ARGREF", "INTERNALSECTIONNAME", "PYSEC",
  "PYSECNAME", "PYSECOBJ", "SECTIONKEYWORD", "SECTION", "CONNECTKEYWORD",
  "ACCESSKEYWORD", "RANGEVAR", "MECHANISM", "INSERTKEYWORD", "FORALL",
  "NRNPNTVAR", "FORSEC", "IFSEC", "UNINSERTKEYWORD", "SETPOINTERKEYWORD",
  "SECTIONREF", "BEGINTEMPLATE", "ENDTEMPLATE", "NEW", "OBJECTVAR",
  "TEMPLATE", "OBJVARDECL", "PUBLICDECL", "EXTERNALDECL", "OBFUNCTION",
  "STRFUNCTION", "OBJECTARG", "STRINGARG", "ROP", "'='", "OR", "AND", "GT",
  "GE", "LT", "LE", "EQ", "NE", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UNARYMINUS", "NOT", "'^'", "'\\n'", "'('", "')'", "'['", "']'", "'.'",
  "','", "'{'", "'}'", "':'", "';'", "'&'", "$accept", "list", "$@1",
  "asgn", "object", "$@2", "ob", "ob1", "$@3", "func_or_range_array_case",
  "template", "objvarlist", "objvarlst1", "obvarname", "publiclist",
  "externallist", "strnasgn", "string1", "string2", "strlist", "string",
  "stmt", "@4", "@5", "@6", "@7", "$@8", "$@9", "@10", "$@11", "@12",
  "@13", "@14", "@15", "@16", "@17", "iterator", "$@18", "section", "$@19",
  "pysec", "pysec1", "section_or_ob", "@20", "sectiondecl", "sectionname",
  "rangevar", "rangevar1", "$@21", "pointer", "for_init", "for_st",
  "for_cond", "for_inc", "cond", "while", "if", "begin", "end", "stmtlist",
  "expr", "function", "doublelist", "newarray", "numdimen", "newname",
  "varname", "$@22", "wholearray", "argrefdim", "array", "prlist",
  "delsym", "defn", "$@23", "$@24", "$@25", "$@26", "procname", "procstmt",
  "$@27", "arglist", "arglist1", "arglist2", "eqn_list", "$@28",
  "dep_list", "equation", "$@29", "lhs", "$@30", "rhs", "$@31", "local",
  "local1", "localobj", "local2", "ckvar", "anyname", YY_NULLPTR
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
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,    61,
     334,   335,   336,   337,   338,   339,   340,   341,    43,    45,
      42,    47,    37,   342,   343,    94,    10,    40,    41,    91,
      93,    46,    44,   123,   125,    58,    59,    38
};
# endif

#define YYPACT_NINF -439

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-439)))

#define YYTABLE_NINF -161

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -439,  1130,  -439,  -439,  -439,  -439,  2155,    23,  -439,   -52,
    -439,  -439,     1,  -439,  -439,  2213,    22,    22,    22,   -38,
    -439,   -61,   -35,  -439,    26,    26,  -439,  -439,  -439,  -439,
    -439,  -439,  -439,    51,  -439,    22,  -439,  -439,  -439,  -439,
     -12,  -439,  -439,  -439,   -33,  -439,  -439,    54,   280,  -439,
    2431,  -439,  -439,  -439,    14,   127,   828,  2431,   917,   -22,
    -439,  2431,    74,  -439,  -439,  -439,  -439,  2325,  2325,  -439,
    1983,  -439,    17,  -439,  1906,   -62,    19,  -439,    16,    18,
    -439,    30,  1023,    39,    47,  1706,  -439,   -55,  -439,  -439,
      31,    31,  2747,  -439,    42,   -49,    49,  -439,  -439,  -439,
     -60,  -439,  -439,    46,    -2,  2764,    52,  -439,  -439,  -439,
    2325,  -439,  1401,    48,   108,  -439,    59,  2431,  -439,  2764,
    -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,    26,
     140,  -439,  -439,  -439,    56,    57,    62,  -439,  -439,  -439,
    2431,  -439,    26,    65,  -439,  -439,    67,    71,  2325,    91,
      80,    -7,  2431,  -439,    81,    84,    11,  -439,    87,  -439,
    -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,
    -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,
    -439,  -439,  -439,  -439,  -439,    96,    96,  -439,    90,    85,
    -439,  -439,  -439,  -439,  -439,  -439,  2325,  2431,  -439,  -439,
      86,    86,  -439,  2557,  1230,  -439,  -439,  -439,  2764,  2097,
    2431,  -439,  2431,   185,  -439,  -439,    96,  2431,  -439,     6,
    -439,  2325,  2325,  2325,  2325,  1906,  1906,  2325,  2325,  2325,
    2325,  2325,  2325,  2325,  2325,  2325,  2325,  2325,  2325,  2325,
    2325,  -439,    97,   140,  2039,  1307,  -439,  2269,  2325,  2155,
    -439,  2576,  -439,    92,     8,   188,   100,  1906,    11,   122,
    1307,   104,   105,   106,   109,   111,  -439,   114,  -439,    26,
    -439,  1307,  -439,  -439,  -439,  -439,   117,  1307,  1307,  2764,
    2325,  2325,  -439,   116,  -439,   119,   114,  -439,  -439,  -439,
    2325,  2325,  -439,  -439,  1906,  -439,  -439,  -439,  -439,   162,
    2325,    11,  -439,  2473,   120,   114,  -439,  -439,  -439,  1504,
    -439,  2431,  -439,  2764,  -439,  -439,  -439,  -439,  -439,  -439,
    -439,  -439,  2764,   673,   121,  2764,  2595,  -439,  -439,   281,
    1126,    44,    44,    44,    44,    44,    44,    32,    32,    86,
      86,    86,    86,  1307,  -439,  -439,  2764,  2431,   163,  -439,
    2764,   125,   124,  -439,  2614,  -439,  2764,  -439,  -439,  -439,
    -439,  -439,  1307,  -439,  -439,  2325,   130,  1307,   133,   143,
     144,  -439,  2325,   139,  -439,     3,   145,   166,   148,   149,
     150,  2764,  2494,   203,  2431,   139,    80,  2633,   -86,  2764,
    -439,  1906,  1906,  1906,  1906,  2652,   152,  -439,  2431,   139,
    -439,  -439,  -439,   158,  -439,   180,  2325,  1806,  1906,  -439,
    -439,   246,   165,   167,  -439,  -439,     2,  -439,  -439,  -439,
    1307,  -439,  1906,  2325,   169,  -439,  2427,  -439,   170,   157,
     157,   157,  2515,  2325,  -439,  -439,  -439,   262,  2325,  -439,
    2325,   157,  -439,  -439,  -439,  -439,   114,  -439,  2325,  -439,
    -439,  -439,  -439,  -439,   171,   163,   114,  1307,   175,  2325,
    2671,  -439,   176,  -439,  1906,  -439,  1307,  -439,  2325,  -439,
    -439,  2690,  -439,  2325,  -439,   248,  -439,  -439,  -439,  -439,
    2536,  -439,  2325,  -439,  -439,  2764,  2450,  -439,   139,  2709,
    -439,  -439,  -439,  -439,   163,  -439,   139,   179,  1307,  -439,
    2764,   200,  -439,  -439,  -439,   181,  2728,  -439,  -439,  2764,
    2431,   237,   182,  -439,  2764,   278,  2325,  -439,  -439,  -439,
     187,  2325,  -439,  -439,  -439,  1906,  1906,  -439,  2431,  -439,
     184,  2431,  -439,  2764,  -439,   642,  -439,  -439,  -439,  1230,
    2431,  -439,  -439,  1906,  2325,  -439,  -439,  1605,  -439,  -439,
    2764,  -439,  -439
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,    14,   164,    58,     0,     0,   158,     0,
     151,   157,   158,   195,   158,   158,     0,     0,     0,     0,
     158,     0,     0,   196,     0,     0,   158,   158,   158,   158,
     158,   158,    56,     0,    12,     0,   158,   158,   158,    25,
     167,   165,   211,   127,     0,   158,   125,   133,     0,   142,
       0,   158,   158,   158,     0,   158,     0,     0,     0,     0,
     158,     0,     0,   197,   198,    24,    59,     0,     0,     3,
       0,    90,   172,    30,   171,    29,     0,    66,    37,    38,
      67,     0,     0,    64,     0,     0,   128,   169,   140,   151,
       0,     0,     0,   158,    65,   166,     0,    92,   172,   171,
      29,   214,    55,     0,   169,   213,    78,   217,    69,   206,
       0,   156,     0,     0,   123,    81,     0,     0,    70,    71,
     262,   227,   228,   230,   229,   218,   226,   220,   224,     0,
       0,     8,     9,   244,   241,     0,     0,    74,    76,   205,
       0,    75,     0,     0,   222,    73,     0,     0,     0,   168,
     210,     0,     0,   158,     0,     0,     0,   100,    29,    99,
     158,   263,   264,   279,   265,   266,   267,   268,   281,   275,
     282,   283,   284,   276,   269,   270,   278,   271,   277,   272,
     273,   274,   280,   101,   112,     0,     0,   102,     0,     0,
      97,    40,    36,    39,    33,   158,     0,     0,    47,    49,
     185,   194,   178,     0,     0,     5,    68,   159,    63,     0,
       0,    11,     0,     0,    10,    54,     0,     0,     6,     0,
     103,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     0,     0,     0,   233,     4,     0,     0,     0,
     158,     0,   150,     0,   166,     0,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   199,     0,   204,     0,
     242,   233,    60,    62,    84,    13,     0,   233,   233,    16,
       0,     0,   130,     0,   129,    93,   135,   139,   211,   126,
       0,     0,   134,   143,     0,   118,   114,   120,   116,     0,
       0,     0,    23,     0,    41,    42,    46,   177,   163,     0,
     104,     0,    52,    20,    31,    48,    50,    53,    51,    61,
     208,   141,    18,     0,     0,   152,     0,   159,   159,   193,
     192,   186,   187,   188,   189,   190,   191,   179,   180,   181,
     182,   183,   184,   233,   200,    21,    15,     0,     0,   237,
     238,     0,   234,   235,     0,   216,   215,   207,   176,   149,
     107,   109,   233,   159,   124,     0,     0,   233,     0,     0,
       0,   175,     0,   201,   245,   250,     0,     0,     0,     0,
       0,    17,     0,     0,     0,   136,   209,     0,     0,    94,
     159,     0,     0,     0,     0,     0,     0,    26,     0,    43,
     161,    91,   162,     0,   158,   170,     0,     0,     0,   155,
      87,    88,     0,     0,   148,   146,     0,   239,   144,   174,
       0,   170,     0,     0,     0,    79,     0,    77,     0,     0,
       0,     0,     0,     0,   252,   248,   243,     0,     0,    28,
       0,     0,    57,    27,   212,   131,   137,   132,     0,   113,
     159,   159,   159,   159,     0,     0,    44,   233,    34,     0,
       0,   154,     0,   159,     0,   173,   233,   147,     0,   236,
     159,     0,   122,     0,    72,   254,   219,   221,   225,   202,
       0,   247,     0,   250,   246,   251,     0,   223,   138,     0,
     119,   115,   121,   117,     0,    98,    45,     0,   233,    32,
     106,     0,   153,    80,   159,     0,     0,   108,   110,    82,
       0,   258,   255,   203,   253,     0,     0,    95,    96,    19,
       0,     0,    89,   240,   145,     0,     0,   256,     0,   231,
     259,     0,   252,    85,    35,     0,   159,   159,   260,     0,
       0,   257,   249,     0,     0,   111,    83,     0,   261,   159,
     105,   232,    86
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -439,  -439,  -439,   282,  -439,  -439,    -1,     4,  -439,  -439,
    -439,  -439,  -439,  -111,  -439,  -439,  -439,    66,   443,  -439,
      72,   249,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,
    -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,    45,  -439,
    -439,  -439,  -439,  -439,  -439,   -96,     9,  -211,  -439,  -438,
    -439,   201,  -439,  -439,   204,  -439,  -439,  2397,  -300,  -248,
     373,  -439,  -439,    53,  -279,  -439,     0,  -439,  -159,  -439,
      13,  -439,  -439,  -439,  -439,  -439,  -439,  -439,    79,  -409,
    -439,  -258,  -439,  -128,  -439,  -439,  -439,  -439,  -439,  -189,
    -439,  -235,  -439,  -439,  -439,  -439,  -439,  -126,   368
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   143,    98,    73,   195,    99,   100,   404,   499,
      76,    77,   304,   305,    78,    79,    80,   206,    82,    83,
     272,   402,   259,   526,   377,   543,   204,   301,   422,   423,
     525,   294,   392,   394,   391,   393,   257,   258,   103,   153,
      86,   284,   155,   156,   285,   286,   104,    88,   160,   417,
      89,   111,   324,   408,   225,    90,    91,   288,   310,   309,
     208,    93,    94,   266,   373,   267,    95,   250,   289,   149,
     150,   106,   108,    96,   262,   263,   276,   264,   125,   476,
     539,   351,   352,   353,    97,   375,   134,   436,   483,   437,
     438,   481,   482,   511,   512,   529,   530,   126,   273
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,   293,   366,   467,   268,    75,   434,   385,   321,     8,
      87,   448,   115,   376,   320,   219,   209,   495,   247,   379,
     380,   477,   478,   221,   133,   135,   399,   410,   411,   244,
     120,   107,   487,    29,     8,   131,   302,   121,   122,   210,
      26,   210,   222,   282,   283,   110,    85,   157,   245,    43,
      44,   123,   158,    38,    46,    39,   518,   113,    29,   129,
     124,   132,    49,   425,   189,   142,   148,    81,   151,   113,
     113,   187,   101,    74,    43,    44,   221,   196,    75,    46,
      58,    59,   199,    87,    74,   412,   244,    65,   321,    75,
     449,   357,   154,   159,    87,   248,   127,   128,   112,   468,
     188,     5,    43,    44,   424,   245,   360,    46,   435,   428,
     361,    74,   254,   205,   144,   211,    75,   268,   212,    85,
     213,    87,   237,   238,   239,    26,   214,   240,   224,   265,
      85,    32,   235,   236,   237,   238,   239,    37,    38,   240,
      39,   217,   274,   218,   243,   246,   256,   219,   120,   255,
     490,   491,   492,   493,   249,   292,   260,    85,   269,   271,
     158,   275,   270,   503,   277,    58,    59,   488,   278,   280,
     507,     8,    65,    66,   113,    43,    44,   496,   290,   281,
      46,   240,   300,    49,   295,   297,   291,   113,   210,   158,
     158,   299,    26,   316,   343,    29,   320,   362,   359,   497,
     365,   367,   368,   369,   522,    38,   370,    39,   505,   371,
     414,    43,    44,   372,   378,   317,    46,   383,    49,    49,
     158,   384,   398,   419,    74,    74,   420,   407,   427,    75,
      75,   429,    58,    59,    87,    87,   545,   546,   433,    65,
     520,   430,   431,   439,   440,   458,   441,   442,   443,   552,
      84,   296,   298,   445,   455,   457,    74,   364,   459,   464,
     475,    75,   158,   465,   466,   484,    87,   472,   474,   374,
      85,    85,   498,   494,   502,   312,   510,   519,   521,   523,
     528,   532,   318,    72,   531,   534,   540,   456,   446,   319,
     223,   547,   469,    74,   515,   226,   344,   542,    75,     0,
     396,   386,    85,    87,     0,   158,     0,     0,    74,    26,
       0,   349,     0,    75,   113,   355,     0,     0,    87,     0,
       0,     0,    38,   207,    39,     0,   349,     0,    43,    44,
       0,     0,     0,    46,   220,     0,   388,   349,     0,    85,
       0,     0,     0,   349,   349,     0,     0,   415,   418,    58,
      59,     0,   158,     0,    85,     0,    65,   416,     0,     0,
       0,   253,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,    92,     0,   240,     0,     0,   105,
       0,     0,     0,     0,     0,     0,     0,     0,   119,     0,
      74,    74,    74,    74,     0,    75,    75,    75,    75,     0,
      87,    87,    87,    87,     0,     0,    74,    74,     0,   349,
       0,    75,    75,     0,     0,     0,    87,    87,   183,     0,
       0,    74,     0,     0,   192,   193,    75,     0,   349,   198,
       0,    87,     0,   349,     0,     0,    85,    85,    85,    85,
     200,   201,     0,   203,     0,     0,     0,     0,     0,   102,
       0,     0,    85,    85,   415,   418,     0,     0,     0,   158,
       0,     0,     0,    74,   416,     0,     0,    85,    75,     0,
       0,     0,     0,    87,   327,   328,     0,     0,     0,     0,
       0,     0,     0,   251,     0,   261,   349,     0,     0,     0,
       0,     0,     0,   415,   418,     0,     0,     0,   158,     0,
       0,     0,     0,   416,     0,     0,   363,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     287,   279,     0,   349,    74,    74,     0,     0,     0,    75,
      75,     0,   349,     0,    87,    87,     0,     0,     0,     0,
       0,     0,    74,   390,     0,     0,    74,    75,     0,     0,
       0,    75,    87,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,   349,   306,     0,     0,     0,   303,
      85,    85,     0,     0,     0,     0,     0,     0,   314,     0,
     315,     0,   313,     0,     0,     0,     0,     0,    85,     0,
       0,     0,    85,     0,   322,   323,   325,   326,     0,     0,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,     0,     0,   346,   350,     0,
     313,   354,   356,     0,     0,     0,     0,     0,   102,   102,
       0,     0,     0,   350,     0,     0,     0,     0,     0,     0,
     450,   451,   452,   453,   350,     0,     0,     0,     0,     0,
     350,   350,   102,   381,   382,     0,   462,   463,     0,   102,
       0,     0,     0,   387,   389,     0,     0,     0,     0,     0,
       0,   470,     0,   395,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,   102,     0,
       0,     0,   102,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   102,   413,   350,     0,     0,     0,
     102,   102,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   350,     0,   240,   426,     0,
     350,     0,     0,     0,     0,   432,     0,   544,     0,     0,
       0,     0,   287,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   306,     0,   240,     0,
       0,   405,     0,     0,   536,   537,     0,     0,   406,   460,
       0,     0,     0,     0,     0,     0,   102,     0,     0,     0,
       0,     0,   549,   350,     0,     0,   471,     0,     0,     0,
       0,     0,     0,     0,     0,   102,   480,     0,     0,     0,
     102,   485,     0,   486,     0,     0,     0,     0,     0,     0,
       0,   489,     0,     0,     0,     0,     0,     0,     0,   191,
     350,     0,   500,   161,     0,     0,   162,   163,   164,   350,
       0,   506,     0,   165,   166,     0,   509,     0,     0,     0,
       0,     0,     0,   167,     0,   514,     0,   168,     0,     0,
     169,     0,     0,   102,     0,     0,   170,     0,     0,   171,
     172,   350,   173,     0,     0,     0,     0,     0,   527,     0,
       0,   174,     0,     0,   175,   176,     0,     0,   177,   533,
       0,     0,     0,   178,   535,     0,   538,   179,   180,   541,
     102,     0,   181,   182,     0,     0,     0,     0,   548,   102,
       0,     0,     0,     0,     0,     0,     0,   550,   194,     0,
     -22,   -22,   -22,   -22,   -22,   -22,   -22,     0,   -22,   -22,
     -22,   -22,   -22,   -22,   -22,     0,     0,     0,   -22,   -22,
       0,   102,   -22,   -22,   -22,     0,   -22,   -22,   -22,   -22,
     -22,   -22,   -22,   -22,     0,     0,     0,   -22,   -22,   -22,
       0,   -22,   -22,   -22,   -22,   -22,   -22,     0,     0,   -22,
     -22,   -22,   -22,   -22,     0,   -22,   -22,     0,   -22,   -22,
     -22,   -22,     0,     0,     0,     0,   -22,   -22,   -22,     0,
       0,   -22,   -22,   -22,   -22,   -22,     0,   -22,   -22,   -22,
     -22,   -22,   -22,   -22,   -22,   -22,   -22,   -22,   -22,   -22,
       0,   -22,   -22,   -22,   -22,   -22,   -22,   -22,   -22,   -22,
     -22,   -22,   -22,   -22,   215,     0,     0,   -55,   -55,   -55,
     -55,   -55,   -55,     0,   -55,   -55,   -55,   -55,   -55,   -55,
     -55,     0,     0,     0,   -55,   -55,     0,     0,   -55,   -55,
     -55,     0,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,
       0,     0,     0,   -55,   -55,   -55,     0,   -55,   -55,   -55,
     -55,   -55,   -55,     0,     0,   -55,   -55,   -55,   -55,   -55,
       0,   -55,   -55,     0,   -55,   -55,   -55,   -55,     0,     0,
       0,     0,   -55,   -55,   -55,     0,     0,   -55,   -55,   -55,
     -55,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -55,     0,     0,     0,     0,   -55,     0,   -55,
     -55,   -55,     0,     0,     0,     0,   -55,   -55,     0,   -55,
       2,     3,     0,     0,     4,     5,     6,     7,     8,     9,
       0,    10,    11,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,     0,    26,
      27,    28,    29,    30,    31,    32,    33,    34,     0,    35,
      36,    37,    38,     0,    39,    40,    41,    42,    43,    44,
       0,     0,    45,    46,    47,    48,    49,     0,    50,    51,
       0,    52,    53,    54,    55,     0,    56,    57,     0,    58,
      59,    60,    61,    62,    63,    64,    65,    66,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,    67,
       0,   240,     0,     0,    68,     0,    69,    70,     0,     0,
       0,   308,     0,    71,  -160,  -160,  -160,  -160,  -160,  -160,
       0,  -160,  -160,     0,  -160,  -160,  -160,  -160,     0,     0,
       0,  -160,  -160,     0,     0,  -160,  -160,  -160,     0,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,     0,     0,     0,
    -160,  -160,  -160,     0,  -160,  -160,  -160,  -160,  -160,  -160,
       0,     0,  -160,  -160,  -160,  -160,  -160,     0,  -160,  -160,
       0,  -160,  -160,  -160,  -160,     0,     0,     0,     0,  -160,
    -160,  -160,     0,     0,  -160,  -160,  -160,  -160,     0,     0,
       0,     4,     5,     0,     0,     8,     9,     0,     0,  -160,
       0,     0,    13,     0,  -160,     0,  -160,  -160,    19,     0,
       0,     0,    23,  -160,  -160,     0,    26,     0,     0,    29,
       0,     0,    32,     0,     0,     0,     0,     0,    37,    38,
       0,    39,    40,    41,    42,    43,    44,     0,     0,     0,
      46,     0,     0,    49,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,    58,    59,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    67,     0,     0,     0,
       0,    68,     0,     0,    70,     4,     5,     6,     7,     8,
       9,     0,    10,    11,   348,    12,    13,    14,    15,     0,
       0,     0,    19,    20,     0,     0,    23,    24,    25,     0,
      26,    27,    28,    29,    30,    31,    32,    33,     0,     0,
       0,    36,    37,    38,     0,    39,    40,    41,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,     0,    50,
      51,     0,    52,    53,    54,    55,     0,     0,     0,     0,
      58,    59,    60,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,     0,     0,     0,    68,     0,     0,    70,     0,
       0,     0,     0,     0,    71,     0,     0,   252,     4,     5,
       6,     7,     8,     9,     0,    10,    11,     0,    12,    13,
      14,    15,     0,     0,     0,    19,    20,     0,     0,    23,
      24,    25,     0,    26,    27,    28,    29,    30,    31,    32,
      33,     0,     0,     0,    36,    37,    38,     0,    39,    40,
      41,    42,    43,    44,     0,     0,    45,    46,    47,    48,
      49,     0,    50,    51,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    58,    59,    60,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,    68,     0,
     400,    70,     0,     0,     0,     0,     0,    71,   401,     4,
       5,     6,     7,     8,     9,     0,    10,    11,     0,    12,
      13,    14,    15,     0,     0,     0,    19,    20,     0,     0,
      23,    24,    25,     0,    26,    27,    28,    29,    30,    31,
      32,    33,     0,     0,     0,    36,    37,    38,     0,    39,
      40,    41,    42,    43,    44,     0,     0,    45,    46,    47,
      48,    49,     0,    50,    51,     0,    52,    53,    54,    55,
       0,     0,     0,     0,    58,    59,    60,     0,     0,    63,
      64,    65,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,    68,
       0,   400,    70,     0,     0,     0,     0,     0,    71,   551,
       4,     5,     6,     7,     8,     9,     0,    10,    11,     0,
      12,    13,    14,    15,     0,     0,     0,    19,    20,     0,
       0,    23,    24,    25,     0,    26,    27,    28,    29,    30,
      31,    32,    33,     0,     0,     0,    36,    37,    38,     0,
      39,    40,    41,    42,    43,    44,     0,     0,    45,    46,
      47,    48,    49,     0,    50,    51,     0,    52,    53,    54,
      55,     0,     0,     0,     0,    58,    59,    60,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,   219,     0,    71,
       4,     5,     6,     7,     8,     9,     0,    10,    11,     0,
      12,    13,    14,    15,     0,     0,     0,    19,    20,     0,
       0,    23,    24,    25,     0,    26,    27,    28,    29,    30,
      31,    32,    33,     0,     0,     0,    36,    37,    38,     0,
      39,    40,    41,    42,    43,    44,     0,     0,    45,    46,
      47,    48,    49,     0,    50,    51,     0,    52,    53,    54,
      55,     0,     0,     0,     0,    58,    59,    60,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,     0,     0,     0,
      68,     0,     0,    70,   461,     0,     0,     0,     0,    71,
       4,     5,     6,     7,     8,     9,     0,    10,    11,     0,
      12,    13,    14,    15,     0,     0,     0,    19,    20,     0,
       0,    23,    24,    25,     0,    26,    27,    28,    29,    30,
      31,    32,    33,     0,     0,     0,    36,    37,    38,     0,
      39,    40,    41,    42,    43,    44,     0,     0,    45,    46,
      47,    48,    49,     0,    50,    51,     0,    52,    53,    54,
      55,     0,     0,     0,     0,    58,    59,    60,     0,     0,
      63,    64,    65,    66,   202,     0,     0,     4,     0,     0,
       0,     8,     9,     0,     0,    67,     0,     0,    13,     0,
      68,     0,     0,    70,    19,     0,     0,     0,    23,    71,
       0,     0,    26,     0,     0,    29,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    38,     0,    39,    40,    41,
      42,    43,    44,     0,     0,     0,    46,     0,     0,    49,
     345,     0,     0,     4,     0,     0,     0,     8,     9,     0,
       0,     0,    58,    59,    13,     0,     0,    63,    64,    65,
      19,     0,     0,     0,    23,     0,     0,     0,    26,     0,
       0,    29,    67,     0,     0,     0,     0,    68,     0,     0,
      70,    38,     0,    39,    40,    41,    42,    43,    44,     0,
       0,     0,    46,     0,     0,    49,     0,     0,     0,     0,
       0,     4,     5,     0,     0,     8,     9,     0,    58,    59,
       0,     0,    13,    63,    64,    65,     0,     0,    19,     0,
       0,     0,    23,     0,     0,     0,    26,     0,    67,    29,
       0,     0,    32,    68,     0,     0,    70,     0,    37,    38,
       0,    39,    40,    41,    42,    43,    44,     0,     0,     0,
      46,     0,     0,    49,     0,     0,     0,     0,     0,     4,
       5,     0,     0,     8,     9,   311,    58,    59,     0,     0,
      13,    63,    64,    65,    66,     0,    19,     0,     0,     0,
      23,     0,     0,     0,    26,     0,    67,    29,     0,     0,
      32,    68,     0,     0,    70,     0,    37,    38,     0,    39,
      40,    41,    42,    43,    44,     0,     0,     0,    46,     0,
       0,    49,     0,     0,     0,     0,     0,     4,     0,     0,
       0,     8,     9,     0,    58,    59,     0,     0,    13,    63,
      64,    65,    66,     0,    19,     0,     0,     0,    23,     0,
       0,     0,    26,     0,    67,    29,     0,     0,     0,    68,
       0,     0,    70,     0,     0,    38,     0,    39,    40,    41,
      42,    43,    44,     0,     0,     0,    46,     0,     0,    49,
       0,     0,     0,     4,     0,     0,     0,     8,     9,     0,
       0,   117,    58,    59,    13,     0,     0,    63,    64,    65,
      19,     0,     0,     0,    23,     0,     0,     0,    26,     0,
       0,    29,    67,     0,     0,     0,     0,    68,     0,     0,
      70,    38,     0,    39,    40,    41,    42,    43,    44,     0,
       0,     0,    46,     0,     0,    49,     0,     0,     0,     4,
       0,     0,     0,     8,     9,     0,     0,   311,    58,    59,
      13,     0,     0,    63,    64,    65,    19,     0,     0,     0,
      23,     0,     0,     0,    26,     0,     0,    29,    67,     0,
       0,     0,     0,    68,     0,     0,    70,    38,     0,    39,
      40,    41,    42,    43,    44,     0,     0,     0,    46,     0,
       0,    49,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,     0,     0,     0,    63,
      64,    65,     0,     0,     0,   109,     0,     0,     0,   114,
       0,   116,   118,     0,    67,     0,     0,   130,     0,    68,
       0,     0,    70,   136,   137,   138,   139,   140,   141,     0,
       0,     0,     0,   145,   146,   147,   161,     0,     0,   162,
     163,   164,   152,     0,     0,     0,   165,   166,   184,   185,
     186,     0,   190,     0,     0,     0,   167,   197,     0,     0,
     168,     0,     0,   169,     0,     0,     0,     0,     0,   170,
       0,     0,   171,   172,     0,   173,     0,     0,     0,     0,
       0,     0,     0,     0,   174,     0,     0,   175,   176,     0,
     242,   177,     0,     0,     0,     0,   178,     0,     0,     0,
     179,   180,     0,     0,     0,   181,   182,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
       0,     0,   240,     0,     0,     0,     0,     0,     0,   473,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,     0,   240,     0,     0,     0,     0,
       0,     0,   516,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,     0,     0,   240,     0,
       0,     0,     0,   397,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,     0,     0,   240,
       0,     0,     0,     0,   444,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,     0,     0,
     240,     0,     0,     0,     0,   479,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,     0,
       0,   240,     0,     0,     0,     0,   513,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
       0,     0,   240,     0,     0,   307,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,     0,
       0,   240,     0,     0,   358,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,     0,     0,
     240,     0,     0,   409,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,     0,     0,   240,
       0,     0,   421,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,     0,     0,   240,     0,
       0,   447,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,     0,     0,   240,     0,     0,
     454,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,     0,     0,   240,     0,     0,   501,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,     0,   240,     0,     0,   508,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,     0,     0,   240,     0,     0,   517,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,     0,     0,   240,     0,     0,   524,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
       0,     0,   240,   241,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,     0,     0,   240
};

static const yytype_int16 yycheck[] =
{
       1,   160,   260,     1,   130,     1,     3,   286,   219,     8,
       1,    97,    12,   271,     8,   101,    78,   455,    78,   277,
     278,   430,   431,    78,    24,    25,   305,   327,   328,    78,
       8,     8,   441,    32,     8,    96,   195,    15,    16,   101,
      29,   101,    97,    50,    51,    97,     1,    48,    97,    48,
      49,    29,    48,    42,    53,    44,   494,    12,    32,    97,
      38,    96,    56,   363,    55,    14,    78,     1,   101,    24,
      25,    57,     6,    74,    48,    49,    78,    99,    74,    53,
      69,    70,     8,    74,    85,   343,    78,    76,   299,    85,
     390,   250,    47,    48,    85,    97,    17,    18,    97,    97,
      55,     5,    48,    49,   362,    97,    98,    53,   105,   367,
     102,   112,   112,    96,    35,    96,   112,   243,   102,    74,
     102,   112,    90,    91,    92,    29,    96,    95,    97,   129,
      85,    35,    88,    89,    90,    91,    92,    41,    42,    95,
      44,   102,   142,    96,   102,    96,    38,   101,     8,   101,
     450,   451,   452,   453,   102,   156,    97,   112,   102,    97,
     156,    96,   105,   463,    97,    69,    70,   446,    97,    78,
     470,     8,    76,    77,   129,    48,    49,   456,    97,    99,
      53,    95,    97,    56,   185,   186,   102,   142,   101,   185,
     186,   101,    29,     8,    97,    32,     8,    97,   106,   457,
      78,    97,    97,    97,   504,    42,    97,    44,   466,    98,
      47,    48,    49,    99,    97,   216,    53,   101,    56,    56,
     216,   102,   102,    98,   225,   226,   102,   106,    98,   225,
     226,    98,    69,    70,   225,   226,   536,   537,    99,    76,
     498,    98,    98,    98,    78,   404,    98,    98,    98,   549,
       1,   185,   186,    50,   102,    97,   257,   258,    78,    13,
     103,   257,   258,    98,    97,     3,   257,    98,    98,   269,
     225,   226,    97,   102,    98,   209,    28,    98,    78,    98,
      43,     3,   216,     1,   102,    98,   102,   398,   384,   217,
      89,   539,   420,   294,   483,    91,   243,   532,   294,    -1,
     301,   288,   257,   294,    -1,   301,    -1,    -1,   309,    29,
      -1,   245,    -1,   309,   269,   249,    -1,    -1,   309,    -1,
      -1,    -1,    42,    74,    44,    -1,   260,    -1,    48,    49,
      -1,    -1,    -1,    53,    85,    -1,   291,   271,    -1,   294,
      -1,    -1,    -1,   277,   278,    -1,    -1,   348,   348,    69,
      70,    -1,   348,    -1,   309,    -1,    76,   348,    -1,    -1,
      -1,   112,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,     1,    -1,    95,    -1,    -1,     6,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
     391,   392,   393,   394,    -1,   391,   392,   393,   394,    -1,
     391,   392,   393,   394,    -1,    -1,   407,   408,    -1,   343,
      -1,   407,   408,    -1,    -1,    -1,   407,   408,    50,    -1,
      -1,   422,    -1,    -1,    56,    57,   422,    -1,   362,    61,
      -1,   422,    -1,   367,    -1,    -1,   391,   392,   393,   394,
      67,    68,    -1,    70,    -1,    -1,    -1,    -1,    -1,     6,
      -1,    -1,   407,   408,   455,   455,    -1,    -1,    -1,   455,
      -1,    -1,    -1,   464,   455,    -1,    -1,   422,   464,    -1,
      -1,    -1,    -1,   464,   225,   226,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,   117,   420,    -1,    -1,    -1,
      -1,    -1,    -1,   494,   494,    -1,    -1,    -1,   494,    -1,
      -1,    -1,    -1,   494,    -1,    -1,   257,    -1,    -1,   464,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,   148,    -1,   457,   525,   526,    -1,    -1,    -1,   525,
     526,    -1,   466,    -1,   525,   526,    -1,    -1,    -1,    -1,
      -1,    -1,   543,   294,    -1,    -1,   547,   543,    -1,    -1,
      -1,   547,   543,    -1,    -1,    -1,   547,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   498,   197,    -1,    -1,    -1,   196,
     525,   526,    -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,
     212,    -1,   209,    -1,    -1,    -1,    -1,    -1,   543,    -1,
      -1,    -1,   547,    -1,   221,   222,   223,   224,    -1,    -1,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,    -1,    -1,    -1,   244,   245,    -1,
     247,   248,   249,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,   260,    -1,    -1,    -1,    -1,    -1,    -1,
     391,   392,   393,   394,   271,    -1,    -1,    -1,    -1,    -1,
     277,   278,   209,   280,   281,    -1,   407,   408,    -1,   216,
      -1,    -1,    -1,   290,   291,    -1,    -1,    -1,    -1,    -1,
      -1,   422,    -1,   300,    -1,    -1,    -1,    -1,    -1,   311,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   245,    -1,
      -1,    -1,   249,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   260,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   464,   271,   347,   343,    -1,    -1,    -1,
     277,   278,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,   362,    -1,    95,   365,    -1,
     367,    -1,    -1,    -1,    -1,   372,    -1,   105,    -1,    -1,
      -1,    -1,   384,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,   398,    -1,    95,    -1,
      -1,    98,    -1,    -1,   525,   526,    -1,    -1,   105,   406,
      -1,    -1,    -1,    -1,    -1,    -1,   343,    -1,    -1,    -1,
      -1,    -1,   543,   420,    -1,    -1,   423,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   362,   433,    -1,    -1,    -1,
     367,   438,    -1,   440,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   448,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
     457,    -1,   459,     5,    -1,    -1,     8,     9,    10,   466,
      -1,   468,    -1,    15,    16,    -1,   473,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,   482,    -1,    29,    -1,    -1,
      32,    -1,    -1,   420,    -1,    -1,    38,    -1,    -1,    41,
      42,   498,    44,    -1,    -1,    -1,    -1,    -1,   510,    -1,
      -1,    53,    -1,    -1,    56,    57,    -1,    -1,    60,   516,
      -1,    -1,    -1,    65,   521,    -1,   528,    69,    70,   531,
     457,    -1,    74,    75,    -1,    -1,    -1,    -1,   540,   466,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   544,     1,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    12,
      13,    14,    15,    16,    17,    -1,    -1,    -1,    21,    22,
      -1,   498,    25,    26,    27,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    44,    45,    46,    47,    48,    49,    -1,    -1,    52,
      53,    54,    55,    56,    -1,    58,    59,    -1,    61,    62,
      63,    64,    -1,    -1,    -1,    -1,    69,    70,    71,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,     1,    -1,    -1,     4,     5,     6,
       7,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
      17,    -1,    -1,    -1,    21,    22,    -1,    -1,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    44,    45,    46,
      47,    48,    49,    -1,    -1,    52,    53,    54,    55,    56,
      -1,    58,    59,    -1,    61,    62,    63,    64,    -1,    -1,
      -1,    -1,    69,    70,    71,    -1,    -1,    74,    75,    76,
      77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    98,    -1,    -1,    -1,    -1,   103,   104,    -1,   106,
       0,     1,    -1,    -1,     4,     5,     6,     7,     8,     9,
      -1,    11,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    -1,    39,
      40,    41,    42,    -1,    44,    45,    46,    47,    48,    49,
      -1,    -1,    52,    53,    54,    55,    56,    -1,    58,    59,
      -1,    61,    62,    63,    64,    -1,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    89,
      -1,    95,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,     1,    -1,   103,     4,     5,     6,     7,     8,     9,
      -1,    11,    12,    -1,    14,    15,    16,    17,    -1,    -1,
      -1,    21,    22,    -1,    -1,    25,    26,    27,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    44,    45,    46,    47,    48,    49,
      -1,    -1,    52,    53,    54,    55,    56,    -1,    58,    59,
      -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    69,
      70,    71,    -1,    -1,    74,    75,    76,    77,    -1,    -1,
      -1,     4,     5,    -1,    -1,     8,     9,    -1,    -1,    89,
      -1,    -1,    15,    -1,    94,    -1,    96,    97,    21,    -1,
      -1,    -1,    25,   103,   104,    -1,    29,    -1,    -1,    32,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    44,    45,    46,    47,    48,    49,    -1,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    70,    -1,    -1,
      -1,    74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    97,     4,     5,     6,     7,     8,
       9,    -1,    11,    12,   107,    14,    15,    16,    17,    -1,
      -1,    -1,    21,    22,    -1,    -1,    25,    26,    27,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    44,    45,    46,    47,    48,
      49,    -1,    -1,    52,    53,    54,    55,    56,    -1,    58,
      59,    -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,
      69,    70,    71,    -1,    -1,    74,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,   103,    -1,    -1,   106,     4,     5,
       6,     7,     8,     9,    -1,    11,    12,    -1,    14,    15,
      16,    17,    -1,    -1,    -1,    21,    22,    -1,    -1,    25,
      26,    27,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    44,    45,
      46,    47,    48,    49,    -1,    -1,    52,    53,    54,    55,
      56,    -1,    58,    59,    -1,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    69,    70,    71,    -1,    -1,    74,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,   103,   104,     4,
       5,     6,     7,     8,     9,    -1,    11,    12,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    21,    22,    -1,    -1,
      25,    26,    27,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    44,
      45,    46,    47,    48,    49,    -1,    -1,    52,    53,    54,
      55,    56,    -1,    58,    59,    -1,    61,    62,    63,    64,
      -1,    -1,    -1,    -1,    69,    70,    71,    -1,    -1,    74,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,   103,   104,
       4,     5,     6,     7,     8,     9,    -1,    11,    12,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    21,    22,    -1,
      -1,    25,    26,    27,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      44,    45,    46,    47,    48,    49,    -1,    -1,    52,    53,
      54,    55,    56,    -1,    58,    59,    -1,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    69,    70,    71,    -1,    -1,
      74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,   103,
       4,     5,     6,     7,     8,     9,    -1,    11,    12,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    21,    22,    -1,
      -1,    25,    26,    27,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      44,    45,    46,    47,    48,    49,    -1,    -1,    52,    53,
      54,    55,    56,    -1,    58,    59,    -1,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    69,    70,    71,    -1,    -1,
      74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,   103,
       4,     5,     6,     7,     8,     9,    -1,    11,    12,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    21,    22,    -1,
      -1,    25,    26,    27,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      44,    45,    46,    47,    48,    49,    -1,    -1,    52,    53,
      54,    55,    56,    -1,    58,    59,    -1,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    69,    70,    71,    -1,    -1,
      74,    75,    76,    77,     1,    -1,    -1,     4,    -1,    -1,
      -1,     8,     9,    -1,    -1,    89,    -1,    -1,    15,    -1,
      94,    -1,    -1,    97,    21,    -1,    -1,    -1,    25,   103,
      -1,    -1,    29,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    -1,    44,    45,    46,
      47,    48,    49,    -1,    -1,    -1,    53,    -1,    -1,    56,
       1,    -1,    -1,     4,    -1,    -1,    -1,     8,     9,    -1,
      -1,    -1,    69,    70,    15,    -1,    -1,    74,    75,    76,
      21,    -1,    -1,    -1,    25,    -1,    -1,    -1,    29,    -1,
      -1,    32,    89,    -1,    -1,    -1,    -1,    94,    -1,    -1,
      97,    42,    -1,    44,    45,    46,    47,    48,    49,    -1,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,     4,     5,    -1,    -1,     8,     9,    -1,    69,    70,
      -1,    -1,    15,    74,    75,    76,    -1,    -1,    21,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    29,    -1,    89,    32,
      -1,    -1,    35,    94,    -1,    -1,    97,    -1,    41,    42,
      -1,    44,    45,    46,    47,    48,    49,    -1,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,     4,
       5,    -1,    -1,     8,     9,    68,    69,    70,    -1,    -1,
      15,    74,    75,    76,    77,    -1,    21,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    29,    -1,    89,    32,    -1,    -1,
      35,    94,    -1,    -1,    97,    -1,    41,    42,    -1,    44,
      45,    46,    47,    48,    49,    -1,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,     4,    -1,    -1,
      -1,     8,     9,    -1,    69,    70,    -1,    -1,    15,    74,
      75,    76,    77,    -1,    21,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    29,    -1,    89,    32,    -1,    -1,    -1,    94,
      -1,    -1,    97,    -1,    -1,    42,    -1,    44,    45,    46,
      47,    48,    49,    -1,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    -1,     4,    -1,    -1,    -1,     8,     9,    -1,
      -1,    68,    69,    70,    15,    -1,    -1,    74,    75,    76,
      21,    -1,    -1,    -1,    25,    -1,    -1,    -1,    29,    -1,
      -1,    32,    89,    -1,    -1,    -1,    -1,    94,    -1,    -1,
      97,    42,    -1,    44,    45,    46,    47,    48,    49,    -1,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,     4,
      -1,    -1,    -1,     8,     9,    -1,    -1,    68,    69,    70,
      15,    -1,    -1,    74,    75,    76,    21,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    29,    -1,    -1,    32,    89,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    97,    42,    -1,    44,
      45,    46,    47,    48,    49,    -1,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    74,
      75,    76,    -1,    -1,    -1,     8,    -1,    -1,    -1,    12,
      -1,    14,    15,    -1,    89,    -1,    -1,    20,    -1,    94,
      -1,    -1,    97,    26,    27,    28,    29,    30,    31,    -1,
      -1,    -1,    -1,    36,    37,    38,     5,    -1,    -1,     8,
       9,    10,    45,    -1,    -1,    -1,    15,    16,    51,    52,
      53,    -1,    55,    -1,    -1,    -1,    25,    60,    -1,    -1,
      29,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    42,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    -1,    56,    57,    -1,
      93,    60,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    74,    75,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,   100,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,   100,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,   100,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,   100,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,    98,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    98,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,    98,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    98,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    98,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,
      98,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    98,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    98,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    95,    -1,    -1,    98,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    95,    -1,    -1,    98,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    -1,    95,    96,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    95
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   109,     0,     1,     4,     5,     6,     7,     8,     9,
      11,    12,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    39,    40,    41,    42,    44,
      45,    46,    47,    48,    49,    52,    53,    54,    55,    56,
      58,    59,    61,    62,    63,    64,    66,    67,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    89,    94,    96,
      97,   103,   111,   112,   114,   115,   118,   119,   122,   123,
     124,   125,   126,   127,   129,   146,   148,   154,   155,   158,
     163,   164,   168,   169,   170,   174,   181,   192,   111,   114,
     115,   125,   126,   146,   154,   168,   179,     8,   180,   165,
      97,   159,    97,   146,   165,   174,   165,    68,   165,   168,
       8,    15,    16,    29,    38,   186,   205,   186,   186,    97,
     165,    96,    96,   174,   194,   174,   165,   165,   165,   165,
     165,   165,    14,   110,   186,   165,   165,   165,    78,   177,
     178,   101,   165,   147,   146,   150,   151,   114,   115,   146,
     156,     5,     8,     9,    10,    15,    16,    25,    29,    32,
      38,    41,    42,    44,    53,    56,    57,    60,    65,    69,
      70,    74,    75,   206,   165,   165,   165,    57,   146,   154,
     165,     1,   206,   206,     1,   113,    99,   165,   206,     8,
     168,   168,     1,   168,   134,    96,   125,   129,   168,    78,
     101,    96,   102,   102,    96,     1,    78,   102,    96,   101,
     129,    78,    97,   159,    97,   162,   162,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      95,    96,   165,   102,    78,    97,    96,    78,    97,   102,
     175,   168,   106,   129,   174,   101,    38,   144,   145,   130,
      97,   206,   182,   183,   185,   174,   171,   173,   205,   102,
     105,    97,   128,   206,   174,    96,   184,    97,    97,   168,
      78,    99,    50,    51,   149,   152,   153,   206,   165,   176,
      97,   102,   114,   176,   139,   114,   125,   114,   125,   101,
      97,   135,   176,   168,   120,   121,   206,    98,     1,   167,
     166,    68,   125,   168,   206,   206,     8,   114,   125,   128,
       8,   155,   168,   168,   160,   168,   168,   129,   129,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,    97,   171,     1,   168,    68,   107,   125,
     168,   189,   190,   191,   168,   125,   168,   176,    98,   106,
      98,   102,    97,   129,   114,    78,   189,    97,    97,    97,
      97,    98,    99,   172,   174,   193,   189,   132,    97,   189,
     189,   168,   168,   101,   102,   172,   178,   168,   146,   168,
     129,   142,   140,   143,   141,   168,   114,   100,   102,   172,
      96,   104,   129,   206,   116,    98,   105,   106,   161,    98,
     166,   166,   189,   206,    47,   114,   154,   157,   174,    98,
     102,    98,   136,   137,   189,   166,   168,    98,   189,    98,
      98,    98,   168,    99,     3,   105,   195,   197,   198,    98,
      78,    98,    98,    98,   100,    50,   153,    98,    97,   166,
     129,   129,   129,   129,    98,   102,   121,    97,   176,    78,
     168,    98,   129,   129,    13,    98,    97,     1,    97,   191,
     129,   168,    98,   102,    98,   103,   187,   187,   187,   100,
     168,   199,   200,   196,     3,   168,   168,   187,   172,   168,
     166,   166,   166,   166,   102,   157,   172,   189,    97,   117,
     168,    98,    98,   166,   129,   189,   168,   166,    98,   168,
      28,   201,   202,   100,   168,   197,   102,    98,   157,    98,
     189,    78,   166,    98,    98,   138,   131,   206,    43,   203,
     204,   102,     3,   168,    98,   168,   129,   129,   206,   188,
     102,   206,   199,   133,   105,   166,   166,   167,   206,   129,
     168,   104,   166
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   108,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   110,   109,   109,   111,   111,   111,   111,   111,
     111,   111,   113,   112,   112,   112,   112,   112,   112,   114,
     115,   116,   115,   115,   117,   117,   118,   118,   118,   118,
     118,   119,   120,   120,   120,   120,   121,   122,   122,   123,
     123,   124,   124,   124,   124,   125,   125,   125,   126,   126,
     127,   127,   128,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   130,   131,   129,   132,   133,   129,   129,   129,   129,
     134,   129,   129,   129,   129,   129,   129,   135,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   136,   129,   137,
     138,   129,   139,   129,   140,   129,   141,   129,   142,   129,
     143,   129,   144,   145,   144,   147,   146,   146,   146,   148,
     149,   149,   150,   151,   150,   152,   152,   152,   152,   153,
     154,   154,   156,   155,   157,   157,   157,   157,   157,   158,
     158,   159,   160,   161,   161,   162,   163,   164,   165,   166,
     167,   167,   167,   167,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   169,   169,   169,   169,   170,
     170,   171,   172,   172,   173,   174,   175,   174,   174,   176,
     177,   178,   178,   179,   179,   179,   179,   180,   182,   181,
     183,   181,   184,   181,   185,   181,   186,   186,   186,   186,
     186,   188,   187,   189,   189,   190,   190,   191,   191,   191,
     191,   192,   193,   192,   194,   194,   195,   195,   196,   195,
     198,   197,   200,   199,   201,   201,   202,   202,   203,   203,
     204,   204,   205,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     0,     4,     2,     3,     3,     4,     3,     7,
       3,     3,     0,     3,     1,     1,     4,     5,     5,     1,
       1,     0,     6,     2,     0,     3,     2,     1,     1,     2,
       2,     3,     1,     2,     3,     4,     1,     2,     3,     2,
       3,     3,     3,     3,     2,     1,     1,     5,     1,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     6,     2,     2,     2,     2,     5,     2,     5,
       6,     0,     0,    10,     0,     0,    11,     4,     4,     7,
       0,     4,     1,     3,     4,     7,     7,     0,     6,     2,
       2,     2,     2,     2,     3,    10,     6,     0,     7,     0,
       0,    10,     0,     5,     0,     6,     0,     6,     0,     6,
       0,     6,     4,     0,     2,     0,     3,     1,     1,     3,
       1,     3,     4,     0,     2,     1,     2,     3,     4,     1,
       1,     3,     0,     3,     1,     4,     1,     2,     1,     4,
       3,     0,     1,     3,     2,     3,     2,     1,     0,     0,
       0,     2,     2,     1,     1,     1,     1,     1,     2,     1,
       4,     1,     1,     5,     4,     4,     4,     3,     2,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     1,     1,     1,     3,
       3,     2,     3,     4,     1,     2,     0,     4,     3,     2,
       1,     0,     4,     1,     1,     3,     3,     1,     0,     6,
       0,     6,     0,     6,     0,     6,     1,     1,     1,     1,
       1,     0,     6,     0,     1,     1,     3,     1,     1,     2,
       5,     2,     0,     5,     1,     3,     2,     2,     0,     5,
       0,     2,     0,     2,     0,     1,     2,     3,     0,     1,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
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
#line 141 "parse.y" /* yacc.c:1646  */
    { ntab = 0;}
#line 2338 "parse.c" /* yacc.c:1646  */
    break;

  case 3:
#line 142 "parse.y" /* yacc.c:1646  */
    { return '\n';}
#line 2344 "parse.c" /* yacc.c:1646  */
    break;

  case 4:
#line 143 "parse.y" /* yacc.c:1646  */
    { return '\n';}
#line 2350 "parse.c" /* yacc.c:1646  */
    break;

  case 5:
#line 145 "parse.y" /* yacc.c:1646  */
    { hoc_ob_check(-1); code2(nopop, STOP); return 1; }
#line 2356 "parse.c" /* yacc.c:1646  */
    break;

  case 6:
#line 147 "parse.y" /* yacc.c:1646  */
    { codein(STOP); return 1; }
#line 2362 "parse.c" /* yacc.c:1646  */
    break;

  case 7:
#line 149 "parse.y" /* yacc.c:1646  */
    { TPDYNAM; code2(print, STOP); return 1; }
#line 2368 "parse.c" /* yacc.c:1646  */
    break;

  case 8:
#line 151 "parse.y" /* yacc.c:1646  */
    { debug(); return '\n';}
#line 2374 "parse.c" /* yacc.c:1646  */
    break;

  case 9:
#line 153 "parse.y" /* yacc.c:1646  */
    { return 'e';}
#line 2380 "parse.c" /* yacc.c:1646  */
    break;

  case 10:
#line 155 "parse.y" /* yacc.c:1646  */
    {code(prstr); code2(hoc_newline, STOP); return 1; }
#line 2386 "parse.c" /* yacc.c:1646  */
    break;

  case 11:
#line 158 "parse.y" /* yacc.c:1646  */
    { return '\n';}
#line 2392 "parse.c" /* yacc.c:1646  */
    break;

  case 12:
#line 164 "parse.y" /* yacc.c:1646  */
    {hoc_help();}
#line 2398 "parse.c" /* yacc.c:1646  */
    break;

  case 13:
#line 164 "parse.y" /* yacc.c:1646  */
    { return '\n'; }
#line 2404 "parse.c" /* yacc.c:1646  */
    break;

  case 14:
#line 166 "parse.y" /* yacc.c:1646  */
    {clean_err(); hoc_execerror("parse error", (char*)0);
#if LINT
if (0) {
if(yydebug);
goto yynewstate;
}
#endif
		}
#line 2417 "parse.c" /* yacc.c:1646  */
    break;

  case 15:
#line 176 "parse.y" /* yacc.c:1646  */
    {Symbol *s; TPD; s = spop();
		hoc_obvar_declare(s, VAR, 1);
		code3(varpush, s, assign); codei((yyvsp[-1].narg)); PN;}
#line 2425 "parse.c" /* yacc.c:1646  */
    break;

  case 16:
#line 180 "parse.y" /* yacc.c:1646  */
    {  TPD; defnonly("$"); argcode(argassign, (yyvsp[-2].narg)); codei((yyvsp[-1].narg)); (yyval.inst)=(yyvsp[0].inst); PN;}
#line 2431 "parse.c" /* yacc.c:1646  */
    break;

  case 17:
#line 182 "parse.y" /* yacc.c:1646  */
    { TPD; defnonly("$&"); argrefcode(hoc_argrefasgn, (yyvsp[-3].narg), (yyvsp[-2].narg)); codei((yyvsp[-1].narg)); (yyval.inst)=(yyvsp[0].inst); PN;}
#line 2437 "parse.c" /* yacc.c:1646  */
    break;

  case 18:
#line 185 "parse.y" /* yacc.c:1646  */
    { TPD; code(range_const); codesym(spop()); codei((yyvsp[-1].narg)); PN;}
#line 2443 "parse.c" /* yacc.c:1646  */
    break;

  case 19:
#line 189 "parse.y" /* yacc.c:1646  */
    { Inst* p; hoc_opasgn_invalid((yyvsp[-5].narg));
		 code(hoc_newobj); codesym(hoc_which_template((yyvsp[-3].sym))); codei((yyvsp[-1].narg));
		 p = (Inst*)spop();
		 if (p) { p->i += 2; }
		}
#line 2453 "parse.c" /* yacc.c:1646  */
    break;

  case 20:
#line 195 "parse.y" /* yacc.c:1646  */
    {Inst* p; TPDYNAM; code(hoc_object_asgn); codei((yyvsp[-1].narg));
		 p = (Inst*)spop();
		 if (p) { p->i += 2; }
		}
#line 2462 "parse.c" /* yacc.c:1646  */
    break;

  case 21:
#line 200 "parse.y" /* yacc.c:1646  */
    {myerr("assignment to variable, make sure right side is a number");}
#line 2468 "parse.c" /* yacc.c:1646  */
    break;

  case 22:
#line 204 "parse.y" /* yacc.c:1646  */
    {pushi(OBJECTVAR);pushs((yyvsp[0].sym)); pushi(CHECK);}
#line 2474 "parse.c" /* yacc.c:1646  */
    break;

  case 23:
#line 205 "parse.y" /* yacc.c:1646  */
    {(yyval.inst) = (yyvsp[0].inst); code(hoc_objectvar); spop(); codesym((yyvsp[-2].sym));}
#line 2480 "parse.c" /* yacc.c:1646  */
    break;

  case 24:
#line 207 "parse.y" /* yacc.c:1646  */
    {defnonly("$o"); (yyval.inst) = argcode(hoc_objectarg, (yyvsp[0].narg)); pushi(OBJECTVAR);}
#line 2486 "parse.c" /* yacc.c:1646  */
    break;

  case 25:
#line 209 "parse.y" /* yacc.c:1646  */
    {(yyval.inst) = code(hoc_autoobject); codesym((yyvsp[0].sym)); pushi(OBJECTVAR);}
#line 2492 "parse.c" /* yacc.c:1646  */
    break;

  case 26:
#line 211 "parse.y" /* yacc.c:1646  */
    {TPD; (yyval.inst) = (yyvsp[-1].inst); code(hoc_constobject); codesym((yyvsp[-3].sym)); pushi(OBJECTVAR);}
#line 2498 "parse.c" /* yacc.c:1646  */
    break;

  case 27:
#line 213 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-3].inst); code(call); codesym((yyvsp[-4].sym)); codei((yyvsp[-1].narg));
		  code(hoc_known_type); codei(OBJECTVAR); pushi(OBJECTVAR);}
#line 2505 "parse.c" /* yacc.c:1646  */
    break;

  case 28:
#line 216 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-3].inst); code(call); codesym((yyvsp[-4].sym)); codei((yyvsp[-1].narg));
		  code(hoc_known_type); codei(OBJECTVAR); pushi(OBJECTVAR);}
#line 2512 "parse.c" /* yacc.c:1646  */
    break;

  case 29:
#line 220 "parse.y" /* yacc.c:1646  */
    { spop(); }
#line 2518 "parse.c" /* yacc.c:1646  */
    break;

  case 30:
#line 223 "parse.y" /* yacc.c:1646  */
    { pushs((Symbol*)0); }
#line 2524 "parse.c" /* yacc.c:1646  */
    break;

  case 31:
#line 224 "parse.y" /* yacc.c:1646  */
    {pushs((yyvsp[0].sym));pushi(NOCHECK);}
#line 2530 "parse.c" /* yacc.c:1646  */
    break;

  case 32:
#line 225 "parse.y" /* yacc.c:1646  */
    {int isfunc; Inst* p;
		 isfunc = ipop();
		 code(hoc_object_component); codesym(spop()); codei(ipop());
		 codei((yyvsp[0].narg));
		 codei(0); codesym(0);
		 p = codei(isfunc); /* for USE_PYTHON */
		 spop();
		 pushs((Symbol*)p); /* in case assigning to a PythonObject we will want to update isfunc to 2 */
		}
#line 2544 "parse.c" /* yacc.c:1646  */
    break;

  case 33:
#line 234 "parse.y" /* yacc.c:1646  */
    {myerr("object syntax is o1.o2.o3.");}
#line 2550 "parse.c" /* yacc.c:1646  */
    break;

  case 34:
#line 237 "parse.y" /* yacc.c:1646  */
    {(yyval.narg) = 0; pushi(0);}
#line 2556 "parse.c" /* yacc.c:1646  */
    break;

  case 35:
#line 239 "parse.y" /* yacc.c:1646  */
    {(yyval.narg) = (yyvsp[-1].narg); pushi(1);}
#line 2562 "parse.c" /* yacc.c:1646  */
    break;

  case 36:
#line 241 "parse.y" /* yacc.c:1646  */
    {hoc_begintemplate((yyvsp[0].sym));}
#line 2568 "parse.c" /* yacc.c:1646  */
    break;

  case 39:
#line 244 "parse.y" /* yacc.c:1646  */
    {hoc_endtemplate((yyvsp[0].sym));}
#line 2574 "parse.c" /* yacc.c:1646  */
    break;

  case 40:
#line 245 "parse.y" /* yacc.c:1646  */
    {myerr("begintemplate Name\npublic namelist\nexternal namelist\n...\nendtemplate Name");}
#line 2580 "parse.c" /* yacc.c:1646  */
    break;

  case 41:
#line 248 "parse.y" /* yacc.c:1646  */
    {(yyval.inst) = (yyvsp[-1].inst);}
#line 2586 "parse.c" /* yacc.c:1646  */
    break;

  case 42:
#line 251 "parse.y" /* yacc.c:1646  */
    {code(hoc_objvardecl); codesym((yyvsp[0].sym)); codei(0);}
#line 2592 "parse.c" /* yacc.c:1646  */
    break;

  case 43:
#line 253 "parse.y" /* yacc.c:1646  */
    {code(hoc_objvardecl); codesym((yyvsp[-1].sym)); codei((yyvsp[0].narg));}
#line 2598 "parse.c" /* yacc.c:1646  */
    break;

  case 44:
#line 255 "parse.y" /* yacc.c:1646  */
    {code(hoc_objvardecl); codesym((yyvsp[0].sym)); codei(0);}
#line 2604 "parse.c" /* yacc.c:1646  */
    break;

  case 45:
#line 257 "parse.y" /* yacc.c:1646  */
    {code(hoc_objvardecl); codesym((yyvsp[-1].sym)); codei((yyvsp[0].narg));}
#line 2610 "parse.c" /* yacc.c:1646  */
    break;

  case 46:
#line 260 "parse.y" /* yacc.c:1646  */
    {
		  Symbol* s;
		  s = hoc_decl((yyvsp[0].sym));
		  if (s->type != UNDEF && s->type != OBJECTVAR) {
			acterror(s->name, " already declared");
		  }
		  (yyval.sym) = s;
		}
#line 2623 "parse.c" /* yacc.c:1646  */
    break;

  case 47:
#line 270 "parse.y" /* yacc.c:1646  */
    {hoc_add_publiclist((yyvsp[0].sym));}
#line 2629 "parse.c" /* yacc.c:1646  */
    break;

  case 48:
#line 272 "parse.y" /* yacc.c:1646  */
    {hoc_add_publiclist((yyvsp[0].sym));}
#line 2635 "parse.c" /* yacc.c:1646  */
    break;

  case 49:
#line 275 "parse.y" /* yacc.c:1646  */
    {hoc_external_var((yyvsp[0].sym));}
#line 2641 "parse.c" /* yacc.c:1646  */
    break;

  case 50:
#line 277 "parse.y" /* yacc.c:1646  */
    {hoc_external_var((yyvsp[0].sym));}
#line 2647 "parse.c" /* yacc.c:1646  */
    break;

  case 51:
#line 282 "parse.y" /* yacc.c:1646  */
    {hoc_opasgn_invalid((yyvsp[-1].narg)); code(assstr);}
#line 2653 "parse.c" /* yacc.c:1646  */
    break;

  case 52:
#line 284 "parse.y" /* yacc.c:1646  */
    {Inst* p = (Inst*) spop(); pushi(STRING); TPDYNAM; code(hoc_object_asgn);
		 hoc_opasgn_invalid((yyvsp[-1].narg)); codei((yyvsp[-1].narg)); hoc_ob_check(-1); code(nopop);
		 if (p) { p->i += 2; }
		}
#line 2662 "parse.c" /* yacc.c:1646  */
    break;

  case 53:
#line 289 "parse.y" /* yacc.c:1646  */
    {code(hoc_asgn_obj_to_str); hoc_opasgn_invalid((yyvsp[-1].narg)); }
#line 2668 "parse.c" /* yacc.c:1646  */
    break;

  case 54:
#line 290 "parse.y" /* yacc.c:1646  */
    {myerr("string assignment: both sides need to be a string");}
#line 2674 "parse.c" /* yacc.c:1646  */
    break;

  case 56:
#line 295 "parse.y" /* yacc.c:1646  */
    {(yyval.inst) = code(hoc_push_string); codesym((yyvsp[0].sym));}
#line 2680 "parse.c" /* yacc.c:1646  */
    break;

  case 57:
#line 297 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-3].inst); code(call); codesym((yyvsp[-4].sym)); codei((yyvsp[-1].narg));}
#line 2686 "parse.c" /* yacc.c:1646  */
    break;

  case 58:
#line 301 "parse.y" /* yacc.c:1646  */
    {(yyval.inst) = code(hoc_push_string); codesym((yyvsp[0].sym));}
#line 2692 "parse.c" /* yacc.c:1646  */
    break;

  case 59:
#line 303 "parse.y" /* yacc.c:1646  */
    {defnonly("$s"); (yyval.inst) = argcode(hoc_stringarg, (yyvsp[0].narg));}
#line 2698 "parse.c" /* yacc.c:1646  */
    break;

  case 60:
#line 307 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-1].inst); }
#line 2704 "parse.c" /* yacc.c:1646  */
    break;

  case 62:
#line 311 "parse.y" /* yacc.c:1646  */
    {
			Symbol* s = hoc_decl((yyvsp[0].sym));
			if (s->type == UNDEF) {
				hoc_obvar_declare(s, STRING, 0);
				OPSTR(s) = (char**)emalloc(sizeof(char*));
				*OPSTR(s) = 0;
			}else if (s->type != STRING) {
				acterror(s->name, " already declared");
			}
			hoc_assign_str(OPSTR(s), "");
		}
#line 2720 "parse.c" /* yacc.c:1646  */
    break;

  case 63:
#line 324 "parse.y" /* yacc.c:1646  */
    { code(nopop); hoc_ob_check(-1); /*don't check*/}
#line 2726 "parse.c" /* yacc.c:1646  */
    break;

  case 68:
#line 332 "parse.y" /* yacc.c:1646  */
    { code(nopop); }
#line 2732 "parse.c" /* yacc.c:1646  */
    break;

  case 69:
#line 334 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[0].inst);}
#line 2738 "parse.c" /* yacc.c:1646  */
    break;

  case 70:
#line 336 "parse.y" /* yacc.c:1646  */
    { defnonly("return"); (yyval.inst)=(yyvsp[0].inst); code(procret); }
#line 2744 "parse.c" /* yacc.c:1646  */
    break;

  case 71:
#line 338 "parse.y" /* yacc.c:1646  */
    {  if (indef == 3) {
			TPDYNAM; (yyval.inst) = (yyvsp[0].inst);
			code(hocobjret);
		   }else{
			TPD; defnonly("return"); (yyval.inst)=(yyvsp[0].inst);
			code(funcret);
		   }
		}
#line 2757 "parse.c" /* yacc.c:1646  */
    break;

  case 72:
#line 347 "parse.y" /* yacc.c:1646  */
    {(yyval.inst) = code(hoc_newobj_ret); codesym(hoc_which_template((yyvsp[-3].sym))); codei((yyvsp[-1].narg));
		 code(hocobjret);
		}
#line 2765 "parse.c" /* yacc.c:1646  */
    break;

  case 73:
#line 351 "parse.y" /* yacc.c:1646  */
    { if (indef != 2) {
acterror("iterator_statement used outside an iterator declaration", 0);
		  }
			code(hoc_iterator_stmt);
		}
#line 2775 "parse.c" /* yacc.c:1646  */
    break;

  case 74:
#line 357 "parse.y" /* yacc.c:1646  */
    { code(Break); (yyval.inst) = (yyvsp[0].inst); }
#line 2781 "parse.c" /* yacc.c:1646  */
    break;

  case 75:
#line 359 "parse.y" /* yacc.c:1646  */
    { code(Stop); (yyval.inst) = (yyvsp[0].inst); }
#line 2787 "parse.c" /* yacc.c:1646  */
    break;

  case 76:
#line 361 "parse.y" /* yacc.c:1646  */
    { code(Continue); (yyval.inst) = (yyvsp[0].inst); }
#line 2793 "parse.c" /* yacc.c:1646  */
    break;

  case 77:
#line 363 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-3].inst); code(call); codesym((yyvsp[-4].sym)); codei((yyvsp[-1].narg)); code(nopop); }
#line 2799 "parse.c" /* yacc.c:1646  */
    break;

  case 78:
#line 365 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[0].inst); code(hoc_newline); }
#line 2805 "parse.c" /* yacc.c:1646  */
    break;

  case 79:
#line 367 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-3].inst);
		  ((yyvsp[-2].inst))[0].i = relative((yyvsp[-1].inst), (yyvsp[-2].inst), 0); /* body */
		  ((yyvsp[-2].inst))[1].i = relative((yyvsp[0].inst), (yyvsp[-2].inst), 1); /* exit */
		}
#line 2814 "parse.c" /* yacc.c:1646  */
    break;

  case 80:
#line 372 "parse.y" /* yacc.c:1646  */
    {
			((yyvsp[-4].inst))[1].i =relative((yyvsp[-1].inst), (yyvsp[-4].inst), 1);	/* body */
			((yyvsp[-4].inst))[2].i =relative((yyvsp[0].inst), (yyvsp[-4].inst), 2); /* exit from the for */
			((yyvsp[-4].inst))[3].i  =relative((yyvsp[-2].inst), (yyvsp[-4].inst), 3);	/* increment */
			(yyval.inst) = (yyvsp[-5].inst);
#if LINT
if (0){YYERROR;}
#endif
			}
#line 2828 "parse.c" /* yacc.c:1646  */
    break;

  case 81:
#line 382 "parse.y" /* yacc.c:1646  */
    { Symbol *s; (yyval.inst) = Code(varpush); codesym(s = spop());
			hoc_obvar_declare(s, VAR, 1);
		  }
#line 2836 "parse.c" /* yacc.c:1646  */
    break;

  case 82:
#line 386 "parse.y" /* yacc.c:1646  */
    {TPD; TPD; hoc_opasgn_invalid((yyvsp[-3].narg)); 
		    (yyval.inst) = code(shortfor); codei(0); codei(0);}
#line 2843 "parse.c" /* yacc.c:1646  */
    break;

  case 83:
#line 389 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-8].inst); ((yyvsp[-2].inst))[1].i = relative((yyvsp[-1].inst), (yyvsp[-2].inst), 1); /* body */
			   ((yyvsp[-2].inst))[2].i = relative((yyvsp[0].inst), (yyvsp[-2].inst), 2); /* exit */
		  }
#line 2851 "parse.c" /* yacc.c:1646  */
    break;

  case 84:
#line 393 "parse.y" /* yacc.c:1646  */
    { Symbol *s; (yyval.inst) = Code(varpush); codesym(s = spop());
			hoc_obvar_declare(s, VAR, 1);
		  }
#line 2859 "parse.c" /* yacc.c:1646  */
    break;

  case 85:
#line 397 "parse.y" /* yacc.c:1646  */
    {TPD; TPD; hoc_opasgn_invalid((yyvsp[-3].narg)); 
		     code(hoc_parallel_begin);
		     (yyval.inst) = code(shortfor); codei(0); codei(0);}
#line 2867 "parse.c" /* yacc.c:1646  */
    break;

  case 86:
#line 401 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-8].inst); ((yyvsp[-2].inst))[1].i = relative((yyvsp[-1].inst), (yyvsp[-2].inst), 1); /* body */
			   ((yyvsp[-2].inst))[2].i = relative((yyvsp[0].inst), (yyvsp[-2].inst), 2); /* exit */
			code(hoc_parallel_end);
		  }
#line 2876 "parse.c" /* yacc.c:1646  */
    break;

  case 87:
#line 406 "parse.y" /* yacc.c:1646  */
    {
		((yyvsp[-3].inst))[1].i = relative((yyvsp[-1].inst), (yyvsp[-3].inst), 1);	/* body of loop */
		((yyvsp[-3].inst))[2].i = relative((yyvsp[0].inst), (yyvsp[-3].inst), 2); }
#line 2884 "parse.c" /* yacc.c:1646  */
    break;

  case 88:
#line 410 "parse.y" /* yacc.c:1646  */
    {
		((yyvsp[-3].inst))[1].i = relative((yyvsp[-1].inst), (yyvsp[-3].inst), 1);	/* thenpart */
		((yyvsp[-3].inst))[3].i = relative((yyvsp[0].inst), (yyvsp[-3].inst), 3); }
#line 2892 "parse.c" /* yacc.c:1646  */
    break;

  case 89:
#line 414 "parse.y" /* yacc.c:1646  */
    {
		((yyvsp[-6].inst))[1].i = relative((yyvsp[-4].inst), (yyvsp[-6].inst), 1);	/* thenpart */
		((yyvsp[-6].inst))[2].i = relative((yyvsp[-1].inst), (yyvsp[-6].inst), 2);	/* elsepart */
		((yyvsp[-6].inst))[3].i = relative((yyvsp[0].inst), (yyvsp[-6].inst), 3); }
#line 2901 "parse.c" /* yacc.c:1646  */
    break;

  case 90:
#line 419 "parse.y" /* yacc.c:1646  */
    { ntab++;}
#line 2907 "parse.c" /* yacc.c:1646  */
    break;

  case 91:
#line 421 "parse.y" /* yacc.c:1646  */
    {
		ntab--; (yyval.inst) = (yyvsp[-1].inst);
		}
#line 2915 "parse.c" /* yacc.c:1646  */
    break;

  case 93:
#line 427 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-1].inst); }
#line 2921 "parse.c" /* yacc.c:1646  */
    break;

  case 94:
#line 429 "parse.y" /* yacc.c:1646  */
    { TPD; (yyval.inst) = (yyvsp[-2].inst); code(connectsection);}
#line 2927 "parse.c" /* yacc.c:1646  */
    break;

  case 95:
#line 431 "parse.y" /* yacc.c:1646  */
    { TPD; (yyval.inst) = (yyvsp[-5].inst); code(simpleconnectsection);}
#line 2933 "parse.c" /* yacc.c:1646  */
    break;

  case 96:
#line 433 "parse.y" /* yacc.c:1646  */
    { TPD; (yyval.inst) = (yyvsp[-5].inst); code(connectpointer); codesym(spop());}
#line 2939 "parse.c" /* yacc.c:1646  */
    break;

  case 97:
#line 434 "parse.y" /* yacc.c:1646  */
    {code(nrn_cppp);}
#line 2945 "parse.c" /* yacc.c:1646  */
    break;

  case 98:
#line 435 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-4].inst); code(connect_point_process_pointer);
			 hoc_ob_check(-1);}
#line 2952 "parse.c" /* yacc.c:1646  */
    break;

  case 99:
#line 438 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[0].inst); code(sec_access);}
#line 2958 "parse.c" /* yacc.c:1646  */
    break;

  case 100:
#line 440 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[0].inst); hoc_ob_check(SECTION); code(sec_access_object);}
#line 2964 "parse.c" /* yacc.c:1646  */
    break;

  case 101:
#line 442 "parse.y" /* yacc.c:1646  */
    { Symbol* s = (yyvsp[0].sym);
			(yyval.inst) = Code(mech_access);
			if (s->type != MECHANISM) {
				s = hoc_table_lookup(s->name, hoc_built_in_symlist);
				if (!s || s->type != MECHANISM) {
					acterror((yyvsp[0].sym)->name, "is not a MECHANISM");
				}
			}
			codesym(s);}
#line 2978 "parse.c" /* yacc.c:1646  */
    break;

  case 102:
#line 452 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = Code(mech_uninsert); codesym((yyvsp[0].sym));}
#line 2984 "parse.c" /* yacc.c:1646  */
    break;

  case 103:
#line 454 "parse.y" /* yacc.c:1646  */
    { code(sec_access_pop);}
#line 2990 "parse.c" /* yacc.c:1646  */
    break;

  case 104:
#line 456 "parse.y" /* yacc.c:1646  */
    { code(sec_access_pop); hoc_ob_check(-1);
			insertcode((yyvsp[-1].inst), (yyvsp[0].inst), ob_sec_access);}
#line 2997 "parse.c" /* yacc.c:1646  */
    break;

  case 105:
#line 459 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; TPD; TPD; code(range_interpolate); codesym(spop());
		  codei((yyvsp[-3].narg));
		}
#line 3005 "parse.c" /* yacc.c:1646  */
    break;

  case 106:
#line 463 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(range_interpolate_single); codesym(spop());
		  codei((yyvsp[-1].narg));
		}
#line 3013 "parse.c" /* yacc.c:1646  */
    break;

  case 107:
#line 467 "parse.y" /* yacc.c:1646  */
    {Symbol *s; code(varpush); codesym(s = spop());
		 hoc_obvar_declare(s, VAR, 1);
		 (yyval.inst) = code(for_segment); codei(0); codei(0);}
#line 3021 "parse.c" /* yacc.c:1646  */
    break;

  case 108:
#line 471 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-4].inst); ((yyvsp[-2].inst))[1].i = relative((yyvsp[-1].inst), (yyvsp[-2].inst), 1); /* body */
			((yyvsp[-2].inst))[2].i = relative((yyvsp[0].inst), (yyvsp[-2].inst), 2); /* exit */
		}
#line 3029 "parse.c" /* yacc.c:1646  */
    break;

  case 109:
#line 475 "parse.y" /* yacc.c:1646  */
    {Symbol *s; code(varpush); codesym(s = spop());
		hoc_obvar_declare(s, VAR, 1);}
#line 3036 "parse.c" /* yacc.c:1646  */
    break;

  case 110:
#line 478 "parse.y" /* yacc.c:1646  */
    { TPD; (yyval.inst) = code(for_segment1); codei(0); codei(0);}
#line 3042 "parse.c" /* yacc.c:1646  */
    break;

  case 111:
#line 480 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-7].inst); ((yyvsp[-2].inst))[1].i = relative((yyvsp[-1].inst), (yyvsp[-2].inst), 1); /* body */
			((yyvsp[-2].inst))[2].i = relative((yyvsp[0].inst), (yyvsp[-2].inst), 2); /* exit */
		}
#line 3050 "parse.c" /* yacc.c:1646  */
    break;

  case 112:
#line 484 "parse.y" /* yacc.c:1646  */
    {	code(hoc_push_string); codesym((Symbol*)0);
			(yyval.inst) = code(forall_section); codei(0); codei(0);}
#line 3057 "parse.c" /* yacc.c:1646  */
    break;

  case 113:
#line 487 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-3].inst); ((yyvsp[-2].inst))[1].i = relative((yyvsp[-1].inst), (yyvsp[-2].inst), 1); /* body */
			((yyvsp[-2].inst))[2].i = relative((yyvsp[0].inst), (yyvsp[-2].inst), 2); /* exit */
		}
#line 3065 "parse.c" /* yacc.c:1646  */
    break;

  case 114:
#line 491 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = code(forall_section); codei(0); codei(0);}
#line 3071 "parse.c" /* yacc.c:1646  */
    break;

  case 115:
#line 493 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-4].inst); ((yyvsp[-2].inst))[1].i = relative((yyvsp[-1].inst), (yyvsp[-2].inst), 1); /* body */
			((yyvsp[-2].inst))[2].i = relative((yyvsp[0].inst), (yyvsp[-2].inst), 2); /* exit */
		}
#line 3079 "parse.c" /* yacc.c:1646  */
    break;

  case 116:
#line 497 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = code(hoc_ifsec); codei(0); codei(0);}
#line 3085 "parse.c" /* yacc.c:1646  */
    break;

  case 117:
#line 499 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-4].inst); ((yyvsp[-2].inst))[1].i = relative((yyvsp[-1].inst), (yyvsp[-2].inst), 1); /* body */
			((yyvsp[-2].inst))[2].i = relative((yyvsp[0].inst), (yyvsp[-2].inst), 2); /* exit */
		}
#line 3093 "parse.c" /* yacc.c:1646  */
    break;

  case 118:
#line 503 "parse.y" /* yacc.c:1646  */
    {hoc_ob_check(-1);
		(yyval.inst) = code(forall_sectionlist); codei(0); codei(0); }
#line 3100 "parse.c" /* yacc.c:1646  */
    break;

  case 119:
#line 506 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-4].inst); ((yyvsp[-2].inst))[1].i = relative((yyvsp[-1].inst), (yyvsp[-2].inst), 1); /* body */
			((yyvsp[-2].inst))[2].i = relative((yyvsp[0].inst), (yyvsp[-2].inst), 2); /* exit */
		}
#line 3108 "parse.c" /* yacc.c:1646  */
    break;

  case 120:
#line 510 "parse.y" /* yacc.c:1646  */
    { hoc_ob_check(-1);
		 (yyval.inst) = code(hoc_ifseclist); codei(0); codei(0);
		}
#line 3116 "parse.c" /* yacc.c:1646  */
    break;

  case 121:
#line 515 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-4].inst); ((yyvsp[-2].inst))[1].i = relative((yyvsp[-1].inst), (yyvsp[-2].inst), 1); /* body */
			((yyvsp[-2].inst))[2].i = relative((yyvsp[0].inst), (yyvsp[-2].inst), 2); /* exit */
		}
#line 3124 "parse.c" /* yacc.c:1646  */
    break;

  case 122:
#line 522 "parse.y" /* yacc.c:1646  */
    {
		  code(hoc_iterator); codesym((yyvsp[-3].sym)); codei((yyvsp[-1].narg));
		  (yyval.inst) = progp; codein(STOP); codein(STOP);
		}
#line 3133 "parse.c" /* yacc.c:1646  */
    break;

  case 123:
#line 526 "parse.y" /* yacc.c:1646  */
    {code(hoc_push_current_object);}
#line 3139 "parse.c" /* yacc.c:1646  */
    break;

  case 124:
#line 527 "parse.y" /* yacc.c:1646  */
    {codei(ITERATOR);
		  (yyval.inst) = progp; codein(STOP); codein(STOP);
		}
#line 3147 "parse.c" /* yacc.c:1646  */
    break;

  case 125:
#line 532 "parse.y" /* yacc.c:1646  */
    {pushs((yyvsp[0].sym)); pushi(CHECK);}
#line 3153 "parse.c" /* yacc.c:1646  */
    break;

  case 126:
#line 533 "parse.y" /* yacc.c:1646  */
    {code(sec_access_push); codesym(spop()); (yyval.inst) = (yyvsp[0].inst);}
#line 3159 "parse.c" /* yacc.c:1646  */
    break;

  case 127:
#line 535 "parse.y" /* yacc.c:1646  */
    {
		  (yyval.inst) = code(hoc_sec_internal_push);
		  hoc_codeptr((yyvsp[0].ptr));
		}
#line 3168 "parse.c" /* yacc.c:1646  */
    break;

  case 128:
#line 540 "parse.y" /* yacc.c:1646  */
    {
		  nrn_parsing_pysec_ = NULL;
		  (yyval.inst) = code(hoc_sec_internal_push);
		  hoc_codeptr((yyvsp[0].ptr));
		}
#line 3178 "parse.c" /* yacc.c:1646  */
    break;

  case 129:
#line 548 "parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[0].ptr); }
#line 3184 "parse.c" /* yacc.c:1646  */
    break;

  case 131:
#line 553 "parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[0].ptr); }
#line 3190 "parse.c" /* yacc.c:1646  */
    break;

  case 132:
#line 556 "parse.y" /* yacc.c:1646  */
    {TPD;}
#line 3196 "parse.c" /* yacc.c:1646  */
    break;

  case 133:
#line 557 "parse.y" /* yacc.c:1646  */
    {(yyval.inst) = progp; code(connect_obsec_syntax);}
#line 3202 "parse.c" /* yacc.c:1646  */
    break;

  case 134:
#line 558 "parse.y" /* yacc.c:1646  */
    {
#if 0
		 acterror("Sorry. The \"connect ob.sec...\" syntax ",
			"is not implemented");
#endif
		 hoc_ob_check(SECTION); code(ob_sec_access);
		}
#line 3214 "parse.c" /* yacc.c:1646  */
    break;

  case 135:
#line 568 "parse.y" /* yacc.c:1646  */
    { code(add_section); codesym((yyvsp[0].sym)); codei(0);}
#line 3220 "parse.c" /* yacc.c:1646  */
    break;

  case 136:
#line 570 "parse.y" /* yacc.c:1646  */
    { code(add_section); codesym((yyvsp[-1].sym)); codei((yyvsp[0].narg));}
#line 3226 "parse.c" /* yacc.c:1646  */
    break;

  case 137:
#line 572 "parse.y" /* yacc.c:1646  */
    {code(add_section); codesym((yyvsp[0].sym)); codei(0);}
#line 3232 "parse.c" /* yacc.c:1646  */
    break;

  case 138:
#line 574 "parse.y" /* yacc.c:1646  */
    { code(add_section); codesym((yyvsp[-1].sym)); codei((yyvsp[0].narg));}
#line 3238 "parse.c" /* yacc.c:1646  */
    break;

  case 139:
#line 577 "parse.y" /* yacc.c:1646  */
    {
			Symbol* s;
			s = hoc_decl((yyvsp[0].sym));
			if (s->type != UNDEF && s->type != SECTION)
				acterror(s->name, " already declared");
		}
#line 3249 "parse.c" /* yacc.c:1646  */
    break;

  case 140:
#line 585 "parse.y" /* yacc.c:1646  */
    { code(sec_access_push); codesym((Symbol *)0);}
#line 3255 "parse.c" /* yacc.c:1646  */
    break;

  case 142:
#line 588 "parse.y" /* yacc.c:1646  */
    {pushs((yyvsp[0].sym)); pushi(CHECK);}
#line 3261 "parse.c" /* yacc.c:1646  */
    break;

  case 143:
#line 589 "parse.y" /* yacc.c:1646  */
    {(yyval.inst) = (yyvsp[0].inst);}
#line 3267 "parse.c" /* yacc.c:1646  */
    break;

  case 144:
#line 592 "parse.y" /* yacc.c:1646  */
    { code3(varpush, spop(), hoc_evalpointer);}
#line 3273 "parse.c" /* yacc.c:1646  */
    break;

  case 145:
#line 594 "parse.y" /* yacc.c:1646  */
    { TPD; code(rangevarevalpointer); codesym(spop());}
#line 3279 "parse.c" /* yacc.c:1646  */
    break;

  case 146:
#line 596 "parse.y" /* yacc.c:1646  */
    {hoc_ipop(); code(hoc_ob_pointer);}
#line 3285 "parse.c" /* yacc.c:1646  */
    break;

  case 147:
#line 597 "parse.y" /* yacc.c:1646  */
    {myerr("rangevariable needs explicit arc position,eg. v(.5)");}
#line 3291 "parse.c" /* yacc.c:1646  */
    break;

  case 148:
#line 599 "parse.y" /* yacc.c:1646  */
    {(yyval.inst) = argcode(hoc_argrefarg, (yyvsp[0].narg));}
#line 3297 "parse.c" /* yacc.c:1646  */
    break;

  case 149:
#line 605 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-1].inst);}
#line 3303 "parse.c" /* yacc.c:1646  */
    break;

  case 150:
#line 607 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = progp; }
#line 3309 "parse.c" /* yacc.c:1646  */
    break;

  case 151:
#line 610 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = code(forcode); codei(0); codei(0); codei(0); }
#line 3315 "parse.c" /* yacc.c:1646  */
    break;

  case 152:
#line 613 "parse.y" /* yacc.c:1646  */
    { TPD; (yyval.inst) = (yyvsp[0].inst); codein(STOP);}
#line 3321 "parse.c" /* yacc.c:1646  */
    break;

  case 153:
#line 616 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-1].inst); codein(STOP);}
#line 3327 "parse.c" /* yacc.c:1646  */
    break;

  case 154:
#line 618 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = progp; codein(STOP);}
#line 3333 "parse.c" /* yacc.c:1646  */
    break;

  case 155:
#line 621 "parse.y" /* yacc.c:1646  */
    { TPD; codein(STOP); (yyval.inst) = (yyvsp[-1].inst);}
#line 3339 "parse.c" /* yacc.c:1646  */
    break;

  case 156:
#line 624 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[0].inst); }
#line 3345 "parse.c" /* yacc.c:1646  */
    break;

  case 157:
#line 627 "parse.y" /* yacc.c:1646  */
    { (yyval.inst)=code(ifcode); codei(0); codei(0); codei(0); }
#line 3351 "parse.c" /* yacc.c:1646  */
    break;

  case 158:
#line 630 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = progp; }
#line 3357 "parse.c" /* yacc.c:1646  */
    break;

  case 159:
#line 633 "parse.y" /* yacc.c:1646  */
    { codein(STOP); (yyval.inst) = progp; }
#line 3363 "parse.c" /* yacc.c:1646  */
    break;

  case 160:
#line 636 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = progp; prog_error = (yyval.inst); }
#line 3369 "parse.c" /* yacc.c:1646  */
    break;

  case 161:
#line 638 "parse.y" /* yacc.c:1646  */
    {
			prog_parse_recover = progp;
			prog_error = (yyval.inst);
			if (fin && nrn_fw_eq(fin, stdin) && !pipeflag)
			{	int i;
				Printf(">");
				for (i = 0; i < ntab; i++)
					Printf("	");
			}
		}
#line 3384 "parse.c" /* yacc.c:1646  */
    break;

  case 163:
#line 650 "parse.y" /* yacc.c:1646  */
    {myerr("syntax error in compound statement");}
#line 3390 "parse.c" /* yacc.c:1646  */
    break;

  case 164:
#line 655 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = code(constpush); codesym((yyvsp[0].sym)); PN;}
#line 3396 "parse.c" /* yacc.c:1646  */
    break;

  case 165:
#line 657 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = code(pushzero); PN;}
#line 3402 "parse.c" /* yacc.c:1646  */
    break;

  case 166:
#line 659 "parse.y" /* yacc.c:1646  */
    { code3(varpush, spop(), eval); PN;}
#line 3408 "parse.c" /* yacc.c:1646  */
    break;

  case 167:
#line 661 "parse.y" /* yacc.c:1646  */
    { defnonly("$"); (yyval.inst) = argcode(arg, (yyvsp[0].narg)); PN;}
#line 3414 "parse.c" /* yacc.c:1646  */
    break;

  case 168:
#line 663 "parse.y" /* yacc.c:1646  */
    { defnonly("$&"); (yyval.inst) = argrefcode(hoc_argref, (yyvsp[-1].narg), (yyvsp[0].narg)); PN;}
#line 3420 "parse.c" /* yacc.c:1646  */
    break;

  case 169:
#line 666 "parse.y" /* yacc.c:1646  */
    {code(rangepoint); codesym(spop()); PN;}
#line 3426 "parse.c" /* yacc.c:1646  */
    break;

  case 170:
#line 668 "parse.y" /* yacc.c:1646  */
    { TPD; code(rangevareval); codesym(spop()); PN;}
#line 3432 "parse.c" /* yacc.c:1646  */
    break;

  case 171:
#line 672 "parse.y" /* yacc.c:1646  */
    {code(hoc_object_eval);}
#line 3438 "parse.c" /* yacc.c:1646  */
    break;

  case 173:
#line 676 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-3].inst); code(call); codesym((yyvsp[-4].sym)); codei((yyvsp[-1].narg)); PN;}
#line 3444 "parse.c" /* yacc.c:1646  */
    break;

  case 174:
#line 678 "parse.y" /* yacc.c:1646  */
    { code(call); codesym(spop()); codei((yyvsp[-1].narg)); PN;}
#line 3450 "parse.c" /* yacc.c:1646  */
    break;

  case 175:
#line 680 "parse.y" /* yacc.c:1646  */
    { (yyval.inst)=(yyvsp[-1].inst); code(varread); codesym(spop()); PN;}
#line 3456 "parse.c" /* yacc.c:1646  */
    break;

  case 176:
#line 682 "parse.y" /* yacc.c:1646  */
    { TPD; (yyval.inst) = (yyvsp[-1].inst); code(bltin); codesym((yyvsp[-3].sym)); PN;}
#line 3462 "parse.c" /* yacc.c:1646  */
    break;

  case 177:
#line 684 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-1].inst); }
#line 3468 "parse.c" /* yacc.c:1646  */
    break;

  case 178:
#line 686 "parse.y" /* yacc.c:1646  */
    {myerr("syntax error in expression");}
#line 3474 "parse.c" /* yacc.c:1646  */
    break;

  case 179:
#line 688 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(add); PN; }
#line 3480 "parse.c" /* yacc.c:1646  */
    break;

  case 180:
#line 690 "parse.y" /* yacc.c:1646  */
    { TPD; TPD;code(hoc_sub); PN;}
#line 3486 "parse.c" /* yacc.c:1646  */
    break;

  case 181:
#line 692 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(mul); PN;}
#line 3492 "parse.c" /* yacc.c:1646  */
    break;

  case 182:
#line 694 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(hoc_div); PN;}
#line 3498 "parse.c" /* yacc.c:1646  */
    break;

  case 183:
#line 696 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(hoc_cyclic); PN;}
#line 3504 "parse.c" /* yacc.c:1646  */
    break;

  case 184:
#line 698 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(power); PN;}
#line 3510 "parse.c" /* yacc.c:1646  */
    break;

  case 185:
#line 700 "parse.y" /* yacc.c:1646  */
    { TPD; (yyval.inst) = (yyvsp[0].inst); code(negate); PN;}
#line 3516 "parse.c" /* yacc.c:1646  */
    break;

  case 186:
#line 702 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(gt); PN;}
#line 3522 "parse.c" /* yacc.c:1646  */
    break;

  case 187:
#line 704 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(ge); PN;}
#line 3528 "parse.c" /* yacc.c:1646  */
    break;

  case 188:
#line 706 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(lt); PN;}
#line 3534 "parse.c" /* yacc.c:1646  */
    break;

  case 189:
#line 708 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(le); PN;}
#line 3540 "parse.c" /* yacc.c:1646  */
    break;

  case 190:
#line 710 "parse.y" /* yacc.c:1646  */
    { hoc_ob_check(-1); hoc_ob_check(-1); code(eq); PN;}
#line 3546 "parse.c" /* yacc.c:1646  */
    break;

  case 191:
#line 712 "parse.y" /* yacc.c:1646  */
    { hoc_ob_check(-1); hoc_ob_check(-1); code(ne); PN;}
#line 3552 "parse.c" /* yacc.c:1646  */
    break;

  case 192:
#line 714 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(hoc_and); PN;}
#line 3558 "parse.c" /* yacc.c:1646  */
    break;

  case 193:
#line 716 "parse.y" /* yacc.c:1646  */
    { TPD; TPD; code(hoc_or); PN;}
#line 3564 "parse.c" /* yacc.c:1646  */
    break;

  case 194:
#line 718 "parse.y" /* yacc.c:1646  */
    { TPD; (yyval.inst) = (yyvsp[0].inst); code(hoc_not); PN;}
#line 3570 "parse.c" /* yacc.c:1646  */
    break;

  case 199:
#line 726 "parse.y" /* yacc.c:1646  */
    {Symbol *s; code(varpush); codesym(s=spop()); (yyval.inst) = (yyvsp[-1].inst);
		code(arayinstal); codei((yyvsp[0].narg)); hoc_obvar_declare(s, VAR, 0);}
#line 3577 "parse.c" /* yacc.c:1646  */
    break;

  case 200:
#line 729 "parse.y" /* yacc.c:1646  */
    {Symbol *s; code(varpush); codesym(s = spop());
		code(arayinstal); codei((yyvsp[0].narg)); hoc_obvar_declare(s, VAR, 0);}
#line 3584 "parse.c" /* yacc.c:1646  */
    break;

  case 201:
#line 734 "parse.y" /* yacc.c:1646  */
    {pushs((yyvsp[-1].sym)); (yyval.narg) = (yyvsp[0].narg);}
#line 3590 "parse.c" /* yacc.c:1646  */
    break;

  case 202:
#line 737 "parse.y" /* yacc.c:1646  */
    {  TPD; (yyval.narg) = 1; }
#line 3596 "parse.c" /* yacc.c:1646  */
    break;

  case 203:
#line 739 "parse.y" /* yacc.c:1646  */
    {  TPD;(yyval.narg) = (yyval.narg) + 1; }
#line 3602 "parse.c" /* yacc.c:1646  */
    break;

  case 205:
#line 745 "parse.y" /* yacc.c:1646  */
    { pushs((yyvsp[-1].sym)); (yyval.inst) = (yyvsp[0].inst); }
#line 3608 "parse.c" /* yacc.c:1646  */
    break;

  case 206:
#line 747 "parse.y" /* yacc.c:1646  */
    { if ((yyvsp[-1].sym)->subtype == USERPROPERTY) {
			code(sec_access_push); codesym((Symbol *)0);
		  }
		pushs((yyvsp[-1].sym)); pushi(CHECK);
		}
#line 3618 "parse.c" /* yacc.c:1646  */
    break;

  case 207:
#line 752 "parse.y" /* yacc.c:1646  */
    {(yyval.inst) = (yyvsp[-2].inst);}
#line 3624 "parse.c" /* yacc.c:1646  */
    break;

  case 208:
#line 754 "parse.y" /* yacc.c:1646  */
    { if ((yyvsp[0].sym)->subtype != USERPROPERTY) {
			acterror((yyvsp[0].sym)->name, "not a section variable");
		  }
		(yyval.inst) = (yyvsp[-2].inst); pushs((yyvsp[0].sym));
		}
#line 3634 "parse.c" /* yacc.c:1646  */
    break;

  case 209:
#line 768 "parse.y" /* yacc.c:1646  */
    {
		int d1, chk;
		Symbol *sym;
		d1 = ipop();
		chk = ipop();
		sym = spop();
   if (chk) {
	if (!ISARRAY(sym)) {
		if (d1)
			acterror(sym->name, "not an array variable");
	}else{
		if ( d1 == 0 ) { /*fake index list with all 0's*/
			int i;
			for (i=0; i<sym->arayinfo->nsub; i++) {
				code(pushzero);
			}
		}			
		else if ( d1 != sym->arayinfo->nsub) {
			acterror("wrong # of subscripts",sym->name);
		}
	}
   }else {
	pushi(d1); /* must check dynamically */
   }
		pushs(sym);
		}
#line 3665 "parse.c" /* yacc.c:1646  */
    break;

  case 210:
#line 797 "parse.y" /* yacc.c:1646  */
    {
			(yyval.narg) = ipop();
		}
#line 3673 "parse.c" /* yacc.c:1646  */
    break;

  case 211:
#line 803 "parse.y" /* yacc.c:1646  */
    { pushi(0); }
#line 3679 "parse.c" /* yacc.c:1646  */
    break;

  case 212:
#line 805 "parse.y" /* yacc.c:1646  */
    {  TPD;pushi(ipop() + 1); }
#line 3685 "parse.c" /* yacc.c:1646  */
    break;

  case 213:
#line 809 "parse.y" /* yacc.c:1646  */
    { TPDYNAM; code(prexpr);}
#line 3691 "parse.c" /* yacc.c:1646  */
    break;

  case 214:
#line 811 "parse.y" /* yacc.c:1646  */
    { code(prstr); }
#line 3697 "parse.c" /* yacc.c:1646  */
    break;

  case 215:
#line 813 "parse.y" /* yacc.c:1646  */
    { TPDYNAM; code(prexpr);}
#line 3703 "parse.c" /* yacc.c:1646  */
    break;

  case 216:
#line 815 "parse.y" /* yacc.c:1646  */
    { code(prstr); }
#line 3709 "parse.c" /* yacc.c:1646  */
    break;

  case 217:
#line 818 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = code(hoc_delete_symbol); codesym((yyvsp[0].sym)); }
#line 3715 "parse.c" /* yacc.c:1646  */
    break;

  case 218:
#line 822 "parse.y" /* yacc.c:1646  */
    {(yyvsp[0].sym)->type=FUNCTION; indef=1; }
#line 3721 "parse.c" /* yacc.c:1646  */
    break;

  case 219:
#line 824 "parse.y" /* yacc.c:1646  */
    { code(procret); hoc_define((yyvsp[-4].sym));
		 (yyvsp[-4].sym)->u.u_proc->nobjauto = (yyvsp[0].narg) - localcnt;
		 (yyvsp[-4].sym)->u.u_proc->nauto=(yyvsp[0].narg); indef=0; }
#line 3729 "parse.c" /* yacc.c:1646  */
    break;

  case 220:
#line 828 "parse.y" /* yacc.c:1646  */
    { (yyvsp[0].sym)->type=PROCEDURE; indef=1; }
#line 3735 "parse.c" /* yacc.c:1646  */
    break;

  case 221:
#line 830 "parse.y" /* yacc.c:1646  */
    { code(procret); hoc_define((yyvsp[-4].sym));
		 (yyvsp[-4].sym)->u.u_proc->nobjauto = (yyvsp[0].narg) - localcnt;
		 (yyvsp[-4].sym)->u.u_proc->nauto=(yyvsp[0].narg); indef=0; }
#line 3743 "parse.c" /* yacc.c:1646  */
    break;

  case 222:
#line 834 "parse.y" /* yacc.c:1646  */
    { (yyvsp[0].sym)->type = ITERATOR; indef=2; }
#line 3749 "parse.c" /* yacc.c:1646  */
    break;

  case 223:
#line 836 "parse.y" /* yacc.c:1646  */
    {code(procret); hoc_define((yyvsp[-4].sym));
		 (yyvsp[-4].sym)->u.u_proc->nobjauto = (yyvsp[0].narg) - localcnt;
		 (yyvsp[-4].sym)->u.u_proc->nauto = (yyvsp[0].narg); indef = 0; }
#line 3757 "parse.c" /* yacc.c:1646  */
    break;

  case 224:
#line 840 "parse.y" /* yacc.c:1646  */
    { (yyvsp[0].sym)->type=HOCOBJFUNCTION; indef=3; }
#line 3763 "parse.c" /* yacc.c:1646  */
    break;

  case 225:
#line 842 "parse.y" /* yacc.c:1646  */
    { code(procret); hoc_define((yyvsp[-4].sym));
		 (yyvsp[-4].sym)->u.u_proc->nobjauto = (yyvsp[0].narg) - localcnt;
		 (yyvsp[-4].sym)->u.u_proc->nauto=(yyvsp[0].narg); indef=0; }
#line 3771 "parse.c" /* yacc.c:1646  */
    break;

  case 226:
#line 847 "parse.y" /* yacc.c:1646  */
    { Symbol *s; s=yylval.sym;
		if(s->type != UNDEF) acterror(s->name, "already defined");
		s->u.u_proc = (Proc *)emalloc(sizeof(Proc));
		s->u.u_proc->defn.in = STOP;
		s->u.u_proc->list = (Symlist *)0; }
#line 3781 "parse.c" /* yacc.c:1646  */
    break;

  case 231:
#line 857 "parse.y" /* yacc.c:1646  */
    {ntab++;}
#line 3787 "parse.c" /* yacc.c:1646  */
    break;

  case 232:
#line 858 "parse.y" /* yacc.c:1646  */
    {
		ntab--;
		(yyval.narg) = (yyvsp[-4].narg) + (yyvsp[-3].narg);
		}
#line 3796 "parse.c" /* yacc.c:1646  */
    break;

  case 233:
#line 864 "parse.y" /* yacc.c:1646  */
    { (yyval.narg) = 0; }
#line 3802 "parse.c" /* yacc.c:1646  */
    break;

  case 235:
#line 868 "parse.y" /* yacc.c:1646  */
    {(yyval.narg) = 1;}
#line 3808 "parse.c" /* yacc.c:1646  */
    break;

  case 236:
#line 870 "parse.y" /* yacc.c:1646  */
    {(yyval.narg) = (yyvsp[-2].narg) + 1;}
#line 3814 "parse.c" /* yacc.c:1646  */
    break;

  case 237:
#line 873 "parse.y" /* yacc.c:1646  */
    {}
#line 3820 "parse.c" /* yacc.c:1646  */
    break;

  case 238:
#line 875 "parse.y" /* yacc.c:1646  */
    { hoc_ob_check(-1);}
#line 3826 "parse.c" /* yacc.c:1646  */
    break;

  case 240:
#line 878 "parse.y" /* yacc.c:1646  */
    {
		 code(hoc_newobj_arg); codesym(hoc_which_template((yyvsp[-3].sym))); codei((yyvsp[-1].narg));
		}
#line 3834 "parse.c" /* yacc.c:1646  */
    break;

  case 241:
#line 883 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[0].inst); }
#line 3840 "parse.c" /* yacc.c:1646  */
    break;

  case 242:
#line 885 "parse.y" /* yacc.c:1646  */
    {code3(varpush, spop(), eqn_name);
		do_equation = 1; }
#line 3847 "parse.c" /* yacc.c:1646  */
    break;

  case 243:
#line 888 "parse.y" /* yacc.c:1646  */
    { (yyval.inst) = (yyvsp[-3].inst); do_equation = 0; }
#line 3853 "parse.c" /* yacc.c:1646  */
    break;

  case 244:
#line 891 "parse.y" /* yacc.c:1646  */
    {Symbol *s; code3(varpush,s= spop(), dep_make); hoc_obvar_declare(s, VAR, 0);}
#line 3859 "parse.c" /* yacc.c:1646  */
    break;

  case 245:
#line 893 "parse.y" /* yacc.c:1646  */
    {Symbol *s; code3(varpush, s=spop(), dep_make); hoc_obvar_declare(s, VAR, 0);}
#line 3865 "parse.c" /* yacc.c:1646  */
    break;

  case 248:
#line 898 "parse.y" /* yacc.c:1646  */
    { code(eqn_init); }
#line 3871 "parse.c" /* yacc.c:1646  */
    break;

  case 250:
#line 902 "parse.y" /* yacc.c:1646  */
    { code(eqn_lhs); }
#line 3877 "parse.c" /* yacc.c:1646  */
    break;

  case 251:
#line 904 "parse.y" /* yacc.c:1646  */
    { codein(STOP); TPD; }
#line 3883 "parse.c" /* yacc.c:1646  */
    break;

  case 252:
#line 907 "parse.y" /* yacc.c:1646  */
    { code(eqn_rhs); }
#line 3889 "parse.c" /* yacc.c:1646  */
    break;

  case 253:
#line 909 "parse.y" /* yacc.c:1646  */
    { codein(STOP); TPD; }
#line 3895 "parse.c" /* yacc.c:1646  */
    break;

  case 254:
#line 912 "parse.y" /* yacc.c:1646  */
    { (yyval.narg) = 0; localcnt = (yyval.narg);}
#line 3901 "parse.c" /* yacc.c:1646  */
    break;

  case 256:
#line 916 "parse.y" /* yacc.c:1646  */
    {
		Symbol *sp;
		(yyval.narg) = 1; localcnt = (yyval.narg);
		sp = install((yyvsp[0].sym)->name, AUTO, 0.0, &p_symlist);
		sp->u.u_auto = (yyval.narg);
		}
#line 3912 "parse.c" /* yacc.c:1646  */
    break;

  case 257:
#line 923 "parse.y" /* yacc.c:1646  */
    {
		Symbol *sp;
		(yyval.narg) = (yyvsp[-2].narg) + 1; localcnt = (yyval.narg);
		if (hoc_table_lookup((yyvsp[0].sym)->name, p_symlist)) {
			acterror((yyvsp[0].sym)->name, "already declared local");
		}
		sp = install((yyvsp[0].sym)->name, AUTO, 0.0, &p_symlist);
		sp->u.u_auto = (yyval.narg);
		}
#line 3926 "parse.c" /* yacc.c:1646  */
    break;

  case 258:
#line 934 "parse.y" /* yacc.c:1646  */
    { (yyval.narg) = 0;}
#line 3932 "parse.c" /* yacc.c:1646  */
    break;

  case 260:
#line 938 "parse.y" /* yacc.c:1646  */
    {
		Symbol *sp;
		(yyval.narg) = 1;
		if (hoc_table_lookup((yyvsp[0].sym)->name, p_symlist)) {
			acterror((yyvsp[0].sym)->name, "already declared local");
		}
		sp = install((yyvsp[0].sym)->name, AUTOOBJ, 0.0, &p_symlist);
		sp->u.u_auto = (yyval.narg) + localcnt;
		}
#line 3946 "parse.c" /* yacc.c:1646  */
    break;

  case 261:
#line 948 "parse.y" /* yacc.c:1646  */
    {
		Symbol *sp;
		(yyval.narg) = (yyvsp[-2].narg) + 1;
		if (hoc_table_lookup((yyvsp[0].sym)->name, p_symlist)) {
			acterror((yyvsp[0].sym)->name, "already declared local");
		}
		sp = install((yyvsp[0].sym)->name, AUTOOBJ, 0.0, &p_symlist);
		sp->u.u_auto = (yyval.narg) + localcnt;
		}
#line 3960 "parse.c" /* yacc.c:1646  */
    break;

  case 262:
#line 959 "parse.y" /* yacc.c:1646  */
    {  Symbol* s;
	   s = hoc_decl((yyvsp[0].sym));
	   if (s->subtype != NOTUSER)
		acterror("can't redeclare user variable", s->name);
	   (yyval.sym) = s;
	}
#line 3971 "parse.c" /* yacc.c:1646  */
    break;


#line 3975 "parse.c" /* yacc.c:1646  */
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
#line 971 "parse.y" /* yacc.c:1906  */

	/* end of grammar */

static void yyerror(const char* s)	/* called for yacc syntax error */
{
	execerror(s, (char *)0);
}

void acterror(const char* s, const char*t)	/* recover from action error while parsing */
{
	execerror(s,t);
}

static Inst* argrefcode(Pfrv pfrv, int i, int j){
	Inst* in;
	in = argcode(pfrv, i);
	codei(j);
	return in;
}

static Inst* argcode(Pfrv pfrv, int i) {
	Inst* in;
	if (i == 0) {
		Symbol* si = hoc_lookup("i");
		if (si->type != AUTO) {
			acterror("arg index used and i is not a LOCAL variable", 0);
		}
		in = code3(varpush, si, eval);		
		Code(pfrv);
		codei(0);
	}else{
		in = Code(pfrv);
		codei(i);
	}
	return in;
}

static void hoc_opasgn_invalid(int op) {
        if (op) {
                acterror("Invalid assignment operator.", "Only '=' allowed. ");
        }
} 

