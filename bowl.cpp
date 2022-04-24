//bowl.cpp
//the implementation of bowl class

#include "bowl.hpp"
#include <algorithm> //find
#include <cstdlib>   //rand, srand
#include <ctime>     //time
#include <utility>   //swap
#include <iomanip>   //setw

size_t bowl_t::remove(const std::string& target, size_t num){
	size_t i=0;
	while(num){
		auto it = std::find(d_lots.begin(), d_lots.end(), target);
		if(it!=d_lots.end()){
			d_lots.erase(it);
			--num;
			++i;
		}else{ //找不到target
			break;
		}
	}
	return i;
}

void bowl_t::add_range(long long num1, long long num2){
	if(num1 > num2) std::swap(num1, num2);
	size_t total_add = num2-num1+1;
	size_t i = d_lots.size(); //欲加入的第一隻籤的註標
	d_lots.resize( d_lots.size()+total_add ); //若失敗會丟出std::bad_alloc

	while(total_add--){ //做total_add次的加入
		d_lots[i++] = std::to_string(num1++);
	}
}

size_t bowl_t::unadd(size_t num){
	size_t real_remove = (d_lots.size()>num ? num : d_lots.size());
	d_lots.resize(d_lots.size()-real_remove);
	return real_remove;
} 

std::ostream& bowl_t::list(std::ostream& os){
	if(empty()) return std::cout << "**籤筒是空的**\n\n";

	size_t count=0; //已經輸出的籤數
	os << std::left;
	for(size_t i=d_id; i<d_lots.size(); ){
		os <<  std::setw(5) << d_lots[i++] << ((++count)%15 ? ' ' : '\n');
		//輸出時以15枝籤為一行
	}
	return os << (count%15 ? '\n' : ' ') << std::right; //回傳前將對齊方式改回預設值
}

std::ostream& bowl_t::draw(std::ostream& os){
	if(this->empty()){ //若籤筒為空
		throw std::range_error("bowl is empty");
	}
	
	size_t rest = d_lots.size()-d_id; //籤筒中剩餘的籤數
	//抽籤的方式：
	//	從剩餘的籤中隨機選出一枝移到最前方（註標：d_id）
	srand(time(NULL));
	size_t offset = rand()%rest; //其值介於[0,rest)
	std::swap(d_lots[d_id], d_lots[d_id+offset]); //抽出一隻籤並移到籤筒"最前方"
	++d_id;
	os << '#' << d_id << ' ' << d_lots[d_id-1] << '\n';
	return os;
}