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

net::user::user (connection* cxn)
{
        console::t_notify ("NET", "New user " + std::to_string (cxn->id));
        this->cxn = cxn;
}

void net::user::send_message (google::protobuf::MessageLite* msg, uint32_t id)
{
        cxn->queue_message (new message (msg, id));
}

void net::user::flush_messages ()
{
        cxn->flush_message_queue ();
}

net::server::server (sentinel* s, std::string address, enet_uint16 port)
{
        console::t_notify ("SERVER", "Starting server");
        sent = s;
        ll_net.create_host (address, port);
}

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

net::connection::connection (ENetPeer* peer)
{
        this->peer = peer;
        this->id = peer->connectID;
}

void net::connection::queue_message (message* msg)
{
        std::lock_guard<std::mutex> lock (mq_mutex);
        message_queue.push (msg);
}

void net::connection::flush_message_queue ()
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
                //free (msg->data);
                //delete msg;
                message_queue.pop ();
        }
}

net::message::message (ENetPacket* packet)
{
        id = *((uint32_t*)packet->data);
        data = (void*)((uint32_t*)(packet->data)+1);
        length = packet->dataLength - sizeof(id);
        reliable = (packet->flags == ENET_PACKET_FLAG_RELIABLE ? true : false);
}

net::message::message (std::string string)
{
        id = MID(msg);
        const char* data = string.c_str ();
        size_t len = string.length() + 1;
        char* payload = (char*)malloc (sizeof(char) * len);
        strcpy (payload, data);
        this->length = len;
        this->data = payload;
        reliable = true;
}

// empty payload, for sending auth request by server to client
net::message::message (uint32_t com)
{
        id = com;
        length = 0;
        data = malloc (1);
        reliable = true;
}

net::message::message (google::protobuf::MessageLite* message, uint32_t id)
{
        this->id = id;
        length = message->ByteSize ();
        data = malloc (length);
        message->SerializeToArray (data, length);
        reliable = true;
}

net::message::message ()
{
        // *crickets chirping*
}

net::message::~message ()
{
        //free (data);
}

void net::message::read_buffer (google::protobuf::MessageLite* message)
{
        message->ParseFromArray (data, length);
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

void net::peer::process ()
{
        enet_event event (&ll_net);
        switch (event.type)
        {
                case ENET_EVENT_TYPE_CONNECT:
                        this->new_connection (event.cxn);
                        break;
                case ENET_EVENT_TYPE_DISCONNECT:
                        this->destroy_connection (event.cxn);
                        break;
                case ENET_EVENT_TYPE_RECEIVE:
                        this->process_message (event.cxn, event.msg);
                        break;
        }

        for (auto ucxn : users)
        {
                ucxn.second->flush_messages ();
        }
}

net::peer::peer ()
{
}

net::peer::~peer ()
{
}

void net::peer::entry ()
{
        console::t_notify ("PEER", "Entering the packet handling loop.");
        for (;;)
        {
                process ();
                if (sent->close_server)
                        return;
        }
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
                                msg->read_buffer (&server_auth);
                                console::t_notify ("CLIENT", "Server auth'ed us and gave us hash " + std::to_string (server_auth.login_hash ()));
                                server->hash = server_auth.login_hash ();
                                server->state = CXN_SPECTATE;
                        }
                        break;
        }
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
                                msg->read_buffer (&server_auth);
                                console::t_notify ("SERVER", "Client requests auth with username " + server_auth.username () + " and password " + server_auth.password ());
                                uint64_t hash = auth::connect (server_auth.username (), server_auth.password ());
                                console::t_notify ("SERVER", "Sending hash " + std::to_string (hash) + " to client");
                                users[cxn]->hash = hash;
                                users[cxn]->state = CXN_SPECTATE;
                                ige::LoginSuccess client_auth;
                                client_auth.set_login_hash (hash);
                                users[cxn]->send_message (&client_auth, MID(auth));
                        }
                        break;
        }
}
