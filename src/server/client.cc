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
        for (int j = 0; j < 2; j++)
        {
                for (int i = 0; i < 500; i++)
                {
                        host->send (0, time_sentinel->game_time (), MID(timestamp));
                }
                sleep (2);
                double sum = std::accumulate(time_sentinel->time_deltas.begin(), time_sentinel->time_deltas.end(), 0.0);
                time_sentinel->time_delta = sum / time_sentinel->time_deltas.size();
        }

        console::t_notify ("CHAI", "Delta T is " + std::to_string (time_sentinel->time_delta));

        auto tick = std::chrono::milliseconds (10);
        auto delay = std::chrono::milliseconds (time_sentinel->time_delta);

        for (;;)
        {
                auto time_since_epoch = ((se_clock::now ()- time_sentinel->epoch) + delay);
                auto ticks = time_since_epoch / tick;
                ticks++;
                auto time_till_next_tick = (tick * ticks);
                auto tp = (time_till_next_tick + time_sentinel->epoch) - delay;
                auto next_server_tick = std::chrono::duration_cast<std::chrono::milliseconds>(tp - time_sentinel->epoch + delay);
                console::t_notify ("CHAI", "waiting until " + std::to_string ((next_server_tick - delay).count ()));
                console::t_notify ("CHAI", "Tick! " + std::to_string (next_server_tick.count ()));
                std::this_thread::sleep_until ((next_server_tick - delay) + time_sentinel->epoch);
        }

        //chaiscript::ChaiScript chai (chaiscript::Std_Lib::library ());
        //chai.add (chaiscript::fun(&console::t_notify), "t_notify");
        //chai.add (chaiscript::var(net_host), "net");
        //chai.add (chaiscript::var(&time_sentinel->close_server), "shutdown");
        //chai.add (chaiscript::var(window), "window");
        //chai.eval_file ("client.chai");
}
