//
// Created by jeff on 2020/2/15.
//

#ifndef SQL_PARSE_TEST_PARSE_H
#define SQL_PARSE_TEST_PARSE_H


#include <string>
class ParseResult;
bool parse_hana(const std::string& sql, ParseResult* result);
bool beautify_sql(const std::string& src, std::string& out);
bool compress_sql(const std::string& src, std::string& out);


#endif //SQL_PARSE_TEST_PARSE_H
