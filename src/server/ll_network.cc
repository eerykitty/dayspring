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

        id = 0;

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
        // let's close out any remaining connections...
        while(connections.size() > 0)
        {
                auto con = connections.begin ();
                int type;
                enet_peer_disconnect (con->second.peer, 0);
                while (!(type = enet_host_service (server, &event, 1000) > 0) && event.type != ENET_EVENT_TYPE_DISCONNECT)
                {
                        if (type == 0)
                                enet_packet_destroy (event.packet);
                }
                console::t_notify ("NET", "Client " + std::to_string (con->second.id) + " disconnected.");
                connections.erase (con->first);
        }
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
                                        new_connection.id = generate_id ();
                                        new_connection.peer = event.peer;
                                        connections[event.peer -> address.host] = new_connection;
                                        console::t_notify ("NET", "Client " + std::to_string (new_connection.id) + " connected.");
                                        break;

                                case ENET_EVENT_TYPE_RECEIVE:
                                        printf ("%s\n", event.packet -> data);
                                        enet_packet_destroy (event.packet);
                                        sent->shutdown ();
                                        break ;

                                case ENET_EVENT_TYPE_DISCONNECT:
                                        console::t_notify ("NET", "Client " + std::to_string (connections[event.peer -> address.host].id) + " disconnected.");
                                        connections.erase (event.peer -> address.host);
                                        break;
                        }
                }
                if (sent->close_server)
                        return;
        }
}

unsigned int ll_net::generate_id ()
{
        id++;
        return id;
}
