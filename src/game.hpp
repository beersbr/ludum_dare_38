#ifndef GAME_HPP
#define GAME_HPP

#include <glm/glm.hpp>

#include "util.hpp"
#include "core.hpp"
#include "controller_manager.hpp"

#include <map>

#define ATTACK_MOVE_DISTANCE 	0.5f

enum STATE_FUNCTION_ID {
    PLAYER_ACTION,
    PLAYER_DEATH_ANIMATION,
    PLAYER_ATTACK_ANIMATION,
    PLAYER_ATTACK_COMPLETE,
    PLAYER_MOVE_ANIMATION,
    LEVEL_CHANGE_CHECK,
    LEVEL_TRANSITION,
    ENEMY_ACTION,
    ENEMY_DEATH_ANIMATION,
    ENEMY_ATTACK_ANIMATION,
    ENEMY_ATTACK_COMPLETE,
    ENEMY_MOVE_ANIMATION,
    NUM_STATES
};

typedef STATE_FUNCTION_ID (*state_update_function)(scene_t *scene, unsigned int ticks);

typedef struct _game_state_t {
    state_update_function update;
} game_state_t;

STATE_FUNCTION_ID player_action(scene_t *scene, unsigned int ticks);
STATE_FUNCTION_ID player_death_animation(scene_t *scene, unsigned int ticks);
STATE_FUNCTION_ID player_attack_animation(scene_t *scene, unsigned int ticks);
STATE_FUNCTION_ID player_attack_complete(scene_t *scene, unsigned int ticks);
STATE_FUNCTION_ID player_move_animation(scene_t *scene, unsigned int ticks);
STATE_FUNCTION_ID level_change_check(scene_t *scene, unsigned int ticks);
STATE_FUNCTION_ID level_transition(scene_t *scene, unsigned int ticks);
STATE_FUNCTION_ID enemy_action(scene_t *scene, unsigned int ticks);
STATE_FUNCTION_ID enemy_death_animation(scene_t *scene, unsigned int ticks);
STATE_FUNCTION_ID enemy_attack_animation(scene_t *scene, unsigned int ticks);
STATE_FUNCTION_ID enemy_attack_complete(scene_t *scene, unsigned int ticks);
STATE_FUNCTION_ID enemy_move_animation(scene_t *scene, unsigned int ticks);

state_update_function get_state(STATE_FUNCTION_ID id);

#endif