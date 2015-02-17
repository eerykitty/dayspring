#pragma once
#include <unordered_map>
#include <utility>
#include <list>
#include <memory>
#include <functional>

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
		private:
			std::unordered_map <coordinate, std::shared_ptr<std::list<stoicheon::item*>>>       items;
			/*std::unordered_multimap <coordinate, stoicheon::entity*>     entities;
			std::unordered_multimap <coordinate, stoicheon::geometry*>   geometries;*/

			std::list <stoicheon::item*> interior_items;
			/*std::list <stoicheon::entity*> interior_entities;
			std::list <stoicheon::geometry*> interior_geometries;*/

		public:
			cell();

			std::shared_ptr<std::list<stoicheon::item*>> retrieve_items (int x, int y);
			/*void retrieve (int x, int y);
			void retrieve (int x, int y);*/

			void place	(stoicheon::item* to_put,	int x, int y);
			/*void place	(stoicheon::entity* to_put,	int x, int y);
			void place	(stoicheon::geometry* to_put,	int x, int y);*/
	};
}
