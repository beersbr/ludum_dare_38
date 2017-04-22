#ifndef _ROOM_HPP
#define _ROOM_HPP

enum Tile_type { level_entrance, level_exit, item, wall /* TODO: add more room types? */  };

typedef struct {
	// Tile_type type;
	int type;
	// Item_type item;
	int item;
} tile_t;

#endif