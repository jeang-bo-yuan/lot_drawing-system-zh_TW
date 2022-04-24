//drawing.cpp
#include "bowl.hpp"
#include "token.hpp"
#include <unordered_map>
#include <limits>

//prototype
extern void drawing_lots(bowl_t&);

namespace{
    class task{
        static size_t draw_num;
        public:
            //負責執行使用者輸入的指令的函式
            static void set_draw_num  (const token_t&, bowl_t&);
            static void draw          (const token_t&, bowl_t&);
            static void restart       (const token_t&, bowl_t&);
            static void help          (const token_t&, bowl_t&);
            typedef void (*func_pt)(const token_t&, bowl_t&);
    };
    size_t task::draw_num=1;
}

//drawing
void drawing_lots(bowl_t& bowl){
    std::string input;  //輸入內容
    token_t tokens_of_input; //input的token放這
    std::unordered_map<std::string, task::func_pt>
        task_table{
            std::make_pair("set_draw_num",  task::set_draw_num),
            std::make_pair("draw",          task::draw),
            std::make_pair("restart",       task::restart),
            std::make_pair("help",          task::help)
        };
	
    std::cout <<
    R"(Step 2. 抽籤
    按下Enter即可開始抽籤。
		
    輸入help可獲得更多的指令說明。
		輸入finish以結束本程式。
    )"
    << '\n';
    while(true){
        std::cout << "(command) > ";
        std::getline(std::cin, input);
        if(std::cin.fail()) throw std::ios_base::failure("drawing_lots:: input failed");

        if(get_tokens(input, tokens_of_input)){ //若有輸入內容
            auto task_it = task_table.find(tokens_of_input[0]);
            if(task_it!=task_table.end())
            {
                task_it->second(tokens_of_input, bowl);
            }
            else if(tokens_of_input[0]=="finish")
            {
                if(tokens_of_input.size()==1)
                    break;
                else
                    std::cout << "\t錯誤： finish不能有參數\n";
            }
            else
            {
                std::cout << "\t錯誤： 指令 " << tokens_of_input[0] << "不存在\n";
            }
        }else{
            tokens_of_input.push_back("draw");
            task::draw(tokens_of_input, bowl);
        }
    }
}


//task definition
void task::set_draw_num (const token_t& tokens, bowl_t&){
    switch (tokens.size()){
        case 1:
            std::cout << "目前的「預設抽籤數量」： " << task::draw_num << '\n';
            break;
        case 2:
            try{
                task::draw_num = stosize_t(tokens[1]);
								std::cout << "目前的「預設抽籤數量」： " << task::draw_num << '\n';												
            }catch(std::invalid_argument&){
                std::cout << "\t錯誤： set_draw_num參數需為正整數\n";
            }catch(std::out_of_range&){
                std::cout << "\t錯誤： set_draw_num的參數超出size_t的範圍"
                        << std::numeric_limits<size_t>::min() << '~' << std::numeric_limits<size_t>::max() << '\n';
            }
            break;
        default:
            std::cout << "\t錯誤： set_draw_num至多只能有一個參數\n";
            break;
    }
}

void task::draw (const token_t& tokens, bowl_t& bowl){
    size_t to_draw;
    switch (tokens.size()){
        case 1:
            to_draw = task::draw_num;
            break;
        case 2:
            try{
                to_draw = stosize_t(tokens[1]);
                break;
            }catch(std::invalid_argument&){
                std::cout << "\t錯誤： draw的參數需為正整數\n";
            }catch(std::out_of_range&){
                std::cout << "\t錯誤： draw的參數超出size_t的範圍"
                        << std::numeric_limits<size_t>::min() << '~' << std::numeric_limits<size_t>::max() << '\n';
            }
            return;
        default:
            std::cout << "\t錯誤： draw的參數需為0或1個\n";
            return;
    }
    while(to_draw--){
        try{
            bowl.draw(std::cout);
        }catch(std::range_error&){
            std::cout << "籤筒已空\n輸入finish結束程式\n或輸入restart重頭再抽一次\n";
            return;
        }
    }
}

void task::restart(const token_t& tokens, bowl_t& bowl){
    if(tokens.size()==1){
        bowl.restart();
				std::cout << "籤筒已重置\n";
    }else{
        std::cout << "\t錯誤： restart不能加參數\n";
    }
}

void task::help(const token_t& tokens, bowl_t&){
    if(tokens.size()!=1) std::cout << "help指令不能加參數喔^_^\n";
    std::cout <<
R"(指令說明
    set_draw_num <num>
        將「預設抽籤數量」設為num。
        在第一次使用此指令前其值為1。
    set_draw_num
        顯示目前的「預設抽籤數量」。

    draw [num]
        抽出num枝籤。
        若未指定num值，則使用「預設抽籤數量」（行為如同只按下Enter）。

    restart
        重置籤筒。「預設抽籤數量」不會被重置。
    help
		印出本說明
)";
}