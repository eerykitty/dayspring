#include "network.hh"

net::message::message (ENetPacket* packet)
{
        id = *((uint32_t*)packet->data);
        data = (void*)((uint32_t*)(packet->data)+1);
        length = packet->dataLength - sizeof(id);
        reliable = (packet->flags == ENET_PACKET_FLAG_RELIABLE ? true : false);
}

net::message::message (std::string string)
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
net::message::message (uint32_t com)
{
        id = com;
        length = 0;
        data = malloc (1);
        reliable = true;
}

net::message::message (google::protobuf::MessageLite* message, uint32_t id)
{
        this->id = id;
        length = message->ByteSize ();
        data = malloc (length);
        message->SerializeToArray (data, length);
        reliable = true;
}

net::message::message ()
{
        // *crickets chirping*
}

net::message::~message ()
{
        //free (data);
}

bool net::message::read_buffer (google::protobuf::MessageLite* message)
{
        return message->ParseFromArray (data, length);
}
