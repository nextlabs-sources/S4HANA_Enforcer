//
// Created by jeff on 2020/4/24.
//

#include <QueryCloudAZExport.h>
#include "QueryCAZPolicyOpr.h"
#include "tool.h"
#include "assert.h"
#include "log.h"
#include "NXS4HConfig.h"
#include "PolicyExport.h"
#include "NxlDbDataMgr.h"
#include "../parser_yacc/parse_tool.h"
#include <regex>
#include "../parser_yacc/EMS4HAst.h"
#include "ViewManager.h"

std::string modification_field(const std::string & field, ConditionInfo & condinfo, std::set<std::string> & fields, bool dquote);

#define RES_TYPE_NAME "emdatabase"

bool is_digit(char c) { return '0' <= c && c <= '9'; }
bool is_underline(char c) { return '_' == c; }
bool is_letter(char c) { return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); }
bool is_percent(char c) { return '%' == c; }
bool is_end(const std::string& s, unsigned int iIndex) { return s.length() <= iIndex; }
bool is_double_quote(char c) { return '"' == c; }

int WriteQueryPCLog(int logLevel, const char *szLog){
    theLog->WriteLog(logLevel, szLog);
    return 1;
}

static std::set<std::string,CommonFun::IgnoreCaseCmp> g_reserved = { //sap reserved
        "AND", "ALL", "ALTER", "AS", "BEFORE", "BEGIN", "BOTH", "CASE", "CHAR", "CONDITION",
        "CONNECT", "CROSS", "CUBE", "NOT",
        "CURRENT_SCHEMA", "CURRENT_TIME", "CURRENT_TIMESTAMP", "CURRENT_TRANSACTION_ISOLATION_LEVEL",
        "CURRENT_USER", "CURRENT_UTCDATE", "CURRENT_UTCTIME", "CURRENT_UTCTIMESTAMP", "CURRVAL", "CURSOR", "DECLARE",
        "DEFERRED", "DISTINCT", "ELSE", "ELSEIF", "END", "EXCEPT", "EXCEPTION", "EXEC", "FALSE", "FOR", "FROM", "FULL",
        "GROUP", "HAVING", "IF", "IN", "INNER", "INOUT", "INTERSECT", "INTO", "IS", "JOIN", "LEADING", "LEFT", "LIMIT", "LOOP", "MINUS",
        "NATURAL", "NCHAR", "NEXTVAL", "NULL", "ON", "OR", "ORDER", "OUT", "PRIOR", "RETURN", "RETURNS", "REVERSE", "RIGHT", "ROLLUP", "ROWID",
        "SELECT", "SESSION_USER", "SET", "SQL", "START", "SYSUUID", "TABLESAMPLE", "TOP", "TRAILING", "TRUE", "UNION", "UNKNOWN", "USING",
        "UTCTIMESTAMP", "VALUES", "WHEN", "WHERE", "WHILE", "WITH", "CURRENT_CONNECTION", "CURRENT_DATE", "LIKE", "EXISTS", "TO_CHAR", "TO_NUMBER",
        "TRIM", "RTRIM", "LTRIM", "LOWER", "UPPER", "LENGTH", "LENGTHB", "SUBSTR", "SUBSTRING", "ASCII", "ASCIISTR", "CHR", "CONCAT", "INITCAP",
        "LPAD", "RPAD", "INSTR", "REPLACE", "REGEXP_INSTR", "REGEXP_REPLACE", "REGEXP_COUNT", "REGEXP_SUBSTR"
};

bool is_num(const std::string & ss){
    unsigned int pos = 0;
    if(ss.length() <= 0) return false;
    while(pos < ss.length()) {
        if(!is_digit(ss[pos])
           && '.' != ss[pos]
           && ss[0] != '-' )
            return false;
        pos++;
    }
    return true;
}

std::string parse_identifier(const std::string & str, unsigned int & pos) {
    std::string ret;
    if(str[pos] >= '0' && str[pos] <= '9') {
        //error;
    } else if(str[pos] == '"') {//"aaa"
        pos++;
        if(is_digit(str[pos]) ) {
            //error;
        } else {
            while(str[pos] != '\0') {
                if(str[pos] == '"' ) {
                    pos++;
                    if(str[pos] != '"'){
                        break;
                    }
                }
                ret += str[pos];
                pos++;
            }
        }
    } else if(is_underline(str[pos]) || is_letter(str[pos])  ){
        ret += str[pos];
        pos++;
        while(str[pos] != '\0') {
            if(is_underline(str[pos]) || is_letter(str[pos]) || is_digit(str[pos]) ) {
                ret += str[pos];
            } else if(str[pos] == '.') {
                break;
            } else {
                break;
            }
            pos++;
        }
    } else if(str[pos]  == '*') {
        ret += '*';/*support format: * */
    } else {
    }

    while(str[pos] != ',' && str[pos] != '.' && str[pos] != '\0') {
        pos++;
    }

    return  ret;
}

void parse_fields(const std::string & ss, std::set<std::string> & set_fields) {
    unsigned int ipos = 0;
    std::set<std::string> pset ;//= new std::set<std::string>;
    while(ipos < ss.length()) {
        if(ss[ipos] == ' '|| ss[ipos] == ',' ){
        } else {
            std::string scol = parse_identifier(ss, ipos);
            if(!scol.empty()) {
                pset.insert(scol);
            }
        }
        ipos++;
    }
    if(!pset.empty()) set_fields.insert(pset.begin(),pset.end());
}

bool is_write(char c){
    return c==' '||c=='\n'||c=='\t'||c=='\r'||c=='\f';
}
bool is_op(char c){
    return c=='>'||c=='<'||c=='='||c=='!' || c=='|';
}
bool is_paren(char c){
    return c=='(' || c==')' || c == '[' || c==']';
}

bool is_reserved(const std::string &src, std::string & f){
    auto itf = g_reserved.find(src);
    if(itf != g_reserved.end()) {
        f = *itf;
        return true;
    }
    return false;
}


std::string parser_identifier(const std::string &src, unsigned int & p, char c){
    bool bquote = (c=='`'||c=='"');
    if(bquote){
        p++;
    }
    size_t len = src.length();
    std::string ret;
    while(p < len){
        char ct = src[p];
        if(!bquote) { //

            if (is_write(ct) || is_op(ct) || is_paren(ct) || ct==','){
                break;
            }else if(is_underline(ct) || is_letter(ct) || ct=='-' || is_digit(ct)){

            } else {
                return "";

            }
        } else {
            if(ct == c && src[p-1] != '\\'){ /*todo zhuan yi*/
                p++;
                break;
            }
        }
        ret += ct;
        p++;
    }
    return ret;
}

bool parser_string(const std::string &src,unsigned int & p, char c, std::string& ret){
    if(src[p] != c){
        return false;
    }
        
    p++;
    size_t len = src.length();
    bool bend = false;
    while(p < len){
        if(src[p] == c && src[p-1] != '\\'){/*zhuan yi already*/
            p++;
            bend = true;
            break;
        }
        ret += src[p];
        p++;
    }
    if(!bend){
        return false;
    }
    return true;
}

std::string parser_num(const std::string &src,unsigned int & p, char /*c*/){
    size_t len = src.length();
    std::string ret;
    while(p < len){
        char ct = src[p];
        if(is_write(ct) || is_op(ct) || is_paren(ct) || ct==',' || ct == '\0')
            break;
        switch (ct){
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            case '-':
            case '+':
            case '.':
            case '%':
            case 'e':{
                ret += ct;
            }break;
            default:{
                //error;
                return "";
            }break;
        }
        p++;
    }
    return ret;
}

std::string get_qualify(const ConditionInfo* pInfo){
    std::string ret;

    ret = pInfo->tableName;
    if(!ret.empty()){
        ret+='.';
    }

    return  ret;
}

std::string error_opr(const std::string & src, const unsigned int p){
    theLog->WriteLog(log_warning, "policy obligation parser error (at:%d) (condition:%s)",p,src.c_str());
    return PARSE_CONDITION_ERROR_IGNORE_OB;
}

std::string get_user_attr(const std::string & str, ConditionInfo & condinfo, std::set<std::string>& setAttr){
    setAttr = condinfo._puser->GetAttr(str);
    if(setAttr.empty()){
        theLog->WriteLog(log_warning, "policy obligation parser error  (user attr:%s not find)", str.c_str());
        return PARSE_CONDITION_ERROR_IGNORE_OB;  //ignore the whole obligation
    }
    return PARSE_CONDITION_SUCCESS;
}

std::string parser_dollar_attributes(const std::string & dollar_attr, ConditionInfo & condinfo, std::set<std::string> & fields, char c){
    std::string ret;
    size_t ifdot = dollar_attr.find('.');
    std::string ifl;  // $table or $user
    std::string ifr; // field or attr
    if(ifdot!= std::string::npos){
        ifl = dollar_attr.substr(0,ifdot);
        ifr = dollar_attr.substr(ifdot+1);
    } else {
        if(c != '\0')  ret += c;
        ret += dollar_attr;
        if(c != '\0')  ret += c;
        return ret;
    }
    if(ifl.compare("$table") == 0  && c == '\0'){
        unsigned int tpos = 0;
        std::string ident = parser_identifier(ifr, tpos,ifr[0]=='"'?'"':'\0');
        std::string real_field =  modification_field(ident, condinfo,  fields,  ifr[0]=='"');
        ret += real_field;
    } else if(ifl.compare("$user") == 0){
        std::set<std::string> setAttr;
        std::string val = get_user_attr( ifr,  condinfo, setAttr);
        if(val.compare(PARSE_CONDITION_ERROR_IGNORE_OB) == 0){
            return PARSE_CONDITION_ERROR_IGNORE_OB;
        }
        for(auto itAttr=setAttr.begin(); itAttr!=setAttr.end(); itAttr++){
            if (itAttr!=setAttr.begin()){
                ret += ',';
            }
            if(c != '\0')  ret += c;
            ret += *itAttr;
            if(c != '\0')  ret += c;
        }
    } else {
        if(c != '\0')  ret += c;
        ret += dollar_attr;
        if(c != '\0')  ret += c;
        return ret;
    }
    return  ret;
}

std::string  parser_subquery(const std::string & src, ConditionInfo & condinfo, std::set<std::string> & fields, unsigned int & pos, bool b_paren_in){
    size_t len = src.length();
    // unsigned int start = pos;
    int parens = 0;
    bool b_break = false;
    std::string sql;
    while(pos < len){

        switch(src[pos]){
            case '$':{
                std::string dollar_attr ;
                bool dquote = false;
                while(pos<len){
                    if(dquote){
                        if(src[pos]=='"'){
                            dquote = false;
                        }
                    } else {
                        if(src[pos]=='"'){
                            dquote =true;
                        } else if(is_write(src[pos]) || is_op(src[pos]) || is_paren(src[pos]) || src[pos]==',' )
                            break;
                    }
                    dollar_attr += src[pos];
                    pos++;
                }
                std::string dollar_val = parser_dollar_attributes( dollar_attr,  condinfo,  fields,  '\0');
                if(dollar_val .compare(PARSE_CONDITION_ERROR_IGNORE_OB) == 0 ){
                    return PARSE_CONDITION_ERROR_IGNORE_OB;
                }
                sql += dollar_val;
                sql += ' ';

            } break;
            case '\'':{
                std::string str;
                bool bResult = parser_string(src, pos, '\'', str);
                if (bResult)  {
                    std::string dollar_val = parser_dollar_attributes( str,  condinfo,  fields,  '\'');
                    if(dollar_val .compare(PARSE_CONDITION_ERROR_IGNORE_OB) == 0 ){
                        return  PARSE_CONDITION_ERROR_IGNORE_OB;
                    } else {
                        sql += dollar_val;
                        sql += ' ';
                    }
                }
                else{
                    return error_opr(src, pos);
                }
            } break;
            /*case '"':{
                {
                    std::string ident = parser_identifier(src, pos,'"');
                    if(!ident.empty()){
                        sql += get_qualify(&condinfo);
                        sql += '"';
                        sql += ident;
                        sql += '"';
                    } else {
                        return error_opr(src,pos);
                    }
                }
                sql += ' ';
            }break;*/
            case '(':{
                parens++;
                sql += '(';
                pos++;
            }break;
            case ')':{
                if(parens>0){
                    parens--;
                    sql += ')';
                    pos++;
                } else if(b_paren_in){
                    b_break = true;
                }

            } break;
            default: {
                sql += src[pos];
                pos++;
            }break;
        }
        if(b_break){
            break;
        }
    }
    if(sql.length() > 0){
        S4HException exc;
        std::string out;
        if(!compress_sql(sql, out)){
            theLog->WriteLog(log_warning, "policy obligation parser error (at:%d)(sql:%s)(error:%s)",pos,sql.c_str(),out.c_str());
            return PARSE_CONDITION_ERROR_IGNORE_OB;  //ignore the whole obligation
        }
    }
    return sql;
}

std::string modification_field(const std::string & field, ConditionInfo & condinfo, std::set<std::string> & fields, bool dquote){
    //if the sql statement is about the view, and the conditoin is about the base table.
    //we must convert the filed from its table to its view
    std::string ret_field;
    std::string temp = field;
    if (condinfo._tbInfo->_viewTableInfo &&
        condinfo._tbInfo->_viewTableInfo->_viewsource) {
        auto ptrViewSource = condinfo._tbInfo->_viewTableInfo->_viewsource;
        std::vector<const ViewColumnSourceInfo*> vecViewColumnSourceInfo  = ptrViewSource->MatchSourceInfo("", condinfo.real_tbname, field);
        if (vecViewColumnSourceInfo.size()){
            const std::string& strViewColumn =  vecViewColumnSourceInfo[0]->ColumnName();
            theLog->WriteLog(log_info, "parse condition replace table column with view column. table(column):%s(%s), view(column):%s(%s)",
                             condinfo.real_tbname.c_str(), field.c_str(), condinfo._tbInfo->_viewTableInfo->_tb.c_str(),  strViewColumn.c_str() );
            temp = strViewColumn;
        }
    }

    ret_field += get_qualify(&condinfo);
    if(dquote) ret_field += '"';
    ret_field += temp;
    if(dquote) ret_field += '"';
    fields.insert(temp);
    return ret_field;
}

std::string parser_condition(const std::string & src, ConditionInfo & condinfo, std::set<std::string> & fields) {
    std::string ret;
    int i_parens=0;//Parentheses must exist in pairs
    std::string qualify = condinfo.tableName;
    unsigned int p = 0;
    size_t len = src.length();
    while(p < len) {
        switch(src[p]) {
            case '$':{//$user.attr
                std::string dollar_attr ;
                bool dquote = false;
                while(p<len){
                    if(dquote){
                        if(src[p]=='"'){
                            dquote = false;
                        }
                    } else {
                        if(src[p]=='"'){
                            dquote =true;
                        } else if(is_write(src[p]) || is_op(src[p]) || is_paren(src[p]) || src[p]==',' )
                            break;
                    }
                    dollar_attr += src[p];
                    p++;
                }
                std::string dollar_val = parser_dollar_attributes( dollar_attr,  condinfo,  fields,  '\0');
                if(dollar_val .compare(PARSE_CONDITION_ERROR_IGNORE_OB) == 0 ){
                    return PARSE_CONDITION_ERROR_IGNORE_OB;
                }
                ret += dollar_val;
                ret += ' ';

            }break;
            case '>': {
                if(src[p+1]=='='){
                    ret += ">=";
                    p += 2;

                } else {
                    ret += '>';
                    p++;
                }
            } break;
            case '<':{
                if(src[p+1]=='='){
                    ret += "<=";
                    p+=2;
                } else {
                    ret += '<';
                    p++;
                }
            } break;
            case '!':{
                if(src[p+1]=='='){
                    ret += "<>";
                    p += 2;
                } else {
                    return error_opr(src,p);
                }
            } break;
            case '=':{
                ret += '=';
                p++;
            } break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            case '-':
            case '+':
            case '%':{
                std::string str = parser_num(src, p, '\0');
                if(str.empty()){
                    return error_opr(src,p);
                } else {
                    ret += str;
                    ret += ' ';
                }

            }break;
            case ' ':
            case '\n':
            case '\r':
            case '\t':
            case '\f':{
                p++;
                ret += ' ';
            }break;

            case '"':{
                {
                    std::string ident = parser_identifier(src, p,'"');
                    if(!ident.empty()){
                        std::string real_field =  modification_field(ident, condinfo,  fields,  true);
                        ret += real_field;
                    } else {
                        return error_opr(src,p);
                    }
                }
                ret += ' ';
            }break;
            case '\'':{
               std::string str;
               bool bResult = parser_string(src, p, '\'', str);
               if (bResult){
                   std::string dollar_val = parser_dollar_attributes( str,  condinfo,  fields,  '\'');
                   if(dollar_val.compare(PARSE_CONDITION_ERROR_IGNORE_OB) == 0 ){
                       return  PARSE_CONDITION_ERROR_IGNORE_OB;
                   } else {
                       ret += dollar_val;
                       ret += ' ';
                   }
                }
                else{
                    return error_opr(src, p);
                }
            }break;
            case '(':{//
                ret += src[p];
                p++;
                i_parens++;
            }break;
            case ')':{//
                if(i_parens<=0)
                    return error_opr(src,p);
                ret += src[p];
                p++;
                i_parens--;// = false;
            }break;
            case ',':{//
                if(i_parens<=0)
                    return error_opr(src,p);
                ret += src[p];
                p++;
            }break;
            default:{
                if(is_underline(src[p]) || is_letter(src[p])){
                    unsigned int pos_subquery = p;
                    std::string ident = parser_identifier(src, p, '\0');
                    if(!ident.empty()){
                        std::string reserved;
                        if(is_reserved(ident, reserved)){
                            if(reserved.compare("SELECT") == 0){
                                std::string sql = parser_subquery(src, condinfo, fields, pos_subquery, i_parens>0);
                                if(sql.compare(PARSE_CONDITION_ERROR_IGNORE_OB)==0){
                                    return PARSE_CONDITION_ERROR_IGNORE_OB;
                                }
                                p = pos_subquery;
                                ret += sql;
                            } else {
                                ret += reserved;
                                ret += ' ';
                            }
                        } else {
                            std::string real_field =  modification_field(ident, condinfo,  fields,  false);
                            ret += real_field;
                        }
                    } else {
                        return error_opr(src,p);
                    }
                } else {
                    return error_opr(src,p);
                }
            }break;
        }
    }

    if(i_parens>0){
        theLog->WriteLog(log_warning, "policy obligation parser error (Parentheses must exist in pairs), src=%s", src.c_str() );
        //printf("condition error:(Parentheses must exist in pairs)");
        return "";
    }

    //replace variable
    if (ret.find("$system.client")!=std::string::npos){
        std::regex reg_pattern("\\$system.client", std::regex_constants::ECMAScript | std::regex_constants::icase);
        ret = std::regex_replace(ret, reg_pattern, condinfo._puser->_strMANDT);
        theLog->WriteLog(log_info, "parser_condition replace system.client result:%s", ret.c_str() );
    }
    
    return ret;
}

#if 0
EMOperator EMCondition::TransformOp(const std::string & sop, S4HException & e) {
    if(sop.length() == 1) {
        if(sop[0] == '=') {
            return EMOP_EQ;
        } else if (sop[0] == '>') {
            return EMOP_GT;
        } else if (sop[0] == '<') {
            return EMOP_LT;
        }
    } else if (sop.length() == 2) {
        if (sop[0] == '>' && sop[1] == '=') {
            return EMOP_GE;
        } else if (sop[0] == '<' && sop[1] == '=') {
            return EMOP_LE;
        } else if (sop[0] == '!' && sop[1] == '=') {
            return  EMOP_NEQ;
        }else if (sop[0] == '<' && sop[1] == '>') {
            return  EMOP_NEQ;
        } else if(CommonFun::StrCaseCmp(sop.c_str(),"IN") == 0) {
            return  EMOP_IN;
        }
    } else if(sop.length() == 4) {
        if(CommonFun::StrCaseCmp(sop.c_str(),"LIKE") == 0){
            return  EMOP_LIKE;
        }
    } else if(sop.length() > 4) {
        if(sop.find("LIKE") > 0){
            if(sop.find("NOT") >= 0 ) {
                return EMOP_NOT_LIKE;
            } else {
                return  EMOP_LIKE;
            }
        }

    }
    if(!sop.empty())
        e.SetError(S4H_SQL_QUERY_PC_FAIL, "obligation hav nonsupport operation");
    return EMOP_EQ;
}
#endif

std::string EMCondition::Serialize(std::vector<EMCondition*> & /*conds*/, ConditionInfo & /*condinfo*/){
    /*todo*/
    return "";
}

EnforcerPcResult::~EnforcerPcResult(){
    for(auto it: _mask_map){
        for(auto itsub: *(it.second)){
            delete(itsub.second);
        }
        it.second->clear();
        delete(it.second);
    }
}
/*
QueryPcResult * ParserObligation(IPolicyResult* result, std::string & action, ConditionInfo & condinfo, SqlException & e){
    int icount = result->ObligationCount();
    for (int i = 0; i < icount; ++i)
    {
        const IObligation* pobg = result->GetObligationByIndex(i);
        const char* pname = pobg->GetObligationName();
        printf("obg_name:%s\n", pname);
        const IAttributes* pattr = pobg->GetAttributes();
        int icout = pattr->Count();
        for (int j = 0 ; j < icout; ++j)
        {
            const char * sname = nullptr;
            const char * svalue = nullptr;

            CEAttributeType tp = XACML_string;
            pattr->GetAttrByIndex(j, &sname, &svalue, &tp);

            std::string strname(sname);
            std::string strvalue(svalue);
            //policy_model_id
            if (strvalue.empty()) continue;

            printf("obg:(%s, %s)\n", sname, svalue);
        }
    }
}

QueryPcResult * QueryPc(std::string & action, EMDictionary & user, EMDictionary & resource, ConditionInfo & condinfo, SqlException & e){
    QueryPcResult * pret = NULL;
    IPolicyRequest* pRequest = CreatePolicyRequest();
    //action necessary
    pRequest->SetAction(action.c_str());

    //user attr  necessary
    IAttributes* pUser = CreateCEAttr();
    for(auto it: user){
        if(it.first.empty() || it.second.empty()) continue;
        pUser->AddAttribute(it.first.c_str(), it.second.c_str(), XACML_string);
    }
    pRequest->SetUserInfo("user", "User", pUser);

    // Set source info, this is mandatory
    IAttributes* pSourceAttr = CreateCEAttr();
    for(auto it: user){
        if(it.first.empty() || it.second.empty()) continue;
        pSourceAttr->AddAttribute(it.first.c_str(), it.second.c_str(), XACML_string);
    }
    pRequest->SetSource("EMDB", "EMDATABASE", pSourceAttr);

    //host info necessary
    pRequest->SetHostInfo("HostName", "10.23.60.102", NULL);

    const IPolicyRequest* request[] = { pRequest };
    IPolicyResult* result[sizeof(request) / sizeof(request[0])] = { 0 };
    while (true)
    {
        IPolicyResult* result = NULL;
        QueryStatus qs0 = CheckSingleResource(pRequest, &result);
        if (QS_S_OK != qs0 || result == NULL) {
            e.SetInfo(ERR_POLICY, "no match policy");
            return NULL;
        }
        //QueryStatus qs1 = CheckMultiResource(&request[0], sizeof(request)/sizeof(request[0]), result);
        if (result)
        {
            pret = ParserObligation(result, action, condinfo, e);
            FreePolicyResult(result);
        }
        break;
    }
    return pret;
}
*/

bool is_separator_index(const std::string & src, size_t pos, const std::string & sep){
    size_t len = sep.length();
    if(sep.empty()) return false;
    if(src.length() < len+pos) return false;
    for(size_t i = 0; i < len; i++){
        if(src[pos+i] != sep[i]) return false;
    }
    return true;
}

void parser_string2array(std::vector<std::string> &vec, const std::string & src, const std::string & sep){
    size_t len = src.length();
    size_t p = 0;
    size_t last = 0;
    while(p < len){
        if(is_separator_index(src,p,sep)){
            vec.push_back(src.substr(last, p-last));
            p+=sep.length();
            last = p;
        } else {
            p++;
        }
    }
    vec.push_back(src.substr(last));
}

IPolicyRequest * create_request(const std::string & action, EMResDictionary & user, const std::string& resourceID,
                                EMResDictionary & resource, const std::string & user_name){
    IPolicyRequest * ret = CreatePolicyRequest();
    //action necessary
    ret->SetAction(action.c_str());

    //user attr  necessary
    IAttributes* puser = NULL;//
    std::string uname = user_name;
    std::string uid = user_name;
    for(auto it: user){
        if(it.first.empty() || it.second.strValue.empty()) 
            continue;
        if(puser ==NULL)
            puser = CreateCEAttr();
        puser->AddAttribute(it.first.c_str(), it.second.strValue.c_str(), it.second.attrType);
//        if(it.second.attrType == XACML_ArrStr){
//            std::string separtor = (*theConfig)[USER_val_separator];
//            std::vector<std::string> vec;
//            parser_string2array(vec,it.second.strValue,separtor);
//            for(auto val:vec){
//                if(val.empty()) continue;
//                pUser->AddAttribute(it.first.c_str(), val.c_str(), XACML_string);
//            }
//        } else if(it.second.attrType == XACML_ArrInt){
//            std::string separtor = (*theConfig)[USER_val_separator];
//            std::vector<std::string> vec;
//            parser_string2array(vec,it.second.strValue,separtor);
//            for(auto val:vec){
//                if(val.empty()) continue;
//                pUser->AddAttribute(it.first.c_str(), val.c_str(), XACML_int);
//            }
//        }else{
//            pUser->AddAttribute(it.first.c_str(), it.second.strValue.c_str(), it.second.attrType);
//        }
        if(CommonFun::StrCaseCmp("id",it.first.c_str()) == 0){
            uid = it.second.strValue;
        } else if(CommonFun::StrCaseCmp("name",it.first.c_str()) == 0){
            uname = it.second.strValue;
        }
    }

    ret->SetUserInfo(uid.c_str(), uname.c_str(), puser);

    // Set source info, this is mandatory
    IAttributes* psource = NULL;
    for(auto it: resource){
        if(it.first.empty() || it.second.strValue.empty()) 
            continue;
        if(psource == NULL) 
            psource = CreateCEAttr();
        psource->AddAttribute(it.first.c_str(), it.second.strValue.c_str(), it.second.attrType);
    }

    ret->SetSource(resourceID.c_str(), g_conf->GetPolicyModelName().c_str(), psource);

    //host info necessary
    ret->SetHostInfo("HostName", "1.1.1.1", NULL);
    return ret;
}
QueryStatus  single_query_pc(const IPolicyRequest* pcRequest, IPolicyResult** pcResult){
    return CheckSingleResource(pcRequest, pcResult);
}
QueryStatus  multi_query_pc (const IPolicyRequest** pcRequest, int nRequestCount, IPolicyResult** pcResult){
    return  CheckMultiResource(pcRequest,nRequestCount, pcResult);
}

void free_pc_request(IPolicyRequest* p){
    if(NULL!=p->GetUserAttr()){
        FreeCEAttr(const_cast<IAttributes*>(p->GetUserAttr()));
    }
    if(NULL!=p->GetSourceAttr()){
        FreeCEAttr(const_cast<IAttributes*>(p->GetSourceAttr()));
    }
    FreePolicyRequest(p);
}
void free_pc_result(IPolicyResult* p){
    FreePolicyResult(p);
}
void free_pc_result(std::vector<QueryPcResult *> & vp){
    for (auto it:vp) {
        delete(it);
        //it=NULL;
    }
    vp.clear();
}

void parser_result_2_mask_condition(DenyUpdateTableMap & map_dup, MaskConditionMap & map_in, 
                                std::list<PredicateItem>& lstPredicItems, IPolicyResult * result, ConditionInfo & condinfo, S4HException & /*e*/){
    //DenyUpdateTableMap map_dup;
    if(result ==NULL)
        return ;

    std::string action = condinfo._action;
    //if(CommonFun::StrCaseCmp( action.c_str() ,EM_ACTION_SELECT) == 0 )
    size_t icount = result->ObligationCount();
    if(icount == 0){
        //ret->_filter_cond = cond;
    } else {
        for (int i = 0; i < icount; ++i)
        {
            const IObligation* pobg = result->GetObligationByIndex(i);
            const char * pname = pobg->GetObligationName();
            std::string obname(pobg->GetObligationName());
            //printf("oblgation_name:%s\n", obname.c_str());
            const IAttributes* pattr = pobg->GetAttributes();

            if(CommonFun::StrCaseCmp(pname, "data_masking") == 0){
                size_t ic = pattr->Count();
                //assert(ic==8);

                //std::string table;
                std::set<std::string> tables;
                std::set<std::string> excludeTables;
                std::set<std::string> fields;
                std::set<std::string> cond_fields;
                std::string mask_symbol;
                std::string mask_format;
                std::string mask_cond1;
                std::string mask_cond2;
                std::string mask_cond3;

                for (int j = 0; j < ic; ++j)
                {
                    const char * sname = nullptr;
                    const char * svalue = nullptr;

                    CEAttributeType tp = XACML_string;
                    pattr->GetAttrByIndex(j, &sname, &svalue, &tp);

                    std::string strname(sname);
                    std::string strvalue(svalue);
                    //MaskConditionMap map;

                        if (strvalue.empty()) continue;
                        if (strname.compare("mask_tables") == 0) {//Mask Tables	mask_tables	Single Row
                            #if SUPPORT_EXCLUDE_MASKTABLE
                            if (strvalue.find("- ")==0){//"- tablea,tableb" means exclude these table from all table "*"
                                tables.insert(ALL_TABLE_SYMBOLS);
                                std::string strRealValue = strvalue.substr(2);
                                parse_fields(strRealValue, excludeTables);
                            }
                            else
                            #endif
                            {
                                parse_fields(strvalue, tables);
                            }
                        }else if (strname.compare("mask_fields") == 0) {//Mask Fields	mask_fields	Single Row
                            parse_fields(strvalue, fields);
                        }else if (strname.compare("mask_format") == 0) {//Mask Format	mask_format	Single Row		[ms]
                            mask_format = strvalue;/* todo support */
                        }else if (strname.compare("type") == 0) {//Mask Format	mask_format	Single Row		[ms]
                            //todo
                        }else if (strname.compare("mask_symbol") == 0) {//Mask Symbol	mask_symbol	Single Row
                            mask_symbol = strvalue;
                        }else if (strname.compare("mask_cond1") == 0) {//Mask Condition	mask_cond1	Single Row
                            mask_cond1 = parser_condition(strvalue, condinfo, cond_fields);
                            if (mask_cond1.compare(PARSE_CONDITION_ERROR_IGNORE_OB)==0){
                                tables.clear();
                                theLog->WriteLog(log_error, "parse mask conditoin failed for ignore this obligation:%s", strvalue.c_str());
                                break;
                            }
                            
                        }else if (strname.compare("mask_cond2") == 0) {//OR Mask Condition	mask_cond2	Single Row
                            mask_cond2 = parser_condition(strvalue, condinfo, cond_fields);
                            if (mask_cond2.compare(PARSE_CONDITION_ERROR_IGNORE_OB)==0){
                                tables.clear(); //clear tables to make the whole obligation been ignored.
                                theLog->WriteLog(log_error, "parse mask conditoin failed for ignore this obligation:%s", strvalue.c_str());
                                break;
                             }
                        }else if (strname.compare("mask_cond3") == 0) {//OR Mask Condition	mask_cond3	Single Row
                            mask_cond3 = parser_condition(strvalue, condinfo, cond_fields);
                            if (mask_cond3.compare(PARSE_CONDITION_ERROR_IGNORE_OB)==0){
                                tables.clear();
                                theLog->WriteLog(log_error, "parse mask conditoin failed for ignore this obligation:%s", strvalue.c_str());
                                break;
                             }
                        }
                    }
                    if((tables.size()==0) || (tables.find(condinfo.real_tbname)==tables.end()) ){
                        continue;
                    }
                    if(fields.size() == 0){
                        continue;
                    }
                    
                    NxlDBDataMgr* pNxlDBMgr =  NxlDBDataMgr::Instance();
                    if ((CommonFun::StrCaseCmp(mask_format, pNxlDBMgr->KeyMaskMaskFormat() )!=0)  && mask_symbol.empty()){
                        continue;
                    }
                    
                    auto itTable = tables.find(condinfo.real_tbname);
                    if(itTable!=tables.end()){
                        const std::string& tb= *itTable;
                        auto it = map_in.find(tb);
                        if(it == map_in.end()){
                            MaskItemMap *sub = new MaskItemMap();
                            for(auto field:fields){
                                MaskItem * item = new MaskItem();
                                item->_table = tb;
                                item->_cond_fields.insert(cond_fields.begin(), cond_fields.end());
                                item->_condition = mask_cond1;
                                if(!mask_cond2.empty()){
                                    if( item->_condition.empty()){
                                        item->_condition = mask_cond2;
                                    } else {
                                        item->_condition += " OR ";
                                        item->_condition += mask_cond2;
                                    }
                                }
                                if(!mask_cond3.empty()){
                                    if( item->_condition.empty()){
                                        item->_condition = mask_cond3;
                                    } else {
                                        item->_condition += " OR ";
                                        item->_condition += mask_cond3;
                                    }
                                }
                                item->_format = MaskItem::GetMaskFormat( mask_format);
                                item->_symbols = mask_symbol;
                                item->_field = field;

                                #if SUPPORT_EXCLUDE_MASKTABLE
                                item->_excludeTable = excludeTables;
                                #endif

                                sub->insert(std::make_pair(field, item));
                            }
                            map_in.insert(std::make_pair(tb, sub));
                        } else {
                            MaskItemMap *sub = it->second;
                            for(auto field:fields){
                                auto itsub = sub->find(field);
                                if(itsub == sub->end()){
                                    MaskItem * item = new MaskItem();
                                    item->_table = tb;
                                    item->_cond_fields.insert(cond_fields.begin(), cond_fields.end());
                                    item->_condition = mask_cond1;
                                    if(!mask_cond2.empty()){
                                        if( item->_condition.empty()){
                                            item->_condition = mask_cond2;
                                        } else {
                                            item->_condition += " OR ";
                                            item->_condition += mask_cond2;
                                        }
                                    }
                                    if(!mask_cond3.empty()){
                                        if( item->_condition.empty()){
                                            item->_condition = mask_cond3;
                                        } else {
                                            item->_condition += " OR ";
                                            item->_condition += mask_cond3;
                                        }
                                    }
                                    item->_format = MaskItem::GetMaskFormat( mask_format);
                                    item->_symbols = mask_symbol;
                                    item->_field = field;
                                    #if SUPPORT_EXCLUDE_MASKTABLE
                                    item->_excludeTable = excludeTables;
                                    #endif 
                                    sub->insert(std::make_pair(field, item));

                                } else {
                                    //ignore
                                }
                            }
                        }
                    }

                } else if(CommonFun::StrCaseCmp(pname, "predicate_condition") == 0) {
                    size_t icout = pattr->Count();
                    assert(icout==2);

                    for (int j = 0; j < icout; ++j)
                    {
                        const char * sname = nullptr;
                        const char * svalue = nullptr;

                        CEAttributeType tp = XACML_string;
                        pattr->GetAttrByIndex(j, &sname, &svalue, &tp);

                        std::string strname(sname);
                        std::string strvalue(svalue);

                        if (strvalue.empty()) continue;
                        if (strname.compare("condition") == 0) {
                            //filter condition
                            std::set<std::string> cond_fields;

                            std::string temp_cond = parser_condition(strvalue, condinfo, cond_fields);
                            if (temp_cond.compare(PARSE_CONDITION_ERROR_IGNORE_OB)==0){
                                temp_cond.clear();
                                theLog->WriteLog(log_error, "parse mask conditoin failed for ignore this obligation:%s", strvalue.c_str());
                            }

                            if(!temp_cond.empty()) {

                                PredicateItem predicItem;
                                predicItem._condition = temp_cond;
                                predicItem._cond_fields = cond_fields;
                                predicItem._tableInfo= condinfo._tbInfo;
                                predicItem._tableName = condinfo._tbInfo->_tb;
                                lstPredicItems.push_back(predicItem);
                            } else {

                            }
                            //printf("obg:(%s, %s)\n", sname, svalue);
                        }
                        else if (strname.compare("policy_model_id") == 0) {
                        }

                }
            } else if (CommonFun::StrCaseCmp(pname, "update_control") == 0) {
                size_t ic = pattr->Count();
                //assert(ic==8);

                //std::string table;
                //std::set<std::string> tables;
                //std::set<std::string> excludeTables;
                std::set<std::string> fields;
                std::set<std::string> cond_fields;
                std::string deny_update_cond1;
                std::string deny_update_cond2;
                std::string deny_update_cond3;

                for (int j = 0; j < ic; ++j)
                {
                    const char * sname = nullptr;
                    const char * svalue = nullptr;

                    CEAttributeType tp = XACML_string;
                    pattr->GetAttrByIndex(j, &sname, &svalue, &tp);

                    std::string strname(sname);
                    std::string strvalue(svalue);

                    if (strvalue.empty()) continue;
//                    if (strname.compare("deny_update_tables") == 0) {//Mask Tables	mask_tables	Single Row
//
//                        if (strvalue.find("- ")==0){//"- tablea,tableb" means exclude these table from all table "*"
//                            tables.insert(ALL_TABLE_SYMBOLS);
//                            std::string strRealValue = strvalue.substr(2);
//                            parse_fields(strRealValue, excludeTables);
//                        }
//                        else{
//                            parse_fields(strvalue, tables);
//                        }
//                    }else
                        if (strname.compare("fields") == 0) {//deny_update Fields	deny_update_fields	Single Row
                        parse_fields(strvalue, fields);
                    }else if (strname.compare("condition1") == 0) {//deny_update Condition	deny_update_cond1	Single Row
                        deny_update_cond1 = parser_condition(strvalue, condinfo, cond_fields);
                        if (deny_update_cond1.compare(PARSE_CONDITION_ERROR_IGNORE_OB)==0){
                            fields.clear();
                            theLog->WriteLog(log_error, "parse deny_update conditoin failed for ignore this obligation:%s", strvalue.c_str());
                            break;
                        }
                    }else if (strname.compare("condition2") == 0) {//OR deny_update Condition	deny_update_cond2	Single Row
                        deny_update_cond2 = parser_condition(strvalue, condinfo, cond_fields);
                        if (deny_update_cond2.compare(PARSE_CONDITION_ERROR_IGNORE_OB)==0){
                            fields.clear(); //clear tables to make the whole obligation been ignored.
                            theLog->WriteLog(log_error, "parse deny_update conditoin failed for ignore this obligation:%s", strvalue.c_str());
                            break;
                        }
                    }else if (strname.compare("condition3") == 0) {//OR deny_update Condition	deny_update_cond3	Single Row
                        deny_update_cond3 = parser_condition(strvalue, condinfo, cond_fields);
                        if (deny_update_cond3.compare(PARSE_CONDITION_ERROR_IGNORE_OB)==0){
                            fields.clear();
                            theLog->WriteLog(log_error, "parse mask conditoin failed for ignore this obligation:%s", strvalue.c_str());
                            break;
                        }
                    }
                }

                if(fields.size() == 0){
                    continue;
                }


                //for(auto tb:tables)
                {
                    auto it = map_dup.find(condinfo.real_tbname);
                    if(it == map_dup.end()){
                        DenyUpdateItemMap *sub = new DenyUpdateItemMap();
                        for(auto field:fields){
                            UpdateItem * item = new UpdateItem();
                            item->_cond_fields.insert(cond_fields.begin(), cond_fields.end());
                            item->_condition = deny_update_cond1;
                            if(!deny_update_cond2.empty()){
                                if( item->_condition.empty()){
                                    item->_condition = deny_update_cond2;
                                } else {
                                    item->_condition += " OR ";
                                    item->_condition += deny_update_cond2;
                                }
                            }
                            if(!deny_update_cond3.empty()){
                                if( item->_condition.empty()){
                                    item->_condition = deny_update_cond3;
                                } else {
                                    item->_condition += " OR ";
                                    item->_condition += deny_update_cond3;
                                }
                            }
                            item->_field = field;
                           // item->_excludeTable = excludeTables;

                            sub->insert(std::make_pair(field, item));
                        }
                        map_dup.insert(std::make_pair(condinfo.real_tbname, sub));
                    } else {
                        DenyUpdateItemMap *sub = it->second;
                        for(auto field:fields){
                            auto itsub = sub->find(field);
                            if(itsub == sub->end()){
                                UpdateItem * item = new UpdateItem();
                                item->_cond_fields.insert(cond_fields.begin(), cond_fields.end());
                                item->_condition = deny_update_cond1;
                                if(!deny_update_cond2.empty()){
                                    if( item->_condition.empty()){
                                        item->_condition = deny_update_cond2;
                                    } else {
                                        item->_condition += " OR ";
                                        item->_condition += deny_update_cond2;
                                    }
                                }
                                if(!deny_update_cond3.empty()){
                                    if( item->_condition.empty()){
                                        item->_condition = deny_update_cond3;
                                    } else {
                                        item->_condition += " OR ";
                                        item->_condition += deny_update_cond3;
                                    }
                                }
                                item->_field = field;
                                //item->_excludeTable = excludeTables;
                                sub->insert(std::make_pair(field, item));

                            } else {
                                //ignore
                            }
                        }
                    }
                }
            }
        }
    }
}
void parser_result_2_security_config(std::set<std::string> & _tables, std::vector<std::set<std::string>> & _fields,IPolicyResult * result, ConditionInfo & condinfo, S4HException & /*e*/){
    if(result ==NULL)
        return ;
    std::string action = condinfo._action;
    if(CommonFun::StrCaseCmp( action.c_str() ,EM_ACTION_CONFIG) == 0 ) {
        if(result->GetEnforcement() == PolicyEnforcement::Deny){
            return ;
        }
        size_t icount = result->ObligationCount();
        for (int i = 0; i < icount; ++i)
        {
            const IObligation* pobg = result->GetObligationByIndex(i);
            const char * pname = pobg->GetObligationName();
            std::string obname(pobg->GetObligationName());
            //printf("oblgation_name:%s\n", obname.c_str());
            const IAttributes* pattr = pobg->GetAttributes();
            if (CommonFun::StrCaseCmp(pname, "security_tbs") == 0) {
                size_t ic = pattr->Count();
                assert(ic==2);
                for (int j = 0; j < ic; ++j)
                {
                    const char * sname = nullptr;
                    const char * svalue = nullptr;
                    CEAttributeType tp = XACML_string;
                    pattr->GetAttrByIndex(j, &sname, &svalue, &tp);

                    std::string strname(sname);
                    if (strname.compare("tables") == 0) {
                        parse_fields(std::string(svalue), _tables);
                    }
                }
            } else if (CommonFun::StrCaseCmp(pname, "fieldalias") == 0){
                size_t icout = pattr->Count();
                assert(icout==3);
                std::string allval;
                for (int j = 0; j < icout; ++j)
                {
                    const char * sname = nullptr;
                    const char * svalue = nullptr;
                    CEAttributeType tp = XACML_string;
                    pattr->GetAttrByIndex(j, &sname, &svalue, &tp);
                    std::string strname(sname);
                    if (strname.compare("fieldname") == 0) {
                        allval+=svalue;
                        allval+=',';
                    }
                    if (strname.compare("alias") == 0) {
                        allval+=svalue;
                    }
                }
                std::set<std::string> set_fields;
                parse_fields(allval, set_fields);
                _fields.push_back(set_fields);
            }
        }
    }
}
