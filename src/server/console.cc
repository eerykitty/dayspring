#include "console.hh"

std::mutex console::io_mutex;

void console::notify (std::string input)
{
        std::lock_guard<std::mutex> lock (io_mutex);
        std::cout << input << std::endl;
}

void console::t_notify (std::string thread_name, std::string input)
{
        std::lock_guard<std::mutex> lock (io_mutex);
        std::cout << thread_name << "[" << sentinel::format_time (se_clock::now ()) << "] " << input << std::endl;
}
