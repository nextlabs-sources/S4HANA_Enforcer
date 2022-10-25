#ifndef TABLE_META_DATA_IMPL_H
#define TABLE_META_DATA_IMPL_H

#include <vector>
#include <string>
#include "interface.h"

class ITableMetaData;

struct MetaDataItem {
    //enum COLUMN_TYPE { COLUMN_TYPE_STRING, COLUMN_TYPE_NUMBER, COLUMN_TYPE_OTHER };
    EM_SAP_DataType _column_type;
    std::string _column_name;
    std::string _column_type_text;  
    bool _KeyFlag;  //is this is a primary key
};

ITableMetaData *create_table_metadata(void *connection, const std::string& table_name, EMTableType emTbType,  S4HException& e);

class TableMetaDataImpl : public ITableMetaData {
public:
    virtual std::string GetTableName() const override;
    virtual bool HasColumn(const std::string& column_name) const override;
    virtual bool IsColumnCompatibleWithString(const std::string& column_name) const override;
    virtual EM_SAP_DataType GetColumnDataType(const std::string& column_name) const;
    virtual void GetAllColumns(std::vector<std::string>& vecCol) const override;
    virtual std::vector<std::string> GetAllKeyColumns() const override;
    virtual std::string Print()const ;
private:
    std::vector<MetaDataItem>   _meta_data;
    std::string              _table_name;
    friend ITableMetaData *create_table_metadata(void *connection, const std::string& table_name, EMTableType emTbType, S4HException& e);
};

#endif