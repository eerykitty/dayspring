#include <chrono>
#include <thread>
#include <iostream>

#include "server.hh"
#include "ll_network.hh"
#include "console.hh"

int main (int argc, char** argv)
{
        {
                sentinel sent (1);
                ll_net::enet net (&sent, "127.0.0.1", 1234);
                console::t_notify ("MAIN", "Starting sentinel.");
                std::thread sentinel_thread (&sentinel::main, &sent);

                console::t_notify ("MAIN", "Starting networking subsystem.");
                std::thread enet_thread (&ll_net::enet::main, &net);

                sentinel_thread.join ();
                console::t_notify ("MAIN", "Sentinel thread safely joined.");
                enet_thread.join ();
                console::t_notify ("MAIN", "Networking thread safelty joined.");
        }

        console::notify ("have a nice day :3");
        return 0;
}
