#include "TableAliasItem.h"

TableAliasItem::TableAliasItem(const std::string& oTable, const std::string& aTable, const std::string& updateRel)
{
   _original_table = oTable;
   _alias_table = aTable;
   _update_relation = updateRel;
}

void TableAliasItem::AddedFieldAlias(const std::string& oField, const std::string& aField)
{
    _mapFieldAlias[aField]=oField;
}

std::string TableAliasItem::GetOriginalField(const std::string& aliasField) const
{
    auto itField = _mapFieldAlias.find(aliasField);
    if (itField != _mapFieldAlias.end() )
    {
        return itField->second;
    }
    return "";
}