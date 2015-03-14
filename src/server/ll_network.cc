#include "ll_network.hh"

using std::uint32_t;

ll_net::ll_net (sentinel* main_sentinel, std::string endpoint, enet_uint16 port)
{
        sent = main_sentinel;
        if (enet_initialize () != 0)
        {
                console::t_error ("NET", "An error occured initializing ENet, bailing.");
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
                console::t_error ("NET", "Unable to bind server, bailing.");
                exit (1);
        }

        console::t_notify ("NET", "ENet server ready.");
}

ll_net::~ll_net ()
{
        console::t_notify ("NET", "shutting down net");
        console::t_notify ("NET", std::to_string (connections.size ()) + " clients remaining, sending disconnect signals...");
        int clients = connections.size ();
        for (auto it = connections.begin (); it != connections.end (); it++)
        {
                enet_peer_disconnect (it->second.peer, 0);
        }
        int disconnects = 0;
        while (enet_host_service (server, &event, 500) > 0)
        {
                if (event.type == ENET_EVENT_TYPE_RECEIVE)
                        enet_packet_destroy (event.packet);
                else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
                {
                        console::t_notify ("NET", std::to_string (((connection*)(event.peer -> data))->id) + " has disconnected.");
                        disconnects++;
                }
        }
        console::t_notify ("NET", std::to_string (clients - disconnects) + " clients left, hopefully that's it.");
        enet_host_destroy (server);
        enet_deinitialize ();
}

void ll_net::main ()
{
        console::t_notify ("NET", "the packets be rollin yo");
        for (;;)
        {
                if (enet_host_service (server, &event, 100) > 0)
                {
                        switch (event.type)
                        {
                                case ENET_EVENT_TYPE_CONNECT:
                                        connection new_connection;
                                        new_connection.id = event.peer -> connectID;
                                        new_connection.peer = event.peer;
                                        connections[event.peer -> connectID] = new_connection;
                                        event.peer -> data = &connections[event.peer -> connectID];
                                        console::t_notify ("NET", "Client " + std::to_string (new_connection.id) + " connected.");
                                        break;

                                case ENET_EVENT_TYPE_RECEIVE:
                                        console::t_notify ("NET", "received packet from " + std::to_string (event.peer -> connectID));
                                        printf ("%s\n", event.packet -> data);
                                        if (strcmp ((const char*)(event.packet -> data), "END") == 0)
                                                sent->shutdown ();
                                        enet_packet_destroy (event.packet);
                                        break ;

                                case ENET_EVENT_TYPE_DISCONNECT:
                                        console::t_notify ("NET", "Client " + std::to_string (connections[event.peer -> connectID].id) + " disconnected.");
                                        connections.erase (event.peer -> connectID);
                                        break;
                        }
                }
                if (sent->close_server)
                        return;
        }
}
