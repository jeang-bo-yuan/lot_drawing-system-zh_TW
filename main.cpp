//main.cpp
#include "bowl.hpp"

//prototype
extern void creating_bowl(bowl_t&);
extern void drawing_lots(bowl_t&);

int main() {
  std::cout.exceptions(std::ios_base::failbit|std::ios_base::badbit);
  try{
    bowl_t mybowl;

    creating_bowl(mybowl);

    drawing_lots(mybowl);
    
  }catch(std::exception& ex){
		std::cerr << '\n' << ex.what() << "\nProgaram terminates\n";
    return 1;
	}
}