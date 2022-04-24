//test

#include "token.hpp"
#include <cstring> //strchr, strtok
#include <cctype> //isspace
#include <algorithm> //find_if_not

size_t count_tokens(const std::string& src, const char* deli){ //計算字串中以deli其中一字元區分的token數量，deli預設為「空白字元」
	bool prev = false; //若左邊為「分隔字元」則為false；否則為true
	size_t count = 0;
	for(char c:src){
		count += (prev^(!strchr(deli,c)) ? prev^=true : 0);
	}
	return count;

	/*
	想法：
		欲判斷token的數量，會先在原字串前加上一個假想的空格，並從原字串最前方開始兩個兩個檢查。
		若檢查到的這兩個字元剛好是「左邊是『分隔字元』，而右邊是『非分隔字元』」，代表找到一個token。
	演算法：
		令prev為false，並令字元c從原字串最前方開始向後迭代：
			若prev和c為同性質的字元（皆為「分隔字元」或皆不是）
				即prev ^ (!strchr(deli,c)) 為false
				此時count加上0
			否則prev和c為不同性質的字元
				即prev ^ (!strchr(deli,c)) 為true
				此時有兩種情況：
					(1)prev原本為false
						這代表檢查的兩個字元中左邊是「分隔」且右邊是「非分隔」。
						在prev^=true後prev會變成true，所以count加上prev^=true型同加上1。
					(2)prev原本為true
						這代表檢查的兩個字元中左邊是「非分隔」且右邊是「分隔」。
						在prev^=true後prev會變成false，所以count加上prev^=true型同加上0。
	*/
}

size_t get_tokens(std::string src, token_t& token, const char* deli){
	const size_t token_num = count_tokens(src, deli);
	token.resize(token_num); //若resize失敗會丟出std::bad_alloc

	char* tok_str = strtok(const_cast<char*>(src.c_str()), deli);
	for(size_t i=0; i<token_num; ++i){
		token[i] = std::string(tok_str);
		tok_str = strtok(NULL, deli);
	}

	return token.size(); //若大小為0則轉型成false；否則轉型成true
}

size_t stosize_t(const std::string& src){
	if( *std::find_if_not(src.begin(),src.end(), isspace)=='-') //若數字以"-"開頭
		throw std::invalid_argument("stosize_t : negative not allowed");

	size_t pos;
	size_t num_to_return = std::stoull(src,&pos);
	if( std::find_if_not(src.begin()+pos, src.end(), isspace) != src.end() ) //若數字後還有「非空格」的字元
		throw std::invalid_argument("stosize_t : num only");

	return num_to_return;
}

long long stolonglong(const std::string& src){
	size_t pos;
	long long num_to_return = std::stoll(src,&pos);
	if( std::find_if_not(src.begin()+pos, src.end(), isspace) != src.end() ) //若數字後還有「非空格」的字元
		throw std::invalid_argument("stolonglong : num only");

	return num_to_return;
}