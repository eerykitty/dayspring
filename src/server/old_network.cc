#include "network.hh"

std::list<net::connection*> net::connections;

net::connection::connection (ENetPeer* peer)
{
        id = 0; // no auth.
        this->peer = peer;
        state = CXN_HANDSHAKE;
        CXN->queue_message (new ll_net::message (MID(auth)));
}

void net::connection::authenticate (ige::Login credentials)
{
        if (id == 0 && state == CXN_HANDSHAKE)
        {
                id = auth::connect (credentials.username (), credentials.password ());
                state = CXN_SPECTATE;
        }
}
