#include "map.hh"
#include "item.hh"

namespace map
{
	cell::cell()
	{

	}

	std::list<stoicheon::item*>* cell::retrieve_items (int x, int y)
	{
		return &items[mkcoord (x, y)];
	}
        std::list<stoicheon::entity*>* cell::retrieve_entities (int x, int y)
        {
                return &entities[mkcoord (x, y)];
        }
        std::list<stoicheon::geometry*>* cell::retrieve_geometries (int x, int y)
        {
                return &geometries[mkcoord (x, y)];
        }
	/*void cell::retrieve_entity (int x, int y)
	{
	}
	void cell::retrieve_geometry (int x, int y)
	{
	}*/


	void cell::place (stoicheon::item* to_put, int x, int y)
	{
		items[mkcoord (x, y)].push_front (to_put);
                interior_items.push_front (to_put);
	}
        void cell::place (stoicheon::entity* to_put, int x, int y)
        {
                entities[mkcoord (x, y)].push_front (to_put);
                interior_entities.push_front (to_put);
        }
        void cell::place (stoicheon::geometry* to_put, int x, int y)
        {
                geometries[mkcoord (x, y)].push_front (to_put);
                interior_geometries.push_front (to_put);
        }
}
