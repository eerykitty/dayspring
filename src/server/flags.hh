#pragma once

#include <string>
#include <unordered_map>

class flag_table
{
        private:
                std::unordered_map<std::string, bool> flags;
        public:
                bool is_set (std::string string);
                void set (std::string string);
                void clear (std::string string);
};
