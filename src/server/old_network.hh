#pragma once

#include <list>

//#include "ll_network.hh"
#include "flags.hh"
#include "ige_sync.pb.h"
#include "auth.hh"
#include "enet/enet.h"
#include "message.hh"
#include "ll_network.hh"

#define CXN (static_cast<ll_net::connection*>(peer->data)) // low level connection

// high level net code - if anything at this point can be called "high level"

namespace net {

        enum cxn_state {
                CXN_HANDSHAKE, // authentification stage,
                CXN_SPECTATE,  // authenticated, but not playing
                CXN_IN_GAME};  // playing!

        class connection
        {
                private: 
                public:
                        uint64_t id; // unique user id assigned by auth
                        flag_table flags;
                        ENetPeer* peer;
                        cxn_state state;

                        connection (ENetPeer* peer);

                        void authenticate (ige::Login credentials);
        };

        extern std::list<connection*> connections;
}

namespace ll_net
{

        class connection
        {
                friend class enet;

                private:
                std::queue<message*> message_queue;
                void flush_message_queue ();
                std::mutex mq_mutex;
                net::connection* net_cxn;

                public:
                std::uint32_t id;
                ENetPeer* peer;

                void queue_message (message* msg);
        };

        class enet
        {
                private:
                        ENetAddress address;
                        ENetHost* server;
                        ENetEvent event;

                        int max_clients;
                        int channel_count;
                        int bandwith_up;
                        int bandwith_down;

                        std::map<std::uint32_t, connection*> connections;

                        sentinel* sent;

                        void new_connection (ENetEvent event);
                        void destroy_connection (ENetEvent event);
                        void process_packet (ENetEvent event);

                public:
                        enet (sentinel* main_sentinel, std::string endpoint = "", enet_uint16 port = 5125);
                        ~enet ();

                        void main ();
        };
}
