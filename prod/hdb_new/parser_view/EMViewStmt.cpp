//
// Created by jeff on 2020/5/14.
//

#include "EMViewStmt.h"
#include "EMTableExpr.h"
#include "tool.h"
EMViewStmt::~EMViewStmt(){
    _ctes.clear();
    for(auto it:_columns){
        delete(it);
    }
    _columns.clear();
    for(auto it:_tables){
        delete(it);
    }
    _tables.clear();
}

void EMViewStmt::GetAllTableName(std::vector<EMTable> & tables){
    if( _ctes.size() > 0){
        for(auto it:_ctes){
            it.second->GetAllTableName(tables);
        }
    }else {
        for(auto it:_tables){
            switch (it->GetType()){
                case ITableExpr::TB_IDENT:{
                    EMTableExpr_Ident * tbi = dynamic_cast<EMTableExpr_Ident*>(it);
                    if(_ctes.find(tbi->_table._name) == _ctes.end()){
                        tables.push_back(tbi->_table);
                    }
                } break;
                case ITableExpr::TB_SUBQUERY:{
                    EMTableExpr_Select * tbi = dynamic_cast<EMTableExpr_Select*>(it);
                    if(tbi->_stmt && tbi->_stmt->GetType() == IViewStmt::ST_SELECT){
                        tbi->_stmt ->GetAllTableName(tables);
                    }
                }break;
                default:
                    break;
            }

        }
    }


}



const ViewSourceInfo& EMViewStmt::GetViewSources(void *pconn, S4HException & exc){
    if(_info.GetTablesSize() == 0){
        ParserViewSources(pconn, exc);
    }
    return _info;
}

#include "PolicyManager.h"
void EMViewStmt::ParserViewSources(void *pconn, S4HException & exc){
    for(auto itc: _columns){
        std::string field = itc->_name;
        std::string qualify = itc->_tb;
        if(field.compare("*") == 0){
            if(qualify.empty()){
                //analyze table
                for(auto tbi: _tables){
                    switch (tbi->GetType()){
                        case ITableExpr::TB_IDENT:{
                            EMTableExpr_Ident * tb = dynamic_cast<EMTableExpr_Ident*>(tbi);
                            auto it = _ctes.find(tb->_table._name);
                            if(it == _ctes.end()){

                                const std::string& ss = tb->_table._name;
                                const ITableMetaData * meta = PolicyManager::Instance()->GetTableMetaData(ss, EMTABLE_VIEW, pconn, exc);
                                if(meta){
                                    std::vector<std::string> metacols;
                                    meta->GetAllColumns(metacols) ;
                                    std::string metatb = meta->GetTableName();
                                    for(auto itm: metacols){
                                        ViewColumnSourceInfo info(itm.c_str(), metatb.c_str(), itm.c_str());
                                        _info.AddedSourceTable(metatb.c_str());
                                        _info.AddedSourceColumn(itm.c_str(), info);

                                    }
                                }

                            } else {
                                //collect cte all column
                                const ViewSourceInfo& info_sub = it->second->GetViewSources(pconn, exc);
                                _info.AddedSources(info_sub);

                            }
                        } break;
                        case ITableExpr::TB_SUBQUERY:{
                            EMTableExpr_Select * tb = dynamic_cast<EMTableExpr_Select*>(tbi);
                            if(tb->_stmt && tb->_stmt->GetType() == IViewStmt::ST_SELECT){
                                const ViewSourceInfo& info_sub = tb->_stmt->GetViewSources(pconn, exc);
                                _info.AddedSources(info_sub);
                            }
                        }break;
                        default:
                            break;
                    }
                }
            } else {
                //analyze table
                bool bfind = false;
                for(auto tbi: _tables){
                    switch (tbi->GetType()){
                        case ITableExpr::TB_IDENT:{
                            EMTableExpr_Ident * tb = dynamic_cast<EMTableExpr_Ident*>(tbi);
                            if(!tb->_table._alias.empty()){
                                if(tb->_table._alias.compare(qualify) != 0){
                                    continue;
                                }
                            } else if(qualify.compare(tb->_table._name) != 0){
                                continue;
                            }


                            auto it = _ctes.find(tb->_table._name);
                            if(it == _ctes.end()){
                                const std::string& ss = tb->_table._name;
                                const ITableMetaData * meta = PolicyManager::Instance()->GetTableMetaData(ss, EMTABLE_VIEW, pconn, exc);
                                if(meta){
                                    std::vector<std::string> metacols ;
                                    meta->GetAllColumns(metacols) ;
                                    std::string metatb = meta->GetTableName();
                                    for(auto itm: metacols){
                                        ViewColumnSourceInfo info(itm.c_str(), metatb.c_str(), itm.c_str());
                                        _info.AddedSourceTable(metatb.c_str());
                                        _info.AddedSourceColumn(itm.c_str(), info);
                                    }
                                }
                            } else {
                                //collect cte all column
                                const ViewSourceInfo& info_sub = it->second->GetViewSources(pconn, exc);
                                _info.AddedSources(info_sub);

                            }
                            bfind = true;
                        } break;
                        case ITableExpr::TB_SUBQUERY:{
                            EMTableExpr_Select * tb = dynamic_cast<EMTableExpr_Select*>(tbi);
                            if(tb->_alias.empty()){
                                if(tb->_alias.compare(qualify) != 0){
                                    continue;
                                }
                            }
                            if(tb->_stmt && tb->_stmt->GetType() == IViewStmt::ST_SELECT){
                                const ViewSourceInfo& info_sub = tb->_stmt->GetViewSources(pconn, exc);
                                _info.AddedSources(info_sub);
                            }
                            bfind = true;
                        }break;
                        default:
                            break;
                    }
                    if(bfind){
                        break;
                    }
                }
            }
        } else {

            if(qualify.empty()){
                if(_tables.size() == 1) {
                    //no need query table metadata.
                    ITableExpr * tbi = _tables[0];
                    switch (tbi->GetType()){
                        case ITableExpr::TB_IDENT:{

                            EMTableExpr_Ident * tb = dynamic_cast<EMTableExpr_Ident*>(tbi);

                            auto it = _ctes.find(tb->_table._name);
                            if(it == _ctes.end()){

                                std::string stable = construct_key(tb->_table);
                                _info.AddedSourceTable(stable.c_str());
                                std::string scolname = get_view_column(*itc);
                                ViewColumnSourceInfo info(scolname.c_str(), stable.c_str(), field.c_str());
                                _info.AddedSourceColumn(scolname.c_str(), info);

                            } else {
                                //collect cte all column
                                const ViewSourceInfo& info_sub = it->second->GetViewSources(pconn, exc);
                                const ViewColumnSourceInfo  * info = info_sub.GetSourceColumn(field);

                                if(info != NULL){
                                    _info.AddedSourceTable(info->SourceTableName().c_str());
                                    _info.AddedSourceColumn(field.c_str(), *info);
                                }
                            }
                        } break;
                        case ITableExpr::TB_SUBQUERY:{
                            EMTableExpr_Select * tb = dynamic_cast<EMTableExpr_Select*>(tbi);

                            if(tb->_stmt && tb->_stmt->GetType() == IViewStmt::ST_SELECT){
                                const ViewSourceInfo& info_sub = tb->_stmt->GetViewSources(pconn, exc);
                                const ViewColumnSourceInfo  * info = info_sub.GetSourceColumn(field);
                                if(info != NULL){
                                    _info.AddedSourceTable(info->SourceTableName().c_str());
                                    _info.AddedSourceColumn(field.c_str(), *info);
                                }
                            }
                        }break;
                        default:
                            break;
                    }

                } else {

                    for(auto tbi: _tables){
                        switch (tbi->GetType()){
                            case ITableExpr::TB_IDENT:{

                                EMTableExpr_Ident * tb = dynamic_cast<EMTableExpr_Ident*>(tbi);

                                auto it = _ctes.find(tb->_table._name);
                                if(it == _ctes.end()){

                                    const std::string& ss = tb->_table._name;
                                    const ITableMetaData * meta = PolicyManager::Instance()->GetTableMetaData(ss,EMTABLE_VIEW, pconn, exc);
                                    if(meta){
                                        std::vector<std::string> metacols;
                                        meta->GetAllColumns(metacols) ;
                                        std::string metatb = meta->GetTableName();
                                        for(auto itm: metacols){
                                            if(itm.compare(field) == 0){
                                                std::string scolname = get_view_column(*itc);
                                                ViewColumnSourceInfo info(scolname.c_str(), metatb.c_str(), field.c_str());
                                                _info.AddedSourceTable(metatb.c_str());
                                                _info.AddedSourceColumn(scolname.c_str(), info);
                                                break;
                                            }
                                        }
                                    }
                                    
                                } else {
                                    //collect cte all column
                                    const ViewSourceInfo& info_sub = it->second->GetViewSources(pconn, exc);
                                    const ViewColumnSourceInfo  * info = info_sub.GetSourceColumn(field);

                                    if(info != NULL){
                                        _info.AddedSourceTable(info->SourceTableName().c_str());
                                        _info.AddedSourceColumn(field.c_str(), *info);
                                    }
                                }
                            } break;
                            case ITableExpr::TB_SUBQUERY:{
                                EMTableExpr_Select * tb = dynamic_cast<EMTableExpr_Select*>(tbi);

                                if(tb->_stmt && tb->_stmt->GetType() == IViewStmt::ST_SELECT){
                                    const ViewSourceInfo& info_sub = tb->_stmt->GetViewSources(pconn, exc);
                                    const ViewColumnSourceInfo  * info = info_sub.GetSourceColumn(field);
                                    if(info != NULL){
                                        _info.AddedSourceTable(info->SourceTableName().c_str());
                                        _info.AddedSourceColumn(field.c_str(), *info);
                                    }
                                }
                            }break;
                            default:
                                break;
                        }
                    }
                }
            } else {//have  qualify
                auto itQualify = _ctes.find(qualify);
                if( itQualify == _ctes.end()){  // if  not is  cte
                    bool bfind = false;
                    //is table / subquery
                    for(size_t i = 0 ; i < _tables.size(); ++i){
                        ITableExpr * tbi = _tables[i];
                        switch (tbi->GetType()){
                            case ITableExpr::TB_IDENT:{

                                EMTableExpr_Ident * tb = dynamic_cast<EMTableExpr_Ident*>(tbi);
                                if(tb->_table._name.compare(qualify)!=0 && tb->_table._alias.compare(qualify) != 0){
                                    break;
                                }

                                auto itTableName = _ctes.find(tb->_table._name);
                                if(itTableName == _ctes.end()){

                                    std::string stable = construct_key(tb->_table);
                                    _info.AddedSourceTable(stable.c_str());
                                    std::string scolname = get_view_column(*itc);
                                    ViewColumnSourceInfo info(scolname.c_str(), stable.c_str(), field.c_str());
                                    _info.AddedSourceColumn(scolname.c_str(), info);

                                } else {
                                    //collect cte all column
                                    const ViewSourceInfo& info_sub = itTableName->second->GetViewSources(pconn, exc);
                                    const ViewColumnSourceInfo  * info = info_sub.GetSourceColumn(field);

                                    if(info != NULL){
                                        _info.AddedSourceTable(info->SourceTableName().c_str());
                                        _info.AddedSourceColumn(field.c_str(), *info);
                                    }
                                }
                                bfind = true;
                            } break;
                            case ITableExpr::TB_SUBQUERY:{
                                EMTableExpr_Select * tb = dynamic_cast<EMTableExpr_Select*>(tbi);

                                if(tb->_alias.compare(qualify) != 0){//todo subquery must be hav alias
                                    break;
                                }
                                if(tb->_stmt && tb->_stmt->GetType() == IViewStmt::ST_SELECT){
                                    const ViewSourceInfo& info_sub = tb->_stmt->GetViewSources(pconn, exc);
                                    const ViewColumnSourceInfo  * info = info_sub.GetSourceColumn(field);
                                    if(info != NULL){
                                        //_info.AddedSourceTables(info_sub.GetTables());
                                        _info.AddedSourceTable(info->SourceTableName().c_str());
                                        _info.AddedSourceColumn(field.c_str(), *info);
                                    }
                                }
                                bfind = true;
                            }break;
                            default:
                                break;
                        }
                        if(bfind){
                            break;
                        }
                    }
                } else {
                    const ViewSourceInfo& info_sub = itQualify->second->GetViewSources(pconn, exc);
                    const ViewColumnSourceInfo  * info = info_sub.GetSourceColumn(field);

                    if(info != NULL){
                        _info.AddedSourceTable(info->SourceTableName().c_str());
                        _info.AddedSourceColumn(field.c_str(), *info);
                    }
                }
            }
        }
    }
}