#ifndef _LEVEL_HPP
#define _LEVEL_HPP

#include <vector>

#include "tile.hpp"

#define WALL_CREATION_CHANCE	50  // percent

typedef struct _level_t {
	int grid_width;
	int grid_height;
	std::vector<tile_t> grid;

	void create_level( int width, int height );
	void create_wall( int x, int y, char wall );
	int query_location( int location_x, int location_y, char move );
	// Debug crap
	void print_level();
} level_t;

#endif