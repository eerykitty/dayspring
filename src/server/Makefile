all: server client

client: client_main.o server.o console.o ige_sync.pb.o utility.o network.o player.o flags.o auth.o enet.o user.o net_server.o net_client.o message.o display.o client.o
	g++ -std=c++11 -lenet -lprotobuf -o client client_main.o server.o console.o network.o utility.o flags.o auth.o ige_sync.pb.o enet.o user.o net_server.o net_client.o message.o display.o client.o -pthread -g -lSDL2 -lSDL2_image -ldl

server: main.o server.o console.o ige_sync.pb.o utility.o network.o player.o flags.o auth.o enet.o user.o net_server.o message.o
	g++ -std=c++11 -lenet -lprotobuf -o server main.o server.o console.o network.o utility.o flags.o auth.o ige_sync.pb.o enet.o user.o net_server.o message.o -pthread -g

client.o: client.cc
	g++ -c -std=c++11 client.cc -g

main.o: main.cc
	g++ -c -std=c++11 main.cc -pthread -g

client_main.o: client_main.cc
	g++ -c -std=c++11 client_main.cc -pthread -g

server.o: server.cc
	g++ -c -std=c++11 server.cc -pthread -g

console.o: console.cc
	g++ -c -std=c++11 console.cc -pthread -g

network.o: network.cc
	g++ -c -std=c++11 -lprotobuf network.cc -pthread -g

utility.o: utility.cc
	g++ -c -std=c++11 utility.cc -g

ige_sync.pb.o: ige_sync.pb.cc
	protoc --cpp_out=./ ige_sync.proto
	g++ -c -std=c++11 -lprotobuf ige_sync.pb.cc -pthread  -g

player.o: player.cc
	g++ -c -std=c++11 player.cc -g

flags.o: flags.cc
	g++ -c -std=c++11 flags.cc -g

auth.o: auth.cc
	g++ -c -std=c++11 auth.cc -g

enet.o: enet.cc
	g++ -c -std=c++11 enet.cc -g

user.o: user.cc
	g++ -c -std=c++11 user.cc -g

net_server.o: net_server.cc
	g++ -c -std=c++11 net_server.cc -g
net_client.o: net_client.cc
	g++ -c -std=c++11 net_client.cc -g
message.o: message.cc
	g++ -c -std=c++11 message.cc

display.o: display.cc
	g++ -c -std=c++11 display.cc -lSDL2

clean:
	rm *.o
