#include "auth.hh"

// yeah, this does no verification whatsoever.
// ...but actually works for a game, come to think of it! O_o
uint64_t auth::connect (std::string username, std::string password)
{
        std::hash<std::string> string_hash;
        return (uint64_t)string_hash (username + password);
}
