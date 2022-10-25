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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         HANA_STYPE
#define YYLTYPE         HANA_LTYPE
/* Substitute the variable and function names.  */
#define yyparse         hana_parse
#define yylex           hana_lex
#define yyerror         hana_error
#define yydebug         hana_debug
#define yynerrs         hana_nerrs


/* Copy the first part of user declarations.  */
#line 1 "sqlparser_hana.yacc" /* yacc.c:339  */

/**
 * This Grammar is designed for hana.
 * https://github.com/Raphael2017/SQL/blob/master/sql-2003-2.bnf
 * sqlparser.y
 * defines sqlparser_hana_bison.h
 * outputs sqlparser_hana_bison.cpp
 *
 * Bison Grammar File Spec: http://dinosaur.compilertools.net/bison/bison_6.html
 *
 */
/*********************************
 ** Section 1: C Declarations
 *********************************/

#include "sqlparser_hana_bison.h"
#include "sqlparser_hana_flex.h"
#include "serialize_format.h"

#include <stdio.h>
#include <string.h>
//#include <strings.h>

/*
 * We provide parse error includes error message, first line, first column of error lex for debug
 */
int yyerror(YYLTYPE* llocp, ParseResult* result, yyscan_t scanner, const char *msg) {
    result->accept = false;
    result->errFirstLine = llocp->first_line;
    result->errFirstColumn = llocp->first_column;
    result->errDetail = msg;
	return 0;
}

#define YYSTYPE         HANA_STYPE
#define YYLTYPE         HANA_LTYPE


#line 113 "sqlparser_hana_bison.cpp" /* yacc.c:339  */

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
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "sqlparser_hana_bison.h".  */
#ifndef YY_HANA_SQLPARSER_HANA_BISON_H_INCLUDED
# define YY_HANA_SQLPARSER_HANA_BISON_H_INCLUDED
/* Debug traces.  */
#ifndef HANA_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define HANA_DEBUG 1
#  else
#   define HANA_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define HANA_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined HANA_DEBUG */
#if HANA_DEBUG
extern int hana_debug;
#endif
/* "%code requires" blocks.  */
#line 45 "sqlparser_hana.yacc" /* yacc.c:355  */

// %code requires block

#include "node.h"

#line 157 "sqlparser_hana_bison.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef HANA_TOKENTYPE
# define HANA_TOKENTYPE
  enum hana_tokentype
  {
    HANA_NAME = 258,
    HANA_STRING = 259,
    HANA_INTNUM = 260,
    HANA_BOOL = 261,
    HANA_APPROXNUM = 262,
    HANA_NULLX = 263,
    HANA_UNKNOWN = 264,
    HANA_QUESTIONMARK = 265,
    HANA_UMINUS = 266,
    HANA_ALL = 267,
    HANA_AND = 268,
    HANA_ANY = 269,
    HANA_ARRAY = 270,
    HANA_AS = 271,
    HANA_ASC = 272,
    HANA_AVG = 273,
    HANA_BETWEEN = 274,
    HANA_BIGINT = 275,
    HANA_BINARY = 276,
    HANA_BLOB = 277,
    HANA_BOOLEAN = 278,
    HANA_BY = 279,
    HANA_CALL = 280,
    HANA_CASE = 281,
    HANA_CAST = 282,
    HANA_CHAR = 283,
    HANA_CHARACTER = 284,
    HANA_CHARACTERS = 285,
    HANA_CLOB = 286,
    HANA_CNNOP = 287,
    HANA_COALESCE = 288,
    HANA_CODE_UNITS = 289,
    HANA_COLLATE = 290,
    HANA_COMP_EQ = 291,
    HANA_COMP_GE = 292,
    HANA_COMP_GT = 293,
    HANA_COMP_LE = 294,
    HANA_COMP_LT = 295,
    HANA_COMP_NE = 296,
    HANA_CONVERT = 297,
    HANA_CORRESPONDING = 298,
    HANA_COUNT = 299,
    HANA_CROSS = 300,
    HANA_CUME_DIST = 301,
    HANA_CURRENT = 302,
    HANA_CURRENT_TIMESTAMP = 303,
    HANA_CURRENT_USER = 304,
    HANA_DATE = 305,
    HANA_DAY = 306,
    HANA_DEC = 307,
    HANA_DECIMAL = 308,
    HANA_DEFAULT = 309,
    HANA_DELETE = 310,
    HANA_DENSE_RANK = 311,
    HANA_DESC = 312,
    HANA_DISTINCT = 313,
    HANA_DOUBLE = 314,
    HANA_ELSE = 315,
    HANA_END = 316,
    HANA_END_P = 317,
    HANA_ESCAPE = 318,
    HANA_ERROR = 319,
    HANA_EXCEPT = 320,
    HANA_EXCLUDE = 321,
    HANA_EXISTS = 322,
    HANA_FLOAT = 323,
    HANA_FOLLOWING = 324,
    HANA_FOR = 325,
    HANA_FROM = 326,
    HANA_FULL = 327,
    HANA_G = 328,
    HANA_GROUP = 329,
    HANA_GROUPING = 330,
    HANA_HAVING = 331,
    HANA_HOUR = 332,
    HANA_IN = 333,
    HANA_INNER = 334,
    HANA_INSERT = 335,
    HANA_INT = 336,
    HANA_INTEGER = 337,
    HANA_INTERSECT = 338,
    HANA_INTERVAL = 339,
    HANA_MANY = 340,
    HANA_ONE = 341,
    HANA_EXACT = 342,
    HANA_INTO = 343,
    HANA_IS = 344,
    HANA_JOIN = 345,
    HANA_K = 346,
    HANA_LARGE = 347,
    HANA_LEFT = 348,
    HANA_LIKE = 349,
    HANA_M = 350,
    HANA_MAX = 351,
    HANA_MIN = 352,
    HANA_MINUTE = 353,
    HANA_MOD = 354,
    HANA_MONTH = 355,
    HANA_MULTISET = 356,
    HANA_NATIONAL = 357,
    HANA_NATURAL = 358,
    HANA_NCHAR = 359,
    HANA_NCLOB = 360,
    HANA_NO = 361,
    HANA_NOT = 362,
    HANA_NULLIF = 363,
    HANA_NUMERIC = 364,
    HANA_OBJECT = 365,
    HANA_OCTETS = 366,
    HANA_OF = 367,
    HANA_ON = 368,
    HANA_ONLY = 369,
    HANA_OR = 370,
    HANA_ORDER = 371,
    HANA_OTHERS = 372,
    HANA_OUTER = 373,
    HANA_OVER = 374,
    HANA_PARTITION = 375,
    HANA_PERCENT_RANK = 376,
    HANA_PRECEDING = 377,
    HANA_PRECISION = 378,
    HANA_RANGE = 379,
    HANA_RANK = 380,
    HANA_READ = 381,
    HANA_REAL = 382,
    HANA_RECURSIVE = 383,
    HANA_REF = 384,
    HANA_RIGHT = 385,
    HANA_ROW = 386,
    HANA_ROWS = 387,
    HANA_ROW_NUMBER = 388,
    HANA_SCOPE = 389,
    HANA_SECOND = 390,
    HANA_SELECT = 391,
    HANA_SESSION_USER = 392,
    HANA_SET = 393,
    HANA_SETS = 394,
    HANA_SMALLINT = 395,
    HANA_SOME = 396,
    HANA_STDDEV_POP = 397,
    HANA_STDDEV_SAMP = 398,
    HANA_SUM = 399,
    HANA_SYSTEM_USER = 400,
    HANA_THEN = 401,
    HANA_TIES = 402,
    HANA_TIME = 403,
    HANA_TIMESTAMP = 404,
    HANA_TO = 405,
    HANA_UNBOUNDED = 406,
    HANA_UNION = 407,
    HANA_UPDATE = 408,
    HANA_USING = 409,
    HANA_VALUES = 410,
    HANA_VARCHAR = 411,
    HANA_NVARCHAR = 412,
    HANA_VARYING = 413,
    HANA_VAR_POP = 414,
    HANA_VAR_SAMP = 415,
    HANA_WHEN = 416,
    HANA_WHERE = 417,
    HANA_WITH = 418,
    HANA_WITHOUT = 419,
    HANA_YEAR = 420,
    HANA_ZONE = 421,
    HANA_LIMIT = 422,
    HANA_OFFSET = 423,
    HANA_TOP = 424,
    HANA_WAIT = 425,
    HANA_NOWAIT = 426,
    HANA_HINT = 427,
    HANA_RANGE_RESTRICTION = 428,
    HANA_PARAMETERS = 429,
    HANA_UPSERT = 430,
    HANA_REPLACE = 431,
    HANA_PRIMARY = 432,
    HANA_KEY = 433,
    HANA_CONTAINS = 434,
    HANA_P_POINT = 435,
    HANA_SECONDDATE = 436,
    HANA_TINYINT = 437,
    HANA_SMALLDECIMAL = 438,
    HANA_TEXT = 439,
    HANA_BINTEXT = 440,
    HANA_ALPHANUM = 441,
    HANA_VARBINARY = 442,
    HANA_SHORTTEXT = 443,
    HANA_ORDINALITY = 444,
    HANA_UNNEST = 445,
    HANA_HISTORY = 446,
    HANA_OVERRIDING = 447,
    HANA_USER = 448,
    HANA_SYSTEM = 449,
    HANA_VALUE = 450,
    HANA_NULLS = 451,
    HANA_FIRST = 452,
    HANA_LAST = 453,
    HANA_CORR = 454,
    HANA_CORR_SPEARMAN = 455,
    HANA_MEDIAN = 456,
    HANA_STDDEV = 457,
    HANA_VAR = 458,
    HANA_STRING_AGG = 459,
    HANA_MEMBER = 460,
    HANA_SYSTEM_TIME = 461,
    HANA_APPLICATION_TIME = 462,
    HANA_TABLESAMPLE = 463,
    HANA_BERNOULLI = 464,
    HANA_RETURN = 465,
    HANA_LATERAL = 466,
    HANA_BINNING = 467,
    HANA_LAG = 468,
    HANA_LEAD = 469,
    HANA_RANDOM_PARTITION = 470,
    HANA_WEIGHTED_AVG = 471,
    HANA_EXTRACT = 472,
    HANA_FIRST_VALUE = 473,
    HANA_LAST_VALUE = 474,
    HANA_NTH_VALUE = 475,
    HANA_NTILE = 476,
    HANA_SERIES_FILTER = 477,
    HANA_LOCATE_REGEXPR = 478,
    HANA_OCCURRENCES_REGEXPR = 479,
    HANA_REPLACE_REGEXPR = 480,
    HANA_SUBSTR_REGEXPR = 481,
    HANA_PERCENTILE_CONT = 482,
    HANA_PERCENTILE_DISC = 483,
    HANA_TRIM = 484,
    HANA_WITHIN = 485,
    HANA_ROLLUP = 486,
    HANA_CUBE = 487,
    HANA_BEST = 488,
    HANA_SUBTOTAL = 489,
    HANA_BALANCE = 490,
    HANA_TOTAL = 491,
    HANA_MULTIPLE = 492,
    HANA_RESULTSETS = 493,
    HANA_PREFIX = 494,
    HANA_STRUCTURED = 495,
    HANA_RESULT = 496,
    HANA_OVERVIEW = 497,
    HANA_TEXT_FILTER = 498,
    HANA_FILL = 499,
    HANA_UP = 500,
    HANA_MATCHES = 501,
    HANA_SORT = 502,
    HANA_MINUS = 503,
    HANA_ROWCOUNT = 504,
    HANA_LOCKED = 505,
    HANA_IGNORE = 506,
    HANA_SHARE = 507,
    HANA_LOCK = 508,
    HANA_UTCTIMESTAMP = 509,
    HANA_COMMIT = 510,
    HANA_ID = 511,
    HANA_XML = 512,
    HANA_JSON = 513,
    HANA_RETURNS = 514
  };
#endif

/* Value type.  */
#if ! defined HANA_STYPE && ! defined HANA_STYPE_IS_DECLARED

union HANA_STYPE
{
#line 88 "sqlparser_hana.yacc" /* yacc.c:355  */

    struct Node* node;
    int    ival;
    NodeType nodetype;

#line 435 "sqlparser_hana_bison.cpp" /* yacc.c:355  */
};

typedef union HANA_STYPE HANA_STYPE;
# define HANA_STYPE_IS_TRIVIAL 1
# define HANA_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined HANA_LTYPE && ! defined HANA_LTYPE_IS_DECLARED
typedef struct HANA_LTYPE HANA_LTYPE;
struct HANA_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define HANA_LTYPE_IS_DECLARED 1
# define HANA_LTYPE_IS_TRIVIAL 1
#endif



int hana_parse (ParseResult* result, yyscan_t scanner);

#endif /* !YY_HANA_SQLPARSER_HANA_BISON_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 465 "sqlparser_hana_bison.cpp" /* yacc.c:358  */

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
         || (defined HANA_LTYPE_IS_TRIVIAL && HANA_LTYPE_IS_TRIVIAL \
             && defined HANA_STYPE_IS_TRIVIAL && HANA_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  185
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   17026

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  276
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  213
/* YYNRULES -- Number of rules.  */
#define YYNRULES  676
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1209

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   514

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   272,     2,     2,     2,   275,     2,     2,
      12,    13,   265,   270,   264,   271,    14,   274,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   268,   263,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   266,     2,   267,   269,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   273,     2,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262
};

#if HANA_DEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   267,   267,   278,   279,   287,   288,   289,   290,   294,
     299,   304,   312,   313,   321,   326,   334,   335,   339,   343,
     347,   348,   349,   350,   351,   355,   363,   368,   373,   378,
     386,   387,   393,   402,   411,   424,   441,   457,   466,   474,
     475,   483,   484,   492,   493,   501,   510,   521,   526,   533,
     534,   560,   586,   615,   616,   645,   646,   664,   671,   674,
     677,   679,   682,   710,   711,   720,   728,   729,   737,   738,
     746,   747,   748,   762,   763,   770,   782,   783,   786,   794,
     795,   796,   799,   814,   815,   818,   819,   820,   821,   824,
     833,   836,   837,   840,   841,   842,   845,   846,   847,   848,
     853,   854,   858,   866,   867,   875,   884,   887,   891,   898,
     899,   909,   914,   922,   923,   932,   940,   941,   945,   953,
     954,   961,   962,   969,   970,   974,   981,   982,   990,  1004,
    1013,  1014,  1023,  1024,  1025,  1029,  1030,  1041,  1046,  1051,
    1056,  1061,  1066,  1074,  1079,  1087,  1092,  1099,  1100,  1107,
    1112,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1128,  1129,
    1133,  1134,  1138,  1139,  1140,  1141,  1142,  1147,  1155,  1160,
    1164,  1169,  1173,  1179,  1185,  1191,  1201,  1202,  1207,  1213,
    1219,  1226,  1232,  1239,  1245,  1252,  1258,  1265,  1271,  1278,
    1284,  1291,  1297,  1304,  1310,  1321,  1326,  1331,  1336,  1341,
    1346,  1351,  1356,  1361,  1369,  1374,  1379,  1384,  1395,  1396,
    1401,  1406,  1412,  1432,  1439,  1440,  1447,  1454,  1461,  1468,
    1469,  1477,  1502,  1515,  1528,  1542,  1554,  1555,  1559,  1560,
    1561,  1562,  1563,  1564,  1565,  1566,  1567,  1568,  1572,  1580,
    1581,  1589,  1598,  1599,  1607,  1608,  1616,  1617,  1625,  1626,
    1631,  1639,  1640,  1657,  1658,  1659,  1660,  1661,  1663,  1664,
    1665,  1669,  1674,  1681,  1690,  1695,  1705,  1710,  1718,  1723,
    1728,  1733,  1741,  1746,  1754,  1759,  1767,  1776,  1777,  1782,
    1790,  1791,  1799,  1800,  1808,  1809,  1817,  1818,  1823,  1831,
    1832,  1833,  1834,  1839,  1840,  1844,  1845,  1853,  1854,  1861,
    1861,  1865,  1866,  1867,  1868,  1869,  1870,  1871,  1872,  1873,
    1874,  1878,  1879,  1884,  1891,  1892,  1896,  1897,  1898,  1902,
    1903,  1904,  1905,  1906,  1907,  1912,  1920,  1921,  1925,  1926,
    1934,  1939,  1947,  1948,  1957,  1958,  1959,  1960,  1961,  1965,
    1966,  1967,  1972,  1980,  1988,  1995,  2001,  2011,  2012,  2013,
    2014,  2015,  2016,  2017,  2018,  2019,  2020,  2021,  2022,  2023,
    2027,  2034,  2041,  2049,  2056,  2063,  2070,  2077,  2084,  2094,
    2104,  2111,  2119,  2129,  2130,  2131,  2132,  2135,  2142,  2143,
    2144,  2155,  2156,  2160,  2167,  2168,  2175,  2181,  2189,  2190,
    2194,  2200,  2210,  2211,  2212,  2213,  2214,  2215,  2216,  2217,
    2218,  2222,  2223,  2225,  2230,  2235,  2239,  2247,  2248,  2252,
    2253,  2258,  2259,  2268,  2269,  2273,  2274,  2278,  2279,  2280,
    2284,  2288,  2293,  2294,  2295,  2299,  2303,  2304,  2305,  2306,
    2307,  2311,  2317,  2327,  2328,  2336,  2337,  2340,  2345,  2356,
    2362,  2369,  2376,  2386,  2393,  2402,  2409,  2416,  2423,  2430,
    2439,  2446,  2455,  2462,  2470,  2471,  2478,  2479,  2486,  2487,
    2494,  2503,  2504,  2511,  2556,  2557,  2558,  2559,  2560,  2561,
    2562,  2563,  2564,  2565,  2566,  2567,  2568,  2569,  2570,  2571,
    2572,  2573,  2574,  2575,  2576,  2577,  2578,  2579,  2580,  2581,
    2582,  2583,  2584,  2585,  2588,  2589,  2590,  2676,  2680,  2684,
    2688,  2692,  2696,  3072,  3073,  3074,  3078,  3079,  3083,  3084,
    3088,  3089,  3094,  3103,  3104,  3105,  3106,  3107,  3108,  3109,
    3110,  3111,  3112,  3113,  3114,  3115,  3116,  3117,  3118,  3119,
    3124,  3125,  3129,  3130,  3131,  3132,  3133,  3134,  3135,  3136,
    3137,  3138,  3139,  3140,  3141,  3142,  3143,  3144,  3145,  3146,
    3147,  3148,  3149,  3150,  3151,  3152,  3153,  3154,  3155,  3156,
    3157,  3158,  3159,  3160,  3161,  3162,  3163,  3164,  3165,  3166,
    3167,  3168,  3169,  3170,  3171,  3172,  3173,  3174,  3175,  3176,
    3177,  3178,  3179,  3180,  3181,  3182,  3183,  3184,  3185,  3186,
    3187,  3188,  3189,  3190,  3191,  3192,  3193,  3194,  3197,  3198,
    3199,  3200,  3201,  3202,  3203,  3204,  3205,  3206,  3207,  3208,
    3209,  3210,  3211,  3212,  3213,  3215,  3216,  3217,  3218,  3219,
    3220,  3221,  3227,  3228,  3233,  3258,  3259,  3263,  3266,  3271,
    3272,  3273,  3277,  3278,  3286,  3287,  3290,  3291,  3292,  3300,
    3301,  3306,  3314,  3315,  3320,  3328,  3329,  3336,  3337,  3340,
    3341,  3349,  3350,  3361,  3372,  3388,  3389,  3398,  3405,  3410,
    3416,  3423,  3428,  3434,  3441,  3442,  3443,  3446,  3453,  3461,
    3469,  3476,  3484,  3485,  3492,  3498,  3504
};
#endif

#if HANA_DEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "STRING", "INTNUM", "BOOL",
  "APPROXNUM", "NULLX", "UNKNOWN", "QUESTIONMARK", "UMINUS", "'('", "')'",
  "'.'", "ALL", "AND", "ANY", "ARRAY", "AS", "ASC", "AVG", "BETWEEN",
  "BIGINT", "BINARY", "BLOB", "BOOLEAN", "BY", "CALL", "CASE", "CAST",
  "CHAR", "CHARACTER", "CHARACTERS", "CLOB", "CNNOP", "COALESCE",
  "CODE_UNITS", "COLLATE", "COMP_EQ", "COMP_GE", "COMP_GT", "COMP_LE",
  "COMP_LT", "COMP_NE", "CONVERT", "CORRESPONDING", "COUNT", "CROSS",
  "CUME_DIST", "CURRENT", "CURRENT_TIMESTAMP", "CURRENT_USER", "DATE",
  "DAY", "DEC", "DECIMAL", "DEFAULT", "DELETE", "DENSE_RANK", "DESC",
  "DISTINCT", "DOUBLE", "ELSE", "END", "END_P", "ESCAPE", "ERROR",
  "EXCEPT", "EXCLUDE", "EXISTS", "FLOAT", "FOLLOWING", "FOR", "FROM",
  "FULL", "G", "GROUP", "GROUPING", "HAVING", "HOUR", "IN", "INNER",
  "INSERT", "INT", "INTEGER", "INTERSECT", "INTERVAL", "MANY", "ONE",
  "EXACT", "INTO", "IS", "JOIN", "K", "LARGE", "LEFT", "LIKE", "M", "MAX",
  "MIN", "MINUTE", "MOD", "MONTH", "MULTISET", "NATIONAL", "NATURAL",
  "NCHAR", "NCLOB", "NO", "NOT", "NULLIF", "NUMERIC", "OBJECT", "OCTETS",
  "OF", "ON", "ONLY", "OR", "ORDER", "OTHERS", "OUTER", "OVER",
  "PARTITION", "PERCENT_RANK", "PRECEDING", "PRECISION", "RANGE", "RANK",
  "READ", "REAL", "RECURSIVE", "REF", "RIGHT", "ROW", "ROWS", "ROW_NUMBER",
  "SCOPE", "SECOND", "SELECT", "SESSION_USER", "SET", "SETS", "SMALLINT",
  "SOME", "STDDEV_POP", "STDDEV_SAMP", "SUM", "SYSTEM_USER", "THEN",
  "TIES", "TIME", "TIMESTAMP", "TO", "UNBOUNDED", "UNION", "UPDATE",
  "USING", "VALUES", "VARCHAR", "NVARCHAR", "VARYING", "VAR_POP",
  "VAR_SAMP", "WHEN", "WHERE", "WITH", "WITHOUT", "YEAR", "ZONE", "LIMIT",
  "OFFSET", "TOP", "WAIT", "NOWAIT", "HINT", "RANGE_RESTRICTION",
  "PARAMETERS", "UPSERT", "REPLACE", "PRIMARY", "KEY", "CONTAINS",
  "P_POINT", "SECONDDATE", "TINYINT", "SMALLDECIMAL", "TEXT", "BINTEXT",
  "ALPHANUM", "VARBINARY", "SHORTTEXT", "ORDINALITY", "UNNEST", "HISTORY",
  "OVERRIDING", "USER", "SYSTEM", "VALUE", "NULLS", "FIRST", "LAST",
  "CORR", "CORR_SPEARMAN", "MEDIAN", "STDDEV", "VAR", "STRING_AGG",
  "MEMBER", "SYSTEM_TIME", "APPLICATION_TIME", "TABLESAMPLE", "BERNOULLI",
  "RETURN", "LATERAL", "BINNING", "LAG", "LEAD", "RANDOM_PARTITION",
  "WEIGHTED_AVG", "EXTRACT", "FIRST_VALUE", "LAST_VALUE", "NTH_VALUE",
  "NTILE", "SERIES_FILTER", "LOCATE_REGEXPR", "OCCURRENCES_REGEXPR",
  "REPLACE_REGEXPR", "SUBSTR_REGEXPR", "PERCENTILE_CONT",
  "PERCENTILE_DISC", "TRIM", "WITHIN", "ROLLUP", "CUBE", "BEST",
  "SUBTOTAL", "BALANCE", "TOTAL", "MULTIPLE", "RESULTSETS", "PREFIX",
  "STRUCTURED", "RESULT", "OVERVIEW", "TEXT_FILTER", "FILL", "UP",
  "MATCHES", "SORT", "MINUS", "ROWCOUNT", "LOCKED", "IGNORE", "SHARE",
  "LOCK", "UTCTIMESTAMP", "COMMIT", "ID", "XML", "JSON", "RETURNS", "';'",
  "','", "'*'", "'['", "']'", "':'", "'^'", "'+'", "'-'", "'!'", "'|'",
  "'/'", "'%'", "$accept", "sql_stmt", "stmt_list", "stmt", "call_stmt",
  "sql_argument_list", "sql_argument", "value_expression", "sp_name",
  "dql_stmt", "dml_stmt", "insert_stmt", "insert_columns_and_source",
  "insert_select_stmt", "from_constructor", "delete_stmt", "update_stmt",
  "update_table", "opt_simple_from_clause", "simple_from_list",
  "update_elem_list", "update_elem", "select_stmt", "query_expression",
  "query_expression_body", "query_term", "query_primary",
  "select_with_parens", "subquery", "sap_table_subquery", "table_subquery",
  "row_subquery", "simple_table", "opt_where", "opt_from_clause",
  "opt_groupby", "grouping_element_list", "grouping_element",
  "row_order_by_list", "row_order_by", "column_ref_perens",
  "group_set_name", "grouping_option", "grouping_best",
  "grouping_subtotal", "grouping_prefix_tb", "grouping_prefix",
  "grouping_structured_res", "grouping_text_filter", "opt_order_by",
  "order_by", "sort_list", "sort_key", "opt_asc_desc", "opt_having",
  "with_clause", "with_list", "common_table_expr",
  "opt_derived_column_list", "simple_ident_list_with_parens",
  "simple_ident_list", "column_ref_list", "opt_distinct",
  "select_expr_list", "projection", "from_list", "table_reference",
  "table_primary", "table_primary_non_join", "lateral_table",
  "associated_table", "associated_ref_list", "associated_ref",
  "opt_many2one_part", "opt_search_condition", "opt_tablesample",
  "opt_table_qualify", "collection_derived_table",
  "collection_value_expr_list", "collection_value_expr", "as_derived_part",
  "opt_simple_ident_list_with_parens", "column_ref", "relation_factor",
  "func_relation_factor", "joined_table", "qualified_join", "case_join",
  "case_join_when_list", "case_join_when", "ret_join_on",
  "select_expr_list_parens", "opt_case_join_else", "cross_join",
  "join_type", "join_outer", "join_cardinality", "hana_construct_table",
  "construct_column_list", "construct_column", "search_condition",
  "boolean_term", "boolean_factor", "boolean_test", "boolean_primary",
  "predicate", "member_of_predicate", "bool_function",
  "comparison_predicate", "between_predicate", "like_predicate",
  "in_predicate", "null_predicate", "exists_predicate", "row_expr",
  "factor0", "factor1", "factor2", "factor3", "factor4", "row_expr_list",
  "in_expr", "truth_value", "comp_op", "cnn_op", "comp_all_some_any_op",
  "plus_minus_op", "star_div_percent_mod_op", "expr_const", "case_expr",
  "case_arg", "when_clause_list", "when_clause", "case_default",
  "func_expr", "opt_nulls", "aggregate_expression", "aggregate_name",
  "sap_specific_function", "regexpr_func_name", "within_group",
  "regexpr_in_val", "opt_order_by_clause", "order_by_clause",
  "order_by_expression_list", "order_by_expression", "opt_collate_clause",
  "ranking_windowed_function", "ranking_function_name", "over_clause",
  "window_specification", "window_name", "window_specification_details",
  "opt_existing_window_name", "opt_window_partition_clause",
  "opt_window_frame_clause", "window_frame_units", "window_frame_extent",
  "window_frame_start", "window_frame_preceding", "window_frame_between",
  "window_frame_bound", "window_frame_following",
  "opt_window_frame_exclusion", "scalar_function",
  "table_function_param_list", "table_function_param", "expr_point_val",
  "function_call_keyword", "for_xml", "for_json", "opt_returns_clause",
  "opt_option_string_list_p", "option_string_list", "option_string",
  "data_type", "array_type", "predefined_type", "lob_data_type",
  "primary_datetime_field", "all_some_any", "opt_as_label", "as_label",
  "label", "collate_clause", "name_r", "name_f", "reserved", "top_clause",
  "opt_for_update", "for_share_lock", "for_update", "opt_of_ident_list",
  "opt_ignore_lock", "wait_nowait", "limit_num", "offset_num",
  "limit_total", "with_hint_param", "with_hint_param_list", "hint_clause",
  "opt_with_range_restrict", "with_primary_key", "upsert_stmt",
  "replace_stmt", "for_system_time", "sys_as_of_spec", "sys_from_to_spec",
  "sys_between_spec", "for_application_time", "partition_restriction",
  "intnum_list", "tablesample_num", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,    40,    41,    46,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,    59,    44,    42,    91,    93,    58,    94,
      43,    45,    33,   124,    47,    37
};
# endif

#define YYPACT_NINF -806

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-806)))

#define YYTABLE_NINF -532

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     393,    58,  8500,    16,    52,   -22,  8500, 15773,  8500,  8500,
     200,   143,   -92,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,   106,   151,  -806,  -806,   130,  -806,  -806,   191,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806, 16021,    37,  -806,  -806,  -806,  -806,  -806,
     202,  -806,  7998,   155,  8500,   359,   165,   133, 11997, 16021,
     -17,  -806,   242,   139,   139,  -806,  -806,   393,   165,   136,
     165,   165,  -806,  -806,   104,  -806,  -806,   165,   106,  -806,
    -806,  3998,   236,  -806, 13006,   268,    58,   279,   290,  -806,
    -806, 12758,  -806,   146, 12758, 12758, 12758,    50,   307, 12758,
     308,  -806,   313,  7998,   248,  -806,  -806,  -806,  -806,  5881,
    6957,  -806, 13265,   320,  -806,  -806,  -806,  -806, 12758,   -17,
   16021, 16021,   317,  -806, 12248,  -806,   215,  -806,   215,  -806,
     130,   288,   161,   410,   410,   130,   130,  -806,   130,   104,
    -806,  -806,  -806,  -806,  -806,  -806,  3729,   104,  6957,   416,
     440,   462,   465,   469,   480,  -806,   500,   508,  5074,   514,
     524,  -806,  -806,   526,   533,   546,   548,   552,   558,   572,
     580,   581,   582,   583,   585,  6957,  6957,    23,  -806,   438,
    -806,  -806,  -806,   350,   584,  -806,  -806,   506,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,  -806,  -806,   809,   217,   162,
     332,  -806,  -806,  -806,  -806,  -806,  -806,   591,  -806,   592,
    -806,   593,  -806,  -806,   599,   598,   601,   608,   610, 13513,
     609,   611, 16269, 12499,   242,  5074,   104,  -806,  -806,  -806,
     347,  5074, 16021, 12758,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  6150,  -806, 13761,   146,  6957,   300,  -806,  -806,   459,
    -806,   260,  5343,  -806,   -41,  -806,  3470,   -13,  -806,    99,
    -806,   624,  -806,  -806,    24,  -806,   500,   335,   339,   151,
   16021,   349,  -806,   626,   369,   369,   151,   151,  -806,  -806,
    3729,   102,   809,    26,  -806,   127,   468,  6957,   628,  6957,
    6957,  5612,   629,  -806,  6957,  -806,   809,  6957,  6419,  6957,
    6957,  6957,   379,  6957,  6957,  6957,  6957,  6957,   569,   569,
    6957,  -806,  -806,   104,  5074,  1192,  5074,  5074,   135,  6957,
    -806,  8500,  -806,  -806,   596,  -806,   304,  -806,   625,   138,
    6957,   125,   525,   600,   368,  4267,  6957,  6957,  -806,  -806,
    -806,  6957,  -806,  -806,  -806,  6957,  6957,   165,   638,  6688,
    6957,  8751, 11479,   640,   642, 16021,   633, 14009,  -806,   637,
     644,   397,  -806,   657,   659,   665,   666,  -806,  -806,   350,
    -806,   159,   668,    22,   671,  -806,   422,   482,  -806,   127,
      29,  -806,  -806,    69,   677,  -806,   104,   127,   432,   501,
     502,   531,  -806,  -806,   459,  5343,  7747,  5881,   146,  -806,
    7998,  6957,   146,  6957,  -806,    32,  -806, 16021,  -806,   521,
    -806,   459,   522,  -806,   444,  -806,   704,  -806,   457,   673,
      33,  -806,  1192,  -806,  -806,   697,  -806,  -806,  6957,  5074,
     112,  -806,    87,   700,    40,    12,  6957,   701,     0,   702,
      66,   122,  -806,    44,    59,   -30,    65,   643,   121,   121,
      49,    71,    78,   705,   706,    21,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,   708,  -806,   709,  -806,  -806,  -806,  -806,
    -806,  -806,   710,   712,  -806,  -806,  -806,  -806,  -806,   713,
     714,   715,  -806,  -806,   711,  -806,   584,  -806,  -806,  -806,
     235,  -806,    28,  -806,  -806,  -806,  -806,  5343,  -806,  -806,
    -806,   720,   117,  6957,   625,  6957,   615,  6957,  -806,  -806,
    -806,  -806,  -806,   127,  -806,   217,   127,   162,   332,  -806,
    6957,   650,   612,    89,    27, 14257,  -806,   718,   420,  -806,
     472,   724,   725,   703,  -806, 16021, 16021,   727,  6957,    45,
   16269,  8999,  -806,  -806, 16517,   630,   735,   739,   632,   476,
   16021,  5074,    31,  -806,  -806,  -806,  6957, 11738, 14505,  -806,
      58,    58,   737,  -806,   732,  7488,   490,   602,  -806,  -806,
    -806,  -806,  -806,  -806,  -806,   678,   492,  -806,  -806,   104,
     127,  -806,   753,  -806,   578,  -806,   579, 16021,  -806,   509,
    -806,   757,  -806,   626,  -806,    80,   127,    70,   794,  6957,
    -806,   699,  1192,  -806,  -806,  8500,    90,  -806,  -806,  -806,
    6957,  6957,  -806,  -806,   738,   751,  -806,   612,  6957,   742,
     759,   762,   764,   612,   612,   537,   537,  -806,  6957,   774,
     776,   777,   779,   783,   784,   785,  -806,  -806,  6957,    94,
    -806,  6957,    82,  -806,   129,  6957,   127,    41, 14764, 15015,
    -806,   612,  -806, 16021,   780,  9247, 11479,   104,   104,  6957,
    -806,  -806, 16021,   101,   790,   604,  -806,  -806,   787,   789,
     793,   782,   646,   800, 16021,  -806,   648,   350,   801,   798,
     802,  -806,  -806,  -806,   799,  -806,  -806,  -806,   808, 16021,
    7488,   602,   804,  7747,   729,   707,   352,  7747,   707,   707,
     134,   791,   741,  7998,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  6957,  6957,   127,  -806,   810,   811,  -806,    42,
      47, 15274,  -806,  -806,    73,  6957,  -806,  -806,  -806,  -806,
    -806,   749,   612,   612,    74,   103,   814,   817,   827,   828,
     829,   830,   127,  -806,   127,  6957,  6957,   127,  -806,  -806,
     679,   833, 16021,  -806,  -806,  -806,  -806,   840,  9743, 16021,
    -806,   841,  -806,  -806,  -806,    75,  -806,  -806,   844,   838,
    9495, 16517,  -806,   854,   855,  -806,   671,   239,   594,   847,
     857,   858, 16517,   851,   853,  -806,   602,  8249,  -806,   352,
     716,   719,   778,  -806,   473,   352,   352,   773,  7747,  -806,
     165,  5074,   752,  -806,   127,   127,  -806,  -806,  -806,  -806,
     835,   835,   614,  -806,  -806,   616,  -806,    61,   856,  -806,
    -806,  -806,  -806,   869,  -806,  -806,  -806,  -806,  -806,  -806,
     127,   127,   875,  -806,  -806,   868,   760,  9991,  -806,   870,
     871, 10239,   104, 16021,   879,   873,   878,  -806,  -806,   740,
     743,   445,  -806,  -806,   881,   882,  -806,  -806, 16021,  -806,
    -806,   356,   395,   744,  5074,  -806,  -806,   734,   503,  4536,
     350,   733,   171,  -806,   171, 15274,  6957,  -806,  -806,  -806,
     788,   886,  -806,  -806,   883,   788,  -806,  -806, 10487, 10735,
    -806,   888,  -806,   111,   896, 10983, 16765,   464,   474,   816,
     818,  -806,  -806,   114,  -806,  1192,  -806,  -806,   823,  -806,
    -806,   824,   421,   350,  5074,   852,   734,  5074,  4805,   772,
    -806,  -806,   653,  -806,   682,   127,   366,  -806,   722,   722,
    -806,  -806,   909,  -806,  6957,    63,  -806,  -806,  -806,  -806,
    -806, 11231,  -806, 16021,  -806,  -806,   831,   832,   834,   836,
    -806,  -806,  -806, 16021,  -806,  -806,  -806,  -806,  -806,   837,
      76,   721,   859,  -806,   350,  -806,  -806,  4536,   923,   920,
     733,   766,   766,   367,  -806,  -806,  -806,   432,  -806,  -806,
    -806,   456,  -806,  -806,   115,  -806,  -806,  -806,  -806,  -806,
    -806,   721,   921,  -806,  -806,  -806,  -806, 15525,   781,   388,
     717,   717,  -806,  -806,   536,   806,   813,   819,   877,  -806,
    -806,  -806,  -806,  -806,  5881,   874,  7226,   116,  -806,  -806,
    -806,   292,   696,  -806,  -806,   698,  -806,  -806,   124,   184,
    -806,   933,  -806,  -806,  -806,  -806,   358,  -806,   118,  7747,
    5343,   788,   119,  -806, 15525,  -806,  -806,  -806,   947,    25,
    -806,  -806,  -806,   536,   820,  -806,   846,  -806,  -806,   573,
     120,   944,  -806,  -806,   723,   726,   728,  -806,   731,  -806,
    -806,  -806,  5074,   113,  -806,   730,  -806,   803,   964,  -806,
     350,   736,   745,  -806,   746,  -806,   821,   805,  -806
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       5,     0,     0,     0,     0,   622,     0,     0,     0,     0,
       0,     0,     3,     8,     6,     7,    22,    21,    20,    19,
      46,   625,    49,    53,    55,     0,    23,    24,     0,   513,
     536,   568,   353,   537,   538,   519,   539,   569,   540,   570,
     541,   395,   499,   572,   393,   563,   574,   567,   542,   535,
     500,   543,   576,   577,   595,   533,   544,   534,   351,   349,
     501,   575,   498,   545,   597,   571,   546,   547,   559,   548,
     566,   394,   549,   550,   392,   560,   551,   552,   553,   396,
     554,   502,   565,   356,   358,   352,   564,   562,   561,   573,
     555,   556,   357,   359,   557,   497,   558,   589,   588,   578,
     579,   520,   580,   581,   582,   583,   584,   585,   586,   587,
     590,   591,   593,   592,   594,   347,   348,   350,   354,   355,
     518,   596,   521,   397,   398,   399,   400,   522,   523,   524,
     525,   526,   373,   374,   375,   376,   527,   528,   529,   598,
     599,   600,   601,   602,   603,   604,   605,   606,   607,   608,
     609,   610,   611,   612,   613,   614,   615,   616,   617,   618,
     619,   621,   620,     0,   651,    18,   515,   517,   516,   532,
     195,   514,     0,     0,     0,     0,   123,     0,   506,   551,
     111,   113,   116,   116,   116,     1,     2,     5,   123,     0,
     123,   123,   630,   631,   651,   629,   626,   123,   625,    56,
     203,     0,     0,    11,     0,   513,     0,     0,     0,    58,
      60,   506,    59,    63,   506,   506,   506,   162,     0,   506,
     195,   204,   514,     0,     0,   623,   624,   124,   125,     0,
       0,   511,     0,     0,    37,   507,   509,   510,   506,   112,
       0,     0,     0,   117,     0,   660,     0,   663,     0,     4,
       0,   632,     0,   456,   456,     0,     0,    47,     0,   651,
     319,   322,   320,   323,   321,   324,     0,   651,   326,   538,
     519,   569,   570,     0,   446,   448,     0,     0,     0,   571,
       0,   450,   451,   520,   518,   521,   522,   523,   524,   525,
     526,     0,   527,   528,   529,     0,     0,     0,    12,    14,
      61,   291,   289,    16,   242,   244,   246,   248,   251,   260,
     259,   253,   254,   256,   255,   257,   258,    17,   277,   280,
     282,   284,   286,   290,   293,   294,   337,   515,   338,   517,
     336,   516,   335,   334,   532,   178,     0,     0,     0,     0,
     196,     0,     0,     0,   176,     0,   651,   142,   140,   141,
       0,     0,     0,   506,   166,   163,   664,   665,   666,   164,
     165,     0,   139,     0,    63,     0,     0,    25,    26,   655,
      30,     0,     0,   129,     0,   126,   506,    39,    43,     0,
     508,     0,    38,   114,     0,   119,     0,    63,    63,    51,
       0,   636,   627,     0,   454,   454,    50,    52,    54,    48,
       0,     0,   295,     0,    10,   327,     0,     0,     0,     0,
       0,     0,     0,   276,     0,   247,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   287,   288,   651,     0,     0,     0,     0,     0,     0,
     311,     0,   305,   303,   304,   301,   302,   306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   278,   314,
     315,     0,   316,   317,   318,     0,     0,   123,     0,     0,
       0,     0,     0,     0,     0,     0,   198,     0,    57,   536,
       0,   169,   171,   178,     0,     0,     0,   177,   138,    64,
      34,     0,     0,     0,   146,   147,   150,   160,   431,   436,
       0,   433,   435,   178,   196,   205,   651,   295,    33,     0,
       0,     0,    29,    27,   655,     0,     0,     0,    63,   128,
       0,     0,    63,     0,   672,     0,   118,     0,   115,     0,
     658,   655,     0,   661,   633,   121,     0,   637,   634,     0,
       0,   458,     0,   452,   453,     0,   252,   292,     0,     0,
     332,   328,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,     0,     0,   381,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     9,    13,   471,   494,
     492,   495,   464,   487,   474,   490,   470,   496,   473,   469,
     465,   467,   477,   479,   466,   468,   472,   475,   476,   481,
     483,   485,    15,   462,   461,   493,   243,   245,   299,   300,
       0,   249,     0,   512,   307,   308,   309,     0,   297,   272,
     274,     0,   268,     0,     0,     0,     0,     0,   310,   312,
     503,   505,   504,   265,   313,   279,   264,   281,   283,   285,
       0,     0,   401,     0,     0,     0,   180,   179,   647,   648,
     649,     0,     0,     0,   202,     0,     0,   197,     0,     0,
       0,     0,   144,   143,     0,     0,     0,     0,     0,     0,
       0,   158,     0,   137,   161,   432,     0,     0,     0,    35,
       0,     0,     0,    28,     0,     0,    65,   130,   132,   135,
     133,   208,   209,   134,   127,    66,    40,    41,    44,   651,
      45,   671,     0,   120,     0,   659,     0,     0,   638,     0,
     628,     0,   457,     0,   455,    57,   296,     0,     0,     0,
     329,     0,     0,   346,   445,     0,     0,   342,   344,   447,
       0,     0,   441,   263,     0,     0,   382,   401,     0,     0,
       0,     0,     0,   401,   401,     0,     0,   372,     0,     0,
       0,     0,     0,     0,     0,     0,   463,   250,     0,     0,
     275,     0,     0,   273,   270,     0,   262,     0,     0,     0,
     390,   401,   439,     0,     0,     0,     0,   651,   651,     0,
     200,   201,     0,     0,     0,     0,   167,   168,   179,     0,
       0,     0,     0,     0,     0,   148,   151,   159,     0,     0,
       0,   434,   438,   437,   197,   206,    32,    31,     0,     0,
       0,     0,   133,     0,     0,   226,   228,     0,   226,   226,
       0,     0,   109,     0,    36,   673,   657,   662,   122,   635,
     460,   459,     0,     0,   333,   325,     0,     0,   343,     0,
       0,     0,   370,   360,     0,     0,   363,   364,   365,   366,
     361,     0,   401,   401,     0,     0,     0,     0,     0,     0,
       0,     0,   266,   298,   269,     0,     0,   261,   345,   379,
     378,     0,   407,   403,   402,   405,   391,     0,     0,     0,
     182,   181,   650,   652,   653,     0,   199,   170,   175,     0,
       0,     0,   667,     0,     0,   670,   145,     0,     0,     0,
       0,     0,     0,     0,     0,   136,   131,     0,   227,   228,
       0,     0,     0,   225,     0,   228,   228,     0,     0,   212,
     123,     0,   381,    42,   330,   331,   443,   444,   442,   449,
     388,   388,   383,   384,   362,   102,   103,   106,     0,   368,
     369,   371,   488,     0,   491,   478,   480,   482,   484,   486,
     267,   271,     0,   367,   408,     0,   409,     0,   186,   185,
       0,     0,   651,     0,     0,   181,     0,   669,   668,     0,
       0,     0,   149,   676,     0,     0,   207,   656,     0,   221,
     222,     0,     0,     0,     0,   223,   224,     0,     0,     0,
     110,   639,   106,   389,   106,     0,     0,   107,   108,   105,
       0,     0,   380,   404,     0,   100,   194,   193,     0,     0,
     184,   183,   654,     0,   174,     0,     0,     0,     0,     0,
       0,   675,   674,     0,   239,     0,   229,   230,     0,   232,
     234,     0,     0,   211,     0,   219,   214,     0,     0,     0,
      80,    81,    67,    68,    83,    71,     0,    62,   339,   339,
     385,   104,     0,   489,     0,   411,   101,   190,   189,   192,
     191,     0,   173,     0,   530,   531,     0,     0,     0,     0,
     153,   152,   238,     0,   241,   231,   236,   233,   235,     0,
       0,     0,     0,   215,   210,    70,    79,     0,     0,     0,
     639,   642,   642,     0,   387,   386,   377,   410,   414,   413,
     406,     0,   188,   187,     0,   155,   154,   157,   156,   240,
     237,     0,     0,   220,   213,    69,    84,     0,    85,     0,
     645,   645,   340,   341,     0,     0,     0,     0,   426,   415,
     419,   416,   172,   216,     0,     0,     0,     0,    73,    76,
      77,     0,    96,   644,   643,     0,   640,   641,     0,     0,
     422,     0,   424,   418,   417,   420,     0,   412,     0,     0,
       0,   100,     0,    72,     0,    86,    87,    88,     0,    93,
     646,   423,   425,     0,     0,   428,     0,   429,   218,     0,
       0,     0,    78,    74,    97,     0,     0,    82,    91,   421,
     427,   430,     0,   292,    75,     0,    90,    94,     0,    89,
     217,    98,     0,    92,     0,    95,     0,     0,    99
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -806,  -806,   792,  -806,  -806,  -806,   541,  -806,  -806,  -806,
    -806,  -806,  -806,   613,   -61,  -806,  -806,  -806,  -806,  -806,
    -806,   450,  -153,     9,   955,   160,   747,  -806,  -131,  -806,
    -247,  -806,  -806,    43,  -806,  -806,  -806,  -106,  -806,  -181,
    -151,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -174,
    -542,  -806,    -8,  -544,  -806,  -806,   812,   754,   399,   -94,
    -805,  -806,  -172,  -145,   475,  -806,  -514,    91,  -171,  -806,
    -806,   203,   326,  -806,  -806,  -806,  -806,  -806,   341,  -806,
    -806,  -806,  -339,    15,  -806,  -531,  -806,  -806,   -37,  -806,
    -108,  -806,  -806,  -806,  -806,  -231,  -553,  -806,  -806,   -69,
    -251,   574,  -230,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
    -806,  -806,  -806,  -806,  2491,   555,   547,   549,  -160,  -806,
    -351,   391,   402,  -806,  -806,  -806,  -806,  -806,  -420,  -806,
    -806,  -806,   466,  -806,  -452,   -31,  -806,   956,  -806,  1233,
     273,  -806,    98,  -806,  -806,    30,    92,  -806,  1914,  -272,
    -806,   149,  -806,  -806,  -806,  -806,  -806,  -806,   -79,  -806,
    -806,  -149,  -806,  -806,  -150,  -235,   351,  -806,  -806,  -806,
    -806,   631,   775,  -806,   318,   310,  -806,  -530,  -806,   275,
    -806,    48,  -806,   807,  -340,    -2,  -352,  2163,  -806,   839,
    -806,   825,  -806,  -806,  -806,   -60,   -59,   -87,  -806,   259,
    -162,  -332,  -806,  -806,  -806,  -806,  -806,  -806,  -806,  -806,
     843,  -806,  -806
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    10,    11,    12,    13,   297,   298,   299,   164,    14,
      15,    16,   367,   368,   369,    17,    18,   177,   522,   696,
     377,   378,    19,    20,    21,    22,    23,   209,   300,   211,
     212,   301,    24,   346,   518,   822,  1042,  1043,  1137,  1138,
    1139,  1044,  1089,  1090,  1142,  1187,  1199,  1188,  1169,  1055,
    1056,   935,   936,   999,   922,    25,   180,   181,   242,   243,
     384,   534,   229,   374,   375,   686,   811,   688,   689,   214,
     215,   494,   495,   898,   796,   673,   353,   216,   480,   481,
     786,   488,   302,   217,   218,   690,   691,   919,  1035,  1036,
    1113,  1135,  1082,   692,   820,   909,   913,   693,  1023,  1024,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   507,   318,   319,   320,   321,   322,
     403,   619,   611,   455,   456,   457,   461,   465,   323,   324,
     406,   550,   551,   721,   325,  1094,   326,   327,   328,   329,
     852,   871,   735,   736,   932,   933,   992,   330,   331,   770,
     873,   874,   955,   956,  1005,  1100,  1101,  1128,  1150,  1130,
    1131,  1151,  1152,  1157,   332,   500,   501,   502,   333,   192,
     193,   543,   394,   540,   541,   602,   603,   604,   605,   334,
     634,   234,   235,   236,   458,   335,   221,   222,   176,   194,
     195,   196,   391,   710,   538,  1047,  1120,  1146,   650,   651,
     203,   512,   531,    26,    27,   355,   356,   357,   358,   359,
     238,   525,   674
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     170,   213,   687,   482,   170,   182,   170,   170,   573,   574,
      28,   505,   714,   728,   508,   401,   250,   165,   255,   256,
     649,   178,   219,   183,   184,   258,   740,   741,   742,   413,
     245,   247,   257,   516,   747,   440,   433,   526,   441,   547,
     772,   210,   675,   798,   758,   701,   712,   440,   415,   201,
     441,   535,   364,   724,   868,   928,   440,   732,   554,   441,
     929,   520,   440,   440,   784,   441,   441,   563,   564,   565,
       1,   370,   733,   219,   570,   571,   440,   440,   737,   441,
     441,   997,   440,   471,   743,   441,   934,   941,   962,   734,
     172,   744,   210,   -56,   489,   748,   440,   399,   865,   441,
     493,   440,   771,   838,   441,   404,   722,   863,   440,   440,
     440,   441,   441,   441,   887,   546,   942,   440,   643,   505,
     441,   998,   440,   350,  1062,   441,   -78,  1072,  1132,  1163,
     371,  1178,  1182,  1193,   440,   431,   432,   441,   523,   528,
     436,   608,     1,   174,   609,   210,   620,   623,   -56,   401,
     175,   244,   440,   -56,   812,   441,   440,   440,  1013,   441,
     441,   200,   440,   917,   440,   441,   -56,   441,   739,   725,
     220,   187,   170,   233,   188,   719,   237,   182,   665,   189,
     227,   666,   683,   761,   490,   387,   566,   388,   436,   224,
    1098,   997,   572,   485,   436,   866,  1171,     5,  1099,   705,
     185,   618,   340,   202,   199,   726,   624,   607,   186,   237,
     173,   785,   237,   237,   237,   341,   204,   237,   370,   832,
     436,   220,   625,   517,     7,  1111,   228,   918,   799,   223,
     237,   998,   -78,   667,   548,   -56,   237,   197,   182,   385,
     739,   608,   385,   800,   609,   610,   -56,   240,   621,  1154,
     487,   521,   677,    28,   241,   210,  1172,   803,  1104,   344,
     244,   190,   347,   348,   349,  1185,   759,   362,  1186,     5,
     202,   576,     1,   454,   230,   341,   549,   169,     5,   812,
    -530,   169,   169,   169,   169,   454,   382,   434,   527,   669,
     548,   342,   251,   676,   454,   640,   702,   713,   717,   906,
     454,   454,   343,   914,   548,     7,   639,   783,   548,  1155,
     514,   345,   505,   548,   454,   454,   351,   210,   352,   361,
     454,   482,   363,   548,   649,  -531,   805,   969,   970,   676,
     730,   -56,   381,   626,   454,   548,   386,   476,   802,   454,
     483,   486,   676,   615,   679,   616,   454,   454,   454,   697,
     496,   237,   484,   548,   548,   454,   980,   191,   548,   503,
     454,   504,   985,   986,   225,   548,   219,   943,   828,   226,
     219,  1091,   454,   365,   237,   527,  1092,   618,  1073,   527,
    1164,   341,   517,   548,   548,   210,   731,     5,   483,   210,
     454,   252,   971,  1143,   454,   454,   253,   254,  1144,     5,
     454,   497,   454,   390,   988,     1,   365,   506,  1174,   545,
     389,   336,   337,   338,     7,   396,   397,   392,   365,   503,
     797,     2,   393,   805,   519,   503,     7,   462,   407,   870,
     530,   533,  -530,    42,  -513,  1175,   463,   464,   169,   170,
     910,   911,   912,   366,  1026,  1027,  1028,   169,  1048,   169,
    1049,     3,   408,   169,   169,   366,   613,   435,  1052,    50,
     260,   261,   262,   263,   264,   843,   265,  1176,   436,   647,
     486,   849,   850,   654,   409,   657,     4,   410,  1124,   169,
      60,   411,    62,  1029,  1030,  1031,   169,   459,   460,   169,
     169,   169,   412,   210,   169,  1074,   509,   510,   169,   876,
     345,   529,   931,   251,   345,   532,  1125,   169,  1177,  1077,
    1078,  1079,   206,   169,   220,   169,   169,    81,   220,   169,
     414,   814,   536,   537,   545,   703,   417,   806,   807,  1165,
    1166,  1167,     5,  1019,  1020,   219,   418,   824,   419,   805,
     260,   261,   262,   263,   264,   420,   265,    95,   815,     6,
     976,   814,  1066,  1067,   210,   816,   491,   492,   421,     7,
     422,   695,  1068,  1069,   423,   699,   817,  1122,  1123,   818,
     424,     8,     9,   260,   261,   262,   263,   264,   815,   265,
     939,   940,   246,   248,   425,   816,  1125,   915,   916,   984,
     993,   993,   426,   427,   428,   429,   817,   430,   438,   818,
     437,   466,   252,   467,   468,   469,   819,   253,   254,   976,
    1126,   470,   471,   472,   169,   883,   884,   169,   169,  1037,
     473,   814,   474,   477,   478,   511,   341,   169,   169,   524,
     539,   542,   549,   553,   559,   614,   819,   617,   169,   628,
     627,   629,   641,   774,   652,  1179,   653,   655,   815,   658,
     814,   169,   923,   780,   781,   816,   931,   659,   483,   788,
     219,   660,   789,   219,   976,   169,   817,   219,   496,   818,
     990,   661,   662,   219,   503,   483,   804,   815,   663,   210,
     664,  1127,   210,   220,   816,   670,   210,   668,   671,  1192,
    1148,   678,   210,   672,   341,   817,   548,   567,   818,   680,
     681,   704,   706,  1097,  1149,   483,   819,   682,   707,   708,
     715,   709,   711,   723,   727,   729,   169,   738,   745,   746,
     749,   750,   751,   170,   752,   753,   754,   755,   760,   756,
     765,   768,   775,  1033,   769,   819,   776,   777,   778,   791,
     837,   782,   779,   792,   794,   790,   169,   793,   989,   808,
     169,   809,   169,  1149,   813,   821,   823,   219,   825,   826,
     827,   830,   829,   835,   842,   841,   483,   875,   219,   845,
     851,   877,   846,   881,   486,   847,   210,   848,  1140,   855,
     886,   856,   857,  1080,   858,  1162,  1084,   210,   859,   860,
     861,   169,   496,   888,   878,   169,   889,   892,   893,   894,
    1012,   890,   169,   891,   895,   897,   899,   904,   220,  1180,
     900,   220,   903,   902,   901,   220,   439,   905,   920,   545,
     921,   220,   907,   926,   927,  1140,   938,   944,   908,   440,
     945,   439,   441,   442,   443,   444,   445,   446,   447,   483,
     946,   947,   948,   949,   440,   952,   953,   441,   442,   443,
     444,   445,   446,   447,   957,   961,   963,   964,   967,   968,
     973,   972,   974,   975,   977,   978,   987,   983,  1000,   981,
     875,   734,   982,   441,  1001,   448,   959,   960,   995,  1002,
     996,  1003,  1014,  1004,  1008,  1009,   449,  1015,   965,   966,
     448,   450,  1016,  1017,  1021,  1022,  1018,  1032,  1034,  1053,
     886,   449,  1061,  1046,   451,   220,   450,   739,  1063,  1070,
    1054,  1071,  1075,  1076,  1086,  1081,   220,  1087,  1088,   451,
     169,  1093,  1096,  1114,  1105,  1106,  1110,  1107,  1116,  1108,
     169,   169,  1117,  1134,  1112,   169,   169,  1119,  1154,   169,
    1153,  1200,  1168,   833,  1155,   169,  1156,  1141,  1159,  1173,
    1170,  1184,   169,   169,  1190,  1007,  1145,  1194,   166,  1011,
     169,   385,   166,   166,   166,   166,  1191,  1196,  1203,  1202,
    1195,   698,  1197,  1198,  1207,   577,  1025,  1208,  1201,   249,
     198,  1115,   169,  1183,   513,  1161,  1204,  1181,  1051,  1158,
    1205,   239,   694,   483,   383,  1206,   795,   896,   979,  1083,
     169,   787,   452,  1133,  1109,   398,  1058,  1060,   637,   219,
     606,   635,   757,  1011,   638,   763,   720,   452,  1095,   853,
     991,   954,  1129,   994,  1189,  1050,   544,   801,   210,   395,
    1118,   831,   836,  1121,  1147,   882,     0,   259,     0,   380,
       0,     0,   354,   169,   169,     0,     0,   341,   169,     0,
     169,     0,     0,     0,     0,     0,     0,   169,     0,  1103,
     360,   385,     0,     0,     0,     0,   453,   454,     0,   169,
       0,  1025,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   453,   454,     0,   169,   169,     0,     0,   169,     0,
       0,     0,   169,     0,     0,     0,     0,     0,   169,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   483,   169,     0,     0,   166,
       0,     0,     0,     0,     0,     0,     0,     0,   166,     0,
     166,     0,     0,     0,   166,   166,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   169,     0,     0,
       0,     0,     0,   169,   169,     0,     0,   220,     0,     0,
     166,     0,   483,     0,     0,   169,   169,   166,     0,   341,
     166,   166,   166,     0,     0,   166,     0,   169,     0,   166,
       0,     0,   169,     0,     0,     0,     0,     0,   166,     0,
       0,     0,     0,   169,   166,     0,   166,   166,     0,     0,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   578,     0,   579,   580,     0,
       0,     0,     0,     0,     0,     0,   581,     0,     0,     0,
       0,     0,   169,     0,     0,   167,   169,     0,   169,   167,
     167,   167,   167,     0,     0,   582,     0,     0,   583,     0,
       0,     0,     0,   169,   584,     0,     0,     0,     0,     0,
       0,     0,     0,   585,     0,     0,     0,     0,     0,     0,
     169,     0,     0,     0,     0,     0,     0,   586,     0,     0,
       0,     0,     0,   169,   169,     0,     0,     0,     0,     0,
     169,   169,     0,     0,     0,   166,     0,     0,   166,   166,
     587,     0,     0,     0,     0,     0,     0,     0,   166,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   166,
       0,     0,   588,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   166,     0,     0,   589,   169,     0,   169,     0,
       0,     0,     0,   590,   591,     0,   166,     0,   169,     0,
       0,   592,   593,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   594,   595,   596,   597,
     598,   599,   600,   601,     0,     0,     0,     0,     0,     0,
       0,     0,   169,     0,     0,     0,   167,   166,     0,     0,
       0,     0,     0,     0,     0,   167,     0,   167,     0,     0,
       0,   167,   167,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   166,     0,     0,
       0,   166,     0,   166,   169,     0,     0,   167,     0,   169,
       0,     0,     0,     0,   167,     0,     0,   167,   167,   167,
       0,     0,   167,     0,     0,     0,   167,     0,     0,     0,
       0,     0,     0,     0,     0,   167,     0,     0,     0,     0,
       0,   167,   166,   167,   167,     0,   166,   167,     0,     0,
       0,     0,     0,   166,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   167,     0,     0,   167,   167,     0,     0,     0,
       0,     0,     0,     0,     0,   167,   167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   167,     0,     0,     0,
       0,   166,     0,     0,     0,     0,     0,     0,     0,   167,
       0,   166,   166,     0,     0,     0,   166,   166,     0,     0,
     166,     0,     0,   167,     0,     0,   166,     0,     0,     0,
       0,     0,     0,   166,   166,     0,     0,     0,     0,     0,
       0,   166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   166,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,     0,     0,     0,     0,
       0,   166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,     0,     0,   167,     0,
     167,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   166,   166,     0,     0,     0,   166,
       0,   166,     0,     0,     0,     0,     0,     0,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   167,
     166,     0,     0,   167,     0,     0,     0,     0,     0,     0,
     167,     0,     0,     0,     0,   166,   166,     0,     0,   166,
       0,     0,     0,   166,     0,     0,     0,     0,     0,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   166,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   166,     0,
       0,     0,     0,     0,   166,   166,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   166,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   166,     0,
       0,     0,     0,   166,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   166,     0,     0,     0,   167,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   167,   167,
       0,     0,     0,   167,   167,     0,     0,   167,     0,     0,
       0,     0,     0,   167,     0,     0,     0,     0,     0,     0,
     167,   167,     0,   166,     0,     0,   168,   166,   167,   166,
     168,   168,   168,   168,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   166,     0,     0,     0,     0,     0,
     167,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,     0,     0,     0,     0,     0,     0,   167,     0,
       0,     0,     0,     0,   166,   166,     0,     0,     0,     0,
       0,   166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,   167,     0,     0,     0,   167,     0,   167,     0,
       0,     0,     0,     0,     0,   167,     0,   166,     0,   166,
       0,     0,     0,     0,     0,     0,     0,   167,     0,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   167,   167,     0,     0,   167,     0,     0,     0,
     167,     0,     0,     0,     0,     0,   167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   166,   167,     0,     0,   168,     0,     0,
       0,     0,     0,     0,     0,     0,   168,     0,   168,     0,
       0,     0,   168,   168,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   167,     0,     0,     0,     0,
       0,   167,   167,     0,     0,   166,     0,     0,   168,     0,
     166,     0,     0,   167,   167,   168,     0,     0,   168,   168,
     168,     0,     0,   168,     0,   167,     0,   168,     0,     0,
     167,     0,     0,     0,     0,     0,   168,     0,     0,     0,
       0,   167,   168,     0,   168,   168,     0,     0,   168,     0,
       0,     0,     0,     0,     0,   171,     0,     0,     0,   171,
     171,   171,   171,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     167,     0,     0,     0,   167,     0,   167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   167,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,   167,     0,     0,     0,     0,     0,   167,     0,
       0,     0,     0,   168,     0,     0,   168,   168,     0,     0,
       0,     0,     0,     0,     0,     0,   168,   168,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   168,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     168,     0,     0,     0,   167,     0,   167,     0,     0,     0,
       0,     0,     0,     0,   168,     0,   167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   171,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   171,     0,     0,
       0,   171,   171,     0,     0,     0,     0,     0,     0,     0,
     167,     0,     0,     0,     0,   168,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   171,     0,     0,
       0,     0,     0,     0,   171,     0,     0,   171,   171,   171,
       0,     0,   171,     0,     0,   168,     0,     0,     0,   168,
       0,   168,   167,     0,     0,   171,     0,   167,     0,     0,
       0,   171,     0,   171,   171,     0,     0,   171,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     168,     0,     0,     0,   168,     0,     0,     0,     0,     0,
       0,   168,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   171,     0,     0,   171,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   171,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,     0,     0,     0,     0,     0,   168,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   168,
     168,     0,     0,     0,   168,   168,     0,     0,   168,     0,
       0,     0,     0,     0,   168,     0,     0,     0,     0,     0,
       0,   168,   168,     0,     0,     0,     0,     0,     0,   168,
       0,     0,     0,     0,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   168,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,   168,
     171,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   168,   168,     0,     0,     0,   168,     0,   168,
     171,     0,   317,     0,     0,     0,   168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     376,   379,     0,   168,   168,     0,     0,   168,     0,     0,
       0,   168,     0,     0,     0,     0,     0,   168,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,     0,   402,     0,   405,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   416,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   168,     0,     0,     0,
       0,     0,   168,   168,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   168,   168,     0,     0,   171,     0,
       0,     0,     0,     0,     0,     0,   168,     0,   171,   171,
       0,   168,     0,   171,   171,     0,     0,     0,     0,     0,
       0,     0,   168,   171,     0,     0,   416,     0,     0,     0,
     171,     0,   416,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     171,   168,     0,     0,     0,   168,     0,   168,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
       0,   402,   168,     0,     0,     0,     0,     0,   552,     0,
       0,   555,   558,     0,     0,   560,     0,     0,   561,   168,
       0,     0,   499,     0,   568,   569,     0,     0,   499,     0,
       0,   575,   168,   168,     0,   317,     0,   416,   416,   168,
     612,   171,   171,     0,     0,     0,   171,     0,     0,     0,
       0,   622,     0,     0,     0,   171,   633,     0,   636,     0,
       0,     0,     0,     0,     0,     0,     0,   171,     0,     0,
       0,   644,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   171,     0,     0,   168,     0,   168,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   168,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   171,     0,     0,     0,   376,     0,
       0,     0,   379,     0,   700,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   168,     0,     0,     0,   171,     0,     0,     0,   716,
     718,   171,   171,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   168,     0,     0,     0,     0,   168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   762,     0,   764,     0,   766,     0,
     171,     0,     0,     0,     0,     0,   171,     0,     0,     0,
       0,   767,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   171,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
       0,     0,   416,     0,     0,     0,     0,   499,     0,     0,
       0,   171,   171,     0,     0,     0,     0,     0,   171,  1065,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     834,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   839,   840,     0,   171,     0,   171,     0,     0,   844,
       0,     0,     0,     0,     0,     0,   171,     0,     0,   854,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   862,
       0,     0,   864,     0,     0,     0,   867,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     885,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     171,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   924,   925,     0,     0,   171,     0,     0,
       0,     0,     0,     0,     0,     0,   937,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   950,   951,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   416,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,   231,   416,     0,     0,     0,     0,
    1045,     0,     0,     0,     0,     0,     0,   937,    30,   232,
      31,    32,     0,     0,    33,     0,     0,     0,     0,     0,
      34,    35,     0,    36,     0,   440,    37,    38,   441,     0,
       0,     0,     0,     0,     0,    39,    40,     0,     0,    41,
       0,     0,     0,     0,    42,   416,     0,    43,   416,    44,
      45,     0,    46,     0,     0,     0,     0,     0,     0,    47,
       0,     0,    48,     0,     0,     0,    49,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,     0,     0,     0,    55,    56,     0,     0,    57,    58,
      59,    60,    61,    62,    63,     0,     0,    64,  1045,     0,
       0,    65,     0,    66,    67,    68,     0,    69,     0,     0,
      70,     0,     0,     0,    71,    72,    73,     0,    74,    75,
       0,    76,    77,     0,    78,     0,    79,    80,    81,     0,
       0,     0,    82,     0,     0,    83,    84,    85,     0,     0,
      86,    87,    88,    89,    90,   376,     0,     0,     0,    91,
       0,     0,    92,    93,     0,     0,     0,    94,    95,    96,
       0,     0,     0,     0,     0,     0,     0,    97,     0,    98,
      99,   100,   101,     0,   102,   103,   104,   105,   106,   107,
     108,   109,     0,     0,   110,   111,   112,   113,   114,     0,
       0,     0,   115,   116,   117,   118,   119,   120,     0,     0,
       0,     0,   121,   416,     0,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     0,   132,   133,   134,   135,
     136,   137,   138,     0,     0,     0,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,     0,   154,   155,     0,   156,   157,   158,   159,   160,
     161,   162,   205,   260,   261,   262,   263,   264,     0,   265,
       0,   400,     0,   454,     0,     0,     0,    30,     0,    31,
      32,     0,     0,    33,     0,     0,     0,     0,   268,   269,
     270,     0,    36,     0,     0,   271,    38,     0,     0,     0,
       0,     0,     0,     0,   272,    40,   273,     0,    41,     0,
     274,   275,     0,    42,     0,     0,    43,     0,    44,    45,
       0,    46,     0,     0,     0,     0,     0,     0,    47,   276,
       0,    48,     0,     0,     0,    49,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
       0,     0,     0,    55,    56,   277,     0,    57,    58,    59,
      60,    61,    62,    63,     0,     0,    64,     0,     0,   278,
     279,     0,    66,    67,    68,     0,    69,     0,     0,    70,
       0,     0,     0,    71,    72,    73,     0,    74,    75,     0,
      76,    77,   280,    78,     0,    79,    80,    81,     5,   281,
       0,    82,     0,     0,    83,    84,    85,   282,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,    92,    93,     0,     0,     7,    94,    95,    96,     0,
       0,     0,     0,     0,     0,     0,    97,     0,    98,    99,
     100,   283,     0,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,   110,   111,   112,   113,   114,     0,     0,
       0,   115,   116,   117,   118,   119,   284,     0,     0,     0,
       0,   121,     0,     0,   285,   123,   124,   125,   126,   286,
     287,   288,   289,   290,   291,   132,   133,   134,   135,   292,
     293,   294,     0,     0,     0,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
       0,   154,   155,     0,   156,   157,   158,   159,   160,   161,
     162,     0,     0,     0,     0,     0,     0,     0,     0,   295,
     296,   205,   260,   261,   262,   263,   264,     0,   265,     0,
     266,   267,     0,     0,     0,     0,    30,     0,    31,    32,
       0,     0,    33,     0,     0,     0,     0,   268,   269,   270,
       0,    36,     0,     0,   271,    38,     0,     0,     0,     0,
       0,     0,     0,   272,    40,   273,     0,    41,     0,   274,
     275,     0,    42,     0,     0,    43,     0,    44,    45,     0,
      46,     0,     0,     0,     0,     0,     0,    47,   276,     0,
      48,     0,     0,     0,    49,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,     0,
       0,     0,    55,    56,   277,     0,    57,    58,    59,    60,
      61,    62,    63,     0,     0,    64,     0,     0,   278,   279,
       0,    66,    67,    68,     0,    69,     0,     0,    70,     0,
       0,     0,    71,    72,    73,     0,    74,    75,     0,    76,
      77,   280,    78,     0,    79,    80,    81,     0,   281,     0,
      82,     0,     0,    83,    84,    85,   282,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
      92,    93,     0,     0,     0,    94,    95,    96,     0,     0,
       0,     0,     0,     0,     0,    97,     0,    98,    99,   100,
     283,     0,   102,   103,   104,   105,   106,   107,   108,   109,
       0,     0,   110,   111,   112,   113,   114,     0,     0,     0,
     115,   116,   117,   118,   119,   284,     0,     0,     0,     0,
     121,     0,     0,   285,   123,   124,   125,   126,   286,   287,
     288,   289,   290,   291,   132,   133,   134,   135,   292,   293,
     294,     0,     0,     0,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,     0,
     154,   155,     0,   156,   157,   158,   159,   160,   161,   162,
       0,     0,     0,     0,     0,     0,     0,     0,   295,   296,
     205,   260,   261,   262,   263,   264,     0,   265,     0,   372,
       0,     0,   630,     0,   631,    30,     0,    31,    32,     0,
       0,    33,     0,     0,     0,     0,   268,   269,   270,     0,
      36,     0,     0,   271,    38,     0,     0,     0,     0,     0,
       0,     0,   272,    40,   273,     0,    41,     0,   274,   275,
       0,    42,     0,     0,    43,     0,    44,    45,     0,    46,
       0,     0,     0,     0,     0,     0,    47,     0,     0,    48,
       0,     0,     0,    49,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,     0,     0,
       0,    55,    56,   277,     0,    57,    58,    59,    60,    61,
      62,    63,     0,     0,    64,     0,     0,     0,   279,     0,
      66,    67,    68,     0,    69,     0,     0,    70,     0,     0,
       0,    71,    72,    73,     0,    74,    75,     0,    76,    77,
     280,    78,     0,    79,    80,    81,     0,   281,     0,    82,
       0,   632,    83,    84,    85,   282,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,    92,
      93,     0,     0,     0,    94,    95,    96,     0,     0,     0,
       0,     0,     0,     0,    97,     0,    98,    99,   100,   101,
       0,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,   110,   111,   112,   113,   114,     0,     0,     0,   115,
     116,   117,   118,   119,   284,     0,     0,     0,     0,   121,
       0,     0,   285,   123,   124,   125,   126,   286,   287,   288,
     289,   290,   291,   132,   133,   134,   135,   292,   293,   294,
       0,     0,     0,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,     0,   154,
     155,     0,   156,   157,   158,   159,   160,   161,   162,     0,
       0,     0,     0,     0,     0,     0,     0,   295,   296,   205,
     260,   261,   262,   263,   264,     0,   265,     0,  1038,     0,
       0,     0,     0,     0,    30,     0,    31,    32,     0,     0,
      33,     0,     0,     0,     0,   268,   269,   270,     0,    36,
       0,     0,   271,    38,     0,     0,     0,     0,     0,     0,
       0,   272,    40,   273,     0,    41,     0,   274,   275,     0,
      42,     0,     0,    43,     0,    44,    45,     0,    46,     0,
       0,     0,     0,     0,     0,    47,     0,     0,    48,     0,
       0,     0,    49,     0,  1039,     0,    50,     0,     0,     0,
       0,     0,     0,    51,    52,    53,    54,     0,     0,     0,
      55,    56,   277,     0,    57,    58,    59,    60,    61,    62,
      63,     0,     0,    64,     0,     0,     0,   279,     0,    66,
      67,    68,     0,    69,     0,     0,    70,     0,     0,     0,
      71,    72,    73,     0,    74,    75,     0,    76,    77,   280,
      78,     0,    79,    80,    81,     0,   281,     0,    82,     0,
       0,    83,    84,    85,   282,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,    92,    93,
       0,     0,     0,    94,    95,    96,     0,     0,     0,     0,
       0,     0,     0,    97,     0,    98,    99,   100,   101,     0,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
     110,   111,   112,   113,   114,     0,     0,     0,   115,   116,
     117,   118,   119,   284,     0,     0,     0,     0,   121,     0,
       0,   285,   123,   124,   125,   126,   286,   287,   288,   289,
     290,   291,   132,   133,   134,   135,   292,   293,   294,     0,
    1040,  1041,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,     0,   154,   155,
       0,   156,   157,   158,   159,   160,   161,   162,     0,     0,
       0,     0,     0,     0,     0,     0,   295,   296,   205,   260,
     261,   262,   263,   264,     0,   265,     0,   515,  1085,     0,
       0,     0,     0,    30,     0,    31,    32,     0,     0,    33,
       0,     0,     0,     0,   268,   269,   270,     0,    36,     0,
       0,   271,    38,     0,     0,     0,     0,     0,     0,     0,
     272,    40,   273,     0,    41,     0,   274,   275,     0,    42,
       0,     0,    43,     0,    44,    45,     0,    46,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,     0,     0,
       0,    49,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,     0,     0,     0,    55,
      56,   277,     0,    57,    58,    59,    60,    61,    62,    63,
       0,     0,    64,     0,     0,     0,   279,     0,    66,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
      72,    73,     0,    74,    75,     0,    76,    77,   280,    78,
       0,    79,    80,    81,     5,   281,     0,    82,     0,     0,
      83,    84,    85,   282,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,    92,    93,     0,
       0,     7,    94,    95,    96,     0,     0,     0,     0,     0,
       0,     0,    97,     0,    98,    99,   100,   101,     0,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,   110,
     111,   112,   113,   114,     0,     0,     0,   115,   116,   117,
     118,   119,   284,     0,     0,     0,     0,   121,     0,     0,
     285,   123,   124,   125,   126,   286,   287,   288,   289,   290,
     291,   132,   133,   134,   135,   292,   293,   294,     0,     0,
       0,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,     0,   154,   155,     0,
     156,   157,   158,   159,   160,   161,   162,     0,     0,     0,
       0,     0,     0,     0,     0,   295,   296,   205,   260,   261,
     262,   263,   264,     0,   265,     0,   266,     0,     0,     0,
       0,     0,    30,     0,    31,    32,     0,     0,    33,     0,
       0,     0,     0,   268,   269,   270,     0,    36,     0,     0,
     271,    38,     0,     0,     0,     0,     0,     0,     0,   272,
      40,   273,     0,    41,     0,   274,   275,     0,    42,     0,
       0,    43,     0,    44,    45,     0,    46,     0,     0,     0,
       0,     0,     0,    47,   276,     0,    48,     0,     0,     0,
      49,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,    51,    52,    53,    54,     0,     0,     0,    55,    56,
     277,     0,    57,    58,    59,    60,    61,    62,    63,     0,
       0,    64,     0,     0,   278,   279,     0,    66,    67,    68,
       0,    69,     0,     0,    70,     0,     0,     0,    71,    72,
      73,     0,    74,    75,     0,    76,    77,   280,    78,     0,
      79,    80,    81,     0,   281,     0,    82,     0,     0,    83,
      84,    85,   282,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,    92,    93,     0,     0,
       0,    94,    95,    96,     0,     0,     0,     0,     0,     0,
       0,    97,     0,    98,    99,   100,   283,     0,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,   110,   111,
     112,   113,   114,     0,     0,     0,   115,   116,   117,   118,
     119,   284,     0,     0,     0,     0,   121,     0,     0,   285,
     123,   124,   125,   126,   286,   287,   288,   289,   290,   291,
     132,   133,   134,   135,   292,   293,   294,     0,     0,     0,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,     0,   154,   155,     0,   156,
     157,   158,   159,   160,   161,   162,     0,     0,     0,     0,
       0,     0,     0,     0,   295,   296,   205,   260,   261,   262,
     263,   264,     0,   265,     0,   515,     0,     0,     0,     0,
       0,    30,     0,    31,    32,     0,     0,    33,     0,     0,
       0,     0,   268,   269,   270,     0,    36,     0,     0,   271,
      38,     0,     0,     0,     0,     0,     0,     0,   272,    40,
     273,     0,    41,     0,   274,   275,     0,    42,     0,     0,
      43,     0,    44,    45,     0,    46,     0,     0,     0,     0,
       0,     0,    47,     0,     0,    48,     0,     0,     0,    49,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,     0,     0,     0,    55,    56,   277,
       0,    57,    58,    59,    60,    61,    62,    63,     0,     0,
      64,     0,     0,     0,   279,     0,    66,    67,    68,     0,
      69,     0,     0,    70,     0,     0,     0,    71,    72,    73,
       0,    74,    75,     0,    76,    77,   280,    78,     0,    79,
      80,    81,     5,   281,     0,    82,     0,     0,    83,    84,
      85,   282,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,    92,    93,     0,     0,     7,
      94,    95,    96,     0,     0,     0,     0,     0,     0,     0,
      97,     0,    98,    99,   100,   101,     0,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,   110,   111,   112,
     113,   114,     0,     0,     0,   115,   116,   117,   118,   119,
     284,     0,     0,     0,     0,   121,     0,     0,   285,   123,
     124,   125,   126,   286,   287,   288,   289,   290,   291,   132,
     133,   134,   135,   292,   293,   294,     0,     0,     0,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,     0,   154,   155,     0,   156,   157,
     158,   159,   160,   161,   162,     0,     0,     0,     0,     0,
       0,     0,     0,   295,   296,   205,   260,   261,   262,   263,
     264,     0,   265,     0,   372,     0,     0,     0,     0,     0,
      30,     0,    31,    32,     0,     0,    33,     0,     0,     0,
       0,   268,   269,   270,     0,    36,     0,     0,   271,    38,
       0,     0,     0,     0,     0,     0,     0,   272,    40,   273,
       0,    41,     0,   274,   275,     0,    42,     0,     0,    43,
       0,    44,    45,   556,    46,     0,     0,     0,     0,     0,
       0,    47,     0,     0,    48,     0,     0,     0,    49,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,    51,
      52,    53,    54,     0,     0,     0,    55,    56,   277,     0,
      57,    58,    59,    60,    61,    62,    63,     0,     0,    64,
       0,     0,     0,   279,     0,    66,    67,    68,     0,    69,
       0,     0,    70,     0,     0,     0,    71,    72,    73,     0,
      74,    75,     0,    76,    77,   280,    78,     0,    79,    80,
      81,     0,   281,     0,    82,     0,     0,    83,    84,    85,
     282,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,    92,    93,     0,     0,     0,    94,
      95,    96,     0,     0,     0,     0,     0,     0,     0,    97,
       0,    98,    99,   100,   101,     0,   102,   103,   104,   105,
     106,   107,   108,   109,     0,     0,   110,   111,   112,   113,
     114,     0,     0,     0,   115,   116,   117,   118,   119,   284,
       0,     0,     0,     0,   121,     0,     0,   285,   123,   124,
     125,   126,   286,   287,   288,   289,   290,   291,   132,   133,
     134,   135,   292,   293,   294,     0,     0,     0,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,     0,   154,   155,     0,   156,   157,   158,
     159,   160,   161,   162,     0,     0,     0,   557,     0,     0,
       0,     0,   295,   296,   205,   260,   261,   262,   263,   264,
       0,   265,     0,   372,     0,     0,     0,     0,     0,    30,
       0,    31,    32,     0,     0,    33,     0,     0,     0,     0,
     268,   269,   270,     0,    36,     0,     0,   271,    38,     0,
       0,     0,     0,     0,     0,     0,   272,    40,   273,     0,
      41,     0,   274,   275,     0,    42,     0,     0,    43,     0,
      44,    45,     0,    46,     0,     0,     0,     0,     0,     0,
      47,     0,     0,    48,     0,     0,     0,    49,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,     0,     0,     0,    55,    56,   277,     0,    57,
      58,    59,    60,    61,    62,    63,     0,     0,    64,     0,
       0,     0,   279,     0,    66,    67,    68,     0,    69,     0,
       0,    70,     0,     0,     0,    71,    72,    73,     0,    74,
      75,     0,    76,    77,   280,    78,     0,    79,    80,    81,
       0,   281,     0,    82,     0,     0,    83,    84,    85,   282,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,    92,    93,     0,     0,     0,    94,    95,
      96,     0,     0,     0,     0,     0,     0,     0,    97,     0,
      98,    99,   100,   101,     0,   102,   103,   104,   105,   106,
     107,   108,   109,     0,     0,   110,   111,   112,   113,   114,
       0,     0,     0,   115,   116,   117,   118,   119,   284,     0,
       0,     0,     0,   121,     0,     0,   285,   123,   124,   125,
     126,   286,   287,   288,   289,   290,   291,   132,   133,   134,
     135,   292,   293,   294,     0,     0,     0,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,     0,   154,   155,     0,   156,   157,   158,   159,
     160,   161,   162,     0,     0,     0,   373,     0,     0,     0,
       0,   295,   296,   205,   260,   261,   262,   263,   264,     0,
     265,     0,   372,   498,     0,     0,     0,     0,    30,     0,
      31,    32,     0,     0,    33,     0,     0,     0,     0,   268,
     269,   270,     0,    36,     0,     0,   271,    38,     0,     0,
       0,     0,     0,     0,     0,   272,    40,   273,     0,    41,
       0,   274,   275,     0,    42,     0,     0,    43,     0,    44,
      45,     0,    46,     0,     0,     0,     0,     0,     0,    47,
       0,     0,    48,     0,     0,     0,    49,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,     0,     0,     0,    55,    56,   277,     0,    57,    58,
      59,    60,    61,    62,    63,     0,     0,    64,     0,     0,
       0,   279,     0,    66,    67,    68,     0,    69,     0,     0,
      70,     0,     0,     0,    71,    72,    73,     0,    74,    75,
       0,    76,    77,   280,    78,     0,    79,    80,    81,     0,
     281,     0,    82,     0,     0,    83,    84,    85,   282,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,    92,    93,     0,     0,     0,    94,    95,    96,
       0,     0,     0,     0,     0,     0,     0,    97,     0,    98,
      99,   100,   101,     0,   102,   103,   104,   105,   106,   107,
     108,   109,     0,     0,   110,   111,   112,   113,   114,     0,
       0,     0,   115,   116,   117,   118,   119,   284,     0,     0,
       0,     0,   121,     0,     0,   285,   123,   124,   125,   126,
     286,   287,   288,   289,   290,   291,   132,   133,   134,   135,
     292,   293,   294,     0,     0,     0,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,     0,   154,   155,     0,   156,   157,   158,   159,   160,
     161,   162,     0,     0,     0,     0,     0,     0,     0,     0,
     295,   296,   205,   260,   261,   262,   263,   264,     0,   265,
       0,   372,   562,     0,     0,     0,     0,    30,     0,    31,
      32,     0,     0,    33,     0,     0,     0,     0,   268,   269,
     270,     0,    36,     0,     0,   271,    38,     0,     0,     0,
       0,     0,     0,     0,   272,    40,   273,     0,    41,     0,
     274,   275,     0,    42,     0,     0,    43,     0,    44,    45,
       0,    46,     0,     0,     0,     0,     0,     0,    47,     0,
       0,    48,     0,     0,     0,    49,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
       0,     0,     0,    55,    56,   277,     0,    57,    58,    59,
      60,    61,    62,    63,     0,     0,    64,     0,     0,     0,
     279,     0,    66,    67,    68,     0,    69,     0,     0,    70,
       0,     0,     0,    71,    72,    73,     0,    74,    75,     0,
      76,    77,   280,    78,     0,    79,    80,    81,     0,   281,
       0,    82,     0,     0,    83,    84,    85,   282,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,    92,    93,     0,     0,     0,    94,    95,    96,     0,
       0,     0,     0,     0,     0,     0,    97,     0,    98,    99,
     100,   101,     0,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,   110,   111,   112,   113,   114,     0,     0,
       0,   115,   116,   117,   118,   119,   284,     0,     0,     0,
       0,   121,     0,     0,   285,   123,   124,   125,   126,   286,
     287,   288,   289,   290,   291,   132,   133,   134,   135,   292,
     293,   294,     0,     0,     0,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
       0,   154,   155,     0,   156,   157,   158,   159,   160,   161,
     162,     0,     0,     0,     0,     0,     0,     0,     0,   295,
     296,   205,   260,   261,   262,   263,   264,     0,   265,     0,
     372,   642,     0,     0,     0,     0,    30,     0,    31,    32,
       0,     0,    33,     0,     0,     0,     0,   268,   269,   270,
       0,    36,     0,     0,   271,    38,     0,     0,     0,     0,
       0,     0,     0,   272,    40,   273,     0,    41,     0,   274,
     275,     0,    42,     0,     0,    43,     0,    44,    45,     0,
      46,     0,     0,     0,     0,     0,     0,    47,     0,     0,
      48,     0,     0,     0,    49,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,     0,
       0,     0,    55,    56,   277,     0,    57,    58,    59,    60,
      61,    62,    63,     0,     0,    64,     0,     0,     0,   279,
       0,    66,    67,    68,     0,    69,     0,     0,    70,     0,
       0,     0,    71,    72,    73,     0,    74,    75,     0,    76,
      77,   280,    78,     0,    79,    80,    81,     0,   281,     0,
      82,     0,     0,    83,    84,    85,   282,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
      92,    93,     0,     0,     0,    94,    95,    96,     0,     0,
       0,     0,     0,     0,     0,    97,     0,    98,    99,   100,
     101,     0,   102,   103,   104,   105,   106,   107,   108,   109,
       0,     0,   110,   111,   112,   113,   114,     0,     0,     0,
     115,   116,   117,   118,   119,   284,     0,     0,     0,     0,
     121,     0,     0,   285,   123,   124,   125,   126,   286,   287,
     288,   289,   290,   291,   132,   133,   134,   135,   292,   293,
     294,     0,     0,     0,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,     0,
     154,   155,     0,   156,   157,   158,   159,   160,   161,   162,
       0,     0,     0,     0,     0,     0,     0,     0,   295,   296,
     205,   260,   261,   262,   263,   264,     0,   265,     0,   372,
       0,     0,     0,     0,     0,    30,     0,    31,    32,     0,
       0,    33,     0,     0,     0,     0,   268,   269,   270,     0,
      36,     0,     0,   271,    38,     0,     0,     0,     0,     0,
       0,     0,   272,    40,   273,     0,    41,     0,   274,   275,
       0,    42,     0,     0,    43,     0,    44,    45,     0,    46,
       0,     0,     0,     0,     0,     0,    47,     0,     0,    48,
       0,     0,     0,    49,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,     0,     0,
       0,    55,    56,   277,     0,    57,    58,    59,    60,    61,
      62,    63,     0,     0,    64,     0,     0,     0,   279,     0,
      66,    67,    68,     0,    69,     0,     0,    70,     0,     0,
       0,    71,    72,    73,     0,    74,    75,     0,    76,    77,
     280,    78,     0,    79,    80,    81,     0,   281,     0,    82,
       0,     0,    83,    84,    85,   282,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,    92,
      93,     0,     0,     0,    94,    95,    96,     0,     0,     0,
       0,     0,     0,     0,    97,     0,    98,    99,   100,   101,
       0,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,   110,   111,   112,   113,   114,     0,     0,     0,   115,
     116,   117,   118,   119,   284,     0,     0,     0,     0,   121,
       0,     0,   285,   123,   124,   125,   126,   286,   287,   288,
     289,   290,   291,   132,   133,   134,   135,   292,   293,   294,
       0,     0,     0,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,     0,   154,
     155,     0,   156,   157,   158,   159,   160,   161,   162,     0,
       0,     0,     0,     0,     0,     0,     0,   295,   296,   205,
     260,   261,   262,   263,   264,     0,   265,     0,  1160,     0,
       0,     0,     0,     0,    30,     0,    31,    32,     0,     0,
      33,     0,     0,     0,     0,   268,   269,   270,     0,    36,
       0,     0,   271,    38,     0,     0,     0,     0,     0,     0,
       0,   272,    40,   273,     0,    41,     0,   274,   275,     0,
      42,     0,     0,    43,     0,    44,    45,     0,    46,     0,
       0,     0,     0,     0,     0,    47,     0,     0,    48,     0,
       0,     0,    49,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,    51,    52,    53,    54,     0,     0,     0,
      55,    56,   277,     0,    57,    58,    59,    60,    61,    62,
      63,     0,     0,    64,     0,     0,     0,   279,     0,    66,
      67,    68,     0,    69,     0,     0,    70,     0,     0,     0,
      71,    72,    73,     0,    74,    75,     0,    76,    77,   280,
      78,     0,    79,    80,    81,     0,   281,     0,    82,     0,
       0,    83,    84,    85,   282,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,    92,    93,
       0,     0,     0,    94,    95,    96,     0,     0,     0,     0,
       0,     0,     0,    97,     0,    98,    99,   100,   101,     0,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
     110,   111,   112,   113,   114,     0,     0,     0,   115,   116,
     117,   118,   119,   284,     0,     0,     0,     0,   121,     0,
       0,   285,   123,   124,   125,   126,   286,   287,   288,   289,
     290,   291,   132,   133,   134,   135,   292,   293,   294,     0,
       0,     0,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,     0,   154,   155,
       0,   156,   157,   158,   159,   160,   161,   162,     0,     0,
       0,   205,     0,     0,     0,     0,   295,   296,   684,     0,
     810,     0,     0,     0,     0,     0,    30,     0,    31,    32,
       0,     0,    33,     0,     0,     0,     0,     0,    34,    35,
       0,    36,     0,     0,    37,    38,     0,     0,     0,     0,
       0,     0,     0,    39,    40,     0,     0,    41,     0,     0,
       0,     0,    42,     0,     0,    43,     0,    44,    45,     0,
      46,     0,     0,     0,     0,     0,     0,    47,     0,     0,
      48,     0,     0,     0,    49,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,     0,
       0,     0,    55,    56,     0,     0,    57,    58,    59,    60,
      61,    62,    63,     0,     0,    64,     0,     0,     0,    65,
       0,    66,    67,    68,     0,    69,     0,     0,    70,     0,
       0,     0,    71,    72,    73,     0,    74,    75,     0,    76,
      77,     0,    78,     0,    79,    80,    81,     5,     0,     0,
      82,     0,     0,    83,    84,    85,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
      92,    93,     0,     0,     7,    94,    95,    96,     0,     0,
       0,     0,     0,     0,     0,    97,     0,    98,    99,   100,
     101,     0,   102,   103,   104,   105,   106,   107,   108,   109,
       0,   207,   110,   111,   112,   113,   114,     0,     0,     0,
     115,   116,   117,   118,   119,   120,     0,     0,     0,     0,
     121,     0,   208,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,   132,   133,   134,   135,   136,   137,
     138,     0,     0,     0,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,     0,
     154,   155,     0,   156,   157,   158,   159,   160,   161,   162,
     205,     0,     0,     0,     0,     0,   163,   684,     0,   685,
       0,     0,     0,     0,     0,    30,     0,    31,    32,     0,
       0,    33,     0,     0,     0,     0,     0,    34,    35,     0,
      36,     0,     0,    37,    38,     0,     0,     0,     0,     0,
       0,     0,    39,    40,     0,     0,    41,     0,     0,     0,
       0,    42,     0,     0,    43,     0,    44,    45,     0,    46,
       0,     0,     0,     0,     0,     0,    47,     0,     0,    48,
       0,     0,     0,    49,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,     0,     0,
       0,    55,    56,     0,     0,    57,    58,    59,    60,    61,
      62,    63,     0,     0,    64,     0,     0,     0,    65,     0,
      66,    67,    68,     0,    69,     0,     0,    70,     0,     0,
       0,    71,    72,    73,     0,    74,    75,     0,    76,    77,
       0,    78,     0,    79,    80,    81,     0,     0,     0,    82,
       0,     0,    83,    84,    85,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,    92,
      93,     0,     0,     0,    94,    95,    96,     0,     0,     0,
       0,     0,     0,     0,    97,     0,    98,    99,   100,   101,
       0,   102,   103,   104,   105,   106,   107,   108,   109,     0,
     207,   110,   111,   112,   113,   114,     0,     0,     0,   115,
     116,   117,   118,   119,   120,     0,     0,     0,     0,   121,
       0,   208,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     0,   132,   133,   134,   135,   136,   137,   138,
       0,     0,     0,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,     0,   154,
     155,   205,   156,   157,   158,   159,   160,   161,   162,     0,
     206,     0,     0,     0,     0,   163,    30,     0,    31,    32,
       0,     0,    33,     0,     0,     0,     0,     0,    34,    35,
       0,    36,     0,     0,    37,    38,     0,     0,     0,     0,
       0,     0,     0,    39,    40,     0,     0,    41,     0,     0,
       0,     0,    42,     0,     0,    43,     0,    44,    45,     0,
      46,     0,     0,     0,     0,     0,     0,    47,     0,     0,
      48,     0,     0,     0,    49,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,     0,
       0,     0,    55,    56,     0,     0,    57,    58,    59,    60,
      61,    62,    63,     0,     0,    64,     0,     0,     0,    65,
       0,    66,    67,    68,     0,    69,     0,     0,    70,     0,
       0,     0,    71,    72,    73,     0,    74,    75,     0,    76,
      77,     0,    78,     0,    79,    80,    81,     0,     0,     0,
      82,     0,     0,    83,    84,    85,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
      92,    93,     0,     0,     0,    94,    95,    96,     0,     0,
       0,     0,     0,     0,     0,    97,     0,    98,    99,   100,
     101,     0,   102,   103,   104,   105,   106,   107,   108,   109,
       0,   207,   110,   111,   112,   113,   114,     0,     0,     0,
     115,   116,   117,   118,   119,   120,     0,     0,     0,     0,
     121,     0,   208,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,   132,   133,   134,   135,   136,   137,
     138,     0,     0,     0,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,     0,
     154,   155,   205,   156,   157,   158,   159,   160,   161,   162,
       0,   685,     0,     0,     0,     0,   163,    30,     0,    31,
      32,     0,     0,    33,     0,     0,     0,     0,     0,    34,
      35,     0,    36,     0,     0,    37,    38,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,    41,     0,
       0,     0,     0,    42,     0,     0,    43,     0,    44,    45,
       0,    46,     0,     0,     0,     0,     0,     0,    47,     0,
       0,    48,     0,     0,     0,    49,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
       0,     0,     0,    55,    56,     0,     0,    57,    58,    59,
      60,    61,    62,    63,     0,     0,    64,     0,     0,     0,
      65,     0,    66,    67,    68,     0,    69,     0,     0,    70,
       0,     0,     0,    71,    72,    73,     0,    74,    75,     0,
      76,    77,     0,    78,     0,    79,    80,    81,     0,     0,
       0,    82,     0,     0,    83,    84,    85,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,    92,    93,     0,     0,     0,    94,    95,    96,     0,
       0,     0,     0,     0,     0,     0,    97,     0,    98,    99,
     100,   101,     0,   102,   103,   104,   105,   106,   107,   108,
     109,     0,   207,   110,   111,   112,   113,   114,     0,     0,
       0,   115,   116,   117,   118,   119,   120,     0,     0,     0,
       0,   121,     0,   208,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,   132,   133,   134,   135,   136,
     137,   138,     0,     0,     0,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
       0,   154,   155,    29,   156,   157,   158,   159,   160,   161,
     162,     0,     0,     0,     0,     0,     0,   163,    30,     0,
      31,    32,     0,     0,    33,     0,     0,     0,     0,     0,
      34,    35,     0,    36,     0,     0,    37,    38,     0,     0,
       0,     0,     0,     0,     0,    39,    40,     0,     0,    41,
       0,     0,     0,     0,    42,     0,     0,    43,     0,    44,
      45,     0,    46,     0,     0,     0,     0,     0,     0,    47,
       0,     0,    48,     0,     0,     0,    49,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,     0,     0,     0,    55,    56,     0,     0,    57,    58,
      59,    60,    61,    62,    63,     0,     0,    64,     0,     0,
       0,    65,     0,    66,    67,    68,     0,    69,     0,     0,
      70,     0,     0,     0,    71,    72,    73,     0,    74,    75,
       0,    76,    77,     0,    78,     0,    79,    80,    81,     0,
       0,     0,    82,     0,     0,    83,    84,    85,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,    92,    93,     0,     0,     0,    94,    95,    96,
       0,     0,     0,     0,     0,     0,     0,    97,     0,    98,
      99,   100,   101,     0,   102,   103,   104,   105,   106,   107,
     108,   109,     0,     0,   110,   111,   112,   113,   114,     0,
       0,     0,   115,   116,   117,   118,   119,   120,     0,     0,
       0,     0,   121,     0,     0,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     0,   132,   133,   134,   135,
     136,   137,   138,     0,     0,     0,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,     0,   154,   155,   205,   156,   157,   158,   159,   160,
     161,   162,     0,     0,     0,   645,     0,     0,   163,    30,
       0,    31,    32,     0,     0,    33,     0,     0,     0,     0,
       0,    34,    35,     0,    36,     0,     0,    37,    38,     0,
       0,     0,     0,     0,     0,     0,    39,    40,     0,     0,
      41,     0,     0,     0,     0,    42,     0,     0,    43,     0,
      44,    45,     0,    46,     0,     0,     0,     0,     0,     0,
      47,     0,     0,    48,     0,     0,     0,    49,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,     0,     0,     0,    55,    56,     0,     0,    57,
      58,    59,    60,    61,    62,    63,     0,     0,    64,     0,
       0,     0,    65,     0,    66,    67,    68,     0,    69,     0,
       0,    70,     0,     0,     0,    71,    72,    73,     0,    74,
      75,     0,    76,    77,     0,    78,     0,    79,    80,    81,
       0,     0,     0,    82,     0,     0,    83,    84,    85,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,    92,    93,     0,     0,     0,    94,    95,
      96,     0,     0,     0,     0,     0,     0,     0,    97,     0,
      98,    99,   100,   101,     0,   102,   103,   104,   105,   106,
     107,   108,   109,     0,     0,   110,   111,   112,   113,   114,
       0,     0,     0,   115,   116,   117,   118,   119,   120,     0,
       0,     0,     0,   121,     0,     0,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,     0,   132,   133,   134,
     135,   136,   137,   138,     0,     0,     0,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    29,   154,   155,     0,   156,   157,   158,   159,
     160,   161,   162,   645,     0,     0,   646,    30,     0,    31,
      32,     0,     0,    33,     0,     0,     0,     0,     0,    34,
      35,     0,    36,     0,     0,    37,    38,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,    41,     0,
       0,     0,     0,    42,     0,     0,    43,     0,    44,    45,
       0,    46,     0,     0,     0,     0,     0,     0,    47,     0,
       0,    48,     0,     0,     0,    49,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
       0,     0,     0,    55,    56,     0,     0,    57,    58,    59,
      60,    61,    62,    63,     0,     0,    64,     0,     0,     0,
      65,     0,    66,    67,    68,     0,    69,     0,     0,    70,
       0,     0,     0,    71,    72,    73,     0,    74,    75,     0,
      76,    77,     0,    78,     0,    79,    80,    81,     0,     0,
       0,    82,     0,     0,    83,    84,    85,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,    92,    93,     0,     0,     0,    94,    95,    96,     0,
       0,     0,     0,     0,     0,     0,    97,     0,    98,    99,
     100,   101,     0,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,   110,   111,   112,   113,   114,     0,     0,
       0,   115,   116,   117,   118,   119,   120,     0,     0,     0,
       0,   121,     0,     0,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,   132,   133,   134,   135,   136,
     137,   138,     0,     0,     0,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     205,   154,   155,     0,   156,   157,   158,   159,   160,   161,
     162,   879,     0,     0,   646,    30,     0,    31,    32,     0,
       0,    33,     0,     0,     0,     0,     0,    34,    35,     0,
      36,     0,     0,    37,    38,     0,     0,     0,     0,     0,
       0,     0,    39,    40,     0,     0,    41,     0,     0,     0,
       0,    42,     0,     0,    43,     0,    44,    45,     0,    46,
       0,     0,     0,     0,     0,     0,    47,     0,     0,    48,
       0,     0,     0,    49,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,     0,     0,
       0,    55,    56,     0,     0,    57,    58,    59,    60,    61,
      62,    63,     0,     0,    64,     0,     0,     0,    65,     0,
      66,    67,    68,     0,    69,     0,     0,    70,     0,     0,
       0,    71,    72,    73,     0,    74,    75,     0,    76,    77,
       0,    78,     0,    79,    80,    81,     0,     0,     0,    82,
       0,     0,    83,    84,    85,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,    92,
      93,     0,     0,     0,    94,    95,    96,     0,     0,     0,
       0,     0,     0,     0,    97,     0,    98,    99,   100,   101,
       0,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,   110,   111,   112,   113,   114,     0,     0,     0,   115,
     116,   117,   118,   119,   120,     0,     0,     0,     0,   121,
       0,     0,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     0,   132,   133,   134,   135,   136,   137,   138,
       0,     0,     0,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,    29,   154,
     155,     0,   156,   157,   158,   159,   160,   161,   162,   879,
       0,     0,   880,    30,     0,    31,    32,     0,     0,    33,
       0,     0,     0,     0,     0,    34,    35,     0,    36,     0,
       0,    37,    38,     0,     0,     0,     0,     0,     0,     0,
      39,    40,     0,     0,    41,     0,     0,     0,     0,    42,
       0,     0,    43,     0,    44,    45,     0,    46,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,     0,     0,
       0,    49,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,    58,    59,    60,    61,    62,    63,
       0,     0,    64,     0,     0,     0,    65,     0,    66,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
      72,    73,     0,    74,    75,     0,    76,    77,     0,    78,
       0,    79,    80,    81,     0,     0,     0,    82,     0,     0,
      83,    84,    85,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,    92,    93,     0,
       0,     0,    94,    95,    96,     0,     0,     0,     0,     0,
       0,     0,    97,     0,    98,    99,   100,   101,     0,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,   110,
     111,   112,   113,   114,     0,     0,     0,   115,   116,   117,
     118,   119,   120,     0,     0,     0,     0,   121,     0,     0,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,   132,   133,   134,   135,   136,   137,   138,     0,     0,
       0,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    29,   154,   155,     0,
     156,   157,   158,   159,   160,   161,   162,     0,     0,     0,
     880,    30,     0,    31,    32,     0,     0,    33,     0,     0,
       0,     0,     0,    34,    35,     0,    36,     0,     0,    37,
      38,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,    41,     0,     0,     0,     0,    42,     0,     0,
      43,     0,    44,    45,     0,    46,     0,     0,     0,     0,
       0,     0,    47,     0,     0,    48,     0,     0,     0,    49,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,     0,     0,     0,    55,    56,     0,
       0,    57,    58,    59,    60,    61,    62,    63,     0,     0,
      64,     0,     0,     0,    65,     0,    66,    67,    68,     0,
      69,     0,     0,    70,     0,     0,     0,    71,    72,    73,
       0,    74,    75,     0,    76,    77,     0,    78,     0,    79,
      80,    81,     0,     0,     0,    82,     0,     0,    83,    84,
      85,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,    92,    93,     0,     0,     0,
      94,    95,    96,     0,     0,     0,     0,     0,     0,     0,
      97,     0,    98,    99,   100,   101,     0,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,   110,   111,   112,
     113,   114,     0,     0,     0,   115,   116,   117,   118,   119,
     120,     0,     0,     0,     0,   121,     0,     0,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,   132,
     133,   134,   135,   136,   137,   138,     0,     0,     0,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,    29,   154,   155,     0,   156,   157,
     158,   159,   160,   161,   162,     0,     0,     0,   958,    30,
       0,    31,    32,     0,     0,    33,     0,     0,     0,     0,
       0,    34,    35,     0,    36,     0,     0,    37,    38,     0,
       0,     0,     0,     0,     0,     0,    39,    40,     0,     0,
      41,     0,     0,     0,     0,    42,     0,     0,    43,     0,
      44,    45,     0,    46,     0,     0,     0,     0,     0,     0,
      47,     0,     0,    48,     0,     0,     0,    49,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,     0,     0,     0,    55,    56,     0,     0,    57,
      58,    59,    60,    61,    62,    63,     0,     0,    64,     0,
       0,     0,    65,     0,    66,    67,    68,     0,    69,     0,
       0,    70,     0,     0,     0,    71,    72,    73,     0,    74,
      75,     0,    76,    77,     0,    78,     0,    79,    80,    81,
       0,     0,     0,    82,     0,     0,    83,    84,    85,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,    92,    93,     0,     0,     0,    94,    95,
      96,     0,     0,     0,     0,     0,     0,     0,    97,     0,
      98,    99,   100,   101,     0,   102,   103,   104,   105,   106,
     107,   108,   109,     0,     0,   110,   111,   112,   113,   114,
       0,     0,     0,   115,   116,   117,   118,   119,   120,     0,
       0,     0,     0,   121,     0,     0,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,     0,   132,   133,   134,
     135,   136,   137,   138,     0,     0,     0,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   205,   154,   155,     0,   156,   157,   158,   159,
     160,   161,   162,     0,     0,     0,  1006,    30,     0,    31,
      32,     0,     0,    33,     0,     0,     0,     0,     0,    34,
      35,     0,    36,     0,     0,    37,    38,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,    41,     0,
       0,     0,     0,    42,     0,     0,    43,     0,    44,    45,
       0,    46,     0,     0,     0,     0,     0,     0,    47,     0,
       0,    48,     0,     0,     0,    49,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
       0,     0,     0,    55,    56,     0,     0,    57,    58,    59,
      60,    61,    62,    63,     0,     0,    64,     0,     0,     0,
      65,     0,    66,    67,    68,     0,    69,     0,     0,    70,
       0,     0,     0,    71,    72,    73,     0,    74,    75,     0,
      76,    77,     0,    78,     0,    79,    80,    81,     0,     0,
       0,    82,     0,     0,    83,    84,    85,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,    92,    93,     0,     0,     0,    94,    95,    96,     0,
       0,     0,     0,     0,     0,     0,    97,     0,    98,    99,
     100,   101,     0,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,   110,   111,   112,   113,   114,     0,     0,
       0,   115,   116,   117,   118,   119,   120,     0,     0,     0,
       0,   121,     0,     0,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,   132,   133,   134,   135,   136,
     137,   138,     0,     0,     0,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
      29,   154,   155,     0,   156,   157,   158,   159,   160,   161,
     162,     0,     0,     0,  1010,    30,     0,    31,    32,     0,
       0,    33,     0,     0,     0,     0,     0,    34,    35,     0,
      36,     0,     0,    37,    38,     0,     0,     0,     0,     0,
       0,     0,    39,    40,     0,     0,    41,     0,     0,     0,
       0,    42,     0,     0,    43,     0,    44,    45,     0,    46,
       0,     0,     0,     0,     0,     0,    47,     0,     0,    48,
       0,     0,     0,    49,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,     0,     0,
       0,    55,    56,     0,     0,    57,    58,    59,    60,    61,
      62,    63,     0,     0,    64,     0,     0,     0,    65,     0,
      66,    67,    68,     0,    69,     0,     0,    70,     0,     0,
       0,    71,    72,    73,     0,    74,    75,     0,    76,    77,
       0,    78,     0,    79,    80,    81,     0,     0,     0,    82,
       0,     0,    83,    84,    85,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,    92,
      93,     0,     0,     0,    94,    95,    96,     0,     0,     0,
       0,     0,     0,     0,    97,     0,    98,    99,   100,   101,
       0,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,   110,   111,   112,   113,   114,     0,     0,     0,   115,
     116,   117,   118,   119,   120,     0,     0,     0,     0,   121,
       0,     0,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     0,   132,   133,   134,   135,   136,   137,   138,
       0,     0,     0,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,    29,   154,
     155,     0,   156,   157,   158,   159,   160,   161,   162,     0,
       0,     0,  1057,    30,     0,    31,    32,     0,     0,    33,
       0,     0,     0,     0,     0,    34,    35,     0,    36,     0,
       0,    37,    38,     0,     0,     0,     0,     0,     0,     0,
      39,    40,     0,     0,    41,     0,     0,     0,     0,    42,
       0,     0,    43,     0,    44,    45,     0,    46,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,     0,     0,
       0,    49,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,    58,    59,    60,    61,    62,    63,
       0,     0,    64,     0,     0,     0,    65,     0,    66,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
      72,    73,     0,    74,    75,     0,    76,    77,     0,    78,
       0,    79,    80,    81,     0,     0,     0,    82,     0,     0,
      83,    84,    85,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,    92,    93,     0,
       0,     0,    94,    95,    96,     0,     0,     0,     0,     0,
       0,     0,    97,     0,    98,    99,   100,   101,     0,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,   110,
     111,   112,   113,   114,     0,     0,     0,   115,   116,   117,
     118,   119,   120,     0,     0,     0,     0,   121,     0,     0,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,   132,   133,   134,   135,   136,   137,   138,     0,     0,
       0,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    29,   154,   155,     0,
     156,   157,   158,   159,   160,   161,   162,     0,     0,     0,
    1059,    30,     0,    31,    32,     0,     0,    33,     0,     0,
       0,     0,     0,    34,    35,     0,    36,     0,     0,    37,
      38,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,    41,     0,     0,     0,     0,    42,     0,     0,
      43,     0,    44,    45,     0,    46,     0,     0,     0,     0,
       0,     0,    47,     0,     0,    48,     0,     0,     0,    49,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,     0,     0,     0,    55,    56,     0,
       0,    57,    58,    59,    60,    61,    62,    63,     0,     0,
      64,     0,     0,     0,    65,     0,    66,    67,    68,     0,
      69,     0,     0,    70,     0,     0,     0,    71,    72,    73,
       0,    74,    75,     0,    76,    77,     0,    78,     0,    79,
      80,    81,     0,     0,     0,    82,     0,     0,    83,    84,
      85,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,    92,    93,     0,     0,     0,
      94,    95,    96,     0,     0,     0,     0,     0,     0,     0,
      97,     0,    98,    99,   100,   101,     0,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,   110,   111,   112,
     113,   114,     0,     0,     0,   115,   116,   117,   118,   119,
     120,     0,     0,     0,     0,   121,     0,     0,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,   132,
     133,   134,   135,   136,   137,   138,     0,     0,     0,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,    29,   154,   155,     0,   156,   157,
     158,   159,   160,   161,   162,     0,     0,     0,  1010,    30,
       0,    31,    32,     0,     0,    33,     0,     0,     0,     0,
       0,    34,    35,     0,    36,     0,     0,    37,    38,     0,
       0,     0,     0,     0,     0,     0,    39,    40,     0,     0,
      41,     0,     0,     0,     0,    42,     0,     0,    43,     0,
      44,    45,     0,    46,     0,     0,     0,     0,     0,     0,
      47,     0,     0,    48,     0,     0,     0,    49,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,     0,     0,     0,    55,    56,     0,     0,    57,
      58,    59,    60,    61,    62,    63,     0,     0,    64,     0,
       0,     0,    65,     0,    66,    67,    68,     0,    69,     0,
       0,    70,     0,     0,     0,    71,    72,    73,     0,    74,
      75,     0,    76,    77,     0,    78,     0,    79,    80,    81,
       0,     0,     0,    82,     0,     0,    83,    84,    85,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,    92,    93,     0,     0,     0,    94,    95,
      96,     0,     0,     0,     0,     0,     0,     0,    97,     0,
      98,    99,   100,   101,     0,   102,   103,   104,   105,   106,
     107,   108,   109,     0,     0,   110,   111,   112,   113,   114,
       0,     0,     0,   115,   116,   117,   118,   119,   120,     0,
       0,     0,     0,   121,     0,     0,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,     0,   132,   133,   134,
     135,   136,   137,   138,     0,     0,     0,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   648,   154,   155,     0,   156,   157,   158,   159,
     160,   161,   162,     0,     0,     0,  1102,    30,     0,    31,
      32,     0,     0,    33,     0,     0,     0,     0,     0,   269,
     270,     0,    36,     0,     0,   271,    38,     0,     0,     0,
       0,     0,     0,     0,   272,    40,   273,     0,    41,     0,
     274,   275,     0,    42,     0,     0,    43,     0,    44,    45,
       0,    46,     0,     0,     0,     0,     0,     0,    47,     0,
       0,    48,     0,     0,     0,    49,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
       0,     0,     0,    55,    56,   277,     0,    57,    58,    59,
      60,    61,    62,    63,     0,     0,    64,     0,     0,     0,
     279,     0,    66,    67,    68,     0,    69,     0,     0,    70,
       0,     0,     0,    71,    72,    73,     0,    74,    75,     0,
      76,    77,   280,    78,     0,    79,    80,    81,     0,   281,
       0,    82,     0,     0,    83,    84,    85,   282,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,    92,    93,     0,     0,     0,    94,    95,    96,     0,
       0,     0,     0,     0,     0,     0,    97,     0,    98,    99,
     100,   101,     0,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,   110,   111,   112,   113,   114,     0,     0,
       0,   115,   116,   117,   118,   119,   284,     0,     0,     0,
       0,   121,     0,     0,   285,   123,   124,   125,   126,   286,
     287,   288,   289,   290,   291,   132,   133,   134,   135,   292,
     293,   294,     0,     0,     0,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
       0,   154,   155,     0,   156,   157,   158,   159,   160,   161,
     162,    29,   260,   261,   262,   263,   264,     0,   265,     0,
       0,     0,     0,     0,     0,     0,    30,     0,    31,    32,
       0,     0,    33,     0,     0,     0,     0,     0,    34,    35,
       0,    36,     0,     0,    37,    38,     0,     0,     0,     0,
       0,     0,     0,    39,    40,     0,     0,    41,     0,     0,
       0,     0,    42,     0,     0,    43,     0,    44,    45,     0,
      46,     0,     0,     0,     0,     0,     0,    47,     0,     0,
      48,     0,     0,     0,    49,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,     0,
       0,     0,    55,    56,     0,     0,    57,    58,    59,    60,
      61,    62,    63,     0,     0,    64,     0,     0,     0,    65,
       0,    66,    67,    68,     0,    69,     0,     0,    70,     0,
       0,     0,    71,    72,    73,     0,    74,    75,     0,    76,
      77,     0,    78,     0,    79,    80,    81,     0,     0,     0,
      82,     0,     0,    83,    84,    85,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
      92,    93,     0,     0,     0,    94,    95,    96,     0,     0,
       0,     0,     0,     0,     0,    97,     0,    98,    99,   100,
     101,     0,   102,   103,   104,   105,   106,   107,   108,   109,
       0,     0,   110,   111,   112,   113,   114,     0,     0,     0,
     115,   116,   117,   118,   119,   120,     0,     0,     0,     0,
     121,     0,     0,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,   132,   133,   134,   135,   136,   137,
     138,     0,     0,     0,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,     0,
     154,   155,     0,   156,   157,   158,   159,   160,   161,   162,
      29,   231,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,   232,    31,    32,     0,
       0,    33,     0,     0,     0,     0,     0,    34,    35,     0,
      36,     0,     0,    37,    38,     0,     0,     0,     0,     0,
       0,     0,    39,    40,     0,     0,    41,     0,     0,     0,
       0,    42,     0,     0,    43,     0,    44,    45,     0,    46,
       0,     0,     0,     0,     0,     0,    47,     0,     0,    48,
       0,     0,     0,    49,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,     0,     0,
       0,    55,    56,     0,     0,    57,    58,    59,    60,    61,
      62,    63,     0,     0,    64,     0,     0,     0,    65,     0,
      66,    67,    68,     0,    69,     0,     0,    70,     0,     0,
     233,    71,    72,    73,     0,    74,    75,     0,    76,    77,
       0,    78,     0,    79,    80,    81,     0,     0,     0,    82,
       0,     0,    83,    84,    85,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,    92,
      93,     0,     0,     0,    94,    95,    96,     0,     0,     0,
       0,     0,     0,     0,    97,     0,    98,    99,   100,   101,
       0,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,   110,   111,   112,   113,   114,     0,     0,     0,   115,
     116,   117,   118,   119,   120,     0,     0,     0,     0,   121,
       0,     0,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     0,   132,   133,   134,   135,   136,   137,   138,
       0,     0,     0,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,     0,   154,
     155,    29,   156,   157,   158,   159,   160,   161,   162,     0,
       1,     0,     0,     0,     0,     0,    30,     0,    31,    32,
       0,     0,    33,     0,     0,     0,     0,     0,    34,    35,
       0,    36,     0,     0,    37,    38,     0,     0,     0,     0,
       0,     0,     0,    39,    40,     0,     0,    41,     0,     0,
       0,     0,    42,     0,     0,    43,     0,    44,    45,     0,
      46,     0,     0,     0,     0,     0,     0,    47,     0,     0,
      48,     0,     0,     0,    49,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,     0,
       0,     0,    55,    56,     0,     0,    57,    58,    59,    60,
      61,    62,    63,     0,     0,    64,     0,     0,     0,    65,
       0,    66,    67,    68,     0,    69,     0,     0,    70,     0,
       0,     0,    71,    72,    73,     0,    74,    75,     0,    76,
      77,     0,    78,     0,    79,    80,    81,     5,     0,     0,
      82,     0,     0,    83,    84,    85,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
      92,    93,     0,     0,     7,    94,    95,    96,     0,     0,
       0,     0,     0,     0,     0,    97,     0,    98,    99,   100,
     101,     0,   102,   103,   104,   105,   106,   107,   108,   109,
       0,     0,   110,   111,   112,   113,   114,     0,     0,     0,
     115,   116,   117,   118,   119,   120,     0,     0,     0,     0,
     121,     0,     0,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,   132,   133,   134,   135,   136,   137,
     138,     0,     0,     0,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,     0,
     154,   155,   205,   156,   157,   158,   159,   160,   161,   162,
       0,     1,     0,     0,     0,     0,     0,    30,     0,    31,
      32,     0,     0,    33,     0,     0,     0,     0,     0,    34,
      35,     0,    36,     0,     0,    37,    38,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,    41,     0,
       0,     0,     0,    42,     0,     0,    43,     0,    44,    45,
       0,    46,     0,     0,     0,     0,     0,     0,    47,     0,
       0,    48,     0,     0,     0,    49,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
       0,     0,     0,    55,    56,     0,     0,    57,    58,    59,
      60,    61,    62,    63,     0,     0,    64,     0,     0,     0,
      65,     0,    66,    67,    68,     0,    69,     0,     0,    70,
       0,     0,     0,    71,    72,    73,     0,    74,    75,     0,
      76,    77,     0,    78,     0,    79,    80,    81,     5,     0,
       0,    82,     0,     0,    83,    84,    85,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,    92,    93,     0,     0,     7,    94,    95,    96,     0,
       0,     0,     0,     0,     0,     0,    97,     0,    98,    99,
     100,   101,     0,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,   110,   111,   112,   113,   114,     0,     0,
       0,   115,   116,   117,   118,   119,   120,     0,     0,     0,
       0,   121,     0,     0,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,   132,   133,   134,   135,   136,
     137,   138,     0,     0,     0,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
       0,   154,   155,     0,   156,   157,   158,   159,   160,   161,
     162,    29,   231,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    30,   232,    31,    32,
       0,     0,    33,     0,     0,     0,     0,     0,    34,    35,
       0,    36,     0,     0,    37,    38,     0,     0,     0,     0,
       0,     0,     0,    39,    40,     0,     0,    41,     0,     0,
       0,     0,    42,     0,     0,    43,     0,    44,    45,     0,
      46,     0,     0,     0,     0,     0,     0,    47,     0,     0,
      48,     0,     0,     0,    49,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,     0,
       0,     0,    55,    56,     0,     0,    57,    58,    59,    60,
      61,    62,    63,     0,     0,    64,     0,     0,     0,    65,
       0,    66,    67,    68,     0,    69,     0,     0,    70,     0,
       0,     0,    71,    72,    73,     0,    74,    75,     0,    76,
      77,     0,    78,     0,    79,    80,    81,     0,     0,     0,
      82,     0,     0,    83,    84,    85,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
      92,    93,     0,     0,     0,    94,    95,    96,     0,     0,
       0,     0,     0,     0,     0,    97,     0,    98,    99,   100,
     101,     0,   102,   103,   104,   105,   106,   107,   108,   109,
       0,     0,   110,   111,   112,   113,   114,     0,     0,     0,
     115,   116,   117,   118,   119,   120,     0,     0,     0,     0,
     121,     0,     0,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,   132,   133,   134,   135,   136,   137,
     138,     0,     0,     0,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    29,
     154,   155,     0,   156,   157,   158,   159,   160,   161,   162,
     339,     0,     0,     0,    30,     0,    31,    32,     0,     0,
      33,     0,     0,     0,     0,     0,    34,    35,     0,    36,
       0,     0,    37,    38,     0,     0,     0,     0,     0,     0,
       0,    39,    40,     0,     0,    41,     0,     0,     0,     0,
      42,     0,     0,    43,     0,    44,    45,     0,    46,     0,
       0,     0,     0,     0,     0,    47,     0,     0,    48,     0,
       0,     0,    49,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,    51,    52,    53,    54,     0,     0,     0,
      55,    56,     0,     0,    57,    58,    59,    60,    61,    62,
      63,     0,     0,    64,     0,     0,     0,    65,     0,    66,
      67,    68,     0,    69,     0,     0,    70,     0,     0,     0,
      71,    72,    73,     0,    74,    75,     0,    76,    77,     0,
      78,     0,    79,    80,    81,     0,     0,     0,    82,     0,
       0,    83,    84,    85,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,    92,    93,
       0,     0,     0,    94,    95,    96,     0,     0,     0,     0,
       0,     0,     0,    97,     0,    98,    99,   100,   101,     0,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
     110,   111,   112,   113,   114,     0,     0,     0,   115,   116,
     117,   118,   119,   120,     0,     0,     0,     0,   121,     0,
       0,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,   132,   133,   134,   135,   136,   137,   138,     0,
       0,     0,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,     0,   154,   155,
       0,   156,   157,   158,   159,   160,   161,   162,    29,   231,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    30,     0,    31,    32,     0,     0,    33,
       0,     0,     0,     0,     0,    34,    35,     0,    36,     0,
       0,    37,    38,     0,     0,     0,     0,     0,     0,     0,
      39,    40,     0,     0,    41,     0,     0,     0,     0,    42,
       0,     0,    43,     0,    44,    45,     0,    46,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,     0,     0,
       0,    49,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,    58,    59,    60,    61,    62,    63,
       0,     0,    64,     0,     0,     0,    65,     0,    66,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
      72,    73,     0,    74,    75,     0,    76,    77,     0,    78,
       0,    79,    80,    81,     0,     0,     0,    82,     0,     0,
      83,    84,    85,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,    92,    93,     0,
       0,     0,    94,    95,    96,     0,     0,     0,     0,     0,
       0,     0,    97,     0,    98,    99,   100,   101,     0,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,   110,
     111,   112,   113,   114,     0,     0,     0,   115,   116,   117,
     118,   119,   120,     0,     0,     0,     0,   121,     0,     0,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,   132,   133,   134,   135,   136,   137,   138,     0,     0,
       0,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    29,   154,   155,     0,
     156,   157,   158,   159,   160,   161,   162,   475,     0,     0,
       0,    30,     0,    31,    32,     0,     0,    33,     0,     0,
       0,     0,     0,    34,    35,     0,    36,     0,     0,    37,
      38,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,    41,     0,     0,     0,     0,    42,     0,     0,
      43,     0,    44,    45,     0,    46,     0,     0,     0,     0,
       0,     0,    47,     0,     0,    48,     0,     0,     0,    49,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,     0,     0,     0,    55,    56,     0,
       0,    57,    58,    59,    60,    61,    62,    63,     0,     0,
      64,     0,     0,     0,    65,     0,    66,    67,    68,     0,
      69,     0,     0,    70,     0,     0,     0,    71,    72,    73,
       0,    74,    75,     0,    76,    77,     0,    78,     0,    79,
      80,    81,     0,     0,     0,    82,     0,     0,    83,    84,
      85,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,    92,    93,     0,     0,     0,
      94,    95,    96,     0,     0,     0,     0,     0,     0,     0,
      97,     0,    98,    99,   100,   101,     0,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,   110,   111,   112,
     113,   114,     0,     0,     0,   115,   116,   117,   118,   119,
     120,     0,     0,     0,     0,   121,     0,     0,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,   132,
     133,   134,   135,   136,   137,   138,     0,     0,     0,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   205,   154,   155,     0,   156,   157,
     158,   159,   160,   161,   162,   339,     0,     0,     0,    30,
       0,    31,    32,     0,     0,    33,     0,     0,     0,     0,
       0,    34,    35,     0,    36,     0,     0,    37,    38,     0,
       0,     0,     0,     0,     0,     0,    39,    40,     0,     0,
      41,     0,     0,     0,     0,    42,     0,     0,    43,     0,
      44,    45,     0,    46,     0,     0,     0,     0,     0,     0,
      47,     0,     0,    48,     0,     0,     0,    49,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,     0,     0,     0,    55,    56,     0,     0,    57,
      58,    59,    60,    61,    62,    63,     0,     0,    64,     0,
       0,     0,    65,     0,    66,    67,    68,     0,    69,     0,
       0,    70,     0,     0,     0,    71,    72,    73,     0,    74,
      75,     0,    76,    77,     0,    78,     0,    79,    80,    81,
       0,     0,     0,    82,     0,     0,    83,    84,    85,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,    92,    93,     0,     0,     0,    94,    95,
      96,     0,     0,     0,     0,     0,     0,     0,    97,     0,
      98,    99,   100,   101,     0,   102,   103,   104,   105,   106,
     107,   108,   109,     0,     0,   110,   111,   112,   113,   114,
       0,     0,     0,   115,   116,   117,   118,   119,   120,     0,
       0,     0,     0,   121,     0,     0,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,     0,   132,   133,   134,
     135,   136,   137,   138,     0,     0,     0,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    29,   154,   155,     0,   156,   157,   158,   159,
     160,   161,   162,   656,     0,     0,     0,    30,     0,    31,
      32,     0,     0,    33,     0,     0,     0,     0,     0,    34,
      35,     0,    36,     0,     0,    37,    38,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,    41,     0,
       0,     0,     0,    42,     0,     0,    43,     0,    44,    45,
       0,    46,     0,     0,     0,     0,     0,     0,    47,     0,
       0,    48,     0,     0,     0,    49,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
       0,     0,     0,    55,    56,     0,     0,    57,    58,    59,
      60,    61,    62,    63,     0,     0,    64,     0,     0,     0,
      65,     0,    66,    67,    68,     0,    69,     0,     0,    70,
       0,     0,     0,    71,    72,    73,     0,    74,    75,     0,
      76,    77,     0,    78,     0,    79,    80,    81,     0,     0,
       0,    82,     0,     0,    83,    84,    85,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,    92,    93,     0,     0,     0,    94,    95,    96,     0,
       0,     0,     0,     0,     0,     0,    97,     0,    98,    99,
     100,   101,     0,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,   110,   111,   112,   113,   114,     0,     0,
       0,   115,   116,   117,   118,   119,   120,     0,     0,     0,
       0,   121,     0,     0,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,   132,   133,   134,   135,   136,
     137,   138,     0,     0,     0,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
      29,   154,   155,     0,   156,   157,   158,   159,   160,   161,
     162,   773,     0,     0,     0,    30,     0,    31,    32,     0,
       0,    33,     0,     0,     0,     0,     0,    34,    35,     0,
      36,     0,     0,    37,    38,     0,     0,     0,     0,     0,
       0,     0,    39,    40,     0,     0,    41,     0,     0,     0,
       0,    42,     0,     0,    43,     0,    44,    45,     0,    46,
       0,     0,     0,     0,     0,     0,    47,     0,     0,    48,
       0,     0,     0,    49,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,     0,     0,
       0,    55,    56,     0,     0,    57,    58,    59,    60,    61,
      62,    63,     0,     0,    64,     0,     0,     0,    65,     0,
      66,    67,    68,     0,    69,     0,     0,    70,     0,     0,
       0,    71,    72,    73,     0,    74,    75,     0,    76,    77,
       0,    78,     0,    79,    80,    81,     0,     0,     0,    82,
       0,     0,    83,    84,    85,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,    92,
      93,     0,     0,     0,    94,    95,    96,     0,     0,     0,
       0,     0,     0,     0,    97,     0,    98,    99,   100,   101,
       0,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,   110,   111,   112,   113,   114,     0,     0,     0,   115,
     116,   117,   118,   119,   120,     0,     0,     0,     0,   121,
       0,     0,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     0,   132,   133,   134,   135,   136,   137,   138,
       0,     0,     0,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   205,   154,
     155,     0,   156,   157,   158,   159,   160,   161,   162,   656,
       0,     0,     0,    30,     0,    31,    32,     0,     0,    33,
       0,     0,     0,     0,     0,    34,    35,     0,    36,     0,
       0,    37,    38,     0,     0,     0,     0,     0,     0,     0,
      39,    40,     0,     0,    41,     0,     0,     0,     0,    42,
       0,     0,    43,     0,    44,    45,     0,    46,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,     0,     0,
       0,    49,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,    58,    59,    60,    61,    62,    63,
       0,     0,    64,     0,     0,     0,    65,     0,    66,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
      72,    73,     0,    74,    75,     0,    76,    77,     0,    78,
       0,    79,    80,    81,     0,     0,     0,    82,     0,     0,
      83,    84,    85,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,    92,    93,     0,
       0,     0,    94,    95,    96,     0,     0,     0,     0,     0,
       0,     0,    97,     0,    98,    99,   100,   101,     0,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,   110,
     111,   112,   113,   114,     0,     0,     0,   115,   116,   117,
     118,   119,   120,     0,     0,     0,     0,   121,     0,     0,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,   132,   133,   134,   135,   136,   137,   138,     0,     0,
       0,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,     0,   154,   155,     0,
     156,   157,   158,   159,   160,   161,   162,    29,   869,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    30,     0,    31,    32,     0,     0,    33,     0,
       0,     0,     0,     0,    34,    35,     0,    36,     0,     0,
      37,    38,     0,     0,     0,     0,     0,     0,     0,    39,
      40,     0,     0,    41,     0,     0,     0,     0,    42,     0,
       0,    43,     0,    44,    45,     0,    46,     0,     0,     0,
       0,     0,     0,    47,     0,     0,    48,     0,     0,     0,
      49,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,    51,    52,    53,    54,     0,     0,     0,    55,    56,
       0,     0,    57,    58,    59,    60,    61,    62,    63,     0,
       0,    64,     0,     0,     0,    65,     0,    66,    67,    68,
       0,    69,     0,     0,    70,     0,     0,     0,    71,    72,
      73,     0,    74,    75,     0,    76,    77,     0,    78,     0,
      79,    80,    81,     0,     0,     0,    82,     0,     0,    83,
      84,    85,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,    92,    93,     0,     0,
       0,    94,    95,    96,     0,     0,     0,     0,     0,     0,
       0,    97,     0,    98,    99,   100,   101,     0,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,   110,   111,
     112,   113,   114,     0,     0,     0,   115,   116,   117,   118,
     119,   120,     0,     0,     0,     0,   121,     0,     0,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     0,
     132,   133,   134,   135,   136,   137,   138,     0,     0,     0,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,     0,   154,   155,    29,   156,
     157,   158,   159,   160,   161,   162,     0,   872,     0,     0,
       0,     0,     0,    30,     0,    31,    32,     0,     0,    33,
       0,     0,     0,     0,     0,    34,    35,     0,    36,     0,
       0,    37,    38,     0,     0,     0,     0,     0,     0,     0,
      39,    40,     0,     0,    41,     0,     0,     0,     0,    42,
       0,     0,    43,     0,    44,    45,     0,    46,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,     0,     0,
       0,    49,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,    58,    59,    60,    61,    62,    63,
       0,     0,    64,     0,     0,     0,    65,     0,    66,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
      72,    73,     0,    74,    75,     0,    76,    77,     0,    78,
       0,    79,    80,    81,     0,     0,     0,    82,     0,     0,
      83,    84,    85,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,    92,    93,     0,
       0,     0,    94,    95,    96,     0,     0,     0,     0,     0,
       0,     0,    97,     0,    98,    99,   100,   101,     0,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,   110,
     111,   112,   113,   114,     0,     0,     0,   115,   116,   117,
     118,   119,   120,     0,     0,     0,     0,   121,     0,     0,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,   132,   133,   134,   135,   136,   137,   138,     0,     0,
       0,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,     0,   154,   155,     0,
     156,   157,   158,   159,   160,   161,   162,    29,     0,   930,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    30,     0,    31,    32,     0,     0,    33,     0,
       0,     0,     0,     0,    34,    35,     0,    36,     0,     0,
      37,    38,     0,     0,     0,     0,     0,     0,     0,    39,
      40,     0,     0,    41,     0,     0,     0,     0,    42,     0,
       0,    43,     0,    44,    45,     0,    46,     0,     0,     0,
       0,     0,     0,    47,     0,     0,    48,     0,     0,     0,
      49,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,    51,    52,    53,    54,     0,     0,     0,    55,    56,
       0,     0,    57,    58,    59,    60,    61,    62,    63,     0,
       0,    64,     0,     0,     0,    65,     0,    66,    67,    68,
       0,    69,     0,     0,    70,     0,     0,     0,    71,    72,
      73,     0,    74,    75,     0,    76,    77,     0,    78,     0,
      79,    80,    81,     0,     0,     0,    82,     0,     0,    83,
      84,    85,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,    92,    93,     0,     0,
       0,    94,    95,    96,     0,     0,     0,     0,     0,     0,
       0,    97,     0,    98,    99,   100,   101,     0,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,   110,   111,
     112,   113,   114,     0,     0,     0,   115,   116,   117,   118,
     119,   120,     0,     0,     0,     0,   121,     0,     0,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     0,
     132,   133,   134,   135,   136,   137,   138,     0,     0,     0,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,     0,   154,   155,    29,   156,
     157,   158,   159,   160,   161,   162,     0,  1136,     0,     0,
       0,     0,     0,    30,     0,    31,    32,     0,     0,    33,
       0,     0,     0,     0,     0,    34,    35,     0,    36,     0,
       0,    37,    38,     0,     0,     0,     0,     0,     0,     0,
      39,    40,     0,     0,    41,     0,     0,     0,     0,    42,
       0,     0,    43,     0,    44,    45,     0,    46,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,     0,     0,
       0,    49,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,    58,    59,    60,    61,    62,    63,
       0,     0,    64,     0,     0,     0,    65,     0,    66,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
      72,    73,     0,    74,    75,     0,    76,    77,     0,    78,
       0,    79,    80,    81,     0,     0,     0,    82,     0,     0,
      83,    84,    85,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,    92,    93,     0,
       0,     0,    94,    95,    96,     0,     0,     0,     0,     0,
       0,     0,    97,     0,    98,    99,   100,   101,     0,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,   110,
     111,   112,   113,   114,     0,     0,     0,   115,   116,   117,
     118,   119,   120,     0,     0,     0,     0,   121,     0,     0,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,   132,   133,   134,   135,   136,   137,   138,     0,     0,
       0,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    29,   154,   155,     0,
     156,   157,   158,   159,   160,   161,   162,     0,     0,     0,
       0,    30,     0,    31,    32,     0,     0,    33,     0,     0,
       0,     0,     0,    34,    35,     0,    36,     0,     0,    37,
      38,     0,     0,     0,     0,     0,     0,     0,    39,    40,
       0,     0,    41,     0,     0,     0,     0,    42,     0,     0,
      43,     0,    44,    45,     0,    46,     0,     0,     0,     0,
       0,     0,    47,     0,     0,    48,     0,     0,     0,    49,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,     0,     0,     0,    55,    56,     0,
       0,    57,    58,    59,    60,    61,    62,    63,     0,     0,
      64,     0,     0,     0,    65,     0,    66,    67,    68,     0,
      69,     0,     0,    70,     0,     0,     0,    71,    72,    73,
       0,    74,    75,     0,   179,    77,     0,    78,     0,    79,
      80,    81,     0,     0,     0,    82,     0,     0,    83,    84,
      85,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,    92,    93,     0,     0,     0,
      94,    95,    96,     0,     0,     0,     0,     0,     0,     0,
      97,     0,    98,    99,   100,   101,     0,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,   110,   111,   112,
     113,   114,     0,     0,     0,   115,   116,   117,   118,   119,
     120,     0,     0,     0,     0,   121,     0,     0,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,   132,
     133,   134,   135,   136,   137,   138,     0,     0,     0,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,    29,   154,   155,     0,   156,   157,
     158,   159,   160,   161,   162,     0,     0,     0,     0,    30,
       0,    31,    32,     0,     0,    33,     0,     0,     0,     0,
       0,    34,    35,     0,    36,     0,     0,    37,    38,     0,
       0,     0,     0,     0,     0,     0,    39,    40,     0,     0,
      41,     0,     0,     0,     0,    42,     0,     0,    43,     0,
      44,    45,     0,    46,     0,     0,     0,     0,     0,     0,
      47,     0,     0,    48,     0,     0,     0,    49,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,     0,     0,     0,    55,    56,     0,     0,    57,
      58,    59,    60,    61,    62,    63,     0,     0,    64,     0,
       0,     0,    65,     0,    66,    67,    68,     0,    69,     0,
       0,    70,     0,     0,     0,    71,    72,    73,     0,    74,
      75,     0,    76,    77,     0,    78,     0,    79,    80,    81,
       0,     0,     0,    82,     0,     0,    83,    84,    85,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,    92,    93,     0,     0,     0,    94,    95,
      96,     0,     0,     0,     0,     0,     0,     0,    97,     0,
      98,    99,   100,   101,     0,   102,   103,   104,   105,   106,
     107,   108,   109,     0,     0,   110,   111,   112,   113,   114,
       0,     0,     0,   115,   116,   117,   118,   119,   120,     0,
       0,     0,     0,   121,     0,     0,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,     0,   132,   133,   134,
     135,   136,   137,   138,     0,     0,     0,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    29,   154,   155,     0,   156,   157,   158,   159,
     160,   161,   162,     0,     0,     0,     0,   479,     0,    31,
      32,     0,     0,    33,     0,     0,     0,     0,     0,    34,
      35,     0,    36,     0,     0,    37,    38,     0,     0,     0,
       0,     0,     0,     0,    39,    40,     0,     0,    41,     0,
       0,     0,     0,    42,     0,     0,    43,     0,    44,    45,
       0,    46,     0,     0,     0,     0,     0,     0,    47,     0,
       0,    48,     0,     0,     0,    49,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
       0,     0,     0,    55,    56,     0,     0,    57,    58,    59,
      60,    61,    62,    63,     0,     0,    64,     0,     0,     0,
      65,     0,    66,    67,    68,     0,    69,     0,     0,    70,
       0,     0,     0,    71,    72,    73,     0,    74,    75,     0,
      76,    77,     0,    78,     0,    79,    80,    81,     0,     0,
       0,    82,     0,     0,    83,    84,    85,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,    92,    93,     0,     0,     0,    94,    95,    96,     0,
       0,     0,     0,     0,     0,     0,    97,     0,    98,    99,
     100,   101,     0,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,   110,   111,   112,   113,   114,     0,     0,
       0,   115,   116,   117,   118,   119,   120,     0,     0,     0,
       0,   121,     0,     0,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,   132,   133,   134,   135,   136,
     137,   138,     0,     0,     0,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     205,   154,   155,     0,   156,   157,   158,   159,   160,   161,
     162,     0,     0,     0,     0,    30,     0,    31,    32,     0,
       0,    33,     0,     0,     0,     0,     0,    34,    35,     0,
      36,     0,     0,    37,    38,     0,     0,     0,     0,     0,
       0,     0,    39,    40,     0,     0,    41,     0,     0,     0,
       0,    42,     0,     0,    43,     0,    44,    45,     0,    46,
       0,     0,     0,     0,     0,     0,    47,     0,     0,    48,
       0,     0,     0,    49,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,     0,     0,
       0,    55,    56,     0,     0,    57,    58,    59,    60,    61,
      62,    63,     0,     0,    64,     0,     0,     0,    65,     0,
      66,    67,    68,     0,    69,     0,     0,    70,     0,     0,
       0,    71,    72,    73,     0,    74,    75,     0,    76,    77,
       0,    78,     0,    79,    80,    81,     0,     0,     0,    82,
       0,     0,    83,    84,    85,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,    92,
      93,     0,     0,     0,    94,    95,    96,     0,     0,     0,
       0,     0,     0,     0,    97,     0,    98,    99,   100,   101,
       0,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,   110,   111,   112,   113,   114,     0,     0,     0,   115,
     116,   117,   118,   119,   120,     0,     0,     0,     0,   121,
       0,     0,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     0,   132,   133,   134,   135,   136,   137,   138,
       0,     0,     0,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,  1064,   154,
     155,     0,   156,   157,   158,   159,   160,   161,   162,     0,
       0,     0,     0,    30,     0,    31,     0,     0,     0,    33,
       0,     0,     0,     0,     0,    34,     0,     0,    36,     0,
       0,    37,    38,     0,     0,     0,     0,     0,     0,     0,
      39,    40,     0,     0,     0,     0,     0,     0,     0,    42,
       0,     0,    43,     0,     0,    45,     0,    46,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,     0,     0,
       0,    49,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,     0,     0,    60,    61,    62,    63,
       0,     0,    64,     0,     0,     0,    65,     0,    66,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,     0,
      72,    73,     0,     0,    75,     0,    76,    77,     0,    78,
       0,     0,    80,    81,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,     0,     0,     0,     0,     0,
       0,     0,    97,     0,    98,    99,   100,     0,     0,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,   110,
     111,   112,   113,   114,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   121,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,     0,   154,   155,     0,
     156,   157,   158,   159,   160,   161,   162
};

static const yytype_int16 yycheck[] =
{
       2,   172,   516,   342,     6,     7,     8,     9,   428,   429,
       1,   363,   542,    13,   365,   266,   188,     2,   190,   191,
     472,     6,   172,     8,     9,   197,   568,   569,   570,   276,
     183,   184,   194,    74,    13,    35,    13,    13,    38,    13,
      13,   172,    13,    12,    16,    13,    13,    35,   278,    12,
      38,   390,   223,    13,    13,    13,    35,    13,   409,    38,
      13,    74,    35,    35,    19,    38,    38,   418,   419,   420,
      12,   224,    13,   223,   425,   426,    35,    35,    13,    38,
      38,    20,    35,    14,    13,    38,    13,    13,    13,   119,
      74,    13,   223,    13,   345,    74,    35,   259,    16,    38,
     351,    35,    13,    13,    38,   267,    19,    13,    35,    35,
      35,    38,    38,    38,    13,    13,    13,    35,   469,   471,
      38,    60,    35,    73,    13,    38,    13,    13,    13,    13,
     224,    13,    13,    13,    35,   295,   296,    38,    39,   386,
     118,     6,    12,    91,     9,   276,     8,    22,    68,   400,
     172,    12,    35,    73,   685,    38,    35,    35,   963,    38,
      38,   163,    35,    29,    35,    38,    86,    38,   119,   157,
     172,   263,   174,   123,    68,    63,   178,   179,    19,    73,
      15,    22,   514,    66,   346,   246,   421,   248,   118,   174,
     127,    20,   427,   343,   118,    66,    72,   139,   135,   531,
       0,   448,   204,   166,    13,   556,    81,   437,    65,   211,
     194,   166,   214,   215,   216,   206,    14,   219,   371,   149,
     118,   223,    97,   264,   166,   149,    61,    93,   197,    74,
     232,    60,   119,    74,   264,   155,   238,    86,   240,   241,
     119,     6,   244,   212,     9,   110,   166,   264,   110,   125,
     344,   264,   183,   244,    12,   386,    72,   677,  1063,   211,
      12,   155,   214,   215,   216,   240,   617,   219,   243,   139,
     166,   433,    12,   273,   141,   266,   164,     2,   139,   810,
      12,     6,     7,     8,     9,   273,   238,   264,   264,   267,
     264,    12,   156,   264,   273,   467,   264,   264,   549,   813,
     273,   273,    12,   817,   264,   166,   466,   658,   264,   125,
     371,   165,   664,   264,   273,   273,   266,   448,   268,    12,
     273,   660,    14,   264,   776,    12,   678,    88,    89,   264,
     264,   251,    12,   208,   273,   264,    19,   339,   677,   273,
     342,   343,   264,    39,   506,    41,   273,   273,   273,   520,
     352,   353,   343,   264,   264,   273,   909,   251,   264,   361,
     273,   363,   915,   916,     5,   264,   516,   264,   707,    10,
     520,     5,   273,   158,   376,   264,    10,   624,   264,   264,
     264,   372,   264,   264,   264,   516,   264,   139,   390,   520,
     273,   255,   153,     5,   273,   273,   260,   261,    10,   139,
     273,   353,   273,   115,   918,    12,   158,   364,    50,   400,
     250,   175,   176,   177,   166,   255,   256,   256,   158,   421,
     671,    28,    12,   775,   376,   427,   166,   265,    12,   768,
     387,   388,    12,    54,    14,    77,   274,   275,   163,   441,
      88,    89,    90,   195,    88,    89,    90,   172,   992,   174,
     994,    58,    12,   178,   179,   195,   441,    19,  1000,    80,
       4,     5,     6,     7,     8,   737,    10,   109,   118,   471,
     472,   743,   744,   475,    12,   477,    83,    12,    22,   204,
     101,    12,   103,    88,    89,    90,   211,   270,   271,   214,
     215,   216,    12,   624,   219,  1025,   196,   197,   223,   771,
     165,   166,   841,   156,   165,   166,    50,   232,   150,    88,
      89,    90,    12,   238,   516,   240,   241,   138,   520,   244,
      12,    48,   173,   174,   515,   527,    12,   680,   681,   237,
     238,   239,   139,    88,    89,   685,    12,   699,    12,   891,
       4,     5,     6,     7,     8,    12,    10,   168,    75,   156,
     902,    48,    88,    89,   685,    82,   209,   210,    12,   166,
      12,   518,    88,    89,    12,   522,    93,   200,   201,    96,
      12,   178,   179,     4,     5,     6,     7,     8,    75,    10,
     852,   853,   183,   184,    12,    82,    50,   818,   819,   116,
     930,   931,    12,    12,    12,    12,    93,    12,    92,    96,
      16,   269,   255,    12,    12,    12,   133,   260,   261,   961,
     154,    12,    14,    12,   339,   777,   778,   342,   343,   116,
      12,    48,    12,    14,    13,   166,   617,   352,   353,     5,
       4,   262,   164,     5,     5,    39,   133,    12,   363,    39,
     115,   273,     4,   645,     4,  1159,     4,    14,    75,    12,
      48,   376,   823,   655,   656,    82,   995,    13,   660,   661,
     810,   264,   664,   813,  1016,   390,    93,   817,   670,    96,
     921,    14,    13,   823,   676,   677,   678,    75,    13,   810,
      14,  1101,   813,   685,    82,    14,   817,    19,   266,   116,
     154,    14,   823,   211,   685,    93,   264,   422,    96,   198,
     198,   180,   180,  1054,  1124,   707,   133,   176,   264,     5,
      13,   254,    39,    13,    13,    13,   441,    74,    13,    13,
      12,    12,    12,   725,    12,    12,    12,    12,     8,    18,
     115,    81,    14,   984,   122,   133,   264,    13,    13,     4,
     725,    14,    39,     4,   268,   115,   471,   115,   920,    12,
     475,    19,   477,  1173,   264,    77,   264,   907,     5,   181,
     181,     4,   253,    64,    13,    27,   768,   769,   918,    27,
     233,   773,    13,   775,   776,    13,   907,    13,  1117,     5,
     782,     5,     5,  1034,     5,  1136,  1037,   918,     5,     5,
       5,   516,   794,     3,    14,   520,   192,     4,    16,   153,
     962,    14,   527,    14,     4,   157,     5,   809,   810,  1160,
      12,   813,     4,    14,    12,   817,    22,    13,    27,   810,
      79,   823,    93,    13,    13,  1164,    77,    13,   121,    35,
      13,    22,    38,    39,    40,    41,    42,    43,    44,   841,
      13,    13,    13,    13,    35,   166,    13,    38,    39,    40,
      41,    42,    43,    44,    14,    14,    12,    19,     4,     4,
      13,   267,     5,     5,    13,    12,    93,    89,    12,   153,
     872,   119,   153,    38,     5,    81,   878,   879,   264,     4,
     264,    13,     3,   123,    14,    14,    92,    14,   890,   891,
      81,    97,    14,   153,    13,    13,   153,   153,   164,    13,
     902,    92,    14,   170,   110,   907,    97,   119,    12,    93,
      27,    93,    89,    89,   142,    63,   918,   264,   236,   110,
     645,   199,    13,    64,    93,    93,    89,    93,     5,    93,
     655,   656,    12,    12,   213,   660,   661,   171,   125,   664,
     134,  1192,   246,   149,   125,   670,    69,   166,    74,    16,
     252,     4,   677,   678,   134,   957,   239,    13,     2,   961,
     685,   963,     6,     7,     8,     9,   120,   241,     4,   166,
     247,   521,   244,   242,   153,   434,   978,   172,   248,   187,
      25,  1087,   707,  1164,   371,  1136,   250,  1161,   996,  1134,
     245,   179,   517,   995,   240,   249,   670,   794,   907,  1036,
     725,   660,   208,  1111,  1073,   258,  1008,  1009,   461,  1159,
     436,   456,   610,  1015,   465,   624,   550,   208,  1049,   746,
     922,   872,  1101,   931,  1173,   995,   395,   676,  1159,   254,
    1090,   713,   722,  1092,  1121,   776,    -1,   198,    -1,   232,
      -1,    -1,   217,   768,   769,    -1,    -1,  1038,   773,    -1,
     775,    -1,    -1,    -1,    -1,    -1,    -1,   782,    -1,  1061,
     217,  1063,    -1,    -1,    -1,    -1,   272,   273,    -1,   794,
      -1,  1073,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   272,   273,    -1,   809,   810,    -1,    -1,   813,    -1,
      -1,    -1,   817,    -1,    -1,    -1,    -1,    -1,   823,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1117,   841,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
     174,    -1,    -1,    -1,   178,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   872,    -1,    -1,
      -1,    -1,    -1,   878,   879,    -1,    -1,  1159,    -1,    -1,
     204,    -1,  1164,    -1,    -1,   890,   891,   211,    -1,  1160,
     214,   215,   216,    -1,    -1,   219,    -1,   902,    -1,   223,
      -1,    -1,   907,    -1,    -1,    -1,    -1,    -1,   232,    -1,
      -1,    -1,    -1,   918,   238,    -1,   240,   241,    -1,    -1,
     244,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,   957,    -1,    -1,     2,   961,    -1,   963,     6,
       7,     8,     9,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    -1,    -1,   978,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
     995,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,  1008,  1009,    -1,    -1,    -1,    -1,    -1,
    1015,  1016,    -1,    -1,    -1,   339,    -1,    -1,   342,   343,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   352,   353,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   363,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   376,    -1,    -1,   143,  1061,    -1,  1063,    -1,
      -1,    -1,    -1,   151,   152,    -1,   390,    -1,  1073,    -1,
      -1,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1117,    -1,    -1,    -1,   163,   441,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,   174,    -1,    -1,
      -1,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   471,    -1,    -1,
      -1,   475,    -1,   477,  1159,    -1,    -1,   204,    -1,  1164,
      -1,    -1,    -1,    -1,   211,    -1,    -1,   214,   215,   216,
      -1,    -1,   219,    -1,    -1,    -1,   223,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   232,    -1,    -1,    -1,    -1,
      -1,   238,   516,   240,   241,    -1,   520,   244,    -1,    -1,
      -1,    -1,    -1,   527,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   339,    -1,    -1,   342,   343,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   352,   353,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   363,    -1,    -1,    -1,
      -1,   645,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   376,
      -1,   655,   656,    -1,    -1,    -1,   660,   661,    -1,    -1,
     664,    -1,    -1,   390,    -1,    -1,   670,    -1,    -1,    -1,
      -1,    -1,    -1,   677,   678,    -1,    -1,    -1,    -1,    -1,
      -1,   685,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   707,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   441,    -1,    -1,    -1,    -1,    -1,
      -1,   725,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   471,    -1,    -1,    -1,   475,    -1,
     477,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   768,   769,    -1,    -1,    -1,   773,
      -1,   775,    -1,    -1,    -1,    -1,    -1,    -1,   782,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   516,
     794,    -1,    -1,   520,    -1,    -1,    -1,    -1,    -1,    -1,
     527,    -1,    -1,    -1,    -1,   809,   810,    -1,    -1,   813,
      -1,    -1,    -1,   817,    -1,    -1,    -1,    -1,    -1,   823,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   841,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   872,    -1,
      -1,    -1,    -1,    -1,   878,   879,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   890,   891,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   902,    -1,
      -1,    -1,    -1,   907,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   918,    -1,    -1,    -1,   645,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   655,   656,
      -1,    -1,    -1,   660,   661,    -1,    -1,   664,    -1,    -1,
      -1,    -1,    -1,   670,    -1,    -1,    -1,    -1,    -1,    -1,
     677,   678,    -1,   957,    -1,    -1,     2,   961,   685,   963,
       6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   978,    -1,    -1,    -1,    -1,    -1,
     707,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   995,    -1,    -1,    -1,    -1,    -1,    -1,   725,    -1,
      -1,    -1,    -1,    -1,  1008,  1009,    -1,    -1,    -1,    -1,
      -1,  1015,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   768,   769,    -1,    -1,    -1,   773,    -1,   775,    -1,
      -1,    -1,    -1,    -1,    -1,   782,    -1,  1061,    -1,  1063,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   794,    -1,  1073,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   809,   810,    -1,    -1,   813,    -1,    -1,    -1,
     817,    -1,    -1,    -1,    -1,    -1,   823,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1117,   841,    -1,    -1,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,    -1,
      -1,    -1,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   872,    -1,    -1,    -1,    -1,
      -1,   878,   879,    -1,    -1,  1159,    -1,    -1,   204,    -1,
    1164,    -1,    -1,   890,   891,   211,    -1,    -1,   214,   215,
     216,    -1,    -1,   219,    -1,   902,    -1,   223,    -1,    -1,
     907,    -1,    -1,    -1,    -1,    -1,   232,    -1,    -1,    -1,
      -1,   918,   238,    -1,   240,   241,    -1,    -1,   244,    -1,
      -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,     6,
       7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     957,    -1,    -1,    -1,   961,    -1,   963,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   978,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   995,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1008,  1009,    -1,    -1,    -1,    -1,    -1,  1015,    -1,
      -1,    -1,    -1,   339,    -1,    -1,   342,   343,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   352,   353,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   363,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     376,    -1,    -1,    -1,  1061,    -1,  1063,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   390,    -1,  1073,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1117,    -1,    -1,    -1,    -1,   441,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,    -1,    -1,
      -1,    -1,    -1,    -1,   211,    -1,    -1,   214,   215,   216,
      -1,    -1,   219,    -1,    -1,   471,    -1,    -1,    -1,   475,
      -1,   477,  1159,    -1,    -1,   232,    -1,  1164,    -1,    -1,
      -1,   238,    -1,   240,   241,    -1,    -1,   244,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     516,    -1,    -1,    -1,   520,    -1,    -1,    -1,    -1,    -1,
      -1,   527,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   339,    -1,    -1,   342,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   352,   353,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   376,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   390,    -1,    -1,    -1,    -1,    -1,   645,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   655,
     656,    -1,    -1,    -1,   660,   661,    -1,    -1,   664,    -1,
      -1,    -1,    -1,    -1,   670,    -1,    -1,    -1,    -1,    -1,
      -1,   677,   678,    -1,    -1,    -1,    -1,    -1,    -1,   685,
      -1,    -1,    -1,    -1,   441,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   707,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   475,   725,
     477,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   768,   769,    -1,    -1,    -1,   773,    -1,   775,
     527,    -1,   201,    -1,    -1,    -1,   782,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   794,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     229,   230,    -1,   809,   810,    -1,    -1,   813,    -1,    -1,
      -1,   817,    -1,    -1,    -1,    -1,    -1,   823,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   841,    -1,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   872,    -1,    -1,    -1,
      -1,    -1,   878,   879,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   890,   891,    -1,    -1,   645,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   902,    -1,   655,   656,
      -1,   907,    -1,   660,   661,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   918,   670,    -1,    -1,   345,    -1,    -1,    -1,
     677,    -1,   351,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     707,   957,    -1,    -1,    -1,   961,    -1,   963,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   725,    -1,
      -1,   400,   978,    -1,    -1,    -1,    -1,    -1,   407,    -1,
      -1,   410,   411,    -1,    -1,   414,    -1,    -1,   417,   995,
      -1,    -1,   421,    -1,   423,   424,    -1,    -1,   427,    -1,
      -1,   430,  1008,  1009,    -1,   434,    -1,   436,   437,  1015,
     439,   768,   769,    -1,    -1,    -1,   773,    -1,    -1,    -1,
      -1,   450,    -1,    -1,    -1,   782,   455,    -1,   457,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   794,    -1,    -1,
      -1,   470,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   809,    -1,    -1,  1061,    -1,  1063,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1073,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   841,    -1,    -1,    -1,   517,    -1,
      -1,    -1,   521,    -1,   523,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1117,    -1,    -1,    -1,   872,    -1,    -1,    -1,   548,
     549,   878,   879,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   890,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1159,    -1,    -1,    -1,    -1,  1164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   623,    -1,   625,    -1,   627,    -1,
     957,    -1,    -1,    -1,    -1,    -1,   963,    -1,    -1,    -1,
      -1,   640,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   978,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   995,    -1,
      -1,    -1,   671,    -1,    -1,    -1,    -1,   676,    -1,    -1,
      -1,  1008,  1009,    -1,    -1,    -1,    -1,    -1,  1015,  1016,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     719,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   730,   731,    -1,  1061,    -1,  1063,    -1,    -1,   738,
      -1,    -1,    -1,    -1,    -1,    -1,  1073,    -1,    -1,   748,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   758,
      -1,    -1,   761,    -1,    -1,    -1,   765,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     779,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   832,   833,    -1,    -1,  1164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   845,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   865,   866,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   921,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,   984,    -1,    -1,    -1,    -1,
     989,    -1,    -1,    -1,    -1,    -1,    -1,   996,    18,    19,
      20,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    -1,    33,    -1,    35,    36,    37,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    54,  1034,    -1,    57,  1037,    59,
      60,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    -1,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,   107,  1087,    -1,
      -1,   111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,
     120,    -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,
      -1,   131,   132,    -1,   134,    -1,   136,   137,   138,    -1,
      -1,    -1,   142,    -1,    -1,   145,   146,   147,    -1,    -1,
     150,   151,   152,   153,   154,  1134,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,    -1,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,
     180,   181,   182,    -1,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,   194,   195,   196,   197,   198,    -1,
      -1,    -1,   202,   203,   204,   205,   206,   207,    -1,    -1,
      -1,    -1,   212,  1192,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,    -1,   226,   227,   228,   229,
     230,   231,   232,    -1,    -1,    -1,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,    -1,   252,   253,    -1,   255,   256,   257,   258,   259,
     260,   261,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    -1,   273,    -1,    -1,    -1,    18,    -1,    20,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    47,    -1,    49,    -1,
      51,    52,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    95,    96,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,   110,
     111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,
     131,   132,   133,   134,    -1,   136,   137,   138,   139,   140,
      -1,   142,    -1,    -1,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,    -1,    -1,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,
     181,   182,    -1,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,
      -1,   202,   203,   204,   205,   206,   207,    -1,    -1,    -1,
      -1,   212,    -1,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,    -1,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
      -1,   252,   253,    -1,   255,   256,   257,   258,   259,   260,
     261,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   270,
     271,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    13,    -1,    -1,    -1,    -1,    18,    -1,    20,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
      -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    -1,    49,    -1,    51,
      52,    -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    94,    95,    96,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,   107,    -1,    -1,   110,   111,
      -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,    -1,
     142,    -1,    -1,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,
     182,    -1,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,
     202,   203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,
     212,    -1,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,    -1,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,    -1,
     252,   253,    -1,   255,   256,   257,   258,   259,   260,   261,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   270,   271,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    12,
      -1,    -1,    15,    -1,    17,    18,    -1,    20,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,
      33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    47,    -1,    49,    -1,    51,    52,
      -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      -1,    94,    95,    96,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,   124,   125,   126,    -1,   128,   129,    -1,   131,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,    -1,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,
      -1,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,   202,
     203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,   212,
      -1,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
      -1,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,    -1,   252,
     253,    -1,   255,   256,   257,   258,   259,   260,   261,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   270,   271,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    20,    21,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,    33,
      -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    46,    47,    -1,    49,    -1,    51,    52,    -1,
      54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,
      94,    95,    96,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,
     114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,
     124,   125,   126,    -1,   128,   129,    -1,   131,   132,   133,
     134,    -1,   136,   137,   138,    -1,   140,    -1,   142,    -1,
      -1,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
      -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,    -1,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
     194,   195,   196,   197,   198,    -1,    -1,    -1,   202,   203,
     204,   205,   206,   207,    -1,    -1,    -1,    -1,   212,    -1,
      -1,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,    -1,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,    -1,   252,   253,
      -1,   255,   256,   257,   258,   259,   260,   261,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   270,   271,     3,     4,
       5,     6,     7,     8,    -1,    10,    -1,    12,    13,    -1,
      -1,    -1,    -1,    18,    -1,    20,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    29,    30,    31,    -1,    33,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    46,    47,    -1,    49,    -1,    51,    52,    -1,    54,
      -1,    -1,    57,    -1,    59,    60,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,
      95,    96,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,   124,
     125,   126,    -1,   128,   129,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,   139,   140,    -1,   142,    -1,    -1,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,    -1,
      -1,   166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,   179,   180,   181,   182,    -1,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,   194,
     195,   196,   197,   198,    -1,    -1,    -1,   202,   203,   204,
     205,   206,   207,    -1,    -1,    -1,    -1,   212,    -1,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,    -1,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,    -1,   252,   253,    -1,
     255,   256,   257,   258,   259,   260,   261,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   270,   271,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    20,    21,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,    -1,    33,    -1,    -1,
      36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,
      46,    47,    -1,    49,    -1,    51,    52,    -1,    54,    -1,
      -1,    57,    -1,    59,    60,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,    95,
      96,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,   107,    -1,    -1,   110,   111,    -1,   113,   114,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,    -1,   124,   125,
     126,    -1,   128,   129,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,    -1,   142,    -1,    -1,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,    -1,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,   179,   180,   181,   182,    -1,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,   194,   195,
     196,   197,   198,    -1,    -1,    -1,   202,   203,   204,   205,
     206,   207,    -1,    -1,    -1,    -1,   212,    -1,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,    -1,    -1,    -1,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,    -1,   252,   253,    -1,   255,
     256,   257,   258,   259,   260,   261,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   270,   271,     3,     4,     5,     6,
       7,     8,    -1,    10,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    20,    21,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,    -1,    33,    -1,    -1,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      47,    -1,    49,    -1,    51,    52,    -1,    54,    -1,    -1,
      57,    -1,    59,    60,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    -1,    94,    95,    96,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,   114,   115,    -1,
     117,    -1,    -1,   120,    -1,    -1,    -1,   124,   125,   126,
      -1,   128,   129,    -1,   131,   132,   133,   134,    -1,   136,
     137,   138,   139,   140,    -1,   142,    -1,    -1,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,    -1,    -1,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,   179,   180,   181,   182,    -1,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,   194,   195,   196,
     197,   198,    -1,    -1,    -1,   202,   203,   204,   205,   206,
     207,    -1,    -1,    -1,    -1,   212,    -1,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,    -1,    -1,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,    -1,   252,   253,    -1,   255,   256,
     257,   258,   259,   260,   261,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   270,   271,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    20,    21,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,    -1,    33,    -1,    -1,    36,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      -1,    49,    -1,    51,    52,    -1,    54,    -1,    -1,    57,
      -1,    59,    60,    61,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    -1,    94,    95,    96,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,    -1,   124,   125,   126,    -1,
     128,   129,    -1,   131,   132,   133,   134,    -1,   136,   137,
     138,    -1,   140,    -1,   142,    -1,    -1,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,    -1,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,   179,   180,   181,   182,    -1,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,   194,   195,   196,   197,
     198,    -1,    -1,    -1,   202,   203,   204,   205,   206,   207,
      -1,    -1,    -1,    -1,   212,    -1,    -1,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,    -1,    -1,    -1,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,    -1,   252,   253,    -1,   255,   256,   257,
     258,   259,   260,   261,    -1,    -1,    -1,   265,    -1,    -1,
      -1,    -1,   270,   271,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    12,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    20,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,    -1,    33,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,    -1,
      49,    -1,    51,    52,    -1,    54,    -1,    -1,    57,    -1,
      59,    60,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    -1,    94,    95,    96,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,   124,   125,   126,    -1,   128,
     129,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,    -1,   142,    -1,    -1,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
     179,   180,   181,   182,    -1,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,   194,   195,   196,   197,   198,
      -1,    -1,    -1,   202,   203,   204,   205,   206,   207,    -1,
      -1,    -1,    -1,   212,    -1,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,    -1,    -1,    -1,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,    -1,   252,   253,    -1,   255,   256,   257,   258,
     259,   260,   261,    -1,    -1,    -1,   265,    -1,    -1,    -1,
      -1,   270,   271,     3,     4,     5,     6,     7,     8,    -1,
      10,    -1,    12,    13,    -1,    -1,    -1,    -1,    18,    -1,
      20,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    29,
      30,    31,    -1,    33,    -1,    -1,    36,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    46,    47,    -1,    49,
      -1,    51,    52,    -1,    54,    -1,    -1,    57,    -1,    59,
      60,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    -1,    94,    95,    96,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,
     120,    -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,    -1,   142,    -1,    -1,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,    -1,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,
     180,   181,   182,    -1,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,   194,   195,   196,   197,   198,    -1,
      -1,    -1,   202,   203,   204,   205,   206,   207,    -1,    -1,
      -1,    -1,   212,    -1,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,    -1,    -1,    -1,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,    -1,   252,   253,    -1,   255,   256,   257,   258,   259,
     260,   261,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     270,   271,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    13,    -1,    -1,    -1,    -1,    18,    -1,    20,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    47,    -1,    49,    -1,
      51,    52,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    95,    96,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,
     131,   132,   133,   134,    -1,   136,   137,   138,    -1,   140,
      -1,   142,    -1,    -1,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,
     181,   182,    -1,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,
      -1,   202,   203,   204,   205,   206,   207,    -1,    -1,    -1,
      -1,   212,    -1,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,    -1,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
      -1,   252,   253,    -1,   255,   256,   257,   258,   259,   260,
     261,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   270,
     271,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    13,    -1,    -1,    -1,    -1,    18,    -1,    20,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
      -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    -1,    49,    -1,    51,
      52,    -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    94,    95,    96,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,    -1,
     142,    -1,    -1,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,
     182,    -1,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,
     202,   203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,
     212,    -1,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,    -1,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,    -1,
     252,   253,    -1,   255,   256,   257,   258,   259,   260,   261,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   270,   271,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    20,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,
      33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    47,    -1,    49,    -1,    51,    52,
      -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      -1,    94,    95,    96,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,   124,   125,   126,    -1,   128,   129,    -1,   131,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,    -1,   142,
      -1,    -1,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,
      -1,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,   202,
     203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,   212,
      -1,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
      -1,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,    -1,   252,
     253,    -1,   255,   256,   257,   258,   259,   260,   261,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   270,   271,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    20,    21,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,    33,
      -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    46,    47,    -1,    49,    -1,    51,    52,    -1,
      54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,
      94,    95,    96,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,
     114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,
     124,   125,   126,    -1,   128,   129,    -1,   131,   132,   133,
     134,    -1,   136,   137,   138,    -1,   140,    -1,   142,    -1,
      -1,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
      -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,    -1,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
     194,   195,   196,   197,   198,    -1,    -1,    -1,   202,   203,
     204,   205,   206,   207,    -1,    -1,    -1,    -1,   212,    -1,
      -1,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,    -1,
      -1,    -1,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,    -1,   252,   253,
      -1,   255,   256,   257,   258,   259,   260,   261,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,   270,   271,    10,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    18,    -1,    20,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,   131,
     132,    -1,   134,    -1,   136,   137,   138,   139,    -1,    -1,
     142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,    -1,    -1,   166,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,
     182,    -1,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
     202,   203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,
     212,    -1,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,    -1,   226,   227,   228,   229,   230,   231,
     232,    -1,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,    -1,
     252,   253,    -1,   255,   256,   257,   258,   259,   260,   261,
       3,    -1,    -1,    -1,    -1,    -1,   268,    10,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    20,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    -1,
      33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      -1,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,   124,   125,   126,    -1,   128,   129,    -1,   131,   132,
      -1,   134,    -1,   136,   137,   138,    -1,    -1,    -1,   142,
      -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,
      -1,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
     193,   194,   195,   196,   197,   198,    -1,    -1,    -1,   202,
     203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,   212,
      -1,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,    -1,   226,   227,   228,   229,   230,   231,   232,
      -1,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,    -1,   252,
     253,     3,   255,   256,   257,   258,   259,   260,   261,    -1,
      12,    -1,    -1,    -1,    -1,   268,    18,    -1,    20,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,   131,
     132,    -1,   134,    -1,   136,   137,   138,    -1,    -1,    -1,
     142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,
     182,    -1,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
     202,   203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,
     212,    -1,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,    -1,   226,   227,   228,   229,   230,   231,
     232,    -1,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,    -1,
     252,   253,     3,   255,   256,   257,   258,   259,   260,   261,
      -1,    12,    -1,    -1,    -1,    -1,   268,    18,    -1,    20,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,
     131,   132,    -1,   134,    -1,   136,   137,   138,    -1,    -1,
      -1,   142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,
     181,   182,    -1,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,   193,   194,   195,   196,   197,   198,    -1,    -1,
      -1,   202,   203,   204,   205,   206,   207,    -1,    -1,    -1,
      -1,   212,    -1,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,    -1,   226,   227,   228,   229,   230,
     231,   232,    -1,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
      -1,   252,   253,     3,   255,   256,   257,   258,   259,   260,
     261,    -1,    -1,    -1,    -1,    -1,    -1,   268,    18,    -1,
      20,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    -1,    33,    -1,    -1,    36,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    54,    -1,    -1,    57,    -1,    59,
      60,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    -1,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,
     120,    -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,
      -1,   131,   132,    -1,   134,    -1,   136,   137,   138,    -1,
      -1,    -1,   142,    -1,    -1,   145,   146,   147,    -1,    -1,
     150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,    -1,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,
     180,   181,   182,    -1,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,   194,   195,   196,   197,   198,    -1,
      -1,    -1,   202,   203,   204,   205,   206,   207,    -1,    -1,
      -1,    -1,   212,    -1,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,    -1,   226,   227,   228,   229,
     230,   231,   232,    -1,    -1,    -1,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,    -1,   252,   253,     3,   255,   256,   257,   258,   259,
     260,   261,    -1,    -1,    -1,    14,    -1,    -1,   268,    18,
      -1,    20,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    -1,    33,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    54,    -1,    -1,    57,    -1,
      59,    60,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    -1,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,   124,   125,   126,    -1,   128,
     129,    -1,   131,   132,    -1,   134,    -1,   136,   137,   138,
      -1,    -1,    -1,   142,    -1,    -1,   145,   146,   147,    -1,
      -1,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
     179,   180,   181,   182,    -1,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,   194,   195,   196,   197,   198,
      -1,    -1,    -1,   202,   203,   204,   205,   206,   207,    -1,
      -1,    -1,    -1,   212,    -1,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,    -1,   226,   227,   228,
     229,   230,   231,   232,    -1,    -1,    -1,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,     3,   252,   253,    -1,   255,   256,   257,   258,
     259,   260,   261,    14,    -1,    -1,   265,    18,    -1,    20,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,
     131,   132,    -1,   134,    -1,   136,   137,   138,    -1,    -1,
      -1,   142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,
     181,   182,    -1,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,
      -1,   202,   203,   204,   205,   206,   207,    -1,    -1,    -1,
      -1,   212,    -1,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,    -1,   226,   227,   228,   229,   230,
     231,   232,    -1,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
       3,   252,   253,    -1,   255,   256,   257,   258,   259,   260,
     261,    14,    -1,    -1,   265,    18,    -1,    20,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    -1,
      33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      -1,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,   124,   125,   126,    -1,   128,   129,    -1,   131,   132,
      -1,   134,    -1,   136,   137,   138,    -1,    -1,    -1,   142,
      -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,
      -1,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,   202,
     203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,   212,
      -1,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,    -1,   226,   227,   228,   229,   230,   231,   232,
      -1,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,     3,   252,
     253,    -1,   255,   256,   257,   258,   259,   260,   261,    14,
      -1,    -1,   265,    18,    -1,    20,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    33,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    57,    -1,    59,    60,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,   124,
     125,   126,    -1,   128,   129,    -1,   131,   132,    -1,   134,
      -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,
     145,   146,   147,    -1,    -1,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,   179,   180,   181,   182,    -1,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,   194,
     195,   196,   197,   198,    -1,    -1,    -1,   202,   203,   204,
     205,   206,   207,    -1,    -1,    -1,    -1,   212,    -1,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
      -1,   226,   227,   228,   229,   230,   231,   232,    -1,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,     3,   252,   253,    -1,
     255,   256,   257,   258,   259,   260,   261,    -1,    -1,    -1,
     265,    18,    -1,    20,    21,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    -1,    33,    -1,    -1,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,    -1,    59,    60,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    -1,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,   114,   115,    -1,
     117,    -1,    -1,   120,    -1,    -1,    -1,   124,   125,   126,
      -1,   128,   129,    -1,   131,   132,    -1,   134,    -1,   136,
     137,   138,    -1,    -1,    -1,   142,    -1,    -1,   145,   146,
     147,    -1,    -1,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,   179,   180,   181,   182,    -1,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,   194,   195,   196,
     197,   198,    -1,    -1,    -1,   202,   203,   204,   205,   206,
     207,    -1,    -1,    -1,    -1,   212,    -1,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,    -1,   226,
     227,   228,   229,   230,   231,   232,    -1,    -1,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,     3,   252,   253,    -1,   255,   256,
     257,   258,   259,   260,   261,    -1,    -1,    -1,   265,    18,
      -1,    20,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    -1,    33,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    54,    -1,    -1,    57,    -1,
      59,    60,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    -1,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,   124,   125,   126,    -1,   128,
     129,    -1,   131,   132,    -1,   134,    -1,   136,   137,   138,
      -1,    -1,    -1,   142,    -1,    -1,   145,   146,   147,    -1,
      -1,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
     179,   180,   181,   182,    -1,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,   194,   195,   196,   197,   198,
      -1,    -1,    -1,   202,   203,   204,   205,   206,   207,    -1,
      -1,    -1,    -1,   212,    -1,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,    -1,   226,   227,   228,
     229,   230,   231,   232,    -1,    -1,    -1,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,     3,   252,   253,    -1,   255,   256,   257,   258,
     259,   260,   261,    -1,    -1,    -1,   265,    18,    -1,    20,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,
     131,   132,    -1,   134,    -1,   136,   137,   138,    -1,    -1,
      -1,   142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,
     181,   182,    -1,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,
      -1,   202,   203,   204,   205,   206,   207,    -1,    -1,    -1,
      -1,   212,    -1,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,    -1,   226,   227,   228,   229,   230,
     231,   232,    -1,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
       3,   252,   253,    -1,   255,   256,   257,   258,   259,   260,
     261,    -1,    -1,    -1,   265,    18,    -1,    20,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    -1,
      33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      -1,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,   124,   125,   126,    -1,   128,   129,    -1,   131,   132,
      -1,   134,    -1,   136,   137,   138,    -1,    -1,    -1,   142,
      -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,
      -1,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,   202,
     203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,   212,
      -1,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,    -1,   226,   227,   228,   229,   230,   231,   232,
      -1,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,     3,   252,
     253,    -1,   255,   256,   257,   258,   259,   260,   261,    -1,
      -1,    -1,   265,    18,    -1,    20,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    33,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    57,    -1,    59,    60,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,   124,
     125,   126,    -1,   128,   129,    -1,   131,   132,    -1,   134,
      -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,
     145,   146,   147,    -1,    -1,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,   179,   180,   181,   182,    -1,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,   194,
     195,   196,   197,   198,    -1,    -1,    -1,   202,   203,   204,
     205,   206,   207,    -1,    -1,    -1,    -1,   212,    -1,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
      -1,   226,   227,   228,   229,   230,   231,   232,    -1,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,     3,   252,   253,    -1,
     255,   256,   257,   258,   259,   260,   261,    -1,    -1,    -1,
     265,    18,    -1,    20,    21,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    -1,    33,    -1,    -1,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,    -1,    59,    60,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    -1,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,   114,   115,    -1,
     117,    -1,    -1,   120,    -1,    -1,    -1,   124,   125,   126,
      -1,   128,   129,    -1,   131,   132,    -1,   134,    -1,   136,
     137,   138,    -1,    -1,    -1,   142,    -1,    -1,   145,   146,
     147,    -1,    -1,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,   179,   180,   181,   182,    -1,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,   194,   195,   196,
     197,   198,    -1,    -1,    -1,   202,   203,   204,   205,   206,
     207,    -1,    -1,    -1,    -1,   212,    -1,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,    -1,   226,
     227,   228,   229,   230,   231,   232,    -1,    -1,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,     3,   252,   253,    -1,   255,   256,
     257,   258,   259,   260,   261,    -1,    -1,    -1,   265,    18,
      -1,    20,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    -1,    33,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    54,    -1,    -1,    57,    -1,
      59,    60,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    -1,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,   124,   125,   126,    -1,   128,
     129,    -1,   131,   132,    -1,   134,    -1,   136,   137,   138,
      -1,    -1,    -1,   142,    -1,    -1,   145,   146,   147,    -1,
      -1,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
     179,   180,   181,   182,    -1,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,   194,   195,   196,   197,   198,
      -1,    -1,    -1,   202,   203,   204,   205,   206,   207,    -1,
      -1,    -1,    -1,   212,    -1,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,    -1,   226,   227,   228,
     229,   230,   231,   232,    -1,    -1,    -1,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,     3,   252,   253,    -1,   255,   256,   257,   258,
     259,   260,   261,    -1,    -1,    -1,   265,    18,    -1,    20,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    47,    -1,    49,    -1,
      51,    52,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    95,    96,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,
     131,   132,   133,   134,    -1,   136,   137,   138,    -1,   140,
      -1,   142,    -1,    -1,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,
     181,   182,    -1,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,
      -1,   202,   203,   204,   205,   206,   207,    -1,    -1,    -1,
      -1,   212,    -1,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,    -1,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
      -1,   252,   253,    -1,   255,   256,   257,   258,   259,   260,
     261,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    20,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,   131,
     132,    -1,   134,    -1,   136,   137,   138,    -1,    -1,    -1,
     142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,
     182,    -1,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,
     202,   203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,
     212,    -1,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,    -1,   226,   227,   228,   229,   230,   231,
     232,    -1,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,    -1,
     252,   253,    -1,   255,   256,   257,   258,   259,   260,   261,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    -1,
      33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      -1,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,    -1,   131,   132,
      -1,   134,    -1,   136,   137,   138,    -1,    -1,    -1,   142,
      -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,
      -1,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,   202,
     203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,   212,
      -1,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,    -1,   226,   227,   228,   229,   230,   231,   232,
      -1,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,    -1,   252,
     253,     3,   255,   256,   257,   258,   259,   260,   261,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    18,    -1,    20,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,   131,
     132,    -1,   134,    -1,   136,   137,   138,   139,    -1,    -1,
     142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,    -1,    -1,   166,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,
     182,    -1,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,
     202,   203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,
     212,    -1,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,    -1,   226,   227,   228,   229,   230,   231,
     232,    -1,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,    -1,
     252,   253,     3,   255,   256,   257,   258,   259,   260,   261,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    18,    -1,    20,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,
     131,   132,    -1,   134,    -1,   136,   137,   138,   139,    -1,
      -1,   142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,    -1,    -1,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,
     181,   182,    -1,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,
      -1,   202,   203,   204,   205,   206,   207,    -1,    -1,    -1,
      -1,   212,    -1,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,    -1,   226,   227,   228,   229,   230,
     231,   232,    -1,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
      -1,   252,   253,    -1,   255,   256,   257,   258,   259,   260,
     261,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,   131,
     132,    -1,   134,    -1,   136,   137,   138,    -1,    -1,    -1,
     142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,
     182,    -1,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,
     202,   203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,
     212,    -1,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,    -1,   226,   227,   228,   229,   230,   231,
     232,    -1,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     3,
     252,   253,    -1,   255,   256,   257,   258,   259,   260,   261,
      14,    -1,    -1,    -1,    18,    -1,    20,    21,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    33,
      -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,
     114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,
     124,   125,   126,    -1,   128,   129,    -1,   131,   132,    -1,
     134,    -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,
      -1,   145,   146,   147,    -1,    -1,   150,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
      -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,    -1,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
     194,   195,   196,   197,   198,    -1,    -1,    -1,   202,   203,
     204,   205,   206,   207,    -1,    -1,    -1,    -1,   212,    -1,
      -1,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,    -1,   226,   227,   228,   229,   230,   231,   232,    -1,
      -1,    -1,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,    -1,   252,   253,
      -1,   255,   256,   257,   258,   259,   260,   261,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    20,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    33,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    57,    -1,    59,    60,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,   124,
     125,   126,    -1,   128,   129,    -1,   131,   132,    -1,   134,
      -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,
     145,   146,   147,    -1,    -1,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,   179,   180,   181,   182,    -1,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,   194,
     195,   196,   197,   198,    -1,    -1,    -1,   202,   203,   204,
     205,   206,   207,    -1,    -1,    -1,    -1,   212,    -1,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
      -1,   226,   227,   228,   229,   230,   231,   232,    -1,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,     3,   252,   253,    -1,
     255,   256,   257,   258,   259,   260,   261,    14,    -1,    -1,
      -1,    18,    -1,    20,    21,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    -1,    33,    -1,    -1,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,    -1,    59,    60,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    -1,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,   114,   115,    -1,
     117,    -1,    -1,   120,    -1,    -1,    -1,   124,   125,   126,
      -1,   128,   129,    -1,   131,   132,    -1,   134,    -1,   136,
     137,   138,    -1,    -1,    -1,   142,    -1,    -1,   145,   146,
     147,    -1,    -1,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,   179,   180,   181,   182,    -1,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,   194,   195,   196,
     197,   198,    -1,    -1,    -1,   202,   203,   204,   205,   206,
     207,    -1,    -1,    -1,    -1,   212,    -1,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,    -1,   226,
     227,   228,   229,   230,   231,   232,    -1,    -1,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,     3,   252,   253,    -1,   255,   256,
     257,   258,   259,   260,   261,    14,    -1,    -1,    -1,    18,
      -1,    20,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    -1,    33,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    54,    -1,    -1,    57,    -1,
      59,    60,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    -1,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,   124,   125,   126,    -1,   128,
     129,    -1,   131,   132,    -1,   134,    -1,   136,   137,   138,
      -1,    -1,    -1,   142,    -1,    -1,   145,   146,   147,    -1,
      -1,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
     179,   180,   181,   182,    -1,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,   194,   195,   196,   197,   198,
      -1,    -1,    -1,   202,   203,   204,   205,   206,   207,    -1,
      -1,    -1,    -1,   212,    -1,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,    -1,   226,   227,   228,
     229,   230,   231,   232,    -1,    -1,    -1,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,     3,   252,   253,    -1,   255,   256,   257,   258,
     259,   260,   261,    14,    -1,    -1,    -1,    18,    -1,    20,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,
     131,   132,    -1,   134,    -1,   136,   137,   138,    -1,    -1,
      -1,   142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,
     181,   182,    -1,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,
      -1,   202,   203,   204,   205,   206,   207,    -1,    -1,    -1,
      -1,   212,    -1,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,    -1,   226,   227,   228,   229,   230,
     231,   232,    -1,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
       3,   252,   253,    -1,   255,   256,   257,   258,   259,   260,
     261,    14,    -1,    -1,    -1,    18,    -1,    20,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    -1,
      33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      -1,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,   124,   125,   126,    -1,   128,   129,    -1,   131,   132,
      -1,   134,    -1,   136,   137,   138,    -1,    -1,    -1,   142,
      -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,
      -1,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,   202,
     203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,   212,
      -1,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,    -1,   226,   227,   228,   229,   230,   231,   232,
      -1,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,     3,   252,
     253,    -1,   255,   256,   257,   258,   259,   260,   261,    14,
      -1,    -1,    -1,    18,    -1,    20,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    33,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    57,    -1,    59,    60,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,   124,
     125,   126,    -1,   128,   129,    -1,   131,   132,    -1,   134,
      -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,
     145,   146,   147,    -1,    -1,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,   179,   180,   181,   182,    -1,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,   194,
     195,   196,   197,   198,    -1,    -1,    -1,   202,   203,   204,
     205,   206,   207,    -1,    -1,    -1,    -1,   212,    -1,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
      -1,   226,   227,   228,   229,   230,   231,   232,    -1,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,    -1,   252,   253,    -1,
     255,   256,   257,   258,   259,   260,   261,     3,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    20,    21,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    -1,    33,    -1,    -1,
      36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    54,    -1,
      -1,    57,    -1,    59,    60,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,   113,   114,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,    -1,   124,   125,
     126,    -1,   128,   129,    -1,   131,   132,    -1,   134,    -1,
     136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,   145,
     146,   147,    -1,    -1,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,    -1,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,   179,   180,   181,   182,    -1,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,   194,   195,
     196,   197,   198,    -1,    -1,    -1,   202,   203,   204,   205,
     206,   207,    -1,    -1,    -1,    -1,   212,    -1,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,    -1,
     226,   227,   228,   229,   230,   231,   232,    -1,    -1,    -1,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,    -1,   252,   253,     3,   255,
     256,   257,   258,   259,   260,   261,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    20,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    33,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    57,    -1,    59,    60,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,   124,
     125,   126,    -1,   128,   129,    -1,   131,   132,    -1,   134,
      -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,
     145,   146,   147,    -1,    -1,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,   179,   180,   181,   182,    -1,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,   194,
     195,   196,   197,   198,    -1,    -1,    -1,   202,   203,   204,
     205,   206,   207,    -1,    -1,    -1,    -1,   212,    -1,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
      -1,   226,   227,   228,   229,   230,   231,   232,    -1,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,    -1,   252,   253,    -1,
     255,   256,   257,   258,   259,   260,   261,     3,    -1,     5,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    20,    21,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    -1,    33,    -1,    -1,
      36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    54,    -1,
      -1,    57,    -1,    59,    60,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,   113,   114,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,    -1,   124,   125,
     126,    -1,   128,   129,    -1,   131,   132,    -1,   134,    -1,
     136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,   145,
     146,   147,    -1,    -1,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,    -1,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,   179,   180,   181,   182,    -1,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,   194,   195,
     196,   197,   198,    -1,    -1,    -1,   202,   203,   204,   205,
     206,   207,    -1,    -1,    -1,    -1,   212,    -1,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,    -1,
     226,   227,   228,   229,   230,   231,   232,    -1,    -1,    -1,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,    -1,   252,   253,     3,   255,
     256,   257,   258,   259,   260,   261,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    20,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    33,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    57,    -1,    59,    60,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,   124,
     125,   126,    -1,   128,   129,    -1,   131,   132,    -1,   134,
      -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,
     145,   146,   147,    -1,    -1,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,   179,   180,   181,   182,    -1,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,   194,
     195,   196,   197,   198,    -1,    -1,    -1,   202,   203,   204,
     205,   206,   207,    -1,    -1,    -1,    -1,   212,    -1,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
      -1,   226,   227,   228,   229,   230,   231,   232,    -1,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,     3,   252,   253,    -1,
     255,   256,   257,   258,   259,   260,   261,    -1,    -1,    -1,
      -1,    18,    -1,    20,    21,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    -1,    33,    -1,    -1,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,    -1,    59,    60,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    -1,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,   114,   115,    -1,
     117,    -1,    -1,   120,    -1,    -1,    -1,   124,   125,   126,
      -1,   128,   129,    -1,   131,   132,    -1,   134,    -1,   136,
     137,   138,    -1,    -1,    -1,   142,    -1,    -1,   145,   146,
     147,    -1,    -1,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,   179,   180,   181,   182,    -1,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,   194,   195,   196,
     197,   198,    -1,    -1,    -1,   202,   203,   204,   205,   206,
     207,    -1,    -1,    -1,    -1,   212,    -1,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,    -1,   226,
     227,   228,   229,   230,   231,   232,    -1,    -1,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,     3,   252,   253,    -1,   255,   256,
     257,   258,   259,   260,   261,    -1,    -1,    -1,    -1,    18,
      -1,    20,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    -1,    33,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    54,    -1,    -1,    57,    -1,
      59,    60,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    -1,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,   124,   125,   126,    -1,   128,
     129,    -1,   131,   132,    -1,   134,    -1,   136,   137,   138,
      -1,    -1,    -1,   142,    -1,    -1,   145,   146,   147,    -1,
      -1,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
     179,   180,   181,   182,    -1,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,   194,   195,   196,   197,   198,
      -1,    -1,    -1,   202,   203,   204,   205,   206,   207,    -1,
      -1,    -1,    -1,   212,    -1,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,    -1,   226,   227,   228,
     229,   230,   231,   232,    -1,    -1,    -1,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,     3,   252,   253,    -1,   255,   256,   257,   258,
     259,   260,   261,    -1,    -1,    -1,    -1,    18,    -1,    20,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,   124,   125,   126,    -1,   128,   129,    -1,
     131,   132,    -1,   134,    -1,   136,   137,   138,    -1,    -1,
      -1,   142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,
     181,   182,    -1,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,   194,   195,   196,   197,   198,    -1,    -1,
      -1,   202,   203,   204,   205,   206,   207,    -1,    -1,    -1,
      -1,   212,    -1,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,    -1,   226,   227,   228,   229,   230,
     231,   232,    -1,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
       3,   252,   253,    -1,   255,   256,   257,   258,   259,   260,
     261,    -1,    -1,    -1,    -1,    18,    -1,    20,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    -1,
      33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      -1,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,   124,   125,   126,    -1,   128,   129,    -1,   131,   132,
      -1,   134,    -1,   136,   137,   138,    -1,    -1,    -1,   142,
      -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,
      -1,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,   194,   195,   196,   197,   198,    -1,    -1,    -1,   202,
     203,   204,   205,   206,   207,    -1,    -1,    -1,    -1,   212,
      -1,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,    -1,   226,   227,   228,   229,   230,   231,   232,
      -1,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,     3,   252,
     253,    -1,   255,   256,   257,   258,   259,   260,   261,    -1,
      -1,    -1,    -1,    18,    -1,    20,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    57,    -1,    -1,    60,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,
      95,    -1,    -1,    98,    -1,    -1,   101,   102,   103,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,
     125,   126,    -1,    -1,   129,    -1,   131,   132,    -1,   134,
      -1,    -1,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,   179,   180,   181,    -1,    -1,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,   194,
     195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,    -1,   252,   253,    -1,
     255,   256,   257,   258,   259,   260,   261
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    12,    28,    58,    83,   139,   156,   166,   178,   179,
     277,   278,   279,   280,   285,   286,   287,   291,   292,   298,
     299,   300,   301,   302,   308,   331,   479,   480,   299,     3,
      18,    20,    21,    24,    30,    31,    33,    36,    37,    45,
      46,    49,    54,    57,    59,    60,    62,    69,    72,    76,
      80,    87,    88,    89,    90,    94,    95,    98,    99,   100,
     101,   102,   103,   104,   107,   111,   113,   114,   115,   117,
     120,   124,   125,   126,   128,   129,   131,   132,   134,   136,
     137,   138,   142,   145,   146,   147,   150,   151,   152,   153,
     154,   159,   162,   163,   167,   168,   169,   177,   179,   180,
     181,   182,   184,   185,   186,   187,   188,   189,   190,   191,
     194,   195,   196,   197,   198,   202,   203,   204,   205,   206,
     207,   212,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   226,   227,   228,   229,   230,   231,   232,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   252,   253,   255,   256,   257,   258,
     259,   260,   261,   268,   284,   359,   413,   415,   424,   455,
     461,   463,    74,   194,    91,   172,   464,   293,   359,   131,
     332,   333,   461,   359,   359,     0,    65,   263,    68,    73,
     155,   251,   445,   446,   465,   466,   467,    86,   300,    13,
     461,    12,   166,   476,    14,     3,    12,   193,   214,   303,
     304,   305,   306,   344,   345,   346,   353,   359,   360,   440,
     461,   462,   463,    74,   359,     5,    10,    15,    61,   338,
     141,     4,    19,   123,   457,   458,   459,   461,   486,   332,
     264,    12,   334,   335,    12,   298,   334,   298,   334,   278,
     338,   156,   255,   260,   261,   338,   338,   476,   338,   465,
       4,     5,     6,     7,     8,    10,    12,    13,    29,    30,
      31,    36,    45,    47,    51,    52,    70,    96,   110,   111,
     133,   140,   148,   182,   207,   215,   220,   221,   222,   223,
     224,   225,   230,   231,   232,   270,   271,   281,   282,   283,
     304,   307,   358,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   404,   405,   410,   412,   413,   414,   415,
     423,   424,   440,   444,   455,   461,   175,   176,   177,    14,
     461,   299,    12,    12,   457,   165,   309,   457,   457,   457,
      73,   266,   268,   352,   467,   481,   482,   483,   484,   485,
     486,    12,   457,    14,   344,   158,   195,   288,   289,   290,
     298,   335,    12,   265,   339,   340,   390,   296,   297,   390,
     459,    12,   457,   333,   336,   461,    19,   290,   290,   301,
     115,   468,   256,    12,   448,   448,   301,   301,   302,   476,
      12,   376,   390,   396,   476,   390,   406,    12,    12,    12,
      12,    12,    12,   306,    12,   378,   390,    12,    12,    12,
      12,    12,    12,    12,    12,    12,    12,    12,    12,    12,
      12,   394,   394,    13,   264,    19,   118,    16,    92,    22,
      35,    38,    39,    40,    41,    42,    43,    44,    81,    92,
      97,   110,   208,   272,   273,   399,   400,   401,   460,   270,
     271,   402,   265,   274,   275,   403,   269,    12,    12,    12,
      12,    14,    12,    12,    12,    14,   461,    14,    13,    18,
     354,   355,   358,   461,   299,   440,   461,   335,   357,   376,
     476,   209,   210,   376,   347,   348,   461,   457,    13,   390,
     441,   442,   443,   461,   461,   462,   309,   390,   396,   196,
     197,   166,   477,   289,   290,    12,    74,   264,   310,   457,
      74,   264,   294,    39,     5,   487,    13,   264,   306,   166,
     309,   478,   166,   309,   337,   358,   173,   174,   470,     4,
     449,   450,   262,   447,   447,   299,    13,    13,   264,   164,
     407,   408,   390,     5,   396,   390,    61,   265,   390,     5,
     390,   390,    13,   396,   396,   396,   441,   455,   390,   390,
     396,   396,   441,   404,   404,   390,   476,   282,    23,    25,
      26,    34,    53,    56,    62,    71,    85,   108,   130,   143,
     151,   152,   159,   160,   184,   185,   186,   187,   188,   189,
     190,   191,   451,   452,   453,   454,   377,   378,     6,     9,
     110,   398,   390,   359,    39,    39,    41,    12,   306,   397,
       8,   110,   390,    22,    81,    97,   208,   115,    39,   273,
      15,    17,   144,   390,   456,   391,   390,   392,   393,   394,
     338,     4,    13,   396,   390,    14,   265,   461,     3,   410,
     474,   475,     4,     4,   461,    14,    14,   461,    12,    13,
     264,    14,    13,    13,    14,    19,    22,    74,    19,   267,
      14,   266,   211,   351,   488,    13,   264,   183,    14,   476,
     198,   198,   176,   477,    10,    12,   341,   342,   343,   344,
     361,   362,   369,   373,   340,   309,   295,   344,   297,   309,
     390,    13,   264,   461,   180,   477,   180,   264,     5,   254,
     469,    39,    13,   264,   453,    13,   390,   376,   390,    63,
     408,   409,    19,    13,    13,   157,   396,    13,    13,    13,
     264,   264,    13,    13,   119,   418,   419,    13,    74,   119,
     326,   326,   326,    13,    13,    13,    13,    13,    74,    12,
      12,    12,    12,    12,    12,    12,    18,   398,    16,   396,
       8,    66,   390,   397,   390,   115,   390,   390,    81,   122,
     425,    13,    13,    14,   461,    14,   264,    13,    13,    39,
     461,   461,    14,   396,    19,   166,   356,   354,   461,   461,
     115,     4,     4,   115,   268,   348,   350,   376,    12,   197,
     212,   442,   358,   404,   461,   462,   298,   298,    12,    19,
      12,   342,   361,   264,    48,    75,    82,    93,    96,   133,
     370,    77,   311,   264,   476,     5,   181,   181,   358,   253,
       4,   450,   149,   149,   390,    64,   451,   359,    13,   390,
     390,    27,    13,   425,   390,    27,    13,    13,    13,   425,
     425,   233,   416,   416,   390,     5,     5,     5,     5,     5,
       5,     5,   390,    13,   390,    16,    66,   390,    13,     4,
     358,   417,    12,   426,   427,   461,   425,   461,    14,    14,
     265,   461,   475,   476,   476,   390,   461,    13,     3,   192,
      14,    14,     4,    16,   153,     4,   347,   157,   349,     5,
      12,    12,    14,     4,   461,    13,   342,    93,   121,   371,
      88,    89,    90,   372,   342,   371,   371,    29,    93,   363,
      27,    79,   330,   344,   390,   390,    13,    13,    13,    13,
       5,   358,   420,   421,    13,   327,   328,   390,    77,   425,
     425,    13,    13,   264,    13,    13,    13,    13,    13,    13,
     390,   390,   166,    13,   427,   428,   429,    14,   265,   461,
     461,    14,    13,    12,    19,   461,   461,     4,     4,    88,
      89,   153,   267,    13,     5,     5,   462,    13,    12,   343,
     372,   153,   153,    89,   116,   372,   372,    93,   342,   338,
     376,   418,   422,   460,   422,   264,   264,    20,    60,   329,
      12,     5,     4,    13,   123,   430,   265,   461,    14,    14,
     265,   461,   476,   336,     3,    14,    14,   153,   153,    88,
      89,    13,    13,   374,   375,   461,    88,    89,    90,    88,
      89,    90,   153,   376,   164,   364,   365,   116,    12,    78,
     234,   235,   312,   313,   317,   390,   170,   471,   329,   329,
     421,   328,   326,    13,    27,   325,   326,   265,   461,   265,
     461,    14,    13,    12,     3,   463,    88,    89,    88,    89,
      93,    93,    13,   264,   453,    89,    89,    88,    89,    90,
     376,    63,   368,   364,   376,    13,   142,   264,   236,   318,
     319,     5,    10,   199,   411,   411,    13,   396,   127,   135,
     431,   432,   265,   461,   336,    93,    93,    93,    93,   375,
      89,   149,   213,   366,    64,   313,     5,    12,   471,   171,
     472,   472,   200,   201,    22,    50,   154,   404,   433,   434,
     435,   436,    13,   366,    12,   367,    12,   314,   315,   316,
     358,   166,   320,     5,    10,   239,   473,   473,   154,   404,
     434,   437,   438,   134,   125,   125,    69,   439,   339,    74,
      12,   316,   396,    13,   264,   237,   238,   239,   246,   324,
     252,    72,    72,    16,    50,    77,   109,   150,    13,   342,
     396,   325,    13,   315,     4,   240,   243,   321,   323,   437,
     134,   120,   116,    13,    13,   247,   241,   244,   242,   322,
     376,   248,   166,     4,   250,   245,   249,   153,   172
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   276,   277,   278,   278,   279,   279,   279,   279,   280,
     280,   280,   281,   281,   282,   282,   283,   283,   284,   285,
     286,   286,   286,   286,   286,   287,   288,   288,   288,   288,
     289,   289,   289,   290,   291,   291,   292,   293,   293,   294,
     294,   295,   295,   296,   296,   297,   298,   299,   299,   300,
     300,   300,   300,   301,   301,   302,   302,   303,   304,   305,
     306,   307,   308,   309,   309,   310,   311,   311,   312,   312,
     313,   313,   313,   314,   314,   315,   315,   315,   316,   317,
     317,   317,   318,   319,   319,   320,   320,   320,   320,   321,
     321,   322,   322,   323,   323,   323,   324,   324,   324,   324,
     325,   325,   326,   327,   327,   328,   329,   329,   329,   330,
     330,   331,   331,   332,   332,   333,   334,   334,   335,   336,
     336,   337,   337,   338,   338,   338,   339,   339,   340,   340,
     341,   341,   342,   342,   342,   343,   343,   344,   344,   344,
     344,   344,   344,   345,   345,   346,   346,   347,   347,   348,
     348,   349,   349,   349,   349,   349,   349,   349,   350,   350,
     351,   351,   352,   352,   352,   352,   352,   353,   354,   354,
     355,   355,   356,   356,   356,   356,   357,   357,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   360,   360,   360,   360,   361,   361,
     362,   362,   362,   363,   364,   364,   365,   366,   367,   368,
     368,   369,   370,   370,   370,   370,   371,   371,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   372,   373,   374,
     374,   375,   376,   376,   377,   377,   378,   378,   379,   379,
     379,   380,   380,   381,   381,   381,   381,   381,   381,   381,
     381,   382,   382,   383,   384,   384,   385,   385,   386,   386,
     386,   386,   387,   387,   388,   388,   389,   390,   390,   390,
     391,   391,   392,   392,   393,   393,   394,   394,   394,   395,
     395,   395,   395,   395,   395,   396,   396,   397,   397,   398,
     398,   399,   399,   399,   399,   399,   399,   399,   399,   399,
     399,   400,   400,   401,   402,   402,   403,   403,   403,   404,
     404,   404,   404,   404,   404,   405,   406,   406,   407,   407,
     408,   408,   409,   409,   410,   410,   410,   410,   410,   411,
     411,   411,   412,   412,   412,   412,   412,   413,   413,   413,
     413,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     414,   414,   414,   414,   414,   414,   414,   414,   414,   414,
     414,   414,   414,   415,   415,   415,   415,   416,   417,   417,
     417,   418,   418,   419,   420,   420,   421,   421,   422,   422,
     423,   423,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   425,   425,   425,   426,   427,   428,   429,   429,   430,
     430,   431,   431,   432,   432,   433,   433,   434,   434,   434,
     435,   436,   437,   437,   437,   438,   439,   439,   439,   439,
     439,   440,   440,   441,   441,   442,   442,   443,   443,   444,
     444,   444,   444,   444,   444,   444,   444,   444,   444,   444,
     444,   444,   445,   446,   447,   447,   448,   448,   449,   449,
     450,   451,   451,   452,   453,   453,   453,   453,   453,   453,
     453,   453,   453,   453,   453,   453,   453,   453,   453,   453,
     453,   453,   453,   453,   453,   453,   453,   453,   453,   453,
     453,   453,   453,   453,   454,   454,   454,   455,   455,   455,
     455,   455,   455,   456,   456,   456,   457,   457,   458,   458,
     459,   459,   460,   461,   461,   461,   461,   461,   461,   461,
     461,   461,   461,   461,   461,   461,   461,   461,   461,   461,
     462,   462,   463,   463,   463,   463,   463,   463,   463,   463,
     463,   463,   463,   463,   463,   463,   463,   463,   463,   463,
     463,   463,   463,   463,   463,   463,   463,   463,   463,   463,
     463,   463,   463,   463,   463,   463,   463,   463,   463,   463,
     463,   463,   463,   463,   463,   463,   463,   463,   463,   463,
     463,   463,   463,   463,   463,   463,   463,   463,   463,   463,
     463,   463,   463,   463,   463,   463,   463,   463,   463,   463,
     463,   463,   463,   463,   463,   463,   463,   463,   463,   463,
     463,   463,   463,   463,   463,   463,   463,   463,   463,   463,
     463,   463,   464,   464,   464,   465,   465,   466,   467,   467,
     467,   467,   468,   468,   469,   469,   470,   470,   470,   471,
     471,   471,   472,   472,   472,   473,   473,   474,   474,   475,
     475,   476,   476,   476,   476,   477,   477,   478,   479,   479,
     479,   480,   480,   480,   481,   481,   481,   482,   483,   484,
     485,   486,   487,   487,   488,   488,   488
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     3,     0,     1,     1,     1,     6,
       5,     3,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     2,     3,     2,
       1,     4,     4,     2,     5,     6,     7,     2,     3,     0,
       2,     1,     3,     1,     3,     3,     1,     3,     4,     1,
       4,     4,     4,     1,     4,     1,     3,     3,     1,     1,
       1,     1,    10,     0,     2,     2,     0,     4,     1,     3,
       2,     1,     5,     1,     3,     4,     1,     1,     3,     2,
       1,     1,     5,     0,     2,     0,     2,     2,     2,     2,
       2,     0,     2,     0,     2,     4,     0,     2,     4,     8,
       0,     1,     3,     1,     3,     2,     0,     1,     1,     0,
       2,     2,     3,     1,     3,     4,     0,     1,     3,     1,
       3,     1,     3,     0,     1,     1,     1,     3,     2,     1,
       1,     3,     1,     1,     1,     1,     3,     4,     3,     2,
       2,     2,     2,     4,     4,     6,     3,     1,     3,     5,
       1,     0,     4,     4,     5,     5,     5,     5,     0,     1,
       0,     1,     0,     1,     1,     1,     1,     5,     3,     1,
       4,     1,     7,     5,     4,     2,     0,     1,     1,     3,
       3,     5,     5,     7,     7,     6,     6,     9,     9,     8,
       8,     8,     8,     7,     7,     1,     3,     5,     4,     7,
       6,     6,     5,     2,     1,     3,     5,     7,     1,     1,
       6,     5,     3,     5,     1,     2,     4,     6,     3,     0,
       2,     4,     3,     3,     3,     2,     0,     1,     0,     3,
       3,     4,     3,     4,     3,     4,     4,     5,     6,     1,
       3,     2,     1,     3,     1,     3,     1,     2,     1,     3,
       4,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     4,     4,     3,     3,     5,     6,     3,     5,
       4,     6,     3,     4,     3,     4,     2,     1,     2,     3,
       1,     3,     1,     3,     1,     3,     1,     2,     2,     1,
       1,     1,     3,     1,     1,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     5,     0,     1,     1,     2,
       4,     4,     0,     2,     1,     1,     1,     1,     1,     0,
       2,     2,     4,     5,     4,     5,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     5,     6,     5,     5,     5,     5,     6,     6,     6,
       5,     6,     4,     1,     1,     1,     1,     5,     1,     1,
       3,     0,     1,     3,     1,     3,     4,     4,     0,     1,
       4,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     2,     2,     3,     1,     4,     0,     1,     0,
       3,     0,     3,     1,     1,     1,     1,     2,     2,     1,
       2,     4,     1,     2,     1,     2,     0,     3,     2,     2,
       3,     3,     4,     1,     3,     1,     1,     3,     3,     4,
       3,     4,     6,     6,     6,     4,     1,     4,     1,     6,
       1,     1,     4,     4,     0,     2,     0,     3,     1,     3,
       3,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     1,
       4,     1,     4,     1,     4,     1,     4,     1,     4,     6,
       1,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     2,     2,     0,     1,     3,     5,     1,
       1,     1,     0,     2,     0,     2,     0,     1,     2,     0,
       4,     4,     0,     2,     2,     0,     2,     1,     1,     1,
       3,     0,     6,     6,     8,     0,     5,     3,     5,     6,
       3,     5,     7,     3,     1,     1,     1,     5,     6,     6,
       5,     4,     1,     3,     5,     5,     4
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
      yyerror (&yylloc, result, scanner, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if HANA_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined HANA_LTYPE_IS_TRIVIAL && HANA_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParseResult* result, yyscan_t scanner)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (result);
  YYUSE (scanner);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParseResult* result, yyscan_t scanner)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, result, scanner);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, ParseResult* result, yyscan_t scanner)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !HANA_DEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !HANA_DEBUG */


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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ParseResult* result, yyscan_t scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (result);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 3: /* NAME  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5513 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 4: /* STRING  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5519 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 5: /* INTNUM  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5525 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 6: /* BOOL  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5531 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 7: /* APPROXNUM  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5537 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 8: /* NULLX  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5543 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 9: /* UNKNOWN  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5549 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 10: /* QUESTIONMARK  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5555 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 277: /* sql_stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5561 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 278: /* stmt_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5567 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 279: /* stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5573 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 280: /* call_stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5579 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 281: /* sql_argument_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5585 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 282: /* sql_argument  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5591 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 283: /* value_expression  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5597 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 284: /* sp_name  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5603 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 285: /* dql_stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5609 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 286: /* dml_stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5615 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 287: /* insert_stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5621 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 288: /* insert_columns_and_source  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5627 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 289: /* insert_select_stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5633 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 290: /* from_constructor  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5639 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 291: /* delete_stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5645 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 292: /* update_stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5651 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 293: /* update_table  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5657 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 294: /* opt_simple_from_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5663 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 295: /* simple_from_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5669 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 296: /* update_elem_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5675 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 297: /* update_elem  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5681 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 298: /* select_stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5687 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 299: /* query_expression  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5693 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 300: /* query_expression_body  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5699 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 301: /* query_term  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5705 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 302: /* query_primary  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5711 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 303: /* select_with_parens  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5717 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 304: /* subquery  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5723 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 305: /* sap_table_subquery  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5729 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 306: /* table_subquery  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5735 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 307: /* row_subquery  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5741 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 308: /* simple_table  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5747 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 309: /* opt_where  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5753 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 310: /* opt_from_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5759 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 311: /* opt_groupby  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5765 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 312: /* grouping_element_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5771 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 313: /* grouping_element  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5777 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 314: /* row_order_by_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5783 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 315: /* row_order_by  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5789 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 316: /* column_ref_perens  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5795 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 317: /* group_set_name  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5801 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 318: /* grouping_option  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5807 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 319: /* grouping_best  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5813 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 320: /* grouping_subtotal  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5819 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 321: /* grouping_prefix_tb  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5825 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 322: /* grouping_prefix  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5831 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 323: /* grouping_structured_res  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5837 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 324: /* grouping_text_filter  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5843 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 325: /* opt_order_by  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5849 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 326: /* order_by  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5855 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 327: /* sort_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5861 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 328: /* sort_key  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5867 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 329: /* opt_asc_desc  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5873 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 330: /* opt_having  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5879 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 331: /* with_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5885 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 332: /* with_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5891 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 333: /* common_table_expr  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5897 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 334: /* opt_derived_column_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5903 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 335: /* simple_ident_list_with_parens  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5909 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 336: /* simple_ident_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5915 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 337: /* column_ref_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5921 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 338: /* opt_distinct  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5927 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 339: /* select_expr_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5933 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 340: /* projection  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5939 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 341: /* from_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5945 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 342: /* table_reference  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5951 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 343: /* table_primary  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5957 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 344: /* table_primary_non_join  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5963 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 345: /* lateral_table  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5969 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 346: /* associated_table  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5975 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 347: /* associated_ref_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5981 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 348: /* associated_ref  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5987 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 349: /* opt_many2one_part  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5993 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 350: /* opt_search_condition  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 5999 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 351: /* opt_tablesample  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6005 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 352: /* opt_table_qualify  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6011 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 353: /* collection_derived_table  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6017 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 354: /* collection_value_expr_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6023 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 355: /* collection_value_expr  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6029 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 356: /* as_derived_part  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6035 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 357: /* opt_simple_ident_list_with_parens  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6041 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 358: /* column_ref  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6047 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 359: /* relation_factor  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6053 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 360: /* func_relation_factor  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6059 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 361: /* joined_table  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6065 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 362: /* qualified_join  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6071 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 363: /* case_join  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6077 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 364: /* case_join_when_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6083 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 365: /* case_join_when  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6089 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 366: /* ret_join_on  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6095 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 367: /* select_expr_list_parens  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6101 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 368: /* opt_case_join_else  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6107 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 369: /* cross_join  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6113 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 370: /* join_type  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6119 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 372: /* join_cardinality  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6125 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 373: /* hana_construct_table  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6131 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 374: /* construct_column_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6137 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 375: /* construct_column  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6143 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 376: /* search_condition  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6149 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 377: /* boolean_term  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6155 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 378: /* boolean_factor  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6161 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 379: /* boolean_test  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6167 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 380: /* boolean_primary  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6173 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 381: /* predicate  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6179 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 382: /* member_of_predicate  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6185 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 383: /* bool_function  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6191 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 384: /* comparison_predicate  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6197 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 385: /* between_predicate  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6203 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 386: /* like_predicate  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6209 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 387: /* in_predicate  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6215 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 388: /* null_predicate  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6221 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 389: /* exists_predicate  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6227 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 390: /* row_expr  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6233 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 391: /* factor0  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6239 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 392: /* factor1  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6245 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 393: /* factor2  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6251 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 394: /* factor3  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6257 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 395: /* factor4  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6263 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 396: /* row_expr_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6269 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 397: /* in_expr  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6275 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 398: /* truth_value  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6281 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 404: /* expr_const  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6287 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 405: /* case_expr  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6293 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 406: /* case_arg  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6299 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 407: /* when_clause_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6305 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 408: /* when_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6311 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 409: /* case_default  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6317 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 410: /* func_expr  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6323 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 411: /* opt_nulls  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6329 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 412: /* aggregate_expression  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6335 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 413: /* aggregate_name  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6341 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 414: /* sap_specific_function  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6347 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 415: /* regexpr_func_name  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6353 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 416: /* within_group  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6359 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 417: /* regexpr_in_val  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6365 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 418: /* opt_order_by_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6371 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 419: /* order_by_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6377 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 420: /* order_by_expression_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6383 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 421: /* order_by_expression  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6389 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 422: /* opt_collate_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6395 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 423: /* ranking_windowed_function  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6401 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 424: /* ranking_function_name  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6407 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 425: /* over_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6413 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 426: /* window_specification  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6419 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 427: /* window_name  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6425 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 428: /* window_specification_details  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6431 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 429: /* opt_existing_window_name  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6437 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 430: /* opt_window_partition_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6443 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 431: /* opt_window_frame_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6449 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 432: /* window_frame_units  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6455 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 433: /* window_frame_extent  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6461 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 434: /* window_frame_start  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6467 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 435: /* window_frame_preceding  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6473 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 436: /* window_frame_between  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6479 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 437: /* window_frame_bound  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6485 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 438: /* window_frame_following  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6491 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 439: /* opt_window_frame_exclusion  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6497 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 440: /* scalar_function  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6503 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 441: /* table_function_param_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6509 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 442: /* table_function_param  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6515 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 443: /* expr_point_val  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6521 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 444: /* function_call_keyword  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6527 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 445: /* for_xml  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6533 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 446: /* for_json  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6539 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 447: /* opt_returns_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6545 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 448: /* opt_option_string_list_p  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6551 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 449: /* option_string_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6557 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 450: /* option_string  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6563 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 451: /* data_type  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6569 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 452: /* array_type  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6575 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 453: /* predefined_type  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6581 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 454: /* lob_data_type  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6587 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 455: /* primary_datetime_field  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6593 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 457: /* opt_as_label  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6599 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 458: /* as_label  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6605 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 459: /* label  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6611 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 460: /* collate_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6617 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 461: /* name_r  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6623 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 462: /* name_f  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6629 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 463: /* reserved  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6635 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 464: /* top_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6641 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 465: /* opt_for_update  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6647 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 466: /* for_share_lock  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6653 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 467: /* for_update  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6659 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 468: /* opt_of_ident_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6665 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 469: /* opt_ignore_lock  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6671 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 470: /* wait_nowait  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6677 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 471: /* limit_num  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6683 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 472: /* offset_num  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6689 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 473: /* limit_total  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6695 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 474: /* with_hint_param  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6701 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 475: /* with_hint_param_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6707 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 476: /* hint_clause  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6713 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 477: /* opt_with_range_restrict  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6719 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 478: /* with_primary_key  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6725 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 479: /* upsert_stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6731 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 480: /* replace_stmt  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6737 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 481: /* for_system_time  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6743 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 482: /* sys_as_of_spec  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6749 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 483: /* sys_from_to_spec  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6755 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 484: /* sys_between_spec  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6761 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 485: /* for_application_time  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6767 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 486: /* partition_restriction  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6773 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 487: /* intnum_list  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6779 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;

    case 488: /* tablesample_num  */
#line 98 "sqlparser_hana.yacc" /* yacc.c:1257  */
      { delete(((*yyvaluep).node)); }
#line 6785 "sqlparser_hana_bison.cpp" /* yacc.c:1257  */
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (ParseResult* result, yyscan_t scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined HANA_LTYPE_IS_TRIVIAL && HANA_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

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

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

/* User initialization code.  */
#line 67 "sqlparser_hana.yacc" /* yacc.c:1429  */
{
	// Initialize
	yylloc.first_column = 0;
	yylloc.last_column = 0;
	yylloc.first_line = 0;
	yylloc.last_line = 0;
}

#line 6902 "sqlparser_hana_bison.cpp" /* yacc.c:1429  */
  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = yylex (&yylval, &yylloc, scanner);
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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 268 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    g_QuestMarkId = 0;
    (yyval.node) = (yyvsp[-1].node);
    result->result_tree_ = (yyvsp[-1].node);
    result->accept = true;
    YYACCEPT;
}
#line 7097 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 280 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_STMT_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &SEMICOLON_LIST_SERIALIZE_FORMAT;
}
#line 7106 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 287 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7112 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 295 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CALL, 3, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &CALL_SERIALIZE_FORMAT;
}
#line 7121 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 300 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CALL, 3, (yyvsp[-3].node), nullptr, (yyvsp[0].node));
    (yyval.node)->serialize_format = &CALL_SERIALIZE_FORMAT;
}
#line 7130 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 305 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CALL, 3, (yyvsp[-1].node), nullptr, (yyvsp[0].node));
    (yyval.node)->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
#line 7139 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 314 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_STMT_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 7148 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 322 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_SQL_ARGUMENT, E_SQL_ARGUMENT_PROPERTY_CNT, (yyvsp[0].node), nullptr);
    (yyval.node)->serialize_format = &SQL_ARGUMENT_SERIALIZE_FORMAT;
}
#line 7157 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 327 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_SQL_ARGUMENT, E_SQL_ARGUMENT_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &SQL_ARGUMENT_SERIALIZE_FORMAT;
}
#line 7166 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 356 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT, E_INSERT_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &INSERT_SERIALIZE_FORMAT;
}
#line 7175 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 364 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, 3, NULL, (yyvsp[0].node),NULL);
    (yyval.node)->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
}
#line 7184 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 369 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, 3, (yyvsp[-1].node), (yyvsp[0].node),NULL);
    (yyval.node)->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
}
#line 7193 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 374 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
}
#line 7202 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 379 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, 3, nullptr, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
}
#line 7211 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 388 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * over = Node::makeTerminalNode(E_STRING,"OVERRIDING SYSTEM VALUE");
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT_SELECT, 2, over, (yyvsp[0].node));
    (yyval.node)->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
#line 7221 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 394 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * over = Node::makeTerminalNode(E_STRING,"OVERRIDING USER VALUE");
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT_SELECT, 2, over, (yyvsp[0].node));
    (yyval.node)->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
#line 7231 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 403 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_VALUES_CTOR, E_VALUES_CTOR_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &TABLE_VALUE_CTOR_SERIALIZE_FORMAT;
}
#line 7240 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 412 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_DELETE, E_DELETE_PROPERTY_CNT,
    		nullptr,	/* E_DELETE_OPT_WITH 0 */
    		nullptr,	/* E_DELETE_OPT_TOP 1 */
    		(yyvsp[-2].node),		/* E_DELETE_DELETE_RELATION 2 */
    		nullptr,	/* E_DELETE_DELETE_RELATION_OPT_TABLE_HINT 3 */
    		nullptr,        /* E_DELETE_OPT_OUTPUT 4 */
    		nullptr,	/* E_DELETE_FROM_LIST 5 */
    		(yyvsp[-1].node),		/* E_DELETE_OPT_WHERE 6 */
    		(yyvsp[0].node) 	/* E_DELETE_OPT_QUERY_HINT 7 */);
    (yyval.node)->serialize_format = &DELETE_SERIALIZE_FORMAT;
}
#line 7257 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 425 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_DELETE, E_DELETE_PROPERTY_CNT,
        nullptr,	/* E_DELETE_OPT_WITH 0 */
        nullptr,	/* E_DELETE_OPT_TOP 1 */
        (yyvsp[-2].node),		/* E_DELETE_DELETE_RELATION 2 */
        nullptr,	/* E_DELETE_DELETE_RELATION_OPT_TABLE_HINT 3 */
        nullptr,        /* E_DELETE_OPT_OUTPUT 4 */
        nullptr,	/* E_DELETE_FROM_LIST 5 */
        (yyvsp[-1].node),		/* E_DELETE_OPT_WHERE 6 */
        (yyvsp[0].node) 	/* E_DELETE_OPT_QUERY_HINT 7 */);
    (yyval.node)->serialize_format = &DELETE_S_FORMAT;
}
#line 7274 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 442 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_UPDATE, E_UPDATE_PROPERTY_CNT,
    		nullptr,	/* E_UPDATE_OPT_WITH 0 */
    		nullptr,	/* E_UPDATE_OPT_TOP 1 */
    		(yyvsp[-5].node),		    /* E_UPDATE_UPDATE_RELATION 2 */
    		nullptr,	/* E_UPDATE_UPDATE_RELATION_OPT_TABLE_HINT 3 */
    		(yyvsp[-3].node),		    /* E_UPDATE_UPDATE_ELEM_LIST 4 */
    		nullptr,	/* E_UPDATE_OPT_OUTPUT 5 */
    		(yyvsp[-2].node),	        /* E_UPDATE_FROM_LIST 6 */
    		(yyvsp[-1].node),		    /* E_UPDATE_OPT_WHERE 7 */
    		(yyvsp[0].node)		    /* E_UPDATE_OPT_QUERY_HINT 8 */);
    (yyval.node)->serialize_format = &UPDATE_SERIALIZE_FORMAT;
}
#line 7292 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 458 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    if((yyvsp[0].node)){
        (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr, nullptr);
        (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
    } else {
        (yyval.node) = (yyvsp[-1].node);
    }
}
#line 7305 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 467 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node), nullptr, nullptr, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 7314 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 474 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 7320 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 476 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_FROM_CLAUSE, E_FROM_CLAUSE_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &FROM_SERIALIZE_FORMAT;
}
#line 7329 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 485 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_FROM_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 7338 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 494 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_UPDATE_ELEM_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 7347 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 502 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_EQ, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &OP_EQ_SERIALIZE_FORMAT;
}
#line 7356 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 511 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = (yyvsp[0].node);
    // 0 , 1  2 already
    // $$->setChild(E_DIRECT_SELECT_FOR_UPDATE, $2);
    // $$->setChild(E_DIRECT_HINT_CLAUSE, $3);
}
#line 7367 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 522 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_DIRECT_SELECT, E_DIRECT_SELECT_PROPERTY_CNT, nullptr, (yyvsp[-2].node), nullptr, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &SELECT_DIRECT_SERIALIZE_FORMAT;
}
#line 7376 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 527 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_DIRECT_SELECT, E_DIRECT_SELECT_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-2].node), nullptr,(yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &SELECT_DIRECT_SERIALIZE_FORMAT;
}
#line 7385 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 535 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* set_op = Node::makeTerminalNode(E_SET_UNION, "UNION");
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        nullptr,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                        nullptr,             /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        set_op,              /* E_SELECT_SET_OPERATION 6 */
                        (yyvsp[-1].node),                  /* E_SELECT_ALL_SPECIFIED 7 */
                        (yyvsp[-3].node),                  /* E_SELECT_FORMER_SELECT_STMT 8 */
                        (yyvsp[0].node),                  /* E_SELECT_LATER_SELECT_STMT 9 */
                        nullptr,             /* E_SELECT_ORDER_BY 10 */
                        nullptr,             /* E_SELECT_LIMIT 11 */
                        nullptr,             /* E_SELECT_FOR_UPDATE 12 */
                        nullptr,             /* E_SELECT_HINTS 13 */
                        nullptr,             /* E_SELECT_WHEN 14 */
                        nullptr,             /* E_SELECT_OPT_TOP 15 */
                        nullptr,             /* E_SELECT_OPT_WITH 16 */
                        nullptr,             /* E_SELECT_OPT_OPTION 17 */
                        nullptr              /* E_SELECT_OPT_INTO 18 */
                        );
    (yyval.node)->serialize_format = &SELECT_UNION_SERIALIZE_FORMAT;
}
#line 7415 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 561 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* set_op = Node::makeTerminalNode(E_SET_EXCEPT, "EXCEPT");
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        nullptr,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                        nullptr,             /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        set_op,              /* E_SELECT_SET_OPERATION 6 */
                        (yyvsp[-1].node),                  /* E_SELECT_ALL_SPECIFIED 7 */
                        (yyvsp[-3].node),                  /* E_SELECT_FORMER_SELECT_STMT 8 */
                        (yyvsp[0].node),                  /* E_SELECT_LATER_SELECT_STMT 9 */
                        nullptr,             /* E_SELECT_ORDER_BY 10 */
                        nullptr,             /* E_SELECT_LIMIT 11 */
                        nullptr,             /* E_SELECT_FOR_UPDATE 12 */
                        nullptr,             /* E_SELECT_HINTS 13 */
                        nullptr,             /* E_SELECT_WHEN 14 */
                        nullptr,             /* E_SELECT_OPT_TOP 15 */
                        nullptr,             /* E_SELECT_OPT_WITH 16 */
                        nullptr,             /* E_SELECT_OPT_OPTION 17 */
                        nullptr              /* E_SELECT_OPT_INTO 18 */
                        );
    (yyval.node)->serialize_format = &SELECT_EXCEPT_SERIALIZE_FORMAT;
}
#line 7445 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 587 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* set_op = Node::makeTerminalNode(E_SET_EXCEPT, "MINUS");
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        nullptr,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                        nullptr,             /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        set_op,              /* E_SELECT_SET_OPERATION 6 */
                        (yyvsp[-1].node),                  /* E_SELECT_ALL_SPECIFIED 7 */
                        (yyvsp[-3].node),                  /* E_SELECT_FORMER_SELECT_STMT 8 */
                        (yyvsp[0].node),                  /* E_SELECT_LATER_SELECT_STMT 9 */
                        nullptr,             /* E_SELECT_ORDER_BY 10 */
                        nullptr,             /* E_SELECT_LIMIT 11 */
                        nullptr,             /* E_SELECT_FOR_UPDATE 12 */
                        nullptr,             /* E_SELECT_HINTS 13 */
                        nullptr,             /* E_SELECT_WHEN 14 */
                        nullptr,             /* E_SELECT_OPT_TOP 15 */
                        nullptr,             /* E_SELECT_OPT_WITH 16 */
                        nullptr,             /* E_SELECT_OPT_OPTION 17 */
                        nullptr              /* E_SELECT_OPT_INTO 18 */
                        );
    (yyval.node)->serialize_format = &SELECT_EXCEPT_SERIALIZE_FORMAT;
}
#line 7475 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 617 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* set_op = Node::makeTerminalNode(E_SET_INTERSECT, "INTERSECT");
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        nullptr,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                        nullptr,             /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        set_op,              /* E_SELECT_SET_OPERATION 6 */
                        (yyvsp[-1].node),                  /* E_SELECT_ALL_SPECIFIED 7 */
                        (yyvsp[-3].node),                  /* E_SELECT_FORMER_SELECT_STMT 8 */
                        (yyvsp[0].node),                  /* E_SELECT_LATER_SELECT_STMT 9 */
                        nullptr,             /* E_SELECT_ORDER_BY 10 */
                        nullptr,             /* E_SELECT_LIMIT 11 */
                        nullptr,             /* E_SELECT_FOR_UPDATE 12 */
                        nullptr,             /* E_SELECT_HINTS 13 */
                        nullptr,             /* E_SELECT_WHEN 14 */
                        nullptr,             /* E_SELECT_OPT_TOP 15 */
                        nullptr,             /* E_SELECT_OPT_WITH 16 */
                        nullptr,             /* E_SELECT_OPT_OPTION 17 */
                        nullptr              /* E_SELECT_OPT_INTO 18 */
                        );
    (yyval.node)->serialize_format = &SELECT_INTERSECT_SERIALIZE_FORMAT;
}
#line 7505 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 647 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
   // if ($2->getChild(E_DIRECT_SELECT_WITH)) {
    //    yyerror(&@1, result, scanner, "error use common table expression");
   //     YYABORT;
   // }
    //$$ = Node::makeNonTerminalNode(E_SELECT_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2->getChild(E_DIRECT_SELECT_SELECT_CLAUSE));
    //$$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
    //$2->setChild(E_DIRECT_SELECT_SELECT_CLAUSE, nullptr);
    //delete($2);

    (yyval.node) = Node::makeNonTerminalNode(E_SELECT_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 7523 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 665 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 7532 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 683 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                    (yyvsp[-7].node),             /* E_SELECT_DISTINCT 0 */
                    (yyvsp[-6].node),             /* E_SELECT_SELECT_EXPR_LIST 1 */
                    (yyvsp[-5].node),             /* E_SELECT_FROM_LIST 2 */
                    (yyvsp[-4].node),             /* E_SELECT_OPT_WHERE 3 */
                    (yyvsp[-3].node),             /* E_SELECT_GROUP_BY 4 */
                    (yyvsp[-2].node),             /* E_SELECT_HAVING 5 */
                    nullptr,        /* E_SELECT_SET_OPERATION 6 */
                    nullptr,        /* E_SELECT_ALL_SPECIFIED 7 */
                    nullptr,        /* E_SELECT_FORMER_SELECT_STMT 8 */
                    nullptr,        /* E_SELECT_LATER_SELECT_STMT 9 */
                    (yyvsp[-1].node),             /* E_SELECT_ORDER_BY 10 */
                    (yyvsp[0].node),            /* E_SELECT_LIMIT 11 */
                    nullptr,        /* E_SELECT_FOR_UPDATE 12 */
                    nullptr,        /* E_SELECT_HINTS 13 */
                    nullptr,        /* E_SELECT_WHEN 14 */
                    (yyvsp[-8].node),             /* E_SELECT_OPT_TOP 15 */
                    nullptr,        /* E_SELECT_OPT_WITH 16 */
                    nullptr,        /* E_SELECT_OPT_OPTION 17 */
	                nullptr         /* E_SELECT_OPT_INTO 18 */
                    );
    (yyval.node)->serialize_format = &SELECT_SERIALIZE_FORMAT;
}
#line 7561 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 710 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7567 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 712 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_WHERE_CLAUSE, E_WHERE_CLAUSE_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &WHERE_SERIALIZE_FORMAT;
}
#line 7576 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 721 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_FROM_CLAUSE, E_FROM_CLAUSE_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &FROM_SERIALIZE_FORMAT;
}
#line 7585 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 728 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7591 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 730 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_GROUP_BY, E_GROUP_BY_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &GROUP_BY_SERIALIZE_FORMAT;
}
#line 7600 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 739 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 7609 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 746 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"()"); }
#line 7615 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 749 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string name = (yyvsp[-4].node)->Serialize();
    delete((yyvsp[-4].node));
    if((yyvsp[-3].node)){
        name += (yyvsp[-3].node)->Serialize();
        delete((yyvsp[-3].node));
    }
    Node * name_n = Node::makeTerminalNode(E_IDENTIFIER, name);
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT, name_n, nullptr, nullptr, (yyvsp[-1].node), nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
#line 7631 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 764 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 7640 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 771 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string ss = (yyvsp[-2].node)->Serialize();
    delete((yyvsp[-2].node));
    if((yyvsp[-1].node)){
        ss += (yyvsp[-1].node)->Serialize();
        delete((yyvsp[-1].node));
    }
    Node * node_s  = Node::makeTerminalNode(E_STRING, ss );
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, node_s);
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 7656 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 787 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 7665 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 794 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "GROUPING SETS ");}
#line 7671 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 795 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ROLLUP"); }
#line 7677 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 796 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node)  = Node::makeTerminalNode(E_IDENTIFIER, "CUBE"); }
#line 7683 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 800 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
      std::string val;
      if((yyvsp[-4].node)){ val += (yyvsp[-4].node)->Serialize(); delete((yyvsp[-4].node)); }
      if((yyvsp[-3].node)){ val += (yyvsp[-3].node)->Serialize(); delete((yyvsp[-3].node)); }
      if((yyvsp[-2].node)){ val += (yyvsp[-2].node)->Serialize(); delete((yyvsp[-2].node)); }
      if((yyvsp[-1].node)){ val += (yyvsp[-1].node)->Serialize(); delete((yyvsp[-1].node)); }
      if((yyvsp[0].node)){ val += (yyvsp[0].node)->Serialize(); delete((yyvsp[0].node)); }
      if(!val.empty()){
        (yyval.node) = Node::makeTerminalNode(E_STRING, val);
      } else { (yyval.node) = nullptr; }
}
#line 7699 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 814 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7705 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 815 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {    (yyval.node) = Node::makeTerminalNode(E_STRING, "BEST "+ (yyvsp[0].node)->Serialize()); delete((yyvsp[0].node)); }
#line 7711 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 818 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7717 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 819 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {     (yyval.node) = Node::makeTerminalNode(E_STRING, "WITH SUBTOTAL " ); }
#line 7723 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 820 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {  (yyval.node) = Node::makeTerminalNode(E_STRING, "WITH BALANCE " ); }
#line 7729 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 821 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {     (yyval.node) = Node::makeTerminalNode(E_STRING, "WITH TOTAL " ); }
#line 7735 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 825 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
     std::string pref_s ;
     if((yyvsp[-1].node)){ pref_s += (yyvsp[-1].node)->Serialize(); delete((yyvsp[-1].node)); }
     if((yyvsp[0].node)){ pref_s += (yyvsp[0].node)->Serialize(); delete((yyvsp[0].node)); }
     if(!pref_s.empty()){
         (yyval.node) = Node::makeTerminalNode( E_STRING, pref_s );
     } else { (yyval.node) = nullptr;  }
}
#line 7748 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 833 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {    (yyval.node) = Node::makeTerminalNode(E_STRING, " MULTIPLE RESULTSETS " ); }
#line 7754 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 836 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7760 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 837 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {     (yyval.node) = Node::makeTerminalNode(E_STRING, "PREFIX "+ (yyvsp[0].node)->Serialize() ); delete((yyvsp[0].node)); }
#line 7766 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 840 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7772 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 841 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {    (yyval.node) = Node::makeTerminalNode(E_STRING, "BEST INTNUM " ); }
#line 7778 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 842 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {    (yyval.node) = Node::makeTerminalNode(E_STRING, "STRUCTURED RESULT WITH OVERVIEW " ); }
#line 7784 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 845 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7790 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 846 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {  (yyval.node) = Node::makeTerminalNode(E_STRING, "TEXT_FILTER " + (yyvsp[0].node)->Serialize() + " " ); delete((yyvsp[0].node)); }
#line 7796 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 847 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {  (yyval.node) = Node::makeTerminalNode(E_STRING, "TEXT_FILTER " + (yyvsp[-2].node)->Serialize() +  " FILL UP " );  delete((yyvsp[-2].node)); }
#line 7802 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 848 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {  (yyval.node) = Node::makeTerminalNode(E_STRING, "TEXT_FILTER " + (yyvsp[-6].node)->Serialize() +  " FILL UP SORT MATCHES TO TOP " ); delete((yyvsp[-6].node)); }
#line 7808 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 853 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7814 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 859 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ORDER_BY, E_ORDER_BY_PROPERTY_CNT, (yyvsp[0].node), nullptr);
    (yyval.node)->serialize_format = &ORDER_BY_SERIALIZE_FORMAT;
}
#line 7823 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 868 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_SORT_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 7832 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 876 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_SORT_KEY, E_SORT_KEY_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &SORT_KEY_SERIALIZE_FORMAT;
}
#line 7841 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 884 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_SORT_ASC, "");
}
#line 7849 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 888 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_SORT_ASC, "ASC");
}
#line 7857 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 892 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_SORT_DESC, "DESC");
}
#line 7865 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 898 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7871 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 900 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_HAVING, E_HAVING_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &HAVING_SERIALIZE_FORMAT;
}
#line 7880 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 910 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OPT_WITH_CLAUSE, E_OPT_WITH_CLAUSE_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &WITH_CLAUSE_SERIALIZE_FORMAT;
}
#line 7889 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 915 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OPT_WITH_CLAUSE, E_OPT_WITH_CLAUSE_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT;
}
#line 7898 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 924 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_WITH_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 7907 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 933 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_COMMON_TABLE_EXPR, E_COMMON_TABLE_EXPR_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMON_TABLE_EXPR_SERIALIZE_FORMAT;
}
#line 7916 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 940 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7922 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 946 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_SIMPLE_IDENT_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 7931 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 955 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_SIMPLE_IDENT_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 7940 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 963 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_SIMPLE_IDENT_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 7949 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 969 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 7955 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 971 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_ALL, "ALL");
}
#line 7963 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 975 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_DISTINCT, "DISTINCT");
}
#line 7971 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 983 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 7980 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 991 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    if (!(yyvsp[0].node)) {
    	(yyval.node) = Node::makeNonTerminalNode(E_PROJECT_STRING, E_PROJECT_STRING_PROPERTY_CNT, (yyvsp[-1].node));
    	(yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
    }
    else {
	Node* alias_node = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr, nullptr);
        alias_node->serialize_format = &AS_SERIALIZE_FORMAT;

        (yyval.node) = Node::makeNonTerminalNode(E_PROJECT_STRING, E_PROJECT_STRING_PROPERTY_CNT, alias_node);
        (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
    }
}
#line 7998 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1005 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* star_node = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_PROJECT_STRING, E_PROJECT_STRING_PROPERTY_CNT, star_node);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 8008 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1015 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_FROM_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 8017 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1031 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_JOINED_TABLE_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 8026 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1042 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node), nullptr, (yyvsp[-2].node));
    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 8035 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1047 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 8044 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1052 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 8053 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1057 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 8062 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1062 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 8071 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1067 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 8080 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1075 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_LATERAL_QUERY, 1, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &LATERAL_QUERY_SERIALIZE_FORMAT;
}
#line 8089 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1080 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_LATERAL_QUERY, 1, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &LATERAL_QUERY_SERIALIZE_FORMAT;
}
#line 8098 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1088 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ASSOCIATED_TABLE, 3, (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &ASSOCIATED_TAB_SERIALIZE_FORMAT;
}
#line 8107 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1093 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ASSOCIATED_TABLE,3, (yyvsp[-2].node), nullptr, (yyvsp[0].node));
    (yyval.node)->serialize_format = &ASSOCIATED_TAB_SERIALIZE_FORMAT;
}
#line 8116 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1101 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ASSOCIATED_REF_LIST, 2, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &ASSOCIATED_LIST_SERIALIZE_FORMAT;
}
#line 8125 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1108 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ASSOCIATED_REF, 3, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &ASSOCIATED_REF_SERIALIZE_FORMAT;
}
#line 8134 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1113 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ASSOCIATED_REF, 3, (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
}
#line 8143 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1119 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node)=nullptr; }
#line 8149 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1120 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "USING TO ONE JOIN"); }
#line 8155 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1121 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "USING TO MANY JOIN"); }
#line 8161 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1122 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "USING MANY TO ONE JOIN"); }
#line 8167 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1123 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "USING MANY TO MANY JOIN"); }
#line 8173 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1124 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "USING ONE TO ONE JOIN"); }
#line 8179 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1125 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "USING ONE TO MANY JOIN"); }
#line 8185 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 1128 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node)=nullptr; }
#line 8191 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1133 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 8197 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1138 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 8203 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1148 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_UNNEST_TABLE, 2, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &UNNEST_TABLE_FORMAT;
}
#line 8212 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1156 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_COLLECT_VAL_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 8221 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1165 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_COLLECT_VAL, 1, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &COLLECT_VAL_FORMAT;
}
#line 8230 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 1174 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * p1 = Node::makeTerminalNode(E_STRING, "WITH ORDINALITY AS");
    (yyval.node) = Node::makeNonTerminalNode(E_AS_DERIVED_PART, 3, p1, (yyvsp[-3].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &AS_DERIVED_PART_FORMAT;
}
#line 8240 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 1180 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * p1 = Node::makeTerminalNode(E_STRING, "AS");
    (yyval.node) = Node::makeNonTerminalNode(E_AS_DERIVED_PART, 3, p1, (yyvsp[-3].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &AS_DERIVED_PART_FORMAT;
}
#line 8250 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 1186 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * p1 = Node::makeTerminalNode(E_STRING, "WITH ORDINALITY AS");
    (yyval.node) = Node::makeNonTerminalNode(E_AS_DERIVED_PART, 3, p1, (yyvsp[0].node),nullptr);
    (yyval.node)->serialize_format = &AS_DERIVED_PART_FORMAT;
}
#line 8260 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 1192 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * p1 = Node::makeTerminalNode(E_STRING, "AS");
    (yyval.node) = Node::makeNonTerminalNode(E_AS_DERIVED_PART, 3, p1, (yyvsp[0].node), nullptr);
    (yyval.node)->serialize_format = &AS_DERIVED_PART_FORMAT;
}
#line 8270 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 1201 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 8276 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 1208 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 8286 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 1214 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_1;
}
#line 8296 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 1220 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, (yyvsp[-2].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_1;
}
#line 8307 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1227 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_2;
}
#line 8317 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1233 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, (yyvsp[-2].node), (yyvsp[-4].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_2;
}
#line 8328 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 1240 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), (yyvsp[-6].node), nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_3;
}
#line 8338 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 1246 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, (yyvsp[-2].node), (yyvsp[-4].node), (yyvsp[-6].node), nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_3;
}
#line 8349 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 1253 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), nullptr, (yyvsp[-5].node), nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_3;
}
#line 8359 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 1259 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, (yyvsp[-2].node), nullptr, (yyvsp[-5].node), nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_3;
}
#line 8370 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 1266 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), (yyvsp[-6].node), (yyvsp[-8].node));
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 8380 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1272 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, (yyvsp[-2].node), (yyvsp[-4].node), (yyvsp[-6].node), (yyvsp[-8].node));
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 8391 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 1279 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), nullptr, (yyvsp[-7].node));
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 8401 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 1285 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, (yyvsp[-2].node), (yyvsp[-4].node), nullptr, (yyvsp[-7].node));
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 8412 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 1292 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), nullptr, (yyvsp[-5].node), (yyvsp[-7].node));
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 8422 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 1298 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, (yyvsp[-2].node), nullptr, (yyvsp[-5].node), (yyvsp[-7].node));
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 8433 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 1305 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), nullptr, nullptr, (yyvsp[-6].node));
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 8443 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 1311 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, (yyvsp[-2].node), nullptr, nullptr, (yyvsp[-6].node));
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 8454 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 1322 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 8463 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 1327 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_1;
}
#line 8472 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1332 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_2;
}
#line 8481 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1337 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), nullptr, (yyvsp[-3].node), nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_2;
}
#line 8490 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1342 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), (yyvsp[-6].node));
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
#line 8499 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1347 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), nullptr, (yyvsp[-5].node));
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
#line 8508 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1352 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), nullptr, (yyvsp[-3].node), (yyvsp[-5].node));
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
#line 8517 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1357 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), nullptr, nullptr, (yyvsp[-4].node));
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
#line 8526 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1362 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_VAR, 1, (yyvsp[0].node));
    (yyval.node)->serialize_format = &TABLE_VAR_SERIALIZE_FORMAT;
}
#line 8535 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1370 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 8544 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1375 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_1;
}
#line 8553 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1380 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_2;
}
#line 8562 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1385 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), (yyvsp[-6].node));
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
#line 8571 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1402 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, (yyvsp[-4].node), (yyvsp[-5].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &JOINED_TB_1_SERIALIZE_FORMAT;
}
#line 8580 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1407 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* nd = Node::makeTerminalNode(E_JOIN_INNER, "");
    (yyval.node) = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, nd, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &JOINED_TB_1_SERIALIZE_FORMAT;
}
#line 8590 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1413 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CASE_JOIN_TABLE, 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
}
#line 8599 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1433 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CASE_JOIN, 2, (yyvsp[-2].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &CASE_JOIN_SERIALIZE_FORMAT;
}
#line 8608 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1441 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CASE_JOIN_WHEN_LIST, 2, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &SPACE_LIST_SERIALIZE_FORMAT;
}
#line 8617 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1448 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CASE_JOIN_WHEN, 2, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &CASE_JOIN_WHEN_SERIALIZE_FORMAT;
}
#line 8626 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1455 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CASE_JOIN_RET, 3, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &CASE_JOIN_RET_SERIALIZE_FORMAT;
}
#line 8635 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1462 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 8644 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1468 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 8650 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1470 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CASE_JOIN_ELSE, 1, (yyvsp[0].node));
    (yyval.node)->serialize_format = &CASE_JOIN_ELSE_SERIALIZE_FORMAT;
}
#line 8659 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1478 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* nd = Node::makeTerminalNode(E_JOIN_CROSS, "CROSS");
    (yyval.node) = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, nd, (yyvsp[-3].node), (yyvsp[0].node), nullptr);
    (yyval.node)->serialize_format = &JOINED_TB_2_SERIALIZE_FORMAT;
}
#line 8669 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1503 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string val = "FULL ";
    if ((yyvsp[-1].ival))
    {
        val += "OUTER ";
    }
    if((yyvsp[0].node)){
        val+=(yyvsp[0].node)->text();
        delete((yyvsp[0].node));
    }
    (yyval.node) = Node::makeTerminalNode(E_JOIN_FULL, val);
}
#line 8686 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1516 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string val = "LEFT ";
    if ((yyvsp[-1].ival))
    {
        val += "OUTER ";
    }
    if((yyvsp[0].node)){
        val+=(yyvsp[0].node)->text();
        delete((yyvsp[0].node));
    }
    (yyval.node) = Node::makeTerminalNode(E_JOIN_LEFT, val);
}
#line 8703 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1529 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string val = "RIGHT ";
    if ((yyvsp[-1].ival))
    {
        val+="OUTER ";
    }
    if((yyvsp[0].node)){
        val += (yyvsp[0].node)->text();
        delete((yyvsp[0].node));
    }
    (yyval.node) = Node::makeTerminalNode(E_JOIN_RIGHT, val);

}
#line 8721 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1543 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string val = "INNER ";
    if((yyvsp[0].node)){
        val += (yyvsp[0].node)->text();
        delete((yyvsp[0].node));
    }
    (yyval.node) = Node::makeTerminalNode(E_JOIN_INNER, val);
}
#line 8734 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1554 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 8740 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1555 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.ival) = 1; /*this is a flag*/}
#line 8746 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1559 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 8752 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1560 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "MANY TO MANY"); }
#line 8758 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1561 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "MANY TO ONE"); }
#line 8764 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1562 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "MANY TO EXACT ONE"); }
#line 8770 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1563 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "ONE TO MANY"); }
#line 8776 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1564 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "EXACT ONE TO MANY"); }
#line 8782 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1565 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "ONE TO ONE"); }
#line 8788 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1566 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "EXACT ONE TO ONE"); }
#line 8794 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1567 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "ONE TO EXACT ONE"); }
#line 8800 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1568 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "EXACT ONE TO EXACT ONE"); }
#line 8806 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1573 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CONSTRUCT_FROM_TABLE, E_CONSTRUCT_FROM_TABLE_PROPERTY_CNT, (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &CONSTRUCT_FROM_TABLE_SERIALIZE_FORMAT;
}
#line 8815 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1582 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CONSTRUCT_COL_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 8824 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1590 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CONSTRUCT_COL, E_CONSTRUCT_COL_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &CONSTRUCT_COL_SERIALIZE_FORMAT;
}
#line 8833 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1600 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_OR, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_OR);
}
#line 8842 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1609 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_AND, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_AND);
}
#line 8851 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 1618 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NOT, E_OP_UNARY_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NOT);
}
#line 8860 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1627 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_IS, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_IS);
}
#line 8869 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1632 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_IS_NOT, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_IS_NOT);
}
#line 8878 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1641 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 8887 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 1670 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NOT_MEMBER, 2, (yyvsp[-4].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NOT_MEMBER);
}
#line 8896 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 1675 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_MEMBER, 2, (yyvsp[-3].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_MEMBER);
}
#line 8905 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 1682 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * node_str = Node::makeTerminalNode(E_STRING, "CONTAINS");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		node_str, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 8916 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 1691 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode((yyvsp[-1].nodetype), E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format((yyvsp[-1].nodetype));
}
#line 8925 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 1696 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode((yyvsp[-1].nodetype), E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format((yyvsp[-1].nodetype));
}
#line 8934 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 1706 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_BTW, E_OP_TERNARY_PROPERTY_CNT, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_BTW);
}
#line 8943 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 1711 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NOT_BTW, E_OP_TERNARY_PROPERTY_CNT, (yyvsp[-5].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NOT_BTW);
}
#line 8952 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 1719 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_LIKE, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_LIKE);
}
#line 8961 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 1724 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_LIKE, E_OP_TERNARY_PROPERTY_CNT, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_LIKE);
}
#line 8970 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 1729 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NOT_LIKE, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NOT_LIKE);
}
#line 8979 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 1734 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NOT_LIKE, E_OP_TERNARY_PROPERTY_CNT, (yyvsp[-5].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NOT_LIKE);
}
#line 8988 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 1742 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_IN, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_IN);
}
#line 8997 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 1747 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NOT_IN, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NOT_IN);
}
#line 9006 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 1755 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_IS, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_IS);
}
#line 9015 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 1760 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_IS_NOT, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_IS_NOT);
}
#line 9024 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 1768 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_EXISTS, E_OP_UNARY_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_EXISTS);
}
#line 9033 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 1778 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_COLLATE, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &SORT_KEY_SERIALIZE_FORMAT;
}
#line 9042 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 1783 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_CNN, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format((yyvsp[-1].nodetype));
}
#line 9051 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 1792 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_ADD, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format((yyvsp[-1].nodetype));
}
#line 9060 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 283:
#line 1801 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_MUL, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format((yyvsp[-1].nodetype));
}
#line 9069 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 285:
#line 1810 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_POW, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_POW);
}
#line 9078 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 1819 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_POS, E_OP_UNARY_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_POS);
}
#line 9087 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 1824 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NEG, E_OP_UNARY_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NEG);
}
#line 9096 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 1835 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 9105 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 296:
#line 1846 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 9114 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 298:
#line 1855 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 9123 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 301:
#line 1865 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_LE; }
#line 9129 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 302:
#line 1866 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_LT; }
#line 9135 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 303:
#line 1867 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_GE; }
#line 9141 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 304:
#line 1868 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_GT; }
#line 9147 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 305:
#line 1869 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_EQ; }
#line 9153 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 306:
#line 1870 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_NE; }
#line 9159 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 307:
#line 1871 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_GE; }
#line 9165 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 308:
#line 1872 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_LE; }
#line 9171 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 309:
#line 1873 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_NE; }
#line 9177 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 310:
#line 1874 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_NE; }
#line 9183 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 311:
#line 1878 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_CNN; }
#line 9189 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 312:
#line 1879 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_CNN; }
#line 9195 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 313:
#line 1885 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.nodetype) = Node::comp_all_some_any_op_form((yyvsp[-1].nodetype), (yyvsp[0].ival));
}
#line 9203 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 1891 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_ADD; }
#line 9209 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 315:
#line 1892 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_MINUS; }
#line 9215 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 1896 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_MUL; }
#line 9221 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 1897 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_DIV; }
#line 9227 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 318:
#line 1898 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.nodetype) = E_OP_REM; }
#line 9233 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 1913 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CASE, E_CASE_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_CASE);
}
#line 9242 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 1920 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 9248 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 1927 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_WHEN_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &SPACE_LIST_SERIALIZE_FORMAT;
}
#line 9257 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 1935 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_WHEN, E_WHEN_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &WHEN_SERIALIZE_FORMAT;
}
#line 9266 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 1940 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_WHEN, E_WHEN_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &WHEN_SERIALIZE_FORMAT;
}
#line 9275 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 1947 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 9281 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 333:
#line 1949 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_CASE_DEFAULT, E_CASE_DEFAULT_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &ELSE_SERIALIZE_FORMAT;
}
#line 9290 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 339:
#line 1965 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 9296 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 340:
#line 1966 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "NULLS FIRST"); }
#line 9302 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 341:
#line 1967 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "NULLS LAST"); }
#line 9308 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 342:
#line 1973 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "COUNT");
    Node* star = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		name, star, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
#line 9320 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 343:
#line 1981 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "COUNT");
    Node * dist = Node::makeTerminalNode(E_IDENTIFIER, "DISTINCT");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		name, (yyvsp[-1].node), nullptr, dist, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
#line 9332 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 344:
#line 1989 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "COUNT");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		name, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
#line 9343 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 345:
#line 1996 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-4].node), (yyvsp[-1].node), nullptr, (yyvsp[-2].node), nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
#line 9353 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 346:
#line 2002 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* name = Node::makeTerminalNode(E_STRING, "CHAR");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
            name, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
#line 9364 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 347:
#line 2011 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CORR"); }
#line 9370 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 348:
#line 2012 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CORR_SPEARMAN"); }
#line 9376 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 349:
#line 2013 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "MIN"); }
#line 9382 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 350:
#line 2014 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "MEDIAN"); }
#line 9388 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 351:
#line 2015 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "MAX"); }
#line 9394 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 352:
#line 2016 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SUM"); }
#line 9400 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 353:
#line 2017 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "AVG"); }
#line 9406 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 354:
#line 2018 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "STDDEV"); }
#line 9412 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 355:
#line 2019 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "VAR"); }
#line 9418 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 356:
#line 2020 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "STDDEV_POP"); }
#line 9424 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 357:
#line 2021 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "VAR_POP"); }
#line 9430 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 358:
#line 2022 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "STDDEV_SAMP"); }
#line 9436 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 359:
#line 2023 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "VAR_SAMP"); }
#line 9442 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 360:
#line 2028 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "BINNING");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
            name, (yyvsp[-2].node), (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 9453 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 361:
#line 2035 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "SERIES_FILTER");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
            name, (yyvsp[-2].node), (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 9464 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 362:
#line 2042 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "EXTRACT");
    Node * from = Node::makeTerminalNode(E_STRING, "FROM");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, from, nullptr, (yyvsp[-3].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
#line 9476 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 363:
#line 2050 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "FIRST_VALUE");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, nullptr, (yyvsp[-2].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
#line 9487 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 364:
#line 2057 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "LAST_VALUE");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, nullptr, (yyvsp[-2].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
#line 9498 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 365:
#line 2064 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "NTH_VALUE");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, nullptr, (yyvsp[-2].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
#line 9509 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 366:
#line 2071 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "NTILE");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
            name, (yyvsp[-2].node), (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 9520 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 367:
#line 2078 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * in_ = Node::makeTerminalNode(E_STRING, "IN");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                (yyvsp[-5].node), in_, nullptr, (yyvsp[-3].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
#line 9531 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 368:
#line 2085 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "PERCENTILE_CONT");
    Node * over = Node::makeTerminalNode(E_STRING, (yyvsp[-1].node)->text() + (yyvsp[0].node)->text());
    delete((yyvsp[-1].node));
    delete((yyvsp[0].node));
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, over, (yyvsp[-3].node) ,nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
#line 9545 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 369:
#line 2095 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "PERCENTILE_DISC");
    Node * over = Node::makeTerminalNode(E_STRING, (yyvsp[-1].node)->text() + (yyvsp[0].node)->text());
    delete((yyvsp[-1].node));
    delete((yyvsp[0].node));
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, over, (yyvsp[-3].node) ,nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
#line 9559 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 370:
#line 2105 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "STRING_AGG");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, nullptr, (yyvsp[-2].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
#line 9570 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 371:
#line 2112 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "TRIM");
    Node * from = Node::makeTerminalNode(E_STRING, "FROM");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, from, nullptr, (yyvsp[-3].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
#line 9582 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 372:
#line 2120 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "TRIM");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, nullptr, (yyvsp[-1].node), nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
#line 9593 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 373:
#line 2129 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "LOCATE_REGEXPR"); }
#line 9599 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 374:
#line 2130 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OCCURRENCES_REGEXPR"); }
#line 9605 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 375:
#line 2131 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node)  = Node::makeTerminalNode(E_IDENTIFIER, "REPLACE_REGEXPR"); }
#line 9611 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 376:
#line 2132 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SUBSTR_REGEXPR"); }
#line 9617 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 377:
#line 2136 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_WITHIN_GROUP, 1, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &WITHIN_GROUP_SERIALIZE_FORMAT;
}
#line 9626 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 380:
#line 2145 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string val = (yyvsp[-2].node)->text();
    val += " WITH ";
    val += (yyvsp[0].node)->text();
    delete((yyvsp[-2].node));
    delete((yyvsp[0].node));
    (yyval.node) = Node::makeTerminalNode(E_STRING, val);
}
#line 9639 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 381:
#line 2155 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node)=nullptr; }
#line 9645 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 383:
#line 2161 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ORDER_BY_CLAUSE, 1, (yyvsp[0].node));
    (yyval.node)->serialize_format = &AGG_ORDER_SERIALIZE_FORMAT;
}
#line 9654 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 385:
#line 2169 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 9663 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 386:
#line 2176 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ORDER_BY_EXPR, 4,
        		(yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &QUADRUPLE_SERIALIZE_FORMAT;
}
#line 9673 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 387:
#line 2182 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_ORDER_BY_EXPR, 4,
            (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &QUADRUPLE_SERIALIZE_FORMAT;
}
#line 9683 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 388:
#line 2189 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = nullptr;}
#line 9689 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 390:
#line 2195 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-3].node), nullptr, (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 9699 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 391:
#line 2201 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
            (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 9709 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 392:
#line 2210 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "RANK"); }
#line 9715 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 393:
#line 2211 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "DENSE_RANK"); }
#line 9721 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 394:
#line 2212 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "PERCENT_RANK"); }
#line 9727 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 395:
#line 2213 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CUME_DIST"); }
#line 9733 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 396:
#line 2214 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ROW_NUMBER"); }
#line 9739 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 397:
#line 2215 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "LAG"); }
#line 9745 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 398:
#line 2216 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "LEAD"); }
#line 9751 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 399:
#line 2217 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "RANDOM_PARTITION"); }
#line 9757 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 400:
#line 2218 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "WEIGHTED_AVG"); }
#line 9763 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 401:
#line 2222 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) =nullptr; }
#line 9769 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 402:
#line 2224 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OVER "+ (yyvsp[0].node)->text()); delete((yyvsp[0].node)); }
#line 9775 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 403:
#line 2226 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 9781 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 404:
#line 2231 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[-1].node); }
#line 9787 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 406:
#line 2240 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OVER_CLAUSE, E_OVER_CLAUSE_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &OVER_CLAUSE_SERIALIZE_FORMAT;
}
#line 9796 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 407:
#line 2247 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 9802 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 409:
#line 2252 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 9808 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 410:
#line 2254 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 9814 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 411:
#line 2258 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 9820 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 412:
#line 2260 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string s3 = (yyvsp[0].node) ? (yyvsp[0].node)->text() : "";
    (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, (yyvsp[-2].node)->text()+" "+(yyvsp[-1].node)->text()+" "+s3);
    delete((yyvsp[-2].node)); delete((yyvsp[-1].node)); delete((yyvsp[0].node));
}
#line 9830 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 413:
#line 2268 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"ROWS"); }
#line 9836 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 414:
#line 2269 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"RANGE"); }
#line 9842 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 417:
#line 2278 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"UNBOUNDED PRECEDING"); }
#line 9848 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 418:
#line 2279 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"CURRENT ROW"); }
#line 9854 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 420:
#line 2284 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,(yyvsp[-1].node)->text()+" PRECEDING"); delete((yyvsp[-1].node)); }
#line 9860 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 421:
#line 2289 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "BETWEEN "+(yyvsp[-2].node)->text()+" AND "+(yyvsp[0].node)->text()); delete((yyvsp[-2].node)); delete((yyvsp[0].node)); }
#line 9866 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 423:
#line 2294 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"UNBOUNDED FOLLOWING"); }
#line 9872 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 425:
#line 2299 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,(yyvsp[-1].node)->text()+" FOLLOWING"); delete((yyvsp[-1].node)); }
#line 9878 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 426:
#line 2303 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 9884 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 427:
#line 2304 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE CURRENT ROW"); }
#line 9890 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 428:
#line 2305 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE GROUP"); }
#line 9896 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 429:
#line 2306 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE TIES"); }
#line 9902 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 430:
#line 2307 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE NO OTHERS"); }
#line 9908 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 431:
#line 2312 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-2].node), nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 9918 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 432:
#line 2318 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-3].node), (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 9928 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 434:
#line 2329 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 9937 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 437:
#line 2341 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
        (yyval.node) = Node::makeNonTerminalNode(E_POINT_VALUE, 2, (yyvsp[-2].node), (yyvsp[0].node));
        (yyval.node)->serialize_format = &POINT_VALUE_FORMAT;
}
#line 9946 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 438:
#line 2346 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
        (yyval.node) = Node::makeNonTerminalNode(E_POINT_VALUE, 2, (yyvsp[-2].node), (yyvsp[0].node));
        (yyval.node)->serialize_format = &POINT_VALUE_FORMAT;
}
#line 9955 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 439:
#line 2357 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-3].node), (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 9965 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 440:
#line 2363 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "RIGHT");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 9976 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 441:
#line 2370 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "RIGHT");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
        fun_name, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 9987 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 442:
#line 2377 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* expr_list = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-1].node));
    expr_list->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;

    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "LEFT");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, expr_list, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 10001 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 443:
#line 2387 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CAST");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, (yyvsp[-3].node), nullptr, nullptr, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &FUN_CALL_AS_SERIALIZE_FORMAT;
}
#line 10012 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 444:
#line 2394 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* transcoding_name = Node::makeTerminalNode(E_STRING, (yyvsp[-1].node)->text());
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CONVERT");
    delete((yyvsp[-1].node));
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
        		fun_name, (yyvsp[-3].node), nullptr, nullptr, transcoding_name);
    (yyval.node)->serialize_format = &FUN_CALL_USING_SERIALIZE_FORMAT;
}
#line 10025 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 445:
#line 2403 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "COALESCE");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 10036 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 446:
#line 2410 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CURRENT_TIMESTAMP");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 10047 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 447:
#line 2417 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CURRENT_TIMESTAMP");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 10058 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 448:
#line 2424 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CURRENT_USER");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 10069 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 449:
#line 2431 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* expr_list = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-1].node));
    expr_list->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "NULLIF");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, expr_list, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 10082 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 450:
#line 2440 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "SESSION_USER");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 10093 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 451:
#line 2447 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "SYSTEM_USER");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 10104 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 452:
#line 2456 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* xml = Node::makeTerminalNode(E_STRING, "XML");
    (yyval.node) = Node::makeNonTerminalNode(E_FOR_CLAUSE_HANA, 4, xml, (yyvsp[-1].node), (yyvsp[0].node), nullptr);
    (yyval.node)->serialize_format = &FOR_CLAUSE_FORMAT;
}
#line 10114 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 453:
#line 2463 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node* json = Node::makeTerminalNode(E_STRING, "JSON");
    (yyval.node) = Node::makeNonTerminalNode(E_FOR_CLAUSE_HANA, 4, json, (yyvsp[-1].node), (yyvsp[0].node), nullptr);
    (yyval.node)->serialize_format = &FOR_CLAUSE_FORMAT;
}
#line 10124 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 454:
#line 2470 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = NULL;}
#line 10130 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 455:
#line 2472 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_RETURNS_CLAUSE, 1, (yyvsp[0].node));
    (yyval.node)->serialize_format = &RETURNS_CLAUSE_FORMAT;
}
#line 10139 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 456:
#line 2478 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = NULL;}
#line 10145 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 457:
#line 2480 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 10154 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 459:
#line 2488 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 10163 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 460:
#line 2495 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OPTION_STRING, 2, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &OPTION_STRING_FORMAT;
}
#line 10172 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 463:
#line 2512 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = (yyvsp[-1].node);
    (yyvsp[-1].node)->set_text( (yyvsp[-1].node)->text() + " ARRAY ");
}
#line 10181 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 464:
#line 2556 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "DATE");}
#line 10187 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 465:
#line 2557 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "TIME");}
#line 10193 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 466:
#line 2558 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "SECONDDATE");}
#line 10199 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 467:
#line 2559 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "TIMESTAMP");}
#line 10205 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 468:
#line 2560 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "TINYINT");}
#line 10211 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 469:
#line 2561 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "SMALLINT");}
#line 10217 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 470:
#line 2562 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "INTEGER");}
#line 10223 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 471:
#line 2563 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "BIGINT");}
#line 10229 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 472:
#line 2564 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "SMALLDECIMAL");}
#line 10235 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 473:
#line 2565 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "REAL");}
#line 10241 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 474:
#line 2566 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "DOUBLE");}
#line 10247 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 475:
#line 2567 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "TEXT");}
#line 10253 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 476:
#line 2568 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "BINTEXT");}
#line 10259 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 477:
#line 2569 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "VARCHAR");}
#line 10265 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 478:
#line 2570 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "VARCHAR("+(yyvsp[-1].node)->text()+")"); delete((yyvsp[-1].node)); }
#line 10271 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 479:
#line 2571 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "NVARCHAR");}
#line 10277 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 480:
#line 2572 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "NVARCHAR("+(yyvsp[-1].node)->text()+")"); delete((yyvsp[-1].node)); }
#line 10283 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 481:
#line 2573 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "ALPHANUM");}
#line 10289 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 482:
#line 2574 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "ALPHANUM("+(yyvsp[-1].node)->text()+")"); delete((yyvsp[-1].node)); }
#line 10295 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 483:
#line 2575 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "VARBINARY");}
#line 10301 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 484:
#line 2576 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "VARBINARY("+(yyvsp[-1].node)->text()+")"); delete((yyvsp[-1].node)); }
#line 10307 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 485:
#line 2577 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "SHORTTEXT");}
#line 10313 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 486:
#line 2578 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "SHORTTEXT("+(yyvsp[-1].node)->text()+")"); delete((yyvsp[-1].node)); }
#line 10319 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 487:
#line 2579 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "DECIMAL");}
#line 10325 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 488:
#line 2580 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "DECIMAL("+(yyvsp[-1].node)->text()+")"); delete((yyvsp[-1].node)); }
#line 10331 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 489:
#line 2581 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "DECIMAL("+(yyvsp[-3].node)->text()+","+(yyvsp[-1].node)->text()+")"); delete((yyvsp[-3].node));delete((yyvsp[-1].node)); }
#line 10337 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 490:
#line 2582 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "FLOAT");}
#line 10343 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 491:
#line 2583 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "FLOAT("+(yyvsp[-1].node)->text()+")"); delete((yyvsp[-1].node)); }
#line 10349 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 492:
#line 2584 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "BOOLEAN");}
#line 10355 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 494:
#line 2588 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "BLOB");}
#line 10361 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 495:
#line 2589 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "CLOB");}
#line 10367 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 496:
#line 2590 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = Node::makeTerminalNode(E_STRING, "NCLOB");}
#line 10373 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 497:
#line 2677 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "YEAR");
}
#line 10381 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 498:
#line 2681 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "MONTH");
}
#line 10389 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 499:
#line 2685 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "DAY");
}
#line 10397 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 500:
#line 2689 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "HOUR");
}
#line 10405 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 501:
#line 2693 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "MINUTE");
}
#line 10413 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 502:
#line 2697 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "SECOND");
}
#line 10421 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 503:
#line 3072 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 10427 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 504:
#line 3073 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 10433 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 505:
#line 3074 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.ival) = 2; }
#line 10439 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 506:
#line 3078 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 10445 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 508:
#line 3083 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 10451 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 512:
#line 3095 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "COLLATE "+(yyvsp[0].node)->text());
    delete((yyvsp[0].node));
}
#line 10460 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 518:
#line 3108 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "STRING_AGG"); }
#line 10466 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 519:
#line 3109 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CHAR"); }
#line 10472 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 520:
#line 3110 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CONTAINS"); }
#line 10478 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 521:
#line 3111 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "BINNING"); }
#line 10484 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 522:
#line 3112 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "EXTRACT"); }
#line 10490 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 523:
#line 3113 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "FIRST_VALUE"); }
#line 10496 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 524:
#line 3114 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "LAST_VALUE"); }
#line 10502 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 525:
#line 3115 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "NTH_VALUE"); }
#line 10508 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 526:
#line 3116 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "NTILE"); }
#line 10514 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 527:
#line 3117 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "PERCENTILE_CONT"); }
#line 10520 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 528:
#line 3118 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "PERCENTILE_DISC"); }
#line 10526 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 529:
#line 3119 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TRIM"); }
#line 10532 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 533:
#line 3130 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "K"); }
#line 10538 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 534:
#line 3131 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "M"); }
#line 10544 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 535:
#line 3132 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "G"); }
#line 10550 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 536:
#line 3133 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ARRAY"); }
#line 10556 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 537:
#line 3134 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "BINARY"); }
#line 10562 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 538:
#line 3135 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CAST"); }
#line 10568 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 539:
#line 3136 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CHARACTERS"); }
#line 10574 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 540:
#line 3137 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CODE_UNITS"); }
#line 10580 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 541:
#line 3138 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CORRESPONDING"); }
#line 10586 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 542:
#line 3139 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "FOLLOWING"); }
#line 10592 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 543:
#line 3140 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "INTERVAL"); }
#line 10598 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 544:
#line 3141 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "LARGE"); }
#line 10604 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 545:
#line 3142 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "MULTISET"); }
#line 10610 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 546:
#line 3143 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OBJECT"); }
#line 10616 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 547:
#line 3144 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OCTETS"); }
#line 10622 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 548:
#line 3145 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ONLY"); }
#line 10628 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 549:
#line 3146 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "PRECEDING"); }
#line 10634 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 550:
#line 3147 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "PRECISION"); }
#line 10640 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 551:
#line 3148 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "RECURSIVE"); }
#line 10646 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 552:
#line 3149 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "REF"); }
#line 10652 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 553:
#line 3150 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ROW"); }
#line 10658 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 554:
#line 3151 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SCOPE"); }
#line 10664 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 555:
#line 3152 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "UNBOUNDED"); }
#line 10670 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 556:
#line 3153 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "VARCHAR"); }
#line 10676 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 557:
#line 3154 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "WITHOUT"); }
#line 10682 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 558:
#line 3155 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ZONE"); }
#line 10688 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 559:
#line 3156 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OF"); }
#line 10694 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 560:
#line 3157 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "READ"); }
#line 10700 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 561:
#line 3158 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TIMESTAMP"); }
#line 10706 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 562:
#line 3159 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TIME");  }
#line 10712 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 563:
#line 3160 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "DESC"); }
#line 10718 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 564:
#line 3161 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TIES"); }
#line 10724 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 565:
#line 3162 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SETS"); }
#line 10730 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 566:
#line 3163 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OTHERS"); }
#line 10736 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 567:
#line 3164 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "EXCLUDE"); }
#line 10742 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 568:
#line 3165 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ASC"); }
#line 10748 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 569:
#line 3166 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "COALESCE"); }
#line 10754 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 570:
#line 3167 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CONVERT"); }
#line 10760 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 571:
#line 3168 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "NULLIF"); }
#line 10766 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 572:
#line 3169 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "DEFAULT"); }
#line 10772 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 573:
#line 3170 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TO"); }
#line 10778 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 574:
#line 3171 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "DOUBLE"); }
#line 10784 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 575:
#line 3172 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "MOD"); }
#line 10790 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 576:
#line 3173 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "MANY"); }
#line 10796 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 577:
#line 3174 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ONE"); }
#line 10802 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 578:
#line 3175 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "PRIMARY"); }
#line 10808 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 579:
#line 3176 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "KEY"); }
#line 10814 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 580:
#line 3177 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SECONDDATE"); }
#line 10820 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 581:
#line 3178 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TINYINT"); }
#line 10826 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 582:
#line 3179 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SMALLDECIMAL"); }
#line 10832 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 583:
#line 3180 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TEXT"); }
#line 10838 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 584:
#line 3181 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "BINTEXT"); }
#line 10844 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 585:
#line 3182 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ALPHANUM"); }
#line 10850 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 586:
#line 3183 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "VARBINARY"); }
#line 10856 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 587:
#line 3184 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SHORTTEXT"); }
#line 10862 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 588:
#line 3185 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "REPLACE"); }
#line 10868 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 589:
#line 3186 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "PARAMETERS"); }
#line 10874 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 590:
#line 3187 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "HISTORY"); }
#line 10880 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 591:
#line 3188 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OVERRIDING"); }
#line 10886 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 592:
#line 3189 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SYSTEM"); }
#line 10892 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 593:
#line 3190 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "USER"); }
#line 10898 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 594:
#line 3191 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "VALUE"); }
#line 10904 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 595:
#line 3192 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "EXACT"); }
#line 10910 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 596:
#line 3193 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "BERNOULLI"); }
#line 10916 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 597:
#line 3194 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "NCHAR"); }
#line 10922 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 598:
#line 3197 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "BEST"); }
#line 10928 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 599:
#line 3198 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SUBTOTAL"); }
#line 10934 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 600:
#line 3199 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "BALANCE"); }
#line 10940 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 601:
#line 3200 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TOTAL"); }
#line 10946 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 602:
#line 3201 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "MULTIPLE"); }
#line 10952 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 603:
#line 3202 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "RESULTSETS"); }
#line 10958 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 604:
#line 3203 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "PREFIX"); }
#line 10964 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 605:
#line 3204 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "STRUCTURED"); }
#line 10970 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 606:
#line 3205 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "RESULT"); }
#line 10976 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 607:
#line 3206 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OVERVIEW"); }
#line 10982 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 608:
#line 3207 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TEXT_FILTER"); }
#line 10988 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 609:
#line 3208 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "FILL"); }
#line 10994 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 610:
#line 3209 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "UP"); }
#line 11000 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 611:
#line 3210 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "MATCHES"); }
#line 11006 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 612:
#line 3211 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SORT"); }
#line 11012 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 613:
#line 3212 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ROWCOUNT"); }
#line 11018 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 614:
#line 3213 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "LOCKED"); }
#line 11024 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 615:
#line 3215 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SHARE"); }
#line 11030 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 616:
#line 3216 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "LOCK"); }
#line 11036 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 617:
#line 3217 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "UTCTIMESTAMP"); }
#line 11042 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 618:
#line 3218 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "COMMIT"); }
#line 11048 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 619:
#line 3219 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ID"); }
#line 11054 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 620:
#line 3220 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "JSON"); }
#line 11060 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 621:
#line 3221 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "XML"); }
#line 11066 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 622:
#line 3227 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 11072 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 623:
#line 3229 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TOP_CLAUSE, E_OP_UNARY_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &TOP_CLAUSE_FORMAT;
}
#line 11081 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 624:
#line 3234 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_TOP_CLAUSE, E_OP_UNARY_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &TOP_CLAUSE_FORMAT;
}
#line 11090 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 625:
#line 3258 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr;}
#line 11096 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 627:
#line 3263 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, " FOR SHARE LOCK "); }
#line 11102 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 628:
#line 3267 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_FOR_UPDATE, 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &FOR_UPDATE_FORMAT_HANA;
}
#line 11111 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 632:
#line 3277 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = nullptr;}
#line 11117 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 633:
#line 3279 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * p_of = Node::makeTerminalNode(E_UPDATE, "OF");
    (yyval.node) = Node::makeNonTerminalNode(E_OF_COLUMS, 2, p_of, (yyvsp[0].node));
    (yyval.node)->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
#line 11127 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 634:
#line 3286 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = nullptr;}
#line 11133 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 635:
#line 3287 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_STRING, "IGNORE LOCKED");}
#line 11139 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 636:
#line 3290 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {(yyval.node) = nullptr;}
#line 11145 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 637:
#line 3291 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = Node::makeTerminalNode(E_NOWAIT, "NOWAIT");}
#line 11151 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 638:
#line 3293 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_WAIT_INT, 1,  (yyvsp[0].node));
    (yyval.node)->serialize_format = &WAIT_TIME_FORMAT;
}
#line 11160 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 639:
#line 3300 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 11166 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 640:
#line 3302 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_LIMIT_NUM, 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &LIMIT_NUM_FORMAT_HANA;
}
#line 11175 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 641:
#line 3307 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_LIMIT_NUM, 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &LIMIT_NUM_FORMAT_HANA;
}
#line 11184 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 642:
#line 3314 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 11190 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 643:
#line 3316 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OFFSET_NUM, 1, (yyvsp[0].node));
    (yyval.node)->serialize_format = &OFFSET_NUM_FORMAT;
}
#line 11199 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 644:
#line 3321 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_OFFSET_NUM, 1, (yyvsp[0].node));
    (yyval.node)->serialize_format = &OFFSET_NUM_FORMAT;
}
#line 11208 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 645:
#line 3328 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 11214 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 646:
#line 3330 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TOTAL ROWCOUNT");
}
#line 11222 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 650:
#line 3342 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 11231 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 651:
#line 3349 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 11237 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 652:
#line 3351 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
     std::string ss = (yyvsp[-2].node)->text();
     delete((yyvsp[-2].node));
    std::string sval = "WITH HINT(" + ss + ")";
    if((yyvsp[0].node)){
        sval += (yyvsp[0].node)->text();
        delete((yyvsp[0].node));
    }
    (yyval.node) = Node::makeTerminalNode(E_STRING, sval);
}
#line 11252 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 653:
#line 3362 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string ss = (yyvsp[-2].node)->text();
    delete((yyvsp[-2].node));
    std::string sval = "WITH RANGE_RESTRICTION(" + ss + ")";
    if((yyvsp[0].node)){
        sval += (yyvsp[0].node)->text();
        delete((yyvsp[0].node));
    }
    (yyval.node) = Node::makeTerminalNode(E_STRING, sval);
}
#line 11267 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 654:
#line 3373 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string ss1 = (yyvsp[-4].node)->text();
    delete((yyvsp[-4].node));
    std::string ss2 = (yyvsp[-2].node)->text();
    delete((yyvsp[-2].node));
    std::string sval = "WITH PARAMETERS(" + ss1 +"=" + ss2 + ")";
    if((yyvsp[0].node)){
        sval += (yyvsp[0].node)->text();
        delete((yyvsp[0].node));
    }
    (yyval.node) = Node::makeTerminalNode(E_STRING, sval);
}
#line 11284 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 655:
#line 3388 "sqlparser_hana.yacc" /* yacc.c:1646  */
    { (yyval.node) = nullptr; }
#line 11290 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 656:
#line 3390 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string sval = "WITH RANGE_RESTRICTION(" + (yyvsp[-1].node)->text() + ")";
    delete((yyvsp[-1].node));
    (yyval.node) = Node::makeTerminalNode(E_STRING, sval);
}
#line 11300 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 657:
#line 3399 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "WITH PRIMARY KEY");
}
#line 11308 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 658:
#line 3406 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_UPSERT, 6, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node), NULL,NULL);
    (yyval.node)->serialize_format = &UPSERT_SERIALIZE_FORMAT;
}
#line 11317 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 659:
#line 3411 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {

    (yyval.node) = Node::makeNonTerminalNode(E_UPSERT, 6, (yyvsp[-4].node), (yyvsp[-3].node), (yyvsp[-2].node), NULL, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &UPSERT_SERIALIZE_FORMAT;
}
#line 11327 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 660:
#line 3417 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_UPSERT, 6, (yyvsp[-1].node), NULL, (yyvsp[0].node), NULL, NULL,NULL);
    (yyval.node)->serialize_format = &UPSERT_SERIALIZE_FORMAT;
}
#line 11336 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 661:
#line 3424 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_REPLACE, 5, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node), NULL);
    (yyval.node)->serialize_format = &REPLACE_SERIALIZE_FORMAT;
}
#line 11345 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 662:
#line 3429 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    Node * pwith = Node::makeTerminalNode(E_STRING, " WITH PRIMARY KEY");
    (yyval.node) = Node::makeNonTerminalNode(E_REPLACE, 5, (yyvsp[-5].node), (yyvsp[-4].node), (yyvsp[-3].node), NULL, pwith);
    (yyval.node)->serialize_format = &REPLACE_SERIALIZE_FORMAT;
}
#line 11355 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 663:
#line 3435 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_REPLACE, 5, (yyvsp[-1].node), NULL, (yyvsp[0].node), NULL, NULL);
    (yyval.node)->serialize_format = &REPLACE_SERIALIZE_FORMAT;
}
#line 11364 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 667:
#line 3447 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "FOR SYSTEM_TIME AS OF "+(yyvsp[0].node)->Serialize());
    delete((yyvsp[0].node));
}
#line 11373 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 668:
#line 3454 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "FOR SYSTEM_TIME FROM "+(yyvsp[-2].node)->Serialize()+" TO "+(yyvsp[0].node)->Serialize());
    delete((yyvsp[-2].node));
    delete((yyvsp[0].node));
}
#line 11383 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 669:
#line 3462 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "FOR SYSTEM_TIME BETWEEN "+(yyvsp[-2].node)->Serialize()+" AND "+(yyvsp[0].node)->Serialize());
    delete((yyvsp[-2].node));
    delete((yyvsp[0].node));
}
#line 11393 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 670:
#line 3470 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeTerminalNode(E_STRING, "FOR APPLICATION_TIME AS OF "+(yyvsp[0].node)->Serialize());
    delete((yyvsp[0].node));
}
#line 11402 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 671:
#line 3477 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string val = "PARTITION("+(yyvsp[-1].node)->text()+")";
    delete((yyvsp[-1].node));
    (yyval.node) = Node::makeTerminalNode(E_STRING,val );
}
#line 11412 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 673:
#line 3486 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    (yyval.node) = Node::makeNonTerminalNode(E_SIMPLE_IDENT_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 11421 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 674:
#line 3493 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string val = "TABLESAMPLE BERNOULLI("+(yyvsp[-1].node)->text()+")";
    delete((yyvsp[-1].node));
    (yyval.node) = Node::makeTerminalNode(E_STRING,val );
}
#line 11431 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 675:
#line 3499 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string val = "TABLESAMPLE SYSTEM("+(yyvsp[-1].node)->text()+")";
    delete((yyvsp[-1].node));
    (yyval.node) = Node::makeTerminalNode(E_STRING,val );
}
#line 11441 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;

  case 676:
#line 3505 "sqlparser_hana.yacc" /* yacc.c:1646  */
    {
    std::string val = "TABLESAMPLE("+(yyvsp[-1].node)->text()+")";
    delete((yyvsp[-1].node));
    (yyval.node) = Node::makeTerminalNode(E_STRING,val );
}
#line 11451 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
    break;


#line 11455 "sqlparser_hana_bison.cpp" /* yacc.c:1646  */
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
  *++yylsp = yyloc;

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
      yyerror (&yylloc, result, scanner, YY_("syntax error"));
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
        yyerror (&yylloc, result, scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc, result, scanner);
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

  yyerror_range[1] = yylsp[1-yylen];
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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
  yyerror (&yylloc, result, scanner, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, result, scanner);
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
#line 3513 "sqlparser_hana.yacc" /* yacc.c:1906  */

/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */
