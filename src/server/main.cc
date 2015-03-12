#include <chrono>
#include <thread>
#include <iostream>

#include "server.hh"

int main (int argc, char** argv)
{
        sentinel sent (1);
        std::thread sentinel_thread (&sentinel::main, &sent);

        sentinel_thread.join ();

	/*using std::chrono::high_resolution_clock;
        auto tick = std::chrono::seconds (1);
	auto in_le_future = high_resolution_clock::now() + (tick * 4);
	std::this_thread::sleep_until (in_le_future);*/
	return 0;
}
