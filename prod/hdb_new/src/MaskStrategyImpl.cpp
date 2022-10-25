#include "MaskStrategyImpl.h"
#include "tool.h"
#include "log.h"
#include <boost/algorithm/string/replace.hpp>

#define SINGLE_QUOTE_STR "\'"
#define SINGLE_QUOTE_CHAR '\''
#define ESCAPED_SINGLE_QUOTE "''" // Hana doesn't support escape prefixing with '\'
#define SLASH_CHAR '\\'

// SQLCondition::SQL_OP SQLCondition::TransformOp(const std::string & sop, S4HException & e) {
//    if(sop.length() == 1) {
//        if(sop[0] == '=') {
//            return SQL_EQ;
//        } else if (sop[0] == '>') {
//            return SQL_GT;
//        } else if (sop[0] == '<') {
//            return SQL_LT;
//        }
//    } else if (sop.length() == 2) {
//        if (sop[0] == '>' && sop[1] == '=') {
//            return SQL_GTEQ;
//        } else if (sop[0] == '<' && sop[1] == '=') {
//            return SQL_LTEQ;
//        } else if (sop[0] == '!' && sop[1] == '=') {
//            return  SQL_NEQ;
//        }
//    }
//    if(!sop.empty())
//        e.SetError(S4H_SQL_QUERY_PC_FAIL, "mask condition operation is error");
//    return SQL_GT;
//}



void update_map(std::map<std::string,int> & colmap, std::string & col, int op) {
    if(col.empty())
        return;
    std::map<std::string,int>::iterator it = colmap.find(col);
    if(it == colmap.end()) {
        colmap.insert(std::make_pair(col,op));
    } else {
        if(it->second > op) {
            it->second = op;
        }
    }
}

std::map<std::string, int> MaskStrategyImpl::GetConditionColumnAndOp() {
    std::map<std::string,int> ret;
    for (auto it : _conditions) {
        update_map(ret, it._column1, (int)it._op1);
        update_map(ret, it._column2, (int)it._op2);
        update_map(ret, it._column3, (int)it._op3);
    }
    return ret;
}

std::u16string make_column(const std::u16string& scope, const std::string& column) {
    if (scope.length() == 0) return CommonFun::FromUTF8(column);
    else return scope + u"." + CommonFun::FromUTF8(column);
}

std::u16string make_op(SQLCondition::SQL_OP op) {
    switch (op) {
        case SQLCondition::SQL_GTEQ:    return u" >= ";
        case SQLCondition::SQL_GT:      return u" > ";
        case SQLCondition::SQL_LTEQ:    return u" <= ";
        case SQLCondition::SQL_LT:      return u" < ";
        case SQLCondition::SQL_EQ:      return u" = ";
        case SQLCondition::SQL_NEQ:     return u" <> ";
        default: { /* not reached */ }
    }
    return u"";
}

/**
 * Appends an SQL string to the given StringBuilder, including the opening
 * and closing single quotes. Any single quotes internal to sqlString will
 * be escaped.
 *
 * This method is deprecated because we want to encourage everyone
 * to use the "?" binding form.  However, when implementing a
 * ContentProvider, one may want to add WHERE clauses that were
 * not provided by the caller.  Since "?" is a positional form,
 * using it in this case could break the caller because the
 * indexes would be shifted to accomodate the ContentProvider's
 * internal bindings.  In that case, it may be necessary to
 * construct a WHERE clause manually.  This method is useful for
 * those cases.
 *
 * @param sb the StringBuilder that the SQL string will be appended to
 * @param sqlString the raw string to be appended, which may contain single quotes
 * @see https://github.com/aosp-mirror/platform_frameworks_base/blob/master/core/java/android/database/DatabaseUtils.java#appendEscapedSQLString
 * @see https://en.cppreference.com/w/cpp/io/manip/quoted (#include <iomanip>) std::quoted("'abc", SINGLE_QUOTE_CHAR)
 */
std::string quoted_sql_string(const std::string &sqlString, EM_SAP_DataType type = EM_SAP_VARCHAR)
{
	std::string strbuf;
	switch (type)
	{
		//numeric data type family
	case EM_SAP_INTEGER:
	case EM_SAP_TINYINT:
	case EM_SAP_BIGINT:
	case EM_SAP_SMALLINT:
		//approximate numeric data type family
	case EM_SAP_DOUBLE:
	case EM_SAP_DECIMAL:
	case EM_SAP_SMALLDECIMAL:
	case EM_SAP_REAL:
		if (std::string::npos == sqlString.find(SINGLE_QUOTE_CHAR)) {
			return sqlString;
		}
		else if (sqlString.empty()) {
			return "''";
		}
		//string data type family
	case EM_SAP_VARCHAR:
	case EM_SAP_NVARCHAR:
	case EM_SAP_ALPHANUM:
	case EM_SAP_SHORTTEXT:
		//time data type family
	default: strbuf = SINGLE_QUOTE_CHAR; break;
	case EM_SAP_DATE: strbuf = "date'"; break;
	case EM_SAP_TIME: strbuf = "time'"; break;
	case EM_SAP_TIMESTAMP: strbuf = "timestamp'"; break;
	case EM_SAP_SECONDDATE: strbuf = "seconddate'"; break;
		break;
	}
	//size_t nQuotes = std::count(sqlString.begin(), sqlString.end(), SINGLE_QUOTE_CHAR);
	//strbuf.reserve(sqlString.length() + nQuotes + 2);
#if 0
	if (std::string::npos != sqlString.find(SINGLE_QUOTE_CHAR)) {
		int length = sqlString.length();
		for (int idx = 0; idx < length; idx++) {
			char ch = sqlString[idx];
			if (ch == SINGLE_QUOTE_CHAR) {
				strbuf += ESCAPED_CHAR;
			}
			strbuf += ch;
		}
	}
	else
		strbuf += sqlString;
#else
	strbuf += boost::replace_all_copy(sqlString, SINGLE_QUOTE_STR, ESCAPED_SINGLE_QUOTE);
#endif
	strbuf += SINGLE_QUOTE_CHAR;
	return strbuf;
}

//std::u16string make_predicate(const std::u16string& scope, const SQLCondition& condition) {
//    return make_column(scope, condition._column1) + make_op(condition._op1) + u"'" + condition._constant_value1 + u"'";
//}
std::u16string make_predicate(const std::u16string &scope, const std::string &str_col, SQLCondition::SQL_OP op, const std::string &str_value, const MaskStrategyImpl *pMaskStrategy) {
    auto columnType = pMaskStrategy->GetMaskConditionColumnType(str_col);
    theLog->WriteLog(log_info, "make_predicate: %s (%d) = %s", str_col.c_str(), columnType, str_value.c_str());
    //return make_column(scope, str_col) + make_op(op) + u"'" + CommonFun::FromUTF8(str_value) + u"'";
    //return make_column(scope, str_col) + make_op(op) + CommonFun::FromUTF8(str_value);
    return make_column(scope, str_col) + make_op(op) + CommonFun::FromUTF8(quoted_sql_string(str_value, columnType));
}

std::u16string make_and_predicate(const std::set<std::u16string> & vec_col, const std::u16string &scope, const SQLCondition &condition, const MaskStrategyImpl* pMaskStrategy) {
    std::u16string ret;
    std::u16string colname = CommonFun::FromUTF8(condition._column1);
    if(vec_col.find(colname) != vec_col.end()) {
        std::u16string str = make_predicate(scope, condition._column1, condition._op1, condition._constant_value1, pMaskStrategy);
        if(str.length() > 2) ret +=  str;
    }
    colname = CommonFun::FromUTF8(condition._column2);
    if(vec_col.find(colname) != vec_col.end()) {
        std::u16string str = make_predicate(scope, condition._column2, condition._op2, condition._constant_value2, pMaskStrategy);
        if(str.length() > 2) {
            if(!ret.empty()) ret += u" AND ";
            ret += str;
        }
    }
    colname = CommonFun::FromUTF8(condition._column3);
    if(vec_col.find(colname) != vec_col.end()) {
        std::u16string str = make_predicate(scope, condition._column3, condition._op3, condition._constant_value3, pMaskStrategy);
        if(str.length() > 2) {
            if(!ret.empty()) ret += u" AND ";
            ret += str;
        }
    }
    return  ret;

}

void build_symbols_for_condition(std::u16string &mask_symbols, const MaskSymbols &symbols) {
    mask_symbols = CommonFun::FromUTF8(quoted_sql_string(symbols._symbols, symbols._type));
}

bool is_dec(char16_t c) {
    return u'0' <= c && c <= u'9';
}

bool is_dot(char16_t c) {
    return u'.' == c;
}

bool check_datatype(const MaskSymbols & symbols) {
    switch(symbols._type) {
        case EM_SAP_INTEGER:
        case EM_SAP_TINYINT:
        case EM_SAP_BIGINT:
        case EM_SAP_SMALLINT: {
            unsigned int pos = 0;
            while(pos < symbols._symbols.length()) {
                if(!is_dec(symbols._symbols[pos])
                && symbols._symbols[pos] != u'-' )
                    return false;
                pos++;
            }
        }
        break;
        
        case EM_SAP_DOUBLE:
        case EM_SAP_DECIMAL:
        case EM_SAP_SMALLDECIMAL:
        case EM_SAP_REAL:
        {
            unsigned int pos = 0;
            while(pos < symbols._symbols.length()) {
                if(!is_dec(symbols._symbols[pos])
                && !is_dot(symbols._symbols[pos])
                && symbols._symbols[pos] != u'-' )
                    return false;
                pos++;
            }
        }break;
        default:{
        }break;
    }
    return true;
}

std::u16string MaskStrategyImpl::MakeExpressionV2(const std::set<std::u16string> & vec_col, const std::u16string& scope, const std::u16string& alias) {
    std::u16string ret;
    std::u16string alias_name = alias;
    if (alias_name.length() == 0) {
        alias_name = CommonFun::FromUTF8(_masked_column);
    }
    std::u16string mask_symbols;
    //std::u16string mask_symbols = u"'" + CommonFun::FromUTF8(_masked_symbols._symbols) + u"'";
    build_symbols_for_condition(mask_symbols, _masked_symbols);
    if(mask_symbols.empty())
        return ret;

    if (_conditions.size() > 0) {
        // "CASE WHEN %s THEN '***' ELSE %s END AS %s "
        std::u16string conditions;
        for (unsigned int i = 0; i < _conditions.size(); ++i) {
            std::u16string sub = make_and_predicate(vec_col, scope, _conditions[i], this);
            if (!sub.empty() ) {
                if(!conditions.empty()) conditions += u" OR ";
                conditions += sub;
            }
        }
        
        if(conditions.empty())  {
            ret =  u" "+ mask_symbols +u" AS " + alias_name + u" ";
        } else {
            if(check_datatype(_masked_symbols))
                ret = u" CASE WHEN " + conditions + u" THEN "+ mask_symbols +u" ELSE " + make_column(scope, _masked_column) +
                    u" END AS " + alias_name + u" ";
        }
        
    } else {
        ret = u" "+ mask_symbols +u" AS " + alias_name + u" ";
    }
    theLog->WriteLog(log_info, "MaskStrategyImpl::MakeExpressionV2 return %s", CommonFun::ToUTF8(ret).c_str());
    return ret;
}


MaskStrategyImpl *create_maskstrategy(const std::string &maskcol, const std::string & symbols, const std::vector<SQLCondition> &conds) {
    MaskStrategyImpl *pret = new MaskStrategyImpl();
    pret->_masked_column = maskcol;
    pret->_conditions = conds;
    pret->_masked_symbols._symbols =symbols;
    return pret;
}



std::string MaskStrategyImpl::Print() const{
    std::string ret;
    ret+="( maskcol: ";
    ret+=_masked_column;
    ret+=", masksymbol: ";
    ret+=_masked_symbols._symbols;
    ret+=", maskcondition:( ";
    ret+="";

    std::string condss;
    for(auto it:_conditions) {

        std::string conds;

        std::string cond;
        if(! it._column1.empty()) {
            cond+=it._column1;
            cond+= CommonFun::ToUTF8(make_op(it._op1));
            cond+=it._constant_value1;
        }

        std::string cond2;
        if(!it._column2.empty()) {
            cond2+=it._column2;
            cond2+= CommonFun::ToUTF8(make_op(it._op2));
            cond2+=it._constant_value2;
        }


        std::string cond3;
        if(!it._column3.empty()) {
            cond3+=it._column3;
            cond3+=CommonFun::ToUTF8(make_op(it._op3));
            cond3+=it._constant_value3;
        }

        if(cond.length() > 3) {
            conds+=cond;
        }
        if(cond2.length() > 3) {
            if(conds.empty()) {
                conds.append(cond2);
            } else {
                conds+=" AND ";
                conds.append(cond2);
            }
        }
        if(cond3.length() > 3) {
            if(conds.empty()) {
                conds.append(cond3);
            } else {
                conds+=" AND ";
                conds.append(cond3);
            }
        }
        if(!conds.empty()) {
            if(condss.empty() ) {

                condss+="( ";
                condss +=conds;
                condss+=" )";
            } else {
                condss+=" OR ";
                condss+="( ";
                condss +=conds;
                condss+=" )";
            }
        }

    }
    if(condss.length() > 0) {
        ret+=condss;
    }
    ret+=") )";
    return ret;
}