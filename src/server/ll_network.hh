#pragma once

#include <fstream>
#include <string>

#include <stdio.h>
#include <string.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <curve.h>

#include "utility.hh"

#define CERT_PATH (char*)"server_certificate"
#define CLIENT_CERT_DIR (char*)"client_cert"

class ll_net
{
        private:
                zcert_t* certificate;
                zctx_t* context;
                zauth_t* auth;
                curve_server_t* server;
                
                void get_own_certificate ();
                
        public:
                ll_net (std::string address);
                ~ll_net ();
};
