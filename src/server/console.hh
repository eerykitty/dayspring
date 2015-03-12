#pragma once

#include <thread>
#include <mutex>
#include <iostream>
#include <string>

#include "server.hh"

namespace console {
        extern std::mutex io_mutex;

        void notify (std::string input);
}
