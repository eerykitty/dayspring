#pragma once

// the old netcode was pretty good, a good start, but it started to buckle, especially as a division I had made between "low level" and "high level" netcode started to really break down under preasure. In particular, seperating these header files was idiotic, since these two have to cooperate. Instead, I'm essentially reusing the same code from ll_network.cc and network.cc (commit 29550db2f4a3ce6e843cfc1e845d9ec884bc2cb6), but wrapping it up in a much nicer programming model before this one falls apart. The idea is to make things prettier and to allow the client to reuse the netcode, I'm beginning to realize how tedious and pointless it would be to have to implement a "complementary" code for the client's side of the protocol, even if largely different.
// following this train of thought, the code is now unified under the *header* network.hh (the implementation isn't necessarily defined in one file) so that clients/servers/other peers can simply include this header.

#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <queue>
#include <cstdint>
#include <mutex>
#include <list>

#include <stdio.h>
#include <string.h>

#include <enet/enet.h>

#include "ige_sync.pb.h"

#include "utility.hh"
#include "console.hh"

#include "mid.hh"

namespace net {

        enum cxn_state {
                CXN_HANDSHAKE, // authentification stage,
                CXN_SPECTATE,  // authenticated, but not playing
                CXN_IN_GAME};  // playing!

        class connection;

        class enet
        {
                private:
                        ENetHost* host;

                        int max_clients;
                        int channel_count;
                        int bandwith_up;
                        int bandwith_down;

                        std::list<ENetPeer*> peers;

                public:
                        enet ();
                        ~enet ();

                        bool create_host (std::string endpoint, enet_uint16 port = 0);
                        bool bind_host (ENetAddress* address);
                        connection* connect_peer (ENetAddress address);

                        static ENetAddress make_address (std::string endpoint, enet_uint16 port);
        };

        struct message
        {
                std::uint32_t id;
                void* data;
                size_t length;
                bool reliable;
                message (ENetPacket* packet);
                message (std::string string);
                message (uint32_t com);
                message (google::protobuf::MessageLite* message, uint32_t id);
                message ();

                void read_buffer (google::protobuf::MessageLite* message);
        };

        class connection
        {
                private:
                        std::mutex mq_mutex;
                        std::queue<message*> message_queue;
                        ENetPeer* peer;
                public:
                        connection (ENetPeer* peer);

                        void flush_message_queue ();
                        void queue_message (message* msg);
                        void send_message (message* msg);
        };

        class peer
        {
                private:

                public:
                        //peer ();
                        //~peer ();
        
                        enet ll_net;

                        void main ();
        };

        class client : public peer
        {
                private:
                        connection* server;
                        cxn_state state;

                public:
                        client (std::string address, enet_uint16, std::string username, std::string password);
        };

        class server : public peer
        {
                private:

                public:
                        server (std::string address, enet_uint16);
        };
}
