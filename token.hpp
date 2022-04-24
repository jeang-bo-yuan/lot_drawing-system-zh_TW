//test

//token.hpp
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <vector>
#include <string>
#include <stdexcept> //std::invalid_argument

typedef std::vector<std::string> token_t;

//計算字串中「以deli其中一字元區分」的token數量
size_t count_tokens(const std::string& src, const char* deli = " \t\n");
//從src提取「以deli其中一字元區分」的tokens並存入token容器中，並回傳token的數量
size_t get_tokens(std::string src, token_t& token, const char* deli = " \t\n");

//轉型成size_t，並檢查是否包含多餘的文字和負號
size_t stosize_t(const std::string&);
//轉型成long long，並檢查是否包含多餘的文字
long long stolonglong(const std::string&);

/*
可能丟出的例外
    count_tokens| ???
    get_tokens  | std::bad_alloc（若來源字串包含過多的token）
    stosize_t   | std::invalid_argument、std::out_of_range
*/

#endif