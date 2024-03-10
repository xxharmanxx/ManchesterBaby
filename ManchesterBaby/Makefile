
GPP = g++

#The flags we'll be passing to the compiler
CFLAGS= -g -Wall -Wextra -pedantic -std=c++11

#The default build goal, running 'make' will run 'make all'
.DEFAULT_GOAL: all

all: $(wildcard *.cpp)
	$(GPP) $(CFLAGS) $(wildcard *.cpp)  

BabyTest: Baby.cpp baby_test.cpp
	$(GPP) $(CFLAGS) Baby.cpp baby_test.cpp

AssemblerTest: Assembler.cpp A


