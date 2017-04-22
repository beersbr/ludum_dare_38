#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

#include "level.hpp"

int check_walls( tile_t tile, int width, int height ) {
	int num_walls = 0;

	if ( tile.wall_w ) { 
		num_walls++; 
	}

	if ( tile.wall_a ) { 
		num_walls++; 
	}

	if ( tile.wall_s ) { 
		num_walls++; 
	}

	if ( tile.wall_d ) { 
		num_walls++; 
	}

	// We don't want to close the player out, so reset the grid here
	// Also let them always enter the edge of the map
	if( num_walls > 3 || 
	   		tile.x == width || tile.y == height ||
	   		tile.x == 0 || tile.y == 0 ) {
		tile.wall_w = false;
		tile.wall_a = false;
		tile.wall_s = false;
		tile.wall_d = false;
	}

	return num_walls;
}

void level_t::create_level( int width, int height ) {
	int count = 0;

	srand( time(NULL) );

	grid_width = width;
	grid_height = height;

	grid.resize( width*height );

	// Create entrance on bottom row of grid
	grid[rand() % width].type = level_entrance;

	// Create exit at top row of grid
	grid[rand() % width + (width*height-width)].type = level_exit;


	for( int i=0; i<grid_width*grid_height; i++) {
		grid[i].x = i%grid_width;
		grid[i].y = floor(i/grid_width);

		// Add walls to the level (w, a, s, d -> signifying movement direction)
		// WALL_CREATION_CHANCE% chance of a wall being created
		// TODO(JP): Consider having more contiguous walls???

		if( 0 == rand()%WALL_CREATION_CHANCE ) {
			grid[i].wall_w = true;
		}

		if( 0 == rand()%WALL_CREATION_CHANCE ) {
			grid[i].wall_a = true;
		}

		if( 0 == rand()%WALL_CREATION_CHANCE ) {
			grid[i].wall_s = true;
		}

		if( 0 == rand()%WALL_CREATION_CHANCE ) {
			grid[i].wall_d = true;
		}

		// Make sure the player has a way out of the room
		check_walls( grid[i] , width, height );

		// Ensure the boundaries are protected by walls
		if( 0 == grid[i].x ) {
			grid[i].wall_a = true;
		}
		else if ( width == grid[i].x+1 ) {
			grid[i].wall_d = true;
		}

		if( 0 == grid[i].y ) {
			grid[i].wall_s = true;
		}
		else if( height == grid[i].y+1 ) {
			grid[i].wall_w = true;
		}	
	}

	print_level();


	// TODO(JP): add stuff to make level interesting
}

// Add a wall after the creation of the level
// This will also add a wall to the adjacent tile
void level_t::create_wall( int x, int y, char wall ) {
	int index = grid_width*y + x;
	int adjacent_index;
	tile_t tile = grid[index];

	switch( wall ) {
		case 'w':
			if( grid_height > y+1 ) {
				adjacent_index = grid_width*(y+1) + x;
				grid[adjacent_index].wall_s = true;
			}

			grid[index].wall_w = true;
			std::cout << "adding w" << std::endl;
			break;
		case 's':
			if( 0 <= y-1 ) {
				adjacent_index = grid_width*(y-1) + x;
				grid[adjacent_index].wall_w = true;
			}

			grid[index].wall_s = true;
			std::cout << "adding a" << std::endl;
			break;
		case 'a':
			if( 0 <= x-1 ) {
				adjacent_index = grid_width*y + (x-1);
				grid[adjacent_index].wall_d = true;
			}

			grid[index].wall_a = true;
			std::cout << "adding s" << std::endl;
			break;
		case 'd':
			if( grid_height > x+1 ) {
				adjacent_index = grid_width*y + (x+1);
				grid[adjacent_index].wall_a = true;
			}

			grid[index].wall_d = true;
			std::cout << "adding d" << std::endl;
			break;
		default:
			std::cout << "Invalid wall type" << std::endl;
	}
}


// Query a coordinate on the map to check for events at that location
int level_t::query_location( int location_x, int location_y ) {
	int level_event = 0;
	int index = grid_width*location_y + location_x;

	if ( grid_width <= location_x || location_x < 0 ) {
		level_event = -1;
		std::cout << "invalid location x" << std::endl;
	}
	else if ( grid_height <= location_y || location_y < 0) {
		level_event = -1;
		std::cout << "invalid location y" << std::endl;
	}
	else {
		// TODO(JP): check for other things
		level_event = grid[index].type;
	}

	return level_event;
}


// TODO(JP): we'll need the location of the character for this to be useful at all
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

void level_t::print_level() {

	for( int i=0; i<grid_width*grid_height; i++ ) {
		std::cout << grid[i].type << "		" << 
			grid[i].wall_w << ", " <<
			grid[i].wall_a << ", " << 
			grid[i].wall_s << ", " << 
			grid[i].wall_d << 
			"	x: " << grid[i].x <<
			"	y: " << grid[i].y << 
			std::endl;
	}
}
