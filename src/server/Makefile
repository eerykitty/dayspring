all: server

server: main.o server.o console.o ige_sync.pb.o utility.o network.o ll_network.o player.o network.o flags.o auth.o message.o
	g++ -std=c++11 -lenet -lprotobuf -o server main.o server.o console.o network.o ll_network.o utility.o flags.o auth.o message.o ige_sync.pb.o -pthread -g

main.o: main.cc
	g++ -c -std=c++11 main.cc -pthread -g

server.o: server.cc
	g++ -c -std=c++11 server.cc -pthread -g

console.o: console.cc
	g++ -c -std=c++11 console.cc -pthread -g

network.o: network.cc
	g++ -c -std=c++11 -lprotobuf network.cc -pthread -g

utility.o: utility.cc
	g++ -c -std=c++11 utility.cc -g

ll_network.o: ll_network.cc
	g++ -c -std=c++11 -lprotobuf ll_network.cc -g 

ige_sync.pb.o: ige_sync.pb.cc
	protoc --cpp_out=./ ige_sync.proto
	g++ -c -std=c++11 -lprotobuf ige_sync.pb.cc -pthread  -g
player.o: player.cc
	g++ -c -std=c++11 player.cc -g

flags.o: flags.cc
	g++ -c -std=c++11 flags.cc -g

auth.o: auth.cc
	g++ -c -std=c++11 auth.cc -g

message.o: message.cc
	g++ -c -std=c++11 message.cc -g

clean:
	rm *.o
