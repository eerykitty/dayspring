#include "ll_network.hh"

using std::uint32_t;

ll_net::ll_net (sentinel* main_sentinel, std::string endpoint, enet_uint16 port)
{
        sent = main_sentinel;
        if (enet_initialize () != 0)
        {
                console::t_error ("LLNET", "An error occured initializing ENet, bailing.");
                exit (1);
        }
        
        if (endpoint.length () == 0)
                address.host = ENET_HOST_ANY;
        else
                enet_address_set_host (&address, endpoint.c_str());
        address.port = port;

        max_clients = 20;
        bandwith_up = 0;
        bandwith_down = 0;
        channel_count = 2;

        server = enet_host_create (&address, max_clients, channel_count, bandwith_down, bandwith_up);
        if (server == (ENetHost*)NULL)
        {
                console::t_error ("LLNET", "Unable to bind server, bailing.");
                exit (1);
        }

        console::t_notify ("LLNET", "ENet server ready.");
}

ll_net::~ll_net ()
{
        console::t_notify ("LLNET", "shutting down net");
        console::t_notify ("LLNET", std::to_string (connections.size ()) + " clients remaining, sending disconnect signals...");
        int clients = connections.size ();
        for (auto it = connections.begin (); it != connections.end (); it++)
        {
                enet_peer_disconnect (it->second->peer, 0);
        }
        int disconnects = 0;
        while (enet_host_service (server, &event, 500) > 0)
        {
                if (event.type == ENET_EVENT_TYPE_RECEIVE)
                        enet_packet_destroy (event.packet);
                else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
                {
                        console::t_notify ("LLNET", std::to_string (LL_CXN->id) + " has disconnected.");
                        disconnects++;
                }
        }
        console::t_notify ("LLNET", std::to_string (clients - disconnects) + " clients left, hopefully that's it.");
        enet_host_destroy (server);
        enet_deinitialize ();
}

void ll_net::main ()
{
        console::t_notify ("LLNET", "the packets be rollin yo");
        for (;;)
        {
                if (enet_host_service (server, &event, 100) > 0)
                {
                        switch (event.type)
                        {
                                case ENET_EVENT_TYPE_CONNECT:
                                        {
                                                new_connection (event);
                                        }
                                        break;

                                case ENET_EVENT_TYPE_RECEIVE:
                                {
                                        printf ("%s\n", event.packet -> data);
                                        if (strcmp ((const char*)(event.packet -> data), "END") == 0)
                                                sent->shutdown ();
                                        enet_packet_destroy (event.packet);
                                }
                                break ;

                                case ENET_EVENT_TYPE_DISCONNECT:
                                {
                                        destroy_connection (event);
                                }
                                break;
                        }
                }
                if (sent->close_server)
                        return;
        }
}

message::message (ENetPacket* packet)
{
        id = *((uint32_t*)packet->data);
        data = (void*)((uint32_t*)(packet->data)+1);
        length = packet->dataLength - sizeof(id);
        reliable = (packet->flags == ENET_PACKET_FLAG_RELIABLE ? true : false);
}

message::message (std::string string)
{
        id = NET_PACKET_STRING;
        const char* data = string.c_str ();
        size_t length = string.length() + 1;
        char* payload = (char*)malloc (sizeof(char) * length);
        strcpy (payload, data);
        this->length = length;
        data = payload;
        reliable = true;
}

message::message ()
{
        // *crickets chirping*
}

void connection::queue_message (message* msg)
{
        std::lock_guard<std::mutex> lock (mq_mutex);
        message_queue.push (msg);
}

void connection::flush_message_queue ()
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
                delete [] msg;
                message_queue.pop ();
        }
}

void ll_net::new_connection (ENetEvent event)
{
        connection* new_connection = new connection;
        new_connection->id = event.peer -> connectID;
        new_connection->peer = event.peer;
        connections[event.peer -> connectID] = new_connection;
        event.peer -> data = new_connection;
        console::t_notify ("LLNET", "Client " + std::to_string (LL_CXN->id) + " connected.");
}

void ll_net::destroy_connection (ENetEvent event)
{
        console::t_notify ("LLNET", "Client " + std::to_string (LL_CXN->id) + " disconnected.");
        connections.erase (event.peer -> connectID);
}

void ll_net::process_packet (ENetEvent event)
{
        printf ("%s\n", event.packet -> data);
        if (strcmp ((const char*)(event.packet -> data), "END") == 0)
                sent->shutdown ();
        enet_packet_destroy (event.packet);
}
