#include "network.hh"

net::client::client (std::string address, enet_uint16 port, std::string username, std::string password)
{
        console::t_notify ("CLIENT", "Starting client");
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
                                        console::t_notify ("CLIENT", "Server auth'ed us and gave us hash " + std::to_string ((uint64_t)server_auth.login_hash ()));
                                        server->hash = server_auth.login_hash ();
                                        server->state = CXN_SPECTATE;

                                        time_sentinel->epoch_tm = (tm*)malloc(sizeof(tm));

                                        time_sentinel->epoch_tm->tm_sec = 0;//server_auth.epoch().sec ();
                                        time_sentinel->epoch_tm->tm_min = 0;//server_auth.epoch().min ();
                                        time_sentinel->epoch_tm->tm_hour = 0;//server_auth.epoch().hour ();
                                        time_sentinel->epoch_tm->tm_mday = 1;//server_auth.epoch().day ();
                                        time_sentinel->epoch_tm->tm_mon = 0;//server_auth.epoch().mon ();
                                        time_sentinel->epoch_tm->tm_year = 100;//server_auth.epoch().year ();
                                        time_sentinel->epoch_tm->tm_wday = 0;
                                        time_sentinel->epoch_tm->tm_yday = 0;
                                        time_sentinel->epoch_tm->tm_isdst = 0;

                                        time_sentinel->epoch_t = mktime (time_sentinel->epoch_tm);


                                        console::t_notify ("CLIENT", "Server EPOCH is " + std::to_string (time_sentinel->epoch_t));
                                        printf ("%s\n", ctime (&time_sentinel->epoch_t));
                                        time_sentinel->epoch = std::chrono::high_resolution_clock::from_time_t (time_sentinel->epoch_t);
                                        gclient = new game_client ();
                                        client_thread = new std::thread (&game_client::main, gclient);
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

                case MID(timestamp):
                        {
                                ige::SNTP time_sync;
                                if (msg->read_buffer (&time_sync))
                                {
                                        int64_t t0 = time_sync.t0().time();
                                        int64_t t1 = time_sync.t1().time();
                                        int64_t t2 = time_sync.t2().time();
                                        auto t3_m = time_sentinel->game_time ();
                                        int64_t t3 = t3_m->time();
                                        printf ("here are yo times t0 %li t1 %li t2 %li t3 %li\n", t0, t1, t2, t3);
                                        delete t3_m;
                                        int64_t delta = ((t1 - t0) + (t2 - t3)) / 2;
                                        console::notify (std::to_string (delta));
                                        time_sentinel->time_deltas.push_back (delta);
                                }
                        }
                        break;

        }
}

bool net::client::send (uint64_t id, google::protobuf::MessageLite* msg, uint32_t mid)
{
        std::lock_guard<std::mutex> lock (message_mutex);
        server->send_message (msg, mid);
}
