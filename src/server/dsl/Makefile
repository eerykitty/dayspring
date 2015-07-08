all: test

test: main.o stratus.o
	g++ -o out -std=c++11 main.o stratus.o

main.o: main.cc
	g++ -c -std=c++11 main.cc -Wall

stratus.o: stratus.cc
	g++ -c -std=c++11 stratus.cc -Wall
clean:
	rm *.o
