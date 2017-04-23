#ifndef _ROOM_HPP
#define _ROOM_HPP

#define LEVEL_INVALID 	-1

// Any room set to 0 is nothing special
typedef enum TILE_TYPE {
	LEVEL_ENTRANCE = 1,
	LEVEL_EXIT
	// TODO(JP): add more room types?
} TILE_TYPE_T;

typedef struct _tile_t {
	int x, y;
	bool wall_w, wall_a, wall_s, wall_d;

	// TODO(JP): many of these things could probably be replaced by an object
	TILE_TYPE_T type;
} tile_t;

#endif