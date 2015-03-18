#include "network.hh"

net::client::client (sentinel* s, std::string address, enet_uint16 port, std::string username, std::string password)
{
        console::t_notify ("CLIENT", "Starting client");
        sent = s;
        ll_net.create_host (""); // don't want to bind any address for a client
        connection* cxn = ll_net.connect_peer (enet::make_address (address, port));
        server = new user (cxn);
        users[cxn] = server;
        server->state = CXN_HANDSHAKE; // we're not authenticated yet, the server will tell us when we are.
        ige::Login credentials;
        credentials.set_username (username);
        credentials.set_password (password);
        server->send_message(&credentials, MID(auth));
}

void net::client::new_connection (connection* cxn)
{
        // this shouldn't ever be reached...
}

void net::client::destroy_connection (connection* cxn)
{
        // whoops, the server kicked us :(
}

void net::client::process_message (connection* cxn, message* msg)
{
        switch (msg->id)
        {
                case MID(auth):
                        {
                                ige::LoginSuccess server_auth;
                                if (msg->read_buffer (&server_auth))
                                {
                                        console::t_notify ("CLIENT", "Server auth'ed us and gave us hash " + std::to_string (server_auth.login_hash ()));
                                        server->hash = server_auth.login_hash ();
                                        server->state = CXN_SPECTATE;
                                }
                        }
                        break;
                case MID(flag):
                        {
                                ige::SetFlag flag_message;
                                if (msg->read_buffer (&flag_message))
                                {
                                        console::t_notify ("CLIENT", "Flag " + flag_message.flag_name () + " toggled");
                                        server->flags.set (flag_message.flag_name (), flag_message.flag_value ());
                                }
                        }
                        break;
        }
}

bool net::client::send (google::protobuf::MessageLite* msg, uint32_t mid)
{
        std::lock_guard<std::mutex> lock (message_mutex);
        server->send_message (msg, mid);
}
