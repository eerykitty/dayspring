#include "ll_network.hh"

ll_net::ll_net (std::string endpoint = "", enet_uint16 port = 5125)
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
        channel_count = 4;

        server = enet_host_create (&address, max_clients, channel_count, bandwith_down, bandwith_up);
        if (server == (ENetHost*)NULL)
        {
                console::t_error ("NET", "Unable to bind server, bailing.");
                exit (1);
        }
}

ll_net::~ll_net ()
{
        enet_host_destroy (server);
        enet_deinitialize ();
}
