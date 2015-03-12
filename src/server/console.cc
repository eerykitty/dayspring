#include "console.hh"

std::mutex console::io_mutex;

void console::notify (std::string input)
{
        std::lock_guard<std::mutex> lock (io_mutex);
        std::cout << input << std::endl;
}
