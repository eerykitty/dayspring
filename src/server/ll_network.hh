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

#include "mid.hh"

#include "ige_sync.pb.h"

#include "utility.hh"
#include "console.hh"
#include "message.hh"
#include "network.hh"

#define CERT_PATH (char*)"server_certificate"
#define CLIENT_CERT_DIR (char*)"client_cert"

#define LL_CXN (static_cast<connection*>(event.peer->data)) // low level connection

