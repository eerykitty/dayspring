#include "server.hh"
#include "network.hh"
#include "console.hh"

#include "main.hh"

net::peer* net_host;
sentinel* time_sentinel;

int main (int argc, char** argv)
{
        {
                sentinel sent (1);
                net::server server ("127.0.0.1", 1234);
                console::t_notify ("MAIN", "Starting sentinel.");
                std::thread sentinel_thread (&sentinel::main, &sent);

                console::t_notify ("MAIN", "Starting networking subsystem.");
                std::thread enet_thread (&net::server::entry, &server);

                sentinel_thread.join ();
                console::t_notify ("MAIN", "Sentinel thread safely joined.");
                enet_thread.join ();
                console::t_notify ("MAIN", "Networking thread safely joined.");
        }

        console::notify ("have a nice day :3");
        return 0;
}
