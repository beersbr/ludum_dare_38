
#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include "tile.hpp"
#include "item.hpp"

typedef struct _entity_t {
	bool is_tile;
	tile_t *tile;

	bool is_enemy;
	// TODO: add enemy

	bool is_player;
	// TODO: add player

	bool is_item;
	item_t *item;

	bool is_item_holder;
	std::vector<item_t> item_list;

} entity_t;

#endif