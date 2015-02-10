#pragma once
#include <utility>
#include <boost/functional/hash.hpp>

#include "types.hh"

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
