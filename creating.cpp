//creating.cpp
#include "bowl.hpp"
#include "token.hpp"
#include <unordered_map>
#include <limits>

//prototype
extern void creating_bowl(bowl_t&);

namespace{
	class task{
		static size_t prev_add_num;
		public:
			//負責執行使用者輸入的指令的函式
			static void add(const token_t&, bowl_t&);
			static void add_range(const token_t&, bowl_t&);
			static void remove(const token_t&, bowl_t&);
			static void unadd(const token_t&, bowl_t&);
			static void list(const token_t&, bowl_t&);
			static void help(const token_t&, bowl_t&);

			typedef void (*func_pt)(const token_t&, bowl_t&);
	};
	size_t task::prev_add_num;
}


//建立籤筒
void creating_bowl(bowl_t& bowl_to_create){
	token_t token_of_input; //input取token後存放在這
	std::string input;      //使用者輸入內容
	const std::unordered_map<std::string, task::func_pt>
		tasks_table{
			std::make_pair("add",       task::add),
			std::make_pair("add_range", task::add_range),
			std::make_pair("remove",    task::remove),
			std::make_pair("unadd",     task::unadd),
			std::make_pair("list",      task::list),
			std::make_pair("help",      task::help)
		};
	
	std::cout <<
	R"(Step 1. 建立籤筒
	輸入add後面接上每隻籤的內容，即可在籤筒中加入籤。
		
	輸入help可獲得更多指令的說明。
	輸入finish完成籤筒的建立。
	)"
	<< '\n';
	while(true){
		std::cout << "(command)> ";
		std::getline(std::cin, input);
		if(std::cin.fail())	throw std::ios_base::failure("creating_bowl:: input failed");
		
		if(get_tokens(input, token_of_input)){ //若有輸入
			auto task_it = tasks_table.find( token_of_input[0] );
			if(task_it!=tasks_table.end()) //若找到相對的指令
			{	
				task_it->second(token_of_input, bowl_to_create);
			}
			else if (token_of_input[0]=="finish")
			{
				if(token_of_input.size()==1)
					break;
				else
					std::cout << "\t錯誤： finish後不可加參數\n";
			}
			else //沒有相應的指令
			{	
				std::cout << "\t錯誤： 指令 " << token_of_input[0] << " 不存在\n";
			}
		}
	}
}

//task definition
void task::add(const token_t& tokens, bowl_t& bowl){
	task::prev_add_num = 0;
	try{
		for(auto it=++tokens.begin(); it!=tokens.end();++it){
			bowl.add(*it);
			++task::prev_add_num;
			std::cout << "成功新增 " << *it << '\n';
		}
	}catch(std::bad_alloc&){
		std::cout << "**籤筒已滿，無法再加入更多籤**\n\n";
	}
}

void task::add_range(const token_t& tokens, bowl_t& bowl){
	if(tokens.size()==3){
		task::prev_add_num = 0;
		try{
			long long num1 = stolonglong(tokens[1]);
			long long num2 = stolonglong(tokens[2]);
			bowl.add_range(num1, num2);
			std::cout << "成功新增" << num1 << "到" << num2 << "間的整數\n";
			task::prev_add_num = num2-num1+1;
		}catch(std::invalid_argument&){
			std::cout << "\t錯誤： add_range的兩個參數必須為整數\n";
		}catch(std::out_of_range&){
			std::cout << "\t錯誤： 兩個參數所成範圍超出long long的範圍 " 
					  << std::numeric_limits<long long>::min() << '~' << std::numeric_limits<long long>::max() << '\n';
		}catch(std::bad_alloc&){
			std::cout << "**籤筒容量已瀕臨極限，無法加入那麼多的籤**\n\n";
		}
	}else{
		std::cout << "\t錯誤： add_range的參數必須為2個\n";
	}
}

void task::remove(const token_t& tokens, bowl_t& bowl){
	switch(tokens.size()){
		case 2:
			std::cout << "成功移除" << bowl.remove(tokens[1]) << "個" << tokens[1] << '\n';
			task::prev_add_num = 0;
			break;
		case 3:
			try{
				size_t num = stosize_t(tokens[2]);
				std::cout << "成功移除" << bowl.remove(tokens[1], num) << "個" << tokens[1] << '\n';
				task::prev_add_num = 0;
			}catch(std::invalid_argument&){
				std::cout << "\t錯誤： remove的第二個參數必須為正整數\n";
			}catch(std::out_of_range&){
				std::cout << "\t錯誤： remove的第二個參數超出size_t的範圍"
						  << std::numeric_limits<size_t>::min() << '~' << std::numeric_limits<size_t>::max() << '\n';
			}
			break;
		default:
			std::cout << "\t錯誤： remove的參數需為1或2個\n";
	}
}

void task::unadd(const token_t& tokens, bowl_t& bowl){
	switch (tokens.size()){
		case 1:
			std::cout << "成功移除" << bowl.unadd(task::prev_add_num) << "枝籤\n";
			break;
		case 2:
			try{
				size_t num = stosize_t(tokens[1]);
				std::cout << "成功移除" << bowl.unadd(num) << "枝籤\n";
				task::prev_add_num = 0;
			}catch(std::invalid_argument&){
				std::cout << "\t錯誤： unadd的參數必須為正整數\n";
			}catch(std::out_of_range&){
				std::cout << "\t錯誤： unadd的參數超出size_t的範圍"
						  << std::numeric_limits<size_t>::min() << '~' << std::numeric_limits<size_t>::max() << '\n';
			}
			break;
		default:
			std::cout << "\t錯誤： unadd的參數過多\n";
			break;
	}
}

void task::list(const token_t& tokens, bowl_t& bowl){
	if(tokens.size()==1){
		bowl.list(std::cout);
	}else{
		std::cout << "\t錯誤： list指令後不行加參數\n";
	}
}

void task::help(const token_t& tokens, bowl_t&){
	if(tokens.size()!=1) std::cout << "help指令不行加參數喔^_^\n";
	std::cout <<
R"(指令說明
	add <itme1> <item2> ... <itemN>
		將item1, item2...itemN加入到籤筒中 ※ 每枝籤的內容必須以空格區分
	add_range <num1> <num2>
		將num1和num2間的所有整數加入籤筒中 ※ num1、num2必須為整數

	remove <target> [num=1]
		從籤筒中移除num個target ※ 若未指定num的值則預設為1
	unadd [num]
		從籤筒中刪除最新加入的num枝籤。
		當未指定num值時，則刪除上一個「修改籤筒的指令」加入的所有籤。
		「修改籤筒的指令」包含 add、add_range、remove、unadd。

	list
		列出籤筒的內容 ※ 依加入順序排序
	finish
		結束「建立籤筒」
	help
		印出本說明
)";		
}