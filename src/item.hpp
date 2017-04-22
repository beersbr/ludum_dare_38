#ifndef _ITEM_HPP
#define _ITEM_HPP

typedef void (*item_callback)(entity_t *player_entity);

typedef struct _item_t {
	string description;
	string name;

	item_callback before_turn;
	item_callback after_turn;
	item_callback before_move;
	item_callback after_move;
	item_callback before_attack;
	item_callback after_attack;
} item_t;

#endif