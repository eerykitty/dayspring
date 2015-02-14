#include "map.hh"
#include "item.hh"

namespace map
{
	cell::cell()
	{
		
	}

	std::shared_ptr<std::list<stoicheon::item*>> cell::retrieve_items (int x, int y)
	{
		if (items[mkcoord (x, y)].use_count () == 0)
		{
			//items[mkcoord (x, y)] = std::make_shared<std::list<stoicheon::item*>>();
		}
		return items[mkcoord (x, y)];
	}
	/*void cell::retrieve_entity (int x, int y)
	{
	}
	void cell::retrieve_geometry (int x, int y)
	{
	}*/


	void cell::place (stoicheon::item* to_put, int x, int y)
	{
		//items.emplace (mkcoord (x, y), to_put);
	}
	/*void cell::place (stoicheon::entity* to_put, int x, int y)
	{
		entities.emplace (mkcoord (x, y), to_put);
	}
	void cell::place (stoicheon::geometry* to_put, int x, int y)
	{
		geometries.emplace (mkcoord (x, y), to_put);
	}*/

}
