#variable definition
CXX = clang++
override CXXFLAGS += -g -Wall -Wextra
ifeq ($(OS), Windows_NT)
	RM = del /F /Q
	EXEC = main.exe
else
	EXEC = 	main
endif
OBJS = main.o creating.o drawing.o bowl.o token.o
HEADERS = bowl.hpp token.hpp

#rules
.PHONY : all clean

all : $(EXEC)

$(EXEC) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

main.o creating.o drawing.o bowl.o : bowl.hpp
creating.o drawing.o token.o: token.hpp
#compiling from *.cpp to *.o by means of implict rule

clean :
	$(RM) $(OBJS) $(EXEC)