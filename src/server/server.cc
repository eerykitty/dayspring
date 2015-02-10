#include "server.hh"

void sentinel::set_tick_interval (unsigned int count)
{
	tick.rep = std::chrono::milliseconds;
}
