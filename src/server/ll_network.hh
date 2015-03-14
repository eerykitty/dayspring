#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <queue>
#include <cstdint>
#include <mutex>

#include <stdio.h>
#include <string.h>

#include <enet/enet.h>

#include "utility.hh"
#include "console.hh"
#include "network.hh"

#define CERT_PATH (char*)"server_certificate"
#define CLIENT_CERT_DIR (char*)"client_cert"

#define LL_CXN (static_cast<connection*>(event.peer->data)) // low level connection

#define NET_PACKET_STRING 3000
#define NET_PACKET_OP 0

struct message
{
        std::uint32_t id;
        void* data;
        size_t length;
        bool reliable;
        message (ENetPacket* packet);
        message (std::string string);
        message ();
};

class connection
{
        friend class ll_net;

        private:
                std::queue<message*> message_queue;
                void flush_message_queue ();
                std::mutex mq_mutex;

        public:
                std::uint32_t id;
                ENetPeer* peer;
                void queue_message (message* msg);
};

class ll_net
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
                ll_net (sentinel* main_sentinel, std::string endpoint = "", enet_uint16 port = 5125);
                ~ll_net ();

                void main ();
};
