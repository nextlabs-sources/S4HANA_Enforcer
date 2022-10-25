/**
 * lexer
 *
 *
 */


/***************************
 ** Section 1: Definitions
 ***************************/
%{

#include "node.h"
#include "sqlparser_hana_bison.h"
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

std::string escape_double_quotation(const std::string& src, char c) {
    assert(c == '"' || c == '`' || c == ']');
    std::string ret = "";
    for (size_t i = 0; i < src.length(); ++i) {
        if (src[i] == c && src[1+i] == c) {
            ret += c;
            ++i;    // skip one
        }
        else
            ret += src[i];
    }
    return ret;
}

#define YY_USER_ACTION \
		yylloc->first_line = yylloc->last_line; \
		yylloc->first_column = yylloc->last_column; \
		for(int i = 0; yytext[i] != '\0'; i++) { \
				if(yytext[i] == '\n') { \
						yylloc->last_line++; \
						yylloc->last_column = 0; \
				} \
				else { \
						yylloc->last_column++; \
				} \
		}

#define TOKEN(name) { return HANA_##name; }
#define YYSTYPE         HANA_STYPE
#define YYLTYPE         HANA_LTYPE

%}


/***************************
 ** Section 2: Rules
 ***************************/

/* Define the output files */
%option header-file="sqlparser_hana_flex.h"
%option outfile="sqlparser_hana_flex.cpp"

/* Make reentrant */
%option reentrant
%option bison-bridge

/* performance tweeks */
%option never-interactive
%option batch

/* other flags */
%option noyywrap
%option nounput
%option warn
%option case-insensitive
%option prefix="hana_"
%option bison-locations
/* %option nodefault */


%x hint
%x sq
%x sb

space         [ \t\n\r\f]
non_newline   [^\n\r]
sql_comment   (("--"|"#"){non_newline}*)
whitespace    ({space}+|{sql_comment})
hint_begin    (\/\*\+({space}*hint{space}+)?)
c_cmt_begin   \/\*(?:[^\+])
c_cmt_end     \*+\/
c_cmt_cnt     ([^*]|\*+[^/*])*
c_comment     ({c_cmt_begin}{c_cmt_cnt}{c_cmt_end})
comment       ({c_comment}|{sql_comment})
identifer         ([A-Za-z_][A_Za-z0-9_]*)
system_variable  (@@[A-Za-z_][A_Za-z0-9_]*)
temp_variable (@[A-Za-z_][A_Za-z0-9_]*)

quote         '
nquote        "N'"
sqbegin       {quote}
sqend         {quote}
sqdouble      {quote}{quote}
sqcontent     [^']*
sqescape      [\\][^0-7]
sqoctal       [\\][0-7]{1,3}
sqnewline     {quote}{whitespace}{quote}


/***************************
 ** Section 3: Rules
 ***************************/
%%
ALL                 TOKEN(ALL)              /* SQL-2003-R */
AND                 TOKEN(AND)              /* SQL-2003-R */
ANY                 TOKEN(ANY)              /* SQL-2003-R */
ARRAY               TOKEN(ARRAY)            /* SQL-2003-R */
AS                  TOKEN(AS)               /* SQL-2003-R */
ASC                 TOKEN(ASC)              /* SQL-2003-N */
AVG                 TOKEN(AVG)              /* SQL-2003-N */
BETWEEN             TOKEN(BETWEEN)          /* SQL-2003-R */
BIGINT              TOKEN(BIGINT)           /* SQL-2003-R */
BINARY              TOKEN(BINARY)           /* SQL-2003-R */
BLOB                TOKEN(BLOB)             /* SQL-2003-R */
BOOLEAN             TOKEN(BOOLEAN)          /* SQL-2003-R */
BY                  TOKEN(BY)               /* SQL-2003-R */
CASE                TOKEN(CASE)             /* SQL-2003-R */
CAST                TOKEN(CAST)             /* SQL-2003-R */
CALL                TOKEN(CALL)             /* SQL-2003-R */
CHAR                TOKEN(CHAR)             /* SQL-2003-R */
CHARACTER           TOKEN(CHARACTER)        /* SQL-2003-R */
CHARACTERS          TOKEN(CHARACTERS)       /* SQL-2003-N */
CLOB                TOKEN(CLOB)             /* SQL-2003-R */
COALESCE            TOKEN(COALESCE)         /* SQL-2003-N */
CODE_UNITS          TOKEN(CODE_UNITS)       /* SQL-2003-N */
COLLATE             TOKEN(COLLATE)          /* SQL-2003-R */
CONVERT             TOKEN(CONVERT)          /* SQL-2003-N */
CORRESPONDING       TOKEN(CORRESPONDING)    /* SQL-2003-R */
COUNT               TOKEN(COUNT)            /* SQL-2003-N */
CROSS               TOKEN(CROSS)            /* SQL-2003-R */
CUME_DIST           TOKEN(CUME_DIST)        /* SQL-2003-N */
CURRENT             TOKEN(CURRENT)          /* SQL-2003-R */
CURRENT_TIMESTAMP   TOKEN(CURRENT_TIMESTAMP)/* SQL-2003-R */
CURRENT_USER        TOKEN(CURRENT_USER)     /* SQL-2003-R */
DATE                TOKEN(DATE)             /* SQL-2003-R */
DAY                 TOKEN(DAY)              /* SQL-2003-R */
DEC                 TOKEN(DEC)              /* SQL-2003-R */
DECIMAL             TOKEN(DECIMAL)          /* SQL-2003-R */
DEFAULT             TOKEN(DEFAULT)          /* SQL-2003-R */
DELETE              TOKEN(DELETE)           /* SQL-2003-R */
DENSE_RANK          TOKEN(DENSE_RANK)       /* SQL-2003-N */
DESC                TOKEN(DESC)             /* SQL-2003-N */
DISTINCT            TOKEN(DISTINCT)         /* SQL-2003-R */
DOUBLE              TOKEN(DOUBLE)           /* SQL-2003-R */
ELSE                TOKEN(ELSE)             /* SQL-2003-R */
END                 TOKEN(END)              /* SQL-2003-R */
END_P               TOKEN(END_P)            /* LEX-INNER  */
ESCAPE              TOKEN(ESCAPE)           /* SQL-2003-R */
EXCEPT              TOKEN(EXCEPT)           /* SQL-2003-R */
EXCLUDE             TOKEN(EXCLUDE)          /* SQL-2003-N */
EXISTS              TOKEN(EXISTS)           /* SQL-2003-R */
FLOAT               TOKEN(FLOAT)            /* SQL-2003-R */
FOLLOWING           TOKEN(FOLLOWING)        /* SQL-2003-N */
FOR                 TOKEN(FOR)              /* SQL-2003-R */
FROM                TOKEN(FROM)             /* SQL-2003-R */
FULL                TOKEN(FULL)             /* SQL-2003-R */
G                   TOKEN(G)                /* SQL-2003-N */
GROUP               TOKEN(GROUP)            /* SQL-2003-R */
GROUPING            TOKEN(GROUPING)         /* SQL-2003-R */
HAVING              TOKEN(HAVING)           /* SQL-2003-R */
HOUR                TOKEN(HOUR)             /* SQL-2003-R */
IN                  TOKEN(IN)               /* SQL-2003-R */
INNER               TOKEN(INNER)            /* SQL-2003-R */
INSERT              TOKEN(INSERT)           /* SQL-2003-R */
INT                 TOKEN(INT)              /* SQL-2003-R */
INTEGER             TOKEN(INTEGER)          /* SQL-2003-R */
INTERSECT           TOKEN(INTERSECT)        /* SQL-2003-R */
INTERVAL            TOKEN(INTERVAL)         /* SQL-2003-R */
INTO                TOKEN(INTO)             /* SQL-2003-R */
IS                  TOKEN(IS)               /* SQL-2003-R */
JOIN                TOKEN(JOIN)             /* SQL-2003-R */
K                   TOKEN(K)                /* SQL-2003-N */
LARGE               TOKEN(LARGE)            /* SQL-2003-R */
LEFT                TOKEN(LEFT)             /* SQL-2003-R */
LIKE                TOKEN(LIKE)             /* SQL-2003-R */
M                   TOKEN(M)                /* SQL-2003-N */
MAX                 TOKEN(MAX)              /* SQL-2003-N */
MIN                 TOKEN(MIN)              /* SQL-2003-N */
MINUTE              TOKEN(MINUTE)           /* SQL-2003-R */
MOD                 TOKEN(MOD)              /* SQL-2003-N, here is reserved as an operator */
MONTH               TOKEN(MONTH)            /* SQL-2003-R */
MULTISET            TOKEN(MULTISET)         /* SQL-2003-R */
NATIONAL            TOKEN(NATIONAL)         /* SQL-2003-R */
NATURAL             TOKEN(NATURAL)          /* SQL-2003-R */
NCHAR               TOKEN(NCHAR)            /* SQL-2003-R */
NCLOB               TOKEN(NCLOB)            /* SQL-2003-R */
NO                  TOKEN(NO)               /* SQL-2003-R */
NOT                 TOKEN(NOT)              /* SQL-2003-R */
NULLIF              TOKEN(NULLIF)           /* SQL-2003-N */
NUMERIC             TOKEN(NUMERIC)          /* SQL-2003-R */
OBJECT              TOKEN(OBJECT)           /* SQL-2003-N */
OCTETS              TOKEN(OCTETS)           /* SQL-2003-N */
OF                  TOKEN(OF)               /* SQL-2003-R */
ON                  TOKEN(ON)               /* SQL-2003-R */
ONLY                TOKEN(ONLY)             /* SQL-2003-R */
OR                  TOKEN(OR)               /* SQL-2003-R */
ORDER               TOKEN(ORDER)            /* SQL-2003-R */
OTHERS              TOKEN(OTHERS)           /* SQL-2003-N */
OUTER               TOKEN(OUTER)            /* SQL-2003-R */
OVER                TOKEN(OVER)             /* SQL-2003-R */
PARTITION           TOKEN(PARTITION)        /* SQL-2003-R */
PERCENT_RANK        TOKEN(PERCENT_RANK)     /* SQL-2003-N */
PRECEDING           TOKEN(PRECEDING)        /* SQL-2003-N */
PRECISION           TOKEN(PRECISION)        /* SQL-2003-R */
RANGE               TOKEN(RANGE)            /* SQL-2003-R */
RANK                TOKEN(RANK)             /* SQL-2003-N */
READ                TOKEN(READ)             /* SQL-2003-N */
REAL                TOKEN(REAL)             /* SQL-2003-R */
RECURSIVE           TOKEN(RECURSIVE)        /* SQL-2003-R */
REF                 TOKEN(REF)              /* SQL-2003-R */
RIGHT               TOKEN(RIGHT)            /* SQL-2003-R */
ROW                 TOKEN(ROW)              /* SQL-2003-R */
ROWS                TOKEN(ROWS)             /* SQL-2003-R */
ROW_NUMBER          TOKEN(ROW_NUMBER)       /* SQL-2003-N */
SCOPE               TOKEN(SCOPE)            /* SQL-2003-? */
SECOND              TOKEN(SECOND)           /* SQL-2003-R */
SELECT              TOKEN(SELECT)           /* SQL-2003-R */
SESSION_USER        TOKEN(SESSION_USER)     /* SQL-2003-R */
SET                 TOKEN(SET)              /* SQL-2003-R */
SETS                TOKEN(SETS)             /* SQL-2003-N */
SMALLINT            TOKEN(SMALLINT)         /* SQL-2003-R */
SOME                TOKEN(SOME)             /* SQL-2003-R */
STDDEV_POP          TOKEN(STDDEV_POP)       /* SQL-2003-N */
STDDEV_SAMP         TOKEN(STDDEV_SAMP)      /* SQL-2003-N */
SUM                 TOKEN(SUM)              /* SQL-2003-N */
SYSTEM_USER         TOKEN(SYSTEM_USER)      /* SQL-2003-R */
THEN                TOKEN(THEN)             /* SQL-2003-R */
TIES                TOKEN(TIES)             /* SQL-2003-N */
TIME                TOKEN(TIME)             /* SQL-2003-R */
TIMESTAMP           TOKEN(TIMESTAMP)        /* SQL-2003-R */
TO                  TOKEN(TO)               /* SQL-2003-R */
UNBOUNDED           TOKEN(UNBOUNDED)        /* SQL-2003-N */
UNION               TOKEN(UNION)            /* SQL-2003-R */
UPDATE              TOKEN(UPDATE)           /* SQL-2003-R */
USING               TOKEN(USING)            /* SQL-2003-R */
VALUES              TOKEN(VALUES)           /* SQL-2003-R */
VARCHAR             TOKEN(VARCHAR)          /* SQL-2003-R */
VARYING             TOKEN(VARYING)          /* SQL-2003-R */
VAR_POP             TOKEN(VAR_POP)          /* SQL-2003-R */
VAR_SAMP            TOKEN(VAR_SAMP)         /* SQL-2003-R */
WHEN                TOKEN(WHEN)             /* SQL-2003-R */
WHERE               TOKEN(WHERE)            /* SQL-2003-R */
WITH                TOKEN(WITH)             /* SQL-2003-R */
WITHOUT             TOKEN(WITHOUT)          /* SQL-2003-R */
YEAR                TOKEN(YEAR)             /* SQL-2003-R */
ZONE                TOKEN(ZONE)             /* SQL-2003-N */
LIMIT               TOKEN(LIMIT)             /* hana */
OFFSET              TOKEN(OFFSET)            /* hana */
TOP                 TOKEN(TOP)               /* hana */
HINT                TOKEN(HINT)              /* hana */
RANGE_RESTRICTION   TOKEN(RANGE_RESTRICTION) /* hana */
MANY                TOKEN(MANY)              /* hana --left outer many to one join */
ONE                 TOKEN(ONE)               /* hana */
NVARCHAR            TOKEN(NVARCHAR)          /* hana */
UPSERT              TOKEN(UPSERT)
REPLACE             TOKEN(REPLACE)
PRIMARY             TOKEN(PRIMARY)
KEY                 TOKEN(KEY)
PARAMETERS          TOKEN(PARAMETERS)       /* hana */
CONTAINS            TOKEN(CONTAINS)         /* hana */
SECONDDATE          TOKEN(SECONDDATE)       /* hana datatype */
TINYINT             TOKEN(TINYINT)          /* hana datatype */
SMALLDECIMAL        TOKEN(SMALLDECIMAL)     /* hana datatype */
TEXT                TOKEN(TEXT)             /* hana datatype */
BINTEXT             TOKEN(BINTEXT)          /* hana datatype */
ALPHANUM            TOKEN(ALPHANUM)         /* hana datatype */
VARBINARY           TOKEN(VARBINARY)        /* hana datatype */
SHORTTEXT           TOKEN(SHORTTEXT)        /* hana datatype */
ORDINALITY          TOKEN(ORDINALITY)       /* hana */
UNNEST              TOKEN(UNNEST)           /* hana */
HISTORY             TOKEN(HISTORY)          /* hana */
OVERRIDING          TOKEN(OVERRIDING)       /* hana */
USER                TOKEN(USER)             /* hana */
SYSTEM              TOKEN(SYSTEM)           /* hana */
VALUE               TOKEN(VALUE)            /* hana */
NULLS               TOKEN(NULLS)            /* hana */
FIRST               TOKEN(FIRST)            /* hana */
LAST                TOKEN(LAST)             /* hana */
CORR                TOKEN(CORR)             /* hana */
CORR_SPEARMAN       TOKEN(CORR_SPEARMAN)    /* hana */
MEDIAN              TOKEN(MEDIAN)           /* hana */
STDDEV              TOKEN(STDDEV)           /* hana */
VAR                 TOKEN(VAR)              /* hana */
STRING_AGG          TOKEN(STRING_AGG)       /* hana */
MEMBER              TOKEN(MEMBER)           /* hana */
SYSTEM_TIME         TOKEN(SYSTEM_TIME)      /* hana */
APPLICATION_TIME    TOKEN(APPLICATION_TIME) /* hana */
TABLESAMPLE         TOKEN(TABLESAMPLE)      /* hana */
BERNOULLI           TOKEN(BERNOULLI)        /* hana */
EXACT               TOKEN(EXACT)            /* hana */
RETURN              TOKEN(RETURN)           /* hana */
LATERAL             TOKEN(LATERAL)          /* hana */
BINNING             TOKEN(BINNING)          /* hana */
LAG                 TOKEN(LAG)              /* hana */
LEAD                TOKEN(LEAD)             /* hana */
RANDOM_PARTITION    TOKEN(RANDOM_PARTITION) /* hana */
WEIGHTED_AVG        TOKEN(WEIGHTED_AVG)     /* hana */
EXTRACT             TOKEN(EXTRACT)          /* hana */
FIRST_VALUE         TOKEN(FIRST_VALUE)      /* hana */
LAST_VALUE          TOKEN(LAST_VALUE)       /* hana */
NTH_VALUE           TOKEN(NTH_VALUE)        /* hana */
NTILE               TOKEN(NTILE)            /* hana */
LOCATE_REGEXPR      TOKEN(LOCATE_REGEXPR)   /* hana */
OCCURRENCES_REGEXPR TOKEN(OCCURRENCES_REGEXPR)          /* hana */
REPLACE_REGEXPR     TOKEN(REPLACE_REGEXPR)  /* hana */
SUBSTR_REGEXPR      TOKEN(SUBSTR_REGEXPR)   /* hana */
PERCENTILE_CONT     TOKEN(PERCENTILE_CONT)  /* hana */
PERCENTILE_DISC     TOKEN(PERCENTILE_DISC)  /* hana */
TRIM                TOKEN(TRIM)             /* hana */
WITHIN              TOKEN(WITHIN)           /* hana */
SERIES_FILTER       TOKEN(SERIES_FILTER)     /* hana *//* grouping sets option */
ROLLUP              TOKEN(ROLLUP)            /* hana *//* grouping sets option */
CUBE                TOKEN(CUBE)              /* hana *//* grouping sets option */
BEST                TOKEN(BEST)              /* hana *//* grouping sets option */
SUBTOTAL            TOKEN(SUBTOTAL)          /* hana *//* grouping sets option */
BALANCE             TOKEN(BALANCE)           /* hana *//* grouping sets option */
TOTAL               TOKEN(TOTAL)             /* hana *//* grouping sets option */
MULTIPLE            TOKEN(MULTIPLE)          /* hana *//* grouping sets option */
RESULTSETS          TOKEN(RESULTSETS)        /* hana *//* grouping sets option */
PREFIX              TOKEN(PREFIX)            /* hana *//* grouping sets option */
STRUCTURED          TOKEN(STRUCTURED)        /* hana *//* grouping sets option */
RESULT              TOKEN(RESULT)            /* hana *//* grouping sets option */
OVERVIEW            TOKEN(OVERVIEW)          /* hana *//* grouping sets option */
TEXT_FILTER         TOKEN(TEXT_FILTER)       /* hana *//* grouping sets option */
FILL                TOKEN(FILL)              /* hana *//* grouping sets option */
UP                  TOKEN(UP)                /* hana *//* grouping sets option */
MATCHES             TOKEN(MATCHES)           /* hana *//* grouping sets option */
SORT                TOKEN(SORT)              /* hana *//* grouping sets option */
MINUS               TOKEN(MINUS)             /* hana */
ROWCOUNT            TOKEN(ROWCOUNT)          /* hana */
LOCKED              TOKEN(LOCKED)            /* hana */
IGNORE              TOKEN(IGNORE)
SHARE               TOKEN(SHARE)
LOCK                TOKEN(LOCK)
UTCTIMESTAMP        TOKEN(UTCTIMESTAMP)
COMMIT              TOKEN(COMMIT)
ID                  TOKEN(ID)
WAIT                TOKEN(WAIT)
NOWAIT              TOKEN(NOWAIT)
JSON                TOKEN(JSON)
XML                 TOKEN(XML)
RETURNS             TOKEN(RETURNS)

NULL    {
    yylval->node = Node::makeTerminalNode(E_NULL, yytext);
    return HANA_NULLX;
}

("0X"|"0x")[0-9a-fA-F]+ {
    Node* nd = Node::makeTerminalNode(E_INT, yytext);
    yylval->node = nd;
    return HANA_INTNUM;
}

[0-9]+  {
    Node* nd = Node::makeTerminalNode(E_INT, yytext);
    yylval->node = nd;
    nd->terminalToken_.i = atoi(yytext);
    return HANA_INTNUM;
}


[0-9]+E[-+]?[0-9]+ |
[0-9]+"."[0-9]*E[-+]?[0-9]+ |
"."[0-9]+E[-+]?[0-9]+ {
    Node* nd = Node::makeTerminalNode(E_DOUBLE, yytext);
    yylval->node = nd;
    nd->terminalToken_.d = atof(yytext);
    return HANA_APPROXNUM;
}

[0-9]+"."[0-9]* |
"."[0-9]+ {
    Node* nd = Node::makeTerminalNode(E_DOUBLE, yytext);
    yylval->node = nd;
    nd->terminalToken_.d = atof(yytext);
    return HANA_APPROXNUM;
}

TRUE    {
    Node* nd = Node::makeTerminalNode(E_BOOL, yytext);
    yylval->node = nd;
    nd->terminalToken_.i = 1;
    return HANA_BOOL;
}

UNKNOWN {
    yylval->node = Node::makeTerminalNode(E_NULL, yytext);
    return HANA_UNKNOWN;
}

FALSE   {
    Node* nd = Node::makeTerminalNode(E_BOOL, yytext);
    yylval->node = nd;
    nd->terminalToken_.i = 0;
    return HANA_BOOL;
}

{sqbegin}   {
    BEGIN(sq);
    ((ParseResult*)yyextra)->buf_ = std::stringstream{};
    ((ParseResult*)yyextra)->buf_ << '\'';
}

{nquote}    {
    BEGIN(sq);
    ((ParseResult*)yyextra)->buf_ = std::stringstream{};
    ((ParseResult*)yyextra)->buf_ << "N'";
}

<sq>{sqdouble} {
    ((ParseResult*)yyextra)->buf_ << "''";
}

<sq>{sqcontent} {
    ((ParseResult*)yyextra)->buf_ << yytext;
}

<sq>{sqend} {
    BEGIN(INITIAL);
    ((ParseResult*)yyextra)->buf_ << '\'';
    Node* nd = Node::makeTerminalNode(E_STRING, ((ParseResult*)yyextra)->buf_.str().c_str());
    nd->terminalToken_.str = ((ParseResult*)yyextra)->buf_.str();
    yylval->node = nd;
    return HANA_STRING;
}

<sq><<EOF>>  {
  fprintf(stderr, "[SQL-Lexer-Error] Unterminated string\n");
  return HANA_ERROR;
}


\"(\"\"|[^"\\\n])+\" {
    Node* nd = Node::makeTerminalNode(E_IDENTIFIER, yytext);
    yylval->node = nd;
    nd->terminalToken_.str = escape_double_quotation(std::string(yytext+1, yytext + strlen(yytext) - 1), '"');
    nd->terminalToken_.line = yylloc->first_line;
    nd->terminalToken_.column = yylloc->first_column;
    return HANA_NAME;
}


[A-Za-z_][A-Za-z0-9_]* {
	Node* nd = Node::makeTerminalNode(E_IDENTIFIER, yytext);
	yylval->node = nd;
	nd->terminalToken_.str = yytext;
	nd->terminalToken_.line = yylloc->first_line;
    nd->terminalToken_.column = yylloc->first_column;
	return HANA_NAME;
}


{comment}   { /* ignore */ }
[ \t\r\n]+  { /* ignore */ }

[-+&~|^/%*(){}\[\]:,.;!$] { return yytext[0];}


"||"    { return HANA_CNNOP; }
"="     { return HANA_COMP_EQ; }
">="    { return HANA_COMP_GE; }
">"     { return HANA_COMP_GT; }
"<="    { return HANA_COMP_LE; }
"<"     { return HANA_COMP_LT; }
"<>"    { return HANA_COMP_NE; }
"!="    { return HANA_COMP_NE; }
"=>"    { return HANA_P_POINT; }

""

"?"     {
    Node* nd = Node::makeTerminalNode(E_QUESTIONMARK, yytext);
    g_QuestMarkId++;
    nd->questmarkid_ = g_QuestMarkId;
    yylval->node = nd;
    return HANA_QUESTIONMARK;
}


<<EOF>>    { return HANA_END_P; }

.   {
    fprintf(stderr, "[SQL-Lexer-Error] Unknown Character: %c\n", yytext[0]);
    return HANA_ERROR;   /* todo ignore or return ERROR */
}
%%
/***************************
 ** Section 3: User code
 ***************************/

int yyerror(const char *msg) {
    fprintf(stderr, "[SQL-Lexer-Error] %s\n",msg); return HANA_ERROR;
}
