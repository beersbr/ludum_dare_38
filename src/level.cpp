#include <iostream>
#include <ctime>
#include <cstdlib>

#include "level.hpp"

void level_t::create_level( int width, int height ) {
	int count = 0;

	srand( time(NULL) );

	grid_width = width;
	grid_height = height;

	grid.resize( width*height );

	// Create entrance on bottom row of grid
	grid[rand() % width].type = level_entrance;

	// Create exit at top row of grid
	grid[rand() % width + (width*height-width)]. type = level_exit;


	for( int i=0; i<grid_width*grid_height; i++) {
		std::cout << grid[i].type << std::endl;
	}

	// TODO: add stuff to make level interesting
}


// Query a coordinate on the map to check for events at that location
int level_t::query_location( int location_x, int location_y ) {
	int level_event = 0;
	int index = grid_width*location_y + location_x;

	if ( grid_width< location_x ) {
		level_event = -1;
		std::cout << "invalid location x" << std::endl;
	}
	else if ( grid_height < location_y ) {
		level_event = -1;
		std::cout << "invalid location y" << std::endl;
	}
	else {
		// TODO: check for other things
		level_event = grid[index].type;
	}

	return level_event;
}


// TODO: we'll need the location of the character for this to be useful at all
// Move main character left, right, up, or down on the grid if possible
int level_t::move( Movement move ) {
	int success = 0; 

	// TODO: return something signifying an event at the new location

	switch ( move ) {
		case move_left:
			std::cout << "Move Left" << std::endl;
			break;
		case move_right:
			std::cout << "Move Right" << std::endl;
			break;
		case move_up:
			std::cout << "Move Up" << std::endl;
			break;
		case move_down:
			std::cout << "Move Down" << std::endl;
			break;
		default:
			success = -1;
	}

	return success;
}