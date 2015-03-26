#include "server.hh"
#include "network.hh"

#include "main.hh"

sentinel::sentinel (unsigned int count)
{
        time_sentinel = this;
        interval = count;
        close_server = false;
        epoch_t = 0;
        epoch_tm = NULL;
        time_delta = 0;
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
        //start_tm->tm_sec--; // since we're rounding away microseconds but still using a microsecond resolution clock we need to make sure this epoch be in the PAST.
        epoch_tm = (std::tm*)malloc (sizeof (std::tm));
        memcpy (epoch_tm, start_tm, sizeof (std::tm));
        epoch_tm->tm_mday--;
        epoch_tm->tm_sec = 0;//server_auth.epoch().sec ();
        epoch_tm->tm_min = 0;//server_auth.epoch().min ();
        epoch_tm->tm_hour = 0;//server_auth.epoch().hour ();
        epoch_tm->tm_mday = 1;//server_auth.epoch().day ();
        epoch_tm->tm_mon = 0;//server_auth.epoch().mon ();
        epoch_tm->tm_year = 100;//server_auth.epoch().year ();
        epoch_tm->tm_wday = 0;
        epoch_tm->tm_yday = 0;
        epoch_tm->tm_isdst = 0;

        epoch_t = mktime (epoch_tm);
        start = se_clock::from_time_t (epoch_t);
        epoch = se_clock::from_time_t (epoch_t);

        // this might not be necessary?
        //std::this_thread::sleep_until (se_clock::now () + std::chrono::seconds (1));
        // def not necessary.

        console::t_notify ("SENTINEL", "started ticking");

        auto tick = std::chrono::milliseconds (1000);

        for (;;)
        {
                // first we need to calculate the ticks since the epoch
                auto time_since_epoch = se_clock::now () - epoch;
                auto ticks = time_since_epoch / tick;
                ticks++;
                auto time_till_next_tick = tick * ticks;
                tp = time_till_next_tick + epoch;
                //console::notify (std::to_string (time_till_next_tick.count ()));
                //exit (1);
                //uint64_t ticks = (se_clock::now () - epoch).count () / (tick.count () * this->interval);
                //console::t_notify ("TICK", std::to_string (ticks));
                //ticks++;

                //exit (1);

                //tp = epoch + (tick * this->interval) * ticks;

                console::t_notify ("TICK", "TOCK AT " + std::to_string (tp.time_since_epoch ().count ()));
                /*
                 * Tick!
                 */

                // here we should process the network queue.

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

                        console::t_notify ("SENTINEL", "tick/tock off schedule; system overloaded or clock changed. Go fix it."); 
                }
                if (close_server)
                        return;
        }
}

void sentinel::shutdown ()
{
        std::lock_guard<std::mutex> lock (exit_mutex);
        close_server = true;
}

ige::Timestamp* sentinel::game_time ()
{
        ige::Timestamp* dsp_time = new ige::Timestamp;
        auto dsp_dur = se_clock::now () - epoch;
        auto dsp_ms = std::chrono::duration_cast<std::chrono::milliseconds> (dsp_dur);
        dsp_time->set_time (dsp_ms.count () /*+ time_delta*/);
        return dsp_time;
}
