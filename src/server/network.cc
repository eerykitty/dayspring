#include "network.hh"

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
