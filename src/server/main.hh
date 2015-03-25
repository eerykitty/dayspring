#pragma once

namespace net {class peer;}
class sentinel;
class game_client;
namespace std {class thread;}

extern net::peer* net_host;
extern sentinel* time_sentinel;
extern game_client* gclient;
extern std::thread* client_thread;
