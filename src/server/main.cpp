#include <chrono>
#include <thread>
#include <iostream>

int main (int argc, char** argv)
{
	using std::chrono::high_resolution_clock;
	auto in_le_future = high_resolution_clock::now() + std::chrono::seconds (2);
	std::this_thread::sleep_until (in_le_future);
	return 0;
}
