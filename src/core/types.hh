#pragma once

struct coordinate
{
	int x;
	int y;
};

coordinate mkcoord (int x, int y);

bool operator == (const coordinate& lhs, const coordinate& rhs);
