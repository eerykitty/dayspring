#include <numeric>

#include "server.hh"
#include "network.hh"
#include "console.hh"
#include "display.hh"
#include "mid.hh"
#include "main.hh"

//#include "chaiscript/chaiscript.hpp"
//#include "chaiscript/chaiscript_stdlib.hpp"

extern display* window;

void game_client::main ()
{
        net::client* host = static_cast<net::client*> (net_host);
        for (int j = 0; j < 3; j++)
        {
                for (int i = 0; i < 10; i++)
                {
                        host->send (0, time_sentinel->game_time (), MID(timestamp));
                }
                sleep (1);
                time_sentinel->time_delta = std::accumulate (time_sentinel->time_deltas.begin (), time_sentinel->time_deltas.end (), 0) / time_sentinel->time_deltas.size ();
        }

        for (auto delay : time_sentinel->time_deltas)
        {
                console::notify (std::to_string (delay));
        }

        console::notify ("Time delay is " + std::to_string (time_sentinel->time_delta) + " amassed from " + std::to_string (time_sentinel->time_deltas.size ()) + " samples.");

        //chaiscript::ChaiScript chai (chaiscript::Std_Lib::library ());
        //chai.add (chaiscript::fun(&console::t_notify), "t_notify");
        //chai.add (chaiscript::var(net_host), "net");
        //chai.add (chaiscript::var(&time_sentinel->close_server), "shutdown");
        //chai.add (chaiscript::var(window), "window");
        //chai.eval_file ("client.chai");
}
