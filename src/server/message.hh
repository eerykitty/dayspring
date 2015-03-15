#pragma once
#include "enet/enet.h"
#include <string>
#include "string.h"
#include <cstdint>

#include "mid.hh"

namespace ll_net
{
        struct message
        {
                std::uint32_t id;
                void* data;
                size_t length;
                bool reliable;
                message (ENetPacket* packet);
                message (std::string string);
                message (uint32_t com);
                message ();
        };
}
