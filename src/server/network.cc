#include "network.hh"

net::enet::enet ()
{
        if (enet_initialize () != 0)
        {
                console::t_error ("LLNET", "An error occured initializing ENet, bailing.");
                exit (1);
        }
        host = NULL;

        max_clients = 20;
        bandwith_up = 0;
        bandwith_down = 0;
        channel_count = 2;
}

net::enet::~enet ()
{
        if (host != NULL)
        {
                enet_host_destroy (host);
        }
        enet_deinitialize ();
        google::protobuf::ShutdownProtobufLibrary(); // this isn't strictly necesarry nor does it make the most sense here but
}

ENetAddress net::enet::make_address (std::string endpoint, enet_uint16 port)
{
        ENetAddress address;
        enet_address_set_host (&address, endpoint.c_str());
        address.port = port;
        return address;
}

bool net::enet::bind_host (ENetAddress* address)
{
        host = enet_host_create (address, max_clients, channel_count, bandwith_down, bandwith_up);
        if (host == NULL)
        {
                return false;
        }
        return true;
}

bool net::enet::create_host (std::string endpoint, enet_uint16 port)
{
        if (endpoint.length () == 0)
        {
                return bind_host (NULL);
        }
        else
        {
                ENetAddress address = make_address (endpoint, port);
                return bind_host (&address);
        }
}

net::connection* net::enet::connect_peer (ENetAddress address)
{
        ENetPeer* peer = enet_host_connect (host, &address, channel_count, 0);
        ENetEvent event;
        if (enet_host_service (host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
        {
                peers.push_front (peer);
                return new connection (peer);
        }
        else
        {
                return NULL;
        }
}

net::server::server (std::string address, enet_uint16 port)
{
        ll_net.create_host (address, port);
}

net::client::client (std::string address, enet_uint16 port, std::string username, std::string password)
{
        ll_net.create_host (""); // don't want to bind any address for a client
        server = ll_net.connect_peer (enet::make_address (address, port));
        state = CXN_HANDSHAKE; // we're not authenticated yet, the server will tell us when we are.
        ige::Login credentials;
        credentials.set_username (username);
        credentials.set_password (password);
        server->queue_message (new message (&credentials, MID(auth)));
}

net::connection::connection (ENetPeer* peer)
{
        this->peer = peer;
}

void net::connection::queue_message (message* msg)
{
        std::lock_guard<std::mutex> lock (mq_mutex);
        message_queue.push (msg);
}

void net::connection::flush_message_queue ()
{
        std::lock_guard<std::mutex> lock (mq_mutex);

        while (!message_queue.empty ())
        {
                auto msg = message_queue.front ();
                void* data = malloc(msg->length + sizeof(msg->id));
                memcpy (data, &msg->id, sizeof(msg->id));
                memcpy ((uint32_t*)data + 1, msg->data, msg->length);
                ENetPacket* packet = enet_packet_create (data, msg->length + sizeof(msg->id), (msg->reliable == true ? ENET_PACKET_FLAG_RELIABLE : 0));
                enet_peer_send (peer, 0, packet);
                free (msg->data);
                delete msg;
                message_queue.pop ();
        }
}

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

void net::message::read_buffer (google::protobuf::MessageLite* message)
{
        message->ParseFromArray (data, length);
}
