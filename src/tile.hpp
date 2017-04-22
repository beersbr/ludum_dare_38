#ifndef _ROOM_HPP
#define _ROOM_HPP

// Any room set to 0 is nothing special
enum Tile_type {
	level_entrance = 1,
	level_exit
	// TODO(JP): add more room types?
};

typedef struct _tile_t {
	int x, y;
	bool wall_w, wall_a, wall_s, wall_d;

	// TODO(JP): many of these things could probably be replaced by an object
	Tile_type type;
} tile_t;

#endif