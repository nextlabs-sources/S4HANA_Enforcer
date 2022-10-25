//
// Created by jeff on 2020/3/18.
//

#include "EMMaskOpr.h"
#include <assert.h>
#include "EMS4HSelectAst.h"
#include "EMMaskDef.h"
#include "../parser_view/view_interface.h"
#include "../include/KeyMaskOpr.h"
#include "../include/NxlDbDataMgr.h"
#include "SqlDbcUserInfoMgr.h"
#include "PolicyManager.h"
#include "EMS4HMutliSelectAst.h"
#include "log.h"
#include "tool.h"

bool is_dec(char c);
bool is_wirte(char c);
bool is_number(const std::string & ss);
std::string get_mask_symbols( EM_SAP_DataType type, EMMaskFormat format_, const std::string & symbols, const std::string & field);
std::string escape_chars(const std::string &  src,char c);
bool isstring_datatype(EM_SAP_DataType tp);
std::string constrcut_mask_string(EMColumnInfo * col,  MaskItem * maskitem);
bool mask_one_field(EMColumnInfo * col,  MaskItem * maskitem);
bool check_support_star(std::vector<EMTableInfo*> & tables,EMColumnInfo * field);
MaskItem * match_maskitem(EMTableInfo *tbinfo, EMColumnInfo* colinfo, const MaskConditionMap *pmap_mask, const ITableMetaData *ptb);
bool mask_fields_star_no_prefix(EMColumnInfo* field,  EMS4HSelectAst* pSelectAst, const MaskConditionMap *pmap_mask, void* connection, S4HException & exc);
bool mask_fields_star_prefix(EMColumnInfo* field,    EMS4HSelectAst* pSelectAst, const MaskConditionMap *pmap_mask, void* connection, S4HException & exc);

bool get_valid_condition_fields_and_set_type( std::set<std::u16string> & settb, const ITableMetaData * pmeta, MaskItem * pmask) {

    bool bhav = pmeta->HasColumn(pmask->_field);
    if(!bhav){
        theLog->WriteLog(log_error, "get_valid_condition_fields return false for HasColumn=false, col=%s", pmask->_field.c_str() );
        return false;
        //settb.insert(str_mask);
    }
    pmask->_field_type = (pmeta->GetColumnDataType(pmask->_field));
    if (pmask->_field_type==EM_SAP_BLOB){
        theLog->WriteLog(log_error, "get_valid_condition_fields return fasle for BLOB type. col=%s",  pmask->_field.c_str() );
        return false;
    }
    
    
    for(auto it: pmask->_cond_fields){
        bool bhavTemp = pmeta->HasColumn(it);
        if(!bhavTemp){
            theLog->WriteLog(log_error, "get_valid_condition_fields clear mask condition for HasColumn failed. field:%s, condition:%s", it.c_str(), pmask->_condition.c_str() );
            pmask->_condition.clear();/*if condition field is invalid we set condition empty*/
            break;
        } else {
            /*todo pmask condition field check type*/
            settb.insert(CommonFun::FromUTF8(it));
        }
    }
    return true;
}

bool is_dec(char c) {
    return '0' <= c && c <= '9';
}

bool is_number(const std::string & ss){
    unsigned int pos = 0;
    if(ss.length() <= 0) return false;
    while(pos < ss.length()) {
        if(!is_dec(ss[pos])
           && '.' != ss[pos]
           && ss[0] != '-' )
            return false;
        pos++;
    }
    return true;
}

std::string escape_chars(const std::string &  src,char c){
    //assert(c == '"' || c == '`' || c == ']');
    std::string ret ;
    if(c == '\''){
        ret += "'";
        for (size_t i = 0; i < src.length(); ++i) {
            if (src[i] == '\'' || src[i] == '\\') {
                ret += "\\";
            }
            ret += src[i];
        }
        ret += "'";
    } else if(c == '"'){
        ret += "\"";
        for (size_t i = 0; i < src.length(); ++i) {
            if (src[i] == '"' || src[i] == '\\') {
                ret += "\\";
            }
            ret += src[i];
        }
        ret += "\"";
    }

    return ret;
}

std::string get_default_symbols(EM_SAP_DataType type) {
    std::string ret = "'***'";
    switch(type){
        case   EM_SAP_TINYINT:
        case   EM_SAP_SMALLINT:
        case   EM_SAP_INTEGER:
        case   EM_SAP_BIGINT:
        case   EM_SAP_SMALLDECIMAL:
        case   EM_SAP_DECIMAL:
        case   EM_SAP_REAL:
        case   EM_SAP_DOUBLE: {
            ret = "-99999";
        } break;
        case  EM_SAP_DATE:{
            ret = "'1900-01-01'";
        } break;
        case  EM_SAP_SECONDDATE: {
            ret = "'1900-01-01 00:00:00'";
        } break;
        case  EM_SAP_TIME:{
            ret = "'00:00:00'";
        } break;
        case  EM_SAP_TIMESTAMP: {
            ret = "'1900-01-01 00:00:00'";
        } break;
        default:{
        }break;
    }
    return  ret;
}

std::string get_mask_symbols( EM_SAP_DataType type, EMMaskFormat format_, const std::string & symbols, const std::string & field){
    std::string ret;
    switch(format_){
        case EMMASK_FULL:{
            if(symbols.empty()){
                return get_default_symbols(type);
            }
            if( isstring_datatype(type) ){
                ret = escape_chars(symbols, '\'');//"'" + symbols + "'";//
            } else {
                if(is_number(symbols)) {
                    ret =  symbols;
                } else {
                    ret = get_default_symbols(type);
                }
            }
        } break;
        case EMMASK_RANDNUMBER:{
            if( isstring_datatype(type) || symbols.empty()){
                return get_default_symbols(type);
            }
            unsigned int pos =  symbols.find("/");
            if(std::string::npos != pos){
                std::string start_s = symbols.substr(0, pos);
                std::string end_s = symbols.substr(pos+1);
                int start_i = atoi(start_s.c_str());
                int end_i = atoi(end_s.c_str());
                if(start_i < end_i){
                    int nprint=0;
                    int irange = end_i-start_i;
                    //ROUND(RAND()*irange)+ start_i
                    char cret[127] = {0};
                    nprint = snprintf(cret, sizeof(cret),  "ROUND(RAND()*%d+%d)", irange, start_i);
                    ret = cret;
                    //check error
                    if (nprint<0 || nprint>=sizeof(cret)){
                        theLog->WriteLog(log_error, "get_mask_symbols failed for nprint=%d, start_i=%d, end_i=%d", nprint, start_i, end_i);
                        return get_default_symbols(type);
                    }
                } else if(start_i == end_i){
                    ret = start_s;
                } else {
                    return get_default_symbols(type);
                }
            } else {
                return get_default_symbols(type);
            }
        } break;
        case EMMASK_PARTIAL:{
            if(!isstring_datatype(type)){
                return get_default_symbols(type);
            }
            unsigned int pos1 =  symbols.find("/");
            unsigned int pos3 = symbols.rfind("/");
            if(pos3 - pos1 <= 1){
                return get_default_symbols(type);
            }
            std::string pre_start = symbols.substr(0, pos1);
            std::string pre_end = symbols.substr(pos3+1);
            std::string pre_mid = symbols.substr(pos1+1, pos3-pos1-1);
            std::string midle_s = escape_chars(pre_mid, '\'');//"'" + symbols + "'";//

            int start_i = atoi(pre_start.c_str());
            int end_i = atoi(pre_end.c_str());
            if(start_i <0 || end_i <0){
                return get_default_symbols(type);
            }
            char cret[255] = {0};
            int nprint=0;
            if(start_i == 0){
                if(end_i == 0){
                    nprint = snprintf(cret, sizeof(cret), "%s",midle_s.c_str());
                } else {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(%s,SUBSTR(%s,LENGTH(%s)-%d+1))", midle_s.c_str(), field.c_str(), field.c_str(), end_i);
                }
            } else {
                if(end_i == 0){
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(SUBSTR(%s,1,%d),%s)", field.c_str(),start_i, midle_s.c_str());
                } else {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(SUBSTR(%s,1,%d),CONCAT(%s,SUBSTR(%s,LENGTH(%s)-%d+1)))", field.c_str(), start_i, midle_s.c_str(), field.c_str(), field.c_str(), end_i);
                }
            }
 
            //check error
            if (nprint<0 || nprint>=sizeof(cret)){
                theLog->WriteLog(log_error, "get_mask_symbols failed for nprint=%d, start_i=%d, end_i=%d, middle=%s", nprint, start_i, end_i, midle_s.c_str() );
                nprint = snprintf(cret, sizeof(cret), "'***'");
            }

            ret = cret;

        } break;
        case EMMASK_KEY:{
            return get_default_symbols(type);
        } break;
    }


    return ret;
}

bool isstring_datatype(EM_SAP_DataType tp){
    switch(tp){
        case   EM_SAP_TINYINT:
        case   EM_SAP_SMALLINT:
        case   EM_SAP_INTEGER:
        case   EM_SAP_BIGINT:
        case   EM_SAP_SMALLDECIMAL:
        case   EM_SAP_DECIMAL:
        case   EM_SAP_REAL:
        case   EM_SAP_DOUBLE: {
            return  false;
        } break;
        default:{
            return  true;
        }break;
    }
}

std::string constrcut_mask_string(EMColumnInfo * col,  MaskItem * maskitem) {
    std::string node_str;
    assert(maskitem);
    std::string alias;
    std::string field;
    if(col){
        if(col->_alias){
            if(col->_node){
                Node *pnode_field = col->_node->getChild(0);
                if (pnode_field) {
                    field = pnode_field->serialize();
                }
                Node *pnode_alias = col->_node->getChild(1);
                if (pnode_alias) {
                    alias = pnode_alias->serialize();
                }
            } else {
                if(!col->_tb.empty()){
                    bool bHaveTBQuote = col->_tb[0]=='"';
                    field =  (bHaveTBQuote ? "" : "\"") + col->_tb +  (bHaveTBQuote ? "." : "\".");
                } 
                {
                    bool hHaveFieldQuote = col->_field[0]=='"';
                    field  += (hHaveFieldQuote ? "" : "\"") + col->_field +  (hHaveFieldQuote ? "" : "\"");
                    alias = (hHaveFieldQuote ? "" : "\"") + col->_field +  (hHaveFieldQuote ? "" : "\"");
                }   
            }
        } else {
            if(col->_node){
                field = col->_node->serialize();
                bool hHaveFieldQuote = col->_field[0]=='"';
                alias = (hHaveFieldQuote ? "" : "\"") + col->_field +  (hHaveFieldQuote ? "" : "\"");
            } else {
                if(!col->_tb.empty()){
                    bool bHaveTBQuote = col->_tb[0]=='"';
                    field =  (bHaveTBQuote ? "" : "\"") + col->_tb +  (bHaveTBQuote ? "." : "\".");
                } 
                {
                    bool hHaveFieldQuote = col->_field[0]=='"';
                    field  += (hHaveFieldQuote ? "" : "\"") + col->_field +  (hHaveFieldQuote ? "" : "\"");
                    alias = (hHaveFieldQuote ? "" : "\"") + col->_field +  (hHaveFieldQuote ? "" : "\"");
                }

            }
        }
    } else {
        /*select * from tb*/
        std::string strMaskFiled = maskitem->_field;
        std::transform(strMaskFiled.begin(), strMaskFiled.end(), strMaskFiled.begin(), ::toupper); 
        field = "\"" + strMaskFiled + "\"";
        alias = field;
    }

    std::string ss = get_mask_symbols( maskitem->_field_type, maskitem->_format, maskitem->_symbols, maskitem->_field);

    std::string strAlias= alias.empty() ?  " " : (" AS " + alias + " ") ;

    if(col->_type == EMColumnInfo::SUB_COL){
        strAlias.clear();//  EXP:SUM(case when 1>0 then 's' else 'a' end) //no aliases
    }
    if(maskitem->_condition.empty()) {
        node_str =  " " + ss + strAlias;
    } else {
        node_str =  " CASE WHEN " + maskitem->_condition + " THEN "+ ss +" ELSE " + field +
                    " END " + strAlias;
    }

    return node_str;
}

bool mask_one_field(EMColumnInfo * col,  MaskItem * maskitem) {
    if(col == NULL || maskitem == NULL)
        return false;
    std::string node_str = constrcut_mask_string(col, maskitem);
    Node *pnode = Node::makeTerminalNode(E_SPECIAL_TERMINAL, node_str);
    *(col->_node) = *pnode;
    delete (pnode);
    return true;
}


bool is_wirte(char c) {
    if(c == ' ' || c == '\n' || c == '\t' || c == '\f' || c == '\r') return true;
    return false;
}

MaskItem* GetMaskItemFromField(const std::string& curTable, const ITableMetaData* ptbMeta,  const MaskConditionMap* pmap_mask, EMColumnInfo* field)
{
    MaskItem* pmask = NULL;
    MaskItemMap *pmap_mask_item = NULL;


    //if (field->_type == EMColumnInfo::DIRECT_COL )
    {
        auto it = pmap_mask->find(curTable);
        if (it != pmap_mask->end()) {
            pmap_mask_item = const_cast<MaskItemMap *>(it->second);
            if (pmap_mask_item != NULL) {
                auto ititem = pmap_mask_item->find(field->_field);
                if (ititem != pmap_mask_item->end())  {
                    pmask = const_cast<MaskItem *>(ititem->second);
                    theLog->WriteLog(log_info, "GetMaskItemFromField. match mask item for table.field. %s.%s", curTable.c_str(), field->_field.c_str());
                }
            }
        }
    }
    /*
    else if (field->_type == EMColumnInfo::FUNC_COL)
    {
        EMFuncColumnInfo *pFunColInfo = static_cast<EMFuncColumnInfo *>(field);
        if (NULL != pFunColInfo) {
            std::vector<EMColumnInfo *> &paramCol = pFunColInfo->_params;
            for (auto itParamCol = paramCol.begin(); itParamCol != paramCol.end(); itParamCol++)
            {
                EMColumnInfo *pParammCol = *itParamCol;
                if (!ptbMeta->HasColumn(pParammCol->_field)) {
                    theLog->WriteLog(log_info, "GetMaskItemFromField field:%s continue for table doesn't contains this field.", pParammCol->_field.c_str());
                    continue;
                }
                
                auto it = pmap_mask->find(curTable);
                if (it != pmap_mask->end())
                {
                    pmap_mask_item = const_cast<MaskItemMap *>(it->second);
                    if (pmap_mask_item != NULL) {
                        auto ititem = pmap_mask_item->find(pParammCol->_field);
                        if (ititem != pmap_mask_item->end()) {
                            pmask = const_cast<MaskItem *>(ititem->second);
                            //replace mask field name
                            //pmask->_field = field->_field; 
                            theLog->WriteLog(log_info, "GetMaskItemFromField.fun col, match mask item for table.field. %s.%s. fun_field:%s", curTable.c_str(), pParammCol->_field.c_str(), pFunColInfo->_alias_name.c_str());
                            break;
                        }
                    }
                }
            }
        }
    }
*/
    return pmask;
}

bool check_support_star(std::vector<EMTableInfo*> & tables,EMColumnInfo * field){
    if(field->_tb.empty()){
        return false;
    }
    for(auto & itb : tables){
        if(itb->_tb==field->_tb || itb->_alias==field->_tb){
            return true;
        }
    }
    return false;
}

bool mask_fields_v5(EMS4HAst * ast, void* connection, const MaskConditionMap *pmap_mask,
                              const EnforcerCtxImpl * pctx, S4HException & exc)
{
    if(!ast || ast->GetStatementType() != E_STMT_TYPE_SELECT)
        return false;
    EMS4HSelectAst * pselect = dynamic_cast<EMS4HSelectAst*>(ast);
    if (NULL==pselect) {
        theLog->WriteLog(log_error, "mask_fields_v5 return false for convert EMS4HSelectAst failed");
        return false;
    }

    if (NULL == pmap_mask) {
        theLog->WriteLog(log_error, "mask_fields_v5 return false for mask NULL==pmap_mask" );
        return false;
    }

    bool bret = false;
    for (auto field : pselect->_fields)  {
        //select a.*
        //select *
        if (field->_field == "*"){
            if(!pselect->_support_select_star && !check_support_star( pselect->_tables, field)){
                theLog->WriteLog(log_debug, "mask_fields_v5, _support_select_star:%s", pselect->_support_select_star?"true":"false");
                continue;//not support
            }
            if(field->_tb.empty()){
                bool bmasked = mask_fields_star_no_prefix(field, pselect, pmap_mask, connection, exc);
                if(bmasked) bret = true;
            }else {
                bool bmasked = mask_fields_star_prefix(field, pselect, pmap_mask, connection, exc);
                if(bmasked) bret = true;
            }
            continue;
        }
        if (field->_type == EMColumnInfo::FUNC_COL) {
            EMFuncColumnInfo* pFunCol = (EMFuncColumnInfo*)field;
            theLog->WriteLog(log_info, "mask_fields_v5. find fun col.  fun_field:%s , func_name:%s ",  pFunCol->_alias_name.c_str(), pFunCol->_func_name.c_str() );
            for(auto itFunField: pFunCol->_params){
                //sub field
                bool bmasked = mask_one_field( itFunField,  pselect,  connection, pmap_mask,  pctx,  exc);
                if(bmasked) bret=true;
            }
        } else {
            //select field
            bool bmasked = mask_one_field( field,  pselect,  connection, pmap_mask,  pctx,  exc);
            if(bmasked) bret=true;
        }

    }

    return bret;
}

bool mask_one_field(EMColumnInfo* field, EMS4HSelectAst * pselect, void* connection, const MaskConditionMap *pmap_mask, const EnforcerCtxImpl * pctx, S4HException & exc){
    bool bret = false;
    for (auto tb : pselect->_tables)  {
        std::string f_tbale = tb->_tb;
        //theLog->WriteLog(log_info, "mask_fields_v5 table:%s", f_tbale.c_str());  too many logs here

        if (!field->_tb.empty()) { // check qualify.
            if (field->_tb != f_tbale && field->_tb != tb->_alias) {
                theLog->WriteLog(log_info, "mask_fields_v5 field:%s, continue with alias,field->_tb=%s, tb->_alias=%s",
                                 field->_field.c_str(), field->_tb.c_str(), tb->_alias.c_str());
                continue;
            }
        }

        if (field->_node && field->_node->GetType() == E_SPECIAL_TERMINAL)  { //check masked.
            theLog->WriteLog(log_info, "mask_fields_v5 field:%s, continue with E_SPECIAL_TERMINAL", field->_field.c_str());
            continue;
        }

        const ITableMetaData *ptb = PolicyManager::Instance()->GetTableMetaData(f_tbale, tb->_type, connection, exc); /*todo table maybe hav qualify*/
        if (exc._code == S4H_INVALID_CONNECTION || ptb == NULL)  {
            theLog->WriteLog(log_error, "mask_fields_v5 GetTableMetaData return with S4H_INVALID_CONNECTION. table:%s", f_tbale.c_str() );
            return bret;
        }


        //field in table
        if (!ptb->HasColumn(field->_field)) {
            theLog->WriteLog(log_info, "mask_fields_v5 field:%s, continue for table doesn't contains this field", field->_field.c_str());
            continue;
        }

        std::string f_field = field->_field;
        MaskItem *pmask = match_maskitem(tb, field, pmap_mask, ptb);

        if (pmask == NULL) {
            continue;
        }

        do
        {
            std::set<std::u16string> vec_col;
            bool bhav = get_valid_condition_fields_and_set_type(vec_col, ptb, pmask); /*todo vec_col not used and modify mask condition to empty*/
            if (!bhav)  {
                theLog->WriteLog(log_warning, "mask_fields_v5 break for get_valid_condition_fields failed. condition:%s", pmask->_condition.c_str() );
                break;
            }

            if (pmask->_format == EMMASK_KEY && isstring_datatype(pmask->_field_type)){
                insert_maskdata_into_cachetable(pselect->Output(), field, pmask, connection, pctx, exc);
                if (exc._code != 0)
                {
                    theLog->WriteLog(log_warning, "insert_cachetable error:%s", exc._detail.c_str());
                }
            }

            /*if mask condition hava field, but the field is not in group by list. remove the mask condition*/
            if((field->_type==EMColumnInfo::DIRECT_COL) && (pselect->_groupby_list.size()>0)){
                for(auto it_cond_f:pmask->_cond_fields){
                    theLog->WriteLog(log_info, "mask_fields_v5 process group by. condition field:%s", it_cond_f.c_str() );
                    bool find = false;
                    for(auto itGroupByColumn: pselect->_groupby_list){
                        if(CommonFun::StrCaseCmp(it_cond_f, itGroupByColumn->_field) == 0){
                            if( !itGroupByColumn->_tb.empty() ){
                                if( CommonFun::StrCaseCmp(itGroupByColumn->_tb, tb->_tb) != 0 && CommonFun::StrCaseCmp(itGroupByColumn->_tb, tb->_alias) != 0) { //check table name
                                    continue;
                                }
                            }
                            find = true;
                            break;
                        }
                    }

                    if(find == false){
                        theLog->WriteLog(log_info, "mask_fields_v5 clear mask condition group by checking. mask table:%s, mask filed:%s, conditon:%s, condition_field:%s",
                                         tb->_tb.c_str(), pmask->_field.c_str(), pmask->_condition.c_str(), it_cond_f.c_str() );
                        pmask->_condition.clear();
                        break;
                    }
                }
            }
            bool btemp = mask_one_field(field, pmask);
            pselect->AddedMatchedMaskItem(pmask->_qulify, pmask);
            theLog->WriteLog(log_info, "mask_fields_v5 field:%s with mask_one_field==%d", pmask->_field.c_str() , btemp);
            if (btemp)
                bret = btemp;

        } while (false);
    }

    return bret;

}

MaskItem * match_maskitem(EMTableInfo *tbinfo, EMColumnInfo* colinfo, const MaskConditionMap *pmap_mask, const ITableMetaData *ptb){
    MaskItem * pmask = NULL;
    std::string f_tbale = tbinfo->_tb;
    std::string f_field = colinfo->_field;

    //mask table is: table
    pmask = GetMaskItemFromField(f_tbale, ptb, pmap_mask, colinfo);

    // if table is view
    do
    {
        if (NULL != pmask) {
            break;
        }
        if (tbinfo->_viewsource == NULL) {
            break;
        }

        const ViewColumnSourceInfo *psource = tbinfo->_viewsource->GetSourceColumn(f_field);
        if (NULL == psource)  {
            theLog->WriteLog(log_info, "match_maskitem view. GetSourceColumn failed. f_field:%s", f_field.c_str() );
            break;
        }

        //get v_table v_field to find mask info
        std::string temp = psource->SourceTableName();
        std::string t_schema, t_table;
        resolve_key(t_schema, t_table, temp);
        std::string t_field = psource->SourceColumnName();

        //find mask info   -- same as up
        auto it = pmap_mask->find(t_table);
        if (it != pmap_mask->end())  {
            MaskItemMap *pmap_mask_item = const_cast<MaskItemMap *>(it->second);
            if (pmap_mask_item == NULL)
                continue;
            auto ititem = pmap_mask_item->find(t_field);
            if (ititem != pmap_mask_item->end()) {
                pmask = const_cast<MaskItem *>(ititem->second);
                //replace mask field name
                pmask->_strOriginalFiledForTable =  t_field; //pmask->_field;
                pmask->_field = f_field;
                theLog->WriteLog(log_info, "match_maskitem. match mask item for view. v_field:%s , source:%s.%s, _strOriginalFiledForTable=%s", f_field.c_str(), t_table.c_str(), t_field.c_str() ,  pmask->_strOriginalFiledForTable.c_str() );
            }
        }

    } while (false);

    return pmask;
}


bool mask_fields_star_no_prefix(EMColumnInfo* field, EMS4HSelectAst* pSelectAst, const MaskConditionMap *pmap_mask,
                                void* connection, S4HException & exc){
    //need check all table;
    bool bret = false;
    std::string mask_value;

    const std::vector<EMTableInfo*> & tables = pSelectAst->GetTables();
    for (auto tb : tables)  {

        std::string f_tbale = tb->_tb;
        std::string f_alias = tb->_alias;
        std::string prefix;
        if(f_alias.empty()){
            if(tb->_node){
                prefix = tb->_node->serialize();
            } else {
                prefix = f_tbale;
            }
        } else {
            prefix = f_alias;
        }

        theLog->WriteLog(log_info, "mask_fields_star_no_prefix table:%s", f_tbale.c_str());

        const ITableMetaData *ptb = PolicyManager::Instance()->GetTableMetaData(f_tbale, tb->_type, connection, exc); /*todo table maybe hav qualify*/
        if (exc._code == S4H_INVALID_CONNECTION || ptb == NULL)  {
            theLog->WriteLog(log_error, "mask_fields_star_no_prefix GetTableMetaData return with S4H_INVALID_CONNECTION. table:%s", f_tbale.c_str() );
            return false;
        }

        std::vector<std::string> vec_tbcol;
        ptb->GetAllColumns(vec_tbcol);

        for(auto & itcol:vec_tbcol){
            EMColumnInfo col(NULL);
            col._field = itcol;
            col._tb = prefix;
            std::string real_col = "\""+itcol+"\""; //Handling of special symbols

            //mask
            std::string node_str;
            do
            {
                MaskItem *pmask = match_maskitem(tb, &col, pmap_mask, ptb);
                if(!pmask){
                    break;
                }
                std::set<std::u16string> vec_col;
                bool bhav = get_valid_condition_fields_and_set_type(vec_col, ptb, pmask); /*todo vec_col not used and modify mask condition to empty*/
                if (!bhav)  {
                    theLog->WriteLog(log_warning, "mask_fields_star_no_prefix break for get_valid_condition_fields failed. condition:%s", pmask->_condition.c_str() );
                    break;
                }

                col._field = real_col;
                node_str = constrcut_mask_string(&col, pmask);
                if (!node_str.empty()){
                    pSelectAst->AddedMatchedMaskItem("", pmask);    
                }
                theLog->WriteLog(log_info, "mask_fields_star_no_prefix field:%s with constrcut_mask_string==%s", pmask->_field.c_str(), node_str.c_str());

            } while (false);


            if(!mask_value.empty()){
                mask_value += ',';
            }
            if (!node_str.empty()){
                mask_value += node_str;
                bret = true;
            } else {
                if(!prefix.empty()){
                    mask_value += prefix;
                    mask_value += '.';
                }
                mask_value += real_col;
            }

        }
    }
    if(!mask_value.empty()){
        Node * pt = Node::makeTerminalNode(E_SPECIAL_TERMINAL, mask_value.c_str());
        *(field->_node)=*pt;
        delete(pt);
    } else {
        return false;
    }
    return  bret;
}

bool mask_fields_star_prefix(EMColumnInfo* field, EMS4HSelectAst* pSelectAst, const MaskConditionMap *pmap_mask,
                             void* connection, S4HException & exc){
    //need check all table;
    bool bret = false;
    std::string mask_value;
    const std::vector<EMTableInfo*>& tables = pSelectAst->GetTables();
    for (auto tb : tables)  {
        std::string f_tbale = tb->_tb;
        std::string f_alias = tb->_alias;
        if(field->_tb != f_alias && field->_tb != f_tbale){
            continue;
        }

        std::string prefix;
        if(f_alias.empty()){
            if(tb->_node){
                prefix = tb->_node->serialize();
            } else {
                prefix = f_tbale;
            }
        } else {
            prefix = f_alias;
        }

        theLog->WriteLog(log_info, "mask_fields_star_prefix table:%s", f_tbale.c_str());

        const ITableMetaData *ptb = PolicyManager::Instance()->GetTableMetaData(f_tbale, tb->_type, connection, exc); /*todo table maybe hav qualify*/
        if (exc._code == S4H_INVALID_CONNECTION || ptb == NULL)  {
            theLog->WriteLog(log_error, "mask_fields_star_prefix GetTableMetaData return with S4H_INVALID_CONNECTION. table:%s", f_tbale.c_str() );
            return false;
        }

        std::vector<std::string> vec_tbcol;
        ptb->GetAllColumns(vec_tbcol);

        for(auto & itcol:vec_tbcol){
            EMColumnInfo col(NULL);
            col._field = itcol;
            col._tb = prefix;
            std::string real_col = "\""+itcol+"\""; //Handling of special symbols

            //mask
            std::string node_str;
            do
            {
                MaskItem *pmask = match_maskitem(tb, &col, pmap_mask, ptb);
                if(!pmask){
                    break;
                }
                std::set<std::u16string> vec_col;
                bool bhav = get_valid_condition_fields_and_set_type(vec_col, ptb, pmask); /*todo vec_col not used and modify mask condition to empty*/
                if (!bhav)  {
                    theLog->WriteLog(log_warning, "mask_fields_star_prefix break for get_valid_condition_fields failed. condition:%s", pmask->_condition.c_str() );
                    break;
                }

                col._field = real_col;
                node_str = constrcut_mask_string(&col, pmask);

                if (!node_str.empty()){
                    pSelectAst->AddedMatchedMaskItem("", pmask);    
                }
                
                theLog->WriteLog(log_info, "mask_fields_star_prefix field:%s with constrcut_mask_string==%s", pmask->_field.c_str(), node_str.c_str());

            } while (false);


            if(!mask_value.empty()){
                mask_value += ',';
            }
            if (!node_str.empty()){
                mask_value += node_str;
                bret = true;
            } else {
                if(!prefix.empty()){
                    mask_value+=prefix;
                    mask_value += '.';
                }
                mask_value += real_col;
            }

        }
        break;
    }
    if(!mask_value.empty()){
        Node * pt = Node::makeTerminalNode(E_SPECIAL_TERMINAL, mask_value.c_str());
        *(field->_node)=*pt;
        delete(pt);
    } else {
        return false;
    }
    return  bret;
}