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
        console::t_notify ("ENET", "ENet initialized succesfully.");
}

net::enet::~enet ()
{
        if (host != NULL)
        {
                enet_host_destroy (host);
        }
        enet_deinitialize ();
        google::protobuf::ShutdownProtobufLibrary(); // this isn't strictly necesarry nor does it make the most sense here but
        console::t_notify ("ENET", "ENet shutdown succesfully.");
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
        console::t_notify ("ENET", "Attemping to bind to host.");
        host = enet_host_create (address, max_clients, channel_count, bandwith_down, bandwith_up);
        if (host == NULL)
        {
                return false;
        }
        return true;
}

bool net::enet::create_host (std::string endpoint, enet_uint16 port)
{
        console::t_notify ("ENET", "Creating host " + endpoint + ", stand by.");
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
        console::t_notify ("ENET", "Trying to connect to peer, stand by.");
        ENetPeer* peer = enet_host_connect (host, &address, channel_count, 0);
        ENetEvent event;
        if (enet_host_service (host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
        {
                connection* cxn = new connection (peer);
                peers[peer->connectID] = cxn;
                peer->data = &cxn;
                return cxn;
        }
        else
        {
                return NULL;
        }
}

net::connection* net::enet_event::get_connection ()
{
        if (event.peer != NULL)
        {
                auto it = ll_net->peers.find (event.peer->connectID);
                if (it == ll_net->peers.end ())
                { // peer isn't already listed
                        connection* cxn = new connection (event.peer);
                        ll_net->peers[event.peer->connectID] = cxn;
                        event.peer->data = &cxn;
                        return cxn;
                }
                else
                {
                        return it->second;
                }
        }
        else
        {
                return NULL;
        }
}

net::enet_event::enet_event (enet* ll_net)
{
        this->ll_net = ll_net;
        msg = NULL;
        if (enet_host_service (ll_net->host, &event, 500) > 0)
        {
                if (event.type == ENET_EVENT_TYPE_RECEIVE)
                        msg = new message (event.packet);
                type = event.type;
        }
        else
        {
                type = ENET_EVENT_TYPE_NONE;
        }

        this->cxn = get_connection ();
}

net::enet_event::~enet_event ()
{
        delete msg;
        enet_packet_destroy (event.packet);
}
