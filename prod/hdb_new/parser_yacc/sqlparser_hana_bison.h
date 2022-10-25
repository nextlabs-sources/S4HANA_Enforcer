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
#line 45 "sqlparser_hana.yacc" /* yacc.c:1909  */

// %code requires block

#include "node.h"

#line 58 "sqlparser_hana_bison.h" /* yacc.c:1909  */

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
#line 88 "sqlparser_hana.yacc" /* yacc.c:1909  */

    struct Node* node;
    int    ival;
    NodeType nodetype;

#line 336 "sqlparser_hana_bison.h" /* yacc.c:1909  */
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
