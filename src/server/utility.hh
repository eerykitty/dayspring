#pragma once

#include <utility>
#include <string>
#include <boost/functional/hash.hpp>

#include "types.hh"

bool file_exists (char* filename);
bool file_exists (std::string filename);

namespace std
{
	template <> class hash<coordinate>
	{
		public:
			size_t operator() (const coordinate &pair_hash) const
			{
				size_t seed = 0;
				boost::hash_combine (seed, pair_hash.x);
				boost::hash_combine (seed, pair_hash.y);
				return seed;
			}

	};
}
