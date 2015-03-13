#include <chrono>
#include <thread>
#include <iostream>

#include "server.hh"
#include "ll_network.hh"

int main (int argc, char** argv)
{
        sentinel sent (1);
        std::thread sentinel_thread (&sentinel::main, &sent);

        ll_net net ("tcp://127.0.0.1:5555");

        sentinel_thread.join ();

	/*using std::chrono::high_resolution_clock;
        auto tick = std::chrono::seconds (1);
	auto in_le_future = high_resolution_clock::now() + (tick * 4);
	std::this_thread::sleep_until (in_le_future);*/
	return 0;
}
