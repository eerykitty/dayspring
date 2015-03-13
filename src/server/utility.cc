#include "utility.hh"

#include <sys/types.h>
#include <sys/stat.h>

bool file_exists (char* filename)
{
        struct stat info;
        if (stat (filename, &info) == 0)
                return true;
        else
                return false;
}

bool file_exists (std::string filename)
{
        return file_exists (filename.c_str ());
}
