#include "network.hh"

net::server::server (std::string address, enet_uint16 port)
{
        console::t_notify ("SERVER", "Starting server");
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
                                        clients[hash] = users[cxn];
                                        ige::LoginSuccess client_auth;
                                        client_auth.set_login_hash (hash);
                                        
                                        ige::SendEpoch* server_epoch = new ige::SendEpoch;
                                        console::t_notify ("SERVER", "Sending EPOCH " + std::to_string (time_sentinel->epoch_t));
                                        server_epoch->set_sec (time_sentinel->epoch_tm->tm_sec);
                                        server_epoch->set_min (time_sentinel->epoch_tm->tm_min);
                                        server_epoch->set_hour (time_sentinel->epoch_tm->tm_hour);
                                        server_epoch->set_day (time_sentinel->epoch_tm->tm_mday);
                                        server_epoch->set_mon (time_sentinel->epoch_tm->tm_mon);
                                        server_epoch->set_year (time_sentinel->epoch_tm->tm_year);

                                        client_auth.set_allocated_epoch (server_epoch);
        
                                        users[cxn]->send_message (&client_auth, MID(auth));
                                        if (clients.size () == 1)
                                        {
                                                users[cxn]->flags.set ("WIZARD", true);
                                                users[cxn]->sync_flag ("WIZARD");
                                        }
                                }
                        }
                break;

                case MID(timestamp):
                        {
                                ige::Timestamp time_sync;
                                if (msg->read_buffer (&time_sync))
                                {
                                        ige::Timestamp* t0 = new ige::Timestamp;
                                        t0->set_time (time_sync.time());
                                        auto t1 = time_sentinel->game_time ();
                                        auto t2 = time_sentinel->game_time ();
                                        ige::SNTP sntp_packet;
                                        sntp_packet.set_allocated_t0 (t0);
                                        sntp_packet.set_allocated_t1 (t1);
                                        sntp_packet.set_allocated_t2 (t2);
                                        users[cxn]->send_message (&sntp_packet, MID(timestamp));
                                }
                        }
                break;
        }
}

bool net::server::send (uint64_t id, google::protobuf::MessageLite* msg, uint32_t mid)
{
        std::lock_guard<std::mutex> lock (message_mutex);
        if (clients.find (id) != clients.end ())
        {
                clients[id]->send_message (msg, mid);
        }
}
