#pragma once
#include <unordered_map>
#include <utility>

#include "types.hh"
#include "item.hh"
#include "entity.hh"
#include "geometry.hh"
#include "utility.hh"

/*
 * cells are used to store information about the game world; they can be tiled "indefinitely" (with
 * infinite memory and computer power heh), and have a default size of 500 by 500 tiles.
 */

namespace map {
	class cell
	{
		public:
			std::unordered_map <coordinate, stoicheon::item*>       items;
			std::unordered_map <coordinate, stoicheon::entity*>     entities;
			std::unordered_map <coordinate, stoicheon::geometry*>   geometries;

			//public:
			cell();
			void retrieve (stoicheon::item*, int x, int y);
			void retrieve (stoicheon::entity*, int x, int y);
			void retrieve (stoicheon::geometry*, int x, int y);
	};
}
