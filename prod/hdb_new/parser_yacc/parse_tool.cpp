//
// Created by jeff on 2020/7/23.
//
#include "parse_tool.h"
#include "sqlparser_hana_bison.h"
#include "sqlparser_hana_flex.h"
#include "node.h"
#include "serialize_format.h"
#include "assert.h"

struct _Buf {
    _Buf() : line_(0), col_(0) {}
    /* no \n */
    void append(const std::string& ap) {
        s_ << ap;
        col_ += ap.length();
    }
    void new_line() {
        s_ << "\n";
        line_++;
        col_ = 0;
    }
    void adjust_col(int n) {
        append(std::string(n, ' '));
    }
    int get_col() const { return col_; }
    std::string get_str() const { return s_.str(); }
private:
    std::stringstream s_;
    int line_;
    int col_;
};

void _beautify_sql(_Buf *buf, Node *node) {
    int col_f = buf->get_col();
    if (!node->isTerminalToken) {
        for (auto info : *(node->serialize_format->beautify_)) {
            if (info.is_simple == 1) {
                if (info.s0 == "\n") {
                    buf->new_line();
                    buf->adjust_col(col_f);
                    buf->append(info.s2);
                }
                else {
                    buf->append(info.s0);
                }
            }
            else {
                Node* child = node->getChild(info.key);

                if (child) {
                    if (info.s0 == "\n") {
                        buf->new_line();
                        buf->adjust_col(col_f);
                    }
                    else
                        buf->append(info.s0);
                    _beautify_sql(buf, child);
                    if (info.s2 == "\n") {
                        buf->new_line();
                        buf->adjust_col(col_f);
                    }
                    else
                        buf->append(info.s2);
                }
            }
        }
    }
    else {
        buf->append(node->terminalToken_.yytex);
    }
}

bool parse_hana(const std::string& sql, ParseResult* result) {
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (hana_lex_init_extra(result, &scanner)) {
        //fprintf(stderr, "SQLParser2003: Error when initializing lexer!\n");
        return false;
    }

    const char* text = sql.c_str();
    state = hana__scan_string(text, scanner);

    // Parse the tokens.
    // If parsing fails, the result will contain an error object.
    hana_parse(result, scanner);
    hana__delete_buffer(state, scanner);
    hana_lex_destroy(scanner);

    return result->accept;
}

bool beautify_sql(const std::string& src, std::string& out) {
    ParseResult r;
    if (!parse_hana(src, &r))
        return false;
    if (!r.result_tree_)
        return true;
    _Buf b;
    _beautify_sql(&b, r.result_tree_);
    out = b.get_str();
    return true;
}

bool compress_sql(const std::string& src, std::string& out) {
    ParseResult r;
    if (!parse_hana(src, &r)){
        out = r.errDetail;
        return false;
    }
    if (!r.result_tree_)
        return true;
    out = r.result_tree_->serialize();
    return true;
}