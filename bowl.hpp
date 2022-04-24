#ifndef BOWL_HPP
#define BOWL_HPP

#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept> // std::range_error
class bowl_t {
	private:
	  std::vector<std::string> d_lots; //儲存籤的容器
	  size_t d_id; //紀錄籤筒中剩餘的第一隻籤的註標位置
	
	public:
	// ctor
	  bowl_t()
			: d_lots(),d_id(0)
		{}
	  bowl_t(const bowl_t &) = delete;
	  bowl_t(bowl_t &&) = delete;
	
	// modifing
	  //加入
	  void add(const std::string& v){d_lots.push_back(v);} 
	  //加入某個範圍的數字到籤筒中
	  void add_range(long long, long long);
	  //移除指定數量的籤，回傳實際移除的數量
	  size_t remove(const std::string &, size_t = 1); 
	  //移除最近加入的籤，回傳實際移除的數量。若在bowl_t::draw被呼叫後呼叫此函式，則其行為未定義
	  size_t unadd(size_t num);
	
	// functionality
	  //列出剩餘的籤
	  std::ostream& list(std::ostream&); 
	  //抽出一隻籤並輸出
	  std::ostream& draw(std::ostream&); 
	  //重置籤筒
	  void restart(void) noexcept{d_id=0;} 
	  //判斷是否為空
	  bool empty(void) noexcept{return d_id==d_lots.size();} 
};

/*
可能丟出的例外：
	FUNCTION       		| EXCEPTION
	bowl_t::add    		| std::bad_alloc（push_back失敗時）
	bowl_t::add_range 	| std::bad_alloc（d_lots分配記憶體失敗時）
	bowl_t::remove 		| ??
	bowl_t::unadd  		| ??
	bowl_t::list   		| std::ios_base::failure（發生無法預期的輸出錯誤）
	bowl_t::draw   		| std::ios_base::failure（發生無法預料的輸出錯誤）、std::range_error（籤筒為空時）
*/

#endif