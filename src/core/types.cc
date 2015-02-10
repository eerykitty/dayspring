/*
 * helper functions, especially for types.hh
 */

#include "types.hh"

coordinate mkcoord (int x, int y)
{
	coordinate to_return;
	to_return.x = x;
	to_return.y = y;
	return to_return;
}

bool operator == (const coordinate& lhs, const coordinate& rhs)
{
	if (lhs.x == rhs.x && lhs.y == rhs.y)
		return true;
	else
		return false;
}
