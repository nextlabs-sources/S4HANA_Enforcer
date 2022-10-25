#ifndef TABLE_ALIAS_ITEM
#define TABLE_ALIAS_ITEM

#include <string>
#include <map>
#include <vector>

class TableAliasItem{
public:
    TableAliasItem(const std::string& oTable, const std::string& aTable, const std::string& updateRel);
    void AddedFieldAlias(const std::string& oField, const std::string& aField);
    const std::string& OriginalTable() const { return _original_table; }
    const std::string& AliasTable() const { return _alias_table; }
    const std::string& UpdateRelation() const { return _update_relation; }

    std::string GetOriginalField(const std::string& aliasField) const;

private:
    std::string _original_table;
    std::string _alias_table;
    std::string _update_relation;

    ///std::map<std::string, std::vector<std::string>> _mapFieldAlias;
    std::map<std::string, std::string> _mapFieldAlias; // alias field-->original field
};


#endif 