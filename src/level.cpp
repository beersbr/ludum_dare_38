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

	grid.clear();
	grid.resize( width*height );

	// Create entrance on bottom row of grid
	grid[rand() % width].type = TILE_ENTRANCE;

	// Create exit at top row of grid
	grid[rand() % width + (width*height-width)].type = TILE_EXIT;


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
			grid[i].wall_w = true;
		}
		else if( height == grid[i].y+1 ) {
			grid[i].wall_s = true;
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
			if( 0 <= y-1 ) {
				adjacent_index = grid_width*(y-1) + x;
				grid[adjacent_index].wall_s = true;
			}

			grid[index].wall_w = true;
			std::cout << "adding w" << std::endl;
			break;
		case 's':
			if( grid_height > y+1 ) {
				adjacent_index = grid_width*(y+1) + x;
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
int level_t::query_location( int location_x, int location_y, char move ) {
	int tile_event = 0;
	int index;
	int attempt_index;
	int attempt_x = location_x;
	int attempt_y = location_y;

	index = grid_width*location_y + location_x;
		
	switch ( move ) {
		case 'w':
			if ( grid[index].wall_w ) {
				tile_event = TILE_INVALID;
			} 

			attempt_y -= 1;
			break;
		case 'a':
			if ( grid[index].wall_a ) {
				tile_event = TILE_INVALID;
			} 

			attempt_x -= 1;
			break;
		case 's':
			if ( grid[index].wall_s ) {
				tile_event = TILE_INVALID;
			} 

			attempt_y += 1;
			break;
		case 'd':
			if ( grid[index].wall_d ) {
				tile_event = TILE_INVALID;
			} 

			attempt_x += 1;
			break;
		default:
			break;
	}

	attempt_index = grid_width*attempt_y + attempt_x;

	std::cout << "Trying: " << attempt_x << " " << attempt_y << std::endl;

	if ( 0 == tile_event ) {
		if ( attempt_x >= grid_width || attempt_x < 0 ) {
			tile_event = TILE_INVALID;
		}
		else if ( attempt_y >= grid_height || attempt_y < 0) {
			tile_event = TILE_INVALID;
		}
		else {
			// TODO(JP): check for other things
			tile_event = grid[attempt_index].type;
		}
	}

	return tile_event;
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
