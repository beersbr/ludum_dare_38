#ifndef GAME_HPP
#define GAME_HPP

#include <glm/glm.hpp>

#include "util.hpp"
#include "core.hpp"
#include "controller_manager.hpp"


typedef void (*state_update_function)(scene_t *scene, unsigned int ticks);

typedef struct _game_state_t {
    state_update_function update;
} game_state_t;

void update_player(scene_t *scene, unsigned int ticks);

#endif