#include "server.hh"

sentinel::sentinel (unsigned int count)
{
        interval = count;
}

void sentinel::set_tick_interval (unsigned int count)
{
        interval = count;
}

std::string sentinel::format_time (std::chrono::high_resolution_clock::time_point tp)
{
        std::time_t from_epoch = std::chrono::high_resolution_clock::to_time_t (tp);
        std::tm* t = std::localtime (&from_epoch);
        char buff[80];
        std::strftime ( buff,
                        sizeof(buff),
                        "%T",
                        t);
        return std::string (buff);
}

void sentinel::main ()
{
        start = se_clock::now ();
        start_t = se_clock::to_time_t (start);
        start_tm = std::gmtime (&start_t);
        start_tm->tm_sec--; // since we're rounding away microseconds but still using a microsecond resolution clock we need to make sure this epoch be in the PAST.
        start = se_clock::from_time_t (start_t);
       
        // this might not be necessary?
        //std::this_thread::sleep_until (se_clock::now () + std::chrono::seconds (1));

        console::t_notify ("SENTINEL", "started ticking");

        auto tick = std::chrono::seconds (1);

        for (;;)
        {
                tp = se_clock::now () + (tick * this->interval);
                /*
                 * Tick!
                 */

                // here we should process the network queue.

                /*
                 * Tock!
                 */
                console::notify ("tock?");
                if (se_clock::now () < tp)
                {
                        console::notify ("halp?");
                        std::this_thread::sleep_until (tp);
                }
                else
                {
                        // We're overdue, for now all we'll do is notify via console, but
                        // TODO: add calibration routines
                        
                       console::t_notify ("SENTINEL", "tick/tock off schedule; system overloaded or clock changed. Go fix it."); 
                }
        }
}
