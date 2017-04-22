#ifndef _ROOM_HPP
#define _ROOM_HPP

// Any room set to 0 is nothing special
enum Tile_type { level_entrance = 1, level_exit, item, wall /* TODO: add more room types? */  };

typedef struct {
	// TODO: many of these things could probably be replaced by an object
	Tile_type type;
	// Item_type item;
	int item;
} tile_t;

#endif