#include <chrono>
#include <thread>
#include <iostream>

#include "server.hh"
#include "ll_network.hh"
#include "console.hh"

int main (int argc, char** argv)
{
        sentinel sent (1);
        ll_net net ("127.0.0.1", 1234);
        //std::thread sentinel_thread (&sentinel::main, &sent);

        console::t_notify ("MAIN", "Starting networking subsystem.");
        std::thread net_thread (&ll_net::main, &net);

        //sentinel_thread.join ();
        net_thread.join ();

	/*using std::chrono::high_resolution_clock;
        auto tick = std::chrono::seconds (1);
	auto in_le_future = high_resolution_clock::now() + (tick * 4);
	std::this_thread::sleep_until (in_le_future);*/
	return 0;
}
