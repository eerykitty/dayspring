#pragma once
#include <chrono>
#include <ratio>
#include <ctime>
#include <string>
#include <iomanip>
#include <thread>

#include "console.hh"

typedef std::chrono::high_resolution_clock se_clock;

class sentinel
{
	private:
                std::chrono::high_resolution_clock::time_point start, tp;
                std::time_t start_t;
                std::tm* start_tm;
                unsigned int interval;

	public:	
                static std::string format_time (std::chrono::high_resolution_clock::time_point t);
		void set_tick_interval (unsigned int count);
                void main ();
                sentinel (unsigned int count);
};

class dayspring_server
{
	private:
		sentinel sentinel_clock;

	public:
		dayspring_server ();
};
