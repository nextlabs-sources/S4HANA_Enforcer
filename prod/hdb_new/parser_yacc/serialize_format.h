#ifndef SERIALIZE_FORMAT_H
#define SERIALIZE_FORMAT_H

#include <list>
#include <string>

/*
 * todo
 * XXX_SERIALIZE_FORMAT_C means compact format
 * XXX_SERIALIZE_FORMAT_B means beautify format
 *
 * */

/*
 * In XXX_SERIALIZE_FORMAT_C
 * FormatDetail has 2 types depend on is_simple,
 * if is_simple 1 then, it usually means a string literal(s0), key, s2 is ignored
 * if is_simple 0 then, it usually means a syntax node, key means the child index in it's parent, s0 means before (string literal), s1 means after (string literal)
 * */

/*
 * In XXX_SERIALIZE_FORMAT_B
 * Use '\n' in s0 or s1 to control beautify process (is_simple 0)
 * If is_simple 1 then '\n' only in s0.
 * */

struct FormatDetail
{
    char is_simple;
    const std::string s0;
    int key;                //means is child node index
    const std::string s2;
};
//add by jeff  output format
/*
 * ret
for(compact_ / beautify_) {
    if(is_simple) {
        ret += s0
    } else {
        ret +=  s0
        ret += childnode[key]
        ret += s2
    }
}
*/


typedef const std::list<FormatDetail> Form;
struct _FmCB {
    Form *compact_;
    Form *beautify_;
};
#define FmCB const _FmCB

Form OPTION_STRING_FORMAT_C {{0,"",0," "},{1,"= "},{0,"",1," "}};
FmCB OPTION_STRING_FORMAT   {&OPTION_STRING_FORMAT_C,&OPTION_STRING_FORMAT_C};
Form RETURNS_CLAUSE_FORMAT_C {{1," RETURNS "}, {0,"",0," "}};
FmCB RETURNS_CLAUSE_FORMAT  {&RETURNS_CLAUSE_FORMAT_C,&RETURNS_CLAUSE_FORMAT_C};
Form FOR_CLAUSE_FORMAT_C {{1,"FOR "},{0,"",0," "},{0,"",1," "},{0,"",3," "},{0,"",4," "}};
FmCB FOR_CLAUSE_FORMAT      {&FOR_CLAUSE_FORMAT_C,&FOR_CLAUSE_FORMAT_C};
    
Form WITHIN_GROUP_SF  {{0,"WITHIN GROUP(",0,")"}};
FmCB WITHIN_GROUP_SERIALIZE_FORMAT         {&WITHIN_GROUP_SF,&WITHIN_GROUP_SF};

Form TIME_TRAVEL_FORMAT_C  {{1,"AS OF "},{0,"",0," "},{0,"",1," "}};
FmCB TIME_TRAVEL_FORMAT {&TIME_TRAVEL_FORMAT_C,&TIME_TRAVEL_FORMAT_C};

Form TABLE_VAR_SF  {{0,":",0,""}};
FmCB TABLE_VAR_SERIALIZE_FORMAT         {&TABLE_VAR_SF,&TABLE_VAR_SF};
Form ASSOCIATED_TAB_SF {{0,"",0,""},{0,"[",1,"]"},{0,":",2," "}};//relation_factor '[' opt_search_condition ']' ':' associated_ref_list
FmCB ASSOCIATED_TAB_SERIALIZE_FORMAT    {&ASSOCIATED_TAB_SF,&ASSOCIATED_TAB_SF};
Form ASSOCIATED_LIST_SF {{0,"",0,""},{0,".",1," "}};
FmCB ASSOCIATED_LIST_SERIALIZE_FORMAT   {&ASSOCIATED_LIST_SF,&ASSOCIATED_LIST_SF};
Form ASSOCIATED_REF_SF {{0,"",0,""},{1,"["},{0,"",1," "},{0,"",2," "},{1,"]"}};//relation_factor '[' opt_search_condition opt_many2one_part ']'
FmCB ASSOCIATED_REF_SERIALIZE_FORMAT    {&ASSOCIATED_REF_SF,&ASSOCIATED_REF_SF};


Form LATERAL_QUERY_SERIALIZE_FORMAT_C {{1,"LATERAL"},{0,"(",0,") "}};
FmCB LATERAL_QUERY_SERIALIZE_FORMAT {&LATERAL_QUERY_SERIALIZE_FORMAT_C,&LATERAL_QUERY_SERIALIZE_FORMAT_C};

Form CASE_JOIN_RET_SF_C {{0,"RETURN",0," "}, {0,"FROM ",1," "},{0,"ON ",2," "}};//RETURN select_expr_list_parens FROM table_reference ON search_condition
FmCB CASE_JOIN_RET_SERIALIZE_FORMAT     {&CASE_JOIN_RET_SF_C,&CASE_JOIN_RET_SF_C};
Form CASE_JOIN_WHEN_SF_C {{0,"WHEN ",0," "}, {0,"THEN ",1," "}};//WHEN search_condition THEN ret_join_on
FmCB CASE_JOIN_WHEN_SERIALIZE_FORMAT    {&CASE_JOIN_WHEN_SF_C,&CASE_JOIN_WHEN_SF_C};
Form CASE_JOIN_ELSE_SF_C {{0,"ELSE ",0," "}};//ELSE ret_join_on
FmCB CASE_JOIN_ELSE_SERIALIZE_FORMAT    {&CASE_JOIN_ELSE_SF_C,&CASE_JOIN_ELSE_SF_C};
Form CASE_JOIN_SF_C {{0,"CASE JOIN ",0," "}, {0,"",1,""},{1," END "}};//CASE JOIN  case_join_when_list opt_case_join_else  END
FmCB CASE_JOIN_SERIALIZE_FORMAT         {&CASE_JOIN_SF_C,&CASE_JOIN_SF_C};

Form POINT_VALUE_FORMAT_C {{0," ",0," "},{1,"=>"}, {0," ",1,""}};
FmCB POINT_VALUE_FORMAT  {&POINT_VALUE_FORMAT_C, &POINT_VALUE_FORMAT_C};

Form UNNEST_TABLE_FORMAT_C {{0,"UNNEST(",0,")"} ,{0," ",1,""}};
FmCB UNNEST_TABLE_FORMAT  {&UNNEST_TABLE_FORMAT_C, &UNNEST_TABLE_FORMAT_C};
Form COLLECT_VAL_FORMAT_C {{0,"ARRAY(",0,")"} };
FmCB COLLECT_VAL_FORMAT  {&COLLECT_VAL_FORMAT_C, &COLLECT_VAL_FORMAT_C};
Form AS_DERIVED_PART_FORMAT_C {{0,"",0," "},{0,"",1,""},{0,"(",2,")"} };
FmCB AS_DERIVED_PART_FORMAT  {&AS_DERIVED_PART_FORMAT_C, &AS_DERIVED_PART_FORMAT_C};

Form AGG_ORDER_SERIALIZE_FORMAT_C {{0,"ORDER BY ",0," "}};
FmCB AGG_ORDER_SERIALIZE_FORMAT {&AGG_ORDER_SERIALIZE_FORMAT_C,&AGG_ORDER_SERIALIZE_FORMAT_C};

Form REPLACE_SERIALIZE_FORMAT_C     {{0,"REPLACE ",0," "},{0,"",1," "},{0,"",2," "}, {0,"",3," "}, {0,"",4," "}};
Form REPLACE_SERIALIZE_FORMAT_B     {{0,"REPLACE ",0," "},{0,"",1,""},{0,"",2,"\n"}, {0,"",3," "}, {0,"",4," "}};
FmCB REPLACE_SERIALIZE_FORMAT       {&REPLACE_SERIALIZE_FORMAT_C,&REPLACE_SERIALIZE_FORMAT_B};
Form UPSERT_SERIALIZE_FORMAT_C     {{0,"UPSERT ",0," "},{0,"",1,""},{0,"",2," "}, {0,"",3," "}, {0,"",4," "}, {0,"",5," "}};
Form UPSERT_SERIALIZE_FORMAT_B     {{0,"UPSERT ",0," "},{0,"",1," "},{0,"",2,"\n"}, {0,"",3," "}, {0,"",4," "}, {0,"",5," "}};
FmCB UPSERT_SERIALIZE_FORMAT       {&UPSERT_SERIALIZE_FORMAT_C,&UPSERT_SERIALIZE_FORMAT_B};

// select ...from  ...  ? AS "t_00" ("C_0" NVARCHAR(3), "C_1" NVARCHAR(25), "C_2" NVARCHAR(2), "C_3" NVARCHAR(3))
Form CONSTRUCT_FROM_TABLE_C {{0," ",0," AS"},{0," ",1," "},{0,"(",2,")"}};
Form CONSTRUCT_FROM_TABLE_B {{0," ",0," AS"},{0," ",1," \n"},{0,"(",2,")"}};
FmCB CONSTRUCT_FROM_TABLE_SERIALIZE_FORMAT  {&CONSTRUCT_FROM_TABLE_C, &CONSTRUCT_FROM_TABLE_B};

Form CONSTRUCT_COL_C {{0," ",0," "},{0,"",1," "}};
Form CONSTRUCT_COL_B {{0," ",0," "},{0,"",1," "}};
FmCB CONSTRUCT_COL_SERIALIZE_FORMAT  {&CONSTRUCT_COL_C, &CONSTRUCT_COL_B};

Form HINT_CLAUSE_FORMAT_C            {{0,"WITH HINT(",0,") "}};
Form HINT_CLAUSE_FORMAT_B            {{0,"WITH HINT(",0,") "}};
FmCB HINT_CLAUSE_FORMAT              {&HINT_CLAUSE_FORMAT_C, &HINT_CLAUSE_FORMAT_B};

Form FOR_UPDATE_FORMAT_C            {{0," ",0," "},{0,"",1," "},{0,"OF ",2," "}, {0,"",3," "}};
Form FOR_UPDATE_FORMAT_B            {{0," ",0," "},{0,"",1," "},{0,"OF ",2," "}, {0,"",3," "}};
FmCB FOR_UPDATE_FORMAT              {&FOR_UPDATE_FORMAT_C, &FOR_UPDATE_FORMAT_B};

Form FOR_UPDATE_FORMAT_HANA_B            {{1," FOR UPDATE "},{0,"",0," "},{0,"",1," "},{0,"",2," "}};
FmCB FOR_UPDATE_FORMAT_HANA              {&FOR_UPDATE_FORMAT_HANA_B, &FOR_UPDATE_FORMAT_HANA_B};

Form WAIT_TIME_FORMAT_C            {{0,"WAIT ",0," "}};
Form WAIT_TIME_FORMAT_B            {{0,"WAIT ",0," "}};
FmCB WAIT_TIME_FORMAT              {&WAIT_TIME_FORMAT_C, &WAIT_TIME_FORMAT_B};


Form TOP_CLAUSE_FORMAT_C            {{0,"TOP ",0," "}};
Form TOP_CLAUSE_FORMAT_B            {{0,"TOP ",0," "}};
FmCB TOP_CLAUSE_FORMAT              {&TOP_CLAUSE_FORMAT_C, &TOP_CLAUSE_FORMAT_B};

Form OFFSET_NUM_FORMAT_C            {{0,"OFFSET ",0," "}};
Form OFFSET_NUM_FORMAT_B            {{0,"OFFSET ",0," "}};
FmCB OFFSET_NUM_FORMAT              {&OFFSET_NUM_FORMAT_C, &OFFSET_NUM_FORMAT_B};
Form LIMIT_NUM_FORMAT_C            {{0,"LIMIT ",0,""}, {0," ",1," "}};
Form LIMIT_NUM_FORMAT_B            {{0,"LIMIT ",0,""}, {0," ",1," "}};
FmCB LIMIT_NUM_FORMAT              {&LIMIT_NUM_FORMAT_C, &LIMIT_NUM_FORMAT_B};

Form LIMIT_NUM_FORMAT_HANA_B            {{0,"LIMIT ",0," "}, {0,"",1," "}, {0,"",2," "}};
FmCB LIMIT_NUM_FORMAT_HANA              {&LIMIT_NUM_FORMAT_HANA_B, &LIMIT_NUM_FORMAT_HANA_B};

Form SEMICOLON_LIST_SERIALIZE_FORMAT_C        {{0,"",0,""}, {1,";\n"}, {0,"",1,""}};
Form SEMICOLON_LIST_SERIALIZE_FORMAT_B        {{0,"",0,""}, {1,"; "}, {0,"\n",1,""}};
FmCB SEMICOLON_LIST_SERIALIZE_FORMAT          {&SEMICOLON_LIST_SERIALIZE_FORMAT_C, &SEMICOLON_LIST_SERIALIZE_FORMAT_B};

Form SPACE_LIST_SERIALIZE_FORMAT_C            {{0,"",0,""}, {0," ",1,""}};
Form SPACE_LIST_SERIALIZE_FORMAT_B            {{0,"",0,""}, {0,"\n",1,""}};
FmCB SPACE_LIST_SERIALIZE_FORMAT              {&SPACE_LIST_SERIALIZE_FORMAT_C, &SPACE_LIST_SERIALIZE_FORMAT_B};

Form COMMA_LIST_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1,", "}, {0,"",1,""}};
Form COMMA_LIST_SERIALIZE_FORMAT_B            {{0,"",0,""}, {1,", "}, {0,"\n",1,""}};
FmCB COMMA_LIST_SERIALIZE_FORMAT              {&COMMA_LIST_SERIALIZE_FORMAT_C, &COMMA_LIST_SERIALIZE_FORMAT_B};

Form SELECT_SERIALIZE_FORMAT_C                {{0,"",16," "}, {1,"SELECT "},{0,"",13," "}, {0,"",0," "}, {0,"",15," "}, \
    {0,"",1," "}, {0,"",18," "}, {0,"",2," "}, {0,"",3," "}, {0,"",4," "}, {0,"",5," "}, {0,"",8," "}, {0,"",6," "},\
    {0,"",7," "}, {0,"",9," "}, {0,"",10," "}, {0,"",11," "}, {0,"",12," "}, {0,"",14," "}, {0,"",17,""}};
Form SELECT_SERIALIZE_FORMAT_B                {{0,"",16,"\n"}, {1,"SELECT "}, {0,"",13," "}, {0,"",0," "}, {0,"",15," "}, \
    {0,"",1," "}, {0,"",18," "}, {0,"\n",2," "}, {0,"\n",3," "}, {0,"\n",4," "}, {0,"\n",5," "}, {0,"",8," "}, {0,"",6," "},\
    {0,"",7," "}, {0,"",9," "}, {0,"\n",10," "}, {0,"",11," "}, {0,"\n",12," "}, {0,"",14," "}, {0,"",17,""}};
FmCB SELECT_SERIALIZE_FORMAT                  {&SELECT_SERIALIZE_FORMAT_C, &SELECT_SERIALIZE_FORMAT_B};

Form SELECT_DIRECT_SERIALIZE_FORMAT_C         {{0,"",0," "}, {0,"",1," "}, {0,"",2," "}, {0,"",3," "}, {0,"",4," "}};
Form SELECT_DIRECT_SERIALIZE_FORMAT_B         {{0,"",0,"\n"}, {0,"",1," "}, {0,"\n",2," "}, {0,"\n",3," "}, {0,"\n",4," "}};
FmCB SELECT_DIRECT_SERIALIZE_FORMAT           {&SELECT_DIRECT_SERIALIZE_FORMAT_C, &SELECT_DIRECT_SERIALIZE_FORMAT_B};


Form SINGLE_WITH_PARENS_SERIALIZE_FORMAT_C    {{1,"("}, {0,"",0,""}, {1,")"}};
/* SINGLE_WITH_PARENS_SERIALIZE_FORMAT_B */
FmCB SINGLE_WITH_PARENS_SERIALIZE_FORMAT      {&SINGLE_WITH_PARENS_SERIALIZE_FORMAT_C, &SINGLE_WITH_PARENS_SERIALIZE_FORMAT_C};

Form SELECT_UNION_SERIALIZE_FORMAT_C          {{0,"",8,""}, {1, " UNION "}, {0,"",7," "}, {0,"",9,""}};
Form SELECT_UNION_SERIALIZE_FORMAT_B          {{0,"",8,""}, {1, "\n",-1,"UNION "}, {0,"",7," "}, {0,"\n",9,""}};
FmCB SELECT_UNION_SERIALIZE_FORMAT            {&SELECT_UNION_SERIALIZE_FORMAT_C, &SELECT_UNION_SERIALIZE_FORMAT_B};

Form SELECT_INTERSECT_SERIALIZE_FORMAT_C      {{0,"",8,""}, {1, " INTERSECT "}, {0,"",7," "}, {0,"",9,""}};
Form SELECT_INTERSECT_SERIALIZE_FORMAT_B      {{0,"",8,""}, {1, "\n",-1,"INTERSECT "}, {0,"",7," "}, {0,"\n",9,""}};
FmCB SELECT_INTERSECT_SERIALIZE_FORMAT        {&SELECT_INTERSECT_SERIALIZE_FORMAT_C, &SELECT_INTERSECT_SERIALIZE_FORMAT_B};

Form SELECT_EXCEPT_SERIALIZE_FORMAT_C         {{0,"",8,""}, {0," ",6," "}, {0,"",7," "}, {0,"",9,""}};
Form SELECT_EXCEPT_SERIALIZE_FORMAT_B         {{0,"",8,""}, {0,"\n",6," "}, {0,"",7," "}, {0,"\n",9,""}};
FmCB SELECT_EXCEPT_SERIALIZE_FORMAT           {&SELECT_EXCEPT_SERIALIZE_FORMAT_C, &SELECT_EXCEPT_SERIALIZE_FORMAT_B};

Form WHERE_SERIALIZE_FORMAT_C                 {{1,"WHERE "}, {0,"",0,""}};
Form WHERE_SERIALIZE_FORMAT_B                 {{1,"WHERE  "}, {0,"",0,""}};
FmCB WHERE_SERIALIZE_FORMAT                   {&WHERE_SERIALIZE_FORMAT_C, &WHERE_SERIALIZE_FORMAT_B};

Form FROM_SERIALIZE_FORMAT_C                  {{1,"FROM "}, {0,"",0,""}};
Form FROM_SERIALIZE_FORMAT_B                  {{1,"FROM   "}, {0,"",0,""}};
FmCB FROM_SERIALIZE_FORMAT                    {&FROM_SERIALIZE_FORMAT_C, &FROM_SERIALIZE_FORMAT_B};

Form GROUP_BY_SERIALIZE_FORMAT_C              {{1,"GROUP BY "}, {0,"",1," "}, {0,"",0,""}};
/* GROUP_BY_SERIALIZE_FORMAT_B */
FmCB GROUP_BY_SERIALIZE_FORMAT                {&GROUP_BY_SERIALIZE_FORMAT_C, &GROUP_BY_SERIALIZE_FORMAT_C};

Form ORDER_BY_SERIALIZE_FORMAT_C              {{1,"ORDER BY "}, {0,"",0,""}, {0," ",1,""}};
/* ORDER_BY_SERIALIZE_FORMAT_B */
FmCB ORDER_BY_SERIALIZE_FORMAT                {&ORDER_BY_SERIALIZE_FORMAT_C, &ORDER_BY_SERIALIZE_FORMAT_C};

Form SORT_KEY_SERIALIZE_FORMAT_C              {{0,"",0,""}, {0," ",1,""}};
/* SORT_KEY_SERIALIZE_FORMAT_B */
FmCB SORT_KEY_SERIALIZE_FORMAT                {&SORT_KEY_SERIALIZE_FORMAT_C, &SORT_KEY_SERIALIZE_FORMAT_C};


Form HAVING_SERIALIZE_FORMAT_C                {{1,"HAVING "}, {0,"",0,""}};
/* HAVING_SERIALIZE_FORMAT_B */
FmCB HAVING_SERIALIZE_FORMAT                  {&HAVING_SERIALIZE_FORMAT_C, &HAVING_SERIALIZE_FORMAT_C};

Form SINGLE_SERIALIZE_FORMAT_C                {{0,"",0,""}};
/* SINGLE_SERIALIZE_FORMAT_B */
FmCB SINGLE_SERIALIZE_FORMAT                  {&SINGLE_SERIALIZE_FORMAT_C, &SINGLE_SERIALIZE_FORMAT_C};

Form DOUBLE_SERIALIZE_FORMAT_C                {{0,"",0," "}, {0,"",1,""}};
/* DOUBLE_SERIALIZE_FORMAT_B */
FmCB DOUBLE_SERIALIZE_FORMAT                  {&DOUBLE_SERIALIZE_FORMAT_C, &DOUBLE_SERIALIZE_FORMAT_C};

Form TRIBLE_SERIALIZE_FORMAT_C                {{0,"",0," "}, {0,"",1," "}, {0,"",2,""}};
/* TRIBLE_SERIALIZE_FORMAT_B */
FmCB TRIBLE_SERIALIZE_FORMAT                  {&TRIBLE_SERIALIZE_FORMAT_C, &TRIBLE_SERIALIZE_FORMAT_C};

/*
 * ORACLE only support table alias without keyword AS
 *
 * But, opt AS is supported by most DBMS.
 * */
Form AS_SERIALIZE_FORMAT_C                    {{0,"",0,""}, {0," ",4,""}, {0,/*" AS "*/ " " ,1,""}, {0," ",2,""}, {0," ",3,""}};
/* AS_SERIALIZE_FORMAT_B */
FmCB AS_SERIALIZE_FORMAT                      {&AS_SERIALIZE_FORMAT_C, &AS_SERIALIZE_FORMAT_C};

Form QUADRUPLE_SERIALIZE_FORMAT_C             {{0,"",0,""}, {0," ",1,""}, {0," ",2,""}, {0," ",3,""}};
/* QUADRUPLE_SERIALIZE_FORMAT_B */
FmCB QUADRUPLE_SERIALIZE_FORMAT               {&QUADRUPLE_SERIALIZE_FORMAT_C, &QUADRUPLE_SERIALIZE_FORMAT_C};

Form PENTA_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {0," ",1,""}, {0," ",2,""}, {0," ",3,""}, {0," ",4,""}};
/* PENTA_SERIALIZE_FORMAT_C */
FmCB PENTA_SERIALIZE_FORMAT                   {&PENTA_SERIALIZE_FORMAT_C, &PENTA_SERIALIZE_FORMAT_C};

Form PENTA_SERIALIZE_FORMAT_0_C               {{0,"",0,""}, {0," ",4,""}, {0," ",1,""}, {0," ",2,""}, {0," ",3,""}};
/* PENTA_SERIALIZE_FORMAT_0_B */
FmCB PENTA_SERIALIZE_FORMAT_0                 {&PENTA_SERIALIZE_FORMAT_0_C, &PENTA_SERIALIZE_FORMAT_0_C};

Form JOINED_TB_1_SERIALIZE_FORMAT_C           {{0,"",1," "}, {0,"",0,""}, {1," JOIN "}, {0,"",2,""}, {1," ON "}, {0,"",3,""}};
Form JOINED_TB_1_SERIALIZE_FORMAT_B           {{0,"",1," "}, {0,"\n",0," "}, {1,"JOIN "}, {0,"",2,""}, {1,"\n",-1,"ON "}, {0,"",3,""}};
FmCB JOINED_TB_1_SERIALIZE_FORMAT             {&JOINED_TB_1_SERIALIZE_FORMAT_C, &JOINED_TB_1_SERIALIZE_FORMAT_B};


Form JOINED_TB_USING_SERIALIZE_FORMAT_C       {{0,"",1," "}, {0,"",0,""}, {1," JOIN "}, {0,"",2,""}, {1," USING"}, {0,"",3,""}};
Form JOINED_TB_USING_SERIALIZE_FORMAT_B       {{0,"",1," "}, {0,"\n",0,""}, {1," JOIN "}, {0,"",2,""}, {1,"\n",-1,"USING"}, {0,"",3,""}};
FmCB JOINED_TB_USING_SERIALIZE_FORMAT         {&JOINED_TB_USING_SERIALIZE_FORMAT_C, &JOINED_TB_USING_SERIALIZE_FORMAT_B};

Form JOINED_TB_2_SERIALIZE_FORMAT_C           {{0,"",1," "}, {0,"",0,""}, {1," JOIN "}, {0,"",2,""}};
Form JOINED_TB_2_SERIALIZE_FORMAT_B           {{0,"",1," "}, {0,"\n",0,""}, {1," JOIN "}, {0,"",2,""}};
FmCB JOINED_TB_2_SERIALIZE_FORMAT             {&JOINED_TB_2_SERIALIZE_FORMAT_C, &JOINED_TB_2_SERIALIZE_FORMAT_B};

Form OP_EXISTS_SERIALIZE_FORMAT_C             {{1,"EXISTS "}, {0,"",0,""}};
/* OP_EXISTS_SERIALIZE_FORMAT_B */
FmCB OP_EXISTS_SERIALIZE_FORMAT               {&OP_EXISTS_SERIALIZE_FORMAT_C, &OP_EXISTS_SERIALIZE_FORMAT_C};

Form OP_POS_SERIALIZE_FORMAT_C                {{1,"+"}, {0,"",0,""}};
/* OP_POS_SERIALIZE_FORMAT_B */
FmCB OP_POS_SERIALIZE_FORMAT                  {&OP_POS_SERIALIZE_FORMAT_C, &OP_POS_SERIALIZE_FORMAT_C};

Form OP_NEG_SERIALIZE_FORMAT_C                {{1,"-"}, {0,"",0,""}};
/* OP_NEG_SERIALIZE_FORMAT_B */
FmCB OP_NEG_SERIALIZE_FORMAT                  {&OP_NEG_SERIALIZE_FORMAT_C, &OP_NEG_SERIALIZE_FORMAT_C};

Form OP_ADD_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," + "}, {0,"",1,""}};
/* OP_ADD_SERIALIZE_FORMAT_B */
FmCB OP_ADD_SERIALIZE_FORMAT                  {&OP_ADD_SERIALIZE_FORMAT_C, &OP_ADD_SERIALIZE_FORMAT_C};

Form OP_MINUS_SERIALIZE_FORMAT_C              {{0,"",0,""}, {1," - "}, {0,"",1,""}};
/* OP_MINUS_SERIALIZE_FORMAT_B */
FmCB OP_MINUS_SERIALIZE_FORMAT                {&OP_MINUS_SERIALIZE_FORMAT_C, &OP_MINUS_SERIALIZE_FORMAT_C};

Form OP_MUL_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," * "}, {0,"",1,""}};
/* OP_MUL_SERIALIZE_FORMAT_B */
FmCB OP_MUL_SERIALIZE_FORMAT                  {&OP_MUL_SERIALIZE_FORMAT_C, &OP_MUL_SERIALIZE_FORMAT_C};

Form OP_DIV_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," / "}, {0,"",1,""}};
/* OP_DIV_SERIALIZE_FORMAT_C */
FmCB OP_DIV_SERIALIZE_FORMAT                  {&OP_DIV_SERIALIZE_FORMAT_C, &OP_DIV_SERIALIZE_FORMAT_C};

Form OP_REM_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," % "}, {0,"",1,""}};
/* OP_REM_SERIALIZE_FORMAT_B */
FmCB OP_REM_SERIALIZE_FORMAT                  {&OP_REM_SERIALIZE_FORMAT_C, &OP_REM_SERIALIZE_FORMAT_C};

Form OP_POW_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," ^ "}, {0,"",1,""}};
/* OP_POW_SERIALIZE_FORMAT_C */
FmCB OP_POW_SERIALIZE_FORMAT                  {&OP_POW_SERIALIZE_FORMAT_C, &OP_POW_SERIALIZE_FORMAT_C};

Form OP_MOD_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," MOD "}, {0,"",1,""}};
/* OP_MOD_SERIALIZE_FORMAT_B */
FmCB OP_MOD_SERIALIZE_FORMAT                  {&OP_MOD_SERIALIZE_FORMAT_C, &OP_MOD_SERIALIZE_FORMAT_C};

Form OP_LE_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," <= "}, {0,"",1,""}};
/* OP_LE_SERIALIZE_FORMAT_B */
FmCB OP_LE_SERIALIZE_FORMAT                   {&OP_LE_SERIALIZE_FORMAT_C, &OP_LE_SERIALIZE_FORMAT_C};

Form OP_LT_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," < "}, {0,"",1,""}};
/* OP_LT_SERIALIZE_FORMAT_B */
FmCB OP_LT_SERIALIZE_FORMAT                   {&OP_LT_SERIALIZE_FORMAT_C, &OP_LT_SERIALIZE_FORMAT_C};

Form OP_EQ_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," = "}, {0,"",1,""}};
/* OP_EQ_SERIALIZE_FORMAT_B */
FmCB OP_EQ_SERIALIZE_FORMAT                   {&OP_EQ_SERIALIZE_FORMAT_C, &OP_EQ_SERIALIZE_FORMAT_C};

Form OP_GE_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," >= "}, {0,"",1,""}};
/* OP_GE_SERIALIZE_FORMAT_B */
FmCB OP_GE_SERIALIZE_FORMAT                   {&OP_GE_SERIALIZE_FORMAT_C, &OP_GE_SERIALIZE_FORMAT_C};

Form OP_GT_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," > "}, {0,"",1,""}};
/* OP_GT_SERIALIZE_FORMAT_B */
FmCB OP_GT_SERIALIZE_FORMAT                   {&OP_GT_SERIALIZE_FORMAT_C, &OP_GT_SERIALIZE_FORMAT_C};

Form OP_NE_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," <> "}, {0,"",1,""}};
/* OP_NE_SERIALIZE_FORMAT_B */
FmCB OP_NE_SERIALIZE_FORMAT                   {&OP_NE_SERIALIZE_FORMAT_C, &OP_NE_SERIALIZE_FORMAT_C};

Form OP_LE_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," <= ALL "}, {0,"",1,""}};
/* OP_LE_ALL_SERIALIZE_FORMAT_C */
FmCB OP_LE_ALL_SERIALIZE_FORMAT               {&OP_LE_ALL_SERIALIZE_FORMAT_C, &OP_LE_ALL_SERIALIZE_FORMAT_C};

Form OP_LT_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," < ALL "}, {0,"",1,""}};
/* OP_LT_ALL_SERIALIZE_FORMAT_B */
FmCB OP_LT_ALL_SERIALIZE_FORMAT               {&OP_LT_ALL_SERIALIZE_FORMAT_C, &OP_LT_ALL_SERIALIZE_FORMAT_C};

Form OP_EQ_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," = ALL "}, {0,"",1,""}};
/* OP_EQ_ALL_SERIALIZE_FORMAT_B */
FmCB OP_EQ_ALL_SERIALIZE_FORMAT               {&OP_EQ_ALL_SERIALIZE_FORMAT_C, &OP_EQ_ALL_SERIALIZE_FORMAT_C};

Form OP_GE_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," >= ALL "}, {0,"",1,""}};
/* OP_GE_ALL_SERIALIZE_FORMAT_B */
FmCB OP_GE_ALL_SERIALIZE_FORMAT               {&OP_GE_ALL_SERIALIZE_FORMAT_C, &OP_GE_ALL_SERIALIZE_FORMAT_C};

Form OP_GT_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," > ALL "}, {0,"",1,""}};
/* OP_GT_ALL_SERIALIZE_FORMAT_B */
FmCB OP_GT_ALL_SERIALIZE_FORMAT               {&OP_GT_ALL_SERIALIZE_FORMAT_C, &OP_GT_ALL_SERIALIZE_FORMAT_C};

Form OP_NE_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," <> ALL "}, {0,"",1,""}};
/* OP_NE_ALL_SERIALIZE_FORMAT_B */
FmCB OP_NE_ALL_SERIALIZE_FORMAT               {&OP_NE_ALL_SERIALIZE_FORMAT_C, &OP_NE_ALL_SERIALIZE_FORMAT_C};

Form OP_LE_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," <= SOME "}, {0,"",1,""}};
/* OP_LE_SOME_SERIALIZE_FORMAT_B */
FmCB OP_LE_SOME_SERIALIZE_FORMAT              {&OP_LE_SOME_SERIALIZE_FORMAT_C, &OP_LE_SOME_SERIALIZE_FORMAT_C};

Form OP_LT_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," < SOME "}, {0,"",1,""}};
/* OP_LT_SOME_SERIALIZE_FORMAT_B */
FmCB OP_LT_SOME_SERIALIZE_FORMAT              {&OP_LT_SOME_SERIALIZE_FORMAT_C, &OP_LT_SOME_SERIALIZE_FORMAT_C};

Form OP_EQ_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," = SOME "}, {0,"",1,""}};
/* OP_EQ_SOME_SERIALIZE_FORMAT_B */
FmCB OP_EQ_SOME_SERIALIZE_FORMAT              {&OP_EQ_SOME_SERIALIZE_FORMAT_C, &OP_EQ_SOME_SERIALIZE_FORMAT_C};

Form OP_GE_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," >= SOME "}, {0,"",1,""}};
/* OP_GE_SOME_SERIALIZE_FORMAT_B */
FmCB OP_GE_SOME_SERIALIZE_FORMAT              {&OP_GE_SOME_SERIALIZE_FORMAT_C, &OP_GE_SOME_SERIALIZE_FORMAT_C};

Form OP_GT_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," > SOME "}, {0,"",1,""}};
/* OP_GT_SOME_SERIALIZE_FORMAT_B */
FmCB OP_GT_SOME_SERIALIZE_FORMAT              {&OP_GT_SOME_SERIALIZE_FORMAT_C, &OP_GT_SOME_SERIALIZE_FORMAT_C};

Form OP_NE_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," <> SOME "}, {0,"",1,""}};
/* OP_NE_SOME_SERIALIZE_FORMAT_B */
FmCB OP_NE_SOME_SERIALIZE_FORMAT              {&OP_NE_SOME_SERIALIZE_FORMAT_C, &OP_NE_SOME_SERIALIZE_FORMAT_C};

Form OP_LE_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," <= ANY "}, {0,"",1,""}};
/* OP_LE_ANY_SERIALIZE_FORMAT_B */
FmCB OP_LE_ANY_SERIALIZE_FORMAT               {&OP_LE_ANY_SERIALIZE_FORMAT_C, &OP_LE_ANY_SERIALIZE_FORMAT_C};

Form OP_LT_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," < ANY "}, {0,"",1,""}};
/* OP_LT_ANY_SERIALIZE_FORMAT_B */
FmCB OP_LT_ANY_SERIALIZE_FORMAT               {&OP_LT_ANY_SERIALIZE_FORMAT_C, &OP_LT_ANY_SERIALIZE_FORMAT_C};

Form OP_EQ_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," = ANY "}, {0,"",1,""}};
/* OP_EQ_ANY_SERIALIZE_FORMAT_C */
FmCB OP_EQ_ANY_SERIALIZE_FORMAT               {&OP_EQ_ANY_SERIALIZE_FORMAT_C, &OP_EQ_ANY_SERIALIZE_FORMAT_C};

Form OP_GE_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," >= ANY "}, {0,"",1,""}};
/* OP_GE_ANY_SERIALIZE_FORMAT_B */
FmCB OP_GE_ANY_SERIALIZE_FORMAT               {&OP_GE_ANY_SERIALIZE_FORMAT_C, &OP_GE_ANY_SERIALIZE_FORMAT_C};

Form OP_GT_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," > ANY "}, {0,"",1,""}};
/* OP_GT_ANY_SERIALIZE_FORMAT_B */
FmCB OP_GT_ANY_SERIALIZE_FORMAT               {&OP_GT_ANY_SERIALIZE_FORMAT_C, &OP_GT_ANY_SERIALIZE_FORMAT_C};

Form OP_NE_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," <> ANY "}, {0,"",1,""}};
/* OP_NE_ANY_SERIALIZE_FORMAT_B */
FmCB OP_NE_ANY_SERIALIZE_FORMAT               {&OP_NE_ANY_SERIALIZE_FORMAT_C, &OP_NE_ANY_SERIALIZE_FORMAT_C};

Form OP_LIKE_SERIALIZE_FORMAT_C               {{0,"",0,""}, {1," LIKE "}, {0,"",1,""}, {0," ESCAPE ",2,""}};
/* OP_LIKE_SERIALIZE_FORMAT_B */
FmCB OP_LIKE_SERIALIZE_FORMAT                 {&OP_LIKE_SERIALIZE_FORMAT_C, &OP_LIKE_SERIALIZE_FORMAT_C};

Form OP_NOT_LIKE_SERIALIZE_FORMAT_C           {{0,"",0,""}, {1," NOT LIKE "}, {0,"",1,""}, {0," ESCAPE ",2,""}};
/* OP_NOT_LIKE_SERIALIZE_FORMAT_B */
FmCB OP_NOT_LIKE_SERIALIZE_FORMAT             {&OP_NOT_LIKE_SERIALIZE_FORMAT_C, &OP_NOT_LIKE_SERIALIZE_FORMAT_C};

Form OP_MEMBER_SERIALIZE_FORMAT_C             {{0,"",0," "}, {1," MEMBER OF "},{0,"",1," "}};
Form OP_NOT_MEMBER_SERIALIZE_FORMAT_C         {{0,"",0," "}, {1," NOT MEMBER OF "},{0,"",1," "}};
FmCB OP_MEMBER_SERIALIZE_FORMAT               {&OP_MEMBER_SERIALIZE_FORMAT_C,     &OP_MEMBER_SERIALIZE_FORMAT_C};
FmCB OP_NOT_MEMBER_SERIALIZE_FORMAT           {&OP_NOT_MEMBER_SERIALIZE_FORMAT_C, &OP_NOT_MEMBER_SERIALIZE_FORMAT_C};

Form OP_AND_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," AND "}, {0,"",1,""}};
Form OP_AND_SERIALIZE_FORMAT_B                {{0,"",0,""}, {1,"\n",-1,"AND "}, {0,"",1,""}};
FmCB OP_AND_SERIALIZE_FORMAT                  {&OP_AND_SERIALIZE_FORMAT_C, &OP_AND_SERIALIZE_FORMAT_B};

Form OP_OR_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," OR "}, {0,"",1,""}};
Form OP_OR_SERIALIZE_FORMAT_B                 {{0,"",0,""}, {1,"\n",-1," OR "}, {0,"",1,""}};
FmCB OP_OR_SERIALIZE_FORMAT                   {&OP_OR_SERIALIZE_FORMAT_C, &OP_OR_SERIALIZE_FORMAT_B};

Form OP_NOT_SERIALIZE_FORMAT_C                {{1,"NOT "}, {0,"",0,""}};
/* OP_NOT_SERIALIZE_FORMAT_B */
FmCB OP_NOT_SERIALIZE_FORMAT                  {&OP_NOT_SERIALIZE_FORMAT_C, &OP_NOT_SERIALIZE_FORMAT_C};

Form OP_IS_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," IS "}, {0,"",1,""}};
/* OP_IS_SERIALIZE_FORMAT_B */
FmCB OP_IS_SERIALIZE_FORMAT                   {&OP_IS_SERIALIZE_FORMAT_C, &OP_IS_SERIALIZE_FORMAT_C};

Form OP_IS_NOT_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," IS NOT "}, {0,"",1,""}};
/* OP_IS_NOT_SERIALIZE_FORMAT */
FmCB OP_IS_NOT_SERIALIZE_FORMAT               {&OP_IS_NOT_SERIALIZE_FORMAT_C, &OP_IS_NOT_SERIALIZE_FORMAT_C};

Form OP_BETWEEN_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," BETWEEN "}, {0,"",1,""}, {1," AND "}, {0,"",2,""}};
/* OP_BETWEEN_SERIALIZE_FORMAT_B */
FmCB OP_BETWEEN_SERIALIZE_FORMAT              {&OP_BETWEEN_SERIALIZE_FORMAT_C, &OP_BETWEEN_SERIALIZE_FORMAT_C};

Form OP_NOT_BETWEEN_SERIALIZE_FORMAT_C        {{0,"",0,""}, {1," NOT BETWEEN "}, {0,"",1,""}, {1," AND "}, {0,"",2,""}};
/* OP_NOT_BETWEEN_SERIALIZE_FORMAT_B */
FmCB OP_NOT_BETWEEN_SERIALIZE_FORMAT          {&OP_NOT_BETWEEN_SERIALIZE_FORMAT_C, &OP_NOT_BETWEEN_SERIALIZE_FORMAT_C};

Form OP_IN_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," IN "}, {0,"",1,""}};
/* OP_IN_SERIALIZE_FORMAT_B */
FmCB OP_IN_SERIALIZE_FORMAT                   {&OP_IN_SERIALIZE_FORMAT_C, &OP_IN_SERIALIZE_FORMAT_C};

Form OP_NOT_IN_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," NOT IN "}, {0,"",1,""}};
/* OP_NOT_IN_SERIALIZE_FORMAT_B */
FmCB OP_NOT_IN_SERIALIZE_FORMAT               {&OP_NOT_IN_SERIALIZE_FORMAT_C, &OP_NOT_IN_SERIALIZE_FORMAT_C};

Form OP_CNN_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," || "}, {0,"",1,""}};
/* OP_CNN_SERIALIZE_FORMAT_B */
FmCB OP_CNN_SERIALIZE_FORMAT                  {&OP_CNN_SERIALIZE_FORMAT_C, &OP_CNN_SERIALIZE_FORMAT_C};

Form CASE_SERIALIZE_FORMAT_C                  {{1,"CASE "}, {0,"",0," "}, {0,"",1,""}, {0," ",2,""}, {1," END"}};
Form CASE_SERIALIZE_FORMAT_B                  {{1,"CASE "}, {0,"",0," "}, {0,"\n",1,""}, {0,"\n",2,""}, {1,"\n",-1,"END"}};
FmCB CASE_SERIALIZE_FORMAT                    {&CASE_SERIALIZE_FORMAT_C, &CASE_SERIALIZE_FORMAT_B};

Form WHEN_SERIALIZE_FORMAT_C                  {{1,"  WHEN "}, {0,"",0,""}, {1," THEN "}, {0,"",1,""}};
/* WHEN_SERIALIZE_FORMAT_B */
FmCB WHEN_SERIALIZE_FORMAT                    {&WHEN_SERIALIZE_FORMAT_C, &WHEN_SERIALIZE_FORMAT_C};

Form ELSE_SERIALIZE_FORMAT_C                  {{1,"  ELSE "}, {0,"",0,""}};
/* ELSE_SERIALIZE_FORMAT_B */
FmCB ELSE_SERIALIZE_FORMAT                    {&ELSE_SERIALIZE_FORMAT_C, &ELSE_SERIALIZE_FORMAT_C};

Form FUN_CALL_1_OVER_SERIALIZE_FORMAT_C       {{0,"",0,""}, {1,"("}, {0,"",1,""}, {1,")"}, {0," ",2,""}};
/* FUN_CALL_1_OVER_SERIALIZE_FORMAT_B */
FmCB FUN_CALL_1_OVER_SERIALIZE_FORMAT         {&FUN_CALL_1_OVER_SERIALIZE_FORMAT_C, &FUN_CALL_1_OVER_SERIALIZE_FORMAT_C};

Form FUN_CALL_DIS_OVER_SERIALIZE_FORMAT_C     {{0,"",0,""}, {1,"("}, {0,"",3," "}, {0,"",1,""}, {1,")"}, {0," ",2,""}};
/* FUN_CALL_DIS_OVER_SERIALIZE_FORMAT_B */
FmCB FUN_CALL_DIS_OVER_SERIALIZE_FORMAT       {&FUN_CALL_DIS_OVER_SERIALIZE_FORMAT_C, &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT_C};

Form FUN_CALL_314_SERIALIZE_FORMAT_C     {{0,"",0,""}, {1,"("}, {0,"",3," "}, {0,"",1,""}, {0," ",4,""}, {1,")"}, {0," ",2,""}};
/* FUN_CALL_DIS_OVER_SERIALIZE_FORMAT_B */
FmCB FUN_CALL_314_SERIALIZE_FORMAT       {&FUN_CALL_314_SERIALIZE_FORMAT_C, &FUN_CALL_314_SERIALIZE_FORMAT_C};

Form FUN_CALL_AS_SERIALIZE_FORMAT_C           {{0,"",0,""}, {1,"("}, {0,"",1,""}, {1," AS "}, {0,"",4,""}, {1,")"}};
/* FUN_CALL_AS_SERIALIZE_FORMAT_B */
FmCB FUN_CALL_AS_SERIALIZE_FORMAT             {&FUN_CALL_AS_SERIALIZE_FORMAT_C, &FUN_CALL_AS_SERIALIZE_FORMAT_C};

Form FUN_CALL_USING_SERIALIZE_FORMAT_C        {{0,"",0,""}, {1,"("}, {0,"",1,""}, {1," USING "}, {0,"",4,""}, {1,")"}};
/* FUN_CALL_USING_SERIALIZE_FORMAT_B */
FmCB FUN_CALL_USING_SERIALIZE_FORMAT          {&FUN_CALL_USING_SERIALIZE_FORMAT_C, &FUN_CALL_USING_SERIALIZE_FORMAT_C};

Form WITH_CLAUSE_SERIALIZE_FORMAT_C           {{1,"WITH "}, {0,"",0,""}};
/* WITH_CLAUSE_SERIALIZE_FORMAT_B */
FmCB WITH_CLAUSE_SERIALIZE_FORMAT             {&WITH_CLAUSE_SERIALIZE_FORMAT_C, &WITH_CLAUSE_SERIALIZE_FORMAT_C};

Form WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT_C {{1,"WITH RECURSIVE "}, {0,"",0,""}};
/* WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT_B */
FmCB WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT   {&WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT_C, &WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT_C};

Form COMMON_TABLE_EXPR_SERIALIZE_FORMAT_C     {{0,"",0,""}, {0,"",1,""}, {1," AS "}, {0,"",2,""}};
Form COMMON_TABLE_EXPR_SERIALIZE_FORMAT_B     {{0,"",0,""}, {0,"",1,""}, {1,"\n",-1,"AS "}, {0,"",2,""}};
FmCB COMMON_TABLE_EXPR_SERIALIZE_FORMAT       {&COMMON_TABLE_EXPR_SERIALIZE_FORMAT_C, &COMMON_TABLE_EXPR_SERIALIZE_FORMAT_B};

Form TABLE_IDENT_SERIALIZE_FORMAT_1_C         {{0,"",1,""}, {1,"."}, {0,"",0,""}};
/* TABLE_IDENT_SERIALIZE_FORMAT_1_B */
FmCB TABLE_IDENT_SERIALIZE_FORMAT_1           {&TABLE_IDENT_SERIALIZE_FORMAT_1_C, &TABLE_IDENT_SERIALIZE_FORMAT_1_C};

Form TABLE_IDENT_SERIALIZE_FORMAT_2_C         {{0,"",2,""}, {1,"."}, {0,"",1,""}, {1,"."}, {0,"",0,""}};
/* TABLE_IDENT_SERIALIZE_FORMAT_2_B */
FmCB TABLE_IDENT_SERIALIZE_FORMAT_2             {&TABLE_IDENT_SERIALIZE_FORMAT_2_C, &TABLE_IDENT_SERIALIZE_FORMAT_2_C};

Form TABLE_IDENT_SERIALIZE_FORMAT_3_C         {{0,"",3,""}, {1,"."}, {0,"",2,""}, {1,"."}, {0,"",1,""}, {1,"."}, {0,"",0,""}};
/* TABLE_IDENT_SERIALIZE_FORMAT_3_B */
FmCB TABLE_IDENT_SERIALIZE_FORMAT_3           {&TABLE_IDENT_SERIALIZE_FORMAT_3_C, &TABLE_IDENT_SERIALIZE_FORMAT_3_C};

Form OP_NAME_FIELD_SERIALIZE_FORMAT_1_C       {{0,"",1,""}, {1,"."}, {0,"",0,""}};
/* OP_NAME_FIELD_SERIALIZE_FORMAT_1_B */
FmCB OP_NAME_FIELD_SERIALIZE_FORMAT_1         {&OP_NAME_FIELD_SERIALIZE_FORMAT_1_C, &OP_NAME_FIELD_SERIALIZE_FORMAT_1_C};

Form OP_NAME_FIELD_SERIALIZE_FORMAT_2_C       {{0,"",2,""}, {1,"."}, {0,"",1,""}, {1,"."}, {0,"",0,""}};
/* OP_NAME_FIELD_SERIALIZE_FORMAT_2_B */
FmCB OP_NAME_FIELD_SERIALIZE_FORMAT_2         {&OP_NAME_FIELD_SERIALIZE_FORMAT_2_C, &OP_NAME_FIELD_SERIALIZE_FORMAT_2_C};

Form OP_NAME_FIELD_SERIALIZE_FORMAT_3_C       {{0,"",3,""}, {1,"."}, {0,"",2,""}, {1,"."}, {0,"",1,""}, {1,"."}, {0,"",0,""}};
/* OP_NAME_FIELD_SERIALIZE_FORMAT_3_B */
FmCB OP_NAME_FIELD_SERIALIZE_FORMAT_3         {&OP_NAME_FIELD_SERIALIZE_FORMAT_3_C, &OP_NAME_FIELD_SERIALIZE_FORMAT_3_C};

Form OP_NAME_FIELD_SERIALIZE_FORMAT_4_C       {{0,"",4,""}, {1,"."}, {0,"",3,""}, {1,"."}, {0,"",2,""}, {1,"."}, {0,"",1,""}, {1,"."}, {0,"",0,""}};
/* OP_NAME_FIELD_SERIALIZE_FORMAT_4_B */
FmCB OP_NAME_FIELD_SERIALIZE_FORMAT_4         {&OP_NAME_FIELD_SERIALIZE_FORMAT_4_C, &OP_NAME_FIELD_SERIALIZE_FORMAT_4_C};

Form OVER_CLAUSE_SERIALIZE_FORMAT_C           {{1,"OVER("}, {0,"",0,""}, {0," PARTITION BY ",1,""}, {0," ",2,""}, {0," ",3,""}, {1, ")"}};
/* OVER_CLAUSE_SERIALIZE_FORMAT_B */
FmCB OVER_CLAUSE_SERIALIZE_FORMAT             {&OVER_CLAUSE_SERIALIZE_FORMAT_C, &OVER_CLAUSE_SERIALIZE_FORMAT_C};

Form TABLE_VALUE_CTOR_SERIALIZE_FORMAT_C      {{1,"VALUES "}, {0,"",0,""}};
/* TABLE_VALUE_CTOR_SERIALIZE_FORMAT_B */
FmCB TABLE_VALUE_CTOR_SERIALIZE_FORMAT        {&TABLE_VALUE_CTOR_SERIALIZE_FORMAT_C, &TABLE_VALUE_CTOR_SERIALIZE_FORMAT_C};

Form UPDATE_SERIALIZE_FORMAT_C                {{0,"",0," "}, {1,"UPDATE "}, {0,"",1," "}, {0,"",2," "}, {0,"",3," "}
        ,{1,"SET "}, {0,"",4," "}, {0,"",5," "}, {0,"",6," "}, {0,"",7," "}, {0,"",8,""}};
Form UPDATE_SERIALIZE_FORMAT_B                {{0,"",0,"\n"}, {1,"UPDATE "}, {0,"",1," "}, {0,"",2," "}, {0,"",3," "}
        ,{1,"\n",-1,"SET "}, {0,"",4," "}, {0,"",5," "}, {0,"",6," "}, {0,"\n",7," "}, {0,"",8,""}};
FmCB UPDATE_SERIALIZE_FORMAT                  {&UPDATE_SERIALIZE_FORMAT_C, &UPDATE_SERIALIZE_FORMAT_B};

Form DELETE_SERIALIZE_FORMAT_C                {{0,"",0," "}, {1, "DELETE "}, {0,"",1," "}, {0,"FROM ",2," "}, {0,"",3," "}
        , {0,"",4," "}, {0,"",5," "}, {0,"",6," "}, {0,"",7," "}};
Form DELETE_SERIALIZE_FORMAT_B                {{0,"",0," "}, {1, "DELETE "}, {0,"",1," "}, {0,"FROM ",2," "}, {0,"",3," "}
        , {0,"",4," "}, {0,"",5," "}, {0,"\n",6," "}, {0,"",7," "}};
FmCB DELETE_SERIALIZE_FORMAT                  {&DELETE_SERIALIZE_FORMAT_C, &DELETE_SERIALIZE_FORMAT_B};
Form DELETE_S_FORMAT_C2                {{0,"",0," "}, {1, "DELETE HISTORY "}, {0,"",1," "}, {0,"FROM ",2," "}, {0,"",3," "}
        , {0,"",4," "}, {0,"",5," "}, {0,"",6," "}, {0,"",7," "}};
Form DELETE_S_FORMAT_B2                {{0,"",0," "}, {1, "DELETE HISTORY "}, {0,"",1," "}, {0,"FROM ",2," "}, {0,"",3," "}
        , {0,"",4," "}, {0,"",5," "}, {0,"\n",6," "}, {0,"",7," "}};
FmCB DELETE_S_FORMAT                  {&DELETE_S_FORMAT_C2, &DELETE_S_FORMAT_B2};

Form INSERT_SERIALIZE_FORMAT_C                {{1,"INSERT INTO "}, {0,"",0," "}, {0, "",1,""}};
Form INSERT_SERIALIZE_FORMAT_B                {{1,"INSERT INTO "}, {0,"",0," "}, {0, "\n",1,""}};
FmCB INSERT_SERIALIZE_FORMAT                  {&INSERT_SERIALIZE_FORMAT_C, &INSERT_SERIALIZE_FORMAT_B};

Form CALL_SERIALIZE_FORMAT_C                  {{1, "CALL "}, {0,"",0,"("}, {0,"",1,""}, {1,")"},{0,"",2,""}};
/* CALL_SERIALIZE_FORMAT_B */
FmCB CALL_SERIALIZE_FORMAT                    {&CALL_SERIALIZE_FORMAT_C, &CALL_SERIALIZE_FORMAT_C};

Form CALL_SQL_SERVER_SERIALIZE_FORMAT_C       {{1, "{CALL "}, {0,"",0,"("}, {0,"",1,""}, {1,")}"}};
/* CALL_SQL_SERVER_SERIALIZE_FORMAT_B */
FmCB CALL_SQL_SERVER_SERIALIZE_FORMAT         {&CALL_SQL_SERVER_SERIALIZE_FORMAT_C, &CALL_SQL_SERVER_SERIALIZE_FORMAT_C};

Form SQL_ARGUMENT_SERIALIZE_FORMAT_C          {{0,"",0,""}, {0," AS ",1,""}};
/* SQL_ARGUMENT_SERIALIZE_FORMAT_B */
FmCB SQL_ARGUMENT_SERIALIZE_FORMAT            {&SQL_ARGUMENT_SERIALIZE_FORMAT_C, &SQL_ARGUMENT_SERIALIZE_FORMAT_C};

#endif
