CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
LINK = g++
LFLAGS = -lmpfr -lgmp
MAIN_X = main

.PHONY: all

all: .main
	$(LINK) $(LFLAGS) obj/* -o $(MAIN_X)

.main: .number src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o obj/main.o

.number: .interval.o
	

.interval.o: src/number/interval.hpp
	

clean:
	rm -r obj/*
