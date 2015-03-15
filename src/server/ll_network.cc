#include "ll_network.hh"

using std::uint32_t;

namespace ll_net
{

        enet::enet (sentinel* main_sentinel, std::string endpoint, enet_uint16 port)
        {
                sent = main_sentinel;
                if (enet_initialize () != 0)
                {
                        console::t_error ("LLNET", "An error occured initializing ENet, bailing.");
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
                        console::t_error ("LLNET", "Unable to bind server, bailing.");
                        exit (1);
                }

                console::t_notify ("LLNET", "ENet server ready.");
        }

        enet::~enet ()
        {
                console::t_notify ("LLNET", "shutting down net");
                console::t_notify ("LLNET", std::to_string (connections.size ()) + " clients remaining, sending disconnect signals...");
                int clients = connections.size ();
                for (auto it = connections.begin (); it != connections.end (); it++)
                {
                        enet_peer_disconnect (it->second->peer, 0);
                        delete it->second;
                }
                int disconnects = 0;
                while (enet_host_service (server, &event, 500) > 0)
                {
                        if (event.type == ENET_EVENT_TYPE_RECEIVE)
                                enet_packet_destroy (event.packet);
                        else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
                        {
                                // console::t_notify ("LLNET", std::to_string (LL_CXN->id) + " has disconnected.");
                                disconnects++;
                        }
                }
                console::t_notify ("LLNET", std::to_string (clients - disconnects) + " clients left, hopefully that's it.");
                enet_host_destroy (server);
                enet_deinitialize ();
                google::protobuf::ShutdownProtobufLibrary();
        }

        void enet::main ()
        {
                console::t_notify ("LLNET", "the packets be rollin yo");
                for (;;)
                {
                        if (enet_host_service (server, &event, 10) > 0)
                        {
                                switch (event.type)
                                {
                                        case ENET_EVENT_TYPE_CONNECT:
                                                {
                                                        new_connection (event);
                                                }
                                                break;

                                        case ENET_EVENT_TYPE_RECEIVE:
                                                {
                                                        process_packet (event);
                                                        //printf ("%s\n", event.packet -> data);
                                                        //if (strcmp ((const char*)(event.packet -> data), "END") == 0)
                                                        //        sent->shutdown ();
                                                        //enet_packet_destroy (event.packet);
                                                        //LL_CXN->queue_message (new message ("yo lua!!!"));
                                                        //LL_CXN->flush_message_queue ();
                                                }
                                                break ;

                                        case ENET_EVENT_TYPE_DISCONNECT:
                                                {
                                                        destroy_connection (event);
                                                }
                                                break;
                                }
                        }

                        for (auto it = connections.begin (); it != connections.end (); it++)
                        {
                                it->second->flush_message_queue ();
                        }

                        if (sent->close_server)
                                return;
                }
        }

        void connection::queue_message (message* msg)
        {
                std::lock_guard<std::mutex> lock (mq_mutex);
                message_queue.push (msg);
        }

        void connection::flush_message_queue ()
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
                        free (msg->data);
                        delete msg;
                        message_queue.pop ();
                }
        }

        void enet::new_connection (ENetEvent event)
        {
                connection* new_connection = new connection;
                new_connection->id = event.peer -> connectID;
                new_connection->peer = event.peer;
                connections[event.peer -> connectID] = new_connection;
                event.peer -> data = new_connection;
                console::t_notify ("LLNET", "Client " + std::to_string (LL_CXN->id) + " connected.");
                LL_CXN->net_cxn = new net::connection (event.peer);
                net::connections.push_front (LL_CXN->net_cxn);
                if (connections.size () == 1)
                {
                        LL_CXN->net_cxn->flags.set ("WIZARD");
                }
        }

        void enet::destroy_connection (ENetEvent event)
        {
                console::t_notify ("LLNET", "Client " + std::to_string (LL_CXN->id) + " disconnected.");
                delete connections[LL_CXN->id];
                connections.erase (LL_CXN->id);
        }

        void enet::process_packet (ENetEvent event)
        {
                message msg (event.packet);
                switch (msg.id)
                {
                        case MID(auth):
                                {
                                        ige::Login credentials;
                                        if(credentials.ParseFromArray (msg.data, msg.length))
                                        {
                                                LL_CXN->net_cxn->authenticate (credentials);
                                        }
                                }
                       break;

                        case MID(op):
                                {
                                        if (LL_CXN->net_cxn->flags.is_set ("WIZARD"))
                                        {
                                                ige::Command opc;
                                                if(opc.ParseFromArray (msg.data, msg.length))
                                                {
                                                        switch (opc.command_id ())
                                                        {
                                                                case ige::Command::SHUTDOWN:
                                                                        sent->shutdown ();
                                                                break;
                                                        }
                                                }
                                        }
                                }
                        break;
                }
                enet_packet_destroy (event.packet);
        }
}
