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
                ENetAddress address;
                enet_address_set_host (&address, endpoint.c_str());
                address.port = port;
                return bind_host (&address);
        }
}

net::server::server ()
{

}

net::client::client ()
{
}
