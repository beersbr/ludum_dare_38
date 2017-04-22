#ifndef _ITEM_SWORD_HPP
#define _ITEM_SWORD_HPP

#define SWORD_MODIFIER 4


void sword_deal_damage(entity_t *enemy) {
	enemy->enemy_health -= 1 + SWORD_MODIFIER;
	std::cout << "Damage taken.  Current health: " << enemy->enemy_health << std::endl;
}

// Use this to achieve a "magic" effect on the player or whatever you want
// e.g. player gains +1 hp after using the item or moving with it
void sword_special_effect(entity_t *player) {
	std::cout << "special effect" << std::endl;
	player->player_health += 1;
}

#endif