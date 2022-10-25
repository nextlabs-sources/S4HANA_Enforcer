%{
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

%}
/*********************************
 ** Section 2: Bison Parser Declarations
 *********************************/


// Specify code that is included in the generated .h and .c files
%code requires {
// %code requires block

#include "node.h"
}

// Auto update column and line number

// Define the names of the created files (defined in Makefile)
%output  "sqlparser_hana_bison.cpp"
%defines "sqlparser_hana_bison.h"

// Tell bison to create a reentrant parser
%define api.pure full

// Prefix the parser
%define api.prefix {hana_}
%define api.token.prefix {HANA_}

%define parse.error verbose
%locations

%initial-action {
	// Initialize
	@$.first_column = 0;
	@$.last_column = 0;
	@$.first_line = 0;
	@$.last_line = 0;
};


// Define additional parameters for yylex (http://www.gnu.org/software/bison/manual/html_node/Pure-Calling.html)
%lex-param   { yyscan_t scanner }

// Define additional parameters for yyparse
%parse-param { ParseResult* result }
%parse-param { yyscan_t scanner }


/*********************************
 ** Define all data-types (http://www.gnu.org/software/bison/manual/html_node/Union-Decl.html)
 *********************************/
%union
{
    struct Node* node;
    int    ival;
    NodeType nodetype;
}


/*********************************
 ** Destructor symbols
 *********************************/
%destructor { delete($$); }<node>


/*********************************
 ** Token Definition
 *********************************/
%token <node> NAME
%token <node> STRING
%token <node> INTNUM
%token <node> BOOL
%token <node> APPROXNUM
%token <node> NULLX
%token <node> UNKNOWN
%token <node> QUESTIONMARK


//%right 	COLLATE
//%left	OR
//%left	AND
//%right 	NOT
//%left 	COMP_LE COMP_LT COMP_EQ COMP_GT COMP_GE COMP_NE
//%left 	CNNOP
//%left 	LIKE
//%nonassoc BETWEEN
//%nonassoc IN
//%nonassoc IS NULLX BOOL UNKNOWN
//%left 	'+' '-'
//%left 	'*' '/' '%' MOD
//%left 	'^'
%right 	UMINUS
%left 	'(' ')'
%left '.'


%token ALL AND ANY ARRAY AS ASC
       AVG
%token BETWEEN BIGINT BINARY BLOB BOOLEAN BY
%token CALL CASE CAST CHAR CHARACTER CHARACTERS CLOB
       CNNOP COALESCE CODE_UNITS COLLATE COMP_EQ
       COMP_GE COMP_GT COMP_LE COMP_LT COMP_NE
       CONVERT CORRESPONDING COUNT CROSS CUME_DIST
       CURRENT CURRENT_TIMESTAMP CURRENT_USER
%token DATE DAY DEC DECIMAL DEFAULT DELETE
       DENSE_RANK DESC DISTINCT DOUBLE
%token ELSE END END_P ESCAPE ERROR EXCEPT EXCLUDE
%token EXISTS
%token FLOAT FOLLOWING FOR FROM FULL
%token G GROUP GROUPING
%token HAVING HOUR
%token IN INNER INSERT INT INTEGER INTERSECT INTERVAL MANY ONE EXACT
       INTO IS
%token JOIN
%token K
%token LARGE LEFT LIKE
%token M MAX MIN MINUTE MOD MONTH
       MULTISET
%token NATIONAL NATURAL NCHAR NCLOB NO NOT NULLIF
       NUMERIC
%token OBJECT OCTETS OF ON ONLY OR
       ORDER OTHERS OUTER OVER
%token PARTITION PERCENT_RANK PRECEDING PRECISION
%token RANGE RANK READ REAL RECURSIVE REF
       RIGHT ROW ROWS ROW_NUMBER
%token SCOPE SECOND SELECT SESSION_USER SET SETS SMALLINT
       SOME STDDEV_POP STDDEV_SAMP SUM SYSTEM_USER
%token THEN TIES TIME TIMESTAMP TO
%token UNBOUNDED UNION UPDATE USING
%token VALUES VARCHAR NVARCHAR VARYING VAR_POP VAR_SAMP
%token WHEN WHERE WITH WITHOUT
%token YEAR
%token ZONE
%token LIMIT OFFSET TOP WAIT NOWAIT HINT
%token RANGE_RESTRICTION PARAMETERS
%token UPSERT REPLACE PRIMARY  KEY
%token CONTAINS
%token P_POINT
%token SECONDDATE TINYINT SMALLDECIMAL  TEXT  BINTEXT  ALPHANUM  VARBINARY  SHORTTEXT
%token ORDINALITY UNNEST HISTORY OVERRIDING USER SYSTEM VALUE
%token NULLS FIRST LAST CORR CORR_SPEARMAN MEDIAN STDDEV VAR STRING_AGG MEMBER SYSTEM_TIME APPLICATION_TIME
%token TABLESAMPLE BERNOULLI RETURN LATERAL
%token BINNING LAG LEAD RANDOM_PARTITION WEIGHTED_AVG EXTRACT FIRST_VALUE  LAST_VALUE  NTH_VALUE  NTILE SERIES_FILTER
    LOCATE_REGEXPR  OCCURRENCES_REGEXPR  REPLACE_REGEXPR  SUBSTR_REGEXPR  PERCENTILE_CONT  PERCENTILE_DISC  TRIM  WITHIN
%token ROLLUP CUBE BEST SUBTOTAL BALANCE TOTAL MULTIPLE RESULTSETS PREFIX STRUCTURED RESULT OVERVIEW TEXT_FILTER FILL UP  MATCHES SORT
%token MINUS ROWCOUNT   LOCKED IGNORE SHARE LOCK UTCTIMESTAMP COMMIT ID
%token XML JSON RETURNS

%type <node> sql_stmt
%type <node> stmt_list
%type <node> stmt
%type <node> dql_stmt
%type <node> dml_stmt
%type <node> call_stmt sp_name sql_argument_list sql_argument value_expression
%type <node> select_stmt
%type <node> update_stmt upsert_stmt    replace_stmt
%type <node> delete_stmt
%type <node> insert_stmt
%type <node> insert_columns_and_source from_constructor
%type <node> opt_from_clause table_primary_non_join data_type
%type <node> label opt_as_label as_label
%type <node> select_with_parens query_expression query_expression_body
%type <node> query_term query_primary simple_table
%type <node> select_expr_list
%type <node> opt_where opt_groupby opt_order_by order_by opt_having
%type <node> sort_list sort_key opt_asc_desc
%type <node> opt_distinct  projection
%type <node> from_list table_reference table_primary relation_factor
%type <node> joined_table cross_join qualified_join
%type <node> join_type join_cardinality
%type <node> case_join case_join_when_list case_join_when ret_join_on select_expr_list_parens opt_case_join_else
%type <ival> join_outer
%type <node> expr_const
%type <node> search_condition boolean_term boolean_factor boolean_test boolean_primary truth_value
%type <node> predicate comparison_predicate between_predicate in_predicate like_predicate member_of_predicate
%type <node> null_predicate  exists_predicate
%type <node> row_expr row_expr_list factor0 factor1 factor2 factor3 factor4
%type <nodetype> comp_op plus_minus_op star_div_percent_mod_op comp_all_some_any_op cnn_op
%type <node> column_ref
%type <node> case_expr func_expr in_expr function_call_keyword
%type <node> case_arg when_clause_list when_clause case_default
%type <node> with_clause with_list common_table_expr opt_derived_column_list
%type <node> simple_ident_list simple_ident_list_with_parens opt_simple_ident_list_with_parens column_ref_list
%type <node> over_clause window_name window_specification window_specification_details
%type <node> opt_existing_window_name opt_window_partition_clause
%type <node> opt_window_frame_clause window_frame_units window_frame_extent opt_window_frame_exclusion
%type <node> window_frame_start window_frame_between window_frame_preceding window_frame_bound
%type <node> window_frame_following
%type <node> ranking_windowed_function scalar_function bool_function
%type <node> ranking_function_name sap_specific_function
%type <node> update_elem_list update_elem
%type <node> collate_clause
%type <ival> all_some_any
%type <node> predefined_type
%type <node> opt_with_range_restrict with_primary_key
%type <node> primary_datetime_field
%type <node> name_r reserved
%type <node> grouping_element_list grouping_element
%type <node> subquery row_subquery table_subquery sap_table_subquery
%type <node> limit_num offset_num
%type <node> top_clause
%type <node> opt_for_update for_update wait_nowait hint_clause with_hint_param_list with_hint_param
%type <node> hana_construct_table construct_column_list construct_column
%type <node> table_function_param_list table_function_param expr_point_val
%type <node> lob_data_type array_type
%type <node> collection_derived_table collection_value_expr_list collection_value_expr as_derived_part
%type <node> opt_simple_from_clause  simple_from_list update_table insert_select_stmt
%type <node> aggregate_name aggregate_expression
%type <node> func_relation_factor name_f
%type <node> opt_table_qualify for_system_time sys_as_of_spec sys_from_to_spec sys_between_spec partition_restriction intnum_list for_application_time
%type <node> opt_tablesample tablesample_num lateral_table
%type <node> associated_table associated_ref_list  associated_ref opt_many2one_part opt_search_condition
%type <node> opt_order_by_clause  order_by_clause order_by_expression_list order_by_expression opt_collate_clause within_group opt_nulls regexpr_func_name regexpr_in_val
%type <node> group_set_name grouping_option grouping_best grouping_subtotal  grouping_prefix_tb grouping_structured_res grouping_prefix grouping_text_filter row_order_by_list row_order_by column_ref_perens
%type <node> limit_total opt_of_ident_list opt_ignore_lock for_share_lock
%type <node> for_xml  for_json opt_returns_clause opt_option_string_list_p  option_string_list  option_string

//%TYPE <node> opt_agg_orderby
//%type <node> approximate_numeric_type binary_large_object_string_type boolean_type character_string_type collection_type datetime_type
//%type <node> exact_numeric_type national_character_string_type reference_type single_datetime_field user_defined_type_name collection_type
//%type <node> interval_type interval_qualifier start_field end_field numeric_type
//%type <node> large_object_length multiplier char_length_units

%start sql_stmt
%%
/*********************************
 ** Section 3: Grammar Definition
 *********************************/

// Defines our general input.
sql_stmt:
    stmt_list END_P
{
    g_QuestMarkId = 0;
    $$ = $1;
    result->result_tree_ = $1;
    result->accept = true;
    YYACCEPT;
}
;

stmt_list:
    stmt
  | stmt ';' stmt_list
{
    $$ = Node::makeNonTerminalNode(E_STMT_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &SEMICOLON_LIST_SERIALIZE_FORMAT;
}
;

stmt:
    /*EMPTY*/	{ $$ = nullptr; } /*EMPTY STATEMENT*/
  | dql_stmt
  | dml_stmt
  | call_stmt
;

call_stmt:
    CALL sp_name '(' sql_argument_list ')' hint_clause
{
    $$ = Node::makeNonTerminalNode(E_CALL, 3, $2, $4, $6);
    $$->serialize_format = &CALL_SERIALIZE_FORMAT;
}
    | CALL sp_name '(' ')' hint_clause
{
    $$ = Node::makeNonTerminalNode(E_CALL, 3, $2, nullptr, $5);
    $$->serialize_format = &CALL_SERIALIZE_FORMAT;
}
    | CALL sp_name hint_clause
{
    $$ = Node::makeNonTerminalNode(E_CALL, 3, $2, nullptr, $3);
    $$->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
;

sql_argument_list:
    sql_argument
  | sql_argument_list ',' sql_argument
{
    $$ = Node::makeNonTerminalNode(E_STMT_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;

sql_argument:
    value_expression
{
    $$ = Node::makeNonTerminalNode(E_SQL_ARGUMENT, E_SQL_ARGUMENT_PROPERTY_CNT, $1, nullptr);
    $$->serialize_format = &SQL_ARGUMENT_SERIALIZE_FORMAT;
}
  | value_expression AS data_type
{
    $$ = Node::makeNonTerminalNode(E_SQL_ARGUMENT, E_SQL_ARGUMENT_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &SQL_ARGUMENT_SERIALIZE_FORMAT;
}
;

value_expression:
    search_condition	/* <boolean value expression> */
  | row_expr		/* <common value expression> <row value expression> */
;

sp_name:
    relation_factor
;

dql_stmt:
    select_stmt
;

dml_stmt:
    update_stmt
  | delete_stmt
  | insert_stmt
  | upsert_stmt
  | replace_stmt
;

insert_stmt:
    INSERT INTO relation_factor insert_columns_and_source
{
    $$ = Node::makeNonTerminalNode(E_INSERT, E_INSERT_PROPERTY_CNT, $3, $4);
    $$->serialize_format = &INSERT_SERIALIZE_FORMAT;
}
;

insert_columns_and_source:
    				  insert_select_stmt
{
    $$ = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, 3, NULL, $1,NULL);
    $$->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
}
  | simple_ident_list_with_parens insert_select_stmt
{
    $$ = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, 3, $1, $2,NULL);
    $$->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
}
  | simple_ident_list_with_parens from_constructor opt_with_range_restrict
{
    $$ = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, 3, $1, $2, $3);
    $$->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
}
  |                               from_constructor opt_with_range_restrict
{
    $$ = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, 3, nullptr, $1, $2);
    $$->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
}
;

insert_select_stmt:
    select_stmt
    | OVERRIDING SYSTEM VALUE select_stmt
{
    Node * over = Node::makeTerminalNode(E_STRING,"OVERRIDING SYSTEM VALUE");
    $$ = Node::makeNonTerminalNode(E_INSERT_SELECT, 2, over, $4);
    $$->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
    | OVERRIDING USER VALUE select_stmt
{
    Node * over = Node::makeTerminalNode(E_STRING,"OVERRIDING USER VALUE");
    $$ = Node::makeNonTerminalNode(E_INSERT_SELECT, 2, over, $4);
    $$->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
;

from_constructor:
    VALUES row_expr_list
{
    $$ = Node::makeNonTerminalNode(E_VALUES_CTOR, E_VALUES_CTOR_PROPERTY_CNT, $2);
    $$->serialize_format = &TABLE_VALUE_CTOR_SERIALIZE_FORMAT;
}
;

/* hana support <delete statement: searched> */
delete_stmt:
    DELETE FROM table_primary_non_join opt_where hint_clause
{
    $$ = Node::makeNonTerminalNode(E_DELETE, E_DELETE_PROPERTY_CNT,
    		nullptr,	/* E_DELETE_OPT_WITH 0 */
    		nullptr,	/* E_DELETE_OPT_TOP 1 */
    		$3,		/* E_DELETE_DELETE_RELATION 2 */
    		nullptr,	/* E_DELETE_DELETE_RELATION_OPT_TABLE_HINT 3 */
    		nullptr,        /* E_DELETE_OPT_OUTPUT 4 */
    		nullptr,	/* E_DELETE_FROM_LIST 5 */
    		$4,		/* E_DELETE_OPT_WHERE 6 */
    		$5 	/* E_DELETE_OPT_QUERY_HINT 7 */);
    $$->serialize_format = &DELETE_SERIALIZE_FORMAT;
}
    | DELETE HISTORY FROM table_primary_non_join opt_where hint_clause
{
    $$ = Node::makeNonTerminalNode(E_DELETE, E_DELETE_PROPERTY_CNT,
        nullptr,	/* E_DELETE_OPT_WITH 0 */
        nullptr,	/* E_DELETE_OPT_TOP 1 */
        $4,		/* E_DELETE_DELETE_RELATION 2 */
        nullptr,	/* E_DELETE_DELETE_RELATION_OPT_TABLE_HINT 3 */
        nullptr,        /* E_DELETE_OPT_OUTPUT 4 */
        nullptr,	/* E_DELETE_FROM_LIST 5 */
        $5,		/* E_DELETE_OPT_WHERE 6 */
        $6 	/* E_DELETE_OPT_QUERY_HINT 7 */);
    $$->serialize_format = &DELETE_S_FORMAT;
}
;

/* hana support <update statement: searched> */
update_stmt:
    UPDATE update_table  SET update_elem_list opt_simple_from_clause opt_where hint_clause
{
    $$ = Node::makeNonTerminalNode(E_UPDATE, E_UPDATE_PROPERTY_CNT,
    		nullptr,	/* E_UPDATE_OPT_WITH 0 */
    		nullptr,	/* E_UPDATE_OPT_TOP 1 */
    		$2,		    /* E_UPDATE_UPDATE_RELATION 2 */
    		nullptr,	/* E_UPDATE_UPDATE_RELATION_OPT_TABLE_HINT 3 */
    		$4,		    /* E_UPDATE_UPDATE_ELEM_LIST 4 */
    		nullptr,	/* E_UPDATE_OPT_OUTPUT 5 */
    		$5,	        /* E_UPDATE_FROM_LIST 6 */
    		$6,		    /* E_UPDATE_OPT_WHERE 7 */
    		$7		    /* E_UPDATE_OPT_QUERY_HINT 8 */);
    $$->serialize_format = &UPDATE_SERIALIZE_FORMAT;
}
;
update_table:
    relation_factor opt_as_label
{
    if($2){
        $$ = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, $1, $2, nullptr, nullptr, nullptr);
        $$->serialize_format = &AS_SERIALIZE_FORMAT;
    } else {
        $$ = $1;
    }
}
    | relation_factor partition_restriction opt_as_label
{
    $$ = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, $1, $3, nullptr, nullptr, $2);
    $$->serialize_format = &AS_SERIALIZE_FORMAT;
}
;

opt_simple_from_clause:
    { $$ = NULL; }
    | FROM simple_from_list
{
    $$ = Node::makeNonTerminalNode(E_FROM_CLAUSE, E_FROM_CLAUSE_PROPERTY_CNT, $2);
    $$->serialize_format = &FROM_SERIALIZE_FORMAT;
}
;

simple_from_list:
    table_primary_non_join
  | simple_from_list ',' table_primary_non_join
{
    $$ = Node::makeNonTerminalNode(E_FROM_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;

update_elem_list:
    update_elem
  | update_elem_list ',' update_elem
{
    $$ = Node::makeNonTerminalNode(E_UPDATE_ELEM_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;

update_elem:
    row_expr COMP_EQ row_expr
{
    $$ = Node::makeNonTerminalNode(E_OP_EQ, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &OP_EQ_SERIALIZE_FORMAT;
}
;

/* hana support <direct select statement: multiple rows> */
select_stmt:
    query_expression
{
    $$ = $1;
    // 0 , 1  2 already
    // $$->setChild(E_DIRECT_SELECT_FOR_UPDATE, $2);
    // $$->setChild(E_DIRECT_HINT_CLAUSE, $3);
}
;


query_expression:
    query_expression_body opt_for_update hint_clause
{
    $$ = Node::makeNonTerminalNode(E_DIRECT_SELECT, E_DIRECT_SELECT_PROPERTY_CNT, nullptr, $1, nullptr, $2, $3);
    $$->serialize_format = &SELECT_DIRECT_SERIALIZE_FORMAT;
}
  | with_clause query_expression_body opt_for_update hint_clause
{
    $$ = Node::makeNonTerminalNode(E_DIRECT_SELECT, E_DIRECT_SELECT_PROPERTY_CNT, $1, $2, nullptr,$3, $4);
    $$->serialize_format = &SELECT_DIRECT_SERIALIZE_FORMAT;
}
;
query_expression_body:
    query_term
  | query_expression_body UNION opt_distinct query_term
{
    Node* set_op = Node::makeTerminalNode(E_SET_UNION, "UNION");
    $$ = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        nullptr,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                        nullptr,             /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        set_op,              /* E_SELECT_SET_OPERATION 6 */
                        $3,                  /* E_SELECT_ALL_SPECIFIED 7 */
                        $1,                  /* E_SELECT_FORMER_SELECT_STMT 8 */
                        $4,                  /* E_SELECT_LATER_SELECT_STMT 9 */
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
    $$->serialize_format = &SELECT_UNION_SERIALIZE_FORMAT;
}
  | query_expression_body EXCEPT opt_distinct query_term
{
    Node* set_op = Node::makeTerminalNode(E_SET_EXCEPT, "EXCEPT");
    $$ = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        nullptr,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                        nullptr,             /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        set_op,              /* E_SELECT_SET_OPERATION 6 */
                        $3,                  /* E_SELECT_ALL_SPECIFIED 7 */
                        $1,                  /* E_SELECT_FORMER_SELECT_STMT 8 */
                        $4,                  /* E_SELECT_LATER_SELECT_STMT 9 */
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
    $$->serialize_format = &SELECT_EXCEPT_SERIALIZE_FORMAT;
}
  | query_expression_body MINUS opt_distinct query_term
{
    Node* set_op = Node::makeTerminalNode(E_SET_EXCEPT, "MINUS");
    $$ = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        nullptr,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                        nullptr,             /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        set_op,              /* E_SELECT_SET_OPERATION 6 */
                        $3,                  /* E_SELECT_ALL_SPECIFIED 7 */
                        $1,                  /* E_SELECT_FORMER_SELECT_STMT 8 */
                        $4,                  /* E_SELECT_LATER_SELECT_STMT 9 */
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
    $$->serialize_format = &SELECT_EXCEPT_SERIALIZE_FORMAT;
}
;

query_term:
    query_primary
  | query_term INTERSECT opt_distinct query_primary
{
    Node* set_op = Node::makeTerminalNode(E_SET_INTERSECT, "INTERSECT");
    $$ = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        nullptr,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                        nullptr,             /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        set_op,              /* E_SELECT_SET_OPERATION 6 */
                        $3,                  /* E_SELECT_ALL_SPECIFIED 7 */
                        $1,                  /* E_SELECT_FORMER_SELECT_STMT 8 */
                        $4,                  /* E_SELECT_LATER_SELECT_STMT 9 */
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
    $$->serialize_format = &SELECT_INTERSECT_SERIALIZE_FORMAT;
}
;

query_primary:
    simple_table
  | '(' query_expression ')'	/* in hana spec, here should be <query expression body> here generate a reduce/reduce error, however it works (all test passed) */
{
   // if ($2->getChild(E_DIRECT_SELECT_WITH)) {
    //    yyerror(&@1, result, scanner, "error use common table expression");
   //     YYABORT;
   // }
    //$$ = Node::makeNonTerminalNode(E_SELECT_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2->getChild(E_DIRECT_SELECT_SELECT_CLAUSE));
    //$$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
    //$2->setChild(E_DIRECT_SELECT_SELECT_CLAUSE, nullptr);
    //delete($2);

    $$ = Node::makeNonTerminalNode(E_SELECT_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2);
    $$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
;

/* here generate a reduce/reduce error, however it works (all test passed) */
select_with_parens:
    '(' query_expression ')'
{
    $$ = Node::makeNonTerminalNode(E_SELECT_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2);
    $$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
;

subquery: select_with_parens;

sap_table_subquery:
    table_subquery

;
table_subquery: subquery;

row_subquery: subquery;

simple_table:
    SELECT top_clause opt_distinct select_expr_list opt_from_clause opt_where opt_groupby opt_having opt_order_by_clause limit_num
{
    $$ = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                    $3,             /* E_SELECT_DISTINCT 0 */
                    $4,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                    $5,             /* E_SELECT_FROM_LIST 2 */
                    $6,             /* E_SELECT_OPT_WHERE 3 */
                    $7,             /* E_SELECT_GROUP_BY 4 */
                    $8,             /* E_SELECT_HAVING 5 */
                    nullptr,        /* E_SELECT_SET_OPERATION 6 */
                    nullptr,        /* E_SELECT_ALL_SPECIFIED 7 */
                    nullptr,        /* E_SELECT_FORMER_SELECT_STMT 8 */
                    nullptr,        /* E_SELECT_LATER_SELECT_STMT 9 */
                    $9,             /* E_SELECT_ORDER_BY 10 */
                    $10,            /* E_SELECT_LIMIT 11 */
                    nullptr,        /* E_SELECT_FOR_UPDATE 12 */
                    nullptr,        /* E_SELECT_HINTS 13 */
                    nullptr,        /* E_SELECT_WHEN 14 */
                    $2,             /* E_SELECT_OPT_TOP 15 */
                    nullptr,        /* E_SELECT_OPT_WITH 16 */
                    nullptr,        /* E_SELECT_OPT_OPTION 17 */
	                nullptr         /* E_SELECT_OPT_INTO 18 */
                    );
    $$->serialize_format = &SELECT_SERIALIZE_FORMAT;
}
;

opt_where:
    /*EMPTY*/	{ $$ = nullptr; }
  | WHERE search_condition
{
    $$ = Node::makeNonTerminalNode(E_WHERE_CLAUSE, E_WHERE_CLAUSE_PROPERTY_CNT, $2);
    $$->serialize_format = &WHERE_SERIALIZE_FORMAT;
}
;

/* hana not allow tableless */
opt_from_clause:
    FROM from_list
{
    $$ = Node::makeNonTerminalNode(E_FROM_CLAUSE, E_FROM_CLAUSE_PROPERTY_CNT, $2);
    $$->serialize_format = &FROM_SERIALIZE_FORMAT;
}
;

opt_groupby:
    /*EMPTY*/	{ $$ = nullptr; }
  | GROUP BY opt_distinct grouping_element_list
{
    $$ = Node::makeNonTerminalNode(E_GROUP_BY, E_GROUP_BY_PROPERTY_CNT, $4, $3);
    $$->serialize_format = &GROUP_BY_SERIALIZE_FORMAT;
}
;

grouping_element_list:
    grouping_element
  | grouping_element_list ',' grouping_element
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;

grouping_element:
    '(' ')'	{ $$ = Node::makeTerminalNode(E_IDENTIFIER,"()"); }
    | row_expr
    | group_set_name grouping_option '(' row_order_by_list ')'
{
    std::string name = $1->Serialize();
    delete($1);
    if($2){
        name += $2->Serialize();
        delete($2);
    }
    Node * name_n = Node::makeTerminalNode(E_IDENTIFIER, name);
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT, name_n, nullptr, nullptr, $4, nullptr);
    $$->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
;
row_order_by_list:
    row_order_by
    | row_order_by_list ',' row_order_by
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;
row_order_by:
    '(' column_ref_perens opt_order_by ')'
{
    std::string ss = $2->Serialize();
    delete($2);
    if($3){
        ss += $3->Serialize();
        delete($3);
    }
    Node * node_s  = Node::makeTerminalNode(E_STRING, ss );
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, node_s);
    $$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
    |    column_ref_perens
    |    column_ref
;
column_ref_perens:
    '(' row_expr_list ')'
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2);
    $$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
;

group_set_name:
    GROUPING SETS { $$ = Node::makeTerminalNode(E_IDENTIFIER, "GROUPING SETS ");}
    | ROLLUP { $$ = Node::makeTerminalNode(E_IDENTIFIER, "ROLLUP"); }
    | CUBE { $$  = Node::makeTerminalNode(E_IDENTIFIER, "CUBE"); }
;
grouping_option:
      grouping_best limit_num grouping_subtotal grouping_text_filter grouping_prefix_tb
{
      std::string val;
      if($1){ val += $1->Serialize(); delete($1); }
      if($2){ val += $2->Serialize(); delete($2); }
      if($3){ val += $3->Serialize(); delete($3); }
      if($4){ val += $4->Serialize(); delete($4); }
      if($5){ val += $5->Serialize(); delete($5); }
      if(!val.empty()){
        $$ = Node::makeTerminalNode(E_STRING, val);
      } else { $$ = nullptr; }
}
;

grouping_best:
    /* empty */ { $$ = nullptr; }
    | BEST INTNUM {    $$ = Node::makeTerminalNode(E_STRING, "BEST "+ $2->Serialize()); delete($2); }
;
grouping_subtotal:
    /* empty */ { $$ = nullptr; }
    |  WITH SUBTOTAL {     $$ = Node::makeTerminalNode(E_STRING, "WITH SUBTOTAL " ); }
    |  WITH BALANCE {  $$ = Node::makeTerminalNode(E_STRING, "WITH BALANCE " ); }
    |  WITH TOTAL {     $$ = Node::makeTerminalNode(E_STRING, "WITH TOTAL " ); }
;
grouping_prefix_tb:
     grouping_structured_res grouping_prefix
{
     std::string pref_s ;
     if($1){ pref_s += $1->Serialize(); delete($1); }
     if($2){ pref_s += $2->Serialize(); delete($2); }
     if(!pref_s.empty()){
         $$ = Node::makeTerminalNode( E_STRING, pref_s );
     } else { $$ = nullptr;  }
}
    | MULTIPLE RESULTSETS {    $$ = Node::makeTerminalNode(E_STRING, " MULTIPLE RESULTSETS " ); }
;
grouping_prefix:
    /* empty */ { $$ = nullptr; }
    | PREFIX STRING {     $$ = Node::makeTerminalNode(E_STRING, "PREFIX "+ $2->Serialize() ); delete($2); }
;
grouping_structured_res:
    /* empty */ { $$ = nullptr; }
    | STRUCTURED RESULT {    $$ = Node::makeTerminalNode(E_STRING, "BEST INTNUM " ); }
    | STRUCTURED RESULT WITH OVERVIEW {    $$ = Node::makeTerminalNode(E_STRING, "STRUCTURED RESULT WITH OVERVIEW " ); }
;
grouping_text_filter:
    /* empty */ { $$ = nullptr; }
    | TEXT_FILTER STRING  {  $$ = Node::makeTerminalNode(E_STRING, "TEXT_FILTER " + $2->Serialize() + " " ); delete($2); }
    | TEXT_FILTER STRING FILL UP {  $$ = Node::makeTerminalNode(E_STRING, "TEXT_FILTER " + $2->Serialize() +  " FILL UP " );  delete($2); }
    | TEXT_FILTER STRING FILL UP SORT MATCHES TO TOP {  $$ = Node::makeTerminalNode(E_STRING, "TEXT_FILTER " + $2->Serialize() +  " FILL UP SORT MATCHES TO TOP " ); delete($2); }
;


opt_order_by:
    /*EMPTY*/	{ $$ = nullptr; }
  | order_by
;

order_by:
    ORDER BY sort_list
{
    $$ = Node::makeNonTerminalNode(E_ORDER_BY, E_ORDER_BY_PROPERTY_CNT, $3, nullptr);
    $$->serialize_format = &ORDER_BY_SERIALIZE_FORMAT;
}
;

sort_list:
    sort_key
  | sort_list ',' sort_key
{
    $$ = Node::makeNonTerminalNode(E_SORT_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;

sort_key:
    row_expr opt_asc_desc
{
    $$ = Node::makeNonTerminalNode(E_SORT_KEY, E_SORT_KEY_PROPERTY_CNT, $1, $2);
    $$->serialize_format = &SORT_KEY_SERIALIZE_FORMAT;
}
;

opt_asc_desc:
    /*EMPTY*/
{
    $$ = Node::makeTerminalNode(E_SORT_ASC, "");
}
  | ASC
{
    $$ = Node::makeTerminalNode(E_SORT_ASC, "ASC");
}
  | DESC
{
    $$ = Node::makeTerminalNode(E_SORT_DESC, "DESC");
}
;

opt_having:
    /*EMPTY*/	{ $$ = nullptr; }
  | HAVING search_condition
{
    $$ = Node::makeNonTerminalNode(E_HAVING, E_HAVING_PROPERTY_CNT, $2);
    $$->serialize_format = &HAVING_SERIALIZE_FORMAT;
}
;


/* hana support RECURSIVE common table expression */
with_clause:
    WITH with_list
{
    $$ = Node::makeNonTerminalNode(E_OPT_WITH_CLAUSE, E_OPT_WITH_CLAUSE_PROPERTY_CNT, $2);
    $$->serialize_format = &WITH_CLAUSE_SERIALIZE_FORMAT;
}
  | WITH RECURSIVE with_list
{
    $$ = Node::makeNonTerminalNode(E_OPT_WITH_CLAUSE, E_OPT_WITH_CLAUSE_PROPERTY_CNT, $3);
    $$->serialize_format = &WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT;
}
;

with_list:
    common_table_expr
  | with_list ',' common_table_expr
{
    $$ = Node::makeNonTerminalNode(E_WITH_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;

/*todo hana support <search or cycle clause> */
common_table_expr:
    name_r opt_derived_column_list AS table_subquery
{
    $$ = Node::makeNonTerminalNode(E_COMMON_TABLE_EXPR, E_COMMON_TABLE_EXPR_PROPERTY_CNT, $1, $2, $4);
    $$->serialize_format = &COMMON_TABLE_EXPR_SERIALIZE_FORMAT;
}
;

opt_derived_column_list:
    /*EMPTY*/	{ $$ = nullptr; }
  | simple_ident_list_with_parens
;

simple_ident_list_with_parens:
    '(' simple_ident_list ')'
{
    $$ = Node::makeNonTerminalNode(E_SIMPLE_IDENT_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2);
    $$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
;

simple_ident_list:
    name_r
  | simple_ident_list ',' name_r
{
    $$ = Node::makeNonTerminalNode(E_SIMPLE_IDENT_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;
column_ref_list:
    column_ref
    | column_ref_list ',' column_ref
{
    $$ = Node::makeNonTerminalNode(E_SIMPLE_IDENT_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}

opt_distinct:
    /*EMPTY*/	{ $$ = nullptr; }
  | ALL
{
    $$ = Node::makeTerminalNode(E_ALL, "ALL");
}
  | DISTINCT
{
    $$ = Node::makeTerminalNode(E_DISTINCT, "DISTINCT");
}
;

select_expr_list:
    projection
  | select_expr_list ',' projection
{
    $$ = Node::makeNonTerminalNode(E_SELECT_EXPR_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;

projection:
    row_expr opt_as_label
{
    if (!$2) {
    	$$ = Node::makeNonTerminalNode(E_PROJECT_STRING, E_PROJECT_STRING_PROPERTY_CNT, $1);
    	$$->serialize_format = &SINGLE_SERIALIZE_FORMAT;
    }
    else {
	Node* alias_node = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, $1, $2, nullptr, nullptr, nullptr);
        alias_node->serialize_format = &AS_SERIALIZE_FORMAT;

        $$ = Node::makeNonTerminalNode(E_PROJECT_STRING, E_PROJECT_STRING_PROPERTY_CNT, alias_node);
        $$->serialize_format = &SINGLE_SERIALIZE_FORMAT;
    }
}
  | '*'
{
    Node* star_node = Node::makeTerminalNode(E_STAR, "*");
    $$ = Node::makeNonTerminalNode(E_PROJECT_STRING, E_PROJECT_STRING_PROPERTY_CNT, star_node);
    $$->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
;

from_list:
    table_reference
  | from_list ',' table_reference
{
    $$ = Node::makeNonTerminalNode(E_FROM_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;


table_reference:
    table_primary
  | joined_table
  | hana_construct_table
;

table_primary:
    table_primary_non_join
  | '(' joined_table ')'
{
    $$ = Node::makeNonTerminalNode(E_JOINED_TABLE_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2);
    $$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
;


/* todo hana support <table function derived table> */
/* todo hana support <only spec> */
table_primary_non_join:
    relation_factor  opt_table_qualify opt_as_label opt_tablesample
{
    $$ = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, $1, $3, $4, nullptr, $2);
    $$->serialize_format = &AS_SERIALIZE_FORMAT;
}
  | sap_table_subquery opt_as_label opt_simple_ident_list_with_parens
{
    $$ = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, $1, $2, $3, nullptr, nullptr);
    $$->serialize_format = &AS_SERIALIZE_FORMAT;
}
    | scalar_function opt_as_label
{
    $$ = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, $1, $2, nullptr, nullptr, nullptr);
    $$->serialize_format = &AS_SERIALIZE_FORMAT;
}
    | associated_table opt_as_label
{
    $$ = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, $1, $2, nullptr, nullptr, nullptr);
    $$->serialize_format = &AS_SERIALIZE_FORMAT;
}
    | collection_derived_table opt_as_label
{
    $$ = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, $1, $2, nullptr, nullptr, nullptr);
    $$->serialize_format = &AS_SERIALIZE_FORMAT;
}
    | lateral_table opt_as_label
{
    $$ = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, $1, $2, nullptr, nullptr, nullptr);
    $$->serialize_format = &AS_SERIALIZE_FORMAT;
}
;

lateral_table:
      LATERAL '(' scalar_function  ')'
{
    $$ = Node::makeNonTerminalNode(E_LATERAL_QUERY, 1, $3);
    $$->serialize_format = &LATERAL_QUERY_SERIALIZE_FORMAT;
}
    | LATERAL '(' query_expression ')'
{
    $$ = Node::makeNonTerminalNode(E_LATERAL_QUERY, 1, $3);
    $$->serialize_format = &LATERAL_QUERY_SERIALIZE_FORMAT;
}
;

associated_table:
    relation_factor '[' search_condition ']' ':' associated_ref_list
{
    $$ = Node::makeNonTerminalNode(E_ASSOCIATED_TABLE, 3, $1, $3, $6);
    $$->serialize_format = &ASSOCIATED_TAB_SERIALIZE_FORMAT;
}
|    relation_factor ':' associated_ref_list
{
    $$ = Node::makeNonTerminalNode(E_ASSOCIATED_TABLE,3, $1, nullptr, $3);
    $$->serialize_format = &ASSOCIATED_TAB_SERIALIZE_FORMAT;
}
;
associated_ref_list:
    associated_ref
    | associated_ref_list '.' associated_ref
{
    $$ = Node::makeNonTerminalNode(E_ASSOCIATED_REF_LIST, 2, $1, $3);
    $$->serialize_format = &ASSOCIATED_LIST_SERIALIZE_FORMAT;
}
;
associated_ref:
    name_r '[' opt_search_condition opt_many2one_part ']'
{
    $$ = Node::makeNonTerminalNode(E_ASSOCIATED_REF, 3, $1, $3, $4);
    $$->serialize_format = &ASSOCIATED_REF_SERIALIZE_FORMAT;
}
    |  name_r
{
    $$ = Node::makeNonTerminalNode(E_ASSOCIATED_REF, 3, $1, nullptr, nullptr);
    $$->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
}
;
opt_many2one_part:
    /* EMPTY */               { $$=nullptr; }
    | USING TO ONE JOIN     { $$ = Node::makeTerminalNode(E_STRING, "USING TO ONE JOIN"); }
    | USING TO MANY JOIN    { $$ = Node::makeTerminalNode(E_STRING, "USING TO MANY JOIN"); }
    | USING MANY TO ONE JOIN  { $$ = Node::makeTerminalNode(E_STRING, "USING MANY TO ONE JOIN"); }
    | USING MANY TO MANY JOIN { $$ = Node::makeTerminalNode(E_STRING, "USING MANY TO MANY JOIN"); }
    | USING ONE TO ONE JOIN   { $$ = Node::makeTerminalNode(E_STRING, "USING ONE TO ONE JOIN"); }
    | USING ONE TO MANY JOIN  { $$ = Node::makeTerminalNode(E_STRING, "USING ONE TO MANY JOIN"); }
;
opt_search_condition:
    /* EMPTY */ { $$=nullptr; }
    | search_condition
;

opt_tablesample:
      /*empty*/ { $$ = NULL; }
    | tablesample_num
;

opt_table_qualify:
      /*empty*/ { $$ = NULL; }
    | for_system_time
    | for_application_time
    | partition_restriction
    | for_update   /* conflict FOR ... */
;


collection_derived_table:
    UNNEST '(' collection_value_expr_list ')'  as_derived_part
{
    $$ = Node::makeNonTerminalNode(E_UNNEST_TABLE, 2, $3, $5);
    $$->serialize_format = &UNNEST_TABLE_FORMAT;
}
;

collection_value_expr_list:
    collection_value_expr ',' collection_value_expr_list
{
    $$ = Node::makeNonTerminalNode(E_COLLECT_VAL_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
    | collection_value_expr
;

collection_value_expr:
    ARRAY '(' row_expr_list ')'
{
    $$ = Node::makeNonTerminalNode(E_COLLECT_VAL, 1, $3);
    $$->serialize_format = &COLLECT_VAL_FORMAT;
}
| column_ref
;

as_derived_part:
    WITH ORDINALITY AS NAME '(' simple_ident_list ')'
{
    Node * p1 = Node::makeTerminalNode(E_STRING, "WITH ORDINALITY AS");
    $$ = Node::makeNonTerminalNode(E_AS_DERIVED_PART, 3, p1, $4, $6);
    $$->serialize_format = &AS_DERIVED_PART_FORMAT;
}
    | AS NAME  '(' simple_ident_list ')'
{
    Node * p1 = Node::makeTerminalNode(E_STRING, "AS");
    $$ = Node::makeNonTerminalNode(E_AS_DERIVED_PART, 3, p1, $2, $4);
    $$->serialize_format = &AS_DERIVED_PART_FORMAT;
}
    | WITH ORDINALITY AS NAME
{
    Node * p1 = Node::makeTerminalNode(E_STRING, "WITH ORDINALITY AS");
    $$ = Node::makeNonTerminalNode(E_AS_DERIVED_PART, 3, p1, $4,nullptr);
    $$->serialize_format = &AS_DERIVED_PART_FORMAT;
}
    | AS NAME
{
    Node * p1 = Node::makeTerminalNode(E_STRING, "AS");
    $$ = Node::makeNonTerminalNode(E_AS_DERIVED_PART, 3, p1, $2, nullptr);
    $$->serialize_format = &AS_DERIVED_PART_FORMAT;
}
;


opt_simple_ident_list_with_parens:
    /*EMPTY*/	{ $$ = nullptr; }
  | simple_ident_list_with_parens
;

/* keep these that can solve <identifier chain> in hana */
column_ref:
		 			    name_r
{
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			$1, nullptr, nullptr, nullptr, nullptr);
    $$->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
    |   		           name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			$3, $1, nullptr, nullptr, nullptr);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_1;
}
    |   			   name_r '.' '*'
{
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, $1, nullptr, nullptr, nullptr);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_1;
}
    |		          name_r '.' name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			$5, $3, $1, nullptr, nullptr);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_2;
}
    |		          name_r '.' name_r '.' '*'
{
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, $3, $1, nullptr, nullptr);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_2;
}
    |	         name_r '.' name_r '.' name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			$7, $5, $3, $1, nullptr);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_3;
}
    |            name_r '.' name_r '.' name_r '.' '*'
{
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, $5, $3, $1, nullptr);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_3;
}
    |	         name_r '.'      '.' name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			$6, $4, nullptr, $1, nullptr);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_3;
}
    |            name_r '.'      '.' name_r '.' '*'
{
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, $4, nullptr, $1, nullptr);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_3;
}
    |   name_r '.' name_r '.' name_r '.' name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			$9, $7, $5, $3, $1);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
    |   name_r '.' name_r '.' name_r '.' name_r '.' '*'
{
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, $7, $5, $3, $1);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
    |   name_r '.'      '.' name_r '.' name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			$8, $6, $4, nullptr, $1);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
    |   name_r '.'      '.' name_r '.' name_r '.' '*'
{
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, $6, $4, nullptr, $1);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
    |   name_r '.' name_r '.'      '.' name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			$8, $6, nullptr, $3, $1);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
    |   name_r '.' name_r '.'      '.' name_r '.' '*'
{
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, $6, nullptr, $3, $1);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
    |   name_r '.'      '.'      '.' name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			$7, $5, nullptr, nullptr, $1);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
    |   name_r '.'      '.'      '.' name_r '.' '*'
{
    Node* nd = Node::makeTerminalNode(E_STAR, "*");
    $$ = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			nd, $5, nullptr, nullptr, $1);
    $$->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
;

/* keep these that can solve <identifier chain> in hana */
relation_factor:
    	                         name_r
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $1, nullptr, nullptr, nullptr);
    $$->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
  |                     name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $3, $1, nullptr, nullptr);
    $$->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_1;
}
  |            name_r '.' name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $5, $3, $1, nullptr);
    $$->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_2;
}
  |            name_r '.'      '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $4, nullptr, $1, nullptr);
    $$->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_2;
}
  |   name_r '.' name_r '.' name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $7, $5, $3, $1);
    $$->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
  |   name_r '.'      '.' name_r '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $6, $4, nullptr, $1);
    $$->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
  |   name_r '.' name_r '.'      '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $6, nullptr, $3, $1);
    $$->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
  |   name_r '.'      '.'      '.' name_r
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $5, nullptr, nullptr, $1);
    $$->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
|  ':'   name_r
{
    $$ = Node::makeNonTerminalNode(E_TABLE_VAR, 1, $2);
    $$->serialize_format = &TABLE_VAR_SERIALIZE_FORMAT;
}
;

func_relation_factor:
    	                         name_f
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $1, nullptr, nullptr, nullptr);
    $$->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
  |                     name_r '.' name_f
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $3, $1, nullptr, nullptr);
    $$->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_1;
}
  |            name_r '.' name_r '.' name_f
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $5, $3, $1, nullptr);
    $$->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_2;
}
  |   name_r '.' name_r '.' name_r '.' name_f
{
    $$ = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, $7, $5, $3, $1);
    $$->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
;



/* todo hana support <union join> ??? */
joined_table:
    qualified_join
  | cross_join
//                  | natural_join		/* todo */
;

qualified_join:
    table_reference join_type JOIN table_reference ON search_condition
{
    $$ = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, $2, $1, $4, $6);
    $$->serialize_format = &JOINED_TB_1_SERIALIZE_FORMAT;
}
    | table_reference JOIN table_reference ON search_condition
{
    Node* nd = Node::makeTerminalNode(E_JOIN_INNER, "");
    $$ = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, nd, $1, $3, $5);
    $$->serialize_format = &JOINED_TB_1_SERIALIZE_FORMAT;
}
    | table_reference join_type case_join
{
    $$ = Node::makeNonTerminalNode(E_CASE_JOIN_TABLE, 3, $1, $2, $3);
    $$->serialize_format = &TRIBLE_SERIALIZE_FORMAT;
};
/*
                      | table_reference join_type JOIN table_reference USING simple_ident_list_with_parens
                    {
                        $$ = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, $2, $1, $4, $6);
                        $$->serialize_format = &JOINED_TB_USING_SERIALIZE_FORMAT;
                    }
                      | table_reference JOIN table_reference USING simple_ident_list_with_parens
                    {
                        Node* nd = Node::makeTerminalNode(E_JOIN_INNER, "");
                        $$ = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, nd, $1, $3, $5);
                        $$->serialize_format = &JOINED_TB_USING_SERIALIZE_FORMAT;
                    }
                    ;
*/
case_join:
    CASE JOIN  case_join_when_list opt_case_join_else  END
{
    $$ = Node::makeNonTerminalNode(E_CASE_JOIN, 2, $3, $4);
    $$->serialize_format = &CASE_JOIN_SERIALIZE_FORMAT;
}
;
case_join_when_list:
    case_join_when
    | case_join_when case_join_when_list
{
    $$ = Node::makeNonTerminalNode(E_CASE_JOIN_WHEN_LIST, 2, $1, $2);
    $$->serialize_format = &SPACE_LIST_SERIALIZE_FORMAT;
}
;
case_join_when:
    WHEN search_condition THEN ret_join_on
{
    $$ = Node::makeNonTerminalNode(E_CASE_JOIN_WHEN, 2, $2, $4);
    $$->serialize_format = &CASE_JOIN_WHEN_SERIALIZE_FORMAT;
}
;
ret_join_on:
    RETURN select_expr_list_parens FROM table_reference ON search_condition
{
    $$ = Node::makeNonTerminalNode(E_CASE_JOIN_RET, 3, $2, $4, $6);
    $$->serialize_format = &CASE_JOIN_RET_SERIALIZE_FORMAT;
}
;
select_expr_list_parens:
    '(' select_expr_list ')'
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2);
    $$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
;
opt_case_join_else:
    /* EMPTY */ { $$ = nullptr; }
    | ELSE ret_join_on
{
    $$ = Node::makeNonTerminalNode(E_CASE_JOIN_ELSE, 1, $2);
    $$->serialize_format = &CASE_JOIN_ELSE_SERIALIZE_FORMAT;
}
;

cross_join:
    table_reference CROSS JOIN table_primary
{
    Node* nd = Node::makeTerminalNode(E_JOIN_CROSS, "CROSS");
    $$ = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, nd, $1, $4, nullptr);
    $$->serialize_format = &JOINED_TB_2_SERIALIZE_FORMAT;
}
;
        /*
                    natural_join:
                        table_reference NATURAL join_type JOIN table_primary
                    {
                        Node* nd = Node::makeTerminalNode(E_JOIN_NATURAL, "NATURAL " + $3->text());
                        $$ = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, nd, $1, $5, nullptr);
                        $$->serialize_format = &JOINED_TB_2_SERIALIZE_FORMAT;
                    }
                      | table_reference NATURAL JOIN table_primary
                    {
                        Node* nd = Node::makeTerminalNode(E_JOIN_NATURAL, "NATURAL");
                        $$ = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, nd, $1, $4, nullptr);
                        $$->serialize_format = &JOINED_TB_2_SERIALIZE_FORMAT;
                    }
                    ;
        */

join_type:
    FULL join_outer join_cardinality
{
    std::string val = "FULL ";
    if ($2)
    {
        val += "OUTER ";
    }
    if($3){
        val+=$3->text();
        delete($3);
    }
    $$ = Node::makeTerminalNode(E_JOIN_FULL, val);
}
  | LEFT join_outer join_cardinality
{
    std::string val = "LEFT ";
    if ($2)
    {
        val += "OUTER ";
    }
    if($3){
        val+=$3->text();
        delete($3);
    }
    $$ = Node::makeTerminalNode(E_JOIN_LEFT, val);
}
  | RIGHT join_outer join_cardinality
{
    std::string val = "RIGHT ";
    if ($2)
    {
        val+="OUTER ";
    }
    if($3){
        val += $3->text();
        delete($3);
    }
    $$ = Node::makeTerminalNode(E_JOIN_RIGHT, val);

}
  | INNER join_cardinality
{
    std::string val = "INNER ";
    if($2){
        val += $2->text();
        delete($2);
    }
    $$ = Node::makeTerminalNode(E_JOIN_INNER, val);
}
;

join_outer:
    /* EMPTY */                 { $$ = 0; }
  | OUTER                       { $$ = 1; /*this is a flag*/}
;

join_cardinality:
    /* EMPTY */          { $$ = nullptr; }
    | MANY TO MANY      { $$ = Node::makeTerminalNode(E_STRING, "MANY TO MANY"); }
    | MANY TO ONE       { $$ = Node::makeTerminalNode(E_STRING, "MANY TO ONE"); }
    | MANY TO EXACT ONE { $$ = Node::makeTerminalNode(E_STRING, "MANY TO EXACT ONE"); }
    | ONE TO MANY       { $$ = Node::makeTerminalNode(E_STRING, "ONE TO MANY"); }
    | EXACT ONE TO MANY { $$ = Node::makeTerminalNode(E_STRING, "EXACT ONE TO MANY"); }
    | ONE TO ONE        { $$ = Node::makeTerminalNode(E_STRING, "ONE TO ONE"); }
    | EXACT ONE TO ONE  { $$ = Node::makeTerminalNode(E_STRING, "EXACT ONE TO ONE"); }
    | ONE TO EXACT ONE  { $$ = Node::makeTerminalNode(E_STRING, "ONE TO EXACT ONE"); }
    | EXACT ONE TO EXACT ONE     { $$ = Node::makeTerminalNode(E_STRING, "EXACT ONE TO EXACT ONE"); }
 ;

hana_construct_table:
    QUESTIONMARK AS name_r  '(' construct_column_list ')'
{
    $$ = Node::makeNonTerminalNode(E_CONSTRUCT_FROM_TABLE, E_CONSTRUCT_FROM_TABLE_PROPERTY_CNT, $1, $3, $5);
    $$->serialize_format = &CONSTRUCT_FROM_TABLE_SERIALIZE_FORMAT;
}
;

construct_column_list:
    construct_column
|   construct_column_list ',' construct_column
{
    $$ = Node::makeNonTerminalNode(E_CONSTRUCT_COL_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;

construct_column:
    name_r  predefined_type
{
    $$ = Node::makeNonTerminalNode(E_CONSTRUCT_COL, E_CONSTRUCT_COL_PROPERTY_CNT, $1, $2);
    $$->serialize_format = &CONSTRUCT_COL_SERIALIZE_FORMAT;
}
;

/* <search condition> grammar */
search_condition:
    boolean_term
  | search_condition OR boolean_term
{
    $$ = Node::makeNonTerminalNode(E_OP_OR, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format(E_OP_OR);
}
;

boolean_term:
    boolean_factor
  | boolean_term AND boolean_factor
{
    $$ = Node::makeNonTerminalNode(E_OP_AND, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format(E_OP_AND);
}
;

boolean_factor:
    boolean_test
  | NOT boolean_factor
{
    $$ = Node::makeNonTerminalNode(E_OP_NOT, E_OP_UNARY_PROPERTY_CNT, $2);
    $$->serialize_format = Node::op_serialize_format(E_OP_NOT);
}
;

boolean_test:
    boolean_primary
  | boolean_primary IS truth_value
{
    $$ = Node::makeNonTerminalNode(E_OP_IS, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format(E_OP_IS);
}
  | boolean_primary IS NOT truth_value
{
    $$ = Node::makeNonTerminalNode(E_OP_IS_NOT, E_OP_BINARY_PROPERTY_CNT, $1, $4);
    $$->serialize_format = Node::op_serialize_format(E_OP_IS_NOT);
}
;

boolean_primary:
    predicate
  | '(' search_condition ')'
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2);
    $$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
;

/* todo hana support <similar predicate> */
/* todo hana support <unique predicate> */
/* todo hana support <normalized predicate> */
/* todo hana support <match predicate> */
/* todo hana support <overlaps predicate> */
/* todo hana support <distinct predicate> */
/* todo hana support <submultiset predicate> */
/* todo hana support <set predicate> */
/* todo hana support <type predicate> */
predicate:
    comparison_predicate
  | between_predicate
  | in_predicate
  | like_predicate
  | null_predicate
  //| quantified_comparison_predicate
  | exists_predicate
  | bool_function
  | member_of_predicate
;

member_of_predicate:
    row_expr NOT MEMBER OF row_expr
{
    $$ = Node::makeNonTerminalNode(E_OP_NOT_MEMBER, 2, $1, $5);
    $$->serialize_format = Node::op_serialize_format(E_OP_NOT_MEMBER);
}
    | row_expr   MEMBER OF row_expr
{
    $$ = Node::makeNonTerminalNode(E_OP_MEMBER, 2, $1, $4);
    $$->serialize_format = Node::op_serialize_format(E_OP_MEMBER);
}
;
bool_function:
    CONTAINS'(' row_expr_list ')'
{
    Node * node_str = Node::makeTerminalNode(E_STRING, "CONTAINS");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		node_str, $3, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
;
comparison_predicate:
    row_expr comp_all_some_any_op row_expr
{
    $$ = Node::makeNonTerminalNode($2, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format($2);
}
    |row_expr comp_op row_expr
{
    $$ = Node::makeNonTerminalNode($2, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format($2);
}
;



between_predicate:
    row_expr BETWEEN row_expr AND row_expr
{
    $$ = Node::makeNonTerminalNode(E_OP_BTW, E_OP_TERNARY_PROPERTY_CNT, $1, $3, $5);
    $$->serialize_format = Node::op_serialize_format(E_OP_BTW);
}
  | row_expr NOT BETWEEN row_expr AND row_expr
{
    $$ = Node::makeNonTerminalNode(E_OP_NOT_BTW, E_OP_TERNARY_PROPERTY_CNT, $1, $4, $6);
    $$->serialize_format = Node::op_serialize_format(E_OP_NOT_BTW);
}
;

like_predicate:
    row_expr LIKE row_expr
{
    $$ = Node::makeNonTerminalNode(E_OP_LIKE, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format(E_OP_LIKE);
}
  | row_expr LIKE row_expr ESCAPE row_expr
{
    $$ = Node::makeNonTerminalNode(E_OP_LIKE, E_OP_TERNARY_PROPERTY_CNT, $1, $3, $5);
    $$->serialize_format = Node::op_serialize_format(E_OP_LIKE);
}
  | row_expr NOT LIKE row_expr
{
    $$ = Node::makeNonTerminalNode(E_OP_NOT_LIKE, E_OP_BINARY_PROPERTY_CNT, $1, $4);
    $$->serialize_format = Node::op_serialize_format(E_OP_NOT_LIKE);
}
  | row_expr NOT LIKE row_expr ESCAPE row_expr
{
    $$ = Node::makeNonTerminalNode(E_OP_NOT_LIKE, E_OP_TERNARY_PROPERTY_CNT, $1, $4, $6);
    $$->serialize_format = Node::op_serialize_format(E_OP_NOT_LIKE);
}
;

in_predicate:
    row_expr IN in_expr
{
    $$ = Node::makeNonTerminalNode(E_OP_IN, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format(E_OP_IN);
}
  | row_expr NOT IN in_expr
{
    $$ = Node::makeNonTerminalNode(E_OP_NOT_IN, E_OP_BINARY_PROPERTY_CNT, $1, $4);
    $$->serialize_format = Node::op_serialize_format(E_OP_NOT_IN);
}
;

null_predicate:
    row_expr IS NULLX
{
    $$ = Node::makeNonTerminalNode(E_OP_IS, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format(E_OP_IS);
}
  | row_expr IS NOT NULLX
{
    $$ = Node::makeNonTerminalNode(E_OP_IS_NOT, E_OP_BINARY_PROPERTY_CNT, $1, $4);
    $$->serialize_format = Node::op_serialize_format(E_OP_IS_NOT);
}
;

exists_predicate:
    EXISTS table_subquery
{
    $$ = Node::makeNonTerminalNode(E_OP_EXISTS, E_OP_UNARY_PROPERTY_CNT, $2);
    $$->serialize_format = Node::op_serialize_format(E_OP_EXISTS);
}
;

/* <row value ctor> */
row_expr:
    factor0
  | row_expr collate_clause
{
    $$ = Node::makeNonTerminalNode(E_OP_COLLATE, E_OP_BINARY_PROPERTY_CNT, $1, $2);
    $$->serialize_format = &SORT_KEY_SERIALIZE_FORMAT;
}
  | row_expr cnn_op factor0
{
    $$ = Node::makeNonTerminalNode(E_OP_CNN, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format($2);
}
;

factor0:
    factor1
  | factor0 plus_minus_op factor1
{
    $$ = Node::makeNonTerminalNode(E_OP_ADD, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format($2);
}
;

factor1:
    factor2
  | factor1 star_div_percent_mod_op factor2
{
    $$ = Node::makeNonTerminalNode(E_OP_MUL, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format($2);
}
;

factor2:
    factor3
  | factor2 '^' factor3
{
    $$ = Node::makeNonTerminalNode(E_OP_POW, E_OP_BINARY_PROPERTY_CNT, $1, $3);
    $$->serialize_format = Node::op_serialize_format(E_OP_POW);
}
;

factor3:
    factor4
  | '+' factor3
{
    $$ = Node::makeNonTerminalNode(E_OP_POS, E_OP_UNARY_PROPERTY_CNT, $2);
    $$->serialize_format = Node::op_serialize_format(E_OP_POS);
}
  | '-' factor3
{
    $$ = Node::makeNonTerminalNode(E_OP_NEG, E_OP_UNARY_PROPERTY_CNT, $2);
    $$->serialize_format = Node::op_serialize_format(E_OP_NEG);
}
;

factor4:
    column_ref
  | expr_const
  | row_subquery
  | '(' row_expr_list ')'
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2);
    $$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
  | case_expr
  | func_expr
;

row_expr_list:
    row_expr
  | row_expr_list ',' row_expr
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;

in_expr:
    table_subquery
  | '(' row_expr_list ')'
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2);
    $$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
;

truth_value: BOOL | UNKNOWN
;

comp_op:
    COMP_LE		{ $$ = E_OP_LE; }
  | COMP_LT		{ $$ = E_OP_LT; }
  | COMP_GE		{ $$ = E_OP_GE; }
  | COMP_GT		{ $$ = E_OP_GT; }
  | COMP_EQ		{ $$ = E_OP_EQ; }
  | COMP_NE		{ $$ = E_OP_NE; }
  | COMP_GT COMP_EQ	{ $$ = E_OP_GE; }	/* this means qty >     = 15 is ok */
  | COMP_LT COMP_EQ	{ $$ = E_OP_LE; }	/* this means qty <     = 15 is ok */
  | COMP_LT COMP_GT	{ $$ = E_OP_NE; }	/* this means qty <     > 15 is ok */
  | '!' COMP_EQ		{ $$ = E_OP_NE; }	/* this means qty !     = 15 is ok */
;

cnn_op:
    CNNOP		{ $$ = E_OP_CNN; }
  | '|' '|'		{ $$ = E_OP_CNN; }	/* this means 'asd' |     | 'qwe' is ok */
;


comp_all_some_any_op:
    comp_op all_some_any
{
    $$ = Node::comp_all_some_any_op_form($1, $2);
}
;

plus_minus_op:
    '+'		{ $$ = E_OP_ADD; }
  | '-'		{ $$ = E_OP_MINUS; }
;

star_div_percent_mod_op:
    '*'		{ $$ = E_OP_MUL; }
  | '/'		{ $$ = E_OP_DIV; }
  | '%'		{ $$ = E_OP_REM; }
;

expr_const:
    STRING
  | BOOL
  | NULLX
  | INTNUM
  | APPROXNUM
  | QUESTIONMARK
;

/* hana support <simple case> <searched case> */
case_expr:
    CASE case_arg when_clause_list case_default END
{
    $$ = Node::makeNonTerminalNode(E_CASE, E_CASE_PROPERTY_CNT, $2, $3, $4);
    $$->serialize_format = Node::op_serialize_format(E_CASE);
}
;

case_arg:
    /*EMPTY*/	{ $$ = nullptr; }	/* hana support <searched case> */
  | row_expr				/* hana support <simple case> */
;

when_clause_list:
    when_clause
  | when_clause_list when_clause
{
    $$ = Node::makeNonTerminalNode(E_WHEN_LIST, E_LIST_PROPERTY_CNT, $1, $2);
    $$->serialize_format = &SPACE_LIST_SERIALIZE_FORMAT;
}
;

when_clause:
    WHEN search_condition THEN row_expr
{
    $$ = Node::makeNonTerminalNode(E_WHEN, E_WHEN_PROPERTY_CNT, $2, $4);
    $$->serialize_format = &WHEN_SERIALIZE_FORMAT;
}
  | WHEN row_expr THEN row_expr
{
    $$ = Node::makeNonTerminalNode(E_WHEN, E_WHEN_PROPERTY_CNT, $2, $4);
    $$->serialize_format = &WHEN_SERIALIZE_FORMAT;
}
;

case_default:
    /*EMPTY*/	{ $$ = nullptr; }
  | ELSE row_expr
{
    $$ = Node::makeNonTerminalNode(E_CASE_DEFAULT, E_CASE_DEFAULT_PROPERTY_CNT, $2);
    $$->serialize_format = &ELSE_SERIALIZE_FORMAT;
}
;

/* todo */
func_expr:
    function_call_keyword
  | scalar_function
  | ranking_windowed_function
  | aggregate_expression
  | sap_specific_function
;

opt_nulls:
         { $$ = nullptr; }
    | NULLS FIRST { $$ = Node::makeTerminalNode(E_STRING, "NULLS FIRST"); }
    | NULLS LAST  { $$ = Node::makeTerminalNode(E_STRING, "NULLS LAST"); }
;


aggregate_expression :
	COUNT '(' '*' ')'
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "COUNT");
    Node* star = Node::makeTerminalNode(E_STAR, "*");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		name, star, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
	| COUNT '(' DISTINCT row_expr_list ')'
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "COUNT");
    Node * dist = Node::makeTerminalNode(E_IDENTIFIER, "DISTINCT");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		name, $4, nullptr, dist, nullptr);
    $$->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
	| COUNT '(' row_expr ')'
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "COUNT");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		name, $3, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
	| aggregate_name '(' opt_distinct row_expr ')'
{
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		$1, $4, nullptr, $3, nullptr);
    $$->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
	| CHAR '(' INTNUM ')'
{
    Node* name = Node::makeTerminalNode(E_STRING, "CHAR");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
            name, $3, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
;

aggregate_name :
	CORR                { $$ = Node::makeTerminalNode(E_IDENTIFIER, "CORR"); }
	| CORR_SPEARMAN     { $$ = Node::makeTerminalNode(E_IDENTIFIER, "CORR_SPEARMAN"); }
	| MIN               { $$ = Node::makeTerminalNode(E_IDENTIFIER, "MIN"); }
	| MEDIAN            { $$ = Node::makeTerminalNode(E_IDENTIFIER, "MEDIAN"); }
	| MAX               { $$ = Node::makeTerminalNode(E_IDENTIFIER, "MAX"); }
	| SUM               { $$ = Node::makeTerminalNode(E_IDENTIFIER, "SUM"); }
	| AVG               { $$ = Node::makeTerminalNode(E_IDENTIFIER, "AVG"); }
	| STDDEV            { $$ = Node::makeTerminalNode(E_IDENTIFIER, "STDDEV"); }
	| VAR               { $$ = Node::makeTerminalNode(E_IDENTIFIER, "VAR"); }
	| STDDEV_POP        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "STDDEV_POP"); }
	| VAR_POP           { $$ = Node::makeTerminalNode(E_IDENTIFIER, "VAR_POP"); }
	| STDDEV_SAMP       { $$ = Node::makeTerminalNode(E_IDENTIFIER, "STDDEV_SAMP"); }
	| VAR_SAMP          { $$ = Node::makeTerminalNode(E_IDENTIFIER, "VAR_SAMP"); }
;

sap_specific_function:
    BINNING '(' table_function_param_list  ')' over_clause
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "BINNING");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
            name, $3, $5, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
    | SERIES_FILTER '(' table_function_param_list  ')' over_clause
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "SERIES_FILTER");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
            name, $3, $5, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
    | EXTRACT '('  primary_datetime_field FROM row_expr  ')'
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "EXTRACT");
    Node * from = Node::makeTerminalNode(E_STRING, "FROM");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, from, nullptr, $3, $5);
    $$->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
    | FIRST_VALUE '(' row_expr order_by ')'
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "FIRST_VALUE");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, nullptr, $3, $4);
    $$->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
    | LAST_VALUE '(' row_expr order_by ')'
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "LAST_VALUE");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, nullptr, $3, $4);
    $$->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
    | NTH_VALUE '(' row_expr_list order_by ')'
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "NTH_VALUE");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, nullptr, $3, $4);
    $$->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
    | NTILE '(' row_expr_list ')'  over_clause
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "NTILE");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
            name, $3, $5, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
    | regexpr_func_name '(' STRING  IN regexpr_in_val ')'
{
    Node * in_ = Node::makeTerminalNode(E_STRING, "IN");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                $1, in_, nullptr, $3, $5);
    $$->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
    | PERCENTILE_CONT '(' expr_const ')' within_group over_clause
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "PERCENTILE_CONT");
    Node * over = Node::makeTerminalNode(E_STRING, $5->text() + $6->text());
    delete($5);
    delete($6);
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, over, $3 ,nullptr);
    $$->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
    | PERCENTILE_DISC '(' expr_const ')' within_group over_clause
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "PERCENTILE_DISC");
    Node * over = Node::makeTerminalNode(E_STRING, $5->text() + $6->text());
    delete($5);
    delete($6);
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, over, $3 ,nullptr);
    $$->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
    | STRING_AGG '(' row_expr_list opt_order_by_clause ')'
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "STRING_AGG");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, nullptr, $3, $4);
    $$->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
    | TRIM '(' row_expr FROM row_expr ')'
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "TRIM");
    Node * from = Node::makeTerminalNode(E_STRING, "FROM");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, from, nullptr, $3, $5);
    $$->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
    | TRIM '(' row_expr  ')'
{
    Node * name = Node::makeTerminalNode(E_IDENTIFIER, "TRIM");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
                name, nullptr, nullptr, $3, nullptr);
    $$->serialize_format = &FUN_CALL_314_SERIALIZE_FORMAT;
}
;

regexpr_func_name:
    LOCATE_REGEXPR { $$ = Node::makeTerminalNode(E_IDENTIFIER, "LOCATE_REGEXPR"); }
    | OCCURRENCES_REGEXPR { $$ = Node::makeTerminalNode(E_IDENTIFIER, "OCCURRENCES_REGEXPR"); }
    | REPLACE_REGEXPR { $$  = Node::makeTerminalNode(E_IDENTIFIER, "REPLACE_REGEXPR"); }
    | SUBSTR_REGEXPR { $$ = Node::makeTerminalNode(E_IDENTIFIER, "SUBSTR_REGEXPR"); }
;
within_group:
    WITHIN GROUP '(' order_by ')'
{
    $$ = Node::makeNonTerminalNode(E_WITHIN_GROUP, 1, $4);
    $$->serialize_format = &WITHIN_GROUP_SERIALIZE_FORMAT;
}
;
regexpr_in_val:
    column_ref
    | STRING
    | column_ref WITH STRING
{
    std::string val = $1->text();
    val += " WITH ";
    val += $3->text();
    delete($1);
    delete($3);
    $$ = Node::makeTerminalNode(E_STRING, val);
}
;
opt_order_by_clause:
    /*EMPTY*/ {$$=nullptr; }
    | order_by_clause
;

order_by_clause:
    ORDER BY  order_by_expression_list
{
    $$ = Node::makeNonTerminalNode(E_ORDER_BY_CLAUSE, 1, $3);
    $$->serialize_format = &AGG_ORDER_SERIALIZE_FORMAT;
};

order_by_expression_list:
    order_by_expression
    | order_by_expression_list ',' order_by_expression
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;
order_by_expression:
    column_ref opt_collate_clause opt_asc_desc  opt_nulls
{
    $$ = Node::makeNonTerminalNode(E_ORDER_BY_EXPR, 4,
        		$1, $2, $3, $4);
    $$->serialize_format = &QUADRUPLE_SERIALIZE_FORMAT;
}
    | INTNUM   opt_collate_clause opt_asc_desc  opt_nulls
{
    $$ = Node::makeNonTerminalNode(E_ORDER_BY_EXPR, 4,
            $1, $2, $3, $4);
    $$->serialize_format = &QUADRUPLE_SERIALIZE_FORMAT;
}
;
opt_collate_clause:
    /*EMPTY*/ {$$ = nullptr;}
    | collate_clause
;

ranking_windowed_function:
    ranking_function_name '('                ')' over_clause
{
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		$1, nullptr, $4, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
    | ranking_function_name '(' row_expr_list ')' over_clause
{
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
            $1, $3, $5, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
;


ranking_function_name:
    RANK		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "RANK"); }
  | DENSE_RANK		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "DENSE_RANK"); }
  | PERCENT_RANK	{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "PERCENT_RANK"); }
  | CUME_DIST		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "CUME_DIST"); }
  | ROW_NUMBER		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "ROW_NUMBER"); }
  | LAG             { $$ = Node::makeTerminalNode(E_IDENTIFIER, "LAG"); }
  | LEAD            { $$ = Node::makeTerminalNode(E_IDENTIFIER, "LEAD"); }
  | RANDOM_PARTITION    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "RANDOM_PARTITION"); }
  | WEIGHTED_AVG    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "WEIGHTED_AVG"); }
;

over_clause:
    /* EMPTY */ { $$ =nullptr; }
  |  OVER window_name
{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "OVER "+ $2->text()); delete($2); }
  | OVER window_specification
{ $$ = $2; }
;

window_specification:
    '(' window_specification_details ')'
{ $$ = $2; }
;

window_name:
    name_r
;

window_specification_details:
    opt_existing_window_name opt_window_partition_clause opt_order_by opt_window_frame_clause
{
    $$ = Node::makeNonTerminalNode(E_OVER_CLAUSE, E_OVER_CLAUSE_PROPERTY_CNT, $1, $2, $3, $4);
    $$->serialize_format = &OVER_CLAUSE_SERIALIZE_FORMAT;
}
;

opt_existing_window_name:
    /* EMPTY */	{ $$ = nullptr; }
  | window_name
;

opt_window_partition_clause:
    /* EMPTY */ { $$ = nullptr; }
  | PARTITION BY row_expr_list
{ $$ = $3; }
;

opt_window_frame_clause:
    /* EMPTY */ { $$ = nullptr; }
  | window_frame_units window_frame_extent opt_window_frame_exclusion
{
    std::string s3 = $3 ? $3->text() : "";
    $$ = Node::makeTerminalNode(E_IDENTIFIER, $1->text()+" "+$2->text()+" "+s3);
    delete($1); delete($2); delete($3);
}
;

window_frame_units:
    ROWS	{ $$ = Node::makeTerminalNode(E_IDENTIFIER,"ROWS"); }
  | RANGE	{ $$ = Node::makeTerminalNode(E_IDENTIFIER,"RANGE"); }
;

window_frame_extent:
    window_frame_start
  | window_frame_between
;

window_frame_start:
    UNBOUNDED PRECEDING		{ $$ = Node::makeTerminalNode(E_IDENTIFIER,"UNBOUNDED PRECEDING"); }
  | CURRENT ROW			{ $$ = Node::makeTerminalNode(E_IDENTIFIER,"CURRENT ROW"); }
  | window_frame_preceding
;

window_frame_preceding:
    expr_const PRECEDING	{ $$ = Node::makeTerminalNode(E_IDENTIFIER,$1->text()+" PRECEDING"); delete($1); }
;

window_frame_between:
    BETWEEN window_frame_bound AND window_frame_bound
{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "BETWEEN "+$2->text()+" AND "+$4->text()); delete($2); delete($4); }
;

window_frame_bound:
    window_frame_start
  | UNBOUNDED FOLLOWING		{ $$ = Node::makeTerminalNode(E_IDENTIFIER,"UNBOUNDED FOLLOWING"); }
  | window_frame_following
;

window_frame_following:
    expr_const FOLLOWING	{ $$ = Node::makeTerminalNode(E_IDENTIFIER,$1->text()+" FOLLOWING"); delete($1); }
;

opt_window_frame_exclusion:
    /* EMPTY */	{ $$ = nullptr; }
  | EXCLUDE CURRENT ROW		{ $$ = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE CURRENT ROW"); }
  | EXCLUDE GROUP		{ $$ = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE GROUP"); }
  | EXCLUDE TIES		{ $$ = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE TIES"); }
  | EXCLUDE NO OTHERS		{ $$ = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE NO OTHERS"); }
;

scalar_function:
    func_relation_factor '('                      ')'
{
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		$1, nullptr, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
  | func_relation_factor '(' table_function_param_list ')'
{
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		$1, $3, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
;


table_function_param_list:
    table_function_param
  | table_function_param_list ',' table_function_param
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;

table_function_param:
    expr_point_val
    | row_expr

expr_point_val:
    name_r P_POINT expr_const
{
        $$ = Node::makeNonTerminalNode(E_POINT_VALUE, 2, $1, $3);
        $$->serialize_format = &POINT_VALUE_FORMAT;
}
|   name_r P_POINT column_ref
{
        $$ = Node::makeNonTerminalNode(E_POINT_VALUE, 2, $1, $3);
        $$->serialize_format = &POINT_VALUE_FORMAT;
}
;

/* todo function_call_keyword with hana keyword */
/* Function call syntax using official SQL 2003 keywords.
     Because the function name is an official token */
function_call_keyword:
    primary_datetime_field '(' row_expr ')'
{
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		$1, $3, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
  | RIGHT '(' ')'
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "RIGHT");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
    |  RIGHT '('    row_expr_list ')'	/* hana support  */
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "RIGHT");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
        fun_name, $3, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
  | LEFT '(' row_expr ',' row_expr ')'
{
    Node* expr_list = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, $3, $5);
    expr_list->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;

    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "LEFT");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, expr_list, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
  |     CAST '('    row_expr AS data_type ')'	/* hana support <cast specification> */
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CAST");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, $3, nullptr, nullptr, $5);
    $$->serialize_format = &FUN_CALL_AS_SERIALIZE_FORMAT;
}
  |   CONVERT '(' row_expr USING relation_factor ')'	/* hana support <transcoding> */
{
    Node* transcoding_name = Node::makeTerminalNode(E_STRING, $5->text());
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CONVERT");
    delete($5);
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
        		fun_name, $3, nullptr, nullptr, transcoding_name);
    $$->serialize_format = &FUN_CALL_USING_SERIALIZE_FORMAT;
}
  |    COALESCE '('            row_expr_list ')'	/* hana support <case abbreviation> */
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "COALESCE");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, $3, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
  |       CURRENT_TIMESTAMP					/* hana support <current timestamp value function> */
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CURRENT_TIMESTAMP");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    $$->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
  |     CURRENT_TIMESTAMP '('            INTNUM    ')'	/* hana support <current timestamp value function> */
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CURRENT_TIMESTAMP");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, $3, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
  |      CURRENT_USER					/* hana support <general value specification> */
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CURRENT_USER");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    $$->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
  |        NULLIF '('          row_expr ',' row_expr ')'	/* hana support <case abbreviation> */
{
    Node* expr_list = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, $3, $5);
    expr_list->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "NULLIF");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, expr_list, nullptr, nullptr, nullptr);
    $$->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
  |    SESSION_USER					/* hana support <general value specification> */
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "SESSION_USER");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    $$->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
  |   SYSTEM_USER					/* hana support <general value specification> */
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "SYSTEM_USER");
    $$ = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    $$->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
;

for_xml: FOR XML opt_option_string_list_p opt_returns_clause
{
    Node* xml = Node::makeTerminalNode(E_STRING, "XML");
    $$ = Node::makeNonTerminalNode(E_FOR_CLAUSE_HANA, 4, xml, $3, $4, nullptr);
    $$->serialize_format = &FOR_CLAUSE_FORMAT;
}
;
for_json: FOR JSON opt_option_string_list_p opt_returns_clause
{
    Node* json = Node::makeTerminalNode(E_STRING, "JSON");
    $$ = Node::makeNonTerminalNode(E_FOR_CLAUSE_HANA, 4, json, $3, $4, nullptr);
    $$->serialize_format = &FOR_CLAUSE_FORMAT;
}
;
opt_returns_clause:
    /*EMPTY*/ {$$ = NULL;}
    | RETURNS predefined_type
{
    $$ = Node::makeNonTerminalNode(E_RETURNS_CLAUSE, 1, $2);
    $$->serialize_format = &RETURNS_CLAUSE_FORMAT;
}
;
opt_option_string_list_p:
    /*EMPTY*/ {$$ = NULL;}
    | '(' option_string_list ')'
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, $2);
    $$->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
;
option_string_list:
    option_string
    | option_string_list ',' option_string
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;
option_string:
    STRING COMP_EQ STRING
{
    $$ = Node::makeNonTerminalNode(E_OPTION_STRING, 2, $1, $3);
    $$->serialize_format = &OPTION_STRING_FORMAT;
}
;

/* todo hana support <row type> */
data_type:
    predefined_type
    | array_type
    //| user_defined_type_name
    //| reference_type
    //| collection_type
;

array_type:
    predefined_type ARRAY
{
    $$ = $1;
    $1->set_text( $1->text() + " ARRAY ");
}


/*
user_defined_type_name:
    relation_factor
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text());
    delete($1);
}
;

reference_type:
    REF '(' relation_factor ')' SCOPE relation_factor
{
    $$ = Node::makeTerminalNode(E_STRING, "REF("+$3->text()+")SCOPE "+$6->text());
    delete($3);delete($6);
}
;

collection_type:
    data_type ARRAY '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+" ARRAY("+$4->text()+")");
    delete($1);
    delete($4);
}
  | data_type ARRAY
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+" ARRAY");
    delete($1);
}
  | data_type MULTISET
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+" MULTISET");
    delete($1);
}
;
*/

predefined_type:
    DATE  {$$ = Node::makeTerminalNode(E_STRING, "DATE");}
    | TIME  {$$ = Node::makeTerminalNode(E_STRING, "TIME");}
    | SECONDDATE  {$$ = Node::makeTerminalNode(E_STRING, "SECONDDATE");}
    | TIMESTAMP  {$$ = Node::makeTerminalNode(E_STRING, "TIMESTAMP");}
    | TINYINT  {$$ = Node::makeTerminalNode(E_STRING, "TINYINT");}
    | SMALLINT  {$$ = Node::makeTerminalNode(E_STRING, "SMALLINT");}
    | INTEGER  {$$ = Node::makeTerminalNode(E_STRING, "INTEGER");}
    | BIGINT  {$$ = Node::makeTerminalNode(E_STRING, "BIGINT");}
    | SMALLDECIMAL  {$$ = Node::makeTerminalNode(E_STRING, "SMALLDECIMAL");}
    | REAL  {$$ = Node::makeTerminalNode(E_STRING, "REAL");}
    | DOUBLE  {$$ = Node::makeTerminalNode(E_STRING, "DOUBLE");}
    | TEXT  {$$ = Node::makeTerminalNode(E_STRING, "TEXT");}
    | BINTEXT {$$ = Node::makeTerminalNode(E_STRING, "BINTEXT");}
    | VARCHAR  {$$ = Node::makeTerminalNode(E_STRING, "VARCHAR");}
    | VARCHAR '(' INTNUM ')' {$$ = Node::makeTerminalNode(E_STRING, "VARCHAR("+$3->text()+")"); delete($3); }
    | NVARCHAR   {$$ = Node::makeTerminalNode(E_STRING, "NVARCHAR");}
    | NVARCHAR  '(' INTNUM ')' {$$ = Node::makeTerminalNode(E_STRING, "NVARCHAR("+$3->text()+")"); delete($3); }
    | ALPHANUM  {$$ = Node::makeTerminalNode(E_STRING, "ALPHANUM");}
    | ALPHANUM '(' INTNUM ')' {$$ = Node::makeTerminalNode(E_STRING, "ALPHANUM("+$3->text()+")"); delete($3); }
    | VARBINARY  {$$ = Node::makeTerminalNode(E_STRING, "VARBINARY");}
    | VARBINARY '(' INTNUM ')' {$$ = Node::makeTerminalNode(E_STRING, "VARBINARY("+$3->text()+")"); delete($3); }
    | SHORTTEXT  {$$ = Node::makeTerminalNode(E_STRING, "SHORTTEXT");}
    | SHORTTEXT '(' INTNUM ')' {$$ = Node::makeTerminalNode(E_STRING, "SHORTTEXT("+$3->text()+")"); delete($3); }
    | DECIMAL   {$$ = Node::makeTerminalNode(E_STRING, "DECIMAL");}
    | DECIMAL '(' INTNUM ')'  {$$ = Node::makeTerminalNode(E_STRING, "DECIMAL("+$3->text()+")"); delete($3); }
    | DECIMAL '(' INTNUM ',' INTNUM ')' {$$ = Node::makeTerminalNode(E_STRING, "DECIMAL("+$3->text()+","+$5->text()+")"); delete($3);delete($5); }
    | FLOAT  {$$ = Node::makeTerminalNode(E_STRING, "FLOAT");}
    | FLOAT '(' INTNUM ')' {$$ = Node::makeTerminalNode(E_STRING, "FLOAT("+$3->text()+")"); delete($3); }
    | BOOLEAN {$$ = Node::makeTerminalNode(E_STRING, "BOOLEAN");}
    | lob_data_type
    ;
lob_data_type :
    BLOB {$$ = Node::makeTerminalNode(E_STRING, "BLOB");}
    | CLOB  {$$ = Node::makeTerminalNode(E_STRING, "CLOB");}
    | NCLOB  {$$ = Node::makeTerminalNode(E_STRING, "NCLOB");}
    //| <lob_type_name> [MEMORY THRESHOLD <memory_threshold_value>]
    ;
/*
predefined_type:
    character_string_type CHARACTER SET relation_factor collate_clause
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+" CHARACTER SET "+$4->text()+$5->text());
    delete($1);delete($4);delete($5);
}
  | character_string_type CHARACTER SET relation_factor
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+" CHARACTER SET "+$4->text());
    delete($1);delete($4);
}
  | character_string_type                               collate_clause
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+$2->text());
    delete($1);delete($2);
}
  | character_string_type
  | national_character_string_type collate_clause
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+$2->text());
    delete($1);delete($2);
}
  | national_character_string_type
  | binary_large_object_string_type
  | numeric_type
  | boolean_type
  | datetime_type
  | interval_type
;
*/
/*
interval_type:
    INTERVAL interval_qualifier
{
    $$ = Node::makeTerminalNode(E_STRING, "INTERVAL "+$2->text());
    delete($2);
}
;
interval_qualifier:
    start_field TO end_field
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+" TO "+$3->text());
    delete($1);delete($3);
}
  | single_datetime_field
;
start_field:
    primary_datetime_field '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+"("+$3->text()+")");
    delete($3);
}
  | primary_datetime_field
;
end_field:
    primary_datetime_field
  | SECOND '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "SECOND("+$3->text()+")");
    delete($3);
}
  | SECOND
{
    $$ = Node::makeTerminalNode(E_STRING, "SECOND");
}
;
single_datetime_field:
    primary_datetime_field '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+"("+$3->text()+")");
    delete($1);delete($3);
}
  | primary_datetime_field
  | SECOND '(' INTNUM ',' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "SECOND("+$3->text()+","+$5->text()+")");
    delete($3);delete($5);
}

;
*/
primary_datetime_field:
    YEAR
{
    $$ = Node::makeTerminalNode(E_STRING, "YEAR");
}
  | MONTH
{
    $$ = Node::makeTerminalNode(E_STRING, "MONTH");
}
  | DAY
{
    $$ = Node::makeTerminalNode(E_STRING, "DAY");
}
  | HOUR
{
    $$ = Node::makeTerminalNode(E_STRING, "HOUR");
}
  | MINUTE
{
    $$ = Node::makeTerminalNode(E_STRING, "MINUTE");
}
    | SECOND
{
    $$ = Node::makeTerminalNode(E_STRING, "SECOND");
}
;

/*
boolean_type:
    BOOLEAN
{
    $$ = Node::makeTerminalNode(E_STRING, "BOOLEAN");
}
;
*/
/*
datetime_type:
    DATE
{
    $$ = Node::makeTerminalNode(E_STRING, "DATE");
}
  | TIME '(' INTNUM ')' WITH TIME ZONE
{
    $$ = Node::makeTerminalNode(E_STRING, "TIME("+$3->text()+") WITH TIME ZONE");
    delete($3);
}
  | TIME '(' INTNUM ')' WITHOUT TIME ZONE
{
    $$ = Node::makeTerminalNode(E_STRING, "TIME("+$3->text()+") WITHOUT TIME ZONE");
    delete($3);
}
  | TIME '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "TIME("+$3->text()+")");
    delete($3);
}
  | TIME                WITH TIME ZONE
{
    $$ = Node::makeTerminalNode(E_STRING, "TIME WITH TIME ZONE");
}
  | TIME                WITHOUT TIME ZONE
{
    $$ = Node::makeTerminalNode(E_STRING, "TIME WITHOUT TIME ZONE");
}
  | TIMESTAMP '(' INTNUM ')' WITH TIME ZONE
{
    $$ = Node::makeTerminalNode(E_STRING, "TIMESTAMP("+$3->text()+") WITH TIME ZONE");
    delete($3);
}
  | TIMESTAMP '(' INTNUM ')' WITHOUT TIME ZONE
{
    $$ = Node::makeTerminalNode(E_STRING, "TIMESTAMP("+$3->text()+") WITHOUT TIME ZONE");
    delete($3);
}
  | TIMESTAMP '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "TIMESTAMP("+$3->text()+")");
    delete($3);
}
  | TIMESTAMP                WITH TIME ZONE
{
    $$ = Node::makeTerminalNode(E_STRING, "TIMESTAMP WITH TIME ZONE");
}
  | TIMESTAMP                WITHOUT TIME ZONE
{
    $$ = Node::makeTerminalNode(E_STRING, "TIMESTAMP WITHOUT TIME ZONE");
}
;
*/
/*
numeric_type:
    exact_numeric_type
  | approximate_numeric_type
;

exact_numeric_type:
    NUMERIC '(' INTNUM ',' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NUMERIC("+$3->text()+","+$5->text()+")");
    delete($3);delete($5);
}
  | NUMERIC '(' INTNUM            ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NUMERIC("+$3->text()+")");
    delete($3);
}
  | DECIMAL '(' INTNUM ',' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "DECIMAL("+$3->text()+","+$5->text()+")");
    delete($3);delete($5);
}
  | DECIMAL '(' INTNUM            ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "DECIMAL("+$3->text()+")");
    delete($3);
}
  | DEC '(' INTNUM ',' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "DEC("+$3->text()+","+$5->text()+")");
    delete($3);delete($5);
}
  | DEC '(' INTNUM            ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "DEC("+$3->text()+")");
    delete($3);
}
  | DEC
{
    $$ = Node::makeTerminalNode(E_STRING, "DEC");
}
  | SMALLINT
{
    $$ = Node::makeTerminalNode(E_STRING, "SMALLINT");
}
  | INTEGER
{
    $$ = Node::makeTerminalNode(E_STRING, "INTEGER");
}
  | INT
{
    $$ = Node::makeTerminalNode(E_STRING, "INT");
}
  | BIGINT
{
    $$ = Node::makeTerminalNode(E_STRING, "BIGINT");
}
  | NUMERIC
{
    $$ = Node::makeTerminalNode(E_STRING, "NUMERIC");
}
  | DECIMAL
{
    $$ = Node::makeTerminalNode(E_STRING, "DECIMAL");
}
;

approximate_numeric_type:
    FLOAT '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "FLOAT("+$3->text()+")");
    delete($3);
}
  | DOUBLE PRECISION
{
    $$ = Node::makeTerminalNode(E_STRING, "DOUBLE PRECISION");
}
  | FLOAT
{
    $$ = Node::makeTerminalNode(E_STRING, "FLOAT");
}
  | REAL
{
    $$ = Node::makeTerminalNode(E_STRING, "REAL");
}
;
*/
/*
character_string_type:
    CHARACTER '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "CHARACTER("+$3->text()+")");
    delete($3);
}
  | CHAR '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "CHAR("+$3->text()+")");
    delete($3);
}
  | CHARACTER VARYING '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "CHARACTER VARYING("+$4->text()+")");
    delete($4);
}
  | CHAR VARYING '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "CHAR VARYING("+$4->text()+")");
    delete($4);
}
  | VARCHAR '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "VARCHAR("+$3->text()+")");
    delete($3);
}
  | NVARCHAR '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NVARCHAR("+$3->text()+")");
    delete($3);
}
  | CHARACTER LARGE OBJECT '(' large_object_length ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "CHARACTER LARGE OBJECT("+$5->text()+")");
    delete($5);
}
  | CHARACTER LARGE OBJECT
{
    $$ = Node::makeTerminalNode(E_STRING, "CHARACTER LARGE OBJECT");
}
  | CHAR LARGE OBJECT '(' large_object_length ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "CHAR LARGE OBJECT("+$5->text()+")");
    delete($5);
}
  | CHAR LARGE OBJECT
{
    $$ = Node::makeTerminalNode(E_STRING, "CHAR LARGE OBJECT");
}
  | CLOB '(' large_object_length ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "CLOB("+$3->text()+")");
    delete($3);
}
  | CLOB
{
    $$ = Node::makeTerminalNode(E_STRING, "CLOB");
}
  | CHAR
{
    $$ = Node::makeTerminalNode(E_STRING, "CHAR");
}
  | CHARACTER
{
    $$ = Node::makeTerminalNode(E_STRING, "CHARACTER");
}
;
*/
/*
binary_large_object_string_type:
    BINARY LARGE OBJECT '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "BINARY LARGE OBJECT("+$5->text()+")");
    delete($5);
}
  | BINARY LARGE OBJECT
{
    $$ = Node::makeTerminalNode(E_STRING, "BINARY LARGE OBJECT");
}
  | BLOB '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "BLOB("+$3->text()+")");
    delete($3);
}
  | BLOB
{
    $$ = Node::makeTerminalNode(E_STRING, "BLOB");
}
;
*/
/*
national_character_string_type:
    NATIONAL CHARACTER '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NATIONAL CHARACTER("+$4->text()+")");
    delete($4);
}
  | NATIONAL CHARACTER
{
    $$ = Node::makeTerminalNode(E_STRING, "NATIONAL CHARACTER");
}
  | NATIONAL CHAR '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NATIONAL CHAR("+$4->text()+")");
    delete($4);
}
  | NATIONAL CHAR
{
    $$ = Node::makeTerminalNode(E_STRING, "NATIONAL CHAR");
}
  | NCHAR '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NCHAR("+$3->text()+")");
    delete($3);
}
  | NATIONAL CHARACTER VARYING '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NATIONAL CHARACTER VARYING("+$5->text()+")");
    delete($5);
}
  | NATIONAL CHAR VARYING '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NATIONAL CHAR VARYING("+$5->text()+")");
    delete($5);
}
  | NCHAR VARYING '(' INTNUM ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NCHAR VARYING("+$4->text()+")");
    delete($4);
}
  | NATIONAL CHARACTER LARGE OBJECT '(' large_object_length ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NATIONAL CHARACTER LARGE OBJECT("+$6->text()+")");
    delete($6);
}
  | NATIONAL CHARACTER LARGE OBJECT
{
    $$ = Node::makeTerminalNode(E_STRING, "NATIONAL CHARACTER LARGE OBJECT");
}
  | NCHAR LARGE OBJECT '(' large_object_length ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NCHAR LARGE OBJECT("+$5->text()+")");
    delete($5);
}
  | NCHAR LARGE OBJECT
{
    $$ = Node::makeTerminalNode(E_STRING, "NCHAR LARGE OBJECT");
}
  | NCLOB '(' large_object_length ')'
{
    $$ = Node::makeTerminalNode(E_STRING, "NCLOB("+$3->text()+")");
    delete($3);
}
  | NCLOB
{
    $$ = Node::makeTerminalNode(E_STRING, "NCLOB");
}
  | NCHAR
{
    $$ = Node::makeTerminalNode(E_STRING, "NCHAR");
}
;


large_object_length:
    INTNUM multiplier char_length_units
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+" "+$2->text()+" "+$3->text());
    delete($1);delete($2);delete($3);
}
  | INTNUM multiplier
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+" "+$2->text());
    delete($1);delete($2);
}
  | INTNUM            char_length_units
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text()+" "+$2->text());
    delete($1);delete($2);
}
  | INTNUM
{
    $$ = Node::makeTerminalNode(E_STRING, $1->text());
    delete($1);
}
;

char_length_units:
    CHARACTERS
{
    $$ = Node::makeTerminalNode(E_STRING, "CHARACTERS");
}
  | CODE_UNITS
{
    $$ = Node::makeTerminalNode(E_STRING, "CODE_UNITS");
}
  | OCTETS
{
    $$ = Node::makeTerminalNode(E_STRING, "OCTETS");
}
;

multiplier:
    K
{
    $$ = Node::makeTerminalNode(E_STRING, "K");
}
  | M
{
    $$ = Node::makeTerminalNode(E_STRING, "M");
}
  | G
{
    $$ = Node::makeTerminalNode(E_STRING, "G");
}
;
*/


all_some_any:
    ALL		{ $$ = 0; }
  | SOME	{ $$ = 1; }
  | ANY		{ $$ = 2; }
;

opt_as_label:
    /* EMPTY */	{ $$ = nullptr; }
  | as_label
;

as_label:
    AS label	{ $$ = $2; }
  | label
;

label:
    name_r
  | STRING
;


collate_clause:
    COLLATE relation_factor
{
    $$ = Node::makeTerminalNode(E_STRING, "COLLATE "+$2->text());
    delete($2);
}
;

/* NAME with some non-reserved words  , contains function name */
name_r:
    NAME
    | reserved
    | aggregate_name	%prec UMINUS
    | ranking_function_name	%prec UMINUS
    | regexpr_func_name
    | STRING_AGG    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "STRING_AGG"); }
    | CHAR          { $$ = Node::makeTerminalNode(E_IDENTIFIER, "CHAR"); }
    | CONTAINS      { $$ = Node::makeTerminalNode(E_IDENTIFIER, "CONTAINS"); }
    | BINNING       { $$ = Node::makeTerminalNode(E_IDENTIFIER, "BINNING"); }
    | EXTRACT         { $$ = Node::makeTerminalNode(E_IDENTIFIER, "EXTRACT"); }
    | FIRST_VALUE   { $$ = Node::makeTerminalNode(E_IDENTIFIER, "FIRST_VALUE"); }
    | LAST_VALUE    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "LAST_VALUE"); }
    | NTH_VALUE     { $$ = Node::makeTerminalNode(E_IDENTIFIER, "NTH_VALUE"); }
    | NTILE         { $$ = Node::makeTerminalNode(E_IDENTIFIER, "NTILE"); }
    | PERCENTILE_CONT   { $$ = Node::makeTerminalNode(E_IDENTIFIER, "PERCENTILE_CONT"); }
    | PERCENTILE_DISC   { $$ = Node::makeTerminalNode(E_IDENTIFIER, "PERCENTILE_DISC"); }
    | TRIM              { $$ = Node::makeTerminalNode(E_IDENTIFIER, "TRIM"); }

;

name_f:
    NAME
  | reserved
;

reserved:
primary_datetime_field	%prec UMINUS					/* SQL-2003-R, here is non-reserved */
  | K			    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "K"); }		/* SQL-2003-N */
  | M			    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "M"); }		/* SQL-2003-N */
  | G			    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "G"); }		/* SQL-2003-N */
  | ARRAY		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "ARRAY"); }		/* SQL-2003-R, here is non-reserved */
  | BINARY		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "BINARY"); }	/* SQL-2003-R, here is non-reserved */
  | CAST %prec UMINUS	{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "CAST"); }		/* SQL-2003-R, here is non-reserved */
  | CHARACTERS		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "CHARACTERS"); }	/* SQL-2003-N */
  | CODE_UNITS		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "CODE_UNITS"); }	/* SQL-2003-N */
  | CORRESPONDING	{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "CORRESPONDING"); }	/* SQL-2003-R, here is non-reserved */
  | FOLLOWING		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "FOLLOWING"); }	/* SQL-2003-N */
  | INTERVAL		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "INTERVAL"); }	/* SQL-2003-R, here is non-reserved */
  | LARGE		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "LARGE"); }		/* SQL-2003-R, here is non-reserved */
  | MULTISET		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "MULTISET"); }	/* SQL-2003-R, here is non-reserved */
  | OBJECT		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "OBJECT"); }	/* SQL-2003-N */
  | OCTETS		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "OCTETS"); }	/* SQL-2003-N */
  | ONLY		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "ONLY"); }		/* SQL-2003-R, here is non-reserved */
  | PRECEDING		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "PRECEDING"); }	/* SQL-2003-N */
  | PRECISION		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "PRECISION"); }	/* SQL-2003-R, here is non-reserved */
  | RECURSIVE		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "RECURSIVE"); }	/* SQL-2003-R, here is non-reserved */
  | REF			    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "REF"); }		/* SQL-2003-R, here is non-reserved */
  | ROW			    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "ROW"); }		/* SQL-2003-R, here is non-reserved */
  | SCOPE		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "SCOPE"); }		/* SQL-2003-N */
  | UNBOUNDED		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "UNBOUNDED"); }	/* SQL-2003-N */
  | VARCHAR		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "VARCHAR"); }	/* SQL-2003-R, here is non-reserved */
  | WITHOUT		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "WITHOUT"); }	/* SQL-2003-R, here is non-reserved */
  | ZONE		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "ZONE"); }		/* SQL-2003-N */
  | OF			    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "OF"); }		/* SQL-2003-R, here is non-reserved */
  | READ		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "READ"); }		/* SQL-2003-N */
  | TIMESTAMP		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "TIMESTAMP"); }	/* SQL-2003-R, here is non-reserved */
  | TIME		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "TIME");  }		/* SQL-2003-R, here is non-reserved */
  | DESC		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "DESC"); }		/* SQL-2003-N */
  | TIES		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "TIES"); }		/* SQL-2003-N */
  | SETS		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "SETS"); }		/* SQL-2003-N */
  | OTHERS		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "OTHERS"); }	/* SQL-2003-N */
  | EXCLUDE		    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "EXCLUDE"); }	/* SQL-2003-N */
  | ASC			    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "ASC"); }		/* SQL-2003-N */
  | COALESCE %prec UMINUS	{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "COALESCE"); }	/* SQL-2003-N */
  | CONVERT %prec UMINUS	{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "CONVERT"); }	/* SQL-2003-N */
  | NULLIF %prec UMINUS		{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "NULLIF"); }	/* SQL-2003-N */
  | DEFAULT	%prec UMINUS	{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "DEFAULT"); }	/* HANA_20200522 */
  | TO		%prec UMINUS	{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "TO"); }		/* HANA_20200522 */
  | DOUBLE		%prec UMINUS	{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "DOUBLE"); }/* HANA_20200522 */
  | MOD			    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "MOD"); }                   /* HANA_20200522 */
  | MANY			{ $$ = Node::makeTerminalNode(E_IDENTIFIER, "MANY"); }                   /* HANA_20200522 */
  | ONE			    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "ONE"); }                   /* HANA_20200522 */
  | PRIMARY         { $$ = Node::makeTerminalNode(E_IDENTIFIER, "PRIMARY"); }
  | KEY             { $$ = Node::makeTerminalNode(E_IDENTIFIER, "KEY"); }
  | SECONDDATE      { $$ = Node::makeTerminalNode(E_IDENTIFIER, "SECONDDATE"); }
  | TINYINT         { $$ = Node::makeTerminalNode(E_IDENTIFIER, "TINYINT"); }
  | SMALLDECIMAL    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "SMALLDECIMAL"); }
  | TEXT            { $$ = Node::makeTerminalNode(E_IDENTIFIER, "TEXT"); }
  | BINTEXT         { $$ = Node::makeTerminalNode(E_IDENTIFIER, "BINTEXT"); }
  | ALPHANUM        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "ALPHANUM"); }
  | VARBINARY       { $$ = Node::makeTerminalNode(E_IDENTIFIER, "VARBINARY"); }
  | SHORTTEXT       { $$ = Node::makeTerminalNode(E_IDENTIFIER, "SHORTTEXT"); }
  | REPLACE         { $$ = Node::makeTerminalNode(E_IDENTIFIER, "REPLACE"); }
  | PARAMETERS      { $$ = Node::makeTerminalNode(E_IDENTIFIER, "PARAMETERS"); }
  | HISTORY         { $$ = Node::makeTerminalNode(E_IDENTIFIER, "HISTORY"); }
  | OVERRIDING      { $$ = Node::makeTerminalNode(E_IDENTIFIER, "OVERRIDING"); }
  | SYSTEM          { $$ = Node::makeTerminalNode(E_IDENTIFIER, "SYSTEM"); }
  | USER            { $$ = Node::makeTerminalNode(E_IDENTIFIER, "USER"); }
  | VALUE           { $$ = Node::makeTerminalNode(E_IDENTIFIER, "VALUE"); }
  | EXACT           { $$ = Node::makeTerminalNode(E_IDENTIFIER, "EXACT"); }
  | BERNOULLI       { $$ = Node::makeTerminalNode(E_IDENTIFIER, "BERNOULLI"); }
  | NCHAR           { $$ = Node::makeTerminalNode(E_IDENTIFIER, "NCHAR"); }
  /*| ROLLUP        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "ROLLUP"); } */
  /*| CUBE        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "CUBE"); }       */
  | BEST        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "BEST"); }
  | SUBTOTAL        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "SUBTOTAL"); }
  | BALANCE        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "BALANCE"); }
  | TOTAL        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "TOTAL"); }
  | MULTIPLE        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "MULTIPLE"); }
  | RESULTSETS        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "RESULTSETS"); }
  | PREFIX        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "PREFIX"); }
  | STRUCTURED        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "STRUCTURED"); }
  | RESULT        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "RESULT"); }
  | OVERVIEW        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "OVERVIEW"); }
  | TEXT_FILTER        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "TEXT_FILTER"); }
  | FILL        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "FILL"); }
  | UP        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "UP"); }
  | MATCHES        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "MATCHES"); }
  | SORT        { $$ = Node::makeTerminalNode(E_IDENTIFIER, "SORT"); }
  | ROWCOUNT    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "ROWCOUNT"); }
  | LOCKED      { $$ = Node::makeTerminalNode(E_IDENTIFIER, "LOCKED"); }
   /* | IGNORE      { $$ = Node::makeTerminalNode(E_IDENTIFIER, "IGNORE"); } */    /* for update have conflict */
   | SHARE      { $$ = Node::makeTerminalNode(E_IDENTIFIER, "SHARE"); }
   | LOCK       { $$ = Node::makeTerminalNode(E_IDENTIFIER, "LOCK"); }
   |UTCTIMESTAMP    { $$ = Node::makeTerminalNode(E_IDENTIFIER, "UTCTIMESTAMP"); }
   | COMMIT         { $$ = Node::makeTerminalNode(E_IDENTIFIER, "COMMIT"); }
   | ID             { $$ = Node::makeTerminalNode(E_IDENTIFIER, "ID"); }
   | JSON           { $$ = Node::makeTerminalNode(E_IDENTIFIER, "JSON"); }
   | XML            { $$ = Node::makeTerminalNode(E_IDENTIFIER, "XML"); }
   /* | WAIT           { $$ = Node::makeTerminalNode(E_IDENTIFIER, "WAIT"); } */    /* for update have conflict */
   /* | NOWAIT           { $$ = Node::makeTerminalNode(E_IDENTIFIER, "NOWAIT"); } */    /* for update have conflict */
;

top_clause:
    /*EMPTY*/ { $$ = nullptr; }
    | TOP INTNUM
{
    $$ = Node::makeNonTerminalNode(E_TOP_CLAUSE, E_OP_UNARY_PROPERTY_CNT, $2);
    $$->serialize_format = &TOP_CLAUSE_FORMAT;
}
    | TOP QUESTIONMARK
{
    $$ = Node::makeNonTerminalNode(E_TOP_CLAUSE, E_OP_UNARY_PROPERTY_CNT, $2);
    $$->serialize_format = &TOP_CLAUSE_FORMAT;
}
;

            /*
            time_travel:
                AS OF  COMMIT ID INTNUM
            {
                Node * comit = Node::makeTerminalNode(E_STRING, "COMMIT ID");
                $$ = Node::makeNonTerminalNode(E_TIME_TRAVEL, 2,  comit, $5);
                $$->serialize_format = &TIME_TRAVEL_FORMAT;
            }
                | AS OF UTCTIMESTAMP STRING
            {
                Node * utct = Node::makeTerminalNode(E_STRING, "UTCTIMESTAMP");
                $$ = Node::makeNonTerminalNode(E_TIME_TRAVEL, 2,  utct, $4);
                $$->serialize_format = &TIME_TRAVEL_FORMAT;
            }
            ;
            */

opt_for_update:
   /*EMPTY*/ { $$ = nullptr;}
   | for_update
   /*        | time_travel           */
;
for_share_lock:
    FOR SHARE LOCK { $$ = Node::makeTerminalNode(E_STRING, " FOR SHARE LOCK "); }
;
for_update:
     FOR UPDATE opt_of_ident_list wait_nowait opt_ignore_lock
{
    $$ = Node::makeNonTerminalNode(E_FOR_UPDATE, 3, $3, $4, $5);
    $$->serialize_format = &FOR_UPDATE_FORMAT_HANA;
}
    | for_share_lock
    | for_xml
    | for_json
;

opt_of_ident_list:   /* conflict:  select * from a for update x, y ,z  --( y,z will become table name)  */
    /*EMPTY*/ {$$ = nullptr;}
    | OF column_ref_list
{
    Node * p_of = Node::makeTerminalNode(E_UPDATE, "OF");
    $$ = Node::makeNonTerminalNode(E_OF_COLUMS, 2, p_of, $2);
    $$->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
;
opt_ignore_lock:
    /*EMPTY*/ {$$ = nullptr;}
    | IGNORE LOCKED { $$ = Node::makeTerminalNode(E_STRING, "IGNORE LOCKED");}
;
wait_nowait:
    /*EMPTY*/ {$$ = nullptr;}
    | NOWAIT { $$ = Node::makeTerminalNode(E_NOWAIT, "NOWAIT");}
    | WAIT INTNUM
{
    $$ = Node::makeNonTerminalNode(E_WAIT_INT, 1,  $2);
    $$->serialize_format = &WAIT_TIME_FORMAT;
}
;

limit_num:
    /*EMPTY*/ { $$ = nullptr; }
    | LIMIT INTNUM offset_num limit_total
{
    $$ = Node::makeNonTerminalNode(E_LIMIT_NUM, 3, $2, $3, $4);
    $$->serialize_format = &LIMIT_NUM_FORMAT_HANA;
}
    | LIMIT QUESTIONMARK offset_num limit_total
{
    $$ = Node::makeNonTerminalNode(E_LIMIT_NUM, 3, $2, $3, $4);
    $$->serialize_format = &LIMIT_NUM_FORMAT_HANA;
}
;

offset_num:
    /*EMPTY*/   { $$ = nullptr; }
    | OFFSET QUESTIONMARK
{
    $$ = Node::makeNonTerminalNode(E_OFFSET_NUM, 1, $2);
    $$->serialize_format = &OFFSET_NUM_FORMAT;
}
    | OFFSET INTNUM
{
    $$ = Node::makeNonTerminalNode(E_OFFSET_NUM, 1, $2);
    $$->serialize_format = &OFFSET_NUM_FORMAT;
}
;

limit_total:
   /*EMPTY*/   { $$ = nullptr; }
    | TOTAL ROWCOUNT
{
    $$ = Node::makeTerminalNode(E_STRING, "TOTAL ROWCOUNT");
}
;

with_hint_param:
    NAME
    | func_expr
;
with_hint_param_list:
    with_hint_param
    |with_hint_param ',' with_hint_param_list
{
    $$ = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;

hint_clause:
    /*empty*/ { $$ = nullptr; }
    | WITH HINT '(' with_hint_param_list ')' hint_clause
{
     std::string ss = $4->text();
     delete($4);
    std::string sval = "WITH HINT(" + ss + ")";
    if($6){
        sval += $6->text();
        delete($6);
    }
    $$ = Node::makeTerminalNode(E_STRING, sval);
}
    | WITH RANGE_RESTRICTION '(' STRING ')' hint_clause
{
    std::string ss = $4->text();
    delete($4);
    std::string sval = "WITH RANGE_RESTRICTION(" + ss + ")";
    if($6){
        sval += $6->text();
        delete($6);
    }
    $$ = Node::makeTerminalNode(E_STRING, sval);
}
    | WITH PARAMETERS '(' STRING COMP_EQ row_expr ')' hint_clause
{
    std::string ss1 = $4->text();
    delete($4);
    std::string ss2 = $6->text();
    delete($6);
    std::string sval = "WITH PARAMETERS(" + ss1 +"=" + ss2 + ")";
    if($8){
        sval += $8->text();
        delete($8);
    }
    $$ = Node::makeTerminalNode(E_STRING, sval);
}
;

opt_with_range_restrict:
    /*empty*/ { $$ = nullptr; }
    |  WITH RANGE_RESTRICTION '(' STRING ')'
{
    std::string sval = "WITH RANGE_RESTRICTION(" + $4->text() + ")";
    delete($4);
    $$ = Node::makeTerminalNode(E_STRING, sval);
}
;

with_primary_key:
    WITH PRIMARY KEY
{
    $$ = Node::makeTerminalNode(E_STRING, "WITH PRIMARY KEY");
}
;

upsert_stmt:
    UPSERT  relation_factor opt_derived_column_list   from_constructor  opt_where
{
    $$ = Node::makeNonTerminalNode(E_UPSERT, 6, $2, $3, $4, $5, NULL,NULL);
    $$->serialize_format = &UPSERT_SERIALIZE_FORMAT;
}
    | UPSERT  relation_factor opt_derived_column_list  from_constructor with_primary_key opt_with_range_restrict
{

    $$ = Node::makeNonTerminalNode(E_UPSERT, 6, $2, $3, $4, NULL, $5, $6);
    $$->serialize_format = &UPSERT_SERIALIZE_FORMAT;
}
    | UPSERT  relation_factor   select_stmt
{
    $$ = Node::makeNonTerminalNode(E_UPSERT, 6, $2, NULL, $3, NULL, NULL,NULL);
    $$->serialize_format = &UPSERT_SERIALIZE_FORMAT;
}

replace_stmt:
    REPLACE  relation_factor  opt_derived_column_list  from_constructor  opt_where
{
    $$ = Node::makeNonTerminalNode(E_REPLACE, 5, $2, $3, $4, $5, NULL);
    $$->serialize_format = &REPLACE_SERIALIZE_FORMAT;
}
    | REPLACE  relation_factor opt_derived_column_list  from_constructor WITH PRIMARY KEY
{
    Node * pwith = Node::makeTerminalNode(E_STRING, " WITH PRIMARY KEY");
    $$ = Node::makeNonTerminalNode(E_REPLACE, 5, $2, $3, $4, NULL, pwith);
    $$->serialize_format = &REPLACE_SERIALIZE_FORMAT;
}
    | REPLACE  relation_factor   select_stmt
{
    $$ = Node::makeNonTerminalNode(E_REPLACE, 5, $2, NULL, $3, NULL, NULL);
    $$->serialize_format = &REPLACE_SERIALIZE_FORMAT;
}
;
for_system_time:
    sys_as_of_spec
    | sys_from_to_spec
    | sys_between_spec
;
sys_as_of_spec:
    FOR SYSTEM_TIME AS OF STRING
{
    $$ = Node::makeTerminalNode(E_STRING, "FOR SYSTEM_TIME AS OF "+$5->Serialize());
    delete($5);
}
;
sys_from_to_spec:
    FOR SYSTEM_TIME FROM STRING TO STRING
{
    $$ = Node::makeTerminalNode(E_STRING, "FOR SYSTEM_TIME FROM "+$4->Serialize()+" TO "+$6->Serialize());
    delete($4);
    delete($6);
}
;
sys_between_spec:
    FOR SYSTEM_TIME BETWEEN STRING AND STRING
{
    $$ = Node::makeTerminalNode(E_STRING, "FOR SYSTEM_TIME BETWEEN "+$4->Serialize()+" AND "+$6->Serialize());
    delete($4);
    delete($6);
}
;
for_application_time:
    FOR APPLICATION_TIME AS OF STRING
{
    $$ = Node::makeTerminalNode(E_STRING, "FOR APPLICATION_TIME AS OF "+$5->Serialize());
    delete($5);
}
;
partition_restriction:
    PARTITION '(' intnum_list ')'
{
    std::string val = "PARTITION("+$3->text()+")";
    delete($3);
    $$ = Node::makeTerminalNode(E_STRING,val );
}
;
intnum_list:
    INTNUM
    | intnum_list ',' INTNUM
{
    $$ = Node::makeNonTerminalNode(E_SIMPLE_IDENT_LIST, E_LIST_PROPERTY_CNT, $1, $3);
    $$->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
;
tablesample_num:
    TABLESAMPLE BERNOULLI '(' INTNUM ')'
{
    std::string val = "TABLESAMPLE BERNOULLI("+$4->text()+")";
    delete($4);
    $$ = Node::makeTerminalNode(E_STRING,val );
}
    | TABLESAMPLE SYSTEM '(' INTNUM ')'
{
    std::string val = "TABLESAMPLE SYSTEM("+$4->text()+")";
    delete($4);
    $$ = Node::makeTerminalNode(E_STRING,val );
}
    | TABLESAMPLE '(' INTNUM ')'
{
    std::string val = "TABLESAMPLE("+$3->text()+")";
    delete($3);
    $$ = Node::makeTerminalNode(E_STRING,val );
}
;


%%
/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */