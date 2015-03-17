#include "network.hh"

net::server::server (sentinel* s, std::string address, enet_uint16 port)
{
        console::t_notify ("SERVER", "Starting server");
        sent = s;
        ll_net.create_host (address, port);
}

void net::server::new_connection (connection* cxn)
{
        users[cxn] =  new user (cxn);
        users[cxn]->state = CXN_HANDSHAKE;
}

void net::server::destroy_connection (connection* cxn)
{
        //TODO
}

void net::server::process_message (connection* cxn, message* msg)
{
        switch (msg->id)
        {
                case MID(auth):
                        {
                                ige::Login server_auth;
                                if (msg->read_buffer (&server_auth))
                                {
                                        console::t_notify ("SERVER", "Client requests auth with username " + server_auth.username () + " and password " + server_auth.password ());
                                        uint64_t hash = auth::connect (server_auth.username (), server_auth.password ());
                                        console::t_notify ("SERVER", "Sending hash " + std::to_string (hash) + " to client");
                                        users[cxn]->hash = hash;
                                        users[cxn]->state = CXN_SPECTATE;
                                        if (clients.size () == 0)
                                        {
                                                users[cxn]->flags.set ("WIZARD", true);
                                                users[cxn]->sync_flag ("WIZARD");
                                        }
                                        clients[hash] = users[cxn];
                                        ige::LoginSuccess client_auth;
                                        client_auth.set_login_hash (hash);
                                        users[cxn]->send_message (&client_auth, MID(auth));
                                }
                        }
                        break;
        }
}
