#include "ll_network.hh"

ll_net::ll_net (std::string endpoint, enet_uint16 port)
{
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
                                        console::t_notify ("NET", "Client connected.");
                                        break;

                                case ENET_EVENT_TYPE_RECEIVE:
                                        printf ("%s\n", event.packet -> data);
                                        enet_packet_destroy (event.packet);
                                        break ;

                                case ENET_EVENT_TYPE_DISCONNECT:
                                        console::t_notify ("NET", "Client disconnected.");
                                        break;
                        }
                }
        }
}
