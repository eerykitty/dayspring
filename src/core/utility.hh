#pragma once
#include <utility>
#include <boost/functional/hash.hpp>

namespace std
{
        template <> class hash<std::pair <int, int>>
        {
                public:
                        size_t operator()(const std::pair <int, int> &pair_hash) const
                        {
                                size_t seed = 0;
                                boost::hash_combine (seed, pair_hash.first);
                                boost::hash_combine (seed, pair_hash.second);
                                return seed;
                        }
        }
}
