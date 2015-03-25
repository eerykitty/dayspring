#include "server.hh"
#include "network.hh"
#include "console.hh"
#include "display.hh"
#include "main.hh"

#include "chaiscript/chaiscript.hpp"
#include "chaiscript/chaiscript_stdlib.hpp"

extern display* window;

void game_client::main ()
{
        chaiscript::ChaiScript chai (chaiscript::Std_Lib::library ());
        chai.add (chaiscript::fun(&console::t_notify), "t_notify");
        //chai.add (chaiscript::var(net_host), "net");
        chai.add (chaiscript::var(&time_sentinel->close_server), "shutdown");
        //chai.add (chaiscript::var(window), "window");
        chai.eval_file ("client.chai");
}
