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

        console::notify ("Sentinel started ticking at " + format_time (start));

        auto tick = std::chrono::seconds (1);

        for (;;)
        {
                tp = se_clock::now () + (tick * this->interval);
                /*
                 * Tick!
                 */

                // here we should 

                /*
                 * Tock!
                 */
                if (se_clock::now () < tp)
                {
                        std::this_thread::sleep_until (tp);
                }
                else
                {
                        // We're overdue, for now all we'll do is notify via console, but
                        // TODO: add calibration routines
                        
                       console::notify ("[" + format_time (se_clock::now ()) + "] tick/tock off schedule; system overloaded or clock changed."); 
                }
        }
}
