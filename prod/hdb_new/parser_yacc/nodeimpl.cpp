#include "node.h"
#include "parser.h"
#include "Stmt.h"
#include "resolve.h"
#include "ResultPlan.h"

bool CompressSql(const std::string& sql, std::string& out_sql) {
    return parser::compress_sql(sql, out_sql);
}

bool BeautifySql(const std::string& sql, std::string& out_sql) {
    return parser::beautify_sql(sql, out_sql);
}

IPlan* CreatePlan(
        const BaseTableVisit& baseTableVisit,
        const BaseTableColumnVisit& baseTableColumnVisit,
        const StartNewStmt& startNewStmt,
        const WhereClauseVisit& whereClauseVisit,
        const ErrorOccur& errorOccur,
        void* context, IParseResult* parseResult) {
    auto parser_result = dynamic_cast<ParseResult*>(parseResult);
    if (!parser_result) return nullptr;
    auto root = dynamic_cast<Node*>(parser_result->result_tree_);
    if (!root) return nullptr;
    resolve::ResultPlan* ret = new resolve::ResultPlan(
            baseTableVisit, baseTableColumnVisit,
            startNewStmt, whereClauseVisit, errorOccur);
    ret->context_ = context;
    ret->tree_root_ = root;
    ret->type_ = parser_result->type_;
    return ret;
}

void DestroyPlan(IPlan* plan) {
    delete(dynamic_cast<resolve::ResultPlan*>(plan));
}

void VisitPlan(IPlan* p) {
    resolve::ResultPlan* plan = dynamic_cast<resolve::ResultPlan*>(p);
    if (!plan)
        return;
    if (!plan->tree_root_)
        return;

    resolve::resolve_multi_statements(plan, plan->tree_root_);
}

IParseResult* ParseSql(const std::string &sql, DATABASE_TYPE dbms) {
    ParseResult* result = new ParseResult;
    result->type_ = dbms;
    switch (dbms) {
        case DBMS_HANA: {
            parser::parse_hana(sql, result);
        } break;
        case DBMS_TSQL: {
            parser::parse_tsql(sql, result);
        } break;
        default: {
            result->errDetail = "UNKNOWN DATABASE_TYPE";
        }
    }
    return result;
}

void DestroyParseResult(IParseResult* result) {
    delete(dynamic_cast<ParseResult*>(result));
}


