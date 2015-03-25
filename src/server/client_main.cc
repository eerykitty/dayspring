#include "server.hh"
#include "network.hh"
#include "console.hh"
#include "display.hh"
#include "client.hh"

#include "main.hh"

net::peer* net_host;
sentinel* time_sentinel;
game_client* gclient;
std::thread* client_thread;
display* window;

int main (int argc, char** argv)
{
        net_host = NULL;
        time_sentinel = NULL;
        gclient = NULL;
        client_thread = NULL;

        if (argc == 5)
        {
                window = new display;
                window->create_window ();
                sentinel sent (1);
                net::client client (argv[1], atoi (argv[2]), argv[3], argv[4]);

                // console::t_notify ("MAIN", "Starting sentinel.");
                //std::thread sentinel_thread (&sentinel::main, &sent);
                console::t_notify ("MAIN", "Starting networking subsystem.");
                std::thread enet_thread (&net::client::entry, &client);

                //chaiscript::ChaiScript chai (chaiscript::Std_Lib::library ());
                // chai.eval_file ("client.chai");

                //sentinel_thread.join ();
                //console::t_notify ("MAIN", "Sentinel thread safely joined.");
                enet_thread.join ();
                console::t_notify ("MAIN", "Networking thread safelty joined.");
                client_thread->join ();
                window->destroy_window ();
                delete window;
        }

        console::notify ("have a nice day :3");
        return 0;
}
