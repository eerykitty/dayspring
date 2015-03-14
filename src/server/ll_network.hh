#pragma once

#include <fstream>
#include <string>
#include <iostream>

#include <stdio.h>
#include <string.h>

#include <enet/enet.h>

#include "utility.hh"
#include "console.hh"

#define CERT_PATH (char*)"server_certificate"
#define CLIENT_CERT_DIR (char*)"client_cert"

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

        public:
                ll_net (std::string endpoint = "", enet_uint16 port = 5125);
                ~ll_net ();

                void main ();
};
