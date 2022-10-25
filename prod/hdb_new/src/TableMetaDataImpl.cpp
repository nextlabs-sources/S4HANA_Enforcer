#include "talk_with_hdb.h"
#include "../parser_yacc/EMS4HAst.h"
#include "TableMetaDataImpl.h"
#include "tool.h"
#include "log.h"


std::string TableMetaDataImpl::GetTableName() const {
    return _table_name;
}

bool TableMetaDataImpl::HasColumn(const std::string& column_name) const {
    for (auto& it : _meta_data) {
        if (CommonFun::StrCaseCmp(it._column_name, column_name) == 0) return true;
    }
    return false;
}

bool TableMetaDataImpl::IsColumnCompatibleWithString(const std::string& column_name) const {
    for (auto& it : _meta_data) {
        if (CommonFun::StrCaseCmp(it._column_name, column_name) == 0) {
            if(it._column_type == EM_SAP_VARCHAR
            ||it._column_type == EM_SAP_NVARCHAR
              ||it._column_type == EM_SAP_ALPHANUM
                ||it._column_type == EM_SAP_SHORTTEXT)
                return true;
            else
                return false;
        }
    }
    return false;
}

EM_SAP_DataType TableMetaDataImpl::GetColumnDataType(const std::string& column_name) const {
    for (auto& it : _meta_data) {
        if (CommonFun::StrCaseCmp(it._column_name, column_name) == 0)
            return it._column_type;
    }
    return EM_SAP_OTHER;
}

void TableMetaDataImpl::GetAllColumns(std::vector<std::string>& vecCol) const {
    for (auto itMetaData=_meta_data.begin(); itMetaData!=_meta_data.end(); itMetaData++) {
          vecCol.push_back(itMetaData->_column_name);
    }
}

std::vector<std::string> TableMetaDataImpl::GetAllKeyColumns() const 
{
    std::vector<std::string> ret;
    for (auto &it : _meta_data){
        if(it._KeyFlag){
            ret.push_back(it._column_name);
        }
    }
    return ret;
}

EM_SAP_DataType get_type(const char* data_type_name) {
    /* todo */
    if (0 == CommonFun::StrCaseCmp(data_type_name, "VARCHAR") ){
        return EM_SAP_VARCHAR;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "NVARCHAR") ){
        return EM_SAP_NVARCHAR;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "ALPHANUM") ){
        return EM_SAP_ALPHANUM;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "SHORTTEXT") ){
        return EM_SAP_SHORTTEXT;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "TINYINT") ){
        return EM_SAP_TINYINT;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "SMALLINT") ){
        return EM_SAP_SMALLINT;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "BIGINT") ){
        return EM_SAP_BIGINT;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "INTEGER")) {
        return EM_SAP_INTEGER;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "DECIMAL") ){
        return EM_SAP_DECIMAL;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "DOUBLE") ){
        return EM_SAP_DOUBLE;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "SMALLDECIMAL") ){
        return EM_SAP_SMALLDECIMAL;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "REAL")) {
        return EM_SAP_REAL;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "TIMESTAMP")) {
        return EM_SAP_TIMESTAMP;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, "BLOB")) {
        return EM_SAP_BLOB;
    }else {
        return EM_SAP_OTHER;
    }

}

#ifndef TEST
ITableMetaData *create_table_metadata(void *connection, const std::string& table_name,  EMTableType emTbType, S4HException& e) {
    if (nullptr == connection) {
        e.SetError(S4H_INVALID_CONNECTION, "INVALID CONNECTION!");
        return nullptr;
    }
    if (table_name.length() == 0) {
        e.SetError(S4H_INVALID_TABLE_NAME, "INVALID TABLE NAME!");
        return nullptr;
    }
    auto hdb_connection = (SQLDBC::SQLDBC_Connection*)connection;
    auto hdb_stmt = SQLDBC_Connection_createPreparedStatement_Old(hdb_connection);

    std::string query ;
    if(emTbType==EMTABLE_FUNC){
        query =  "SELECT COLUMN_NAME, POSITION, DATA_TYPE_NAME FROM SYS.FUNCTION_PARAMETER_COLUMNS  WHERE SCHEMA_NAME = CURRENT_USER AND FUNCTION_NAME=? AND FUNCTION_NAME=?  ORDER BY POSITION; " ;
    } 
    else{
        query =  "SELECT COLUMN_NAME, DATA_TYPE_NAME, KEYFLAG\n"
                "    FROM ( SELECT COLUMN_NAME, SYSCOLS.POSITION, DATA_TYPE_NAME, SAPCOLS.KEYFLAG, SAPCOLS.LENG, SAPCOLS.DECIMALS FROM SYS.TABLE_COLUMNS AS SYSCOLS INNER JOIN DD03L AS SAPCOLS ON SYSCOLS.TABLE_NAME=SAPCOLS.TABNAME AND SYSCOLS.COLUMN_NAME=SAPCOLS.FIELDNAME WHERE SCHEMA_NAME=CURRENT_USER AND SYSCOLS.TABLE_NAME =  ?\n"
                "    UNION  SELECT COLUMN_NAME, SYSCOLS.POSITION, DATA_TYPE_NAME, SAPCOLS.KEYFLAG, SAPCOLS.LENG, SAPCOLS.DECIMALS FROM SYS.VIEW_COLUMNS  AS SYSCOLS INNER JOIN DD03L AS SAPCOLS ON SYSCOLS.VIEW_NAME=SAPCOLS.TABNAME  AND SYSCOLS.COLUMN_NAME=SAPCOLS.FIELDNAME WHERE SCHEMA_NAME=CURRENT_USER AND SYSCOLS.VIEW_NAME =  ? )\n"
                "            ORDER BY POSITION;";
    }
                             
    SQLDBC_Retcode rc = prepare_3_old(hdb_stmt, (char*)query.c_str(), query.size()*2, SQLDBC_StringEncodingType::Ascii);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
        return nullptr;
    }

    rc = bind_parameter_old(hdb_stmt, 1, SQLDBC_HOSTTYPE_ASCII, (void*)table_name.c_str(), nullptr, 0, SQLDBC_TRUE);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection ,hdb_stmt);
        return nullptr;
    }
    rc = bind_parameter_old(hdb_stmt, 2, SQLDBC_HOSTTYPE_ASCII, (void*)table_name.c_str(), nullptr, 0, SQLDBC_TRUE);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
        return nullptr;
    }

    rc = execute_old(hdb_stmt);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old(&SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
        return nullptr;
    }
    auto result_set = get_result_set_old(hdb_stmt);
    if (result_set == nullptr) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection , hdb_stmt);
        return nullptr;
    }
    rc = SQLDBC_ResultSet_next_Old(result_set);
    if(SQLDBC_OK != rc) {
        e._code = S4H_INVALID_TABLE_NAME;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(result_set)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection, hdb_stmt);
        return nullptr;
    }
    TableMetaDataImpl *impl = new TableMetaDataImpl;
    impl->_table_name = table_name;
    do {
        char column_name[64] = { 0 };
        char column_type_name[64] = { 0 };
        char key_flag[64] = { 0 };
        SQLDBC_ResultSet_getObject_Old(result_set, 1, SQLDBC_HOSTTYPE_ASCII, column_name, nullptr, sizeof(column_name), SQLDBC_TRUE);
        SQLDBC_ResultSet_getObject_Old(result_set, 2, SQLDBC_HOSTTYPE_ASCII, column_type_name, nullptr, sizeof(column_type_name), SQLDBC_TRUE);
        SQLDBC_ResultSet_getObject_Old(result_set, 3, SQLDBC_HOSTTYPE_ASCII, key_flag, nullptr, sizeof(key_flag), SQLDBC_TRUE);
        MetaDataItem item;
        item._column_name = column_name;
        item._column_type_text = column_type_name;
        item._column_type = get_type(column_type_name);  
        item._KeyFlag = key_flag[0]=='X' || key_flag[0]=='x';
        impl->_meta_data.push_back(item);
    } while (SQLDBC_ResultSet_next_Old(result_set) == SQLDBC_OK);
    e._code = S4H_SUCCESS;
    SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
    return impl;
}
#else
ITableMetaData *create_table_metadata(void *connection, const std::string &table_name, S4HException &e) {

    if (nullptr == connection) {
        e._code = S4H_INVALID_CONNECTION;
        e._detail = u"INVALID CONNECTION!";
        return nullptr;
    }
    if (table_name.length() == 0) {
        e._code = S4H_INVALID_TABLE_NAME;
        e._detail = u"INVALID TABLE NAME!";
        return nullptr;
    }
    auto hdb_connection = (SQLDBC::SQLDBC_Connection *) connection;
    auto hdb_stmt = SQLDBC_Connection_createPreparedStatement_Old(hdb_connection);

    std::u16string query = u"SELECT COLUMN_NAME, DATA_TYPE_NAME\n"
                           "    FROM (     SELECT COLUMN_NAME, POSITION, DATA_TYPE_NAME FROM SYS.TABLE_COLUMNS WHERE SCHEMA_NAME = CURRENT_USER AND TABLE_NAME =  ?\n"
                           "        UNION  SELECT COLUMN_NAME, POSITION, DATA_TYPE_NAME FROM SYS.VIEW_COLUMNS  WHERE SCHEMA_NAME = CURRENT_USER AND VIEW_NAME =  ?  )\n"
                           "            ORDER BY POSITION;";
    SQLDBC_Retcode rc = hdb_stmt->prepare( (char *) query.c_str(), SQLDBC_StringEncodingType::UCS2Swapped);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
        return nullptr;
    }

    rc = hdb_stmt->bindParameter( 1, SQLDBC_HOSTTYPE_UCS2_SWAPPED, (void *) table_name.c_str(), nullptr, 0,
                                  SQLDBC_TRUE);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
        return nullptr;
    }
    rc = hdb_stmt->bindParameter(2, SQLDBC_HOSTTYPE_UCS2_SWAPPED, (void *) table_name.c_str(), nullptr, 0,
                                 SQLDBC_TRUE);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
        return nullptr;
    }
    rc =  execute_old(hdb_stmt);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
        return nullptr;
    }
    auto result_set = get_result_set_old(hdb_stmt);
    if (result_set == nullptr) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
        return nullptr;
    }
    rc = SQLDBC_ResultSet_next_Old(result_set);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_TABLE_NAME;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
        return nullptr;
    }
    TableMetaDataImpl *impl = new TableMetaDataImpl;
    impl->_table_name = table_name;
    do {
        char16_t column_name[64] = {0};
        char16_t column_type_name[64] = {0};
        SQLDBC_ResultSet_getObject_Old(result_set, 1, SQLDBC_HOSTTYPE_UCS2_SWAPPED, column_name, nullptr, sizeof(column_name));
        SQLDBC_ResultSet_getObject_Old(result_set, 2, SQLDBC_HOSTTYPE_UCS2_SWAPPED, column_type_name, nullptr, sizeof(column_type_name));
        MetaDataItem item;
        item._column_name = column_name;
        item._column_type = get_type(column_type_name);
        impl->_meta_data.push_back(item);
    } while (SQLDBC_ResultSet_next_Old(result_set) == SQLDBC_OK);
    e._code == S4H_SUCCESS;
    SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
    return impl;

}
#endif

std::string TableMetaDataImpl::Print() const{
    std::string ret;
    ret+="( tablename: ";
    ret+=_table_name;
    ret+=", columns: ";
	char szMeta[500] = { 0 };
    for(auto meta:_meta_data) {
		memset(szMeta, 0, sizeof(szMeta));
		sprintf_s(szMeta, sizeof(szMeta), "(%s, %s, %d)", meta._column_name.c_str(), meta._column_type_text.c_str(), meta._column_type);
		ret += szMeta;
    }
    ret+=" )";

    return  ret;

}


