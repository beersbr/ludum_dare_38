#ifndef _LEVEL_HPP
#define _LEVEL_HPP

#include <vector>

#include "tile.hpp"

#define WALL_CREATION_CHANCE	50  // percent

enum Movement { move_left, move_right, move_up, move_down };

typedef struct _level_t {
	int grid_width;
	int grid_height;
	std::vector<tile_t> grid;

	void create_level( int width, int height );
	int query_location( int location_x, int location_y );
	int move( Movement move );
} level_t;

#endif