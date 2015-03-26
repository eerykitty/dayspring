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
        for (int j = 0; j < 10; j++)
        {
                for (int i = 0; i < 3; i++)
                {
                        host->send (0, time_sentinel->game_time (), MID(timestamp));
                }
                sleep (1);
                time_sentinel->time_delta = std::accumulate (time_sentinel->time_deltas.begin (), time_sentinel->time_deltas.end (), 0) / time_sentinel->time_deltas.size ();
        }

        std::chrono::milliseconds time_delay (time_sentinel->time_delta);
        console::t_notify ("CHAI", "Delta T is " + std::to_string (time_sentinel->time_delta));

        auto tick = std::chrono::milliseconds (1000);
        auto delay = std::chrono::milliseconds (time_sentinel->time_delta);

        for (;;)
        {
                auto time_since_epoch = (se_clock::now () - time_sentinel->epoch) + delay;
                auto ticks = time_since_epoch / tick;
                ticks++;
                auto time_till_next_tick = (tick * ticks);
                auto tp = time_till_next_tick + time_sentinel->epoch;

                //auto tp = se_clock::now () + tick + time_delay;
                console::t_notify ("CHAI", "tick!");
                std::this_thread::sleep_until (tp);
        }

        //chaiscript::ChaiScript chai (chaiscript::Std_Lib::library ());
        //chai.add (chaiscript::fun(&console::t_notify), "t_notify");
        //chai.add (chaiscript::var(net_host), "net");
        //chai.add (chaiscript::var(&time_sentinel->close_server), "shutdown");
        //chai.add (chaiscript::var(window), "window");
        //chai.eval_file ("client.chai");
}
