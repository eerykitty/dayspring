#include "map.hh"
#include "item.hh"

#include "types.hh"

int main(int argc, char** argv)
{
	map::cell test_cell;
	test_cell.place (new stoicheon::item, 1, 1);

	return 0;
}
