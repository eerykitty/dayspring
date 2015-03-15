#include "flags.hh"

bool flag_table::is_set (std::string string)
{
        auto it = flags.find (string);
        if (it == flags.end ())
                return false;
        else if (it->second == true)
                return true;
        else
                return false;
}

void flag_table::set (std::string string)
{
        flags[string] = true;
}

void flag_table::clear (std::string string)
{
        flags[string] = false;
}
