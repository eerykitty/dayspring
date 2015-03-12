all: server

server: main.o server.o console.o
	g++ -std=c++11 -o server main.o server.o console.o -pthread

main.o: main.cc
	g++ -c -std=c++11 main.cc -pthread

server.o: server.cc
	g++ -c -std=c++11 server.cc -pthread

console.o: console.cc
	g++ -c -std=c++11 console.cc -pthread

clean:
	rm *.o
