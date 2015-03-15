#include "message.hh"

ll_net::message::message (ENetPacket* packet)
{
        id = *((uint32_t*)packet->data);
        data = (void*)((uint32_t*)(packet->data)+1);
        length = packet->dataLength - sizeof(id);
        reliable = (packet->flags == ENET_PACKET_FLAG_RELIABLE ? true : false);
}

ll_net::message::message (std::string string)
{
        id = MID(msg);
        const char* data = string.c_str ();
        size_t len = string.length() + 1;
        char* payload = (char*)malloc (sizeof(char) * len);
        strcpy (payload, data);
        this->length = len;
        this->data = payload;
        reliable = true;
}

// empty payload, for sending auth request by server to client
ll_net::message::message (uint32_t com)
{
        id = com;
        length = 0;
        data = malloc (1);
        reliable = true;
}

ll_net::message::message ()
{
        // *crickets chirping*
}
