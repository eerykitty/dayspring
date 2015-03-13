all: server

server: main.o server.o console.o ige_sync.pb.o utility.o ll_network.o
	g++ -std=c++11 -lczmq -lcurve -lboost_system -lboost_filesystem -o server main.o server.o console.o ll_network.o utility.o -pthread

main.o: main.cc
	g++ -c -std=c++11 main.cc -pthread

server.o: server.cc
	g++ -c -std=c++11 server.cc -pthread

console.o: console.cc
	g++ -c -std=c++11 console.cc -pthread

network.o: network.cc
	g++ -c -std=c++11 network.cc -pthread

utility.o: utility.cc
	g++ -c -std=c++11 utility.cc

ll_network.o: ll_network.cc
	g++ -lczmq -lcurve -lboost_system -c -std=c++11 ll_network.cc

ige_sync.pb.o: ige_sync.pb.cc
	protoc --cpp_out=./ ige_sync.proto
	g++ -c -std=c++11 ige_sync.pb.cc -pthread 

clean:
	rm *.o
