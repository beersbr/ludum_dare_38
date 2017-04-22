#include <iostream>
#include "level.hpp"

void level::create_level( int width, int height ) {
	int count = 0;

	grid.resize( width, std::vector<int>(height) );

	// TODO: add stuff to make level interesting
	for( int i=0; i<width; i++ ) {
		for( int j=0; j<height; j++)  {
			grid[i][j] = count;
			count++;
		}
	}
}

// Query a coordinate on the map to check for events at that location
int level::query_location( int location_x, int location_y ) {
	int level_event = 0;

	if ( grid.size() < location_x ) {
		level_event = -1;
		std::cout << "invalid location x" << std::endl;
	}
	else if ( grid[0].size() < location_y ) {
		level_event = -1;
		std::cout << "invalid location y" << std::endl;
	}
	else {
		// TODO: check for other things
		level_event = grid[location_x][location_y];
	}

	return level_event;
}

// TODO: we'll need the location of the character for this to be useful at all
// Move main character left, right, up, or down on the grid if possible
int level::move( Movement move ) {
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