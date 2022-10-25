#ifndef VIEW_MANAGER_H_20200509
#define VIEW_MANAGER_H_20200509
#include <set>
#include <string>
#include <map>
#include <vector>
#include "SQLDBC.h"
#include <memory>

class ViewInfo;

typedef std::map<std::string,ViewInfo*> MAP_VIEWINFO;

//view source info
class ViewColumnSourceInfo{
public:
    ViewColumnSourceInfo(const char* column, const char* srcTable, const char* srcColumn):
    _columnName(column), _sourceTableName(srcTable), _sourceColumnName(srcColumn)
    {}

    const std::string& ColumnName() const { return _columnName; }
    const std::string& SourceTableName() const { return _sourceTableName; }
    const std::string& SourceColumnName() const { return _sourceColumnName; }

    void SetSourceTableName(const char* szTableName) { _sourceTableName = szTableName; }
    void SetSourceColumnName(const char* szColumnName) { _sourceColumnName = szColumnName; }

private:
    std::string _columnName;
    std::string _sourceTableName;
    std::string _sourceColumnName;
};

class ViewSourceInfo{
public:
    void AddedSourceTable(const char* szTable);
    void AddedSourceColumn(const char* szColumn, const ViewColumnSourceInfo& columnSrcInfo);
    void AddedSources(const ViewSourceInfo & info);
    size_t GetTablesSize();
    const std::string Print(const std::string& viewName) const;
    const ViewColumnSourceInfo* GetSourceColumn(const std::string& column) const;
    const std::set<std::string>* GetSourceTables() const { return &_sourceTables; }
    const std::map<std::string,ViewColumnSourceInfo> * GetSourceInfo()const{ return &_SourceColumns; }
    std::vector<const ViewColumnSourceInfo*> MatchSourceInfo(const std::string & col, const std::string & table, const std::string & tbcol) const;

    void ReplaceSourceInfo(const std::string& strTable, const ViewSourceInfo& dstSourceInfo);

    void ClearSourceInfo();

private:
    std::set<std::string>  _sourceTables;  //the table name should be: schema.table or table if schema is empty
    std::map<std::string,ViewColumnSourceInfo> _SourceColumns; //the key is column name of the view. because the column name of the view can't be same. so it can be used as key
};

enum NEED_ENFORCE{
     ENFORCE_UNKNOW,
     ENFORCE_NEED,
     ENFORCE_NONEED,
};

class ViewInfo{
public:
    ViewInfo(const char* szSchemaName, const char* szViewName, const char* szViewDef){
        _schemaName = szSchemaName;
        _viewName = szViewName;
        _viewDefinition = szViewDef;
        _pSourceInfo = NULL;
        _source_last_update = 0;
        _update_time = 0;
        _bSourceInfoPrinted = false;
    }

    const std::string& SchemaName() { return _schemaName; }
    const std::string& ViewName() { return _viewName; }
    const std::string& ViewDefinition() { return _viewDefinition; }
    const std::shared_ptr<ViewSourceInfo> SourceInfo() { return _pSourceInfo; }
    
    void SetSourceInfo(ViewSourceInfo* pSource) { _pSourceInfo.reset(pSource); }

    void SetSourceUpdateTime(time_t t) { _source_last_update= t ;}
    time_t GetSourceUpdateTime() const { return _source_last_update; }

    void SetUpdateTime(time_t t) { _update_time = t; }
    time_t GetUpdateTime() { return _update_time; }

    std::string Print(const char *szPrefix) const;
    bool IsSourceInfoPrinted() { return _bSourceInfoPrinted; }
    void SetSourceInfoPrinted(bool b) { _bSourceInfoPrinted=b; }

private:
    std::string _schemaName;
    std::string _viewName;
    std::string _viewDefinition;
    std::shared_ptr<ViewSourceInfo> _pSourceInfo;

    time_t _update_time;
    time_t _source_last_update;//
    bool  _bSourceInfoPrinted;
};

class ViewManager{
public:
    static ViewManager* GetInstance(){
        if (NULL==_pViewMgr){ //this will be execute in single thread env. so no need to consider multi thread.
            _pViewMgr = new ViewManager();
        }
        return _pViewMgr;
    }

    bool IsViewInfoInited(){ return !_allViewInfo.empty(); }
    void InitViewInfo(SQLDBC::SQLDBC_Connection* pConn);
    void CycleUpdate(void* pDBConn);

    ViewInfo* GetViewInfoFromDefinition(const char* schemaName, const std::string& viewName, void * pconn, bool recursion=true);
    ViewInfo* GetViewInfoFromDDL27S( const std::string& viewName, void * pconn);
    bool QueryViewSourceInfo(const std::string& viewName, void * pconn, ViewSourceInfo& sourceInfo);


private:
    ViewManager(){
        _pViewMgr = NULL;
        _time_last_update = 0;
    }
    double GetViewClearCycle();
    void GetAllViewInfo(SQLDBC::SQLDBC_Connection* pdbConn);
    bool IsViewSourceExpired(const ViewInfo* pViewInfo);


private:
    std::map<std::string,ViewInfo*>  _allViewInfo;  //cached all view info that we need to 

    static ViewManager* _pViewMgr;

	time_t _time_last_update;//
};

#endif 