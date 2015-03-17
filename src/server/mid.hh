#pragma once

#include <cstdint>

// message id's - shared by client and server.

#define MID(x) (static_cast<uint32_t>(mid::x))

enum class mid : std::uint32_t
{
        op = 1,
        timestamp = 2,
        auth = 3,
        flag = 4,
        msg = 100
};
