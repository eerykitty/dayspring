all: test

test: main.o
	g++ -o out -std=c++11 main.o

main.o: main.cc
	g++ -c -std=c++11 main.cc
