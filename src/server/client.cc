#include <numeric>

#include "server.hh"
#include "network.hh"
#include "console.hh"
#include "display.hh"
#include "mid.hh"
#include "main.hh"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "chaiscript/chaiscript.hpp"
#include "chaiscript/chaiscript_stdlib.hpp"

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

        auto tick = std::chrono::milliseconds (500);
        auto delay = std::chrono::milliseconds (time_sentinel->time_delta);

        chaiscript::ChaiScript chai (chaiscript::Std_Lib::library ());
        chai.add (chaiscript::fun(&console::t_notify), "t_notify");

        /*chai.add (chaiscript::fun(&IMG_Load), "IMG_Load");
        chai.add (chaiscript::fun(&SDL_CreateTextureFromSurface), "create_texture_from_surface");
        chai.add (chaiscript::fun(&SDL_FreeSurface), "free_surface");
        chai.add (chaiscript::fun(&SDL_DestroyTexture), "destroy_texture");
        chai.add (chaiscript::fun(&SDL_RenderCopy), "render_copy");
        chai.add (chaiscript::fun(&SDL_CreateWindow), "create_window");
        chai.add (chaiscript::fun(&SDL_CreateRenderer), "create_renderer");
        chai.add (chaiscript::fun(&SDL_SetRenderDrawColor), "set_render_draw_color");
        chai.add (chaiscript::fun(&SDL_RenderPresent), "render_present");*/

        //chai.add (chaiscript::var(net_host), "net");
        //chai.add (chaiscript::var(&time_sentinel->close_server), "shutdown");
        //chai.add (chaiscript::var(window), "window");

        chai.eval_file ("client_chai/init.chai");

        for (;;)
        {
                auto time_since_epoch = ((se_clock::now () - time_sentinel->epoch) + delay);
                auto ticks = time_since_epoch / tick;
                ticks++;
                auto time_till_next_tick = (tick * ticks);
                auto tp = (time_till_next_tick + time_sentinel->epoch) - delay;
                auto next_server_tick = std::chrono::duration_cast<std::chrono::milliseconds>(tp - time_sentinel->epoch + delay);

                chai.eval_file ("client_chai/main.chai");                

                std::this_thread::sleep_until ((next_server_tick - delay) + time_sentinel->epoch);
                if (time_sentinel->close_server)
                        return;
        }
}
