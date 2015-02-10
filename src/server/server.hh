/*
 * Contains the primary server class.
 */

#include <chrono>
#include <ratio>

class sentinel
{
	private:
		std::chrono::milliseconds tick;

	public:	
		void set_tick_interval (unsigned int count);
};

class dayspring_server
{
	private:
		sentinel sentinel_clock;

	public:
		dayspring_server ();
};
