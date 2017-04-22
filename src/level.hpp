#ifndef _LEVEL_HPP
#define _LEVEL_HPP

#include <vector>

#include "room.hpp"

enum Movement { move_left, move_right, move_up, move_down };

typedef struct level {
	std::vector<std::vector<int> > grid;

	void create_level( int width, int height );
	int query_location( int location_x, int location_y );
	int move( Movement move );
} level_t;

#endif