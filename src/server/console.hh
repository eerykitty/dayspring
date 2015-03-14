#pragma once

#include <thread>
#include <mutex>
#include <iostream>
#include <string>

#include "server.hh"

namespace console {
        extern std::mutex io_mutex;

        void notify (std::string input);
        void t_notify (std::string thread_name, std::string input);
        void t_error (std::string thread_name, std::string input);
}
