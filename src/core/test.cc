#include "map.hh"
#include "item.hh"

#include "types.hh"

int main(int argc, char** argv)
{
	map::cell test_cell;
	test_cell.items[mkcoord (1, 1)] = new stoicheon::item;
	return 0;
}
