#ifndef MASK_STRATEGY_IMPL_H
#define MASK_STRATEGY_IMPL_H

#include "interface.h"
#include <map>
#include "StructDefine.h"
#include "EMMaskDef.h"


//struct SQLCondition {
//
//    enum SQL_OP {SQL_GT = 0, SQL_GTEQ, SQL_LT, SQL_LTEQ, SQL_EQ, SQL_NEQ} ;
//    static SQL_OP TransformOp(const std::string & sop, S4HException & e) ;
//    std::string                      _column1;
//    SQL_OP                              _op1;
//    std::string                      _constant_value1;
//    std::string                      _column2;
//    SQL_OP                              _op2;
//    std::string                      _constant_value2;
//    std::string                      _column3;
//    SQL_OP                              _op3;
//    std::string                      _constant_value3;
//};

class MaskStrategyImpl : public IMaskStrategy {
public:
    virtual std::string GetMaskedColumn() const override { return _masked_column; }
    //virtual std::u16string MakeExpression(const std::u16string& scope, const std::u16string& alias) override;
    virtual std::u16string MakeExpressionV2(const std::set<std::u16string> & vec_col, const std::u16string& scope, const std::u16string& alias) ;
    virtual void SetMaskSymbolsType(EM_SAP_DataType type) {_masked_symbols._type = type;}
    virtual MaskSymbols GetMaskSymbols() { return _masked_symbols; };
    virtual void SetMaskSymbols(MaskSymbols & sym) {_masked_symbols = sym; };

    virtual void  SetMaskedTable(const std::string & table ) override { _mask_table = table;    }
    virtual const std::string& GetMaskedTable() const override { return _mask_table; }

    virtual const std::set<std::string> & GetMaskedTables() const {
        return _mask_tables;
    };
    virtual void  SetMaskedTables(const std::set<std::string> & tables ) {
        _mask_tables.insert(tables.begin(),tables.end());
    }

    virtual EM_SAP_DataType GetMaskConditionColumnType(const std::string &column) const override {
        auto it = _columnTypes.find(column);
        return it != _columnTypes.cend() ? _columnTypes.at(column) : EM_SAP_VARCHAR;
    }
    virtual void SetMaskConditionColumnType(const std::string &column, EM_SAP_DataType type) override {
        _columnTypes[column] = type;
    }

    virtual std::map<std::string, int> GetConditionColumnAndOp();
    virtual std::string Print()const;
private:
    std::set<std::string>   _mask_tables;
    std::string                 _mask_table;
    std::string                  _masked_column;   //one field.
    //std::string                  _masked_symbols;
    MaskSymbols             _masked_symbols;
    std::vector<SQLCondition>       _conditions;

    std::map<std::string, EM_SAP_DataType> _columnTypes;

    friend MaskStrategyImpl * create_maskstrategy(const std::string & maskcol, const std::string & symbols, const std::vector<SQLCondition> &  conds);

};
MaskStrategyImpl * create_maskstrategy(const std::string & maskcol, const std::string & symbols, const std::vector<SQLCondition> &  conds);

#endif